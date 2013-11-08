#include <iostream>
#include <typeinfo>

#include <map>

using std::cout;
using std::endl;


#include "viennagrid/forwards.hpp"

#include "viennagrid/storage/view.hpp"
#include "viennagrid/storage/container_collection.hpp"
#include "viennagrid/storage/inserter.hpp"
#include "viennagrid/storage/id_generator.hpp"
#include "viennagrid/storage/hidden_key_map.hpp"
#include "viennagrid/storage/range.hpp"


#include "viennagrid/topology/vertex.hpp"
#include "viennagrid/topology/line.hpp"
#include "viennagrid/topology/simplex.hpp"


#include "viennagrid/element/element_key.hpp"
#include "viennagrid/element/element_orientation.hpp"

#include "viennagrid/config/element_config.hpp"
#include "viennagrid/config/mesh_config.hpp"

#include "viennagrid/point.hpp"

#include "viennagrid/mesh/mesh.hpp"
#include "viennagrid/mesh/element_creation.hpp"


#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
namespace ublas = boost::numeric::ublas;

typedef ublas::vector<double> vector_type;

class my_mesh_config
{
  private:
    //typedef viennagrid::storage::pointer_handle_tag handle_tag;
    //typedef viennagrid::storage::iterator_handle_tag handle_tag;
    typedef viennagrid::storage::id_handle_tag          handle_tag;

  public:

    typedef viennagrid::config::result_of::full_mesh_config< viennagrid::tetrahedron_tag, vector_type, handle_tag >::type     type;
};


