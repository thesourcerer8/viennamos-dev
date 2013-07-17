#ifndef VIENNAGRID_ALGORITHM_CLOSEST_POINTS_HPP
#define VIENNAGRID_ALGORITHM_CLOSEST_POINTS_HPP

/* =======================================================================
   Copyright (c) 2011-2012, Institute for Microelectronics,
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

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <limits>
#include <utility>

#include "viennagrid/forwards.hpp"
#include "viennagrid/topology/all.hpp"
#include "viennagrid/algorithm/norm.hpp"
#include "viennagrid/algorithm/inner_prod.hpp"
#include "viennagrid/algorithm/boundary.hpp"
#include "viennagrid/domain/domain.hpp"

/** @file closest_points.hpp
    @brief Routines for computing the two points of two different objects being closest to each other.
*/


namespace viennagrid
{
  namespace detail
  {

    template <typename CoordType, typename CoordinateSystem>
    std::pair<point_t<CoordType, CoordinateSystem>,
              point_t<CoordType, CoordinateSystem> > const &
    point_pair_with_shortest_distance( std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_1,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_2)
    {
      if ( norm_2(pair_1.first - pair_1.second) <= norm_2(pair_2.first - pair_2.second) )
        return pair_1;

      return pair_2;
    }

    template <typename CoordType, typename CoordinateSystem>
    std::pair<point_t<CoordType, CoordinateSystem>,
              point_t<CoordType, CoordinateSystem> > const &
    point_pair_with_shortest_distance( std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_1,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_2,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_3)
    {
      CoordType dist_pair_1 = norm_2(pair_1.first - pair_1.second);
      CoordType dist_pair_2 = norm_2(pair_2.first - pair_2.second);
      CoordType dist_pair_3 = norm_2(pair_3.first - pair_3.second);

      if (   (dist_pair_1 <= dist_pair_2)
          && (dist_pair_1 <= dist_pair_3) )
        return pair_1;

      if (   (dist_pair_2 <= dist_pair_1)
          && (dist_pair_2 <= dist_pair_3) )
        return pair_2;

      return pair_3;
    }


    template <typename CoordType, typename CoordinateSystem>
    std::pair<point_t<CoordType, CoordinateSystem>,
              point_t<CoordType, CoordinateSystem> > const &
    point_pair_with_shortest_distance( std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_1,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_2,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_3,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_4)
    {
      // Note: Recursive use of point_pair_with_shortest_distance() has a bit of overhead. Feel free to improve this
      return point_pair_with_shortest_distance(point_pair_with_shortest_distance(pair_1, pair_2),
                                               point_pair_with_shortest_distance(pair_3, pair_4));
    }

    template <typename CoordType, typename CoordinateSystem>
    std::pair<point_t<CoordType, CoordinateSystem>,
              point_t<CoordType, CoordinateSystem> > const &
    point_pair_with_shortest_distance( std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_1,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_2,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_3,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_4,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_5)
    {
      // Note: Recursive use of point_pair_with_shortest_distance() has a bit of overhead. Feel free to improve this
      return point_pair_with_shortest_distance(point_pair_with_shortest_distance(pair_1, pair_2, pair_3),
                                               point_pair_with_shortest_distance(pair_4, pair_5));
    }

    template <typename CoordType, typename CoordinateSystem>
    std::pair<point_t<CoordType, CoordinateSystem>,
              point_t<CoordType, CoordinateSystem> > const &
    point_pair_with_shortest_distance( std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_1,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_2,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_3,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_4,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_5,
                                       std::pair<point_t<CoordType, CoordinateSystem>,
                                                 point_t<CoordType, CoordinateSystem> > const & pair_6)
    {
      // Note: Recursive use of point_pair_with_shortest_distance() has a bit of overhead. Feel free to improve this
      return point_pair_with_shortest_distance(point_pair_with_shortest_distance(pair_1, pair_2, pair_3),
                                               point_pair_with_shortest_distance(pair_4, pair_5, pair_6));
    }


    //
    // Closest points between points (trivial)
    //

    // Closest points between two points:
    template <typename PointType>
    std::pair<PointType, PointType>
    closest_points_impl(PointType const & p1,
                        PointType const & p2)
    {
      return std::make_pair(p1, p2);
    }

    template <typename PointAccessorType, typename CoordType1, typename CoordinateSystem1, typename CoordType2, typename CoordinateSystem2>
    std::pair< point_t<CoordType1, CoordinateSystem1>, point_t<CoordType2, CoordinateSystem2> >
    closest_points_impl(PointAccessorType const,
                        point_t<CoordType1, CoordinateSystem1> const & p1,
                        point_t<CoordType2, CoordinateSystem2> const & p2)
    {
      return closest_points_impl(p1, p2);
    }

    template <typename PointAccessorType, typename PointType, typename WrappedConfigType>
    std::pair<PointType, typename viennagrid::result_of::point<element_t< vertex_tag,WrappedConfigType> >::type>
    closest_points_impl(PointAccessorType const accessor,
                        PointType const & p1,
                        element_t<vertex_tag,WrappedConfigType> const & v2)
    {
      return closest_points_impl(p1, accessor(v2));
    }

