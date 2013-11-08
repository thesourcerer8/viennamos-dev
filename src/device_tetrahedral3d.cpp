/* =======================================================================
   Copyright (c) 2011-2013, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                 ViennaMini - The Vienna Device Simulator
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               Josef Weinbub                   weinbub@iue.tuwien.ac.at
               (add your name here)

   license:    see file LICENSE in the ViennaFVM base directory
======================================================================= */


#include "device.tpp"


namespace viennamini
{

// Explicit instantiation
template class ::viennamini::device<MeshTetrahedral3DType, SegmentationTetrahedral3DType, StorageType>;

} // viennamini

