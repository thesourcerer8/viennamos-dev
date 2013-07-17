#ifndef VIENNAGRID_DOMAIN_HPP
#define VIENNAGRID_DOMAIN_HPP

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

#include <typeinfo>
#include "viennagrid/forwards.hpp"

#include "viennagrid/meta/algorithm.hpp"

#include "viennagrid/storage/id_generator.hpp"
#include "viennagrid/storage/inserter.hpp"
#include "viennagrid/storage/algorithm.hpp"

#include "viennagrid/config/element_config.hpp"
#include "viennagrid/config/topology_config.hpp"

#include "viennagrid/element/element_view.hpp"

namespace viennagrid
{



    template<typename container_collection_type>
    class view_domain_setter
    {
    public:

        view_domain_setter(container_collection_type & domain_) : domain(domain_) {}

        template<typename container_type>
        void operator()( container_type & container )
        {
            typedef typename container_type::value_type value_type;
            container.set_base_container( viennagrid::storage::collection::get<value_type>(domain) );
        }


    private:
        container_collection_type & domain;
    };


    template<typename domain_type>
    struct domain_proxy
    {
        domain_proxy( domain_type & domain_ ) : domain(&domain_){}

        domain_type * domain;
    };


    template<typename container_type_, typename change_counter_type>
    struct coboundary_container_wrapper
    {
        typedef container_type_ container_type;
        coboundary_container_wrapper() : change_counter(0) {}

        change_counter_type change_counter;
        container_type container;
    };


    template<typename container_type_, typename change_counter_type>
    struct neighbour_container_wrapper
    {
        typedef container_type_ container_type;
        neighbour_container_wrapper() : change_counter(0) {}

        change_counter_type change_counter;
        container_type container;
    };

    template<typename container_type_, typename change_counter_type>
    struct boundary_information_wrapper
    {
        typedef container_type_ container_type;
        boundary_information_wrapper() : change_counter(0) {}

        change_counter_type change_counter;
        container_type container;
    };

    namespace result_of
    {
      template<typename element_typelist, typename container_typemap>
      struct filter_element_container;

      template<typename container_typemap>
      struct filter_element_container<viennagrid::meta::null_type, container_typemap>
      {
          typedef viennagrid::meta::null_type type;
      };

      template<typename element_type, typename tail, typename container_typemap>
      struct filter_element_container<viennagrid::meta::typelist_t< element_type, tail> , container_typemap>
      {
          typedef typename viennagrid::meta::typemap::result_of::find<container_typemap, element_type>::type result_type;

          typedef typename filter_element_container<tail, container_typemap>::type new_tail;

          typedef typename viennagrid::meta::IF<
              viennagrid::meta::EQUAL< result_type, viennagrid::meta::not_found >::value,
              new_tail,
              viennagrid::meta::typelist_t<
                  result_type,
                  new_tail
              >
          >::type type;
      };


      // domain_element_collection_type
      template <typename WrappedConfigType>
      struct domain_element_collection_type
      {
//         typedef typename WrappedConfigType::type   ConfigType;

        typedef typename config::result_of::element_collection< WrappedConfigType >::type   type;
      };

      template <typename WrappedDomainConfigType, typename ElementTypeList, typename ContainerConfig>
      struct domain_element_collection_type< decorated_domain_view_config<WrappedDomainConfigType, ElementTypeList, ContainerConfig> >
      {
        typedef typename WrappedDomainConfigType::type   DomainConfigType;

        typedef typename domain_element_collection_type<WrappedDomainConfigType>::type   full_domain_element_collection_type;

        typedef typename filter_element_container<ElementTypeList, typename full_domain_element_collection_type::typemap>::type      view_container_collection_typemap;
        typedef typename viennagrid::storage::result_of::view_collection<view_container_collection_typemap, ContainerConfig>::type   type;
      };

      // domain_appendix_type
      template <typename WrappedConfigType>
      struct domain_appendix_type
      {
        typedef typename WrappedConfigType::type   ConfigType;

        typedef typename viennagrid::storage::result_of::collection< typename viennagrid::result_of::coboundary_container_collection_typemap<WrappedConfigType>::type >::type   coboundary_collection_type;
        typedef typename viennagrid::storage::result_of::collection< typename viennagrid::result_of::neighbour_container_collection_typemap< WrappedConfigType>::type >::type   neighbour_collection_type;
        typedef typename viennagrid::storage::result_of::collection< typename viennagrid::result_of::boundary_information_collection_typemap<WrappedConfigType>::type >::type   boundary_information_type;

        typedef typename viennagrid::storage::collection_t<
              typename viennagrid::meta::make_typemap<

                  coboundary_collection_tag,
                  coboundary_collection_type,

                  neighbour_collection_tag,
                  neighbour_collection_type,

                  boundary_information_collection_tag,
                  boundary_information_type

              >::type
        > type;
      };

      template <typename WrappedDomainConfigType, typename ElementTypeList, typename ContainerConfig>
      struct domain_appendix_type< decorated_domain_view_config<WrappedDomainConfigType, ElementTypeList, ContainerConfig> >
      {
        typedef typename domain_appendix_type<WrappedDomainConfigType>::type    type;
      };


      // domain_change_counter_type
      template <typename WrappedConfigType>
      struct domain_change_counter_type
      {
        typedef typename config::result_of::query_config<typename WrappedConfigType::type, config::domain_change_counter_tag>::type  type;
      };

      template <typename WrappedDomainConfigType, typename ElementTypeList, typename ContainerConfig>
      struct domain_change_counter_type< decorated_domain_view_config<WrappedDomainConfigType, ElementTypeList, ContainerConfig> >
      {
        typedef typename config::result_of::query_config<typename WrappedDomainConfigType::type, config::domain_change_counter_tag>::type  type;
      };


      // domain_inserter_type
      template <typename WrappedConfigType>
      struct domain_inserter_type
      {
        typedef typename config::result_of::element_collection<WrappedConfigType>::type                                                       element_collection_type;
        typedef typename viennagrid::result_of::id_generator<WrappedConfigType>::type                                                 id_generator_type;
        typedef typename domain_change_counter_type<WrappedConfigType>::type change_counter_type;

        typedef typename viennagrid::storage::result_of::physical_inserter<element_collection_type, change_counter_type, id_generator_type>::type   type;
      };

      template <typename WrappedDomainConfigType, typename ElementTypeList, typename ContainerConfig>
      struct domain_inserter_type< decorated_domain_view_config<WrappedDomainConfigType, ElementTypeList, ContainerConfig> >
      {
        typedef decorated_domain_view_config<WrappedDomainConfigType, ElementTypeList, ContainerConfig>  argument_type;
        typedef WrappedDomainConfigType ConfigType;