    template <typename PointAccessorType, typename PointType, typename WrappedConfigType>
    std::pair<typename viennagrid::result_of::point<element_t< vertex_tag,WrappedConfigType> >::type, PointType>
    closest_points_impl(PointAccessorType const accessor,
                        element_t<vertex_tag,WrappedConfigType> const & v1,
                        PointType const & p2)
    {
      return closest_points_impl(accessor(v1), p2);
    }

    // Closest points between vertices:
    template <typename PointAccessorType, typename PointType, typename WrappedConfigType>
    std::pair<typename viennagrid::result_of::point<element_t< vertex_tag,WrappedConfigType> >::type, typename viennagrid::result_of::point<element_t< vertex_tag,WrappedConfigType> >::type>
    closest_points_impl(PointAccessorType const accessor,
                        element_t<vertex_tag,WrappedConfigType> const & v1,
                        element_t<vertex_tag,WrappedConfigType> const & v2)
    {
      return closest_points_impl( accessor(v1), accessor(v2) );
    }



    //
    // Closest points between point and line segment
    //

    // Implementation: Supposed to work for arbitrary dimensions
    template <typename PointType, typename LinePointType>
    std::pair<PointType, LinePointType>
            closest_points_point_line(PointType const & p,
                                      LinePointType const & line_p1,
                                      LinePointType const & line_p2)
    {
      //typedef point_t<CoordType, CoordinateSystem>  PointType;
      typedef typename result_of::coord< PointType >::type CoordType;

      //compute t such that projection of p onto [line_p1, line_p2] is given by p' = line_p1 + t * (line_p2 - line_p1)
      CoordType t = viennagrid::inner_prod( (p - line_p1), (line_p2 - line_p1) ) / viennagrid::inner_prod(line_p2 - line_p1, line_p2 - line_p1);

      //restrict t to line segment, i.e. t \in [0, 1]
      t = std::max<CoordType>(0, std::min<CoordType>(1, t));

      LinePointType p_prime = line_p1 + t * (line_p2 - line_p1);  //closest point to p on line

      return std::make_pair(p, p_prime);
    }


    //convenience overload: point and simplex line
    template <typename PointAccessorType, typename PointType, typename WrappedConfigType>
    std::pair< PointType, typename viennagrid::result_of::point<PointAccessorType>::type >
            closest_points_impl(PointAccessorType const accessor,
                                PointType const & p,
                                element_t<simplex_tag<1>,WrappedConfigType> const & el)
    {
      return closest_points_point_line(p,
                                       accessor(vertices(el)[0]),
                                       accessor(vertices(el)[1]));
    }

    //convenience overload: point and hypercube line
    template <typename PointAccessorType, typename PointType, typename WrappedConfigType>
    std::pair< PointType, typename viennagrid::result_of::point<PointAccessorType>::type >
            closest_points_impl(PointAccessorType const accessor,
                                PointType const & p,
                                element_t<hypercube_tag<1>,WrappedConfigType> const & el)
    {
      return closest_points_point_line(p,
                                       accessor(vertices(el)[0]),
                                       accessor(vertices(el)[1]));
    }

    //convenience overload: vertex and simplex line
    template <typename PointAccessorType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair< typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type >
            closest_points_impl(PointAccessorType const accessor,
                                element_t<vertex_tag,WrappedConfigType1> const & v,
                                element_t<simplex_tag<1>,WrappedConfigType2> const & el)
    {
      return closest_points_point_line(accessor(v),
                                       accessor(vertices(el)[0]),
                                       accessor(vertices(el)[1]));
    }

    //convenience overload: vertex and hypercube line
    template <typename PointAccessorType, typename PointType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair< typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type >
            closest_points_impl(PointAccessorType const accessor,
                                element_t<vertex_tag,WrappedConfigType1> const & v,
                                element_t<simplex_tag<1>,WrappedConfigType2> const & el)
    {
      return closest_points_point_line(accessor(v),
                                       accessor(vertices(el)[0]),
                                       accessor(vertices(el)[1]));
    }

    //
    // Distance between two line segments
    //

