//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_BOX_HPP
#define INCLUDE_EVOLUTION_MATH_BOX_HPP 

/**
 * @file    evolution/core/math/Box.hpp
 * @brief   The Box class defines a Box in 3space. The outward normal is 
 *          defined as right hand rule on first-second-third points.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Box
 */

#include <stddef.h>
#include <vector>
#include <algorithm>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/math/Vector3.hpp>
#include <evolution/core/math/Triangle.hpp>


namespace Evolution
{

    /**
     * @brief Internal type used to keep a list of vertices of the Box.
     * @see Vector3
     */
    typedef std::vector<Vector3> VertexList;

    /**
     * @brief Internal type used to keep a list of faces of the Box.
     * @see Triangle
     */
    typedef std::vector<Triangle> TriangleList;


    /**
     * @class Box
     * @brief The Box class defines a Box in 3space. The outward normal is 
     * defined as right hand rule on first-second-third points.
     *
     * @ingroup core_math
     *
     * @see Vector3, Triangle
     **/
    class EVOLUTION_EXPORT_MATH Box
    {
    public: // Structors

        /**
         * @brief  Constructs a unit box.
         *
         * Standard constructor, sets up a unit box centered on .5 .5 .5.
         */
        Box ()
        { 
            Vector3 t(0,0,0);       _vertex_list.push_back (t);
            t[0]=0; t[1]=0; t[2]=1; _vertex_list.push_back (t);
            t[0]=0; t[1]=1; t[2]=0; _vertex_list.push_back (t);
            t[0]=0; t[1]=1; t[2]=1; _vertex_list.push_back (t);
            t[0]=1; t[1]=0; t[2]=0; _vertex_list.push_back (t);
            t[0]=1; t[1]=0; t[2]=1; _vertex_list.push_back (t);
            t[0]=1; t[1]=1; t[2]=0; _vertex_list.push_back (t);
            t[0]=1; t[1]=1; t[2]=1; _vertex_list.push_back (t);
            fill_triangle_list();
        } // end Box()

        /**
         * @brief  Constructs a Box from the 3 points specified.
         *
         * Creates a box centered on (o_x, o_y, o_z), with side lengths of 
         * l_x, l_y and l_z.  
         * The parameters r_x, r_y, and r_z are unused at the moment.
         * 
         * @param o_x [in]; Offset in x dimension
         * @param o_y [in]; Offset in y dimension
         * @param o_z [in]; Offset in z dimension
         * @param l_x [in]; Side length in x dimension
         * @param l_y [in]; Side length in y dimension
         * @param l_z [in]; Side length in z dimension
         * @param r_x [in]; Rotation about x axis (unused)
         * @param r_y [in]; Rotation about y axis (unused)
         * @param r_z [in]; Rotation about z axis (unused)
         */
        Box (double o_x, double o_y, double o_z, 
             double l_x, double l_y, double l_z,
             double r_x=0, double r_y=0, double r_z=0)
        {
            Vector3 t;     
            t.set(o_x-l_x/2, o_y-l_y/2, o_z-l_z/2); _vertex_list.push_back (t);
            t.set(o_x-l_x/2, o_y-l_y/2, o_z+l_z/2); _vertex_list.push_back (t);
            t.set(o_x-l_x/2, o_y+l_y/2, o_z-l_z/2); _vertex_list.push_back (t);
            t.set(o_x-l_x/2, o_y+l_y/2, o_z+l_z/2); _vertex_list.push_back (t);
                                                                        
            t.set(o_x+l_x/2, o_y-l_y/2, o_z-l_z/2); _vertex_list.push_back (t);
            t.set(o_x+l_x/2, o_y-l_y/2, o_z+l_z/2); _vertex_list.push_back (t);
            t.set(o_x+l_x/2, o_y+l_y/2, o_z-l_z/2); _vertex_list.push_back (t);
            t.set(o_x+l_x/2, o_y+l_y/2, o_z+l_z/2); _vertex_list.push_back (t);
            fill_triangle_list();
        } // end Box()

        /**
         * @brief  Standard destructor
         */
        ~Box ()
        {} // end ~Box()

    public: // Access

        /**
         * @brief  Gets the ith vertex.
         *
         * @param index [in]; Index of vertex to return
         * @return A reference to the indexed vertex
         */
        Vector3& operator[](int index)
        {
            return _vertex_list[index];
        }

        /**
         * @brief  Gets the ith face.
         *
         * @param index [in]; Index of vertex to return
         * @return A reference to the Triangle encoding the indexed box face
         */
        Triangle& triangle(int index)
        {
            return _triangle_list[index];
        }

