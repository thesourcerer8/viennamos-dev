/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics,
                       Institute for Analysis and Scientific Computing,
                       TU Wien.

                            -----------------
                     ViennaGrid - The Vienna Grid Library
                            -----------------

   Authors:      Karl Rupp                           rupp@iue.tuwien.ac.at
                 Josef Weinbub                    weinbub@iue.tuwien.ac.at
               
   (A list of additional contributors can be found in the PDF manual)

   License:      MIT (X11), see file LICENSE in the base directory
======================================================================= */

#ifdef _MSC_VER      //Visual Studio complains about potentially dangerous things, which are perfectly legal in our context
  #pragma warning( disable : 4355 )     //use of this in member initializer list
  #pragma warning( disable : 4503 )     //truncated name decoration
#endif

//***********************************************
// Define the input-file format
//***********************************************

#include "viennagrid/forwards.h"
#include "viennagrid/element.hpp"
#include "viennagrid/point.hpp"
#include "viennagrid/domain.hpp"
#include "viennagrid/config/simplex.hpp"
#include "viennagrid/config/others.hpp"
//#include "viennagrid/segment.hpp"
//#include "viennagrid/boundary.hpp"
#include "viennagrid/io/vtk_writer.hpp"


template <viennagrid::dim_type dim, 
          typename SegmentT>
void print_elements(SegmentT & seg)
{
  SegmentT const & const_seg = seg;

  std::cout << "-- non-const --" << std::endl;
  typedef typename viennagrid::result_of::ncell_range<SegmentT, dim>::type  ContainerT;
  typedef typename viennagrid::result_of::iterator<ContainerT>::type           ContainerTIterator;
  
  ContainerT elements = viennagrid::ncells<dim>(seg);
  for (ContainerTIterator it = elements.begin();
       it != elements.end();
       ++it)
  {
    std::cout << *it << std::endl; 
    //it->print_short();
  }
  
  std::cout << "-- const --" << std::endl;
  typedef typename viennagrid::result_of::const_ncell_range<SegmentT, dim>::type   ConstContainerT;
  typedef typename viennagrid::result_of::iterator<ConstContainerT>::type             ConstContainerTIterator;
  
  ConstContainerT const_elements = viennagrid::ncells<dim>(const_seg);
  for (ConstContainerTIterator const_it = const_elements.begin();
       const_it != const_elements.end();
       ++const_it)
  {
    std::cout << *const_it << std::endl; 
    //const_it->print_short();
  }
}