    // Implementation: Supposed to work for arbitrary dimensions
    template <typename PointType>
    std::pair<PointType, PointType>
            closest_points_line_line(PointType const & v0, PointType const & v1, //endpoints of first line
                                     PointType const & w0, PointType const & w1) //endpoints of second line
    {
      //typedef point_t<CoordType, CoordinateSystem>  PointType;
      typedef typename result_of::coord< PointType >::type CoordType;

      // write V(s) = v0 + s * (v1 - v0), s \in [0,1]
      //       W(t) = w0 + t * (w1 - w0), t \in [0,1]

      // compute s and t assuming V(s) and W(t) to be infinite lines:
      // cf. http://www.softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm
      PointType dir_v = v1 - v0;  //direction vector for line V(s)
      PointType dir_w = w1 - w0;  //direction vector for line W(t)

      CoordType v_in_v = viennagrid::inner_prod(dir_v, dir_v);
      CoordType v_in_w = viennagrid::inner_prod(dir_v, dir_w);
      CoordType w_in_w = viennagrid::inner_prod(dir_w, dir_w);

      CoordType denominator = v_in_v * w_in_w - v_in_w * v_in_w;
      //std::cout << "denominator: " << denominator << std::endl;

      if (denominator < 1e-6 * v_in_v * w_in_w) //lines parallel (up to round-off)
      {
        return point_pair_with_shortest_distance(closest_points_point_line(w0, v0, v1),
                                                 closest_points_point_line(w1, v0, v1),
                                                 closest_points_point_line(v0, w0, w1),
                                                 closest_points_point_line(v1, w0, w1));
      }

      //Lines are not parallel: Compute minimizers s, t:
      PointType dir_distance = v0 - w0;  //any vector connecting two points on V and W

      //if (inner_prod(dir_distance, dir_distance) / v_in_v < 1e-10)  //v0 and w0 are the same point
      //  return std::make_pair(v0, w0);

      CoordType v_in_dir_distance = viennagrid::inner_prod(dir_v, dir_distance);
      CoordType w_in_dir_distance = viennagrid::inner_prod(dir_w, dir_distance);

      CoordType s = (v_in_w * w_in_dir_distance - w_in_w * v_in_dir_distance) / denominator;
      CoordType t = (v_in_v * w_in_dir_distance - v_in_w * v_in_dir_distance) / denominator;
      //std::cout << "s = " << s << std::endl;
      //std::cout << "t = " << s << std::endl;

      //Check whether minimizing distance is within line segment, i.e. s,t \in [0,1]
      if ( (s < 0) || (s > 1) || (t < 0) || (t > 1) ) //Note: A more fine-grained check is possible for those looking for optimizations
        return point_pair_with_shortest_distance(closest_points_point_line(w0, v0, v1),
                                                 closest_points_point_line(w1, v0, v1),
                                                 closest_points_point_line(v0, w0, w1),
                                                 closest_points_point_line(v1, w0, w1));

      // compute points on V(s) an W(t) for which distance is smallest:
      PointType min_dist_point_V = v0 + s * dir_v;
      PointType min_dist_point_W = w0 + t * dir_w;

      return std::make_pair(min_dist_point_V, min_dist_point_W);
    }


    //convenience overload: simplex line and simplex line
    template <typename PointAccessorType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair<typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                element_t<simplex_tag<1>,WrappedConfigType1> const & line0,
                                element_t<simplex_tag<1>,WrappedConfigType2> const & line1)
    {
      return closest_points_line_line(accessor(vertices(line0)[0]),
                                      accessor(vertices(line0)[1]),
                                      accessor(vertices(line1)[0]),
                                      accessor(vertices(line1)[1]));
    }