        /**
         * @brief Changes the box's size.
         *
         * @param o_x [in]; Offset in x dimension
         * @param o_y [in]; Offset in y dimension
         * @param o_z [in]; Offset in z dimension
         * @param l_x [in]; Side length in x dimension
         * @param l_y [in]; Side length in y dimension
         * @param l_z [in]; Side length in z dimension
         * @param r_x [in]; Rotation about x axis (unused)
         * @param r_y [in]; Rotation about y axis (unused)
         * @param r_z [in]; Rotation about z axis (unused)
         *
         * @return A reference to the Box object
         */
        Box& set(double o_x, double o_y, double o_z, 
                 double l_x, double l_y, double l_z,
                 double r_x=0, double r_y=0, double r_z=0)
        {
            Vector3 t;     
            _vertex_list.clear();
            t.set(o_x-l_x/2, o_y-l_y/2, o_z-l_z/2); _vertex_list.push_back (t);
            t.set(o_x-l_x/2, o_y-l_y/2, o_z+l_z/2); _vertex_list.push_back (t);
            t.set(o_x-l_x/2, o_y+l_y/2, o_z-l_z/2); _vertex_list.push_back (t);
            t.set(o_x-l_x/2, o_y+l_y/2, o_z+l_z/2); _vertex_list.push_back (t);
                                                                        
            t.set(o_x+l_x/2, o_y-l_y/2, o_z-l_z/2); _vertex_list.push_back (t);
            t.set(o_x+l_x/2, o_y-l_y/2, o_z+l_z/2); _vertex_list.push_back (t);
            t.set(o_x+l_x/2, o_y+l_y/2, o_z-l_z/2); _vertex_list.push_back (t);
            t.set(o_x+l_x/2, o_y+l_y/2, o_z+l_z/2); _vertex_list.push_back (t);
            fill_triangle_list();
            return *this;
        }

    public: // Intersection with lines.

        /**
         * @brief Returns if a line intersects the Box.
         *
         * @param line [in]; Line to test for intersection with the box
         * @return Boolean output: true if the line intersects the box.
         */
        bool line_intersect_test(Line &line);

        /**
         * @brief Returns coordinates of a line/Box intersect.
         *
         * @param line [in]; Line to test for intersection with the box
         * @param intersect [out]; Boolean output: true if the line intersects 
         *                         the box.
         * @return The point that the intersection occurs at, if it occurs. 
         *         Invalid data otherwise.
         */
        Vector3 line_intersect_point(Line &line, bool *intersect);

        /**
         * @brief Returns true if point is inside this box
         *
         * @param point [in]; Point used to determine if point inside box.
         * @return true if point is inside this box
         *         
         */
        bool point_inside_box( const Vector3 &point );

    protected: // Helpers

        /**
         * @brief Fills out the triangle face list.
         * 
         * This uses the vertex list to update the face list of the box.  
         *
         * @return Boolean output: true if the triangle list was filled correctly.
         */
        bool fill_triangle_list()
        {
            //Need proper comparison for positions             
            std::sort (_vertex_list.begin(),
                       _vertex_list.end());
            _triangle_list.clear();
            //Given < sorted vertex listing, create triangles 
            Triangle t;
            //bbb bbt btb btt face 0 1 2 3
            t.set(_vertex_list[0], _vertex_list[1], _vertex_list[2]);       
            _triangle_list.push_back(t);
            t.set(_vertex_list[1], _vertex_list[2], _vertex_list[3]);       
            _triangle_list.push_back(t);

            //bbb bbt tbb tbt face 0 1 4 5
            t.set(_vertex_list[0], _vertex_list[1], _vertex_list[4]);       
            _triangle_list.push_back(t);
            t.set(_vertex_list[1], _vertex_list[4], _vertex_list[5]);       
            _triangle_list.push_back(t);

            //tbb tbt ttb ttt face 4 5 6 7
            t.set(_vertex_list[4], _vertex_list[5], _vertex_list[6]);       
            _triangle_list.push_back(t);
            t.set(_vertex_list[5], _vertex_list[6], _vertex_list[7]);       
            _triangle_list.push_back(t);

            //btb btt ttb ttt face 2 3 6 7
            t.set(_vertex_list[2], _vertex_list[3], _vertex_list[6]);       
            _triangle_list.push_back(t);
            t.set(_vertex_list[3], _vertex_list[6], _vertex_list[7]);       
            _triangle_list.push_back(t);

            //bbb btb tbb ttb face 0 2 4 6
            t.set(_vertex_list[0], _vertex_list[2], _vertex_list[4]);       
            _triangle_list.push_back(t);
            t.set(_vertex_list[2], _vertex_list[4], _vertex_list[6]);       
            _triangle_list.push_back(t);

            //bbt btt tbt ttt face 1 3 5 7
            t.set(_vertex_list[1], _vertex_list[3], _vertex_list[5]);       
            _triangle_list.push_back(t);
            t.set(_vertex_list[3], _vertex_list[5], _vertex_list[7]);       
            _triangle_list.push_back(t);
            return true;
        }

    protected: // Implementation

        VertexList   _vertex_list;   ///< List of box vertices
        TriangleList _triangle_list; ///< List of box faces, cut into triangles
    }; // end class Box

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_MATH_BOX_HPP
