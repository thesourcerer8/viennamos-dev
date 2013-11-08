#ifndef VIENNAGRID_IO_VMESH_WRITER_GUARD
#define VIENNAGRID_IO_VMESH_WRITER_GUARD

/* =======================================================================
   Copyright (c) 2011-2013, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                     ViennaGrid - The Vienna Grid Library
                            -----------------

   License:      MIT (X11), see file LICENSE in the base directory
======================================================================= */


#include <fstream>
#include <sstream>
#include <iostream>

#include "viennagrid/mesh/mesh.hpp"

#include "viennagrid/forwards.hpp"
#include "viennagrid/io/helper.hpp"
#include "viennagrid/io/vtk_common.hpp"

/** @file viennagrid/io/vtk_writer.hpp
    @brief Provides a writer to VTK files
*/

namespace viennagrid
{
  namespace io
  {

    /** @brief Main VMesh writer class. Writes a mesh or a segment to a file
     *
     * @tparam MeshType         Type of the ViennaGrid mesh. 
     * @tparam SegmentationType   Type of the ViennaGrid segmentation. Default is the default segmentation of MeshType
     */
    template < typename MeshType, typename SegmentationType = typename viennagrid::result_of::segmentation<MeshType>::type >
    class vmesh_writer
    {
    public:
      vmesh_writer() 
      { 
        empty_  = "";
        indent_ = "  "; 
      }

      /** @brief Triggers the write process to a *.vmesh file. 
       *
       * @param mesh_obj   The ViennaGrid mesh
       * @param segmentation  The ViennaGrid segmentation.
       * @param inputfile  The inputfile path 
       * @param outputfile The outputfile path
       * 
       */
      void operator()(MeshType const & mesh, SegmentationType const & segmentation, std::string const & inputfile, std::string const & outputfile)
      {
        std::stringstream ss;
        if(outputfile.substr( outputfile.rfind(".")+1 ) == "vmesh")
          ss << outputfile;
        else
          ss << outputfile << ".vmesh";
        std::ofstream writer(ss.str().c_str());

        writer << "<mesh>" << std::endl;
        writer << indent_ << "<file>" << inputfile << "</file>" << std::endl;
        writer << indent_ << "<geomdim>" << typename viennagrid::result_of::point<MeshType>::type().size() << "</geomdim>" << std::endl;
        writer << indent_ << "<celltype>" << viennagrid::result_of::cell_tag<MeshType>::type::name() << "</celltype>" << std::endl;
        writer << indent_ << "<vertices>" << viennagrid::vertices(mesh).size() << "</vertices>" << std::endl;
        writer << indent_ << "<cells>" << viennagrid::cells(mesh).size() << "</cells>" << std::endl;
        writer << indent_ << "<segments>" << segmentation.size() << "</segments>" << std::endl;
        writer << indent_ << "<segmentation>" << std::endl;
        for (typename SegmentationType::const_iterator it = segmentation.begin(); it != segmentation.end(); ++it)
        {
          writer << indent_ << indent_ << "<segment>" << std::endl;
          writer << indent_ << indent_ << indent_ << "<id>" << it->id() << "</id>" << std::endl;
          writer << indent_ << indent_ << indent_ << "<name>" << empty_ << "</name>" << std::endl;
          writer << indent_ << indent_ << indent_ << "<vertices>" << viennagrid::vertices(*it).size() << "</vertices>" << std::endl;
          writer << indent_ << indent_ << indent_ << "<cells>" << viennagrid::cells(*it).size() << "</cells>" << std::endl;
          writer << indent_ << indent_ << "</segment>" << std::endl;
        }
        writer << indent_ << "</segmentation>" << std::endl;
        writer << "</mesh>" << std::endl;
      }
      
      std::string empty_;
      std::string indent_;
    };

  } //namespace io
} //namespace viennagrid

#endif