        typedef typename domain_element_collection_type<argument_type>::type                             view_container_collection_type;
        typedef typename domain_inserter_type<WrappedDomainConfigType>::type                             full_domain_inserter_type;
        typedef typename domain_change_counter_type<WrappedDomainConfigType>::type change_counter_type;

        typedef typename viennagrid::storage::result_of::recursive_inserter<view_container_collection_type, change_counter_type, full_domain_inserter_type>::type      type;
      };
    }

    template <typename WrappedConfigType>
    class domain_t
    {
        typedef domain_t<WrappedConfigType> self_type;

    public:
        typedef WrappedConfigType                    wrapped_config_type;
        typedef typename WrappedConfigType::type     config_type;


        typedef typename result_of::domain_element_collection_type<WrappedConfigType>::type     element_collection_type;
        typedef typename result_of::domain_appendix_type<WrappedConfigType>::type               appendix_type;
        typedef typename result_of::domain_change_counter_type<WrappedConfigType>::type         change_counter_type;
        typedef typename result_of::domain_inserter_type<WrappedConfigType>::type               inserter_type;


        domain_t() : inserter( element_container_collection, change_counter_ ), change_counter_(0) {}

        template<typename OtherWrappedConfig>
        domain_t( domain_proxy<domain_t<OtherWrappedConfig> > proxy ) : change_counter_(0)
        {
            typedef typename domain_t<OtherWrappedConfig>::element_collection_type   other_element_collection_type;

            view_domain_setter< other_element_collection_type > functor(proxy.domain->element_collection());
            viennagrid::storage::collection::for_each(element_container_collection, functor);

            inserter = inserter_type( element_container_collection, change_counter_,proxy.domain->get_inserter() );
        }

        ~domain_t() {}



        // buggy !!
        domain_t(const domain_t & other) : appendix_(other.appendix_), inserter(other.inserter), change_counter_(other.change_counter_)
        {
          inserter.set_domain_info( element_container_collection, change_counter_ );
          increment_change_counter();

          copy_domain(other, *this);
        }

        // buggy !!
        domain_t & operator=( domain_t const & other)
        {
//           element_container_collection = other.element_container_collection;
          appendix_ = other.appendix_;
          inserter = other.inserter;
          change_counter_ = other.change_counter_;
          
          inserter.set_domain_info( element_container_collection, change_counter_ );
          increment_change_counter();
          
          copy_domain(other, *this);
          return *this;
        }
    public:

        // TODO no direct access to collection!
        element_collection_type & element_collection() { return element_container_collection; }
        element_collection_type const & element_collection() const { return element_container_collection; }

        appendix_type & appendix() { return appendix_; }
        appendix_type const & appendix() const { return appendix_; }

        inserter_type & get_inserter() { return inserter; }
        inserter_type const & get_inserter() const { return inserter; }

        bool is_obsolete( change_counter_type change_counter_to_check ) const { return change_counter_to_check != change_counter_; }
        void update_change_counter( change_counter_type & change_counter_to_update ) const { change_counter_to_update = change_counter_; }
        void increment_change_counter() { ++change_counter_; }

    protected:
        element_collection_type element_container_collection;

        appendix_type appendix_;
        inserter_type inserter;

        change_counter_type change_counter_;
    };


    namespace result_of
    {
        template<typename WrappedConfigType>
        struct domain
        {
          typedef domain_t<WrappedConfigType> type;
        };
    }



    template<typename WrappedConfigType>
    bool is_obsolete( domain_t<WrappedConfigType> const & domain, typename domain_t<WrappedConfigType>::change_counter_type change_counter_to_check )
    { return domain.is_obsolete( change_counter_to_check ); }

    template<typename WrappedConfigType>
    void update_change_counter( domain_t<WrappedConfigType> & domain, typename domain_t<WrappedConfigType>::change_counter_type & change_counter_to_update )
    { domain.update_change_counter( change_counter_to_update ); }

    template<typename WrappedConfigType>
    void increment_change_counter( domain_t<WrappedConfigType> & domain )
    { domain.increment_change_counter(); }









    template<typename element_tag, typename coboundary_tag, typename domain_type>
    typename viennagrid::storage::result_of::value_type<
                typename viennagrid::storage::result_of::value_type<
                    typename domain_type::appendix_type,
                    coboundary_collection_tag
                >::type,
                viennagrid::meta::static_pair<element_tag, coboundary_tag>
                >::type & coboundary_collection( domain_type & domain )
    { return viennagrid::storage::collection::get< viennagrid::meta::static_pair<element_tag, coboundary_tag> >( viennagrid::storage::collection::get<coboundary_collection_tag>( domain.appendix() ) );}

    template<typename element_tag, typename coboundary_tag, typename domain_type>
    typename viennagrid::storage::result_of::value_type<
                typename viennagrid::storage::result_of::value_type<
                    typename domain_type::appendix_type,
                    coboundary_collection_tag
                >::type,
                viennagrid::meta::static_pair<element_tag, coboundary_tag>
                >::type const & coboundary_collection( domain_type const & domain )
    { return viennagrid::storage::collection::get< viennagrid::meta::static_pair<element_tag, coboundary_tag> >( viennagrid::storage::collection::get<coboundary_collection_tag>( domain.appendix() ) );}



    template<typename element_tag, typename connector_element_tag, typename domain_type>
    typename viennagrid::storage::result_of::value_type<
                typename viennagrid::storage::result_of::value_type<
                    typename domain_type::appendix_type,
                    neighbour_collection_tag
                >::type,
                viennagrid::meta::static_pair<element_tag, connector_element_tag>
                >::type & neighbour_collection( domain_type & domain )
    { return viennagrid::storage::collection::get< viennagrid::meta::static_pair<element_tag, connector_element_tag> >( viennagrid::storage::collection::get<neighbour_collection_tag>( domain.appendix() ) ); }

    template<typename element_tag, typename connector_element_tag, typename domain_type>
    typename viennagrid::storage::result_of::value_type<
                typename viennagrid::storage::result_of::value_type<
                    typename domain_type::appendix_type,
                    neighbour_collection_tag
                >::type,
                viennagrid::meta::static_pair<element_tag, connector_element_tag>
                >::type const & neighbour_collection( domain_type const & domain )
    { return viennagrid::storage::collection::get< viennagrid::meta::static_pair<element_tag, connector_element_tag> >( viennagrid::storage::collection::get<neighbour_collection_tag>( domain.appendix() ) ); }



