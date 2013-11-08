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

#include "refinement-common.hpp"




template <typename MeshType>
int test(MeshType & mesh_in)
{
  typedef typename viennagrid::result_of::line<MeshType>::type       EdgeType;
  typedef typename viennagrid::result_of::cell<MeshType>::type      CellType;

  typedef typename viennagrid::result_of::line_range<CellType>::type    EdgeOnCellContainer;
  typedef typename viennagrid::result_of::iterator<EdgeOnCellContainer>::type     EdgeOnCellIterator;

  typedef typename viennagrid::result_of::cell_range<MeshType>::type  CellContainer;
  typedef typename viennagrid::result_of::iterator<CellContainer>::type         CellIterator;

  CellContainer cells(mesh_in);
  CellIterator cit = cells.begin();
  CellType & cell = *cit; ++cit;
//   CellType & cell2 = *cit;

  EdgeOnCellContainer edges(cell);

  std::cout << "Volume of reference tetrahedron: " << volume(cell) << std::endl;

  //
  // Tag edges systematically for refinement:
  //

  std::cout << "Cell under test: " << std::endl;
  std::cout << *cit << std::endl;

  //1 edge:
  std::cout << "Testing refinement with one tagged edge: ";
  for (EdgeOnCellIterator eocit1 = edges.begin();
                          eocit1 != edges.end();
                        ++eocit1)
  {
    std::vector<bool> edge_refinement_tag_container(edges.size());
    typename viennagrid::result_of::field<std::vector<bool>, EdgeType>::type edge_refinement_tag_field(edge_refinement_tag_container);
    edge_refinement_tag_field(*eocit1) = true;

    MeshType mesh_refined;
    viennagrid::refine<CellType>( mesh_in, mesh_refined, edge_refinement_tag_field );

    if (sanity_check(mesh_in, mesh_refined) == EXIT_FAILURE)
    {
      print_refinement_edges(cell, edge_refinement_tag_field);
      return EXIT_FAILURE;
    }
  }
  std::cout << "[PASSED]" << std::endl;


  //2 edges:
  std::cout << "Testing refinement with two tagged edges: ";
  for (EdgeOnCellIterator eocit1 = edges.begin();
                          eocit1 != edges.end();
                        ++eocit1)
  {
    for (EdgeOnCellIterator eocit2 = edges.begin();
                            eocit2 != edges.end();
                          ++eocit2)
    {
      std::vector<bool> edge_refinement_tag_container(edges.size());
      typename viennagrid::result_of::field<std::vector<bool>, EdgeType>::type edge_refinement_tag_field(edge_refinement_tag_container);
      edge_refinement_tag_field(*eocit1) = true;
      edge_refinement_tag_field(*eocit2) = true;

      MeshType mesh_refined;
      viennagrid::refine<CellType>( mesh_in, mesh_refined, edge_refinement_tag_field );

      if (sanity_check(mesh_in, mesh_refined) == EXIT_FAILURE)
      {
        print_refinement_edges(cell, edge_refinement_tag_field);
        return EXIT_FAILURE;
      }
    }
  }
  std::cout << "[PASSED]" << std::endl;

  //3 edges:
  std::cout << "Testing refinement with three tagged edges: ";
  for (EdgeOnCellIterator eocit1 = edges.begin();
                          eocit1 != edges.end();
                        ++eocit1)
  {
    for (EdgeOnCellIterator eocit2 = edges.begin();
                            eocit2 != edges.end();
                          ++eocit2)
    {
      for (EdgeOnCellIterator eocit3 = edges.begin();
                              eocit3 != edges.end();
                            ++eocit3)
      {
        std::vector<bool> edge_refinement_tag_container(edges.size());
        typename viennagrid::result_of::field<std::vector<bool>, EdgeType>::type edge_refinement_tag_field(edge_refinement_tag_container);
        edge_refinement_tag_field(*eocit1) = true;
        edge_refinement_tag_field(*eocit2) = true;
        edge_refinement_tag_field(*eocit3) = true;

        MeshType mesh_refined;
        viennagrid::refine<CellType>( mesh_in, mesh_refined, edge_refinement_tag_field );

        if (sanity_check(mesh_in, mesh_refined) == EXIT_FAILURE)
        {
          print_refinement_edges(cell, edge_refinement_tag_field);
          return EXIT_FAILURE;
        }
      }
    }
  }
  std::cout << "[PASSED]" << std::endl;

  //4 edges:
  std::cout << "Testing refinement with four tagged edges: ";
  for (EdgeOnCellIterator eocit1 = edges.begin();
                          eocit1 != edges.end();
                        ++eocit1)
  {
    for (EdgeOnCellIterator eocit2 = edges.begin();
                            eocit2 != edges.end();
                          ++eocit2)
    {
      for (EdgeOnCellIterator eocit3 = edges.begin();
                              eocit3 != edges.end();
                            ++eocit3)
      {
        for (EdgeOnCellIterator eocit4 = edges.begin();
                                eocit4 != edges.end();
                              ++eocit4)
        {
          std::vector<bool> edge_refinement_tag_container(edges.size());
          typename viennagrid::result_of::field<std::vector<bool>, EdgeType>::type edge_refinement_tag_field(edge_refinement_tag_container);
          edge_refinement_tag_field(*eocit1) = true;
          edge_refinement_tag_field(*eocit2) = true;
          edge_refinement_tag_field(*eocit3) = true;
          edge_refinement_tag_field(*eocit4) = true;

          MeshType mesh_refined;
          viennagrid::refine<CellType>( mesh_in, mesh_refined, edge_refinement_tag_field );

          if (sanity_check(mesh_in, mesh_refined) == EXIT_FAILURE)
          {
            print_refinement_edges(cell, edge_refinement_tag_field);
            return EXIT_FAILURE;
          }
        }
      }
    }
  }
  std::cout << "[PASSED]" << std::endl;

  //5 edges:
  std::cout << "Testing refinement with five tagged edges: ";
  for (EdgeOnCellIterator eocit1 = edges.begin();
                          eocit1 != edges.end();
                        ++eocit1)
  {
    for (EdgeOnCellIterator eocit2 = edges.begin();
                            eocit2 != edges.end();
                          ++eocit2)
    {
      for (EdgeOnCellIterator eocit3 = edges.begin();
                              eocit3 != edges.end();
                            ++eocit3)
      {
        for (EdgeOnCellIterator eocit4 = edges.begin();
                                eocit4 != edges.end();
                              ++eocit4)
        {
          for (EdgeOnCellIterator eocit5 = edges.begin();
                                  eocit5 != edges.end();
                                ++eocit5)
          {
            std::vector<bool> edge_refinement_tag_container(edges.size());
            typename viennagrid::result_of::field<std::vector<bool>, EdgeType>::type edge_refinement_tag_field(edge_refinement_tag_container);
            edge_refinement_tag_field(*eocit1) = true;
            edge_refinement_tag_field(*eocit2) = true;
            edge_refinement_tag_field(*eocit3) = true;
            edge_refinement_tag_field(*eocit4) = true;
            edge_refinement_tag_field(*eocit5) = true;

            MeshType mesh_refined;
            viennagrid::refine<CellType>( mesh_in, mesh_refined, edge_refinement_tag_field );

            if (sanity_check(mesh_in, mesh_refined) == EXIT_FAILURE)
            {
              print_refinement_edges(cell, edge_refinement_tag_field);
              return EXIT_FAILURE;
            }
          }
        }
      }
    }
  }
  std::cout << "[PASSED]" << std::endl;

  //6 edges:
  std::vector<bool> edge_refinement_tag_container(edges.size());
  typename viennagrid::result_of::field<std::vector<bool>, EdgeType>::type edge_refinement_tag_field(edge_refinement_tag_container);

  std::cout << "Testing refinement with six tagged edges: ";
  for (EdgeOnCellIterator eocit1 = edges.begin();
                          eocit1 != edges.end();
                        ++eocit1)
  {
    edge_refinement_tag_field(*eocit1) = true;
  }

  {
    MeshType mesh_refined;
    viennagrid::refine<CellType>( mesh_in, mesh_refined, edge_refinement_tag_field );

    if (sanity_check(mesh_in, mesh_refined) == EXIT_FAILURE)
    {
      print_refinement_edges(cell, edge_refinement_tag_field);
      return EXIT_FAILURE;
    }
  }

  std::cout << "[PASSED]" << std::endl;

  return EXIT_SUCCESS;
}


