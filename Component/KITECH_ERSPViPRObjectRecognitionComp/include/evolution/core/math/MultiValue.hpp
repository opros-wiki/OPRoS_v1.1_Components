//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_MULTI_VALUE_HPP
#define INCLUDE_EVOLUTION_MATH_MULTI_VALUE_HPP


/**
 * @file    evolution/core/math/MultiValue.hpp
 * @see Evolution::Interval
 * @see Evolution::MultiValue
 * @see Evolution::ConstIterator
 * @see Evolution::Iterator
 */

#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <limits>
#include <vector>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Contract.hpp>


namespace Evolution
{

    /**
     * @class Interval
     *
     * @brief
     * Simple structure representing a discretization of a real
     * interval [start, stop] into a finite set of points, each
     * separated by a sub-interval of size "step".
     *
     * @ingroup core_math
     */
    class EVOLUTION_EXPORT_MATH Interval
    {
    public: // Structors

        /**
         * @brief  Constructs an interval from 0 to 0 with stepsize 0 and 1
         * points in it.
         */
        Interval () : _start (0), _step (0), _stop (0), _point_count (1) {}

        /**
         * @brief  Constructs an interval with the specified starting point,
         * step between points, and stoppoing point.
         *
         * For example, if start was 0, stop was 30, and step was 10, then
         * the interval will contain 4 points: 0, 10, 20, 30
         *
         * @param start [in]; Initial value of the interval
         * @param step [in]; Step
         * @param stop [in]; Final value of the interval
         */
        Interval (double start, double step, double stop)
            : _start (start), _step (step), _stop (stop)
        {
            update_point_count ();
        } // end Interval()

    public: // Access

        /**
         * @brief  Returns starting point of interval
         *
         * @return Starting point of the interval
         */
        double get_start () const { return (_start); }

        /**
         * @brief  Returns interval step size
         *
         * @return Step size
         */
        double get_step () const { return (_step); }

        /**
         * @brief  Returns stopping point of interval
         *
         * @return Stop point of the interval
         */
        double get_stop () const { return (_stop); }

        /**
         * @brief  Returns the number of discrete points in the interval.
         * This should be floor((stop-start)/step) + 1.
         *
         * @return Number of samples in the interval
         */
        unsigned long get_point_count () const { return (_point_count); }

    public: // Conversion

        /**
         * @brief  Converts a real coordinate to an index.
         *
         * @param r [in]; Coordinate of a point in the interval
         *
         * @return Index of the given coordinate
         */
        unsigned long real_to_index (double r) const
        {
            volatile double idx = ((r - _start) / _step);
            return ((unsigned long)idx) ;
        }

        /**
         * @brief  Converts an index to a real coordinate.
         *
         * @param i [in]; Index within the interval
         *
         * @return Coordinate corresponding to the given index
         */
        double index_to_real (unsigned long i) const
        {
            return (_step * i + _start);
        }

    public: // Setting

        /**
         * @brief  Modifies the interval's start, step, and stop values.
         *
         * @param start [in]; Initial value of the interval
         * @param step [in]; Step
         * @param stop [in]; Final value of the interval
         */
        void set_interval (double start, double step, double stop)
        {
            _start = start;
            _step = step;
            _stop = stop;
            update_point_count ();
        } // end set_interval()

    public: // Tests

        /**
         * @brief  Tests if two intervals are equal
         * 
         * @param other [in]; Interval to be compared with this Interval.
         *
         * @return Boolean value: "true" if the intervals are equal.
         */
        bool operator== (const Interval& other) const
        {
            return ((fabs (_start - other._start) <= std::numeric_limits<double>::epsilon ()) &&
                    (fabs (_step  - other._step) <= std::numeric_limits<double>::epsilon ()) &&
                    (fabs (_stop  - other._stop) <= std::numeric_limits<double>::epsilon ()));
        }

        /**
         * @brief  Test if two intervals are not equal
         * 
         * @param other [in]; Interval to be compared with this Interval.
         *
         * @return Boolean value: "true" if the intervals are not equal.
         */
        bool operator!= (const Interval& other) const
        {
            return (!operator== (other));
        }

    protected: // Helpers