    //convenience overload: hypercube line and simplex line
    template <typename PointAccessorType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair<typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                element_t<hypercube_tag<1>,WrappedConfigType1> const & line0,
                                element_t<simplex_tag<1>,WrappedConfigType2> const & line1)
    {
      return closest_points_line_line(accessor(vertices(line0)[0]),
                                      accessor(vertices(line0)[1]),
                                      accessor(vertices(line1)[0]),
                                      accessor(vertices(line1)[1]));
    }

    //convenience overload: simplex line and hypercube line
    template <typename PointAccessorType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair<typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                element_t<simplex_tag<1>,WrappedConfigType1> const & line0,
                                element_t<hypercube_tag<1>,WrappedConfigType2> const & line1)
    {
      return closest_points_line_line(accessor(vertices(line0)[0]),
                                      accessor(vertices(line0)[1]),
                                      accessor(vertices(line1)[0]),
                                      accessor(vertices(line1)[1]));
    }

    //convenience overload: hypercube line and hypercube line
    template <typename PointAccessorType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair<typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                element_t<hypercube_tag<1>,WrappedConfigType1> const & line0,
                                element_t<hypercube_tag<1>,WrappedConfigType2> const & line1)
    {
      return closest_points_line_line(accessor(vertices(line0)[0]),
                                      accessor(vertices(line0)[1]),
                                      accessor(vertices(line1)[0]),
                                      accessor(vertices(line1)[1]));
    }


    //
    // Distance between point and triangle
    //

    // Implementation: Supposed to work for arbitrary dimensions
    // Projects p onto the plane spanned by the triangle, then computes the shortest distance in the plane and uses Pythagoras for the full distance
    template <typename PointType>
    std::pair<PointType,PointType>
            closest_points_point_triangle(PointType const & p,
                                          PointType const & v0,
                                          PointType const & v1,
                                          PointType const & v2) //endpoints of second line
    {
      typedef typename result_of::coord< PointType >::type CoordType;

      // write T(s) =  v0 + s * (v1 - v0) + t * (v2 - v0), {s,t} \in [0,1], s+t < 1 for the triangle T
      //            =: v0 + s * u0 + t * u1
      //
      // Projection p' of p is given by the solution of the system
      //
      //  <u0, u0> * s + <u1, u0> * t = <u, u0>
      //  <u0, u1> * s + <u1, u1> * t = <u, u1>,
      //
      // where u = p - v0. This is a 2x2-system that is directly inverted.
      //
      // Write
      //
      // A = ( <u0, u0>   <u1, u0> ) = ( a  b )
      //     ( <u0, u1>   <u1, u1> )   ( c  d )
      //
      // and use b = c because of symmetry of the inner product.

      PointType u0 = v1 - v0;
      PointType u1 = v2 - v0;
      PointType u  = p  - v0;

      CoordType a = viennagrid::inner_prod(u0, u0);
      CoordType b = viennagrid::inner_prod(u0, u1);
      CoordType d = viennagrid::inner_prod(u1, u1);
      CoordType u_in_u0 = viennagrid::inner_prod(u, u0);
      CoordType u_in_u1 = viennagrid::inner_prod(u, u1);

      CoordType denominator = a * d - b * b;

      if (denominator < 1e-6 * a * d) //triangle is VERY thin: TODO: NUMERIC!!! 1e-6
      {
        std::cerr << "ViennaGrid: Warning: Strongly degenerated triangle detected: " << std::endl
                  << "vertex 0: " << v0 << std::endl
                  << "vertex 1: " << v1 << std::endl
                  << "vertex 2: " << v2 << std::endl;
      }

      CoordType s = (  d * u_in_u0 - b * u_in_u1) / denominator;
      CoordType t = (- b * u_in_u0 + a * u_in_u1) / denominator;

      PointType p_prime = v0 + s * u0 + t * u1;  //projection of p onto triangular plane

      // nonzero distance is encountered only if p_prime is outside the triangle
      if (    (s < 0 || s > 1)
           || (t < 0 || t > 1)
           || (s + t > 1) )     //p_prime is outside the triangle
      {
        // safe mode: Compute distances to all edges. Can be optimized further by using information from s, t, etc.
        return point_pair_with_shortest_distance(std::make_pair(p, closest_points_point_line(p_prime, v0, v1).second),
                                                 std::make_pair(p, closest_points_point_line(p_prime, v0, v2).second),
                                                 std::make_pair(p, closest_points_point_line(p_prime, v1, v2).second));
      }

      return std::make_pair(p, p_prime);
    }


    //convenience overload: point
    template <typename PointAccessorType, typename CoordType, typename CoordinateSystem, typename WrappedConfigType>
    std::pair<point_t<CoordType, CoordinateSystem>, typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                point_t<CoordType, CoordinateSystem> const & p,
                                element_t<simplex_tag<2>,WrappedConfigType> const & el)
    {
      return closest_points_point_triangle(p,
                                           accessor(vertices(el)[0]),
                                           accessor(vertices(el)[1]),
                                           accessor(vertices(el)[2]));
    }

    //convenience overload: vertex
    template <typename PointAccessorType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair<typename viennagrid::result_of::point<PointAccessorType>::type,typename viennagrid::result_of::point<PointAccessorType>::type>
    closest_points_impl(PointAccessorType const accessor,
                        element_t<vertex_tag,WrappedConfigType1> const & v,
                        element_t<simplex_tag<2>,WrappedConfigType2> const & el)
    {
      return closest_points_impl( accessor(v), el );
    }





    //
    // Distance between point and quadrilateral (using decomposition into two triangles)
    //

    //convenience overload: point
    //
    // Keep reference orientation in mind:
    //
    //   v[2]        v[3]
    //    * --------- *
    //    |   \       |
    //    |       \   |
    //    * --------- *
    //   v[0]        v[1]
    //
    template <typename PointAccessorType, typename CoordType, typename CoordinateSystem, typename WrappedConfigType>
    std::pair<point_t<CoordType, CoordinateSystem>, typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                point_t<CoordType, CoordinateSystem> const & p,
                                element_t<hypercube_tag<2>,WrappedConfigType> const & el)
    {
      return point_pair_with_shortest_distance(closest_points_point_triangle(p,
                                                                             accessor(vertices(el)[0]),
                                                                             accessor(vertices(el)[1]),
                                                                             accessor(vertices(el)[2])),
                                               closest_points_point_triangle(p,
                                                                             accessor(vertices(el)[1]),
                                                                             accessor(vertices(el)[3]),
                                                                             accessor(vertices(el)[2]))
                                              );
    }

    //convenience overload: vertex
    template <typename PointAccessorType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair<typename viennagrid::result_of::point<PointAccessorType>::type,typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                element_t<vertex_tag,WrappedConfigType1> const & v,
                                element_t<hypercube_tag<2>,WrappedConfigType2> const & el)
    {
      return closest_points_impl( accessor(v), el);
    }



    //
    // Distance between point and tetrahedron
    //

    // Implementation: Supposed to work for arbitrary geometric dimensions
    // Projects p onto the plane spanned by the triangle, then computes the shortest distance in the plane and uses Pythagoras for the full distance
    template <typename PointType>
    std::pair<PointType,PointType>
            closest_points_point_tetrahedron(PointType const & p,
                                             PointType const & v0,
                                             PointType const & v1,
                                             PointType const & v2,
                                             PointType const & v3)
    {
      typedef typename result_of::coord< PointType >::type CoordType;

      // write T(s) =  v0 + r * (v1 - v0) + s * (v2 - v0) + t * (v3 - v1), {r,s,t} \in [0,1], r+s+t < 1 for the tetrahedron T
      //            =: v0 + r * u0 + s * u1 + t * u2
      //
      // Projection p' of p is given by the solution of the system
      //
      //  <u0, u0> * r + <u1, u0> * s + <u2, u0> * t = <u, u0>
      //  <u0, u1> * r + <u1, u1> * s + <u2, u1> * t = <u, u1>
      //  <u0, u2> * r + <u1, u2> * s + <u2, u2> * t = <u, u2>
      //
      // where u = p - v0. This is a 3x3-system that is directly inverted using Cramer's rule.
      //

      PointType u0 = v1 - v0;
      PointType u1 = v2 - v0;
      PointType u2 = v3 - v0;
      PointType u  = p  - v0;

      CoordType a_00 = viennagrid::inner_prod(u0, u0);
      CoordType a_01 = viennagrid::inner_prod(u1, u0);
      CoordType a_02 = viennagrid::inner_prod(u2, u0);

      CoordType a_10 = a_01;
      CoordType a_11 = viennagrid::inner_prod(u1, u1);
      CoordType a_12 = viennagrid::inner_prod(u2, u1);

      CoordType a_20 = a_02;
      CoordType a_21 = a_12;
      CoordType a_22 = viennagrid::inner_prod(u2, u2);

      CoordType u_in_u0 = viennagrid::inner_prod(u, u0);
      CoordType u_in_u1 = viennagrid::inner_prod(u, u1);
      CoordType u_in_u2 = viennagrid::inner_prod(u, u2);

      CoordType det_A =  a_00 * a_11 * a_22 + a_01 * a_12 * a_20 + a_02 * a_10 * a_21
                       - a_20 * a_11 * a_02 - a_21 * a_12 * a_00 - a_22 * a_10 * a_01;

      if (det_A < 1e-6 * std::sqrt(a_00 * a_11 * a_22)) //tetrahedron is VERY thin:
      {
        std::cerr << "ViennaGrid: Warning: Strongly degenerated tetrahedron detected: " << std::endl
                  << "vertex 0: " << v0 << std::endl
                  << "vertex 1: " << v1 << std::endl
                  << "vertex 2: " << v2 << std::endl
                  << "vertex 3: " << v3 << std::endl;
      }

      //     | <u, u0>   a_01   a_02  |
      // r = | <u, u1>   a_11   a_12  |  / det_A  and similarly for s and t
      //     | <u, u2>   a_21   a_22  |
      CoordType r = (  u_in_u0 * a_11 * a_22 + a_01 * a_12 * u_in_u2 + a_02 * u_in_u1 * a_21
                     - u_in_u2 * a_11 * a_02 - a_21 * a_12 * u_in_u0 - a_22 * u_in_u1 * a_01 ) / det_A;
      CoordType s = (  a_00 * u_in_u1 * a_22 + u_in_u0 * a_12 * a_20 + a_02 * a_10 * u_in_u2
                     - a_20 * u_in_u1 * a_02 - u_in_u2 * a_12 * a_00 - a_22 * a_10 * u_in_u0 ) / det_A;
      CoordType t = (  a_00 * a_11 * u_in_u2 + a_01 * u_in_u1 * a_20 + u_in_u0 * a_10 * a_21
                     - a_20 * a_11 * u_in_u0 - a_21 * u_in_u1 * a_00 - u_in_u2 * a_10 * a_01 ) / det_A;

      PointType p_prime = v0 + r * u0 + s * u1 + t * u2;  //projection of p onto triangular plane

      // nonzero distance is encountered only if p_prime is outside the triangle
      if (    (r < 0 || r > 1)
           || (s < 0 || s > 1)
           || (t < 0 || t > 1)
           || (r + s + t > 1) )     //p_prime is outside the triangle
      {
        // safe mode: Compute distances to all edges. Can be optimized further by using information from s, t, etc.
        return point_pair_with_shortest_distance(std::make_pair(p, closest_points_point_triangle(p_prime, v0, v1, v2).second),
                                                 std::make_pair(p, closest_points_point_triangle(p_prime, v0, v1, v3).second),
                                                 std::make_pair(p, closest_points_point_triangle(p_prime, v0, v2, v3).second),
                                                 std::make_pair(p, closest_points_point_triangle(p_prime, v1, v2, v3).second));
      }

      return std::make_pair(p, p_prime);
    }


    //convenience overload: point
    template <typename PointAccessorType, typename CoordType, typename CoordinateSystem, typename WrappedConfigType>
    std::pair<point_t<CoordType, CoordinateSystem>,typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                point_t<CoordType, CoordinateSystem> const & p,
                                element_t<simplex_tag<3>,WrappedConfigType> const & el)
    {
      return closest_points_point_tetrahedron(p,
                                              accessor(vertices(el)[0]),
                                              accessor(vertices(el)[1]),
                                              accessor(vertices(el)[2]),
                                              accessor(vertices(el)[3]));
    }

    //convenience overload: vertex
    template <typename PointAccessorType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair<typename viennagrid::result_of::point<PointAccessorType>::type,typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                element_t<vertex_tag,WrappedConfigType1> const & v,
                                element_t<simplex_tag<3>,WrappedConfigType2> const & el)
    {
      return closest_points_impl( accessor(v), el);
    }



    //
    // Distance between point and hexahedron (using decomposition into six tetrahedra)
    //

    //convenience overload: point
    template <typename PointAccessorType, typename PointType, typename WrappedConfigType>
    std::pair<PointType,typename viennagrid::result_of::point<PointAccessorType>::type>
            closest_points_impl(PointAccessorType const accessor,
                                PointType const & p,
                                element_t<hypercube_tag<3>,WrappedConfigType> const & el)
    {
      return point_pair_with_shortest_distance(closest_points_point_tetrahedron(p,
                                                                                accessor(vertices(el)[0]),
                                                                                accessor(vertices(el)[1]),
                                                                                accessor(vertices(el)[3]),
                                                                                accessor(vertices(el)[7])),
                                               closest_points_point_tetrahedron(p,
                                                                                accessor(vertices(el)[0]),
                                                                                accessor(vertices(el)[1]),
                                                                                accessor(vertices(el)[7]),
                                                                                accessor(vertices(el)[5])),
                                               closest_points_point_tetrahedron(p,
                                                                                accessor(vertices(el)[0]),
                                                                                accessor(vertices(el)[2]),
                                                                                accessor(vertices(el)[6]),
                                                                                accessor(vertices(el)[7])),
                                               closest_points_point_tetrahedron(p,
                                                                                accessor(vertices(el)[0]),
                                                                                accessor(vertices(el)[3]),
                                                                                accessor(vertices(el)[2]),
                                                                                accessor(vertices(el)[7])),
                                               closest_points_point_tetrahedron(p,
                                                                                accessor(vertices(el)[0]),
                                                                                accessor(vertices(el)[5]),
                                                                                accessor(vertices(el)[7]),
                                                                                accessor(vertices(el)[4])),
                                               closest_points_point_tetrahedron(p,
                                                                                accessor(vertices(el)[0]),
                                                                                accessor(vertices(el)[6]),
                                                                                accessor(vertices(el)[4]),
                                                                                accessor(vertices(el)[7]))
                                              );
    }

    //convenience overload: vertex
    template <typename PointAccessorType, typename WrappedConfigType1, typename WrappedConfigType2>
    std::pair<typename viennagrid::result_of::point<PointAccessorType>::type,typename viennagrid::result_of::point<PointAccessorType>::type>
    closest_points_impl(PointAccessorType const accessor,
                        element_t<vertex_tag,WrappedConfigType1> const & v,
                        element_t<hypercube_tag<3>,WrappedConfigType2> const & el)
    {
      return closest_points_impl( accessor(v), el);
    }

    //
    /////////////////////// closest_points_on_boundary ////////////////////
    //

    //
    // Closest points between points (trivial)
    //

    // Closest points between two points (overloads for vertices follow):
    template <typename PointType>
    std::pair<PointType, PointType>
    closest_points_on_boundary_impl(PointType const & p1,
                        PointType const & p2)
    {
      return std::make_pair(p1, p2);
    }

    template <typename PointAccessorType, typename CoordType1, typename CoordinateSystem1, typename CoordType2, typename CoordinateSystem2>
    std::pair< point_t<CoordType1, CoordinateSystem1>, point_t<CoordType1, CoordinateSystem1> >
    closest_points_on_boundary_impl(PointAccessorType const,
                                    point_t<CoordType1, CoordinateSystem1> const & p1,
                                    point_t<CoordType2, CoordinateSystem2> const & p2)
    {
      return closest_points_on_boundary_impl(p1, p2);
    }

    template <typename PointAccessorType, typename PointType, typename WrappedConfigType>
    std::pair<PointType, typename viennagrid::result_of::point<element_t< vertex_tag,WrappedConfigType> >::type>
    closest_points_on_boundary_impl(PointAccessorType const accessor,
                                    PointType const & p1,
                                    element_t<vertex_tag,WrappedConfigType> const & v2)
    {
      return std::make_pair(p1, accessor(v2));
    }

    template <typename PointAccessorType, typename PointType, typename WrappedConfigType>
    std::pair<typename viennagrid::result_of::point<element_t< vertex_tag,WrappedConfigType> >::type, PointType>
    closest_points_on_boundary_impl(PointAccessorType const accessor,
                                    element_t<vertex_tag,WrappedConfigType> const & v1,
                                    PointType const & p2)
    {
      return std::make_pair(accessor(v1), p2);
    }

    // Closest points between vertices:
    template <typename PointAccessorType, typename PointType, typename WrappedConfigType>
    std::pair<typename viennagrid::result_of::point<element_t< vertex_tag,WrappedConfigType> >::type, typename viennagrid::result_of::point<element_t< vertex_tag,WrappedConfigType> >::type>
    closest_points_on_boundary_impl(PointAccessorType const accessor,
                                    element_t<vertex_tag,WrappedConfigType> const & v1,
                                    element_t<vertex_tag,WrappedConfigType> const & v2)
    {
      return std::make_pair( accessor(v1), accessor(v2) );
    }

    ////////////////// Distance from point to container ////////////////////

    /** @tparam ContainerType   Any topological object (ncell, segment, domain) */
    template <typename FacetTypeOrTag, typename PointAccessorType, typename PointType, typename Something>
    std::pair<PointType, typename viennagrid::result_of::point<PointAccessorType>::type>
    closest_points_on_boundary_point_to_any(PointAccessorType const accessor,
                                            PointType const & p,
                                            Something const & cont)
    {
      typedef typename result_of::element_tag<FacetTypeOrTag>::type FacetTag;
      typedef std::pair<PointType, typename viennagrid::result_of::point<Something>::type>       PairType;

      typedef typename viennagrid::result_of::const_element_range<Something, FacetTag>::type           FacetRange;
      typedef typename viennagrid::result_of::iterator<FacetRange>::type                  FacetIterator;

      PairType closest_pair;
      double shortest_distance = std::numeric_limits<double>::max();

      FacetRange facets = viennagrid::elements(cont);
      for (FacetIterator fit = facets.begin();
                         fit != facets.end();
                       ++fit)
      {
        if (!is_boundary<FacetTag>(cont, *fit))
          continue;

        PairType pair = closest_points_impl(accessor, p, *fit);
        double cur_norm = norm_2(pair.first - pair.second);
        if (cur_norm < shortest_distance)
        {
          closest_pair = pair;
          shortest_distance = cur_norm;
        }
      }

      return closest_pair;
    }



    template <typename PointAccessorType, typename PointType, typename ElementTag, typename WrappedConfigType>
    std::pair<PointType, typename viennagrid::result_of::point<PointAccessorType>::type>
    closest_points_on_boundary_impl(PointAccessorType const & domain,
                                    PointType const & v,
                                    element_t<ElementTag,WrappedConfigType> const & el)
    {
      return closest_points_on_boundary_point_to_any<typename ElementTag::facet_tag>(domain, v, el);
    }

    template <typename WrappedConfigType, typename PointType>
    std::pair<PointType, typename viennagrid::result_of::point< domain_t<WrappedConfigType> >::type>
    closest_points_on_boundary_impl(domain_t<WrappedConfigType> const & domain,
                                    PointType const & p)
    {
        typedef typename result_of::cell_tag< domain_t<WrappedConfigType> >::type CellTag;
      return closest_points_on_boundary_point_to_any<typename CellTag::facet_tag>( default_point_accessor(domain), p, domain );
    }



    template <typename PointAccessorType,  typename WrappedConfigType1, typename EA2, typename WrappedConfigType2>
    std::pair<typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type>
    closest_points_on_boundary_impl(PointAccessorType const accessor,
                                    element_t<vertex_tag,WrappedConfigType1> const & v,
                                    element_t<EA2,WrappedConfigType2> const & el)
    {
      return closest_points_on_boundary_impl( accessor(v), el);
    }

    template <typename WrappedDomainConfigType, typename WrappedConfigType>
    std::pair< typename viennagrid::result_of::point< domain_t<WrappedDomainConfigType> >::type, typename viennagrid::result_of::point< domain_t<WrappedDomainConfigType> >::type >
    closest_points_on_boundary_impl(domain_t<WrappedDomainConfigType> const & domain,
                                    element_t<vertex_tag,WrappedConfigType> const & v)
    {
      return closest_points_on_boundary_impl( default_point_accessor(domain)(v), domain);
    }


    ////////// Distance from Container to Container /////////////////////

    template <typename PointAccessorType, typename Something1, typename Something2>
    std::pair< typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type >
    closest_points_on_boundary_generic(PointAccessorType const accessor,
                                       Something1 const & el1,
                                       Something2 const & el2)
    {


      typedef typename result_of::facet_tag<Something1>::type FacetTag1;
      typedef typename result_of::facet_tag<Something2>::type FacetTag2;


      typedef std::pair< typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type > PairType;

      typedef typename viennagrid::result_of::const_element_range<Something1, FacetTag1>::type           FacetRange1;
      typedef typename viennagrid::result_of::iterator<FacetRange1>::type                   FacetIterator1;

      typedef typename viennagrid::result_of::const_element_range<Something2, FacetTag2>::type           FacetRange2;
      typedef typename viennagrid::result_of::iterator<FacetRange2>::type                   FacetIterator2;

      PairType closest_pair;
      double shortest_distance = std::numeric_limits<double>::max();


      FacetRange1 facets1 = viennagrid::elements(el1);
      FacetRange2 facets2 = viennagrid::elements(el2);

      for (FacetIterator1 fit1 = facets1.begin();
                          fit1 != facets1.end();
                        ++fit1)
      {
        if (!is_boundary(*fit1, el1))
          continue;

        for (FacetIterator2 fit2 = facets2.begin();
                            fit2 != facets2.end();
                          ++fit2)
        {
          if (!is_boundary(*fit2, el2))
            continue;

          PairType p = closest_points_impl(accessor, *fit1, *fit2);
          double cur_norm = norm_2(p.first - p.second);
          if (cur_norm < shortest_distance)
          {
            closest_pair = p;
            shortest_distance = cur_norm;
          }
        }
      }

      return closest_pair;
    }


    template <typename PointAccessorType,
              typename ElementTag1, typename WrappedConfigType1,
              typename ElementTag2, typename WrappedConfigType2>
    std::pair< typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type >
    closest_points_on_boundary_impl(PointAccessorType const accessor,
                                    element_t<ElementTag1,WrappedConfigType1> const & el1,
                                    element_t<ElementTag2,WrappedConfigType2> const & el2)
    {
      return closest_points_on_boundary_generic(accessor, el1, el2);
    }

    template <typename PointAccessorType,
              typename WrappedDomainConfigType,
              typename ElementTag, typename WrappedConfigType>
    std::pair< typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type >
    closest_points_on_boundary_impl(PointAccessorType const accessor,
                                    domain_t<WrappedDomainConfigType> const & domain,
                                    element_t<ElementTag,WrappedConfigType> const & el1)
    {
      return closest_points_on_boundary_generic(accessor, domain, el1);
    }


    template <typename PointAccessorType,
              typename WrappedDomainConfigType1,
              typename WrappedDomainConfigType2>
    std::pair< typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type >
    closest_points_on_boundary_impl(PointAccessorType const accessor,
                                    domain_t<WrappedDomainConfigType1> const & domain1,
                                    domain_t<WrappedDomainConfigType2> const & domain2)
    {
      return closest_points_on_boundary_generic(accessor, domain1, domain2);
    }



    //////////////////////////// other helpers ////////////////////////

    template <typename T>
    struct topological_id
    {
        //by default, typename is unknown, thus force error by not defining 'value'
    };

    template <typename CoordType, typename CoordinateSystem>
    struct topological_id< point_t<CoordType, CoordinateSystem> >
    {
      enum { value = 1 };
    };


    template <long dim, typename WrappedConfigType>
    struct topological_id< element_t<simplex_tag<dim>, WrappedConfigType> >
    {
      enum { value = 10000 + dim }; //10.000 dimensions are certainly far from being ever instantiated
    };

    template <long dim, typename WrappedConfigType>
    struct topological_id< element_t<hypercube_tag<dim>, WrappedConfigType> >
    {
      enum { value = 20000 + dim };
    };

    template <typename WrappedConfigType>
    struct topological_id< domain_t<WrappedConfigType> >
    {
      enum { value = 100000 };
    };



    template <typename T, typename U>
    struct topologically_sorted
    {
      enum { value = (topological_id<T>::value <= topological_id<U>::value) ? true : false };
    };


    template <typename T,
              typename U,
              bool correct_order = topologically_sorted<T, U>::value >
    struct ascending_topological_order
    {
      static T const & first(T const & t, U const &) { return t; }
      static U const & second(T const &, U const & u) { return u; }
    };

    template <typename T,
              typename U>
    struct ascending_topological_order<T, U, false>
    {
      static U const & first(T const &, U const & u) { return u; }
      static T const & second(T const & t, U const &) { return t; }
    };

  } //namespace detail

  //
  // The public interface functions
  //
  /** @brief Returns the distance between n-cells */
  template <typename PointAccessorType, typename Something1, typename Something2>
  std::pair< typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type >
  closest_points(PointAccessorType const accessor,
                 Something1 const & el1,
                 Something2 const & el2)
  {
    return detail::closest_points_impl(accessor,
                                       detail::ascending_topological_order<Something1, Something2>::first(el1, el2),
                                       detail::ascending_topological_order<Something1, Something2>::second(el1, el2));
  }

  template <typename Something1, typename Something2>
  std::pair< typename viennagrid::result_of::point<Something1>::type, typename viennagrid::result_of::point<Something1>::type >
  closest_points(Something1 const & el1,
                 Something2 const & el2)
  {
    return closest_points_impl(default_point_accessor(el1),
                                       detail::ascending_topological_order<Something1, Something2>::first(el1, el2),
                                       detail::ascending_topological_order<Something1, Something2>::second(el1, el2));
  }



  template <typename PointAccessorType, typename Something1, typename Something2>
  std::pair< typename viennagrid::result_of::point<PointAccessorType>::type, typename viennagrid::result_of::point<PointAccessorType>::type >
  closest_points_on_boundary(PointAccessorType const accessor,
                 Something1 const & el1,
                 Something2 const & el2)
  {
    return detail::closest_points_on_boundary_impl(accessor,
                                       detail::ascending_topological_order<Something1, Something2>::first(el1, el2),
                                       detail::ascending_topological_order<Something1, Something2>::second(el1, el2));
  }

  /** @brief Returns the distance between n-cells */
  template <typename Something1, typename Something2>
  std::pair< typename viennagrid::result_of::point<Something1>::type, typename viennagrid::result_of::point<Something1>::type >
  closest_points_on_boundary(Something1 const & el1,
                             Something2 const & el2)
  {
    return closest_points_on_boundary_impl(default_point_accessor(el1),
                                       detail::ascending_topological_order<Something1, Something2>::first(el1, el2),
                                       detail::ascending_topological_order<Something1, Something2>::second(el1, el2));
  }


} //namespace viennagrid
#endif
