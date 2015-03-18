//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_DATA_WRITER_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_DATA_WRITER_HPP


/**
 * @file    evolution/core/behavior/BehaviorDataWriter.hpp
 * @brief   A variable type data structure, 
 *          used to pass a communication between connected behaviors.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::BehaviorDataWriter
 */


#include <evolution/core/behavior/BehaviorData.hpp>


namespace Evolution
{


    /**
     * @brief
     * A variable type of data structure, used to pass a communication
     * between connected behaviors.
     *
     * Because behavior data is likely to be passed between shared
     * libraries, and on some platforms this implies heap allocation
     * issues, only methods that perform no allocation or deallocation
     * are inlined. In addition, heap construction should occur only
     * through a call to create_behavior_data().
     *
     * @see  IBehavior
     *
     * @ingroup core_behavior */
    class EVOLUTION_EXPORT_BEHAVIOR BehaviorDataWriter : public BehaviorData
    {
    public: // Structors

        /**
         * @brief
         * Default constructor.
         */
        BehaviorDataWriter ();

        /**
         * @brief
         * Copy constructor.
         *
         * @param other [in]; The BehaviorData object to be copied.
         */
        BehaviorDataWriter (const BehaviorData& other);

        /**
         * @brief
         * Copy constructor.
         *
         * @param other [in]; The BehaviorDataWriter object to be copied.
         */
        BehaviorDataWriter (const BehaviorDataWriter& other);

        /**
         * @brief
         * Destructor.
         */
        ~BehaviorDataWriter ();

        /**
         * @brief
         * Clear the BehaviorDataWriter object of its value.
         */
        void clear ();

    public: // Assignment

        /**
         * @brief Copy data.
         * 
         * @param other [in]; The BehaviorData object to be copied.
         */
        void copy (const BehaviorData& other);

        /**
         * @brief Assignment operator.
         *
         * @param other [in]; The BehaviorData object to be copied by the
         *      assignment.
         *
         * @return A reference to this object after the assignment.
         */
        BehaviorDataWriter& operator= (const BehaviorData& other);

        /**
         * @brief
         * Assignment operator.
         *
         * @param other [in]; The BehaviorDataWriter object to be copied by the
         *      assignment.
         *
         * @return A reference to this object after the assignment.
         */
        BehaviorDataWriter& operator= (const BehaviorDataWriter& other);

    public: // Access

        /**
         * @brief
         * Returns the data as a string, if the BehaviorDataWriter
         * object is of type TYPE_STRING.
         *
         * @return The string value in the BehaviorDataWriter object, or
         *      0 if the BehaviorDataWriter object is not of type
         *      TYPE_STRING.
         */
        char* get_string () const
        { 
            if (_type != TYPE_STRING)
            {
                return (0);
            }
            return (_data._string);
        } // end get_string()

        /**
         * @brief
         * Returns the data as a point set, if the BehaviorDataWriter
         * object is of type TYPE_POINT_SET.
         *
         * @return Pointer to the PointSet object contained by the
         *      BehaviorDataWriter object, or 0 if the BehaviorDataWriter
         *      object is not of type TYPE_POINT_SET.
         */      
        PointSet* get_point_set () const
        {
            if (_type != TYPE_POINT_SET)
            {
                return (0);
            }
            return (_data._point_set);
        } // end get_pointset()

        /**
         * @brief
         * Returns the data as a multi-value pointer, if the BehaviorDataWriter
         * object is of type TYPE_MULTIVALUE.
         *
         * @return Pointer to the Multi-Value object contained by the
         *      BehaviorDataWriter object, or 0 if the BehaviorDataWriter
         *      object is not of TYPE_MULTIVALUE.
         */
        MultiValue* get_multivalue () const
        {
            if (_type != TYPE_MULTIVALUE)
            {
                return (0);
            }
            return (_data._multivalue);
        } // end get_multivalue()

        /**
         * @brief
         * Returns the data as a Matrix<double> pointer, if the 
         *      BehaviorDataWriter object is of type TYPE_MATRIX_DOUBLE.
         *
         * @return Pointer to the Matrix object contained by the
         *      BehaviorDataWriter object, or 0 if the BehaviorDataWriter
         *      object is not of TYPE_MATRIX_DOUBLE.
         */
        Matrix<double>* get_matrix () const
        {
            if (_type != TYPE_MATRIX_DOUBLE)
            {
                return (0);
            }
            return (_data._double_matrix);
        } // end get_matrix()

