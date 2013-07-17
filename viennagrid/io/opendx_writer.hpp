#ifndef VIENNAGRID_IO_OPENDX_WRITER_GUARD
#define VIENNAGRID_IO_OPENDX_WRITER_GUARD

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

#include <fstream>
#include <iostream>
#include "viennagrid/forwards.hpp"
#include "viennagrid/domain/domain.hpp"
#include "viennagrid/io/helper.hpp"
#include "viennagrid/domain/accessor.hpp"

/** @file opendx_writer.hpp
    @brief Provides a writer for OpenDX files
*/

namespace viennagrid
{
  namespace io
  {

    /////////////////// OpenDX export ////////////////////////////


    /** @brief Fix for a OpenDX bug: if floating-values occur, no integers (i.e. only zeros after decimal point) are allowed
     *
     * As a remedy, we add a value of 1e-5. Better to have a small error in the visualization instead of a crashed OpenDX...
     *
     * @tparam FloatingPointType  Dummy template argument to disable external linkage. Almost always float or double.
     */
    template <typename FloatingPointType>
    FloatingPointType DXfixer(FloatingPointType value)
    {
      if (value * 10000 == static_cast<long>(value * 10000))
        return value + 0.00001;

      return value;
    }

    /** @brief A helper class returning dimension-dependent attribute strings. */
    template <int DIM>
    struct DXHelper {};

    template <>
    struct DXHelper<1>
    {
      static std::string getAttributes()
      { return "attribute \"element type\" string \"line\" "; }
    };

    template <>
    struct DXHelper<2>
    {
      static std::string getAttributes()
      { return "attribute \"element type\" string \"triangles\" "; }
    };

    template <>
    struct DXHelper<3>
    {
      static std::string getAttributes()
      { return "attribute \"element type\" string \"tetrahedra\" "; }
    };

    /** @brief The OpenDX writer object. Does not support segments - always the full domain is written.
     *
     * @tparam DomainType   The ViennaGrid domain.
     */
    template <typename DomainType>
    class opendx_writer
    {

        typedef typename viennagrid::result_of::point<DomainType>::type PointType;
        typedef typename viennagrid::result_of::coord<PointType>::type CoordType;
        enum { geometric_dim = viennagrid::traits::static_size<PointType>::value };

        typedef typename viennagrid::result_of::cell_tag<DomainType>::type CellTag;

        typedef typename result_of::element<DomainType, viennagrid::vertex_tag>::type                           VertexType;
        typedef typename result_of::element<DomainType, CellTag>::type     CellType;

        typedef typename viennagrid::result_of::const_element_range<DomainType, viennagrid::vertex_tag>::type   VertexRange;
        typedef typename viennagrid::result_of::iterator<VertexRange>::type              VertexIterator;

        typedef typename viennagrid::result_of::const_element_range<DomainType, CellTag>::type     CellRange;
        typedef typename viennagrid::result_of::iterator<CellRange>::type                                        CellIterator;

        typedef typename viennagrid::result_of::const_element_range<CellType, viennagrid::vertex_tag>::type      VertexOnCellRange;
        typedef typename viennagrid::result_of::iterator<VertexOnCellRange>::type         VertexOnCellIterator;

        typedef std::map< std::string, base_dynamic_accessor_t<const double, VertexType> * > VertexScalarOutputAccessorContainer;
        typedef std::map< std::string, base_dynamic_accessor_t<const double, CellType> * > CellScalarOutputAccessorContainer;