//test for 3d tetrahedral case:
void test(viennagrid::config::tetrahedral_3d)
{
  typedef viennagrid::config::tetrahedral_3d          ConfigType;
  typedef viennagrid::result_of::domain<ConfigType>::type              Domain;
  typedef ConfigType::cell_tag                        CellTag;
  typedef viennagrid::segment_t<ConfigType>           SegmentType;
  
  //typedef viennagrid::TestDomainConfig::DimensionTag              DimensionTag;
  typedef viennagrid::result_of::point<ConfigType>::type          PointType;
  typedef viennagrid::result_of::ncell<ConfigType, 0>::type       VertexType;
  typedef viennagrid::result_of::ncell<ConfigType, 1>::type       EdgeType;
  typedef viennagrid::result_of::ncell<ConfigType,
                                                     CellTag::dim>::type   CellType;

  Domain domain;
  
  PointType p0(0.0, 0.0, 0.0);
  PointType p1(0.5, 0.0, 0.0);
  PointType p2(1.0, 0.0, 0.0);
  PointType p3(0.0, 1.0, 0.0);
  PointType p4(0.5, 1.0, 0.0);
  PointType p5(1.0, 1.0, 0.0);
  PointType p6(0.5, 0.0, 1.0);

  //upgrade to vertex:
  VertexType v0(p0, 0);
  VertexType v1(p1, 1);
  VertexType v2(p2, 2);
  VertexType v3(p3, 3);
  VertexType v4(p4, 4);
  VertexType v5(p5, 5);
  VertexType v6(p6, 6);

  VertexType * vertices[7];
  
  std::cout << "Adding vertices to domain..." << std::endl;
  vertices[0] = domain.push_back(v0); std::cout << vertices[0] << std::endl;
  vertices[1] = domain.push_back(v1); std::cout << vertices[1] << std::endl;
  vertices[2] = domain.push_back(v2); std::cout << vertices[2] << std::endl;
  vertices[3] = domain.push_back(v3); std::cout << vertices[3] << std::endl;
  vertices[4] = domain.push_back(v4); std::cout << vertices[4] << std::endl;
  vertices[5] = domain.push_back(v5); std::cout << vertices[5] << std::endl;
  vertices[6] = domain.push_back(v6); std::cout << vertices[6] << std::endl;
  
  std::cout << "Creating segments..." << std::endl;
  domain.segments().resize(2);

  std::cout << "Adding cells to segments..." << std::endl;
  CellType simplex;
  
  std::cout << "Filling segment 0..." << std::endl;
  vertices[0] = &(viennagrid::ncells<0>(domain)[0]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[6]);
  vertices[3] = &(viennagrid::ncells<0>(domain)[3]);
  simplex.vertices(vertices);
  domain.segments()[0].push_back(simplex);
  
  vertices[0] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[6]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[3]);
  vertices[3] = &(viennagrid::ncells<0>(domain)[4]);
  simplex.vertices(vertices);
  domain.segments()[0].push_back(simplex);

  std::cout << "Filling segment 1..." << std::endl;
  vertices[0] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[2]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[6]);
  vertices[3] = &(viennagrid::ncells<0>(domain)[4]);
  simplex.vertices(vertices);
  domain.segments()[1].push_back(simplex);
  
  vertices[0] = &(viennagrid::ncells<0>(domain)[2]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[6]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[4]);
  vertices[3] = &(viennagrid::ncells<0>(domain)[5]);
  simplex.vertices(vertices);
  domain.segments()[1].push_back(simplex);
  
  
  std::cout << "Vertices in Segment 0: " << viennagrid::ncells<0>(domain.segments()[0]).size() << std::endl;
  std::cout << "Vertices in Segment 1: " << viennagrid::ncells<0>(domain.segments()[1]).size() << std::endl;
  std::cout << "Edges in Segment 0: "    << viennagrid::ncells<1>(domain.segments()[0]).size() << std::endl;
  std::cout << "Edges in Segment 1: "    << viennagrid::ncells<1>(domain.segments()[1]).size() << std::endl;
  std::cout << "Cells in Segment 0: "    << viennagrid::ncells<CellTag::dim>(domain.segments()[0]).size() << std::endl;
  std::cout << "Cells in Segment 1: "    << viennagrid::ncells<CellTag::dim>(domain.segments()[1]).size() << std::endl;
  
  std::cout << "Printing vertices in segment 0:" << std::endl;
  print_elements<0>(domain.segments()[0]);

  std::cout << "Printing vertices in segment 1:" << std::endl;
  print_elements<0>(domain.segments()[1]);
  
  std::cout << "Printing cells in segment 0:" << std::endl;
  print_elements<Domain::config_type::cell_tag::dim>(domain.segments()[0]);

  std::cout << "Printing cells in segment 1:" << std::endl;
  print_elements<Domain::config_type::cell_tag::dim>(domain.segments()[1]);
  
  std::cout << "Test for direct operator[] access: " << std::endl;
  //viennagrid::ncells<0>(domain.segments()[0])[0].print_short();
  std::cout << viennagrid::ncells<3>(domain.segments()[0])[0] << std::endl;
  
  viennagrid::io::vtk_writer<Domain> my_vtk_writer;
  my_vtk_writer(domain, "multi_segment_tet");
}