    template<typename element_tag, typename domain_type>
    typename viennagrid::storage::result_of::value_type<
        typename viennagrid::storage::result_of::value_type<
            typename domain_type::appendix_type,
            boundary_information_collection_tag
        >::type,
        element_tag
    >::type & boundary_information_collection( domain_type & domain )
    { return viennagrid::storage::collection::get<element_tag>( viennagrid::storage::collection::get<boundary_information_collection_tag>( domain.appendix() ) ); }

    template<typename element_tag, typename domain_type>
    typename viennagrid::storage::result_of::value_type<
        typename viennagrid::storage::result_of::value_type<
            typename domain_type::appendix_type,
            boundary_information_collection_tag
        >::type,
        element_tag
    >::type const & boundary_information_collection( domain_type const & domain )
    { return viennagrid::storage::collection::get<element_tag>( viennagrid::storage::collection::get<boundary_information_collection_tag>( domain.appendix() ) ); }

}


namespace viennagrid
{

    namespace result_of
    {
        template<typename something>
        struct container_collection_typemap;

        template<typename typemap>
        struct container_collection_typemap< storage::collection_t<typemap> >
        {
            typedef typemap type;
        };

        template<typename typemap>
        struct container_collection_typemap< const storage::collection_t<typemap> >
        {
            typedef typemap type;
        };

        template<typename key_type, typename value_type, typename tail>
        struct container_collection_typemap< viennagrid::meta::typelist_t< viennagrid::meta::static_pair<key_type, value_type>, tail > >
        {
            typedef viennagrid::meta::typelist_t< viennagrid::meta::static_pair<key_type, value_type>, tail > type;
        };



        template<typename typemap>
        struct element_collection< storage::collection_t<typemap> >
        {
            typedef storage::collection_t<typemap> type;
        };


        template<typename ConfigType>
        struct element_collection< domain_t<ConfigType> >
        {
            typedef typename domain_t<ConfigType>::element_collection_type type;
        };

        template<typename ConfigType>
        struct element_collection< const domain_t<ConfigType> >
        {
            typedef typename domain_t<ConfigType>::element_collection_type type;
        };

        template<typename domain_type>
        struct element_typelist
        {
            typedef typename element_collection<domain_type>::type container_collection;
            typedef typename viennagrid::meta::typemap::result_of::key_typelist<typename container_collection::typemap >::type type;
        };

        template<typename domain_type>
        struct element_taglist
        {
            typedef typename viennagrid::meta::typelist::TRANSFORM<
                element_tag,
                typename element_typelist<domain_type>::type
            >::type type;
        };

        template<typename something, typename boundary_element_type>
        struct is_boundary_type;

        template<typename boundary_element_type>
        struct is_boundary_type< viennagrid::meta::null_type, boundary_element_type >
        {
            static const bool value = false;
        };

        template<typename head, typename tail, typename boundary_element_type>
        struct is_boundary_type< viennagrid::meta::typelist_t<head, tail>, boundary_element_type >
        {
            typedef typename head::first boundary_container_type;
            typedef typename boundary_container_type::value_type element_type;

            static const bool value = viennagrid::meta::EQUAL< element_type, boundary_element_type >::value ? true : is_boundary_type<tail, boundary_element_type>::value;
        };

        template<typename element_tag, typename WrappedConfigType, typename boundary_element_type>
        struct is_boundary_type< element_t<element_tag, WrappedConfigType>, boundary_element_type>
        {
            typedef typename element_t<element_tag, WrappedConfigType>::bnd_cell_container_typelist bnd_cell_container_typelist;
            static const bool value = is_boundary_type<bnd_cell_container_typelist, boundary_element_type>::value;
        };




        template<typename element_typelist, typename element_type>
        struct referencing_element_typelist_impl;

        template<typename element_type>
        struct referencing_element_typelist_impl<viennagrid::meta::null_type, element_type>
        {
            typedef viennagrid::meta::null_type type;
        };

        template<typename head, typename tail, typename element_type>
        struct referencing_element_typelist_impl< viennagrid::meta::typelist_t<head, tail>, element_type >
        {
            typedef typename referencing_element_typelist_impl<tail, element_type>::type tail_typelist;

            typedef typename viennagrid::meta::IF<
                is_boundary_type<head, element_type>::value,
                viennagrid::meta::typelist_t< head, tail_typelist>,
                tail_typelist
            >::type type;
        };


        template<typename domain_type, typename element_type_or_tag>
        struct referencing_element_typelist
        {
            typedef typename element<domain_type, element_type_or_tag>::type element_type;
            typedef typename element_typelist<domain_type>::type elements;
            typedef typename referencing_element_typelist_impl<elements, element_type>::type type;
        };




        template<typename element_typelist>
        struct element_to_tag;

        template<>
        struct element_to_tag<viennagrid::meta::null_type>
        {
            typedef viennagrid::meta::null_type type;
        };

        template<typename element_tag, typename WrappedConfigType, typename tail>
        struct element_to_tag< viennagrid::meta::typelist_t< element_t<element_tag, WrappedConfigType>, tail > >
        {
            typedef viennagrid::meta::typelist_t<
                element_tag,
                typename element_to_tag<tail>::type
            > type;
        };




        template<typename something>
        struct cell_dimension;

        template<>
        struct cell_dimension<viennagrid::meta::null_type>
        {
            static const int value = -1;
        };

        template<typename key_type, typename value_type, typename tail>
        struct cell_dimension< viennagrid::meta::typelist_t< viennagrid::meta::static_pair<key_type, value_type>, tail > >
        {
            typedef typename value_type::value_type element_type;
            static const int current_topologic_dim = viennagrid::result_of::topologic_dimension<element_type>::value;
            static const int tail_topologic_dim = cell_dimension<tail>::value;

            static const int value = (current_topologic_dim < tail_topologic_dim) ? tail_topologic_dim : current_topologic_dim;
        };

        template<typename typemap>
        struct cell_dimension< storage::collection_t<typemap> >
        {
            static const int value = cell_dimension<typemap>::value;
        };

        template<typename ConfigType>
        struct cell_dimension< domain_t<ConfigType> >
        {
            static const int value = cell_dimension<typename domain_t<ConfigType>::element_collection_type>::value;
        };






    }

    template <typename WrappedConfigType, typename ElementTypeList, typename ContainerConfig>
    class decorated_domain_view_config
    {
      private:
//         typedef typename DomainConfigType::type      config_type;