        /**
         * @brief  Recompute the number of discrete points the interval covers.
         */
        void update_point_count ();

    protected: // Implementation
        double         _start; ///< The start value (beginning) of the interval
        double         _step; ///< The step size of each partition in the interval
        double         _stop; ///< The stop (end) value of the interval
        unsigned long  _point_count; ///< The number of discrete points in the interval
    }; // end class Interval

    /**
     * @brief  A vector of intervals
     */
    typedef std::vector<Interval> IntervalVector;

    /**
     * @brief  Outputs the contents of an interval to an output stream.
     * @param out [in]; Stream to which to print the contents of the Interval.
     * @param ival [in]; Interval whose contents will be printed to the stream.
     *
     * @return Stream reference given in the out parameter.
     */
    inline std::ostream&
    operator<< (std::ostream& out, const Evolution::Interval& ival)
    {
        return out << '[' << ival.get_start () << ':' << ival.get_step () << ':' << ival.get_stop () << ']';
    }


    /**
     * @class MultiValue
     *
     * @brief
     * A discretization of a continuous, multi-dimensional function.
     *
     * Each dimensional variable ranges over a discrete domain defined
     * by an Interval object. A MultiValue is useful for representing
     * a discrete set of values for any multi-dimensional, real
     * function.
     *
     * @ingroup core_math
     * 
     * @see Interval
     */
    class EVOLUTION_EXPORT_MATH MultiValue
    {
    public: // Types

        /**
         * @brief Alias for convenience and cleanliness
         */
        typedef unsigned long SizeType;

        // Forward declarations.
        /// Forward declaration of class ConstIterator
        class EVOLUTION_EXPORT_MATH ConstIterator;
        /// Forward declaration of class Iterator
        class EVOLUTION_EXPORT_MATH Iterator;
        /// Forward declaration of class ConstIterator
        friend class ConstIterator;
        /// Forward declaration of class Iterator
        friend class Iterator;

    public: // Structors

        /**
         * @brief  Constructs a multivalue over 0 input variables
         */
        MultiValue () : _end (get_safe_reference ())
        {
            _dimensionality = _size = 0;
            _intervals = NULL;
            _data = NULL;
        } // end MultiValue ()

        /**
         * @brief  Constructs a multivalue.  The first argument is the
         * number of dimensions (input variables) for the multivalue.
         * Each subsequent argument is an interval defining the domain
         * for an input variable.
         *
         * For example, if A and B are intervals, this is the constructor call
         * that create a multivalue whose inputs range over these intervals:
         *
         * MultiValue f(2, A, B);
         *
         * @param dimensionality [in]; Number of dimensions of the MultiValue
         */
        MultiValue (SizeType dimensionality, ...)
            : _dimensionality (dimensionality), _intervals (NULL), 
              _size (0), _data (NULL), _end (get_safe_reference ())
        {
            va_list args;
            va_start (args, dimensionality);
            set_dimensionality_var (dimensionality, args);
            va_end (args);
        } // end MultiValue (SizeType dimensionality, ...)

        /**
         * @brief  Copy constructor
         *
         * @param other [in]; MultiValue to be copied.
         */
        MultiValue (const MultiValue& other)
            : _dimensionality (0), _intervals (NULL), 
              _size (0), _data (NULL), _end (get_safe_reference ())
        {
            copy (other);
        } // end MultiValue (const MultiValue& other)

        /**
         * @brief  Standard destructor
         */
        ~MultiValue ()
        {
            clear ();
        } // end ~MultiValue()

    public: // Sizing

        /**
         * @brief  Changes the multivalue's dimensionality.  Requires a new set
         * of input intervals.  For example, if the multivalue now has
         * dimensionality 3 over intervals A, B, C:
         *
         * f.set_dimensionality(3, A, B, C);
         *
         * @param dimensionality [in]; Number of dimensions of the MultiValue
         */
        void set_dimensionality (SizeType dimensionality, ...);

        /**
         * @brief Processes va_args for set_dimensionality.
         *
         * @param dimensionality [in]; Number of dimensions of the MultiValue
         * @param args [in]; List of arguments
         */
        void set_dimensionality_var (SizeType dimensionality, va_list args);