      public:
        /** @brief Triggers the writing of the domain to a file
         *
         * @param domain    A ViennaGrid domain
         * @param filename  Name of the file
         */
        int operator()(DomainType const & domain, std::string const & filename)
        {
          typedef DXHelper<geometric_dim>  DXHelper;

          std::ofstream writer(filename.c_str());
          if (!writer.is_open())
          {
            throw cannot_open_file_exception(filename);
            return EXIT_FAILURE;
          }

          std::size_t pointnum = viennagrid::elements<vertex_tag>(domain).size();

          writer << "object \"points\" class array type float rank 1 shape " << geometric_dim << " items ";
          writer << pointnum << " data follows" << std::endl;

          //Nodes:
          VertexRange vertices = viennagrid::elements<vertex_tag>(domain);
          for (VertexIterator vit = vertices.begin();
              vit != vertices.end();
              ++vit)
          {
            PointWriter<geometric_dim>::write(writer, viennagrid::point( domain, *vit ) );
            writer << std::endl;
          }
          writer << std::endl;

          //Cells:
          std::size_t cellnum = viennagrid::elements<CellTag>(domain).size();
          writer << "object \"grid_Line_One\" class array type int rank 1 shape " << (geometric_dim + 1) << " items " << cellnum << " data follows" << std::endl;

          CellRange cells = viennagrid::elements<CellTag>(domain);
          for (CellIterator cit = cells.begin();
              cit != cells.end();
              ++cit)
          {
            VertexOnCellRange vertices_for_cell = viennagrid::elements<vertex_tag>(*cit);
            for (VertexOnCellIterator vocit = vertices_for_cell.begin();
                vocit != vertices_for_cell.end();
                ++vocit)
            {
              VertexType const & vertex = *vocit;
              writer << vertex.id() << " ";
            }
            writer << std::endl;
          }

          writer << DXHelper::getAttributes() << std::endl;
          writer << "attribute \"ref\" string \"positions\" " << std::endl;
          writer << std::endl;

          //set output-format:
          writer.flags(std::ios::fixed);
          writer.precision(5);

          //write quantity:
          if (vertex_scalar_data.size() > 0)
          {
            writer << "object \"VisData\" class array items " << pointnum << " data follows" << std::endl;
            //some quantity here

            for (VertexIterator vit = vertices.begin();
                vit != vertices.end();
                ++vit)
            {
              writer << DXfixer( vertex_scalar_data.begin()->second->access(*vit) );
              writer << std::endl;
            }

            writer << "attribute \"dep\" string \"positions\"" << std::endl;
          }
          else if (cell_scalar_data.size() > 0)
          {
            writer << "object \"VisData\" class array items " << cellnum << " data follows" << std::endl;

            //some quantity here
            for (CellIterator cit = cells.begin();
                cit != cells.end();
                ++cit)
            {
              writer << DXfixer( cell_scalar_data.begin()->second->access(*cit) );
              writer << std::endl;
            }
            writer << "attribute \"dep\" string \"connections\"" << std::endl;
          }

          writer << std::endl;
          writer << "object \"AttPotential\" class field " << std::endl;
          writer << "component \"data\" \"VisData\" " << std::endl;
          writer << "component \"positions\" \"points\"" << std::endl;
          writer << "component \"connections\" \"grid_Line_One\"" << std::endl;

          return EXIT_SUCCESS;

        } // operator()


    private:


      template<typename MapType, typename AccessorType>
      void add_to_container(MapType & map, AccessorType const accessor, std::string const & name)
      {
          typename MapType::iterator it = map.find(name);
          if (it != map.end())
          {
            delete it->second;
            it->second = new dynamic_accessor_t<const AccessorType>( accessor );
          }
          else
            map[name] = new dynamic_accessor_t<const AccessorType>( accessor );
      }


    public:

        
        /** @brief Adds scalar data on vertices for writing to the OpenDX file. Only one quantity at a time is supported! */
        template <typename T>
        void add_scalar_data_on_vertices(T const accessor, std::string name)
        {
          if (vertex_scalar_data.size() > 0)
            std::cout << "* ViennaGrid: Warning: OpenDX vertex data " << name
                      << " will be ignored, because other data is already available!" << std::endl;

          add_to_container( vertex_scalar_data, accessor, name );
        }

        /** @brief Adds scalar data on cells for writing to the OpenDX file. Note that vertex data has precedence. Only one quantity at a time is supported! */
        template <typename T>
        void add_scalar_data_on_cells(T const accessor, std::string name)
        {
          if (cell_scalar_data.size() > 0)
            std::cout << "* ViennaGrid: Warning: OpenDX cell data " << name
                      << " will be ignored, because other cell data is already available!" << std::endl;

          if (vertex_scalar_data.size() > 0)
            std::cout << "* ViennaGrid: Warning: OpenDX cell data " << name
                      << " will be ignored, because other vertex data is already available!" << std::endl;

          add_to_container( cell_scalar_data, accessor, name );
        }


      private:

        VertexScalarOutputAccessorContainer          vertex_scalar_data;
        CellScalarOutputAccessorContainer            cell_scalar_data;

    }; // opendx_writer


    /** @brief Registers scalar-valued data on vertices at the OpenDX writer. At most one data set is allowed.
      *
      * @tparam KeyType     Type of the key used with ViennaData
      * @tparam DataType    Type of the data as used with ViennaData
      * @tparam DomainType  The ViennaGrid domain type
      * @param  writer      The OpenDX writer object for which the data should be registered
      * @param  key         The key object for ViennaData
      * @param  quantity_name        Ignored. Only used for a homogeneous interface with VTK reader/writer.
      */
    template <typename DomainT, typename AccessorT>
    opendx_writer<DomainT> & add_scalar_data_on_vertices(opendx_writer<DomainT> & writer,
                                                         AccessorT const accessor,
                                                         std::string const & quantity_name)
    {
      writer.add_scalar_data_on_vertices(accessor, quantity_name);
      return writer;
    }




    template <typename DomainT, typename AccessorT>
    opendx_writer<DomainT> & add_scalar_data_on_cells(opendx_writer<DomainT> & writer,
                                                      AccessorT const accessor,
                                                      std::string const & quantity_name)
    {
      writer.add_scalar_data_on_cells(accessor, quantity_name);
      return writer;
    }



  } //namespace io
} //namespace viennagrid

#endif


