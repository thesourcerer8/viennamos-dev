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

#include "viennagrid/algorithm/boundary.hpp"
#include "viennagrid/algorithm/centroid.hpp"
#include "viennagrid/config/default_configs.hpp"
#include "viennagrid/io/vtk_reader.hpp"
#include "viennagrid/io/vtk_writer.hpp"
#include "viennagrid/io/opendx_writer.hpp"
#include "viennagrid/io/netgen_reader.hpp"

void print(std::vector<std::string> const & vec)
{
  for (std::size_t i=0; i<vec.size(); ++i)
    std::cout << vec[i] << std::endl;
}

template <typename MeshType, typename ReaderType>
void test(ReaderType & my_reader, std::string const & infile, std::string const & outfile)
{

  typedef typename viennagrid::result_of::segmentation<MeshType>::type          SegmentationType;

  typedef typename viennagrid::result_of::vertex<MeshType>::type                VertexType;
  typedef typename viennagrid::result_of::cell<MeshType>::type                  CellType;

  typedef typename viennagrid::result_of::vertex_range<MeshType>::type          VertexContainer;
  typedef typename viennagrid::result_of::iterator<VertexContainer>::type         VertexIterator;

  typedef typename viennagrid::result_of::cell_range<MeshType>::type            CellRange;
  typedef typename viennagrid::result_of::iterator<CellRange>::type               CellIterator;

  MeshType mesh;
  SegmentationType segmentation(mesh);

  try
  {
    my_reader(mesh, segmentation, infile);
  }
  catch (std::exception const & ex)
  {
     std::cerr << ex.what() << std::endl;
     std::cerr << "File-Reader failed. Aborting program..." << std::endl;
     exit(EXIT_FAILURE);
  }


  std::vector<double> vtk_vertex_double_data;
  std::vector<double> vtk_vertex_long_data;
  std::vector< std::vector<double> > vtk_vertex_vector_data;

  typename viennagrid::result_of::accessor< std::vector<double>, VertexType >::type vtk_vertex_double_accessor( vtk_vertex_double_data );
  typename viennagrid::result_of::accessor< std::vector<double>, VertexType >::type vtk_vertex_long_accessor( vtk_vertex_long_data );
  typename viennagrid::result_of::accessor< std::vector< std::vector<double> >, VertexType >::type vtk_vertex_vector_accessor( vtk_vertex_vector_data );


  //write some dummy data:
  VertexContainer vertices(mesh);
  for (VertexIterator vit = vertices.begin();
      vit != vertices.end();
      ++vit)
  {
    vtk_vertex_double_accessor(*vit) = viennagrid::point(*vit)[0];
    vtk_vertex_long_accessor(*vit) = vit->id().get();

    vtk_vertex_vector_accessor(*vit).resize(3);
    vtk_vertex_vector_accessor(*vit)[0] = viennagrid::point(*vit)[0];
    vtk_vertex_vector_accessor(*vit)[1] = viennagrid::point(*vit)[1];
  }



  std::vector<double> vtk_cell_double_data;
  std::vector<double> vtk_cell_long_data;
  std::vector< std::vector<double> > vtk_cell_vector_data;

  typename viennagrid::result_of::accessor< std::vector<double>, CellType >::type vtk_cell_double_accessor( vtk_cell_double_data );
  typename viennagrid::result_of::accessor< std::vector<double>, CellType >::type vtk_cell_long_accessor( vtk_cell_long_data );
  typename viennagrid::result_of::accessor< std::vector< std::vector<double> >, CellType >::type vtk_cell_vector_accessor( vtk_cell_vector_data );


  int index = 0;
  CellRange cells(mesh);
  for (CellIterator cit = cells.begin();
                    cit != cells.end();
                   ++cit, ++index)
  {
    vtk_cell_double_accessor(*cit) = viennagrid::centroid(*cit)[0];
    vtk_cell_long_accessor(*cit) = cit->id().get();

    vtk_cell_vector_accessor(*cit).resize(3);
    vtk_cell_vector_accessor(*cit)[0] = viennagrid::centroid(*cit)[0];
    vtk_cell_vector_accessor(*cit)[1] = viennagrid::centroid(*cit)[1];
  }

  //test writers:
  viennagrid::io::vtk_writer<MeshType> my_vtk_writer;
  viennagrid::io::add_scalar_data_on_vertices(my_vtk_writer, vtk_vertex_double_accessor, "data_double");
  viennagrid::io::add_scalar_data_on_vertices(my_vtk_writer, vtk_vertex_long_accessor, "data_long");
  viennagrid::io::add_vector_data_on_vertices(my_vtk_writer, vtk_vertex_vector_accessor, "data_point");

  viennagrid::io::add_scalar_data_on_cells(my_vtk_writer, vtk_cell_double_accessor, "data_double");
  viennagrid::io::add_scalar_data_on_cells(my_vtk_writer, vtk_cell_long_accessor, "data_long");
  viennagrid::io::add_vector_data_on_cells(my_vtk_writer, vtk_cell_vector_accessor, "data_point");

  my_vtk_writer(mesh, segmentation, outfile);

  viennagrid::io::opendx_writer<MeshType> my_dx_writer;
  my_dx_writer(mesh, outfile + ".odx");
}