//test for 3d hexahedral case:
void test(viennagrid::config::hexahedral_3d)
{
  typedef viennagrid::config::hexahedral_3d          ConfigType;
  typedef viennagrid::result_of::domain<ConfigType>::type              Domain;
  typedef ConfigType::cell_tag                        CellTag;
  typedef viennagrid::segment_t<ConfigType>           SegmentType;
  
  //typedef viennagrid::TestDomainConfig::DimensionTag              DimensionTag;
  typedef viennagrid::result_of::point<ConfigType>::type          PointType;
  typedef viennagrid::result_of::ncell<ConfigType, 0>::type       VertexType;
  typedef viennagrid::result_of::ncell<ConfigType, 1>::type       EdgeType;
  typedef viennagrid::result_of::ncell<ConfigType,
                                                     CellTag::dim>::type   CellType;

  Domain domain;
  
  PointType p0(0.0, 0.0, 0.0);
  PointType p1(0.5, 0.0, 0.0);
  PointType p2(1.0, 0.0, 0.0);
  PointType p3(1.0, 1.0, 0.0);
  PointType p4(0.5, 1.0, 0.0);
  PointType p5(0.0, 1.0, 0.0);
  PointType p6(0.0, 0.0, 1.0);
  PointType p7(0.5, 0.0, 1.0);
  PointType p8(1.0, 0.0, 1.0);
  PointType p9(1.0, 1.0, 1.0);
  PointType p10(0.5, 1.0, 1.0);
  PointType p11(0.0, 1.0, 1.0);

  //upgrade to vertex:
  VertexType v0(p0, 0);
  VertexType v1(p1, 1);
  VertexType v2(p2, 2);
  VertexType v3(p3, 3);
  VertexType v4(p4, 4);
  VertexType v5(p5, 5);
  VertexType v6(p6, 6);
  VertexType v7(p7, 7);
  VertexType v8(p8, 8);
  VertexType v9(p9, 9);
  VertexType v10(p10, 10);
  VertexType v11(p11, 11);

  VertexType * vertices[12];
  
  std::cout << "Adding vertices to domain..." << std::endl;
  vertices[0] = domain.push_back(v0); std::cout << vertices[0] << std::endl;
  vertices[1] = domain.push_back(v1); std::cout << vertices[1] << std::endl;
  vertices[2] = domain.push_back(v2); std::cout << vertices[2] << std::endl;
  vertices[3] = domain.push_back(v3); std::cout << vertices[3] << std::endl;
  vertices[4] = domain.push_back(v4); std::cout << vertices[4] << std::endl;
  vertices[5] = domain.push_back(v5); std::cout << vertices[5] << std::endl;
  vertices[6] = domain.push_back(v6); std::cout << vertices[6] << std::endl;
  vertices[7] = domain.push_back(v7); std::cout << vertices[7] << std::endl;
  vertices[8] = domain.push_back(v8); std::cout << vertices[8] << std::endl;
  vertices[9] = domain.push_back(v9); std::cout << vertices[9] << std::endl;
  vertices[10] = domain.push_back(v10); std::cout << vertices[10] << std::endl;
  vertices[11] = domain.push_back(v11); std::cout << vertices[11] << std::endl;
  
  std::cout << "Creating segments..." << std::endl;
  domain.segments().resize(2);

  std::cout << "Adding cells to segments..." << std::endl;
  CellType simplex;
  
  std::cout << "Filling segment 0..." << std::endl;
  vertices[0] = &(viennagrid::ncells<0>(domain)[0]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[4]);
  vertices[3] = &(viennagrid::ncells<0>(domain)[5]);
  vertices[4] = &(viennagrid::ncells<0>(domain)[6]);
  vertices[5] = &(viennagrid::ncells<0>(domain)[7]);
  vertices[6] = &(viennagrid::ncells<0>(domain)[10]);
  vertices[7] = &(viennagrid::ncells<0>(domain)[11]);
  simplex.vertices(vertices);
  domain.segments()[0].push_back(simplex);

  std::cout << "Filling segment 1..." << std::endl;
  vertices[0] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[2]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[3]);
  vertices[3] = &(viennagrid::ncells<0>(domain)[4]);
  vertices[4] = &(viennagrid::ncells<0>(domain)[7]);
  vertices[5] = &(viennagrid::ncells<0>(domain)[8]);
  vertices[6] = &(viennagrid::ncells<0>(domain)[9]);
  vertices[7] = &(viennagrid::ncells<0>(domain)[10]);
  simplex.vertices(vertices);
  domain.segments()[1].push_back(simplex);
  
  
  std::cout << "Vertices in Segment 0: " << viennagrid::ncells<0>(domain.segments()[0]).size() << std::endl;
  std::cout << "Vertices in Segment 1: " << viennagrid::ncells<0>(domain.segments()[1]).size() << std::endl;
  std::cout << "Edges in Segment 0: "    << viennagrid::ncells<1>(domain.segments()[0]).size() << std::endl;
  std::cout << "Edges in Segment 1: "    << viennagrid::ncells<1>(domain.segments()[1]).size() << std::endl;
  std::cout << "Cells in Segment 0: "    << viennagrid::ncells<CellTag::dim>(domain.segments()[0]).size() << std::endl;
  std::cout << "Cells in Segment 1: "    << viennagrid::ncells<CellTag::dim>(domain.segments()[1]).size() << std::endl;
  
  std::cout << "Printing vertices in segment 0:" << std::endl;
  print_elements<0>(domain.segments()[0]);

  std::cout << "Printing vertices in segment 1:" << std::endl;
  print_elements<0>(domain.segments()[1]);
  
  std::cout << "Printing cells in segment 0:" << std::endl;
  print_elements<Domain::config_type::cell_tag::dim>(domain.segments()[0]);

  std::cout << "Printing cells in segment 1:" << std::endl;
  print_elements<Domain::config_type::cell_tag::dim>(domain.segments()[1]);
  
  std::cout << "Test for direct operator[] access: " << std::endl;
  //viennagrid::ncells<0>(domain.segments()[0])[0].print_short();
  std::cout << viennagrid::ncells<3>(domain.segments()[0])[0] << std::endl;
  
  viennagrid::io::vtk_writer<Domain> my_vtk_writer;
  my_vtk_writer(domain, "Testing/multi_segment_hex");
}


