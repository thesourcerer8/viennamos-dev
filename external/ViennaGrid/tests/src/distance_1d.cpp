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

#include <cmath>

#include "viennagrid/forwards.hpp"
#include "viennagrid/domain/config.hpp"
#include "viennagrid/point.hpp"
#include "viennagrid/algorithm/distance.hpp"

void fuzzy_check(double a, double b)
{
  if (a != b)
  {
    if (   (std::abs(a - b) / std::max( std::abs(a), std::abs(b) ) > 1e-10) 
        && (std::abs(a - b) > 1e-10)
    )
    {
      std::cerr << "FAILED!" << std::endl;
      std::cerr << "Result mismatch: " << a << " vs. " << b << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  std::cout << "PASSED! (" << a << ", " << b << ")" << std::endl;
}


//
// Line 2d
//

void setup_domain(viennagrid::config::line_1d_domain & domain)
{
  typedef viennagrid::config::line_1d_domain                      DomainType;
  typedef viennagrid::line_tag                                    CellTag;
  
  typedef viennagrid::result_of::point_type<DomainType>::type          PointType;
  typedef viennagrid::result_of::element<DomainType, viennagrid::vertex_tag>::type       VertexType;
  typedef viennagrid::result_of::element_hook<DomainType, viennagrid::vertex_tag>::type       VertexHandleType;  
  typedef viennagrid::result_of::element<DomainType, CellTag>::type        CellType;
  
  const size_t s = 6;
  PointType p[s];
  VertexHandleType v[s];
                                       
  p[0] = PointType(2.0);
  p[1] = PointType(3.0);
  p[2] = PointType(5.0);
  p[3] = PointType(6.0);
  p[4] = PointType(2.5);
  p[5] = PointType(5.5);

  //upgrade to vertex:
  std::cout << "Adding vertices to domain..." << std::endl;
  for (int i = 0; i < s; ++i)
  {
    v[i] = viennagrid::create_element<VertexType>( domain );
    viennagrid::point( domain, v[i] ) = p[i];
  }

  std::cout << "Adding cells to domain..." << std::endl;
  viennagrid::create_element<CellType>( domain, v, v+2 );
  viennagrid::create_element<CellType>( domain, v+2, v+4 );
  viennagrid::create_element<CellType>( domain, v+4, v+6 );
}

void test(viennagrid::config::line_1d_domain)
{
  typedef viennagrid::config::line_1d_domain                            Domain;
  typedef viennagrid::line_tag                                          CellTag;
  
  typedef viennagrid::result_of::point_type<Domain>::type                PointType;
  typedef viennagrid::result_of::element<Domain, viennagrid::vertex_tag>::type             VertexType;
  typedef viennagrid::result_of::element<Domain, viennagrid::line_tag>::type             EdgeType;
  typedef viennagrid::result_of::element<Domain, CellTag>::type  CellType;

  Domain domain;
  
  setup_domain(domain);
  
  PointType A(0);
  PointType B(1.0);
  PointType C(1.5);
  PointType D(2.0);
  PointType E(2.5);
  PointType F(3.0);
  PointType G(3.5);
  PointType H(4.0);
  
  CellType line0 = viennagrid::elements<CellTag>(domain)[0];
  CellType line1 = viennagrid::elements<CellTag>(domain)[1];
  CellType line2 = viennagrid::elements<CellTag>(domain)[2];
  
  //
  // Distance checks for t0
  //
  
  std::cout << "Distance of point A to line0... ";
  fuzzy_check( viennagrid::distance(domain, A, line0), 2.0 );
  
  std::cout << "Distance of point B to line0... ";
  fuzzy_check( viennagrid::distance(domain, B, line0), 1.0 );

  std::cout << "Distance of point C to line0... ";
  fuzzy_check( viennagrid::distance(domain, C, line0), 0.5 );
  
  std::cout << "Distance of point D to line0... ";
  fuzzy_check( viennagrid::distance(domain, D, line0), 0 );
  
  std::cout << "Distance of point E to line0... ";
  fuzzy_check( viennagrid::distance(domain, E, line0), 0 );
  
  std::cout << "Distance of point F to line0... ";
  fuzzy_check( viennagrid::distance(domain, F, line0), 0 );
  
  std::cout << "Distance of point G to line0... ";
  fuzzy_check( viennagrid::distance(domain, G, line0), 0.5 );
  
  std::cout << "Distance of point H to line0... ";
  fuzzy_check( viennagrid::distance(domain, H, line0), 1.0 );

  std::cout << "Distance of point H to line0... ";
  fuzzy_check( viennagrid::distance(domain, H, line0), 1.0 );
  
  std::cout << std::endl;
  
  //
  // Distance line to line
  //
  
  std::cout << "Distance of line0 to line0... ";
  fuzzy_check( viennagrid::distance(domain, line0, line0), 0.0 );
  
  std::cout << "Distance of line0 to line1... ";
  fuzzy_check( viennagrid::distance(domain, line0, line1), 2.0 );

  std::cout << "Distance of line0 to line2... ";
  fuzzy_check( viennagrid::distance(domain, line0, line2), 0.0 );

  
  std::cout << "Distance of line1 to line0... ";
  fuzzy_check( viennagrid::distance(domain, line1, line0), 2.0 );
  
  std::cout << "Distance of line1 to line1... ";
  fuzzy_check( viennagrid::distance(domain, line1, line1), 0.0 );
  
  std::cout << "Distance of line1 to line2... ";
  fuzzy_check( viennagrid::distance(domain, line1, line2), 0.0 );

  
  std::cout << "Distance of line1 to line0... ";
  fuzzy_check( viennagrid::distance(domain, line2, line0), 0.0 );
  
  std::cout << "Distance of line1 to line1... ";
  fuzzy_check( viennagrid::distance(domain, line2, line1), 0.0 );
  
  std::cout << "Distance of line1 to line2... ";
  fuzzy_check( viennagrid::distance(domain, line2, line2), 0.0 );
  
  
}



int main()
{
  std::cout << "*****************" << std::endl;
  std::cout << "* Test started! *" << std::endl;
  std::cout << "*****************" << std::endl;

  std::cout << "==== Testing line mesh in 1D ====" << std::endl;
  test(viennagrid::config::line_1d_domain());
  
  std::cout << "*******************************" << std::endl;
  std::cout << "* Test finished successfully! *" << std::endl;
  std::cout << "*******************************" << std::endl;
  
  return EXIT_SUCCESS;
}