        /**
         * @brief  Resets the multivalue to a dimension 0 multivalue.
         */
        void clear ()
        {
            delete[] _intervals;
            delete[] _data;
            _dimensionality = _size = 0;
            _end.set_position ((MultiValue&)(*this), 0);
        } // end clear()

    public: // Information

        /**
         * @brief Converts an index on a dimension to the point on that
         * dimension's interval.
         *
         * @param  index     [in]; Index into dimension "dimension"
         * @param  dimension [in]; Dimension of the multivalue from which to get data.
         */
        double index_to_point (SizeType index, SizeType dimension) const
        {
            PRECOND (dimension < _dimensionality);
            PRECOND ((index >= 0) && (index < (_intervals [dimension].get_point_count ())));
            return ((_intervals [dimension].get_start ()) + index * (_intervals [dimension].get_step ()));
        } // end index_to_point ()

    public: // Tests

        /**
         * @brief  Tests if two MultiValue have the same dimensions and
         * intervals.
         * @param other [in]; MultiValue to be compared with this MultiValue.
         *
         * @return Boolean value: "true" if both MultiValues have the
         * same dimensions and intervals.
         */
        bool is_compatible (const MultiValue& other) const;

        /**
         * @brief  Operator == : tests if two MultiValue are equal.
         *
         * @param other [in]; MultiValue to be compared with this MultiValue.
         *
         * @return Boolean value: "true" if the MultiValues are equal.
         */
        bool operator== (const MultiValue& other) const;

        /**
         * @brief  Operator != : tests if two MultiValue are not equal.
         *
         * @param other [in]; MultiValue to be compared with this MultiValue.
         *
         * @return Boolean value: "true" if the MultiValues are not equal.
         */
        bool operator!= (const MultiValue& other) const { return (!operator== (other)); }

    public: // Access

        /**
         * @brief Returns the dimensionality, the number of dimensions
         * of the data.
         *
         * @return Number of dimensions of the data
         */
        SizeType get_dimensionality () const { return (_dimensionality); }

        /**
         * @brief Returns the specified interval.
         *
         * @param dimension [in]; Index of dimension
         *
         * @return Interval corresponding to the given dimension
         */
        const Interval& get_interval (SizeType dimension) const
        {
            ASSERT (dimension < _dimensionality);
            return (_intervals [dimension]);
        } // end get_interval()

        /**
         * @brief Operator () : accesses a MultiValue.
         *
         * @pre  Indices are valid.
         *
         * For example, to access the value at point (x,y,z), you would do:
         * double value = my_multivalue(x, y, z);
         *
         * @param i1 [in]; Index in the MultiValue
         *
         * @return Value at given indices
         */
        const double operator() (SizeType i1, ...) const
        {
            // Store the index arguments.
            va_list args;
            va_start (args, i1);
            SizeType index = calculate_raw_index (i1, args);
            va_end (args);
            return (_data [index]);
        } // end const operator()

        /**
         * @brief Accesses a MultiValue.
         *
         * @pre  Indices are valid.
         *
         * For example, to access the value at point (x,y,z), you would do:
         * double value = my_multivalue.get_value(x, y, z);
         *
         * @param i1 [in]; Index in the MultiValue
         *
         * @return Value at given indices
         */
        double get_value (SizeType i1, ...) const
        {
            // Store the index arguments.
            va_list args;
            va_start (args, i1);
            SizeType index = calculate_raw_index (i1, args);
            va_end (args);
            return (_data [index]);
        } // end get_value()

        /**
         * @brief Accesses a MultiValue.
         *
         * @pre  Indices are valid.
         *
         * For example, to access the value at point (x,y,z), you would do:
         * SizeType ind[] = {x, y, z};
         * double value = my_multivalue.get_value(ind);
         *
         * @param indices [in]; Array of indices in the MultiValue
         *
         * @return Value at given indices
         */
        double get_value(SizeType indices[]) const 
        {
            SizeType index = calculate_raw_index(indices);
            return (_data [index]);
        }

    public: // Setting

