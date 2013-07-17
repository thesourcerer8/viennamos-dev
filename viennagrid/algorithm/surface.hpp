#ifndef VIENNAGRID_ALGORITHM_SURFACE_HPP
#define VIENNAGRID_ALGORITHM_SURFACE_HPP

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

#include "viennagrid/forwards.hpp"
#include "viennagrid/algorithm/volume.hpp"
#include "viennagrid/algorithm/boundary.hpp"

/** @file surface.hpp
    @brief Computes the surface of different cell types as well as domains and segments
*/


namespace viennagrid
{
  namespace detail
  {
    /** @brief Implementation for the calculation of the surface of a domain or segment */
    template <typename ElementTypeOrTag, typename DomainType>
    typename viennagrid::result_of::coord< DomainType >::type
    surface_domainsegment(DomainType const & domain)
    {
      typedef typename viennagrid::result_of::const_element_range<DomainType, ElementTypeOrTag>::type  ElementRange;
      typedef typename viennagrid::result_of::iterator<ElementRange>::type         ElementIterator;

      typename viennagrid::result_of::coord<DomainType>::type result = 0;

      ElementRange facets = viennagrid::elements(domain);
      for (ElementIterator fit = facets.begin();
                         fit != facets.end();
                       ++fit)
      {
        if (is_boundary(domain, *fit))
          result += viennagrid::volume(*fit);
      }
      return result;
    }
  } //namespace detail



  //
  // The public interface functions
  //
  /** @brief Returns the surface of a n-cell */
  template <typename PointAccessorType, typename ElementTag, typename WrappedConfigType>
  typename viennagrid::result_of::coord< PointAccessorType >::type
  surface(PointAccessorType const accessor, element_t<ElementTag, WrappedConfigType> const & element)
  {
    typedef typename viennagrid::result_of::const_facet_range< element_t<ElementTag, WrappedConfigType> >::type   ElementBoundaryRange;
    typedef typename viennagrid::result_of::iterator<ElementBoundaryRange>::type       ElementBoundaryIterator;

    typedef typename viennagrid::result_of::coord< PointAccessorType >::type value_type;

    value_type result = 0;

    ElementBoundaryRange boundary = viennagrid::facets(element);
    for (ElementBoundaryIterator ebit = boundary.begin();
                                  ebit != boundary.end();
                                ++ebit)
    {
      result += viennagrid::volume(accessor, *ebit);
    }

    return result;
  }

  template < typename ElementTag, typename WrappedConfigType>
  typename viennagrid::result_of::coord< element_t<ElementTag, WrappedConfigType> >::type
  surface( element_t<ElementTag, WrappedConfigType> const & element)
  {
    return surface( default_point_accessor(element), element );
  }



  //special case: domain
  /** @brief Returns the surface of a domain */
  template <typename ElementTypeOrTag, typename WrappedConfigType>
  typename viennagrid::result_of::coord< domain_t<WrappedConfigType> >::type
  surface(domain_t<WrappedConfigType> const & d)
  {
    return detail::surface_domainsegment<ElementTypeOrTag>(d);
  }

  template <typename WrappedConfigType>
  typename viennagrid::result_of::coord< domain_t<WrappedConfigType> >::type
  surface(domain_t<WrappedConfigType> const & d)
  {
    typedef typename viennagrid::result_of::cell_tag< domain_t<WrappedConfigType> >::type CellTag;
    return detail::surface_domainsegment< typename viennagrid::result_of::facet_tag<CellTag>::type>(d);
  }

  template<typename SegmentationType>
  typename viennagrid::result_of::coord< typename SegmentationType::domain_type >::type
  surface(segment_t<SegmentationType> const & segment)
  {
    typedef typename viennagrid::result_of::cell_tag< typename SegmentationType::domain_type >::type CellTag;
    return detail::surface_domainsegment< typename viennagrid::result_of::facet_tag<CellTag>::type >(segment);
  }