        typedef typename config::result_of::element_collection< WrappedConfigType >::type                                                   element_collection_type;
        typedef typename viennagrid::result_of::id_generator<WrappedConfigType>::type                                               id_generator_type;

        typedef typename result_of::filter_element_container<ElementTypeList, typename element_collection_type::typemap>::type        view_container_collection_typemap;
        typedef typename viennagrid::storage::result_of::view_collection<view_container_collection_typemap, ContainerConfig>::type    view_container_collection_type;


      public:
        typedef view_container_collection_typemap    type;

        typedef WrappedConfigType                     domain_config_type;
    };


    template <typename WrappedConfigType, typename E, typename C, typename ElementTypeList, typename ContainerConfig>
    class decorated_domain_view_config< decorated_domain_view_config<WrappedConfigType, E, C>, ElementTypeList, ContainerConfig>
    {
      private:
//         typedef typename DomainConfigType::type      config_type;

        typedef typename config::result_of::element_collection< WrappedConfigType >::type                                                   element_collection_type;
        typedef typename viennagrid::result_of::id_generator<WrappedConfigType>::type                                               id_generator_type;

        typedef typename result_of::filter_element_container<ElementTypeList, typename element_collection_type::typemap>::type        view_container_collection_typemap;
        typedef typename viennagrid::storage::result_of::view_collection<view_container_collection_typemap, ContainerConfig>::type    view_container_collection_type;


      public:
        typedef view_container_collection_typemap    type;

        typedef typename decorated_domain_view_config<WrappedConfigType, E, C>::domain_config_type      domain_config_type;
    };


    namespace result_of
    {
        template<
            typename domain_type,
            typename element_typelist =
                typename viennagrid::meta::typemap::result_of::key_typelist<
                    typename element_collection<domain_type>::type::typemap
                >::type,
            typename container_config =
                storage::default_view_container_config
            >
        struct domain_view_from_typelist
        {
            typedef domain_t< decorated_domain_view_config<typename domain_type::wrapped_config_type, element_typelist, container_config> >  type;
        };


        template<typename DomainType,
                 typename ElementType0 = viennagrid::meta::null_type, typename ElementType1 = viennagrid::meta::null_type,
                 typename ElementType2 = viennagrid::meta::null_type, typename ElementType3 = viennagrid::meta::null_type,
                 typename ElementType4 = viennagrid::meta::null_type, typename ElementType5 = viennagrid::meta::null_type,
                 typename ElementType6 = viennagrid::meta::null_type, typename ElementType7 = viennagrid::meta::null_type,
                 typename ElementType8 = viennagrid::meta::null_type, typename ElementType9 = viennagrid::meta::null_type>
        struct domain_view
        {
            typedef typename domain_view_from_typelist<
                DomainType,
                typename viennagrid::meta::make_typelist<
                  typename element<DomainType, ElementType0>::type,
                  typename element<DomainType, ElementType1>::type,
                  typename element<DomainType, ElementType2>::type,
                  typename element<DomainType, ElementType3>::type,
                  typename element<DomainType, ElementType4>::type,
                  typename element<DomainType, ElementType5>::type,
                  typename element<DomainType, ElementType6>::type,
                  typename element<DomainType, ElementType7>::type,
                  typename element<DomainType, ElementType8>::type,
                  typename element<DomainType, ElementType9>::type
                >::type
            >::type type;
        };


        template<typename DomainType>
        struct domain_view<DomainType,
            viennagrid::meta::null_type, viennagrid::meta::null_type, viennagrid::meta::null_type,
            viennagrid::meta::null_type, viennagrid::meta::null_type, viennagrid::meta::null_type,
            viennagrid::meta::null_type, viennagrid::meta::null_type, viennagrid::meta::null_type,
            viennagrid::meta::null_type>
        {
            typedef typename domain_view_from_typelist<DomainType>::type type;
        };
    }


    template<typename topology_type>
    domain_proxy<topology_type> make_view( topology_type & domain )
    {
        return domain_proxy<topology_type>( domain );
    }




    template<typename container_collection_type>
    class handle_domain_functor
    {
    public:

        handle_domain_functor(container_collection_type & collection_) : collection(collection_) {}

        template<typename container_type>
        void operator()( container_type & container )
        {
            typedef typename container_type::value_type value_type;
            //container.set_base_container(  );
            storage::view::handle( viennagrid::storage::collection::get<value_type>(collection), container);
        }


    private:
        container_collection_type & collection;
    };

    template<typename view_type, typename domain_type>
    void handle_domain( view_type & view, domain_type & domain )
    {
        handle_domain_functor< typename result_of::element_collection<domain_type>::type > functor( element_collection(domain) );
        viennagrid::storage::collection::for_each( element_collection(view), functor);
    }





    template<typename ConfigType>
    typename domain_t<ConfigType>::element_collection_type & element_collection( domain_t<ConfigType> & domain)
    { return domain.element_collection(); }

    template<typename ConfigType>
    typename domain_t<ConfigType>::element_collection_type const & element_collection( domain_t<ConfigType> const & domain)
    { return domain.element_collection(); }


    template<typename ConfigType>
    typename domain_t<ConfigType>::inserter_type & inserter(domain_t<ConfigType> & domain)
    { return domain.get_inserter(); }

    template<typename ConfigType>
    typename domain_t<ConfigType>::inserter_type const & inserter(domain_t<ConfigType> const & domain)
    { return domain.get_inserter(); }


    template<typename ConfigType>
    typename domain_t<ConfigType>::inserter_type::id_generator_type & id_generator(domain_t<ConfigType> & domain)
    { return inserter(domain).get_id_generator(); }

    template<typename ConfigType>
    typename domain_t<ConfigType>::inserter_type::id_generator_type const & id_generator(domain_t<ConfigType> const & domain)
    { return inserter(domain).get_id_generator(); }





    template<typename ElementTypeOrTag, typename ConfigType>
    typename viennagrid::result_of::id_type<
      typename viennagrid::result_of::element< domain_t<ConfigType>, ElementTypeOrTag>::type
    >::type max_id( domain_t<ConfigType> const & domain )
    { return id_generator(domain).max_id( viennagrid::meta::tag< typename viennagrid::result_of::element< domain_t<ConfigType>, ElementTypeOrTag>::type >() ); }




    template<typename something>
    void clear_domain( something & s )
    {
        viennagrid::storage::container_collection::clear_all( viennagrid::element_collection(s) );
    }


