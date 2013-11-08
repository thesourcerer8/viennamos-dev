#ifndef VIENNAGRID_ELEMENT_CREATION_HPP
#define VIENNAGRID_ELEMENT_CREATION_HPP

/* =======================================================================
   Copyright (c) 2011-2013, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                     ViennaGrid - The Vienna Grid Library
                            -----------------

   License:      MIT (X11), see file LICENSE in the base directory
======================================================================= */

#include "viennagrid/mesh/mesh.hpp"
#include "viennagrid/mesh/segmentation.hpp"
#include "viennagrid/topology/plc.hpp"
#include "viennagrid/algorithm/norm.hpp"

/** @file viennagrid/mesh/element_creation.hpp
    @brief Contains functions for creating elements within a mesh or segment
*/

namespace viennagrid
{
  // doxygen doku in forwards.hpp
  template<typename ElementTypeOrTagT, typename MeshOrSegmentHandleTypeT, typename VertexHandleIteratorT>
  typename result_of::handle<MeshOrSegmentHandleTypeT, ElementTypeOrTagT>::type
  make_element(
        MeshOrSegmentHandleTypeT & mesh_obj,
        VertexHandleIteratorT vertices_begin,
        VertexHandleIteratorT const & vertices_end)
  {
    typedef typename viennagrid::result_of::element<MeshOrSegmentHandleTypeT, ElementTypeOrTagT>::type ElementType;
    ElementType element = ElementType( detail::inserter(mesh_obj).get_physical_container_collection() );

    size_t element_index = 0;
    for ( ; vertices_begin != vertices_end; ++vertices_begin, ++element_index )
        viennagrid::set_vertex( element, *vertices_begin, element_index );

    return detail::push_element<true, true>(mesh_obj, element).first;
  }

  // doxygen doku in forwards.hpp
  template<typename ElementTypeOrTagT, typename MeshOrSegmentHandleTypeT, typename VertexHandleIteratorT, typename IDT>
  typename result_of::handle<MeshOrSegmentHandleTypeT, ElementTypeOrTagT>::type
  make_element_with_id(
        MeshOrSegmentHandleTypeT & mesh_obj,
        VertexHandleIteratorT vertices_begin,
        VertexHandleIteratorT const & vertices_end,
        IDT id)
  {
    typedef typename viennagrid::result_of::element<MeshOrSegmentHandleTypeT, ElementTypeOrTagT>::type ElementType;
    ElementType element = ElementType( detail::inserter(mesh_obj).get_physical_container_collection() );

    element.id( id );

    size_t element_index = 0;
    for ( ; vertices_begin != vertices_end; ++vertices_begin, ++element_index )
        viennagrid::set_vertex( element, *vertices_begin, element_index );

    return detail::push_element<false, true>(mesh_obj, element ).first;
  }



  /** @brief Function for creating an cell within a mesh or a segment
  *
  * @param  mesh_segment            The mesh or segment object where the cell should be created
  * @param  vertices_begin          An iterator pointing to the first vertex handle of the cell
  * @param  vertices_end            An iterator defining the end of the vertices
  * @return                         A handle to the created cell
  */
  template<typename MeshOrSegmentHandleTypeT, typename VertexHandleIteratorT>
  typename result_of::cell_handle<MeshOrSegmentHandleTypeT>::type
  make_cell(
        MeshOrSegmentHandleTypeT & mesh_segment,
        VertexHandleIteratorT vertices_begin,
        VertexHandleIteratorT const & vertices_end)
  {
    typedef typename viennagrid::result_of::cell_tag<MeshOrSegmentHandleTypeT>::type CellTagType;
    return make_element<CellTagType>( mesh_segment, vertices_begin, vertices_end );
  }


