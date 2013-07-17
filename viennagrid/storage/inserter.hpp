#ifndef VIENNAGRID_STORAGE_INSERTER_HPP
#define VIENNAGRID_STORAGE_INSERTER_HPP


//#include "viennagrid/storage/reference.hpp"
#include "viennagrid/storage/container_collection.hpp"
#include "viennagrid/storage/container_collection_element.hpp"

namespace viennagrid
{
    namespace storage
    {

        template<typename container_collection_type, typename change_counter_type, typename id_generator_type_>
        class physical_inserter_t
        {
        public:
            typedef container_collection_type physical_container_collection_type;
            typedef id_generator_type_ id_generator_type;

            physical_inserter_t() : collection(0), change_counter(0) {}
            physical_inserter_t(container_collection_type & _collection) : collection(&_collection), change_counter(0) {}
            physical_inserter_t(container_collection_type & _collection, id_generator_type id_generator_) : collection(&_collection), change_counter(0), id_generator(id_generator_) {}

            physical_inserter_t(container_collection_type & _collection, change_counter_type & change_counter_) :
                collection(&_collection), change_counter(&change_counter_) {}
            physical_inserter_t(container_collection_type & _collection, change_counter_type & change_counter_, id_generator_type id_generator_) :
                collection(&_collection), change_counter(&change_counter_), id_generator(id_generator_) {}

            void set_domain_info(container_collection_type & _collection, change_counter_type & change_counter_)
            {
                collection = &_collection;
                change_counter = &change_counter_;
            }

            template<bool generate_id, bool call_callback, typename value_type, typename inserter_type>
            std::pair<
                typename viennagrid::storage::result_of::container_of<container_collection_type, value_type>::type::handle_type,
                bool
            >
                    physical_insert( value_type element, inserter_type & inserter )
            {
                typedef typename viennagrid::storage::result_of::container_of<container_collection_type, value_type>::type::iterator iterator;
                typedef typename viennagrid::storage::result_of::container_of<container_collection_type, value_type>::type container_type;
                typedef typename viennagrid::storage::result_of::container_of<container_collection_type, value_type>::type::handle_tag handle_tag;
                typedef typename viennagrid::storage::result_of::container_of<container_collection_type, value_type>::type::handle_type handle_type;


                container_type & container = viennagrid::storage::collection::get< value_type >( *collection );

                if ( generate_id && !container.is_present( element ) )
                    viennagrid::storage::id::set_id(element, id_generator( viennagrid::meta::tag<value_type>() ) );

                if (!generate_id)
                  id_generator.set_max_id( element.id() );

                std::pair<handle_type, bool> ret = container.insert( element );
                if (change_counter) ++(*change_counter);

                if (call_callback)
                    viennagrid::storage::container_collection_element::insert_callback(
                        container.dereference_handle(ret.first),
                        ret.second,
                        inserter);

                inserter.handle_insert( ret.first, viennagrid::meta::tag<value_type>() );

                return ret;
            }

            template<typename handle_type, typename value_type>
            void handle_insert( handle_type, viennagrid::meta::tag<value_type> ) {}


            template<bool generate_id, bool call_callback, typename value_type>
            std::pair<
                typename viennagrid::storage::result_of::container_of<container_collection_type, value_type>::type::handle_type,
                bool
            >
                insert( const value_type & element )
            {
                return physical_insert<generate_id, call_callback>( element, *this );
            }

            template<typename value_type>
            std::pair<
                typename viennagrid::storage::result_of::container_of<container_collection_type, value_type>::type::handle_type,
                bool
            >
                operator()( const value_type & element )
            {
                return insert<true, true>( element );
            }

            container_collection_type & get_physical_container_collection() { return *collection; }
            container_collection_type const & get_physical_container_collection() const { return *collection; }

            id_generator_type & get_id_generator() { return id_generator; }
            id_generator_type const & get_id_generator() const { return id_generator; }

        private:
            container_collection_type * collection;
            change_counter_type * change_counter;

            id_generator_type id_generator;
        };








        template<typename view_collection_type, typename change_counter_type, typename dependend_inserter_type>
        class recursive_inserter_t
        {
        public:
            recursive_inserter_t() : view_collection(0), change_counter(0), dependend_inserter(0) {}
            recursive_inserter_t(view_collection_type & collection_) : view_collection(&collection_), change_counter(0), dependend_inserter(0) {}

