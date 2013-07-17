#include <iostream>
#include <typeinfo>

#include "viennagrid/config/default_configs.hpp"

#include "viennagrid/domain/accessor.hpp"
#include "viennagrid/domain/segmentation.hpp"
#include "viennagrid/domain/element_creation.hpp"

#include "viennagrid/algorithm/boundary.hpp"
#include "viennagrid/algorithm/interface.hpp"

int main()
{
  typedef viennagrid::triangular_3d_domain                        domain_type;
  typedef viennagrid::result_of::domain_view< domain_type >::type view_type;
  
  
  
  typedef viennagrid::result_of::vertex< domain_type >::type     vertex_type;
  typedef viennagrid::result_of::line< domain_type >::type       line_type;
  
  typedef viennagrid::result_of::vertex_handle< domain_type >::type     vertex_handle_type;
  typedef viennagrid::result_of::triangle_handle< domain_type >::type   triangle_handle_type;
  typedef viennagrid::result_of::triangle< domain_type >::type          triangle_type;
   
  typedef viennagrid::result_of::oriented_3d_hull_segmentation<domain_type>::type segmentation_type;
//   typedef viennagrid::result_of::segmentation<domain_type>::type segmentation_type;
  
  typedef segmentation_type::segment_type segment_type;
  typedef segmentation_type::segment_id_type segment_id_type;
  
  domain_type domain;
  segmentation_type segmentation(domain);
  
  
  vertex_handle_type vh0 = viennagrid::make_vertex( domain );
  vertex_handle_type vh1 = viennagrid::make_vertex( domain );
  vertex_handle_type vh2 = viennagrid::make_vertex( domain );
  vertex_handle_type vh3 = viennagrid::make_vertex( domain );
  vertex_handle_type vh4 = viennagrid::make_vertex( domain );
  
  triangle_handle_type th0 = viennagrid::make_triangle( domain, vh0, vh1, vh2 );
  triangle_handle_type th1 = viennagrid::make_triangle( domain, vh0, vh2, vh3 );
  triangle_handle_type th2 = viennagrid::make_triangle( domain, vh0, vh3, vh4 );
  triangle_handle_type th3 = viennagrid::make_triangle( domain, vh0, vh4, vh1 );

  
  segment_type seg0 = segmentation.make_segment();
  segment_type seg1 = segmentation.make_segment(); 
  
    
  triangle_type & tri0 = viennagrid::dereference_handle(domain, th0);
  triangle_type & tri1 = viennagrid::dereference_handle(domain, th1);
  triangle_type & tri2 = viennagrid::dereference_handle(domain, th2);
  triangle_type & tri3 = viennagrid::dereference_handle(domain, th3);
  
  viennagrid::add( seg0, tri0 );
  viennagrid::add( seg0, tri1 );
  viennagrid::add( seg1, tri2 );
  viennagrid::add( seg1, tri3 );
  
  typedef viennagrid::result_of::element_range< domain_type, viennagrid::line_tag >::type line_range;
  line_range lines = viennagrid::elements( domain );
  for (line_range::iterator it = lines.begin(); it != lines.end(); ++it)
  {
    std::cout << *it << " INTERFACE? " << viennagrid::is_interface( seg0, seg1, *it ) << std::endl;
  }

  typedef viennagrid::result_of::element_range< domain_type, viennagrid::vertex_tag >::type vertex_range;
  vertex_range vertices = viennagrid::elements( domain );
  for (vertex_range::iterator it = vertices.begin(); it != vertices.end(); ++it)
  {
    std::cout << *it << " INTERFACE? " << viennagrid::is_interface( seg0, seg1, *it ) << std::endl;
  }
  
  
  return 0;
}