    template<typename WrappedConfigType, typename handle_type>
    typename storage::handle::result_of::value_type<handle_type>::type & dereference_handle( domain_t<WrappedConfigType> & domain, handle_type const & handle)
    {
        typedef typename storage::handle::result_of::value_type<handle_type>::type value_type;
        return storage::collection::get<value_type>(element_collection(domain)).dereference_handle( handle );
    }

    template<typename WrappedConfigType, typename handle_type>
    typename storage::handle::result_of::value_type<handle_type>::type const & dereference_handle( domain_t<WrappedConfigType> const & domain, handle_type const & handle)
    {
        typedef typename storage::handle::result_of::value_type<handle_type>::type value_type;
        return storage::collection::get<value_type>(element_collection(domain)).dereference_handle( handle );
    }



    template<typename domain_type, typename ElementTag, typename WrappedConfigType>
    typename result_of::handle<domain_type, element_t<ElementTag, WrappedConfigType> >::type handle( domain_type & domain, element_t<ElementTag, WrappedConfigType> & element)
    { return storage::collection::get< element_t<ElementTag, WrappedConfigType> >(element_collection(domain)).handle( element ); }

    template<typename domain_type, typename ElementTag, typename WrappedConfigType>
    typename result_of::const_handle<domain_type, element_t<ElementTag, WrappedConfigType> >::type handle( domain_type const & domain, element_t<ElementTag, WrappedConfigType> const & element)
    { return storage::collection::get< element_t<ElementTag, WrappedConfigType> >(element_collection(domain)).handle( element ); }


    template<typename domain_type, typename handle_type>
    handle_type handle( domain_type &, handle_type handle) { return handle; }

    template<typename domain_type, typename handle_type>
    const handle_type handle( domain_type const &, handle_type handle) { return handle; }




  template<typename domain_type,
           typename ElementTag, typename WrappedConfigType,
           typename handle_type>
  typename viennagrid::result_of::element<domain_type, viennagrid::vertex_tag>::type
  local_vertex(const domain_type & domain,
               element_t<ElementTag, WrappedConfigType> const & host_ncell,
               handle_type const & bnd_kcell_handle,
               std::size_t index)
  {
    typedef typename viennagrid::storage::handle::result_of::value_type<handle_type>::type element_type_2;
    const element_type_2 & bnd_kcell = viennagrid::dereference_handle(domain, bnd_kcell_handle);
    return viennagrid::elements< viennagrid::vertex_tag >(bnd_kcell)[host_ncell.global_to_local_orientation(bnd_kcell_handle, index)];
  }



    template<typename container_type>
    class dereference_handle_comperator
    {
    public:

        dereference_handle_comperator(const container_type & container_) : container(container_) {}

        template<typename handle>
        bool operator() ( handle h1, handle h2 )
        {
            return &viennagrid::dereference_handle( container, h1 ) < &viennagrid::dereference_handle( container, h2 );
        }

    private:
        const container_type & container;
    };

}



namespace viennagrid
{

    namespace result_of
    {
      template<typename DomainSegmentType, typename element_type_or_tag>
      struct is_element_present;

        template<typename WrappedConfigType, typename element_type_or_tag>
        struct is_element_present< domain_t<WrappedConfigType>, element_type_or_tag >
        {
          typedef typename viennagrid::result_of::element_tag<element_type_or_tag>::type tag;
          static const bool value = config::result_of::is_element_present<WrappedConfigType, tag>::value;
        };

        template<typename WrappedConfigType, typename element_type_or_tag>
        struct is_element_present< const domain_t<WrappedConfigType>, element_type_or_tag >
        {
          static const bool value = is_element_present<domain_t<WrappedConfigType>, element_type_or_tag>::value;
        };




        template<typename typelist, typename element_tag>
        struct is_element_present_helper;

        template<typename element_tag>
        struct is_element_present_helper<meta::null_type, element_tag>
        {
            static const bool value = false;
        };

        template<typename element_type, typename tail, typename element_tag>
        struct is_element_present_helper< meta::typelist_t<element_type, tail>, element_tag>
        {
            static const bool value =
              viennagrid::meta::EQUAL<
                typename element_type::tag,
                element_tag
              >::value ||
              is_element_present_helper<tail, element_tag>::value;
        };


        template <typename WrappedDomainConfigType, typename ElementTypeList, typename ContainerConfig, typename element_type_or_tag>
        struct is_element_present< domain_t< decorated_domain_view_config<WrappedDomainConfigType, ElementTypeList, ContainerConfig> >, element_type_or_tag >
        {
          typedef typename viennagrid::result_of::element_tag<element_type_or_tag>::type tag;
          static const bool value = is_element_present_helper<ElementTypeList, tag>::value;
        };

        template <typename WrappedDomainConfigType, typename ElementTypeList, typename ContainerConfig, typename element_type_or_tag>
        struct is_element_present< const domain_t< decorated_domain_view_config<WrappedDomainConfigType, ElementTypeList, ContainerConfig> >, element_type_or_tag >
        {
          static const bool value = is_element_present<domain_t< decorated_domain_view_config<WrappedDomainConfigType, ElementTypeList, ContainerConfig> >, element_type_or_tag>::value;
        };





        template<typename DomainSegmentType, typename element_type_or_tag>
        struct element
        {
            typedef typename meta::STATIC_ASSERT< is_element_present<DomainSegmentType, element_type_or_tag>::value >::type ERROR_ELEMENT_IS_NOT_PRESENT_IN_DOMAIN;
            typedef typename element< typename element_collection< DomainSegmentType >::type, element_type_or_tag >::type type;
        };

        template<typename DomainSegmentType, typename element_type_or_tag>
        struct handle
        {
            typedef typename meta::STATIC_ASSERT< is_element_present<DomainSegmentType, element_type_or_tag>::value >::type ERROR_ELEMENT_IS_NOT_PRESENT_IN_DOMAIN;
            typedef typename handle< typename element_collection< DomainSegmentType >::type, element_type_or_tag >::type type;
        };

        template<typename DomainSegmentType, typename element_type_or_tag>
        struct const_handle
        {
            typedef typename meta::STATIC_ASSERT< is_element_present<DomainSegmentType, element_type_or_tag>::value >::type ERROR_ELEMENT_IS_NOT_PRESENT_IN_DOMAIN;
            typedef typename const_handle< typename element_collection< DomainSegmentType >::type, element_type_or_tag >::type type;
        };