template <typename MeshType, typename ReaderType>
void test_vtk(ReaderType & my_reader, std::string const & infile, std::string const & outfile)
{

  typedef typename viennagrid::result_of::segmentation<MeshType>::type          SegmentationType;

  typedef typename viennagrid::result_of::vertex<MeshType>::type                VertexType;
  typedef typename viennagrid::result_of::cell<MeshType>::type                  CellType;

  typedef typename viennagrid::result_of::vertex_range<MeshType>::type          VertexContainer;
  typedef typename viennagrid::result_of::iterator<VertexContainer>::type         VertexIterator;

  typedef typename viennagrid::result_of::cell_range<MeshType>::type            CellRange;
  typedef typename viennagrid::result_of::iterator<CellRange>::type               CellIterator;

  MeshType mesh;
  SegmentationType segmentation(mesh);



  std::vector<double> vtk_vertex_double_data;
  std::vector<double> vtk_vertex_long_data;
  std::vector< std::vector<double> > vtk_vertex_vector_data;

  typename viennagrid::result_of::accessor< std::vector<double>, VertexType >::type vtk_vertex_double_accessor( vtk_vertex_double_data );
  typename viennagrid::result_of::accessor< std::vector<double>, VertexType >::type vtk_vertex_long_accessor( vtk_vertex_long_data );
  typename viennagrid::result_of::accessor< std::vector< std::vector<double> >, VertexType >::type vtk_vertex_vector_accessor( vtk_vertex_vector_data );

  std::vector<double> vtk_cell_double_data;
  std::vector<double> vtk_cell_long_data;
  std::vector< std::vector<double> > vtk_cell_vector_data;

  typename viennagrid::result_of::accessor< std::vector<double>, CellType >::type vtk_cell_double_accessor( vtk_cell_double_data );
  typename viennagrid::result_of::accessor< std::vector<double>, CellType >::type vtk_cell_long_accessor( vtk_cell_long_data );
  typename viennagrid::result_of::accessor< std::vector< std::vector<double> >, CellType >::type vtk_cell_vector_accessor( vtk_cell_vector_data );



  viennagrid::io::add_scalar_data_on_vertices(my_reader, vtk_vertex_double_accessor, "data_double" );
  viennagrid::io::add_scalar_data_on_vertices(my_reader, vtk_vertex_long_accessor, "data_long" );
  viennagrid::io::add_vector_data_on_vertices(my_reader, vtk_vertex_vector_accessor, "data_point" );

  viennagrid::io::add_scalar_data_on_cells(my_reader, vtk_cell_double_accessor, "data_double");
  viennagrid::io::add_scalar_data_on_cells(my_reader, vtk_cell_long_accessor, "data_long");
  viennagrid::io::add_vector_data_on_cells(my_reader, vtk_cell_vector_accessor, "data_point");


  try
  {
    my_reader(mesh, segmentation, infile);
  }
  catch (std::exception const & ex)
  {
     std::cerr << ex.what() << std::endl;
     std::cerr << "File-Reader failed. Aborting program..." << std::endl;
     exit(EXIT_FAILURE);
  }


  //write some dummy data:
  VertexContainer vertices(mesh);
  for (VertexIterator vit = vertices.begin();
      vit != vertices.end();
      ++vit)
  {
    double data_double = vtk_vertex_double_accessor(*vit);
    long data_long = static_cast<long>(vtk_vertex_long_accessor(*vit));
    std::vector<double> data_point = vtk_vertex_vector_accessor(*vit);

    assert( fabs(data_double - viennagrid::point(*vit)[0]) < 1e-4 && "Vertex check failed: data_double!");
    assert( (data_long == vit->id().get()) && "Vertex check failed: data_long!");
    assert( fabs(data_point[0] - viennagrid::point(*vit)[0]) < 1e-4
            && fabs(data_point[1] - viennagrid::point(*vit)[1]) < 1e-4
            && "Vertex check failed: data_point!");
  }

  CellRange cells(mesh);
  for (CellIterator cit = cells.begin();
                    cit != cells.end();
                   ++cit)
  {
    double data_double = vtk_cell_double_accessor(*cit);
    long data_long = static_cast<long>(vtk_cell_long_accessor(*cit));
    std::vector<double> data_point = vtk_cell_vector_accessor(*cit);

    assert( fabs(data_double - viennagrid::centroid(*cit)[0]) < 1e-4 && "Cell check failed: data_double!");
    assert( (data_long == cit->id().get()) && "Cell check failed: data_long!");
    assert( fabs(data_point[0] - viennagrid::centroid(*cit)[0]) < 1e-4
            && fabs(data_point[1] - viennagrid::centroid(*cit)[1]) < 1e-4
            && "Cell check failed: data_point!");
  }

  //test writers:
  viennagrid::io::vtk_writer<MeshType> my_vtk_writer;
  viennagrid::io::add_scalar_data_on_vertices(my_vtk_writer, vtk_vertex_double_accessor, "data_double");
  viennagrid::io::add_scalar_data_on_vertices(my_vtk_writer, vtk_vertex_long_accessor, "data_long");
  viennagrid::io::add_vector_data_on_vertices(my_vtk_writer, vtk_vertex_vector_accessor, "data_point");

  viennagrid::io::add_scalar_data_on_cells(my_vtk_writer, vtk_cell_double_accessor, "data_double");
  viennagrid::io::add_scalar_data_on_cells(my_vtk_writer, vtk_cell_long_accessor, "data_long");
  viennagrid::io::add_vector_data_on_cells(my_vtk_writer, vtk_cell_vector_accessor, "data_point");

  my_vtk_writer(mesh, segmentation, outfile);

  viennagrid::io::opendx_writer<MeshType> my_dx_writer;
  my_dx_writer(mesh, outfile + ".odx");
}