template <unsigned int A>
struct cell_vertex_permutator
{
  template <typename PointType>
  static void apply(std::vector<PointType> & points); //no default implementation
};

template <>
struct cell_vertex_permutator<1>
{
  template <typename VertexHandleInContainerT, typename VertexHandleOutContainerT>
  static void apply(VertexHandleInContainerT const & vertices_in, VertexHandleOutContainerT & vertices_out)
  {
    vertices_out[0] = vertices_in[0];
    vertices_out[1] = vertices_in[1];
    vertices_out[2] = vertices_in[2];
    vertices_out[3] = vertices_in[3];
  }
};

template <>
struct cell_vertex_permutator<2>
{
  template <typename VertexHandleInContainerT, typename VertexHandleOutContainerT>
  static void apply(VertexHandleInContainerT const & vertices_in, VertexHandleOutContainerT & vertices_out)
  {
    vertices_out[0] = vertices_in[1];
    vertices_out[1] = vertices_in[2];
    vertices_out[2] = vertices_in[0];
    vertices_out[3] = vertices_in[3];
  }
};

template <>
struct cell_vertex_permutator<3>
{
  template <typename VertexHandleInContainerT, typename VertexHandleOutContainerT>
  static void apply(VertexHandleInContainerT const & vertices_in, VertexHandleOutContainerT & vertices_out)
  {
    vertices_out[0] = vertices_in[2];
    vertices_out[1] = vertices_in[0];
    vertices_out[2] = vertices_in[1];
    vertices_out[3] = vertices_in[3];
  }
};