        /**
         * @brief Operator = : copies one multivalue to the current MultiValue.
         *
         * @param other [in]; MultiValue to be copied.
         * @return A reference to this MultiValue
         */
        MultiValue& operator= (const MultiValue& other)
        {
            copy (other);
            return (*this);
        } // end operator=()

        /**
         * @brief Operator () : accesses a MultiValue.
         *
         * @pre  Indices are valid.
         *
         * For example, to access the value at point (x,y,z), you would do:
         * double value = my_multivalue(x, y, z);
         *
         * @param i1 [in]; Index in the MultiValue
         *
         * @return Reference to the value at given indices
         */
        double& operator() (SizeType i1, ...)
        {
            // Store the index arguments.
            va_list args;
            va_start (args, i1);
            SizeType index = calculate_raw_index (i1, args);
            va_end (args);
            return (_data [index]);
        } // end operator()

        /**
         * @brief Sets an element in the MultiValue.
         *
         * For example, to set the value at point (x,y,z) to, you would do:
         * my_multivalue.set_value(a, x, y, z);
         *
         * @param value [in]; Value to be set.
         * @param i1 [in]; Index in the MultiValue
         */
        void set_value (double value, SizeType i1, ...)
        {
            // Store the index arguments.
            va_list args;
            va_start (args, i1);
            SizeType index = calculate_raw_index (i1, args);
            va_end (args);
            _data [index] = value;
        } // end set_value()
        
        /**
         * @brief Sets an element in the MultiValue.
         *
         * For example, to set the value at point (x,y,z) to, you would do:
         * SizeType ind[] = {x, y, z};
         * my_multivalue.set_value(a, ind);
         *
         * @param value [in]; Value to be set.
         * @param indices [in]; Array of indices in the MultiValue
         */
        void set_value(double value, SizeType indices[]) const 
        {
            SizeType index = calculate_raw_index(indices);
            _data[index] = value;
        }

        /**
         * @brief Copies one multivalue to the current MultiValue.
         *
         * @param other [in]; MultiValue to be copied.
         */
        void copy (const MultiValue& other);

    protected: // Helpers

        /**
         * @brief From a list of dimensional indices, calculates the index
         * into the flat, implementation array.
         *
         * The flat, data index is the sum of each dimensional index
         * multiplied by the product of the partition sizes of less
         * significant intervals. That is, with indices I0, I1, .. , In
         * on dimensions D0, D1, ... Dn, the flat data index is:
         *
         *   data_index = I0 * D1 * .. * Dn + I1 * D2 * .. * Dn + .. + In
         *
         * @param index0 [in]; Index in the MultiValue
         * @param args [in]; List of arguments
         *
         * @return Index in the flat implementation array
         *
         * @pre  Indices are valid.
         */
        SizeType calculate_raw_index (SizeType index0, va_list args) const;
        
        /**
         * @brief From a list of dimensional indices, calculates the index
         * into the flat, implementation array.
         *
         * The flat, data index is the sum of each dimensional index
         * multiplied by the product of the partition sizes of less
         * significant intervals. That is, with indices I0, I1, .. , In
         * on dimensions D0, D1, ... Dn, the flat data index is:
         *
         *   data_index = I0 * D1 * .. * Dn + I1 * D2 * .. * Dn + .. + In
         *
         * @param indices [in]; Array of indices in the MultiValue
         *
         * @return Index in the flat implementation array
         *
         * @pre  Indices are valid.
         */
        SizeType calculate_raw_index(SizeType indices[]) const;

    public: // Iterators

        /**
         * @brief Obtains a read-only iterator at the beginning of the data.
         *
         * @return Iterator pointing at the beginning of the MultiValue
         */
        ConstIterator begin () const { return ConstIterator ((MultiValue&)(*this), _data);}

        /**
         * @brief Obtains a read-write iterator at the beginning of the data.
         *
         * @return Iterator pointing at the beginning of the MultiValue
         */
        Iterator begin () { return Iterator (*this, _data); }

        /**
         * @brief Obtains a read-only iterator at the end of the data.
         *
         * @return Iterator pointing at the end of the MultiValue
         */
        ConstIterator end () const { return (_end); }