int main()
{


    //
    // typedefing and setting up the geometric mesh
    //

    typedef viennagrid::mesh<my_mesh_config> mesh_type;
    mesh_type mesh;

    //
    // typedefs for the element types
    //

    typedef viennagrid::result_of::element<mesh_type, viennagrid::vertex_tag>::type vertex_type;
    typedef viennagrid::result_of::handle<mesh_type, viennagrid::vertex_tag>::type vertex_handle_type;

    typedef viennagrid::result_of::element<mesh_type, viennagrid::line_tag>::type line_type;
    typedef viennagrid::result_of::element<mesh_type, viennagrid::triangle_tag>::type triangle_type;

    typedef viennagrid::result_of::element<mesh_type, viennagrid::tetrahedron_tag>::type tetrahedron_type;
    typedef viennagrid::result_of::handle<mesh_type, viennagrid::tetrahedron_tag>::type tetrahedron_handle_type;


//     typedef viennagrid::result_of::handle<mesh_type, viennagrid::polygon_tag>::type tetrahedron_handle_type;


    //
    // Adding a tetrahedron
    //

    // creates four vertices within the mesh, vh is short vor vertex handle
    // make_element is responsible for resizing all meta-info container which are associated with vertex_type
    vertex_handle_type vh0 = viennagrid::make_vertex( mesh );
    vertex_handle_type vh1 = viennagrid::make_vertex( mesh );
    vertex_handle_type vh2 = viennagrid::make_vertex( mesh );
    vertex_handle_type vh3 = viennagrid::make_vertex( mesh );


    // create geometric information for the vertices
    vector_type p0(3);
    p0[0] = 0.0; p0[1] = 0.0; p0[2] = 0.0;
    vector_type p1(3);
    p1[0] = 1.0; p1[1] = 0.0; p1[2] = 0.0;
    vector_type p2(3);
    p2[0] = 0.0; p2[1] = 1.0; p2[2] = 0.0;
    vector_type p3(3);
    p3[0] = 0.0; p3[1] = 0.0; p3[2] = 1.0;


    // set the geometric information for the vertices
    // is equivalent to viennagrid::look_up<vector_type>(mesh, vhX)
    viennagrid::point(mesh, vh0) = p0;
    viennagrid::point(mesh, vh1) = p1;
    viennagrid::point(mesh, vh2) = p2;
    viennagrid::point(mesh, vh3) = p3;


    // creates a handle buffer for the vertex handles of the tetdrahedron
    std::vector<vertex_handle_type> handles(4);
    handles[0] = vh0; handles[1] = vh1; handles[2] = vh2; handles[3] = vh3;

    // creates the tetrahedron within the mesh, all boundary cell generation is done here implicit
    tetrahedron_handle_type tet = viennagrid::make_element<tetrahedron_type>( mesh, handles.begin(), handles.end() );
    std::cout << tet << std::endl;

    // set a double value to a tetdrahedron
//     viennagrid::look_up<double>(mesh, tet) = 1.0;

    //
    // display the mesh content
    //

    cout << "All vertices of the mesh" << endl;
    std::copy( viennagrid::elements<viennagrid::vertex_tag>(mesh).begin(), viennagrid::elements<viennagrid::vertex_tag>(mesh).end(), std::ostream_iterator<vertex_type>(cout, "\n") );
    cout << endl;

    cout << "All lines of the mesh" << endl;
    std::copy( viennagrid::elements<viennagrid::line_tag>(mesh).begin(), viennagrid::elements<viennagrid::line_tag>(mesh).end(), std::ostream_iterator<line_type>(cout, "\n") );
    cout << endl;

    cout << "All triangles of the mesh" << endl;
    std::copy( viennagrid::elements<viennagrid::triangle_tag>(mesh).begin(), viennagrid::elements<viennagrid::triangle_tag>(mesh).end(), std::ostream_iterator<triangle_type>(cout, "\n") );
    cout << endl;

    cout << "All tetraherons of the mesh" << endl;
    std::copy( viennagrid::elements<viennagrid::tetrahedron_tag>(mesh).begin(), viennagrid::elements<viennagrid::tetrahedron_tag>(mesh).end(), std::ostream_iterator<tetrahedron_type>(cout, "\n") );
    cout << endl;


    //
    // doing some boundary cell iteration
    //

    const tetrahedron_type & test_tet = *viennagrid::elements<viennagrid::tetrahedron_tag>(mesh).begin();
    const triangle_type & test_tri = *viennagrid::elements<viennagrid::triangle_tag>(mesh).begin();



    typedef viennagrid::result_of::const_element_range<tetrahedron_type, viennagrid::triangle_tag>::type tetrahedron_triangle_range;
    typedef viennagrid::result_of::const_iterator<tetrahedron_triangle_range>::type tetrahedron_triangle_iterator;

    cout << "All triangles of the first tetdrahedron in the mesh" << endl;
    tetrahedron_triangle_range tri_range = viennagrid::elements<viennagrid::triangle_tag>(test_tet);
    for (tetrahedron_triangle_iterator it = tri_range.begin(); it != tri_range.end(); ++it)
        cout << *it << endl;
    cout << endl;

    cout << "Once more with std::copy" << endl;
    std::copy( tri_range.begin(), tri_range.end(), std::ostream_iterator<triangle_type>(cout, "\n") );
    cout << endl;




    typedef viennagrid::result_of::const_element_range<triangle_type, viennagrid::line_tag>::type triangle_line_range;
    typedef viennagrid::result_of::const_iterator<triangle_line_range>::type triangle_line_iterator;

    cout << "All lines of the first triangle in the mesh" << endl;
    triangle_line_range lin_range = viennagrid::elements<viennagrid::line_tag>(test_tri);
    for (triangle_line_iterator it = lin_range.begin(); it != lin_range.end(); ++it)
        cout << *it << endl;
    cout << endl;

    cout << "Once more with std::copy" << endl;
    std::copy( lin_range.begin(), lin_range.end(), std::ostream_iterator<line_type>(cout, "\n") );
    cout << endl;




    //
    // geometric iteration
    //

    // iterating over all vertices and piping out the point information
//     typedef viennagrid::result_of::const_element_range<tetrahedron_type, viennagrid::vertex_tag>::type tetrahedron_vertex_range;
//     typedef viennagrid::result_of::const_iterator<tetrahedron_vertex_range>::type tetrahedron_vertex_iterator;
//
//     cout << "All vertices of the first tetdrahedron in the mesh USING ncells<dim>()" << endl;
//     tetrahedron_vertex_range vtx_range = viennagrid::elements<viennagrid::vertex_tag>(test_tet);
//     for (tetrahedron_vertex_iterator it = vtx_range.begin(); it != vtx_range.end(); ++it)
//         cout << *it << " geometric information: " << viennagrid::look_up<vector_type>( mesh, *it ) << endl;
//     cout << endl;
//
//
//     typedef viennagrid::result_of::const_element_range<mesh_type, viennagrid::vertex_tag>::type mesh_vertex_range_2;
//     typedef viennagrid::result_of::const_iterator<mesh_vertex_range_2>::type mesh_vertex_iterator_2;
//
//     cout << "All vertices of the first tetdrahedron in the mesh USING elements<tag>()" << endl;
//     mesh_vertex_range_2 mesh_vtx_range_2 = viennagrid::elements<viennagrid::vertex_tag>(mesh);
//     for (mesh_vertex_iterator_2 it = mesh_vtx_range_2.begin(); it != mesh_vtx_range_2.end(); ++it)
//         cout << *it << " geometric information: " << viennagrid::look_up<vector_type>( mesh, *it ) << endl;
//     cout << endl;
//
//
//     typedef viennagrid::result_of::const_element_range<mesh_type, vertex_type>::type mesh_vertex_range_3;
//     typedef viennagrid::result_of::const_iterator<mesh_vertex_range_3>::type mesh_vertex_iterator_3;
//
//     cout << "All vertices of the first tetdrahedron in the mesh USING elements<type>()" << endl;
//     mesh_vertex_range_3 mesh_vtx_range_3 = viennagrid::elements<vertex_type>(mesh);
//     for (mesh_vertex_iterator_3 it = mesh_vtx_range_3.begin(); it != mesh_vtx_range_3.end(); ++it)
//         cout << *it << " geometric information: " << viennagrid::look_up<vector_type>( mesh, *it ) << endl;
//     cout << endl;
//
//
//     // iterating over all tetrahedrons and piping out the double meta-information
//     typedef viennagrid::result_of::const_element_range<mesh_type, viennagrid::tetrahedron_tag>::type tetrahedron_range;
//     typedef viennagrid::result_of::const_iterator<tetrahedron_range>::type tetrahedron_iterator;
//
//     cout << "All tetdrahedrons in the mesh" << endl;
//     tetrahedron_range tet_range = viennagrid::elements<viennagrid::tetrahedron_tag>(mesh);
//     for (tetrahedron_iterator it = tet_range.begin(); it != tet_range.end(); ++it)
//     {
//         cout << *it << endl;
//         cout << "   geometric information: " << viennagrid::look_up<double>( mesh, *it ) << endl;
//     }
//     cout << endl;












    const mesh_type & test = mesh;

    typedef viennagrid::result_of::const_element_range<mesh_type, viennagrid::vertex_tag>::type const_vertex_range;
    typedef viennagrid::result_of::iterator<const_vertex_range>::type const_vertex_iterator;

    const_vertex_range r = viennagrid::elements<viennagrid::vertex_tag>(test);
    for (const_vertex_iterator i = r.begin(); i != r.end(); ++i)
    {
        cout << *i << endl;
        cout << viennagrid::point(test, *i) << endl;
    }


    return 0;
}