//test for 2d triangular case:
void test(viennagrid::config::triangular_2d)
{
  typedef viennagrid::config::triangular_2d           ConfigType;
  typedef viennagrid::result_of::domain<ConfigType>::type              Domain;
  typedef ConfigType::cell_tag                        CellTag;
  typedef viennagrid::segment_t<ConfigType>           SegmentType;
  
  //typedef viennagrid::TestDomainConfig::DimensionTag              DimensionTag;
  typedef viennagrid::result_of::point<ConfigType>::type          PointType;
  typedef viennagrid::result_of::ncell<ConfigType, 0>::type       VertexType;
  typedef viennagrid::result_of::ncell<ConfigType, 1>::type       EdgeType;
  typedef viennagrid::result_of::ncell<ConfigType,
                                                     CellTag::dim>::type   CellType;

  Domain domain;
  
  PointType p0(0.0, 0.0);
  PointType p1(0.5, 0.0);
  PointType p2(1.0, 0.0);
  PointType p3(0.0, 1.0);
  PointType p4(0.5, 1.0);
  PointType p5(1.0, 1.0);

  //upgrade to vertex:
  VertexType v0(p0, 0);
  VertexType v1(p1, 1);
  VertexType v2(p2, 2);
  VertexType v3(p3, 3);
  VertexType v4(p4, 4);
  VertexType v5(p5, 5);

  VertexType * vertices[6];
  
  std::cout << "Adding vertices to domain..." << std::endl;
  vertices[0] = domain.push_back(v0); std::cout << vertices[0] << std::endl;
  vertices[1] = domain.push_back(v1); std::cout << vertices[1] << std::endl;
  vertices[2] = domain.push_back(v2); std::cout << vertices[2] << std::endl;
  vertices[3] = domain.push_back(v3); std::cout << vertices[3] << std::endl;
  vertices[4] = domain.push_back(v4); std::cout << vertices[4] << std::endl;
  vertices[5] = domain.push_back(v5); std::cout << vertices[5] << std::endl;
  
  std::cout << "Creating segments..." << std::endl;
  domain.segments().resize(2);

  std::cout << "Adding cells to segments..." << std::endl;
  CellType simplex;
  
  std::cout << "Filling segment 0..." << std::endl;
  vertices[0] = &(viennagrid::ncells<0>(domain)[0]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[3]);
  simplex.vertices(vertices);
  domain.segments()[0].push_back(simplex);
  
  vertices[0] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[4]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[3]);
  simplex.vertices(vertices);
  domain.segments()[0].push_back(simplex);

  std::cout << "Filling segment 1..." << std::endl;
  vertices[0] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[2]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[4]);
  simplex.vertices(vertices);
  domain.segments()[1].push_back(simplex);
  
  vertices[0] = &(viennagrid::ncells<0>(domain)[2]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[5]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[4]);
  simplex.vertices(vertices);
  domain.segments()[1].push_back(simplex);
  
  
  std::cout << "Vertices in Segment 0: " << viennagrid::ncells<0>(domain.segments()[0]).size() << std::endl;
  std::cout << "Vertices in Segment 1: " << viennagrid::ncells<0>(domain.segments()[1]).size() << std::endl;
  std::cout << "Edges in Segment 0: "    << viennagrid::ncells<1>(domain.segments()[0]).size() << std::endl;
  std::cout << "Edges in Segment 1: "    << viennagrid::ncells<1>(domain.segments()[1]).size() << std::endl;
  std::cout << "Cells in Segment 0: "    << viennagrid::ncells<CellTag::dim>(domain.segments()[0]).size() << std::endl;
  std::cout << "Cells in Segment 1: "    << viennagrid::ncells<CellTag::dim>(domain.segments()[1]).size() << std::endl;
  
  std::cout << "Printing vertices in segment 0:" << std::endl;
  print_elements<0>(domain.segments()[0]);

  std::cout << "Printing vertices in segment 1:" << std::endl;
  print_elements<0>(domain.segments()[1]);
  
  std::cout << "Printing cells in segment 0:" << std::endl;
  print_elements<Domain::config_type::cell_tag::dim>(domain.segments()[0]);

  std::cout << "Printing cells in segment 1:" << std::endl;
  print_elements<Domain::config_type::cell_tag::dim>(domain.segments()[1]);

  std::cout << "Test for direct operator[] access: " << std::endl;
