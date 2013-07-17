#include <iostream>
#include <typeinfo>

#include "viennagrid/config/default_configs.hpp"

#include "viennagrid/domain/accessor.hpp"
#include "viennagrid/domain/segmentation.hpp"
#include "viennagrid/domain/element_creation.hpp"



int main()
{
  typedef viennagrid::triangular_3d_domain                        domain_type;
  typedef viennagrid::result_of::domain_view< domain_type >::type view_type;
  
  typedef viennagrid::result_of::vertex_handle< domain_type >::type     vertex_handle_type;
  typedef viennagrid::result_of::triangle_handle< domain_type >::type   triangle_handle_type;
  typedef viennagrid::result_of::triangle< domain_type >::type          triangle_type;
  

  // triangular hull segmentation, each triangular face has a segment for its positive orientation and its negative orientation
  typedef viennagrid::result_of::oriented_3d_hull_segmentation<domain_type>::type segmentation_type;
  
  // defining the segment type and segment id type
  typedef viennagrid::result_of::segment<segmentation_type>::type segment_type;
  typedef viennagrid::result_of::segment_id<segmentation_type>::type segment_id_type;

  
  domain_type domain;
  segmentation_type segmentation(domain);
  
  // create some vertices
  vertex_handle_type vh0 = viennagrid::make_vertex( domain );
  vertex_handle_type vh1 = viennagrid::make_vertex( domain );
  vertex_handle_type vh2 = viennagrid::make_vertex( domain );
  vertex_handle_type vh3 = viennagrid::make_vertex( domain );
  vertex_handle_type vh4 = viennagrid::make_vertex( domain );
  vertex_handle_type vh5 = viennagrid::make_vertex( domain );
  
  // create some triangles
  triangle_handle_type th0 = viennagrid::make_triangle( domain, vh0, vh1, vh2 );
  triangle_handle_type th1 = viennagrid::make_triangle( domain, vh1, vh2, vh3 );
  triangle_handle_type th2 = viennagrid::make_triangle( domain, vh3, vh4, vh5 );
  
  

  // create a segment, segment id will be 0 because of first segment
  segment_type seg0 = segmentation.make_segment();
  
  // create a segment, segment id will be 4 because explicit segment_id
  segment_type seg1 = segmentation[ segment_id_type(4) ];
  
  // create a segment, segment id will be 5 because highest segment id was 4
  segment_type seg2 = segmentation.make_segment();
  
  // print the segments
  std::cout << "Segments in Segmentation" << std::endl;
  for ( segmentation_type::iterator it = segmentation.begin(); it != segmentation.end(); ++it)
    std::cout << (*it).id() << std::endl;
  
  std::cout << std::endl;
  
  
  triangle_type & tri0 = viennagrid::dereference_handle(domain, th0);
  triangle_type & tri1 = viennagrid::dereference_handle(domain, th1);
  triangle_type & tri2 = viennagrid::dereference_handle(domain, th2);
  
  // add triangle tr0 to seg0
  viennagrid::add( seg0, tri0 );
  std::cout << "Triangle 0 in Segment 0: " << viennagrid::is_in_segment( seg0, tri0 ) << std::endl;
  
  // erase triangle tr0 from seg0
  viennagrid::erase( seg0, tri0 );
  std::cout << "Triangle 0 in Segment 0: " <<  viennagrid::is_in_segment( seg0, tri0 ) << std::endl;
  
  
  // add triangle tr0 to seg0, tri1 to seg1, tri2 to seg2
  viennagrid::add( seg0, tri0 );
  viennagrid::add( seg1, tri1 );
  viennagrid::add( seg2, tri2 );
  
  // setting and querying additional segment information for tri0
  std::cout << "Triangle 0 in Segment 0: " <<  *viennagrid::element_segment_info( seg0, tri0 ) << std::endl;
  *viennagrid::element_segment_info( seg0, tri0 ) = true;
  std::cout << "Triangle 0 in Segment 0: " <<  *viennagrid::element_segment_info( seg0, tri0 ) << std::endl;
  std::cout << std::endl;
  
  // printing all triangles from all segments
  typedef viennagrid::result_of::element_range< segment_type, viennagrid::triangle_tag >::type range_type;
  
  std::cout << "Triangles of Segment 0" << std::endl;
  range_type range = viennagrid::elements( seg0 );
  for (range_type::iterator it = range.begin(); it != range.end(); ++it)
    std::cout << *it << std::endl;
  
  std::cout << "Triangles of Segment 1" << std::endl;
  range = viennagrid::elements( seg1 );
  for (range_type::iterator it = range.begin(); it != range.end(); ++it)
    std::cout << *it << std::endl;
  
  std::cout << "Triangles of Segment 2" << std::endl;
  range = viennagrid::elements( seg2 );
  for (range_type::iterator it = range.begin(); it != range.end(); ++it)
    std::cout << *it << std::endl;
  
  std::cout << std::endl;
  
  // Adding additional triangles to seg2 and printing them
  viennagrid::make_triangle( seg2, vh1, vh4, vh5 );
  viennagrid::make_triangle( seg2, vh2, vh4, vh5 );
  
  std::cout << "Triangles of Segment 2, added 2 additional" << std::endl;
  range = viennagrid::elements( seg2 );
  for (range_type::iterator it = range.begin(); it != range.end(); ++it)
    std::cout << *it << std::endl;
  std::cout << std::endl;
  
  // Printing vertices from seg2, each vertex should only be printed once
  typedef viennagrid::result_of::vertex_range<segment_type>::type vertex_range;
  std::cout << "Vertices of Segment 2" << std::endl;
  vertex_range vertices = viennagrid::elements( seg2 );
  for (vertex_range::iterator it = vertices.begin(); it != vertices.end(); ++it)
    std::cout << *it << std::endl;
    
    return 0;
}
