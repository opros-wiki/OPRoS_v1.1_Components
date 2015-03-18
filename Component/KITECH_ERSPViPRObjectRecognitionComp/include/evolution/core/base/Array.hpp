//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


/**
 * @file   evolution/core/base/Array.hpp
 * @brief  Encapsulates contiguous memory arrays.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::Array
 * @see Evolution::CharArray
 * @see Evolution::ShortArray
 * @see Evolution::LongArray
 * @see Evolution::PointerArray
 * @see Evolution::DoubleArray
 */

#ifndef INCLUDE_EVOLUTION_BASE_ARRAY_HPP
#define INCLUDE_EVOLUTION_BASE_ARRAY_HPP

#include <iostream>
#include <sys/types.h>
#include <string.h>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ResultCodes.hpp>

namespace Evolution
{

    /**
     * @class Array
     * @brief Encapsulates contiguous memory arrays.
     *
     * This is an array base class encapsulating contiguous memory arrays 
     * without templates or STL.  No virtual method is defined for 
     * efficiency reasons and because no polymorphism among arrays 
     * of different types is expected.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE Array
    {
    public:  // Constants

        enum { INITIAL_CAPACITY = 32 };
        
    public: // Structors
        /**
         * @brief
         * Constructor
         *
         * @param element_size [in]; The size of each element in the array
         */
        
        Array(size_t element_size);
       
        /**
         * @brief Standard copy constructor
         *
         * @param array [in]; The array to be copied
         */
        Array(const Array& array);

        /**
         * @brief Sets one array equal to another array.
         *
         * @param array [in]; The array to be assigned
         *
         * @return A reference to the assigned array.
         */
        const Array& operator= (const Array& array);
        
        /**
         * @brief
         * Destructor.
         */
        ~Array();

    public: // Equality

        /**
         * @brief Equality test.
         *
         * @param array [in]; The array to be compared against
         *
         * @return True if the arrays are equal or false otherwise.
         */
        bool operator== (const Array& array) const;

        /**
         * @brief Inequality test.
         *
         * @param array [in]; The array to be compared against
         *
         * @return True if the arrays are not equal or false otherwise.
         */
        bool operator!= (const Array& array) const { return !(*this == array); }

    public: // Accessors

        /**
         * @brief Returns the size of each element.
         *
         * @return The size of the element.
         */
        size_t element_size() const;

        /**
         * @brief Returns the number of elements in the array.
         *
         * @return The number of elements in the array.
         */
        size_t size() const;

        /**
         * @brief Returns the array's current allocated capacity in number of elements.
         *
         * @return The capacity of the array.
         */
        size_t capacity() const;

        /**
         * @brief Returns the array expansion step size.
         *
         * @return The step size.
         */
        size_t step_size() const;

        /**
         * @brief Return the array data buffer pointer.
         *
         * @return The data buffer.
         */
        const void* data() const;
  
        /**
         * @brief Guarantees that size() returns a specified size.
         *
         * @param size [in]; The size of the array
         */
        void set_size(size_t size);

    public: // Modifiers

        /**
         * @brief Set the array expansion step size.
         *
         * @param step_size [in]; The new expansion step size
         */
        void set_step_size(size_t step_size);

        /**
         * @brief Clears the array.
         */
        void clear();

        /**
         * @brief Pushes new elements onto the back of the array.
         *
         * Add one or more new elements to the end of the array, and increases
         * array capacity as necessary.
         * 
         * @param new_data      [in]; Pointer to the new elements
         * @param element_count [in]; Number of new elements to add to array
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        Result  push_back_data(const void* new_data, size_t element_count = 1);
            
        /**
         * @brief Removes one or more elements from the array.
         *
         * @param start_index [in]; index to begin removal
         * @param element_count [in]; number of elements to delete
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        Result  remove(size_t start_index, size_t element_count = 1);
                
        /**
         * @brief Flattens the array into a contiguous memory buffer, prefixed
         * by the array's properties.
         * 
         * Creates a buffer whose size is the sum of the Array structure 
         * size and the Array data size.  The structure is copied in first,
         * and the the data is copied in after.  The buffer is allocated
         * on the heap and must be cleaned up with delete[] after use.
         * An array can be reconstructed from a flattened buffer with
         * the unflatten call.
         * 
         * @param size [out]; The size of the buffer
         * 
         * @return  A pointer to the flat buffer, a char array allocated on 
         *          the heap.  Must be cleaned up with delete[] after use.
         */
        char* flatten(size_t& size) const;
        
        /**
         * @brief Unflatten an array from a flattened array buffer.
         *
         * The type of the source and destination arrays must be the same.
         * If they are not, the result is undetermined.
         * 
         * @param buffer [in]; The flattened array buffer to be reconstructed
         *
         * @return  RESULT_SUCESS on success, or a result code describing the failure.
         */
        Result unflatten(const char* buffer);
                    