        /**
         * @brief
         * Returns the data as a Matrix<Vector2> pointer, if the 
         * BehaviorDataWriter object is of type TYPE_VECTOR_FIELD.
         *
         * @return Pointer to the Matrix<Vector2> object contained by the
         *      BehaviorDataWriter object, or 0 if the BehaviorDataWriter
         *      object is not of type TYPE_VECTOR_FIELD.
         */
        Matrix<Vector2>* get_vector_field () const
        {
            if (_type != TYPE_VECTOR_FIELD)
            {
                return (0);
            }
            return (_data._vector_matrix);
        } // end get_vector_matrix()

        /**
         * @brief
         * Returns the data as a constant char_array pointer, if the 
         * BehaviorDataWriter object is of type TYPE_CHAR_ARRAY.
         *
         * @return Pointer to the CharArray object contained by the
         *      BehaviorDataWriter object, or 0 if the BehaviorDataWriter
         *      object is not of type TYPE_CHAR_ARRAY.
         */
        CharArray* get_char_array () const
        {
            if (_type != TYPE_CHAR_ARRAY)
            {
                return (0);
            }
            return (_data._char_array);
        } // end get_char_array()

        /**
         * @brief
         * Returns the data as a double_array, if the BehaviorDataWriter
         * object is of type TYPE_DOUBLE_ARRAY.
         *
         * @return Pointer to the DoubleArray object contained by
         *  the BehaviorDataWriter object, or 0 if the BehaviorDataWriter
         *  object is not of type TYPE_DOUBLE_ARRAY.
         */
        DoubleArray* get_double_array () const
        {
            if (_type != TYPE_DOUBLE_ARRAY)
            {
                return (0);
            }
            return (_data._double_array);
        } // end get_double_array()

    public: // Setting

        /**
         * @brief
         * Sets the data type.
         *
         * @param type [in]; The data type to set for the 
         *      BehaviorDataWriter object.
         */
        void set_type (DataType type);

        /**
         * @brief
         * Stores a Boolean value.
         *
         * @param value [in]; The boolean value to set.
         */
        void set_boolean (bool value)
        {
            set_type (TYPE_BOOLEAN);
            _data._bool = value;
        } // end set_boolean ()

        /**
         * @brief
         * Stores an enumerated value.
         *
         * @param value [in]; The enumerated value to set.
         */
        void set_enum (long value)
        {
            set_type (TYPE_ENUM);
            _data._enum = value;
        } // end set_enum()

        /**
         * @brief
         * Stores a double value.
         *
         * @param value [in]; The double value to set.
         */
        void set_double (double value)
        {
            set_type (TYPE_DOUBLE);
            _data._double = value;
        } // end set_double()

        /**
         * @brief
         * Stores a string value.
         *
         * @param value [in]; The string value to set.
         */
        void set_string (const char* value);
        
        /**
         * @brief
         * Sets the dimensionality of a multi-value.
         *
         * @param dimensionality [in]; The dimensionality of the
         * multivalue.  
         * @param ... [in]; The list of long parameters specifying the dimensionality of 
         * a new dimension of the multivalue.
         */
        void set_multivalue (unsigned long dimensionality, ...);

        /**
         * @brief
         * Sets a multivalue.
         *
         * @param multivalue [in]; The multi-value to set the BehaviorDataWriter
         *      object to.
         */
        void set_multivalue (const MultiValue& multivalue);

        /**
         * @brief
         * Sets a multi-value, taking ownership of the memory.
         *
         * @param multivalue [in]; The pointer to a multi-value to set.  The
         *      ownership of the memory used by the multivalue remains with
         *      the caller, not with the BehaviorDataWriter object.
         */
        void set_multivalue (MultiValue* multivalue);

        /**
         * @brief
         * Sets a matrix of doubles as the value in the BehaviorDataWriter
         * object.
         * 
         * @param double_matrix [in]; The matrix to set in the
         *      BehaviorDataWriter object.
         */
        void set_matrix (const Matrix<double> & double_matrix); 