            recursive_inserter_t(view_collection_type & _collection, change_counter_type & change_counter_) :
                view_collection(&_collection), change_counter(&change_counter_) {}
            recursive_inserter_t(view_collection_type & _collection, dependend_inserter_type & dependend_inserter_) :
                view_collection(&_collection), change_counter(0), dependend_inserter(&dependend_inserter_) {}

            recursive_inserter_t(view_collection_type & _collection, change_counter_type & change_counter_, dependend_inserter_type & dependend_inserter_) :
                view_collection(&_collection), change_counter(&change_counter_), dependend_inserter(&dependend_inserter_) {}


//             recursive_inserter_t(view_collection_type & collection_, dependend_inserter_type & dependend_inserter_) :
//                view_collection(&collection_), dependend_inserter(&dependend_inserter_) {}


            void set_domain_info(view_collection_type & _collection, change_counter_type & change_counter_)
            {
                view_collection = &_collection;
                change_counter = &change_counter_;
            }


            template<typename handle_type, typename value_type>
            void handle_insert( handle_type ref, viennagrid::meta::tag<value_type> )
            {
                viennagrid::storage::container_collection::handle_or_ignore( *view_collection, ref, viennagrid::meta::tag<value_type>() );

                dependend_inserter->handle_insert( ref, viennagrid::meta::tag<value_type>() );
                if (change_counter) ++(*change_counter);
            }


            typedef typename dependend_inserter_type::physical_container_collection_type physical_container_collection_type;

            template<bool generate_id, bool call_callback, typename value_type, typename inserter_type>
            std::pair<
                typename viennagrid::storage::result_of::container_of<physical_container_collection_type, value_type>::type::handle_type,
                bool
            >
                physical_insert( const value_type & element, inserter_type & inserter )
            {
                return dependend_inserter->physical_insert<generate_id, call_callback>( element, inserter );
            }



            template<bool generate_id, bool call_callback, typename value_type>
            std::pair<
                typename viennagrid::storage::result_of::container_of<physical_container_collection_type, value_type>::type::handle_type,
                bool
            >
                insert( const value_type & element )
            {
                return physical_insert<generate_id, call_callback>( element, *this );
            }

            template<typename value_type>
            std::pair<
                typename viennagrid::storage::result_of::container_of<physical_container_collection_type, value_type>::type::handle_type,
                bool
            >
                operator()( const value_type & element )
            {
                return insert<true, true>( element );
            }

            physical_container_collection_type & get_physical_container_collection() { return dependend_inserter->get_physical_container_collection(); }
            physical_container_collection_type const & get_physical_container_collection() const { return dependend_inserter->get_physical_container_collection(); }

            typedef typename dependend_inserter_type::id_generator_type id_generator_type;
            id_generator_type & get_id_generator() { return dependend_inserter->get_id_generator(); }
            id_generator_type const & get_id_generator() const { return dependend_inserter->get_id_generator(); }


        private:
            view_collection_type * view_collection;
            change_counter_type * change_counter;

            dependend_inserter_type * dependend_inserter;
        };



        namespace inserter
        {
            template<typename dependend_inserter_type, typename container_collection_type, typename change_counter_type>
            recursive_inserter_t<container_collection_type, dependend_inserter_type, change_counter_type> get_recursive(
                  dependend_inserter_type const & inserter,
                  container_collection_type & collection,
                  change_counter_type & change_counter)
            {
                return recursive_inserter_t<container_collection_type, dependend_inserter_type, change_counter_type>(inserter, change_counter, collection);
            }
        }


        namespace result_of
        {
            template<typename container_collection_type, typename change_counter_type, typename dependend_inserter_type>
            struct recursive_inserter
            {
                typedef recursive_inserter_t<container_collection_type, change_counter_type, dependend_inserter_type> type;
            };

            template<typename container_collection_type, typename change_counter_type, typename id_generator_type>
            struct physical_inserter
            {
                typedef physical_inserter_t<container_collection_type, change_counter_type, id_generator_type> type;
            };
        }

    }
}

#endif