int main()
{
  std::cout << "*****************" << std::endl;
  std::cout << "* Test started! *" << std::endl;
  std::cout << "*****************" << std::endl;

  std::string path = "../../examples/data/";

  //Stage 1: Read from Netgen files, write to VTK
  viennagrid::io::netgen_reader my_netgen_reader;

  //test<viennagrid::config::line_1d>(my_netgen_reader, path + "line8.mesh", "io_1d");
  std::cout << "*********** Reading from Netgen, 2d ***********" << std::endl;
  test<viennagrid::triangular_2d_mesh>(my_netgen_reader, path + "square32.mesh", "io_2d");

  std::cout << "*********** Reading from Netgen, 3d ***********" << std::endl;
  test<viennagrid::tetrahedral_3d_mesh>(my_netgen_reader, path + "cube48.mesh", "io_3d");



  //Stage 2: Read VTK files, write to VTK
  //test<viennagrid::config::line_1d>("io_1d", "io_1d_2");

  viennagrid::io::vtk_reader<viennagrid::triangular_2d_mesh>  vtk_reader_2d;


  std::cout << "*********** Reading from VTK, 2d ***********" << std::endl;
  test_vtk<viennagrid::triangular_2d_mesh>(vtk_reader_2d, "io_2d_main.pvd", "io_2d_2");

  std::cout << "-- Scalar vertex quantities: --" << std::endl;
  print(vtk_reader_2d.scalar_vertex_data_names(1));
  std::cout << "-- Vector vertex quantities: --" << std::endl;
  print(vtk_reader_2d.vector_vertex_data_names(1));
  std::cout << "-- Scalar cell quantities: --" << std::endl;
  print(vtk_reader_2d.scalar_cell_data_names(1));
  std::cout << "-- Vector cell quantities: --" << std::endl;
  print(vtk_reader_2d.vector_cell_data_names(1));

  assert( (vtk_reader_2d.scalar_vertex_data_names(1).size() == 2) && "Not all data parsed!");
  assert( (vtk_reader_2d.vector_vertex_data_names(1).size() == 1) && "Not all data parsed!");
  assert( (vtk_reader_2d.scalar_cell_data_names(1).size() == 2) && "Not all data parsed!");
  assert( (vtk_reader_2d.vector_cell_data_names(1).size() == 1) && "Not all data parsed!");


  viennagrid::io::vtk_reader<viennagrid::tetrahedral_3d_mesh>  vtk_reader_3d;


  std::cout << "*********** Reading from VTK, 3d ***********" << std::endl;
  test_vtk<viennagrid::tetrahedral_3d_mesh>(vtk_reader_3d, "io_3d_main.pvd", "io_3d_2");

  std::cout << "-- Scalar vertex quantities: --" << std::endl;
  print(vtk_reader_3d.scalar_vertex_data_names(1));
  std::cout << "-- Vector vertex quantities: --" << std::endl;
  print(vtk_reader_3d.vector_vertex_data_names(1));
  std::cout << "-- Scalar cell quantities: --" << std::endl;
  print(vtk_reader_3d.scalar_cell_data_names(1));
  std::cout << "-- Vector cell quantities: --" << std::endl;
  print(vtk_reader_3d.vector_cell_data_names(1));

  assert( (vtk_reader_3d.scalar_vertex_data_names(1).size() == 2) && "Not all data parsed!");
  assert( (vtk_reader_3d.vector_vertex_data_names(1).size() == 1) && "Not all data parsed!");
  assert( (vtk_reader_3d.scalar_cell_data_names(1).size() == 2) && "Not all data parsed!");
  assert( (vtk_reader_3d.vector_cell_data_names(1).size() == 1) && "Not all data parsed!");

//   test<viennagrid::config::tetrahedral_3d>(vtk_reader_3d, "multi-segment_main.pvd", "io_3d_2");

  std::cout << "*******************************" << std::endl;
  std::cout << "* Test finished successfully! *" << std::endl;
  std::cout << "*******************************" << std::endl;

  return EXIT_SUCCESS;
}