        /**
         * @brief Obtains a read-write iterator at the end of the data.
         *
         * @return Iterator pointing at the end of the MultiValue
         */
        Iterator end () { return (_end); }

    public: // Raw access

        /**
         * @brief Obtains the value at the specified raw index.
         *
         * @param index [in]; Index in the flat implementation array
         *
         * @return Value stored at index location
         */
        double get_raw_value (SizeType index) const { return(_data [index]); }

        /**
         * @brief Sets the value at the specified raw index.
         *
         * @param value [in]; Value to be set.
         * @param index [in]; Index in the flat implementation array
         */
        void set_raw_value (double value, SizeType index) const { _data [index] = value;}

        /**
         * @brief Obtains the raw data storage.
         *
         * @return Pointer to the flat implementation array
         */
        double* get_raw_data () { return (_data);}

        /**
         * @brief Obtains the raw data storage.
         *
         * @return Pointer to the flat implementation array
         */
        const double* get_raw_data () const { return (_data);} 

        /**
         * @brief Obtains the size of the raw data.
         *
         * @return Number of elements in the flat implementation array
         */
        SizeType get_raw_size () const { return (_size); }

        /**
         * @brief Calculates the dimensional indices of the given raw
         * index position. Basically the inverse of
         * calculate_raw_index(). The user of this function is assumed
         * to allocate the memory for indices. Improper allocation
         * might result in unexpected errors.
         *
         * @param  raw_index [in]; Raw index into the flat implementation array to convert to a set of discrete indices.
         * @param  indices [out]; Output array of indices, of size equal to the dimensionality of the Multivalue.
         *
         * @return Pointer to indices (second argument of the function)
         */
        SizeType* raw_index_to_indices (SizeType raw_index, SizeType* indices) const;

    public: // Operands

        /**
         * @brief Operator += : adds one MultiValue to the current one
         * and stores the sum in the current MultiValue.
         *
         * @param input [in]; MultiValue to be added to the this MultiValue.
         * @return A reference to this MultiValue
         *
         * @pre Both multivalues have the same dimensionality.
         */
        MultiValue& operator+= (const MultiValue& input);

        /**
         * @brief  Scales a MultiValue by a scalar value (double).
         *
         * @param scalar [in]; Value to be used to scale each element of the MultiValue.
          * @return A reference to this MultiValue
        */
        MultiValue& operator*= (const double scalar);

    public: // Output

        /**
         * @brief Print the contents of a MultiValue to the specified stream.
         * @param out [in]; Stream to which to print the contents of MultiValue.
         */
        void print (std::ostream& out) const;

    public: // Type definitions

        /**
         * @class ConstIterator
         *
         * @brief An iterator that allows read-only access to a MultiValue
         *
         * @ingroup core_math
         */
        class EVOLUTION_EXPORT_MATH ConstIterator
        {
        public: // Types
            /**
             * @brief Alias for convenience and cleanliness
             */
            typedef MultiValue::SizeType SizeType; 

        public: // Structors

            /**
             * @brief Creates a constant iterator over a multivalue.
             *
             * @param mv [in]; MultiValue to be indexed by the iterator.
             * @param pos [in]; Current position of the iterator in the multivalue.
             */
            ConstIterator (MultiValue& mv, double* pos = 0) : _mv (&mv), _pos (pos) {}

            /**
             * @brief Standard destructor
             */
            ~ConstIterator () {}

        public: // Assignment

            /**
             * @brief Operator = : copies one iterator to the current one.
             *
             * @param other [in]; Iterator to be copied.
             * @return A reference to this ConstIterator
             */
            ConstIterator& operator= (const ConstIterator& other)
            {
                _mv = other._mv;
                _pos = other._pos;
                return (*this);
            } // end operator=()

            /**
             * @brief Sets the iterator's position in the multivalue to the
             * given position pointer
             *
             * @param mv [in]; MultiValue to be indexed by the iterator.
             * @param pos [in]; Current position of the iterator in the multivalue.
             */
            void set_position (MultiValue& mv, double* pos)
            {
                _mv = &mv;
                _pos = pos;
            } // end set_position()

        public: // Access