    template<typename boundary_element_type_or_tag, typename domain_type, typename element_type>
    typename viennagrid::result_of::handle<domain_type, boundary_element_type_or_tag>::type smallest_boundary_volume( domain_type & domain, element_type & element )
    {
        typedef typename viennagrid::result_of::coord<domain_type>::type numeric_type;
        typedef typename viennagrid::result_of::handle<element_type, boundary_element_type_or_tag>::type boundary_element_handle;
        typedef typename viennagrid::result_of::element_range<element_type, boundary_element_type_or_tag>::type boundary_element_range_type;
        typedef typename viennagrid::result_of::iterator<boundary_element_range_type>::type boundary_element_range_iterator;

        boundary_element_range_type boundary_elements = viennagrid::elements( element );
        boundary_element_range_iterator it = boundary_elements.begin();

        boundary_element_handle best_handle = it.handle();
        numeric_type best_volume = viennagrid::volume( domain, *it );

        for (; it != boundary_elements.end(); ++it)
        {
            numeric_type volume = viennagrid::volume( domain, *it );
            if (volume < best_volume)
            {
                best_handle = it.handle();
                best_volume = volume;
            }
        }

        return best_handle;
    }

    template<typename boundary_element_type_or_tag, typename domain_type, typename element_type>
    typename viennagrid::result_of::const_handle<domain_type, boundary_element_type_or_tag>::type smallest_boundary_volume( domain_type const & domain, element_type const & element )
    {
        typedef typename viennagrid::result_of::coord<domain_type>::type numeric_type;
        typedef typename viennagrid::result_of::const_handle<element_type, boundary_element_type_or_tag>::type boundary_element_handle;
        typedef typename viennagrid::result_of::const_element_range<element_type, boundary_element_type_or_tag>::type boundary_element_range_type;
        typedef typename viennagrid::result_of::iterator<boundary_element_range_type>::type boundary_element_range_iterator;

        boundary_element_range_type boundary_elements = viennagrid::elements( element );
        boundary_element_range_iterator it = boundary_elements.begin();

        boundary_element_handle best_handle = it.handle();
        numeric_type best_volume = viennagrid::volume( domain, *it );

        for (; it != boundary_elements.end(); ++it)
        {
            numeric_type volume = viennagrid::volume( domain, *it );
            if (volume < best_volume)
            {
                best_handle = it.handle();
                best_volume = volume;
            }
        }

        return best_handle;
    }

    template<typename boundary_element_type_or_tag, typename domain_type, typename element_type>
    typename viennagrid::result_of::handle<domain_type, boundary_element_type_or_tag>::type largest_boundary_volume( domain_type & domain, element_type & element )
    {
        typedef typename viennagrid::result_of::coord<domain_type>::type numeric_type;
        typedef typename viennagrid::result_of::handle<element_type, boundary_element_type_or_tag>::type boundary_element_handle;
        typedef typename viennagrid::result_of::element_range<element_type, boundary_element_type_or_tag>::type boundary_element_range_type;
        typedef typename viennagrid::result_of::iterator<boundary_element_range_type>::type boundary_element_range_iterator;

        boundary_element_range_type boundary_elements = viennagrid::elements( element );
        boundary_element_range_iterator it = boundary_elements.begin();

        boundary_element_handle best_handle = it.handle();
        numeric_type best_volume = viennagrid::volume( domain, *it );

        for (; it != boundary_elements.end(); ++it)
        {
            numeric_type volume = viennagrid::volume( domain, *it );
            if (volume > best_volume)
            {
                best_handle = it.handle();
                best_volume = volume;
            }
        }

        return best_handle;
    }

    template<typename boundary_element_type_or_tag, typename domain_type, typename element_type>
    typename viennagrid::result_of::const_handle<domain_type, boundary_element_type_or_tag>::type largest_boundary_volume( domain_type const & domain, element_type const & element )
    {
        typedef typename viennagrid::result_of::coord<domain_type>::type numeric_type;
        typedef typename viennagrid::result_of::const_handle<element_type, boundary_element_type_or_tag>::type boundary_element_handle;
        typedef typename viennagrid::result_of::const_element_range<element_type, boundary_element_type_or_tag>::type boundary_element_range_type;
        typedef typename viennagrid::result_of::iterator<boundary_element_range_type>::type boundary_element_range_iterator;

        boundary_element_range_type boundary_elements = viennagrid::elements( element );
        boundary_element_range_iterator it = boundary_elements.begin();

        boundary_element_handle best_handle = it.handle();
        numeric_type best_volume = viennagrid::volume( domain, *it );

        for (; it != boundary_elements.end(); ++it)
        {
            numeric_type volume = viennagrid::volume( domain, *it );
            if (volume > best_volume)
            {
                best_handle = it.handle();
                best_volume = volume;
            }
        }

        return best_handle;
    }


} //namespace viennagrid
#endif
