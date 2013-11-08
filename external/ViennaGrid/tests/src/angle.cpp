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

#include <cmath>

#include "viennagrid/forwards.hpp"
#include "viennagrid/mesh/element_creation.hpp"
#include "viennagrid/config/default_configs.hpp"
#include "viennagrid/point.hpp"
#include "viennagrid/algorithm/angle.hpp"

void fuzzy_check(double a, double b)
{
  if (a != b)
  {
    if (   (std::abs(a - b) / std::max( std::abs(a), std::abs(b) ) > 1e-10)
        && (std::abs(a - b) > 1e-6)
    )
    {
      std::cerr << "FAILED!" << std::endl;
      std::cerr << "Result mismatch: " << a << " vs. " << b << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  std::cout << "PASSED! (" << a << ", " << b << ")" << std::endl;
}

template <typename PointType>
void fuzzy_check(PointType const & a, PointType const & b)
{
  for (std::size_t i=0; i<a.size(); ++i)
    fuzzy_check(a[i], b[i]);
}



int main()
{
  std::cout << "*****************" << std::endl;
  std::cout << "* Test started! *" << std::endl;
  std::cout << "*****************" << std::endl;

  typedef viennagrid::line_1d_mesh                       Mesh1D;
  typedef viennagrid::result_of::point<Mesh1D>::type     PointType1D;

  typedef viennagrid::triangular_2d_mesh                 Mesh2D;
  typedef viennagrid::result_of::point<Mesh2D>::type     PointType2D;

  typedef viennagrid::tetrahedral_3d_mesh                Mesh3D;
  typedef viennagrid::result_of::point<Mesh3D>::type     PointType3D;

  double pi        = std::acos(-1.0);
  double pi_over_2 = std::acos( 0.0);

  //
  // Part 1: one-dimensional
  //
  std::cout << " --- Checking 1d ---" << std::endl;
  PointType1D x_1d(1);
  PointType1D y_1d(2);
  PointType1D z_1d(-1);

  fuzzy_check(viennagrid::angle(x_1d, x_1d), 0);
  fuzzy_check(viennagrid::angle(x_1d, y_1d), 0);
  fuzzy_check(viennagrid::angle(x_1d, z_1d), pi);

  //
  // Part 2: two-dimensional
  //
  std::cout << " --- Checking 2d ---" << std::endl;
  PointType2D x_2d(1, 1);
  PointType2D y_2d(-1, 1);
  PointType2D z_2d(0, 2);

  fuzzy_check(viennagrid::angle(x_2d, x_2d), 0);
  fuzzy_check(viennagrid::angle(x_2d, -x_2d), pi);
  fuzzy_check(viennagrid::angle(x_2d, y_2d),  pi_over_2);
  fuzzy_check(viennagrid::angle(x_2d, y_2d, z_2d), pi_over_2);

  if ( viennagrid::angle(x_2d, x_2d, x_2d) != viennagrid::angle(x_2d, x_2d, x_2d) )
    std::cout << "NaN check PASSED" << std::endl;
  else
    exit(EXIT_FAILURE);

  //
  // Part 3: three-dimension
  //
  std::cout << " --- Checking 3d ---" << std::endl;
  PointType3D u_3d( 1,  1,  2); u_3d /= viennagrid::norm(u_3d);
  PointType3D x_3d( 1,  1,  1); x_3d /= viennagrid::norm(x_3d);
  PointType3D y_3d( 1,  1, -1); y_3d /= viennagrid::norm(y_3d);
  PointType3D z_3d( 1, -1,  1); z_3d /= viennagrid::norm(z_3d);
  PointType3D a_3d(-1, -1, -1); a_3d /= viennagrid::norm(a_3d);
  PointType3D b_3d( 2 * x_3d[0],  0,  0);

  fuzzy_check(viennagrid::angle(x_3d, x_3d), 0);
  fuzzy_check(viennagrid::angle(u_3d, y_3d), pi_over_2);
  fuzzy_check(viennagrid::angle(x_3d, y_3d, z_3d), pi_over_2/2.0);

  if ( viennagrid::angle(x_3d, x_3d, x_3d) != viennagrid::angle(x_3d, x_3d, x_3d) )
    std::cout << "NaN check PASSED" << std::endl;
  else
    exit(EXIT_FAILURE);

  fuzzy_check(viennagrid::solid_angle(x_3d, y_3d, z_3d),       4*pi / 12.0);
  fuzzy_check(viennagrid::solid_angle(x_3d, y_3d, z_3d, b_3d), 4*pi / 12.0);

  fuzzy_check(viennagrid::dihedral_angle(x_3d, y_3d, z_3d,
                                         x_3d, y_3d, a_3d), pi_over_2/2.0);

  std::cout << "*******************************" << std::endl;
  std::cout << "* Test finished successfully! *" << std::endl;
  std::cout << "*******************************" << std::endl;

  return EXIT_SUCCESS;
}