        /**
         * @brief Guarantees that size() returns the value passed in the
         * new_size parameter, padding the array with 0's if necessary.
         *                    
         * @param new_size [in]; The desired capacity
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        Result resize(size_t new_size);
            
        /**
         * @brief Set the array capacity to a specified value with an option
         * to preserve the previous array values as much as possible.
         *                    
         * @param capacity [in]; The desired capacity
         * @param preserve [in]; Preserve previous array data if true
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        Result  set_capacity(size_t capacity, bool preserve = true) ;
        
    protected: // Helpers. 
        /**
         * @brief Allocates memory for a given element count.
         *
         * @param element_count [in]; The number of elements for which to
         * allocate space
         * 
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        Result  allocate(size_t element_count);
       
        /**
         * @brief Calculates the next capacity given some expected expansion of
         * the array.
         *
         * The current algorithm for expanding array is to add capacities
         * in _step_size increments or double the capacity each time if 
         * _step_size = 0.
         *
         * @param expansion [in]; Expected expansion of array
         * 
         * @return The next capacity after the specified array expansion,
         *         in element counts. 
         */
        size_t next_capacity(size_t expansion) const;
                
        /**
         * @brief Expands the array by a specified expansion value.
         *          
         * @param expansion [in]; The amount to expand, in element count
         *                           
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        Result  expand(size_t expansion) ;
       
        /**
         * @brief Get the pointer to a specified element in the array.
         *
         * @param index [in]; The index to the desired element
         *
         * @return  The pointer to the desired element or 0 if
         *          the index is out of bound.
         */
        const char* get_element_pointer(size_t index) const;
            
    protected:

        /**
         * @brief The size, in bytes, of each array element.
         */
        size_t     _element_size;

        /**
         * @brief The number of elements defined in the array.
         */
        size_t     _size;

        /**
         * @brief The number of elements the array is allocated to store.
         */
        size_t     _capacity;

        /**
         * @brief The capacity increment when more space is needed.
         */
        size_t     _step_size;

