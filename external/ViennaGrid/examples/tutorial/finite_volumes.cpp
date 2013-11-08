/* =======================================================================
   Copyright (c) 2011-2013, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                     ViennaGrid - The Vienna Grid Library
                            -----------------

   License:      MIT (X11), see file LICENSE in the base directory
======================================================================= */

#ifdef _MSC_VER
  #pragma warning( disable : 4503 )     //truncated name decoration
#endif

#include <iostream>
#include <fstream>
#include <vector>

#include "viennagrid/config/default_configs.hpp"

#include "viennagrid/algorithm/volume.hpp"
#include "viennagrid/algorithm/voronoi.hpp"
#include "viennagrid/io/netgen_reader.hpp"

#include "viennagrid/mesh/coboundary_iteration.hpp"

//
// A very simple sparse matrix class
//
template <typename T>
class sparse_matrix
{
public:
  sparse_matrix(std::size_t rows = 1, std::size_t cols = 1) : rows_(rows), cols_(cols) {}

  T & operator()(std::size_t i, std::size_t j) { return entries_[i][j]; }
  T const & operator()(std::size_t i, std::size_t j) const { return entries_[i][j]; }

  void resize(std::size_t rows, std::size_t cols)
  {
    rows_ = rows;
    cols_ = cols;
    entries_.resize(rows);
  }

private:
  std::size_t rows_;
  std::size_t cols_;
  std::vector<std::map<std::size_t, T> >  entries_;
};

//
// The assembly routine for Poisson equation:  div( grad(psi) ) = 1
//
template <typename MeshType, typename MatrixType, typename VectorType>
void assemble(MeshType & mesh,
              MatrixType & system_matrix,
              VectorType & load_vector)
{
  typedef typename viennagrid::result_of::cell<MeshType>::type CellType;

  typedef typename viennagrid::result_of::vertex<MeshType>::type                         VertexType;
  typedef typename viennagrid::result_of::line<MeshType>::type                         EdgeType;

  typedef typename viennagrid::result_of::vertex_range<MeshType>::type     VertexContainer;
  typedef typename viennagrid::result_of::iterator<VertexContainer>::type          VertexIterator;

  typedef typename viennagrid::result_of::coboundary_range<MeshType, viennagrid::vertex_tag, viennagrid::line_tag>::type EdgeOnVertexContainer;
  typedef typename viennagrid::result_of::iterator<EdgeOnVertexContainer>::type    EdgeOnVertexIterator;

  std::size_t current_dof = 0;

  //
  // Compute Voronoi info
  //
  typedef typename viennagrid::result_of::const_cell_handle<MeshType>::type    ConstCellHandleType;

  std::deque<double> interface_areas;
  std::deque< typename viennagrid::result_of::voronoi_cell_contribution<ConstCellHandleType>::type > interface_contributions;

  std::deque<double> vertex_box_volumes;
  std::deque< typename viennagrid::result_of::voronoi_cell_contribution<ConstCellHandleType>::type > vertex_box_volume_contributions;

  std::deque<double> edge_box_volumes;
  std::deque< typename viennagrid::result_of::voronoi_cell_contribution<ConstCellHandleType>::type > edge_box_volume_contributions;


  // Write Voronoi info to default ViennaData keys:
  viennagrid::apply_voronoi<CellType>(
          mesh,
          viennagrid::make_accessor<EdgeType>(interface_areas),
          viennagrid::make_accessor<EdgeType>(interface_contributions),
          viennagrid::make_accessor<VertexType>(vertex_box_volumes),
          viennagrid::make_accessor<VertexType>(vertex_box_volume_contributions),
          viennagrid::make_accessor<EdgeType>(edge_box_volumes),
          viennagrid::make_accessor<EdgeType>(edge_box_volume_contributions)
  );

  typename viennagrid::result_of::accessor< std::deque<double>, EdgeType >::type interface_area_accessor( interface_areas );
  typename viennagrid::result_of::accessor< std::deque<double>, EdgeType >::type edge_box_volume_accessor( edge_box_volumes );


  std::deque<long> dof_container;
  typename viennagrid::result_of::accessor< std::deque<long>, VertexType >::type dof_accessor( dof_container );

  //
  // Iterate over all vertices in the mesh and enumerate degrees of freedom (aka. unknown indices)
  //
  VertexContainer vertices = viennagrid::elements<viennagrid::vertex_tag>(mesh);
  for (VertexIterator vit = vertices.begin();
      vit != vertices.end();
      ++vit)
  {
    //boundary condition: Assuming homogeneous Dirichlet boundary conditions at x=0 and x=1
    //if ( (vit->point()[0] == 0) || (vit->point()[0] == 1) )
    if ( (viennagrid::point(mesh, *vit)[0] == 0) || (viennagrid::point(mesh, *vit)[0] == 1) )
      dof_accessor(*vit) = -1;
    else
    {
      dof_accessor(*vit) = current_dof;
      ++current_dof;
    }
  }

  std::cout << "Assigned degrees of freedom: " << current_dof << std::endl;

  //resize global system matrix and load vector to the number of unknowns:
  system_matrix.resize(current_dof, current_dof);
  load_vector.resize(current_dof);


  //
  // Poisson equation assembly:  div( grad(psi) ) = 1
  //
  for (VertexIterator vhit = vertices.begin();
        vhit != vertices.end();
        ++vhit)
  {
    VertexType & vertex = *vhit;
    long row_index = dof_accessor(vertex);

    //std::cout << vertex << " " << row_index << std::endl;

    if (row_index < 0) //this is a Dirichlet boundary condition
      continue;

    //EdgeOnVertexContainer edges = viennagrid::ncells<1>(*vit, mesh);
      EdgeOnVertexContainer edges = viennagrid::coboundary_elements<viennagrid::vertex_tag, viennagrid::line_tag>(mesh, vhit.handle());
    for (EdgeOnVertexIterator eovit = edges.begin();
          eovit != edges.end();
          ++eovit)
    {
      VertexType const * other_vertex_ptr = &(viennagrid::elements<viennagrid::vertex_tag>(*eovit)[0]);
      if (other_vertex_ptr == &(vertex)) //one of the two vertices of the edge is different from *vit
        other_vertex_ptr = &(viennagrid::elements<viennagrid::vertex_tag>(*eovit)[1]);

      long col_index        = dof_accessor(*other_vertex_ptr);
      double edge_len       = viennagrid::volume(*eovit);
      double interface_area = interface_area_accessor(*eovit);

      //std::cout << "  " << *other_vertex_ptr << std::endl;
      //std::cout << "  " << col_index << " " << edge_len << " " << interface_area << std::endl;

      if (col_index >= 0)
        system_matrix(row_index, col_index) = - interface_area / edge_len;

      system_matrix(row_index, row_index) += interface_area / edge_len;

      //std::cout << "  " << system_matrix(row_index, col_index) << " " << system_matrix(row_index, row_index) << std::endl;
      //std::cout << std::endl;

      //Note: volume stored on edges consists of volumes of both adjacent boxes.
      load_vector[row_index] += edge_box_volume_accessor(*eovit) / 2.0;
    } //for edges
  } //for vertices

} //assemble()