        /**
         * @brief
         * Sets a matrix of doubles as the value in the BehaviorDataWriter
         * object, keeping ownership of the memory.
         *
         * @param double_matrix [in]; Pointer to the double matrix to set
         *      the BehaviorDataWriter object to, but keeping control of
         *      the memory allocated for the double matrix.
         */
        void set_matrix (Matrix<double> * double_matrix); 

        /**
         * @brief
         * Sets a vector field as the value in the BehaviorDataWriter
         * object.
         *
         * @param vectorfield [in]; The vector field to set in the 
         *      BehaviorDataWriter object.
         */
        void set_vector_field (const Matrix<Vector2> & vectorfield);

        /**
         * @brief
         * Sets a vector field as the value in the BehaviorDataWriter
         * object, but keeps ownership of the vector field's memory.
         * 
         * @param vectorfield [in]; Pointer to the vector field object
         *      to be contained by the BehaviorDataWriter object, with
         *      the caller retaining control over the vector field's
         *      memory.
         */
        void set_vector_field (Matrix<Vector2> * vectorfield);

        /**
         * @brief
         * Sets a CharArray as the value in the BehaviorDataWriter
         * object.
         * 
         * @param char_array [in]; Pointer to the CharArray object
         *      to be contained by the BehaviorDataWriter object.
         */
        void set_char_array (const CharArray& char_array);
        
        /**
         * @brief
         * Sets a CharArray as the value in the BehaviorDataWriter
         * object, but keeps ownership of the CharArray's memory.
         * 
         * @param char_array [in]; Pointer to the CharArray object
         *      to be contained by the BehaviorDataWriter object, with
         *      the caller retaining control over the CharArray's
         *      memory.
         */
        void set_char_array (CharArray* char_array);
        
        /**
         * @brief
         * Sets a DoubleArray as the value in the BehaviorDataWriter
         * object.
         * 
         * @param double_array [in]; Pointer to the DoubleArray object
         *      to be contained by the BehaviorDataWriter object.
         */
        void set_double_array (const DoubleArray& double_array);
        
        /**
         * @brief
         * Sets a DoubleArray as the value in the BehaviorDataWriter
         * object, but keeps ownership of the DoubleArray's memory.
         * 
         * @param double_array [in]; Pointer to the DoubleArray object
         *      to be contained by the BehaviorDataWriter object, with
         *      the caller retaining control over the DoubleArray's
         *      memory.
         */
        void set_double_array (DoubleArray* double_array);

        /**
         * @brief
         * Sets a DoubleArray as the value in the BehaviorDataWriter
         * object by specifying an array of doubles and the size of 
         * the array.
         * 
         * @param double_array [in]; Pointer to an array of doubles to
         *      to be contained by the BehaviorDataWriter object as a
         *      DoubleArray.
         * @param size [in]; The size of the double array to set.
         */
        void set_double_array (const double* double_array, int size);
    
        /**
         * @brief
         * Sets a BehaviorDataWriter object to contain an image.
         *
         * @param image [in]; The image that the BehaviorDataWriter
         *      object will contain.
         */
        void set_image (const Image& image);
    
        /**
         * @brief
         * Sets an image in a BehaviorDataWriter object, while maintaining
         * control of the image's memory.
         * 
         * @param image [in]; Pointer to the image object to set.  The caller
         *      retains control of the memory allocated for the image.
         */
        void set_image (Image* image);

        /**
         * @brief
         * Sets a point set on a BehaviorDataWriter object.
         * 
         * @param point_set [in]; The PointSet object to be contained by
         *      the BehaviorDataWriter object.
         */
        void set_point_set (const PointSet& point_set);

        /**
         * @brief
         * Set a point set as the value of a BehaviorDataWriter object,
         * but keeping ownership of the PointSet's memory.
         * 
         * @param point_set [in]; Pointer to the PointSet object to set
         *      as the value of the BehaviorDataWriter object, with the
         *      caller keeping control of the PointSet's memory.
         */
        void set_point_set (PointSet* point_set);

        /**
         * @brief
         * Set an IObject as the value of a BehaviorDataWriter object and 
         * adds a reference to it.
         * 
         * @param object [in];  Pointer to the IObject object to set as the
         *      BehaviorDataWriter object's value, with a reference added.
         *      The reference will be released when the BehaviorDataWriter
         *      object is destroyed.
         */
        void set_object (IObject* object);

    }; // end class BehaviorData


} // end namespace Evolution


#endif //INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_DATA_HPP