        /**
         * @brief The pointer to the array's data.
         */
        char*           _data;
    }; // end class Array



    /**
     * @class CharArray
     * @brief An array of characters.
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE CharArray : public Array
    {
    public:
        /**
         * @brief Constructor.
         */
        CharArray();
        /**
         * @brief Constructor.
         *
         * @pre data is valid pointer.
         *
         * @param data [in]; A pointer to char data
         * @param size [in]; Number of char elements in data
         */
        CharArray(const char * data, int size);

        /**
         * @brief Constructor.
         *
         * @pre data is a valid pointer.
         *
         * @param data [in] A null terminated buffer
         */
        CharArray(const char * data);

        /**
         * @brief Destructor.
         */
        ~CharArray();

        /**
         * @brief Returns the indexed element.
         *
         * @pre index < size();
         *
         * @param index [in]; The 0-based index of the desired element
         *
         * @return  The indexed element if found, or 0 if
         *          the index is out of bound.
         */
        char& operator[] (size_t index) const;

        /**
         * @brief Const raw access.
         *
         * @return The raw const data.
         */
        const char* get_char_data () const { return (_data); }

        /**
         * @brief Raw access.
         *
         * @return The raw char data.
         */
        char* get_char_data () { return (_data); }

        /**
         * @brief Push a single new element to the back of the array,
         * expand if necessary.
         *
         * @param element [in]; A value to be inserted into the array
         *
         * @return  RESULT_SUCCESS on success, or an error result on failure.
         */
        Result push_back(char element);
    };        


    /**
     * @brief Print a CharArray to output stream.
     *
     * @param out   [in]; The output stream to print out to
     * @param array [in]; The CharArray to print
     *
     * @return The output stream that was printed out to.
     */
    inline std::ostream&
    operator<< (std::ostream& out, const CharArray& array)
    {
        unsigned int print_length;

        // Opening bracket
        out << "[";

        if (array.size() > 256)
        {
            print_length = 256;
        }
        else
        {
            print_length = (unsigned int) array.size();
        }

        // Data contents
        for (unsigned int i = 0; i < print_length; i++)
        {
            unsigned char ch = array[i];
            if (isprint(ch))
            {
                out << " " << ch;
            }
            else
            {
                out << " 0x" << std::hex << (unsigned int) ((unsigned char) ch) << std::dec;
            }
        }

        if (print_length < array.size())
        {
            out << " ...(" << (array.size() - print_length) << " of "
                << array.size() << " not shown)";
        }

        // Closing bracket
        out << " ]";
        return out;
    }

    /**
     * @class ShortArray
     * @brief An array of shorts.
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE ShortArray : public Array
    {
    public: // Structors
        /**
         * @brief Constructor.
         */
        ShortArray();

        /**
         * @brief Constructor.
         *
         * @pre data is valid pointer.
         *
         * @param data [in]; Pointer to short data
         * @param size [in]; Number of short elements in data
         */
        ShortArray(const short* data, size_t size);

        /**
         * @brief Destructor.
         */
        ~ShortArray();
            
        /**
         * @brief Returns the indexed element.
         *
         * @pre index < size();
         *
         * @param index [in]; The 0-based index of the desired element
         *
         * @return  The indexed element if found, or 0 if
         *          the index is out of bound.
         */
        short& operator[] (size_t index) const;

        /**
         * @brief Push a single new element to the back of the array,
         * and expand if necessary.
         *
         * @param element [in]; A value to be inserted into the array
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        Result push_back(short element);
    }; // end class ShortArray



    /**
     * @class LongArray
     * @brief An array of longs.
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE LongArray : public Array
    {
    public:
        /**
         * @brief Constructor.
         */
        LongArray();

        /**
         * @brief Constructor.
         *
         * @pre data is valid pointer.
         *
         * @param data [in]; Pointer to long data
         * @param size [in]; Number of long elements in data
         */
        LongArray(const long* data, size_t size);

        /**
         * @brief Destructor.
         */
        ~LongArray();
            
        /**
         * @brief Returns the indexed element.
         *
         * @pre index < size();
         *
         * @param index [in]; The 0-based index of the desired element
         *
         * @return  The indexed element if found, or 0 if
         *          the index is out of bound.
         */
        long& operator[] (size_t index) const;

        /**
         * @brief Pushes a single new element to the back of the array,
         * expands if necessary.
         *
         * @param element [in]; A value to be inserted into the array
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        Result push_back(long element);
    }; // end class LongArray



    /**
     * @class PointerArray
     * @briefAn array of pointers. 
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE PointerArray : public Array
    {
    public:
        /**
         * @brief
         * Constructor.
         */
        PointerArray();

        /**
         * @brief Constructor.
         *
         * @pre data is valid pointer.
         *
         * @param data [in]; Pointer to void* data
         * @param size [in]; Number of void* elements in data
         */
        PointerArray(const void** data, size_t size);

        /**
         * @brief Destructor.
         */
        ~PointerArray();
            
        /**
         * @brief Returns the indexed element.
         *
         * @pre index < size();
         *
         * @param index [in]; The 0-based indexed of the desired element
         *
         * @return  The indexed element if found, or 0 if
         *          the index is out of bound.
         */
        void*& operator[] (size_t index) const;
        /**
         * @brief Push a single new element to the back of the array,
         * expand if necessary.
         *
         * @param element [in]; A value to be inserted into the array
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        Result push_back(void* element);
    }; // end class PointerArray



    /**
     * @class DoubleArray
     * @brief An array of doubles.
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE DoubleArray : public Array
    {
    public:
        /**
         * @brief Constructor.
         */
        DoubleArray();

        /**
         * @brief Constructor.
         *
         * @pre data is a valid pointer.
         *
         * @param data [in]; Pointer to double data
         * @param size [in]; Number of double elements in data
         */
        DoubleArray(const double* data, size_t size);

        /**
         * @brief Constructor.
         *
         * @pre data is a valid pointer.
         *
         * @param data [in]; Pointer to float data
         * @param size [in]; Number of float elements in data
         */
        DoubleArray(const float* data, size_t size);
        
        /**
         * @brief Destructor.
         */
        ~DoubleArray();
        
        /**
         * @brief Returns the indexed element.
         *
         * @pre index < size();
         *
         * @param index [in]; The 0-based indexed of the desired element
         *
         * @return  The indexed element if found, or 0 if
         *          the index is out of bound.
         */
        double& operator[] (size_t index) const;

        /**
         * @brief Push a single new element to the back of the array,
         * expand if necessary.
         *
         * @param element [in]; A value to be inserted into the array
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        Result push_back(double element);
    }; // end class DoubleArray

    /**
     * @brief Print a DoubleArray to output stream.
     *
     * @param out   [in]; The output stream to print out to
     * @param array [in]; The DoubleArray to print
     *
     * @return The output stream that was printed out to.
     */
    inline std::ostream&
    operator<< (std::ostream& out, const DoubleArray& array)
    {

        // Opening bracket
        out << "[";

        // Data contents
        for (unsigned int i = 0; i < array.size(); i++)
        {
            out.precision(24);
            out << " " << array[i];
        }

        // Closing bracket
        out << " ]";
        
        return out;
    }
    
} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_BASE_ARRAY_HPP