//  viennagrid::ncells<0>(domain.segments()[0])[0].print_short();
  std::cout << viennagrid::ncells<2>(domain.segments()[0])[0] << std::endl;
  
  viennagrid::io::vtk_writer<Domain> my_vtk_writer;
  my_vtk_writer(domain, "Testing/multi_segment_tri");
  
}

//test for 2d quadrilateral case:
void test(viennagrid::config::quadrilateral_2d)
{
  typedef viennagrid::config::quadrilateral_2d           ConfigType;
  typedef viennagrid::result_of::domain<ConfigType>::type              Domain;
  typedef ConfigType::cell_tag                        CellTag;
  typedef viennagrid::segment_t<ConfigType>           SegmentType;
  
  //typedef viennagrid::TestDomainConfig::DimensionTag              DimensionTag;
  typedef viennagrid::result_of::point<ConfigType>::type          PointType;
  typedef viennagrid::result_of::ncell<ConfigType, 0>::type       VertexType;
  typedef viennagrid::result_of::ncell<ConfigType, 1>::type       EdgeType;
  typedef viennagrid::result_of::ncell<ConfigType,
                                                     CellTag::dim>::type   CellType;

  Domain domain;
  
  PointType p0(0.0, 0.0);
  PointType p1(0.5, 0.0);
  PointType p2(1.0, 0.0);
  PointType p3(0.0, 1.0);
  PointType p4(0.5, 1.0);
  PointType p5(1.0, 1.0);

  //upgrade to vertex:
  VertexType v0(p0, 0);
  VertexType v1(p1, 1);
  VertexType v2(p2, 2);
  VertexType v3(p3, 3);
  VertexType v4(p4, 4);
  VertexType v5(p5, 5);

  VertexType * vertices[6];
  
  std::cout << "Adding vertices to domain..." << std::endl;
  vertices[0] = domain.push_back(v0); std::cout << vertices[0] << std::endl;
  vertices[1] = domain.push_back(v1); std::cout << vertices[1] << std::endl;
  vertices[2] = domain.push_back(v2); std::cout << vertices[2] << std::endl;
  vertices[3] = domain.push_back(v3); std::cout << vertices[3] << std::endl;
  vertices[4] = domain.push_back(v4); std::cout << vertices[4] << std::endl;
  vertices[5] = domain.push_back(v5); std::cout << vertices[5] << std::endl;
  
  std::cout << "Creating segments..." << std::endl;
  domain.segments().resize(2);

  std::cout << "Adding cells to segments..." << std::endl;
  CellType quad;
  
  std::cout << "Filling segment 0..." << std::endl;
  vertices[0] = &(viennagrid::ncells<0>(domain)[0]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[4]);
  vertices[3] = &(viennagrid::ncells<0>(domain)[5]);
  quad.vertices(vertices);
  domain.segments()[0].push_back(quad);

  std::cout << "Filling segment 1..." << std::endl;
  vertices[0] = &(viennagrid::ncells<0>(domain)[1]);
  vertices[1] = &(viennagrid::ncells<0>(domain)[2]);
  vertices[2] = &(viennagrid::ncells<0>(domain)[3]);
  vertices[3] = &(viennagrid::ncells<0>(domain)[4]);
  quad.vertices(vertices);
  domain.segments()[1].push_back(quad);
  
  
  std::cout << "Vertices in Segment 0: " << viennagrid::ncells<0>(domain.segments()[0]).size() << std::endl;
  std::cout << "Vertices in Segment 1: " << viennagrid::ncells<0>(domain.segments()[1]).size() << std::endl;
  std::cout << "Edges in Segment 0: "    << viennagrid::ncells<1>(domain.segments()[0]).size() << std::endl;
  std::cout << "Edges in Segment 1: "    << viennagrid::ncells<1>(domain.segments()[1]).size() << std::endl;
  std::cout << "Cells in Segment 0: "    << viennagrid::ncells<CellTag::dim>(domain.segments()[0]).size() << std::endl;
  std::cout << "Cells in Segment 1: "    << viennagrid::ncells<CellTag::dim>(domain.segments()[1]).size() << std::endl;
  
  std::cout << "Printing vertices in segment 0:" << std::endl;
  print_elements<0>(domain.segments()[0]);

  std::cout << "Printing vertices in segment 1:" << std::endl;
  print_elements<0>(domain.segments()[1]);
  
  std::cout << "Printing cells in segment 0:" << std::endl;
  print_elements<Domain::config_type::cell_tag::dim>(domain.segments()[0]);

  std::cout << "Printing cells in segment 1:" << std::endl;
  print_elements<Domain::config_type::cell_tag::dim>(domain.segments()[1]);

  std::cout << "Test for direct operator[] access: " << std::endl;
//  viennagrid::ncells<0>(domain.segments()[0])[0].print_short();
  std::cout << viennagrid::ncells<2>(domain.segments()[0])[0] << std::endl;
  
  viennagrid::io::vtk_writer<Domain> my_vtk_writer;
  my_vtk_writer(domain, "Testing/multi_segment_quad");
  
}

int main()
{
  std::cout << "*****************" << std::endl;
  std::cout << "* Test started! *" << std::endl;
  std::cout << "*****************" << std::endl;
  
  std::cout << "--- Testing with triangular_2d ---" << std::endl;
  test(viennagrid::config::triangular_2d());
  std::cout << "--- Testing with tetrahedral_3d ---" << std::endl;
  test(viennagrid::config::tetrahedral_3d());

  std::cout << "--- Testing with quadrilateral_2d ---" << std::endl;
  test(viennagrid::config::quadrilateral_2d());
  std::cout << "--- Testing with hexahedral_3d ---" << std::endl;
  test(viennagrid::config::hexahedral_3d());
  
  std::cout << "*******************************" << std::endl;
  std::cout << "* Test finished successfully! *" << std::endl;
  std::cout << "*******************************" << std::endl;
  return EXIT_SUCCESS;
}