  /** @brief Function for creating an cell with ID within a mesh or a segment
  *
  * @param  mesh_segment            The mesh or segment object where the cell should be created
  * @param  vertices_begin          An iterator pointing to the first vertex handle of the cell
  * @param  vertices_end            An iterator defining the end of the vertices
  * @param  id                      The cell ID to be used for the cell within the mesh or segment
  * @return                         A handle to the created cell
  */
  template<typename MeshOrSegmentHandleTypeT, typename VertexHandleIteratorT>
  typename result_of::cell_handle<MeshOrSegmentHandleTypeT>::type
  make_cell_with_id(
        MeshOrSegmentHandleTypeT & mesh_segment,
        VertexHandleIteratorT vertices_begin,
        VertexHandleIteratorT const & vertices_end,
        typename viennagrid::result_of::cell<MeshOrSegmentHandleTypeT>::type::id_type id)
  {
    typedef typename viennagrid::result_of::cell_tag<MeshOrSegmentHandleTypeT>::type CellTagType;
    return make_element_with_id<CellTagType>( mesh_segment, vertices_begin, vertices_end, id );
  }






  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT>
  typename result_of::vertex_handle<MeshOrSegmentHandleTypeT>::type make_vertex(MeshOrSegmentHandleTypeT & mesh_segment)
  {
    typedef typename result_of::element<MeshOrSegmentHandleTypeT, vertex_tag>::type element_type;
    return detail::push_element<true, true>(mesh_segment, element_type() ).first;
  }

  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT>
  typename result_of::vertex_handle<MeshOrSegmentHandleTypeT>::type make_vertex(
        MeshOrSegmentHandleTypeT & mesh_obj,
        typename result_of::point<MeshOrSegmentHandleTypeT>::type const & point)
  {
    typename viennagrid::result_of::vertex_handle<MeshOrSegmentHandleTypeT>::type vtx_handle = make_vertex(mesh_obj);
    viennagrid::point(mesh_obj, vtx_handle) = point;
    return vtx_handle;
  }

  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT>
  typename result_of::vertex_handle<MeshOrSegmentHandleTypeT>::type make_vertex_with_id(
        MeshOrSegmentHandleTypeT & mesh_obj,
        typename viennagrid::result_of::element<MeshOrSegmentHandleTypeT, vertex_tag>::type::id_type id,
        typename result_of::point<MeshOrSegmentHandleTypeT>::type const & point)
  {
    typedef typename result_of::vertex<MeshOrSegmentHandleTypeT>::type VertexType;
    VertexType element;
    element.id( id );

    typename result_of::vertex_handle<MeshOrSegmentHandleTypeT>::type ret = detail::push_element<false, true>(mesh_obj, element ).first;
    viennagrid::point(mesh_obj, ret) = point;

    return ret;
  }

  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT>
  typename result_of::vertex_handle<MeshOrSegmentHandleTypeT>::type make_unique_vertex(
        MeshOrSegmentHandleTypeT & mesh_obj,
        typename result_of::point<MeshOrSegmentHandleTypeT>::type const & point,
        typename result_of::coord<MeshOrSegmentHandleTypeT>::type tolerance)
  {
    typedef typename result_of::element_range<MeshOrSegmentHandleTypeT, vertex_tag>::type vertex_range_type;
    typedef typename result_of::iterator<vertex_range_type>::type vertex_range_iterator;

    vertex_range_type vertices(mesh_obj);
    for (vertex_range_iterator hit = vertices.begin(); hit != vertices.end(); ++hit)
    {
        if (viennagrid::norm_2( point - viennagrid::point(mesh_obj, *hit) ) < tolerance )
            return hit.handle();
    }

    return make_vertex(mesh_obj, point);
  }

  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT>
  typename result_of::vertex_handle<MeshOrSegmentHandleTypeT>::type make_unique_vertex(
        MeshOrSegmentHandleTypeT & mesh_obj,
        typename result_of::point<MeshOrSegmentHandleTypeT>::type const & p)
  {
    return make_unique_vertex( mesh_obj, p, viennagrid::norm_2(p) / 1e6 );
  }


  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT, typename VertexHandleT>
  typename result_of::line_handle<MeshOrSegmentHandleTypeT>::type make_line(
        MeshOrSegmentHandleTypeT & mesh_obj,
        VertexHandleT v0, VertexHandleT v1)
  {
    viennagrid::static_array<VertexHandleT, 2> handles;
    handles[0] = v0;
    handles[1] = v1;

    return make_element<viennagrid::line_tag>( mesh_obj, handles.begin(), handles.end() );
  }

  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT, typename VertexHandleT>
  typename result_of::edge_handle<MeshOrSegmentHandleTypeT>::type make_edge(
        MeshOrSegmentHandleTypeT & mesh_obj,
        VertexHandleT v0, VertexHandleT v1)
  {
    viennagrid::static_array<VertexHandleT, 2> handles;
    handles[0] = v0;
    handles[1] = v1;

    return make_element<viennagrid::edge_tag>( mesh_obj, handles.begin(), handles.end() );
  }

  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT, typename VertexHandleT>
  typename result_of::triangle_handle<MeshOrSegmentHandleTypeT>::type make_triangle(
        MeshOrSegmentHandleTypeT & mesh_obj,
        VertexHandleT v0, VertexHandleT v1, VertexHandleT v2)
  {
    viennagrid::static_array<VertexHandleT, 3> handles;
    handles[0] = v0;
    handles[1] = v1;
    handles[2] = v2;

    return make_element<viennagrid::triangle_tag>( mesh_obj, handles.begin(), handles.end() );
  }

  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT, typename VertexHandleT>
  typename result_of::quadrilateral_handle<MeshOrSegmentHandleTypeT>::type make_quadrilateral(
        MeshOrSegmentHandleTypeT & mesh_obj,
        VertexHandleT v0, VertexHandleT v1, VertexHandleT v2, VertexHandleT v3)
  {
    viennagrid::static_array<VertexHandleT, 4> handles;
    handles[0] = v0;
    handles[1] = v1;
    handles[2] = v2;
    handles[3] = v3;

    return make_element<viennagrid::quadrilateral_tag>( mesh_obj, handles.begin(), handles.end() );
  }


  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT, typename LineHandleIteratorT, typename VertexHandleIteratorT, typename PointIteratorT>
  typename result_of::plc_handle<MeshOrSegmentHandleTypeT>::type make_plc(
        MeshOrSegmentHandleTypeT & mesh_obj,
        LineHandleIteratorT    lines_begin,           LineHandleIteratorT     lines_end,
        VertexHandleIteratorT  loose_vertices_begin,  VertexHandleIteratorT   loose_vertices_end,
        PointIteratorT         hole_points_begin,     PointIteratorT          hole_points_end)
  {
    typedef typename viennagrid::result_of::element<MeshOrSegmentHandleTypeT, plc_tag>::type PLCType;;
    typedef typename result_of::handle<MeshOrSegmentHandleTypeT, plc_tag>::type PLCHandleType;
    PLCType plc( detail::inserter(mesh_obj).get_physical_container_collection() );

    for ( ; lines_begin != lines_end; ++lines_begin)
      plc.container( viennagrid::line_tag() ).insert_unique_handle( *lines_begin );

    for ( ; loose_vertices_begin != loose_vertices_end; ++loose_vertices_begin)
      plc.container( viennagrid::vertex_tag() ).insert_unique_handle( *loose_vertices_begin );

    PLCHandleType handle = viennagrid::detail::push_element<true, true>(mesh_obj, plc ).first;

    PLCType & inserted_plc = viennagrid::dereference_handle(mesh_obj, handle);

    std::copy(hole_points_begin, hole_points_end, std::back_inserter( inserted_plc.appendix() ) );
    return handle;
  }






  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT, typename VertexHandleT>
  typename result_of::tetrahedron_handle<MeshOrSegmentHandleTypeT>::type make_tetrahedron(
        MeshOrSegmentHandleTypeT & mesh_obj,
        VertexHandleT v0, VertexHandleT v1, VertexHandleT v2, VertexHandleT v3)
  {
    viennagrid::static_array<VertexHandleT, 4> handles;
    handles[0] = v0;
    handles[1] = v1;
    handles[2] = v2;
    handles[3] = v3;

    return make_element<viennagrid::tetrahedron_tag>( mesh_obj, handles.begin(), handles.end() );
  }