template <typename MeshType, typename CellPermutatorA, typename CellPermutatorB>
void fill_mesh(MeshType & mesh,
                 CellPermutatorA const &,
                 CellPermutatorB const &)
{
  typedef typename viennagrid::result_of::point<MeshType>::type          PointType;
  typedef typename viennagrid::result_of::vertex_handle<MeshType>::type       VertexHandleType;

  std::vector<PointType> points(5);
  PointType & p0 = points[0];
  PointType & p1 = points[1];
  PointType & p2 = points[2];
  PointType & p3 = points[3];
  PointType & p4 = points[4];

  p0[0] = 0.0; p0[1] = 0.0; p0[2] = 0.0;
  p1[0] = 1.0; p1[1] = 0.0; p1[2] = 0.0;
  p2[0] = 0.0; p2[1] = 1.0; p2[2] = 0.0;
  p3[0] = 0.0; p3[1] = 0.0; p3[2] = 1.0;
  p4[0] = 1.0; p4[1] = 1.0; p4[2] = 1.0;

  //upgrade to vertex:
  VertexHandleType vh[5];

  for (int i = 0; i < 5; ++i)
    vh[i] = viennagrid::make_vertex( mesh, points[i] );


  {
    VertexHandleType cur_vh[4];
    cur_vh[0] = vh[0];
    cur_vh[1] = vh[1];
    cur_vh[2] = vh[2];
    cur_vh[3] = vh[3];

    VertexHandleType permutated_vh[4];
    CellPermutatorA::apply(cur_vh, permutated_vh);

    viennagrid::make_cell( mesh, permutated_vh, permutated_vh+4 );
  }

  {
    VertexHandleType cur_vh[4];
    cur_vh[0] = vh[1];
    cur_vh[1] = vh[2];
    cur_vh[2] = vh[3];
    cur_vh[3] = vh[4];

    VertexHandleType permutated_vh[4];
    CellPermutatorB::apply(cur_vh, permutated_vh);

    viennagrid::make_cell( mesh, permutated_vh, permutated_vh+4 );
  }
}


template <unsigned int A>
struct mesh_tester
{
  static int apply()
  {
    typedef viennagrid::tetrahedral_3d_mesh    MeshType;

    std::cout << std::endl;
    std::cout << "Testing mesh " << A << "..." << std::endl;

    {
      MeshType mesh;
      fill_mesh(mesh, cell_vertex_permutator<A>(), cell_vertex_permutator<1>());
      if (test(mesh) == EXIT_FAILURE)
      {
        std::cerr << "FAILED!" << std::endl;
        return EXIT_FAILURE;
      }
    }
    {
      MeshType mesh;
      fill_mesh(mesh, cell_vertex_permutator<A>(), cell_vertex_permutator<2>());
      if (test(mesh) == EXIT_FAILURE)
      {
        std::cerr << "FAILED!" << std::endl;
        return EXIT_FAILURE;
      }
    }
    {
      MeshType mesh;
      fill_mesh(mesh, cell_vertex_permutator<A>(), cell_vertex_permutator<3>());
      if (test(mesh) == EXIT_FAILURE)
      {
        std::cerr << "FAILED!" << std::endl;
        return EXIT_FAILURE;
      }
    }

    return mesh_tester<A-1>::apply();
  }
};

//terminate recursion:
template <>
struct mesh_tester<0>
{
  static int apply() { return EXIT_SUCCESS; }
};

int main()
{

  std::cout << "*****************" << std::endl;
  std::cout << "* Test started! *" << std::endl;
  std::cout << "*****************" << std::endl;


  if (mesh_tester<3>::apply() == EXIT_SUCCESS)
  {
    std::cout << "SUCCESS!" << std::endl;
  }
  else
    return EXIT_FAILURE;

  std::cout << "*******************************" << std::endl;
  std::cout << "* Test finished successfully! *" << std::endl;
  std::cout << "*******************************" << std::endl;


  return EXIT_SUCCESS;
}