        template<typename DomainSegmentType, typename element_type_or_tag>
        struct element_range
        {
            typedef typename meta::STATIC_ASSERT< is_element_present<DomainSegmentType, element_type_or_tag>::value >::type ERROR_ELEMENT_IS_NOT_PRESENT_IN_DOMAIN;
            typedef typename element_range< typename element_collection< DomainSegmentType >::type, element_type_or_tag >::type type;
        };

        template<typename DomainSegmentType, typename element_type_or_tag>
        struct const_element_range
        {
            typedef typename meta::STATIC_ASSERT< is_element_present<DomainSegmentType, element_type_or_tag>::value >::type ERROR_ELEMENT_IS_NOT_PRESENT_IN_DOMAIN;
            typedef typename const_element_range< typename element_collection< DomainSegmentType >::type, element_type_or_tag >::type type;
        };









        template<typename typemap>
        struct topologic_cell_dimension_impl;

        template<>
        struct topologic_cell_dimension_impl<viennagrid::meta::null_type>
        {
            static const int value = -1;
        };

        template<typename element_type, typename element_container_type, typename tail>
        struct topologic_cell_dimension_impl< viennagrid::meta::typelist_t< viennagrid::meta::static_pair<element_type, element_container_type>, tail > >
        {
            static const int tail_cell_dimension = topologic_cell_dimension_impl<tail>::value;
            static const int current_element_dimension = topologic_dimension<element_type>::value;

            static const int value = (tail_cell_dimension > current_element_dimension) ? tail_cell_dimension : current_element_dimension;
        };

        template<typename something>
        struct topologic_cell_dimension
        {
            static const int value = topologic_cell_dimension_impl<
                    typename container_collection_typemap<
                        typename element_collection<something>::type
                    >::type
                >::value;
        };





        template<typename typemap, int topologic_dimension>
        struct elements_of_topologic_dim_impl;

        template<int topologic_dimension>
        struct elements_of_topologic_dim_impl< viennagrid::meta::null_type, topologic_dimension >
        {
            typedef viennagrid::meta::null_type type;
        };

        template<typename element_type, typename element_container_type, typename tail, int topologic_dimension>
        struct elements_of_topologic_dim_impl< viennagrid::meta::typelist_t< viennagrid::meta::static_pair<element_type, element_container_type>, tail >, topologic_dimension >
        {
            typedef typename elements_of_topologic_dim_impl<tail, topologic_dimension>::type tail_typelist;

            typedef typename viennagrid::meta::IF<
                viennagrid::result_of::topologic_dimension<element_type>::value == topologic_dimension,
                typename viennagrid::meta::typelist::result_of::push_back<tail_typelist, element_type>::type,
                tail_typelist
            >::type type;
        };

        template<typename something, int topologic_dimension>
        struct elements_of_topologic_dim
        {
            typedef typename elements_of_topologic_dim_impl<
                typename container_collection_typemap<
                    typename element_collection<something>::type
                >::type,
                topologic_dimension>::type type;
        };



        template<typename something>
        struct cells
        {
            typedef typename elements_of_topologic_dim<
                something,
                topologic_cell_dimension<something>::value
            >::type type;
        };


        template<typename something>
        struct cell
        {
            typedef typename cells<something>::type all_cell_types;
            typedef typename viennagrid::meta::STATIC_ASSERT< viennagrid::meta::typelist::result_of::size<all_cell_types>::value == 1 >::type static_assert_typedef;

            typedef typename viennagrid::meta::typelist::result_of::at<all_cell_types,0>::type type;
        };

        template<typename something>
        struct cell_tag
        {
            typedef typename element_tag< typename cell<something>::type >::type type;
        };
    }



    template<typename domain_type, typename functor>
    struct for_each_element_functor
    {
        for_each_element_functor( domain_type & d, functor f ) : f_(f), domain_(d) {}

        template<typename element_type>
        void operator() ( viennagrid::meta::tag<element_type> )
        {
            typedef typename viennagrid::result_of::element_range<domain_type, element_type>::type element_range_type;
            typedef typename viennagrid::result_of::iterator<element_range_type>::type element_range_iterator;

            element_range_type range = viennagrid::elements(domain_);
            for (element_range_iterator it = range.begin(); it != range.end(); ++it)
                f_(*it);
        }

        template<typename element_type>
        void operator() ( viennagrid::meta::tag<element_type> ) const
        {
            typedef typename viennagrid::result_of::const_element_range<domain_type, element_type>::type element_range_type;
            typedef typename viennagrid::result_of::iterator<element_range_type>::type element_range_iterator;

            element_range_type range = viennagrid::elements(domain_);
            for (element_range_iterator it = range.begin(); it != range.end(); ++it)
                f_(*it);
        }

        functor f_;
        domain_type & domain_;
    };



    template<int topologic_dimension, typename domain_type, typename functor>
    void for_each( domain_type & domain, functor f )
    {
        for_each_element_functor<domain_type, functor> for_each_functor(domain, f);
        typedef typename viennagrid::result_of::elements_of_topologic_dim<domain_type, topologic_dimension>::type element_typelist;

        viennagrid::meta::typelist::for_each<element_typelist>( for_each_functor );
    }

    template<int topologic_dimension, typename domain_type, typename functor>
    void for_each( domain_type const & domain, functor f )
    {
        for_each_element_functor<const domain_type, functor> for_each_functor(domain, f);
        typedef typename viennagrid::result_of::elements_of_topologic_dim<domain_type, topologic_dimension>::type element_typelist;

        viennagrid::meta::typelist::for_each<element_typelist>( for_each_functor );
    }





    template<typename element_type_or_tag, typename WrappedConfigType>
    typename result_of::element_range<domain_t<WrappedConfigType>, element_type_or_tag>::type elements(domain_t<WrappedConfigType> & domain)
    {
        return elements<element_type_or_tag>( element_collection(domain) );
    }

    template<typename element_type_or_tag, typename WrappedConfigType>
    typename result_of::const_element_range<domain_t<WrappedConfigType>, element_type_or_tag>::type elements(domain_t<WrappedConfigType> const & domain)
    {
        return elements<element_type_or_tag>( element_collection(domain) );
    }


    template<typename element_type_or_tag, typename domain_type>
    typename viennagrid::result_of::id_type< typename viennagrid::result_of::element<domain_type, element_type_or_tag>::type >::type id_upper_bound( domain_type const & domain )
    {
        typedef typename viennagrid::result_of::element<domain_type, element_type_or_tag>::type element_type;
        return id_generator(domain).max_id( viennagrid::meta::tag<element_type>() );
    }