  // doxygen doku in forwards.hpp
  template<typename MeshOrSegmentHandleTypeT, typename VertexHandleT>
  typename result_of::hexahedron_handle<MeshOrSegmentHandleTypeT>::type make_hexahedron(
        MeshOrSegmentHandleTypeT & mesh_obj,
        VertexHandleT v0, VertexHandleT v1, VertexHandleT v2, VertexHandleT v3,
        VertexHandleT v4, VertexHandleT v5, VertexHandleT v6, VertexHandleT v7)
  {
    viennagrid::static_array<VertexHandleT, 8> handles;
    handles[0] = v0;
    handles[1] = v1;
    handles[2] = v2;
    handles[3] = v3;
    handles[4] = v4;
    handles[5] = v5;
    handles[6] = v6;
    handles[7] = v7;

    return make_element<viennagrid::hexahedron_tag>( mesh_obj, handles.begin(), handles.end() );
  }


  // doxygen doku in forwards.hpp
  template<typename ElementT, typename DomainOrSegmentHandleT>
  typename viennagrid::result_of::handle<
      DomainOrSegmentHandleT,
      typename viennagrid::result_of::element_tag<ElementT>::type
    >::type copy_element( ElementT const & element, DomainOrSegmentHandleT & domain_segment )
  {
    typedef typename viennagrid::result_of::element_tag<ElementT>::type             ElementTag;
    typedef typename viennagrid::result_of::vertex_handle<DomainOrSegmentHandleT>::type   VertexHandleType;
    std::vector<VertexHandleType> vertex_handles;

    typedef typename viennagrid::result_of::const_vertex_range<ElementT>::type      VertexRangeType;
    typedef typename viennagrid::result_of::iterator<VertexRangeType>::type         VertexRangeIterator;

    VertexRangeType vertices(element);
    for (VertexRangeIterator vit = vertices.begin(); vit != vertices.end(); ++vit)
      vertex_handles.push_back( make_unique_vertex(domain_segment, viennagrid::point(*vit)) );

    return make_element<ElementTag>( domain_segment, vertex_handles.begin(), vertex_handles.end() );
  }

}



#endif