            /**
             * @brief Obtains the current value at the iterator's location.
             *
             * @return Value at iterator's position
             */
            double get_value () const
            {
                return (*_pos);
            } // end get_value()

        public: // Tests

            /**
             * @brief  Operator == : tests if two iterator are equal.
             *
             * @param other [in]; Iterator to be compared with this iterator.
             *
             * @return Boolean value: "true" if the iterators are equal.
             */
            bool operator== (const ConstIterator& other) const
            {
                return (_pos == other._pos);
            } // end operator==()

            /**
             * @brief  Operator != : tests if two iterator are equal.
             *
             * @param other [in]; Iterator to be compared with this iterator.
             *
             * @return Boolean value: "true" if the iterators are not equal.
             */
            bool operator!= (const ConstIterator& other) const
            {
                return (_pos != other._pos);
            } // end operator==()

        public: // Iteration

            /**
             * @brief Iterates to the next coordinate in the multivalue.
             * @return A reference to this ConstIterator
             */
            ConstIterator& operator++ ()
            {
                ++_pos;
                return (*this);
            } // end operator++()

        public: // Calculation

            /**
             * @brief For the current iterator position, obtains the
             * equivalent index in the specified dimension.
             *
             * @param indices [out]; Output array of indices, of size
             * equal to the dimensionality of the Multivalue.
             *
             * @return Pointer to indices (second argument of the function)
             */
            SizeType* get_current_indices (SizeType* indices) const
            {
                return (_mv->raw_index_to_indices (SizeType (_pos - _mv->get_raw_data ()),
                                                   indices));
            } // end get_index()

        protected: // Implementation

            MultiValue*  _mv;  ///< MultiValue that this iterator iterates over.
            double*      _pos; ///< Pointer to the currently selected output value of the multivalue.
        }; // end inner class ConstIterator


        /**
         * @class Iterator
         *
         * @brief An iterator that allows read and write access to a
         * MultiValue
         *
         * @ingroup core_math
         */
        class EVOLUTION_EXPORT_MATH Iterator : public ConstIterator
        {
        public: // Structors

            /**
             * @brief Constructs a read/write iterator
             * @param mv [in]; MultiValue to be indexed by the iterator.
             * @param pos [in]; Current position of the iterator in the multivalue.
             */
            Iterator (MultiValue& mv, double* pos = 0) : ConstIterator (mv, pos) {}

        public: // Setting

            /**
             * @brief Sets the multivalue output point that this iterator
             * refers to as "value".
             *
             * @param value [in]; Value to be set
             */
            void set_value (double value) { *_pos = value; }

        }; // end inner class Iterator

    protected: // Helpers

        /**
         * @brief Fool compilers into allowing reference to this in
         * initializer list.
         *
         * @return A reference to this MultiValue
         */
        MultiValue& get_safe_reference () { return (*this); }

    protected: // Implementation
        SizeType   _dimensionality; ///< The dimension of the multivalue (number of inputs)
        Interval*  _intervals; ///< A list of the input interval ranges
        SizeType   _size; ///< The total size of the data array
        double*    _data; ///< The actual output data values
        Iterator   _end; ///< An iterator pointing to the end of the data array
    }; // end class MultiValue


    // External MultiValue operators

    /**
     * @brief Returns the sum of two multivalues
     *
     * @pre Input multivalues must have the same dimension and intervals
     */
    inline MultiValue
    operator+ (const MultiValue& a, const MultiValue& b)
    {
        MultiValue output(a);
        output += b;
        return output;
    }

    /**
     * @brief Returns the scaling of a multivalue by a scalar value
     */
    inline MultiValue
    operator* (const MultiValue& mv, const double scalar)
    {
        MultiValue output(mv);
        output *= scalar;
        return output;
    }

    /**
     * @brief Returns the scaling of a multivalue by a scalar value
     */
    inline MultiValue
    operator* (const double scalar, const MultiValue& mv)
    {
        return mv * scalar;
    }


    /**
     * @brief Print a multivalue to an output stream
     */
    inline std::ostream& 
    operator<< (std::ostream& out, const Evolution::MultiValue& mv)
    {
        mv.print (out);
        return (out);
    }

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_MATH_MULTI_VALUE_HPP