    template<typename domain_type, typename id_type>
    typename viennagrid::result_of::iterator< typename viennagrid::result_of::element_range<domain_type, typename id_type::value_type::tag>::type >::type
            find(domain_type & domain, id_type id )
    {
        typedef typename id_type::value_type element_type;
        typedef typename element_type::tag element_tag;
        typedef typename viennagrid::result_of::element_range<domain_type, element_tag>::type RangeType;
        RangeType range = viennagrid::elements<element_tag>(domain);
        return std::find_if(
                    range.begin(),
                    range.end(),
                    viennagrid::storage::id_compare<id_type>(id)
            );
    }

    template<typename domain_type, typename id_type>
    typename viennagrid::result_of::const_iterator< typename viennagrid::result_of::element_range<domain_type, typename id_type::value_type::tag>::type >::type
            find(const domain_type & domain, id_type id )
    {
        typedef typename id_type::value_type element_type;
        typedef typename element_type::tag element_tag;
        typedef typename viennagrid::result_of::const_element_range<domain_type, element_tag>::type RangeType;
        RangeType range = viennagrid::elements<element_tag>(domain);
        return std::find_if(
                    range.begin(),
                    range.end(),
                    viennagrid::storage::id_compare<id_type>(id)
            );
    }



    template<typename ElementTag, typename WrappedConfigType, typename HandleType>
    void set_vertex(
            viennagrid::element_t<ElementTag, WrappedConfigType> & element,
            HandleType vertex_handle,
//             typename viennagrid::result_of::handle< viennagrid::element_t<ElementTag, WrappedConfigType>, vertex_tag >::type vertex_handle,
            unsigned int pos
        )
    {
        element.container( viennagrid::dimension_tag<0>() ).set_handle( vertex_handle, pos );
    }

    template<typename domain_type>
    typename result_of::handle<domain_type, viennagrid::vertex_tag>::type get_vertex_handle( domain_type & domain, unsigned int pos )
    {
        typedef typename result_of::element<domain_type, viennagrid::vertex_tag>::type vertex_type;
        return viennagrid::storage::collection::get<vertex_type>( element_collection<>(domain) ).handle_at(pos);
    }



  
    template<bool generate_id, bool call_callback, typename domain_type, typename ElementTag, typename WrappedConfigType>
    std::pair<
                typename viennagrid::storage::result_of::container_of<
                    typename result_of::element_collection<domain_type>::type,
                    viennagrid::element_t<ElementTag, WrappedConfigType>
                >::type::handle_type,
                bool
            >
        push_element( domain_type & domain, viennagrid::element_t<ElementTag, WrappedConfigType> const & element)
    {
//         increment_change_counter(domain);
        return inserter(domain).template insert<generate_id, call_callback>(element);
    }









  

    namespace result_of
    {
        template<typename point_container_type>
        struct point {};

        template<typename ConfigType>
        struct point< domain_t<ConfigType> >
        {
            typedef typename viennagrid::result_of::vertex< domain_t<ConfigType> >::type::appendix_type type;
        };

        template<typename ConfigType>
        struct point< const domain_t<ConfigType> >
        {
            typedef typename viennagrid::result_of::vertex< domain_t<ConfigType> >::type::appendix_type type;
        };

        template<typename ElementTag, typename WrappedConfigType>
        struct point< element_t<ElementTag, WrappedConfigType> >
        {
            typedef typename viennagrid::result_of::vertex< element_t<ElementTag, WrappedConfigType> >::type::appendix_type type;
        };

        template<typename ElementTag, typename WrappedConfigType>
        struct point< const element_t<ElementTag, WrappedConfigType> >
        {
            typedef typename viennagrid::result_of::vertex< element_t<ElementTag, WrappedConfigType> >::type::appendix_type type;
        };
    }




    template<typename ConfigType>
    typename result_of::point< domain_t<ConfigType> >::type & point(domain_t<ConfigType> &, typename result_of::vertex< domain_t<ConfigType> >::type & vertex)
    { return vertex.appendix(); }

    template<typename ConfigType>
    typename result_of::point< domain_t<ConfigType> >::type const & point(domain_t<ConfigType> const &, typename result_of::vertex< domain_t<ConfigType> >::type const & vertex)
    { return vertex.appendix(); }


    template<typename ConfigType>
    typename result_of::point< domain_t<ConfigType> >::type & point(domain_t<ConfigType> & domain, typename result_of::vertex_handle< domain_t<ConfigType> >::type vertex_handle)
    { return dereference_handle(domain, vertex_handle).appendix(); }

    template<typename ConfigType>
    typename result_of::point< domain_t<ConfigType> >::type const & point(domain_t<ConfigType> const & domain, typename result_of::const_vertex_handle< domain_t<ConfigType> >::type vertex_handle)
    { return dereference_handle(domain, vertex_handle).appendix(); }



    template<typename domain_type, typename id_type>
    typename viennagrid::result_of::iterator< typename viennagrid::result_of::element_range<domain_type, typename id_type::value_type::tag>::type >::type
            find_by_id(domain_type & domain, id_type id)
    {
        typedef typename id_type::value_type element_type;
        typedef typename element_type::tag element_tag;
        typedef typename viennagrid::result_of::element_range<domain_type, element_tag>::type RangeType;
        typedef typename viennagrid::result_of::iterator<RangeType>::type RangeIterator;

        RangeType range = viennagrid::elements<element_tag>(domain);
        for (RangeIterator it = range.begin(); it != range.end(); ++it)
        {
            if ( viennagrid::dereference_handle(domain, it.handle()).id() == id )
                return it;
        }

        return range.end();
    }

    template<typename domain_type, typename id_type>
    typename viennagrid::result_of::const_iterator< typename viennagrid::result_of::const_element_range<domain_type, typename id_type::value_type::tag>::type >::type
            find_by_id(domain_type const & domain, id_type id)
    {
        typedef typename id_type::value_type element_type;
        typedef typename element_type::tag element_tag;
        typedef typename viennagrid::result_of::const_element_range<domain_type, element_tag>::type RangeType;
        typedef typename viennagrid::result_of::const_iterator<RangeType>::type RangeIterator;

        RangeType range = viennagrid::elements<element_tag>(domain);
        for (RangeIterator it = range.begin(); it != range.end(); ++it)
        {
            if ( viennagrid::dereference_handle(domain, it.handle()).id() == id )
                return it;
        }

        return range.end();
    }

