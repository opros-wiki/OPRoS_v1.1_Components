//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_DATA_HPP
#define INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_DATA_HPP


/**
 * @file    evolution/core/behavior/BehaviorData.hpp
 * @brief  Defines a read-only variable-type data structure, to be passed a
 * communication between connected behaviors.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::BehaviorData
 */


#include <string.h>
#include <math.h>
#include <ostream>
#include <evolution/core/base/Array.hpp>
#include <evolution/core/resource/Image.hpp>
#include <evolution/core/math/PointSet.hpp>
#include <evolution/core/base/IObject.hpp>


namespace Evolution
{
    // Forward declarations
    class Image;
    class Vector2;
    template<class T> class Matrix;
    class PointSet;
    class MultiValue;

    /**
     * @brief
     * A read-only variable-type data structure, to be passed a
     * communication between connected behaviors.
     *
     * BehaviorData objects cannot be created or manipulated. Instead,
     * a BehaviorDataWriter object must be created to cast up to
     * BehaviorData when its data must be protected.
     *
     * @see IBehavior, BehaviorData
     *
     * @ingroup core_behavior
     */

    class EVOLUTION_EXPORT_BEHAVIOR BehaviorData
    {
    public: // Types

        /// Behavior data type tags
        enum DataType
        {
            TYPE_UNKNOWN,       ///< Unknown data
            TYPE_BOOLEAN,       ///< Boolean data
            TYPE_ENUM,          ///< Enumerated data type
            TYPE_DOUBLE,        ///< Double precision floating point data
            TYPE_STRING,        ///< String data
            TYPE_MULTIVALUE,    ///< MultiValue data type
            TYPE_MATRIX_DOUBLE, ///< Matrix<double> data type
            TYPE_MATRIX_VECTOR, ///< Matrix<Vector2> data type
            TYPE_VECTOR_FIELD = TYPE_MATRIX_VECTOR, ///< Matrix<Vector2> data type
            TYPE_CHAR_ARRAY,    ///< CharArray data type
            TYPE_DOUBLE_ARRAY,  ///< DoubleArray data type
            TYPE_IMAGE,         ///< Image data type
            TYPE_POINT_SET,     ///< PointSet data type
            TYPE_IOBJECT,       ///< IObject data type
            TYPE_POINTER        ///< Generic (void) pointer data type
        }; // end enum DataType

        /**
         * @brief
         * Returns the string name of a data type.
         *
         * @param data_type [in]; The data type to obtain the name for
         *
         * @return The string name of the data type.
         */
        static const char *data_type_get_name(DataType data_type);

        /**
         * @brief
         * Returns the data type for a given string name.
         *
         * @param name [in]; The data type string name
         * @param data_type_out [in]; The data type identified by 'name'
         *
         * @return true if the string name identified a valid data type.
         */
        static bool data_type_from_name(const char *name,
                                        DataType *data_type_out);


    protected: // Structors (all hidden)

        /**
         * @brief
         * Default constructor.
         */
        BehaviorData ();

        /**
         * @brief
         * Copy constructor.
         *
         * @param other [in]; The BehaviorData object to copy
         */
        BehaviorData (const BehaviorData& other);

        /**
         * @brief
         * Destructor.
         */
        virtual ~BehaviorData ();

    protected: // Assignment (hidden)

        /**
         * @brief
         * Assignment operator.
         * 
         * @param other [in]; The BehaviorData object to be assigned from
         *
         * @return The result of the BehaviorData object assignment.
         */
        BehaviorData& operator= (const BehaviorData& other) { return (*this); }

    public: // Reference count

        /**
         * @brief Adds a reference.
         *
         * @return The current reference count.
         */
        unsigned short add_ref ();

        /**
         * @brief
         * Removes a reference.
         *
         * @return The current reference count.
         */
        unsigned short remove_ref ();

    public: // Access

        /**
         * @brief Returns the data type.
         *
         * @return The data type.
         */
        DataType get_type () const { return (_type); }

        /**
         * @brief Returns the data as a Boolean.
         *
         * @return The boolean value of the behavior data.
         */
        bool get_boolean () const { return (_data._bool); }

        /**
         * @brief Returns a enumerated value.
         *
         * @return The enumberated value of the behavior data.
         */
        long get_enum () const { return (_data._enum); }

        /**
         * @brief Returns the data as a double.
         *
         * @return The double value of the behavior data.
         */
        double get_double () const { return (_data._double); }

        /** 
         * @brief Returns the data as a constant string, if the
         * appropriate type.
         *
         * @return The string value of the behavior data, or 0 if the
         * BehaviorData object does not hold a string value.
         */
        const char* get_string () const
        {           
            if (_type != TYPE_STRING)
            {
                return (0);
            }
            return (_data._string);
        } // end const get_string()

        /** 
         * @brief Returns the data as a constant multivalue pointer, if the 
         * BehaviorData object holds the appropriate type.
         *
         * @return Pointer to the multivalue object contained in the 
         * BehaviorData object.
         */
        const MultiValue* get_multivalue () const
        {
            if (_type != TYPE_MULTIVALUE)
            {
                return (0);
            }
            return (_data._multivalue);
        } // end get_multivalue()

        /**
         * @brief Returns the data as a constant char_array pointer, if the
         * BehaviorData object is of the appropriate type.
         *
         * @return pointer to the CharArray if one is held by the 
         * BehaviorData object. 
         */
        const CharArray* get_char_array () const
        {
            if (_type != TYPE_CHAR_ARRAY)
            {
                return (0);
            }
            return (_data._char_array);
        } // end get_char_array()
        
