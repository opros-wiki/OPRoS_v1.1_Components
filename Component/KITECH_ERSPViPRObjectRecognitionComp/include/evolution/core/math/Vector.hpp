//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_MATH_VECTOR_HPP
#define INCLUDE_EVOLUTION_MATH_VECTOR_HPP


/**
 * @file    evolution/core/math/Vector.hpp
 * @brief   Vector template class.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::Vector
 */

#include <math.h>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/Logging.hpp>
#include <evolution/core/math/MathUtils.hpp>
#include <evolution/core/math/Vector3.hpp>


namespace Evolution
{

    /**
     * @class Vector
     *
     * @brief An n-dimensional vector
     *
     * @ingroup core_math
     * @ingroup core_math_aibo
     */
    template <class Type> class Vector
    {
    public: // Structors

        /**
         * @brief Construct a blank vector
         */
        Vector () : _size(0), _data(NULL) {}

        /**
         * @brief Construct a zero-based vector.
         * 
         * @param size [in]; dimension of the newly created vector.
         */
        explicit Vector (unsigned size) : _size(size), _data(new Type[size])
        {
            if (_data == NULL)
            {
                _size = 0;
            }
        }
        
        /**
         * @brief Construct a vector with all elements equal to a constant .
         * @param value [in]; value to initialize vector values to.
         * @param size [in]; dimension of the newly created vector.
         */
        Vector (const Type & value, unsigned size) 
            : _size(size), _data(new Type[size])
        {
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _size; i++)
                {
                    _data[i] = value;
                }
            }
            else
            {
                _size = 0;
            }
        }
        
        /**
         * @brief Construct a vector and initialize it with an array.
         * @param array [in]; data to use to fill new vector
         * @param size [in]; length of array 
         */
        Vector (const Type * array, unsigned size) 
            : _size(size), _data(new Type[size])
        {
            if (_data != NULL)
            {
                memcpy (_data, array, size*sizeof(Type));
            }
            else
            {
                _size = 0;
            }
        }
        
        /**
         * @brief Copy constructor.
         * @param vec [in]; Vector to copy.
         */
        Vector (const Vector<Type> & vec)
            : _size(vec.size ()), _data(new Type[_size])
        {
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _size; i++)
                {
                    _data[i] = vec[i];
                }
            }
            else
            {
                _size = 0;
            }
        }

        /**
         * @brief Copies the data of a Vector3 into this, resizing to 3.
         * @param vec [in]; Vector3 to copy.
         */
        Vector (const Vector3& vec) // Vector3 are always size 3.
            : _size(3), _data(new Type[_size])
        {
            if (_data != NULL)
            {
                _data[0] = vec[0];
                _data[1] = vec[1];
                _data[2] = vec[2];
            }
            else
            {
                _size = 0;
            }
        }   

        /**
         * @brief Destructor.
         */
        ~Vector ()
        {
            clear ();
        }

    public: // Assignment

        /**
         * @brief Assign one vector to another vector.
         * @param vec [in]; Vector to copy.
         * @return reference to *this
         */
        Vector& operator= (const Vector<Type> & vec)
        {
            if (this == &vec)
            {
                return *this;
            }
            
            clear ();
            _size = vec.size ();
            _data = new Type[_size];
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _size; i++)
                {
                    _data[i] = vec[i];
                }
            }
            else
            {
                _size = 0;
            }
            return *this;
        }

        /**
         * @brief Assign a value to all the elements of a vector.
         * @param value [in]; value to set all elements to.
         * @return reference to *this
         */
        Vector& operator= (const Type & value)
        {
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _size; i++)
                {
                    _data[i] = value;
                }                
            }
            return *this;
        }

        /**
         * @brief Copies other into this, starting at offset in this, and going
         *        until one runs out.
         * @param other [in]; vector to copy into *this
         * @param offset [in]; initial offset into this.
         * @return reference to *this
         */
        Vector& copy (const Vector<Type>& other, int offset = 0)
        {
            unsigned ii=0, oii=offset;
            for( ii=0; ii< other.size() && oii < size(); ii++, oii++)
            {
                _data[oii] = other[ii];
            }
            return *this;
        }

    public: // Data access

        /**
         * @brief Retrieve vector element value.
         * @param index [in]; index into vector
         * @return value at index
         */
        Type & operator[] (const int index) 
        { 
            ASSERT ((unsigned)index < _size);
            return _data[index]; 
        }

        /**
         * @brief Constant retrieve of vector element value.
         * @param index [in]; index into vector
         * @return value at index
         */
        const Type & operator[] (const int index) const 
        {
            ASSERT ((unsigned)index < _size);
            return _data[index];
        }

        /**
         * @brief Retrieve vector element value.
         * @param index [in]; index into vector
         * @return value at index
         */
        Type & operator[] (const unsigned int index) 
        { 
            ASSERT (index < _size);
            return _data[index]; 
        }

        /**
         * @brief Constant retrieve of vector element value.
         * @param index [in]; index into vector
         * @return value at index
         */
        const Type & operator[] (const unsigned int index) const 
        { 
            ASSERT (index < _size);
            return _data[index]; 
        }

        /**
         * @brief Size of vector.
         * @return number of elements in vector
         */
        unsigned size() const { return _size; }
       
        /**
         * @brief Return vector as a constant pointer.
         */
        operator const Type * () { return _data; }

        /**
         * @brief Return vector as a constant pointer.
         * @return pointer to internal data array
         */
        const Type * get_data () const { return _data; }

    public: // Modification

        /**
         * @brief Manually set a vector's contents.
         * @param array [in]; array of new elements
         * @param size [in]; number of new elements
         * @return reference to *this
         */
        Vector& set (const Type *array, unsigned size)
        {
            if (array == _data)
            {
                return *this;
            }

            if ((_size != size) || (_data == NULL))
            {
                delete [] _data;
                _size = size;
                _data = new Type[_size];
            }

            if (_data != NULL)
            {
                memcpy (_data, array, _size*sizeof(Type));
            }
            else
            {
                _size = 0;
            }
            return *this;
        }

        /**
         * @brief Manually set a vector's contents.
         *
         * This sets a vectors contents by setting size = (stop - start) /
         * step, then iterating over the vector, setting the 0th element to
         * start, and generating the n'th element by adding step to the n-1'th
         * element.
         *
         * @param start [in]; value to start at
         * @param step  [in]; value for step size
         * @param stop  [in]; value to stop at
         * @return value at index
         */
        Vector& set (const Type start, const Type step, const Type stop)
        {
            unsigned size = (unsigned)ceil((stop - start) / step) + 1;
            if( size != _size)
            {
                clear();
                _size = size; // Size needs to be reassigned right after _data delete.
                _data = new Type[_size];
            }

            if (_data != NULL)
            {
                Type titer = start;
                for (unsigned i = 0; i < _size; i++)
                {
                    _data[i] = titer;
                    titer += step;
                }
                _data[_size - 1] = stop;
            }
            else
            {
                _size = 0;
            }
            return *this;
        } 

        /**
         * @brief Clears out the vector.
         * @return reference to *this
         */
        Vector& clear ()
        {
            if (_data != NULL)
            {
                delete [] _data;
                _data = NULL;
            }
            _size = 0;
            return *this;
        }

        /**
         * @brief Resize the vector and initialize its contents.
         * @param value [in]; value to initialize each element of vector to
         * @param size [in]; length of resized vector
         * @return reference to *this
         */
        Vector& resize (const Type & value, unsigned size)
        {
            if(_size != size)
            {
                clear ();
                _size = size;
                _data = new Type[_size];
            }
            if (_data != NULL)
            {
                for (unsigned i = 0; i < _size; i++)
                {
                    _data[i] = value;
                }
            }
            else
            {
                _size = 0;
            }
            return *this;
        }

        /**
         * @brief Resize the vector and assimilate the buffer.
         * @param array [in]; data to use for new elements
         * @param size [in]; length of array
         * @return reference to *this
         */
        Vector& acquire_buffer (Type * array, unsigned size)
        {
            if(!array) return *this; // Can't be a null vector.

            clear ();
            _size = size;
            _data = array;

            return *this;
        }

        /**
         * @brief Resize the vector, copy the contents if the new array
         *        is bigger than the old one.
         *
         * Resize the vector, copy the contents if the new array is bigger than
         * the old one.
         *
         * As an example, resizing [1 2 3 4] to 6 would result in [1 2 3 4 0 0].
         *
         * @param size [in]; length of new vector
         * @return reference to *this
         */
        Vector& resize (unsigned size)
        {
            if (_data != NULL)
            {
                if (_size == size)      // We have the right size.
                {
                    return *this;
                }
                Type * new_data = new Type[size];
                if (new_data == NULL)
                {
                    clear ();
                }
                else 
                {
                    if (_size < size)  // Copy contents.
                    {
                        memcpy (new_data, _data, _size*sizeof(Type));
                        for(unsigned i = _size;i < size; i++)
                        {
                            new_data[i] = 0;
                        }
                        delete [] _data;
                        _size = size;
                        _data = new_data;
                        return *this;
                    }
                    else 
                    {
                        memcpy (new_data, _data, size*sizeof(Type));
                        delete [] _data;
                        _size = size;
                        _data = new_data;
                        return *this;
                    }
                }
            }

            _size = size;
            _data = new Type[_size];
            if (_data == NULL)
            {
                _size = 0;
            }
            return *this;
        }

        /**
         * @brief Only ensure size is proper (_data is lost if not the
         * correct size). Do _nothing_ else. For speed.
         * @param size [in]; length of new vector
         * @return reference to *this
         */
        Vector& fast_resize (unsigned size)
        {
            if (_data != NULL)
            {
                if (_size == size)      // We have the right size
                {
                    return *this;
                }
                delete[] _data;
            }
            _size = size;
            _data = new Type[_size];
            if (_data == NULL)
            {
                _size = 0;
            }
            return *this;
        }

        /**
         * @brief Reorder from lowest to highest.  Only works if operator< is defined for the
         *  Type of the vector.
         * @return true if successfull, false if error
         */
        bool reorder_ascending ()
        {
            if (_data == NULL)
            {
                return false;
            }
            qsort( _data, _size, sizeof(Type), (int (*) (const void * , const void *))&vector_cmp_function);
            return true;
        }

        /**
         * @brief Changes this vector to the specified subsequence of this vector
         *
         * Copies subsequence from low to high into destination.
         * For example: v = [1 2 3 4 5 6 7 8 9] 
         * v.subsequence(2,4)
         * v == [3 4]
         *
         * @param low [in]; index to start subsequence copy at
         * @param high [in]; index to finish subsequence copy at
         * @return true if successful, false if not
         */
        bool subsequence (unsigned low, unsigned high) 
        {
            if (_data == NULL || high < low )
            {
                return false;
            }
            Vector<Type> temp;
            if(subsequence_copy(low, high, &temp))
            {
                *this = temp;
            }
            else
            {
                return false;
            }
            return true;
        }

        /**
         * @brief Copies subsequence from low to high into destination.
         *
         * Copies subsequence from low to high into destination.
         * For example: v = [1 2 3 4 5 6 7 8 9] 
         * v.subsequence_copy(2,4, &k)
         * k == [3 4]
         *
         * @param low [in]; index to start subsequence copy at
         * @param high [in]; index to finish subsequence copy at
         * @param dest [out]; vector to write subsequence to
         * @return true if successful, false if not
         */
        bool subsequence_copy (unsigned low, unsigned high, Vector<Type>* dest) const
        {
            if (_data == NULL || dest == NULL)
            {
                return false;
            }
            dest->resize((Type)0, high-low);
            unsigned ii=0, jj=0;
            for( ii=low, jj=0; ii<high; ii++, jj++)
            {
                (*dest)[jj] = _data[ii];
            }
            return true;
        }

        /**
         * @brief This function generates a Gaussian kernel of size "size".
         * @param size [in]; size of Gaussian convolution kernel (must be odd)
         * @return reference to *this
         * @pre size%2 == 0 && size >= 3
         */
        Vector& generate_gaussian( unsigned size )
        {
            if(size < 3 || !(size%2)) // Preconditions.
            {
                return *this;
            }
            if(size == 3)
            {
                resize(0.0, 3);
                _data[0] = .25;
                _data[1] = .5;
                _data[2] = .25;
                return *this;
            }
            Vector<Type> kernel; // Make the base Gaussian kernel.
            kernel.resize((Type)0, 3);
            kernel[0] = .25;
            kernel[1] = .5;
            kernel[2] = .25;
           
            int it_num = size /2 - 1;
            *this = kernel; 
            for(int i=0; i < it_num ; i++)
            {
                filter_expand(kernel);
            }
            return *this;
        }

        /**
         * @brief Creates a size length random vector, with min value low and
         *        max value high, uniform distribution.
         * @param size [in]; length of new vector
         * @param low [in]; lowest possible value of an element
         * @param high [in]; highest possible value of an element
         * @return value at index
         */
        bool randomize(unsigned size, const Type &low, const Type &high)
        {
            fast_resize(size);
            if(size != this->size()) return false;
            Type diff = high - low;
            for(int i=0; (unsigned)i<size ; i++)
            {
                _data[i] = Type(rand()/((double) RAND_MAX)) * diff + low;
            }
            return true;
        }

        /**
         * @brief Generates a signal.
         *
         * Generates a random signal.
         * @param size [in]; length of the signal
         * @param maxamp [in]; max value of the signal
         * @param minamp [in]; min value of the signal
         * @param maxstep [in]; max positive change from t-1 to t
         * @param minstep [in]; min positive change from t-1 to t
         * @return true if successful, false otherwise
         */
        bool generate_signal(unsigned size, const Type& maxamp=1.0, const Type& minamp=-1.0, const Type& maxstep=0.1, const Type& minstep=0)
        {
            fast_resize(size);
            if(size != this->size()) return false;
            Type range = maxamp - minamp;
            Type stepwidth = maxstep - minstep;

            _data[0] = Type(rand()/((double) RAND_MAX)) * range;
            for(int i=1; (unsigned)i < size ; i ++)
            {
                // Nothing guarantees this doesn't grow without bound, 
                // aside from the laws of thermodynamics.  
                _data[i] = _data[i-1] + Type(rand()/((double) RAND_MAX)) * stepwidth * Type((rand() % 2) ? -1 : 1);
                if(_data[i] > maxamp) _data[i] = maxamp;
                if(_data[i] < minamp) _data[i] = minamp;
            }
            return true;
        }    

        /**
         * @brief Generates a size length vector, starting at low and incrementing increment at each value.
         * @param size [in]; length of vector to generate
         * @param low [in]; low (initial) value of vector
         * @param increment [in]; increment from element n-1 to element n
         * @return value at index
         */
        bool generate_increasing_vector ( unsigned int size, Type low = 0, Type increment = 1 )
        {
            fast_resize(size);
            unsigned int ii=0;
            Type val = low;
            for( ii=0; ii<size; ii++)
            {
                _data[ii] = val;
                val += increment;
            }
            return true;
        }

    public: // Arithmetic operators.
        /**
         * @brief Add one vector to another vector (if the arrays have
         *        different size, the result has the size of the bigger one).
         * @param vec [in]; vector to add to *this
         * @return reference to *this
         */
        Vector& operator+= (const Vector<Type> & vec)
        {
            if (vec.size () > _size)
            {
                Type * new_data = new Type[vec.size ()];
                if (new_data == NULL)
                {
                    clear ();
                    return *this;
                }
                unsigned i = 0;
                for (i = 0; i < _size; i++)
                {
                    new_data[i] = _data[i] + vec[i];
                }
                for (i = _size; i < vec.size (); i++)
                {
                    new_data[i] = vec[i];
                }
                delete [] _data;
                _size = vec.size ();
                _data = new_data;
            }
            else // vec.size() <= _size, stop after vec.size()
            {
                for (unsigned i = 0; i < vec.size (); i++)
                {
                    _data[i] += vec[i];
                }
            }
            return *this;
        }

        /**
         * @brief Add a constant value to an existing vector.
         * @param value [in]; scalar to add to each element of *this
         * @return reference to *this
         */
        Vector& operator+= (const Type & value)
        {
            for (unsigned i = 0; i < size (); i++)
            {
                _data[i] += value;
            }
            return *this;
        }

        /**
         * @brief Subtract one vector from another vector (if the arrays have
         *        different size, the result has the size of the bigger one).
         * @param vec [in]; vector to subtract from *this
         * @return reference to *this
         */
        Vector& operator-= (const Vector<Type> & vec)
        {
            if (vec.size () > _size)
            {
                Type * new_data = new Type[vec.size ()];
                if (new_data == NULL)
                {
                    clear ();
                    return *this;
                }
                unsigned i = 0;
                for (i = 0; i < _size; i++)
                {
                    new_data[i] = _data[i] - vec[i];
                }
                for (i = _size; i < vec.size (); i++)
                {
                    new_data[i] = -vec[i];
                }
                delete [] _data;
                _size = vec.size ();
                _data = new_data;
            }
            else // vec.size() <= _size, stop after vec.size()
            {
                for (unsigned i = 0; i < vec.size (); i++)
                {
                    _data[i] -= vec[i];
                }
            }
            return *this;
        }

        /**
         * @brief Subtract a constant value to an existing vector.
         * @param value [in]; scalar to subtract from each element of *this
         * @return reference to *this
         */
        Vector& operator-= (const Type & value)
        {
            for (unsigned i = 0; i < size (); i++)
            {
                _data[i] -= value;
            }
            return *this;
        }

        /**
         * @brief Scale a vector.
         * @param scale [in]; value to scale vector by
         * @return reference to *this
         */
        Vector& operator*= (const Type scale)
        {
            for (unsigned i = 0; i < _size; i++)
            {
                _data[i] *= scale;
            }
            return *this;
        }

        /**
         * @brief Return the sum of two vectors.
         * @param vec [in]; vector to add to *this
         * @return Vector contaning vec + *this
         */
        Vector operator+ (const Vector<Type> & vec) const
        {
            Vector<Type> res = (*this);
            return (res += vec);
        }

        /**
         * @brief Return the difference of two vectors.
         * @param vec [in]; vector to subtract from *this
         * @return Vector containing *this - vec
         */
        Vector operator- (const Vector<Type> & vec) const
        {
            Vector<Type> res = (*this);
            return (res -= vec);
        }

        /**
         * @brief Return the scaling of a vector.
         * @param scale [in]; amount to scale vector by
         * @return Vector containing *this * scale
         */
        Vector operator* (const Type scale) const
        {
            Vector<Type> res = (*this);
            return (res *= scale);
        }

    public: // Binary operators.
        /**
         * @brief Test if two vectors are equal.
         * @param vec [in]; vector to compare against
         * @return true if equal, false otherwise
         */
        bool operator== (const Vector<Type> & vec) const
        {
            if (vec.size () != _size)
            {
                return false;
            }
            for (unsigned i = 0; i < _size; i++)
            {
// @todo Need a template equal function!
                if (double_not_equal((double) _data[i], (double) vec[i]))
                {
                    return false;
                } 
            }
            return true;
        }

        /**
         * @brief Test if two vectors are not equal.
         * @param vec [in]; vector to compare against
         * @return true if not equal, false otherwise
         */
        bool operator!= (const Vector<Type> & vec) const
        {
            return !(*this == vec);
        }

        /**
         * @brief Test if the vector is zero.
         * @return true if vector is all zero, false otherwise.
         */
        bool is_zeros ()
        {
            for (unsigned i = 0; i < _size; i++)
            {
// @todo Need a template equal function!
                if (double_not_equal((double) _data[i], 0.0))
                {
                    return false;
                } 
            }
            return true;
        }

    public: // Mathematical computations, elementwise.
        /// Typedef for pointwise function declaration.
        typedef Type (*DfuncType)(Type);

        /**
         * @brief Apply a function, pointwise, to each element of the Vector.
         * @param dfunc [in]; function to apply to selected elements.
         * @return reference to this object
         */
        Vector& pointwise_function( DfuncType dfunc )
        {
            unsigned ii=size();
            if(ii==0) return *this;
            for( ii=0; ii<size(); ii++ )
            {
                _data[ii] = dfunc(_data[ii]);
            }
            return *this;
        }

        /**
         * @brief Apply a function, pointwise, from min to max elements.
         * @param dfunc [in]; function to apply to selected elements.
         * @param min [in]; min index to apply dfunc to
         * @param max [in]; max index to apply dfunc to
         * @return reference to this object
         */
        Vector& pointwise_function( DfuncType dfunc, unsigned min, unsigned max)
        {
            if(min < 0 || max > size()) return *this;
            unsigned ii=size();
            if(ii==0) return *this;
            for( ii=min; ii<max; ii++ )
            {
                _data[ii] = dfunc(_data[ii]);
            }
            return *this;
        }

        /**
         * @brief Apply a function, pointwise, to any ith element where mask[i] > mask_threshold.
         * 
         * @param dfunc [in]; function to apply to selected elements.
         * @param mask [in]; Vector to be used as a mask.
         * @param mask_threshold [in]; threshold for masking.
         * @return reference to this object
         */
        Vector& pointwise_function( DfuncType dfunc, const Vector<double>& mask, double mask_threshold=0.5)
        {
            if(mask.size != size()) return *this;
            unsigned ii=size();
            if(ii==0) return *this;
            for( ii=0; ii<size(); ii++ )
            {
                if( mask[ii] > mask_threshold)
                {
                    _data[ii] = dfunc(_data[ii]);
                }
            }
            return *this;
        }

        /**
         * @brief Square each element.
         * @return reference to this object
         */
        Vector& square_elements()
        {
            unsigned ii=size();
            if(ii==0) return *this;
            for( ii=0; ii<size(); ii++ )
            {
                _data[ii] *= _data[ii];
            }
            return *this;
        }

        /**
         * @brief Square root each element.
         * @return reference to this object
         */
        Vector& sqrt_elements()
        {
            unsigned ii=size();
            if(ii==0) return *this;
            for( ii=0; ii<size(); ii++ )
            {
                _data[ii] = sqrt(_data[ii]);
            }
            return *this;
        }

    public: // Mathematical computations

        /**
         * @brief Return the Euclidean norm of the vector.
         * @return Euclidean norm of this vector.
         */
        double norm()
        {
            return sqrt(this->dot(*this));
        }

        /**
         * @brief Dot product of a vector with another vector.
         * @param vec [in]; vector to dot with
         * @return dot product
         */
        Type dot (const Vector<Type>& vec) const
        {
            PRECOND (_size == vec.size());
            //if(_size != vec.size()) //{   //return nan; //} @todo
            
            Type dot = 0;
            for(unsigned i = 0; i < _size; i++)
            {
                dot += (_data[i] * vec[i]);
            }
            return dot;
        }

        /**
         * @brief Scale this vector by another vector, elementwise.
         * 
         * Scale this vector by another vector, elementwise. 
         * [2 2 2] scale [2 2 3] is [4 4 6]
         * @param vec [in]; vector to scale with
         * @return reference to *this
         * @pre vec.size == this->size()
         */
        Vector& scale (const Vector<Type>& vec)
        {
            PRECOND (_size == vec.size());
            //if(_size != vec.size()) //{   //return nan; //} @todo
            
            for(unsigned i = 0; i < _size; i++)
            {
                _data[i] *= vec[i];
            }
            return *this;
        }
        

        /**
         * @brief Cross product of a vector with another vector. (currently unimplemented)
         * @param v [in]; vector to cross with.
         * @return reference to *this
         */
        Vector& cross (const Vector<Type>& v)
        {
// @todo this should only be valid for 3D vectors!
            return *this;
        }

        /**
         * @brief Filters the vector with the specified kernel double* of length filterlen. Filterlen must be odd.
         * @param filter [in]; filter to apply
         * @param filterlen [in]; length of filter
         * @return reference to *this
         */
        Vector& filter (const Type * filter, unsigned int filterlen)
        {
            if (filterlen % 2 == 0) return *this; // Odd filter lengths only.

            unsigned int f=0, i=0;
            Type acc;
            for(i=0;i<size()-filterlen ;i++)
            {
                acc = 0;
                for(f=0;f<filterlen;f++)
                {
                    acc += _data[i+f] * filter[f];   
                }
                _data[i] = acc;
            }
            for(;i<size();i++)
            {
                _data[i] = 0;
            }
            return *this;
        }

        /**
         * @brief Filters the vector with the specified kernel Vector. Length must be odd.
         * @param filter [in]; filter to apply
         * @return reference to *this
         */
        Vector& filter (const Vector<Type>& filter)
        {
            if (filter.size() % 2 == 0) return *this; // Odd filter lengths only.

            unsigned int f=0, i=0;
            Type acc;
            for(i=0;i<size()-filter.size() ;i++)
            {
                acc = 0;
                for(f=0;f<filter.size();f++)
                {
                    acc += _data[i+f] * filter[f];   
                }
                _data[i] = acc;
            }
            for(;i<size();i++)
            {
                _data[i] = 0;
            }
            return *this;
        }

        /**
         * @brief Filters vector with filter, padding ends with 0s. 
         * This vector is changed to be len + filter.len - 1 long.
         * @param filter [in]; filter to apply
         * @return reference to *this
         */
        Vector& filter_expand (const Vector<Type>& filter)
        {
            if(filter.size() > _size) return *this;
            if(filter.size()%2 == 0) return *this; // Odd filter sizes only.

            int f=0, i=0;
            unsigned newsize   = _size + filter.size() - 1; // Safe, size always at least 1.
            Type *   newbuf    = new Type[newsize];
            int      halfblock = filter.size() / 2 ;
            Type acc;

            for(i=0 ; (unsigned)i < newsize ; i++)
            {
                newbuf[i] = 0;
            }

            // First bit.
            for(i = -halfblock ; i < halfblock ; i++)
            {
                acc = 0;
                for(f = -halfblock ; f <= halfblock ; f++)
                {
                    if(i+f >= 0)
                    {
                        acc += _data[ i + f] * filter[ f + halfblock ];
                    }
                } 
                newbuf[ i + halfblock] = acc; 
            }

            // Last bit.
            for(i = (signed)_size-halfblock ; i <= (signed)_size + halfblock - 1 ; i++)
            {
                acc = 0;
                for(f = -halfblock ; f <= halfblock ; f++)
                {
                    if(i+f < (signed)_size)
                    {
                        acc += _data[ i + f] * filter[ f + halfblock ];
                    }
                } 
                newbuf[ i + halfblock] = acc; 
            }

            // Middle bit.
            for(i = halfblock ; i < (int)(size() - halfblock) ; i++) // Main portion.
            {
                acc = 0;
                for(f = -halfblock ; f <= halfblock ; f++)
                {
                    acc += _data[ i + f ] * filter[ f + halfblock ];   
                }
                newbuf[i + halfblock] = acc;
            }

            acquire_buffer(newbuf, newsize);
            return *this;
        }
   
    public: // Statistical operators

        /**
         * @brief Compute the max value of the vector.
         * @return the maximum value in the matrix
         */
        Type max_value () const
        {
            PRECOND (_data != NULL);
            Type max_value = _data[0];
            for (unsigned i = 1; i < _size; i++)
            {
                if (_data[i] > max_value)
                {
                    max_value = _data[i];
                }
            }
            return max_value;
        }

        /**
         * @brief Compute the min value of the vector.
         * @return the minimum value in the vector
         */
        Type min_value () const
        {
            PRECOND (_data != NULL);
            Type min_value = _data[0];
            for (unsigned i = 1; i < _size; i++)
            {
                if (_data[i] < min_value)
                {
                    min_value = _data[i];
                }
            }
            return min_value;
        }

        /**
         * @brief Compute the mean value of the vector.
         * @return mean of vector
         */
        double mean () const
        {
            PRECOND (_data != NULL);
            PRECOND (_size != 0);
            Type sum = (Type) 0;
            for (unsigned i = 0; i < _size; i++)
            {
                sum += _data[i];
            }
            return (sum/((double) _size));
        }

        /**
         * @brief Compute the mean value of the vector, less the pct/2 upper and pct/2 chunks of the vector.
         * @param pct [in]; percentage of vector's outlying elements to trim before mean calculation
         * @return trimmed mean of vector
         */
        double trimmed_mean (double pct=.10) const
        {
            PRECOND (_data != NULL);
            PRECOND (_size != 0);
            Type sum = (Type) 0;
            unsigned i=0; 
            unsigned lower= unsigned(ceil(_size * (pct/2.0)));
            unsigned upper= unsigned(floor(_size * (1.0 - pct/2.0))); 
            unsigned size = upper - lower ;

            Type * copy = new Type[_size];
            memcpy( copy, _data, _size * sizeof(Type));
            qsort( copy, _size, sizeof(Type), (int (*) (const void * , const void *))&vector_cmp_function);

            for (i = lower; i <upper; i++)
            {
                sum += copy[i];
            }
            delete[] copy;
            return (sum/((double) size));
        }

        /**
         * @brief Compute the sum of the vector's components.
         * @return sum of all vector's elements
         */
        double sum () const
        {
            PRECOND (_data != NULL);
            PRECOND (_size != 0);
            Type sum = (Type) 0;
            for (unsigned i = 0; i < _size; i++)
            {
                sum += _data[i];
            }
            return sum;
        }

        /**
         * @brief Compute the span of the middle 75% of elements.
         * @param pct [in]; percentage of center valued elements to use.
         * @return iqr of vector
         */
        double iqr (double pct=.75) const
        {
            PRECOND (_data != NULL);
            PRECOND (_size != 0);
            pct = 1.0 - pct;
            unsigned lower= unsigned(ceil(_size * (pct/2.0)));
            unsigned upper= unsigned(floor(_size * (1.0 - pct/2.0))); 

            Type * copy = new Type[_size];
            memcpy( copy, _data, _size * sizeof(Type));
            qsort( copy, _size, sizeof(Type), (int (*) (const void * , const void *))&vector_cmp_function);

            double retval = copy[upper] - copy[lower];
            delete[] copy;
            return  retval;
        }
        
        /**
         * @brief Compute the standard deviation of the vector.
         * @return standard deviation of vector
         */
        double std () const
        {
            PRECOND (_data != NULL);
            PRECOND (_size != 0);
            double data_mean = mean ();
            Type sum = (Type) 0;
            for (unsigned i = 0; i < _size; i++)
            {
                sum += (_data[i] - (Type) data_mean) * 
                    (_data[i] - (Type) data_mean);
            }
            return (sqrt(sum/((double) _size)));
        }

        /**
         * @brief Compute the standard deviation of the vector, less the pct/2 upper and pct/2 lower parts of the vector.
         * @param pct [in]; percentage of vector's outlying elements to trim before stddev calculation
         * @return trimmed standard deviation value of vector
         */
        double trimmed_std (double pct=.10) const
        {
            PRECOND (_data != NULL);
            PRECOND (_size != 0);
            double data_mean = trimmed_mean (pct);
            Type sum = (Type) 0;
            unsigned lower= unsigned(ceil(_size * (pct/2.0)));
            unsigned upper= unsigned(floor(_size * (1.0 - pct/2.0))); 
            unsigned size = upper - lower;

            Type * copy = new Type[_size];
            memcpy( copy, _data, _size * sizeof(Type));
            qsort( copy, _size, sizeof(Type), (int (*) (const void * , const void *))&vector_cmp_function);

            for (unsigned i = lower; i < upper; i++)
            {
                sum += (copy[i] - (Type) data_mean) * 
                    (copy[i] - (Type) data_mean);
            }
            delete[] copy;
            return (sqrt(sum/((double) size)));
        }
        
        /**
         * @brief Compute the standard deviation of the vector 
         *        (normalized by N-1).
         * @return unbiased standard deviation of this vector
         */
        double std_unbiased () const
        {
            PRECOND (_data != NULL);
            PRECOND (_size != 0);
            if (_size == 1)
            {
                return ((Type) 0);
            }

            double data_mean = mean ();
            Type sum = (Type) 0;
            for (unsigned i = 0; i < _size; i++)
            {
                sum += (_data[i] - (Type) data_mean) * 
                    (_data[i] - (Type) data_mean);
            }
            return (sqrt(sum/((double) (_size-1))));
        }
        
        /**
         * @brief Compute the median value of the vector.
         * @return median value of vector
         */
        double median ()
        {
            PRECOND (_data != NULL);
            PRECOND (_size != 0);
            if (_size&1) // _size is odd.
            {
                // Need to copy the array because kth_smallest modify it.
                Type *copy = new Type[_size];
                memcpy (copy, _data, _size*sizeof(Type));
                unsigned median_pos = _size/2;
                double median = kth_smallest (copy, _size, median_pos);
                delete [] copy;
                // Return the middle element.
                return median;
            }
            else         // _size is even.
            {
                // Need to copy the array because kth_smallest modify it.
                Type *copy = new Type[_size];
                memcpy (copy, _data, _size*sizeof(Type));
                unsigned median_pos = _size/2 - 1;
                double median = kth_smallest (copy, _size, median_pos);

                memcpy (copy, _data, _size*sizeof(Type));
                median_pos += 1;
                median += kth_smallest (copy, _size, median_pos);
                delete [] copy;
                // Return the mean of the two middle elements.
                return (median/2);
            }
        }

        /**
         * @brief Compute the pct percentile value of the vector.
         *
         * Default behavior computes the 50th percentile of the vector
         * (median).  Any other percentile can be specified, as a value [0,1].
         *
         * @param pct [in]; percentile value to find.
         * @return value at the pct percentile in vector
         */
        double percentile ( double pct = .50)
        {
            PRECOND (_data != NULL);
            PRECOND (_size != 0);
            PRECOND (pct >= 0.0 && pct <= 1.0);
            // Need to copy the array because kth_smallest modify it.
            Type *copy = new Type[_size];
            memcpy (copy, _data, _size*sizeof(Type));
            unsigned pct_pos = unsigned(round(((double)_size - 1.0) * pct));
            double percentile = kth_smallest (copy, _size, pct_pos);
            delete [] copy;
            // Return the percentile element.
            return percentile;
        }

    public: // Output

        /**
         * @brief Print a the contents of a Vector to the specified stream.
         * @param out [in]; ostream to write to
         * @param vec [in]; vector to write
         * @return reference to input ostream
         */
        friend std::ostream& 
        operator<< (std::ostream& out, const Evolution::Vector<Type> & vec)
        {
            unsigned D = vec.size ();
            out << "{\n";
            out << "size:\t" << D << "\n"; 
            out << "vector:\t( ";
            if(D > 0)
            {
                for (unsigned i = 0; i < D-1; i++)
                {
                    out << vec[i] << ", ";
                }
                if (D > 1)
                {
                    out << vec[D-1] << " ";
                }
            }
            out << ")\n";
            return out << "}";
        }

    protected: // Useful for sorting.

        /**
         * @brief Internal, used to compare vectors.
         */
        static int vector_cmp_function(const Type *l, const Type *r)
        {
            return (*l < *r) ? -1 : (*l > *r) ? 1 : 0;
        }

    protected: // Implementation.

        unsigned _size;    ///< Size of the Vector.
        Type     *_data;   ///< Data.

    }; // end class Vector.

    // External operators.
    /**
     * @brief Return the scaling of a vector.
     * @param scale [in]; scalar to multiply
     * @param vec [in]; vector to multiply
     * @return Vector that is scale * vec
     */
    template<class Type>
    inline EVOLUTION_EXPORT_MATH Vector<Type> operator* (const Type scale, 
                                                         const Vector<Type>& vec)
    {
        return (vec * scale);
    }

} // end namespace Evolution.

#endif //INCLUDE_EVOLUTION_MATH_VECTOR_HPP