    template<typename domain_type, typename handle_type>
    typename viennagrid::result_of::iterator< typename viennagrid::result_of::element_range<domain_type, typename storage::handle::result_of::value_type<handle_type>::type >::type >::type
            find_by_handle(domain_type & domain, handle_type handle)
    {
        typedef typename storage::handle::result_of::value_type<handle_type>::type element_type;
        typedef typename element_type::tag element_tag;
        typedef typename viennagrid::result_of::element_range<domain_type, element_tag>::type RangeType;
        typedef typename viennagrid::result_of::iterator<RangeType>::type RangeIterator;

        RangeType range = viennagrid::elements<element_tag>(domain);
        for (RangeIterator it = range.begin(); it != range.end(); ++it)
        {
            if ( it.handle() == handle )
                return it;
        }

        return range.end();
    }

    template<typename domain_type, typename handle_type>
    typename viennagrid::result_of::const_iterator< typename viennagrid::result_of::const_element_range<domain_type, typename storage::handle::result_of::value_type<handle_type>::type >::type  >::type
            find_by_handle(domain_type const & domain, handle_type handle)
    {
        typedef typename storage::handle::result_of::value_type<handle_type>::type element_type;
        typedef typename element_type::tag element_tag;
        typedef typename viennagrid::result_of::const_element_range<domain_type, element_tag>::type RangeType;
        typedef typename viennagrid::result_of::const_iterator<RangeType>::type RangeIterator;

        RangeType range = viennagrid::elements<element_tag>(domain);
        for (RangeIterator it = range.begin(); it != range.end(); ++it)
        {
            if ( it.handle() == handle )
                return it;
        }

        return range.end();
    }




    template<typename DomainT, typename SourceElementT, typename DestinationElementT>
    struct copy_element_setters
    {
      copy_element_setters(DomainT & domain_, SourceElementT const & source_element_, DestinationElementT & destination_element_) :
        domain(domain_), source_element(source_element_), destination_element(destination_element_) {}

      template<typename BoundaryElementT>
      void operator() ( viennagrid::meta::tag<BoundaryElementT> )
      {
        typedef typename result_of::const_element_range<SourceElementT, BoundaryElementT>::type     SourceBoundaryElementRangeType;
        typedef typename result_of::iterator<SourceBoundaryElementRangeType>::type                  SourceBoundaryElementRangeIterator;

        typedef typename result_of::element_range<SourceElementT, BoundaryElementT>::type           DestinationBoundaryElementRangeType;
        typedef typename result_of::iterator<DestinationBoundaryElementRangeType>::type             DestinationBoundaryElementRangeIterator;

        SourceBoundaryElementRangeType      source_boundary_elements      = viennagrid::elements(source_element);
        DestinationBoundaryElementRangeType destination_boundary_elements = viennagrid::elements(destination_element);

        SourceBoundaryElementRangeIterator      sit = source_boundary_elements.begin();
        DestinationBoundaryElementRangeIterator dit = destination_boundary_elements.begin();

        for (; sit != source_boundary_elements.end(); ++sit, ++dit)
        {
          dit.handle() = viennagrid::find_by_id( domain, sit->id() ).handle();
        }
      }

      DomainT & domain;
      SourceElementT const & source_element;
      DestinationElementT & destination_element;
    };


    template<typename ElementTypelistT>
    struct copy_domain_helper;

    template<>
    struct copy_domain_helper< meta::null_type >
    {
      template<typename SourceWrappedConfigT, typename DestinationWrappedConfigT>
      static void copy_elements( domain_t<SourceWrappedConfigT> const & source_domain, domain_t<DestinationWrappedConfigT> & destination_domain )
      {}
    };


    template<typename SourceElementT, typename TailT>
    struct copy_domain_helper< meta::typelist_t<SourceElementT, TailT> >
    {
      template<typename SourceWrappedConfigT, typename DestinationWrappedConfigT>
      static void copy_elements( domain_t<SourceWrappedConfigT> const & source_domain, domain_t<DestinationWrappedConfigT> & destination_domain )
      {
        typedef typename viennagrid::result_of::element_tag<SourceElementT>::type ElementTag;

        typedef domain_t<SourceWrappedConfigT>          SourceDomainType;
        typedef domain_t<DestinationWrappedConfigT>     DestinationDomainType;

        typedef typename viennagrid::result_of::const_element_range<SourceDomainType, SourceElementT>::type     SourceElementRangeType;
        typedef typename viennagrid::result_of::iterator<SourceElementRangeType>::type                          SourceElementRangeIterator;

        typedef typename viennagrid::result_of::element<DestinationDomainType, ElementTag>::type DestinationElementType;
        typedef typename viennagrid::result_of::handle<DestinationDomainType, ElementTag>::type DestinationElementHandleType;

        SourceElementRangeType source_elements = viennagrid::elements( source_domain );
        for (SourceElementRangeIterator it = source_elements.begin(); it != source_elements.end(); ++it)
        {
          DestinationElementType destination_element = *it;
          DestinationElementHandleType handle = viennagrid::push_element<false, false>(destination_domain, destination_element).first;

          typedef typename viennagrid::result_of::boundary_element_typelist<SourceElementT>::type BoundaryElementTypelist;
          typedef typename viennagrid::result_of::element_typelist<DestinationDomainType>::type DestinationElementTypelist;

          typedef typename viennagrid::meta::typelist::result_of::intersection<
                    BoundaryElementTypelist,
                    DestinationElementTypelist
                >::type ElementTypelist;

          copy_element_setters<DestinationDomainType, SourceElementT, DestinationElementType> setter( destination_domain, *it, viennagrid::dereference_handle(destination_domain, handle) );
          viennagrid::meta::typelist::for_each<ElementTypelist>(setter);
        }

        copy_domain_helper<TailT>::copy_elements( source_domain, destination_domain );
      }
    };

    template<typename SourceWrappedConfigT, typename DestinationWrappedConfigT>
    void copy_domain( domain_t<SourceWrappedConfigT> const & source_domain, domain_t<DestinationWrappedConfigT> & destination_domain )
    {
      typedef domain_t<SourceWrappedConfigT>          SourceDomainType;
      typedef domain_t<DestinationWrappedConfigT>     DestinationDomainType;

      typedef typename viennagrid::result_of::element_typelist<SourceDomainType>::type      SourceTypelist;
      typedef typename viennagrid::result_of::element_typelist<DestinationDomainType>::type DestinationTypelist;

          typedef typename viennagrid::meta::typelist::result_of::intersection<
                    SourceTypelist,
                    DestinationTypelist
                >::type ElementTypelist;

      copy_domain_helper<SourceTypelist>::copy_elements( source_domain, destination_domain );
    }

}

#endif