        /**
         * @brief Returns the data as a constant double_array pointer, if the
         * BehaviorData object is of appropriate type.
         * 
         * @return pointer to the DoubleArray object if one is held
         * by the BehaviorData object.
         */
        const DoubleArray* get_double_array () const
        {
            if (_type != TYPE_DOUBLE_ARRAY)
            {
                return (0);
            }
            return (_data._double_array);
        } // end get_double_array()

        /**
         * @brief Returns the data as a constant image pointer, if the
         * BehaviorData object is of the appropriate type.
         *
         * @return The Image held by the BehaviorData object if the
         * BehaviorData object is of type TYPE_IMAGE.
         */
        Image* get_image () const;

        /**
         * @brief Returns the data as a constant Matrix<double> pointer, if the
         * BehaviorData object is of type TYPE_MATRIX_DOUBLE.
         *
         * @return A pointer to the Matrix<double> object if the BehaviorData
         * object is of type TYPE_MATRIX_DOUBLE.         
         */
        const Matrix<double>* get_matrix () const
        {
            if (_type != TYPE_MATRIX_DOUBLE)
            {
                return NULL;
            }
            return (_data._double_matrix); 
        }

        /**
         * @brief Returns the data as a constant Matrix<Vector2> pointer, if the
         * BehaviorData object is of type TYPE_VECTOR_FIELD.
         *
         * @return A pointer to Matrix<Vector2> if the BehaviorData
         * object is of type TYPE_VECTOR_FIELD.
         */
        const Matrix<Vector2>* get_vector_field () const
        {
            if (_type != TYPE_VECTOR_FIELD)
            {
                return (0);
            }
            return (_data._vector_matrix); 
        }

        /**
         * @brief Returns the data as a constant point set, if the BehaviorData
         * object is of type TYPE_POINT_SET.
         *
         * @return Pointer to the PointSet object help by the BehaviorData
         * object of type TYPE_POINT_SET.
         */ 
        const PointSet* get_point_set () const
        {
            if (_type != TYPE_POINT_SET)
            {
                return (0);
            }
            return (_data._point_set);
        } // end get_pointset()

        /**
         * @brief  Returns an IObject pointer, adding a reference to it.
         *
         * Note that the returned pointer must have remove_ref()
         * called on it when no longer needed.
         *
         * @return An IObject pointer, if the BehaviorData object is of
         * type TYPE_IOBJECT.
         */
        IObject* get_object () const
        {
            if (_type != TYPE_IOBJECT)
            {
                return (NULL);
            }
            _data._object->add_ref ();
            return (_data._object);
        } // end get_object()

    public: // Equality

        /**
         * @brief Compare two behaviors for equality.
         * 
         * Assumes that operator== has been defined for each specific 
         * data type.
         *
         * @param other [in]; The BehaviorData object to compare to
         *
         * @return true if the compared objects are equivalent.
         */
        bool operator== (const BehaviorData& other) const
        {
            // Confirm that types are equal
            DataType type = get_type();
            if (type != other.get_type())
                return false;

            // Compare data contents
            switch (type)
            {
            case TYPE_UNKNOWN:
                return true;
            case TYPE_BOOLEAN:
                return get_boolean() == other.get_boolean();
            case TYPE_ENUM:
                return get_enum() == other.get_enum();
            case TYPE_DOUBLE:
                return (fabs(get_double() - other.get_double()) < .0000001);
            case TYPE_STRING:
                return (strcmp(get_string(), other.get_string()) == 0);
            case TYPE_MULTIVALUE:
                return get_multivalue() == other.get_multivalue();
            case TYPE_DOUBLE_ARRAY:
                return (*get_double_array()) == (*other.get_double_array());
            case TYPE_CHAR_ARRAY:
                return *get_char_array() == *other.get_char_array();
            case TYPE_IMAGE:
                return *get_image() == *other.get_image();
            case TYPE_POINT_SET:
                return *get_point_set() == *(other.get_point_set());
            case TYPE_IOBJECT:
                // Use pointer comparison
                return (_data._object == other._data._object);
            case TYPE_POINTER:
                // Use pointer comparison here too.  And let's
                // not introduce accessor methods just for this.
                return (_data._pointer == other._data._pointer);
            default:
                return (false);
            }
        } // end operator==

        /**
         * @brief Negative comparison.
         * 
         * Compare to see if two BehaviorData objects are not equal.
         * Assumes that operator== has been defined for each specific 
         * data type.
         *
         * @param other [in]; The BehaviorData object to compare to
         *
         * @return true if the compared objects are not equivalent.
         */
        bool operator!= (const BehaviorData& other) const
        {
            return (!(*this == other));
        }

    public: // Output

        /**
         * @brief Print data in human readable format to output stream.
         *
         * @param out [in]; The output stream to print to
         */
        void print (std::ostream& out) const;

    protected: // Implementation

        DataType   _type;  ///< Behavior data type 

        /// Behavior data variant for holding the data
        union
        {
            bool             _bool;
            long             _enum;
            double           _double;
            char*            _string;
            MultiValue*      _multivalue;
            Matrix<double>*  _double_matrix;
            Matrix<Vector2>* _vector_matrix;
            CharArray*       _char_array;
            DoubleArray*     _double_array;
            Image*           _image;
            PointSet*        _point_set;
            IObject*         _object;
            void*            _pointer;
        } _data;

        unsigned short     _ref_count; ///< Behavior data reference count
    };

    /**
     * @brief Print a BehaviorData object to output stream.
     *
     * @param out  [in]; The output stream to print out to
     * @param data [in]; The BehaviorData object to print
     *
     * @return The output stream that was printed out to.
     */
    inline std::ostream&
    operator<< (std::ostream& out, const BehaviorData& data)
    {
        data.print (out);
        return (out);
    }

} // end namespace Evolution

#endif //INCLUDE_EVOLUTION_BEHAVIOR_BEHAVIOR_DATA_HPP