int main()
{
  typedef viennagrid::tetrahedral_3d_mesh MeshType;
  typedef viennagrid::tetrahedral_3d_segmentation SegmentationType;

//   typedef viennagrid::triangular_2d_mesh                      MeshType;        //use this for a 2d example
//   typedef viennagrid::triangular_2d_segmentation                SegmentationType;  //use this for a 2d example

  std::cout << "----------------------------------------------------------" << std::endl;
  std::cout << "-- ViennaGrid tutorial: Finite Volumes using ViennaGrid --" << std::endl;
  std::cout << "----------------------------------------------------------" << std::endl;
  std::cout << std::endl;

  MeshType mesh;
  SegmentationType segmentation(mesh);

  viennagrid::io::netgen_reader reader;
  #ifdef _MSC_VER      //Visual Studio builds in a subfolder
  std::string path = "../../examples/data/";
  #else
  std::string path = "../../examples/data/";
  #endif
  reader(mesh, segmentation, path + "cube48.mesh"); //use this for a 3d example
//   reader(mesh, segmentation, path + "square32.mesh"); //use this for a 2d example

  sparse_matrix<double> system_matrix;
  std::vector<double> load_vector;

  assemble(mesh, system_matrix, load_vector);

  //
  // Next step: Solve here using a linear algebra library, e.g. ViennaCL. (not included in this tutorial to avoid external dependencies)
  // Instead, we print the matrix
  std::cout << std::endl << "System matrix: " << std::endl;
  for (std::size_t i=0; i<load_vector.size(); ++i)
  {
    std::cout << "Row " << i << ": ";
    for (std::size_t j=0; j<load_vector.size(); ++j)
    {
      if (system_matrix(i,j) != 0)
        std::cout << "(" << j << ", " << system_matrix(i,j) << "), ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << "Load vector: " << std::endl;
  for (std::size_t i=0; i<load_vector.size(); ++i)
    std::cout << load_vector[i] << " " << std::endl;
  std::cout << std::endl;

  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << " \\o/    Tutorial finished successfully!    \\o/ " << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;

  return EXIT_SUCCESS;
}


