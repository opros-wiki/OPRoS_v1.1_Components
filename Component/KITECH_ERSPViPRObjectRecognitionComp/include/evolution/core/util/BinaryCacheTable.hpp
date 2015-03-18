//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_UTIL_BINARYCACHETABLE_HPP
#define INCLUDE_EVOLUTION_CORE_UTIL_BINARYCACHETABLE_HPP


/**
 * @file    evolution/core/util/BinaryCacheTable.hpp
 * @brief   This Bit-value table is useful for caching binary decision values
 *          over an arbitarily sized dimensional space.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::BinaryCacheTable
 */

#include <float.h>
#include <vector>
#include <math.h>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/ObjectImpl.hpp>
#include <evolution/core/base/Logging.hpp>

namespace Evolution 
{
    /**
     * @class BinaryCacheTable
     * 
     * @brief Useful 1 bit deep three dimensional table.
     *
     * To use this, instantiate it with x,y,z bounds, or min/max x,y,z (and it
     * will compute the bounds for you, and assume you want 0 as the start for
     * x,y,z).  Once the object has a size and memory allocated to it, use
     * set_val(x,y,z,truth_value) to set the truth of a given location, and
     * get_val(x,y,z) to get the truth of a given location.  Bounded and
     * unbounded versions of both of these functions exist.  Naturally, if you
     * are confident of safety, the unbounded version gives you more speed.
     * However, using the bounded guarantees that reads outside the table will
     * return false, and writes outside the table will result in no unsafe
     * writes.
     *
     * This table is slightly slower than the CompactCacheTable, but
     * supports arbitrarily sized dimensions.  x,y and z can all be made
     * as wide as is needed.  If you need 3 dimensions wider than 32bits,
     * use this table.  Otherwise, use the slightly faster
     * CompactCacheTable.
     *
     * This has often been useful to set up look up tables for functions with
     * large domains and long computation times.
     *
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL BinaryCacheTable : public ObjectImpl
    {
    public: // Logging
        /**
         * @brief Log facilities
         */
        ERSP_LOG_CLASS_CATEGORY_DECLARE(BinaryCacheTable, "Evolution.Core.Util.BinaryCacheTable");

    public: // Structors
        /**
         * @brief Default Constructor for BinaryCacheTable. Gives sane, but not
         * useful, values.
         */
        BinaryCacheTable ()
        {
            init();
        };

        /**
         * @brief Constructor that sets up a basic table of given width/height.
         *
         * @param width [in]; The width of the table.
         * @param height [in]; The height of the table.
         * @param depth [in]; The depth of the table.
         */
        BinaryCacheTable (int width, int height, int depth)
        {
            init();
            realloc_memory(width, height, depth);
        };

        /**
         * @brief Constructor that sets up a basic cleared table of given
         * bounds.
         *
         * @param minx [in]; The lower x bound.
         * @param miny [in]; The lower y bound.
         * @param minz [in]; The lower z bound.
         * @param maxx [in]; The upper x bound.
         * @param maxy [in]; The upper y bound.
         * @param maxz [in]; The upper z bound.
         */
        BinaryCacheTable (int minx, int miny, int minz, int maxx, int maxy, int maxz)
        {
            init();
            realloc_memory(minx, miny, minz, maxx, maxy, maxz);
        };

        /**
         * @brief Destructor for BinaryCacheTable
         */
        ~BinaryCacheTable ()
        {
            if(_width*_height*_depth > 0 && _data != NULL)
            {
                delete[] _data;
            }
        }

    protected: // Constructor Helper.

        /**
         * @brief Initialize bounds and dimensions to 0 for an empty table.
         */
        inline void init()
        {
            _width = 0; _height = 0; _depth = 0;
            _minx = 0;  _miny = 0;   _minz = 0;
            _maxx = 0;  _maxy = 0;   _maxz = 0;
            _data = NULL;
        } 

    public: // Queries

        /**
         * @brief Get the width of the table (x).
         *
         * @return The table's width
         */
        inline int get_width()  //get table width (x)
        { 
            return _width;
        } 

        /**
         * @brief Get the height of the table (y).
         *
         * @return The table's height
         */
        inline int get_height()  //get table height (y)
        { 
            return _height;
        } 

        /**
         * @brief Get the depth of the table (z).
         *
         * @return The table's depth.
         */
        inline int get_depth()  //get table depth (z)
        { 
            return _depth;
        } 

        /**
         * @brief Get the number of 32-bit slices per z.
         *
         * @return the number of slices per z.
         */
        inline int get_slice()  //get table slice!
        { 
            return _slice;
        } 

        /**
         * @brief Get the minimum x coordinate.
         *
         * @return The minimum x coordinate.
         */
        inline int get_minx()  //get table min x
        { 
            return _minx;
        } 

        /**
         * @brief Get the minimum y coordinate.
         *
         * @return The minimum y coordinate.
         */
        inline int get_miny()  //get table min y
        { 
            return _miny;
        } 

        /**
         * @brief Get the minimum z coordinate.
         *
         * @return The minimum z coordinate.
         */
        inline int get_minz()  //get table min z
        { 
            return _minz;
        } 

        /**
         * @brief Get the maximum x coordinate.
         *
         * @return The maximum x coordinate.
         */
        inline int get_maxx()  //get table max x
        { 
            return _maxx;
        } 

        /**
         * @brief Get the maximum y coordinate.
         *
         * @return The maximum y coordinate.
         */
        inline int get_maxy()  //get table max y
        { 
            return _maxy;
        } 

        /**
         * @brief Get the maximum z coordinate.
         *
         * @return The maximum z coordinate.
         */
        inline int get_maxz()  //get table max z
        { 
            return _maxz;
        } 

        /**
         * @brief Gets the truth value at the x,y,z index of the table.
         *
         * @param x [in]; The x index
         * @param y [in]; The y index
         * @param z [in]; The z index
         *
         * @return The boolean value at the x,y,z location in the table.
         */
        inline bool get_val(int x, int y, int z)  
        {
            if(z < _minz || z > _maxz) return false; //invalid vertical index. only checking due to overflow errors, which are nasty to detect in the structure of this table.
            unsigned long int aval= 1<<(z-_minz); //set up mask, equivalent to pow(2, z). useful overflow? never!
            unsigned long int ind = (z - _minz) / 32;//get 3rd d index into _data.
            size_t index = (y-_miny) * _width * _slice + (x-_minx) * _slice + ind; //x,y ind, then up z
            return ((_data[index] & aval) != 0); //and the created mask with the data element. a non-zero value indicates truth.
        }

        /**
         * @brief Gets the truth value at the x,y,z index of the table,
         * checking for out of bounds conditions.
         *
         * @param x [in]; The x index
         * @param y [in]; The y index
         * @param z [in]; The z index
         *
         * @return The boolean value at the x,y,z location in the table.
         */
        inline bool get_val_safe(int x, int y, int z)  
        {
            if(x < _minx || x >= _maxx ) return false; //invalid width index. 
            if(y < _miny || y >= _maxy ) return false; //invalid height index. 
            if(z < _minz || z >= _maxz ) return false; //invalid vertical index. 
            unsigned long int aval= 1<<(z-_minz); //set up mask, equivalent to pow(2, z)
            unsigned long int ind = (z - _minz) / 32;//get 3rd d index into _data.
            size_t index = (y-_miny) * _width * _slice + (x-_minx) * _slice + ind; //x,y ind, then up z
//fprintf(stderr, "%d %d %d: %d is index, %u is value, %u is mask, %u is bool\n", x, y, z, index, _data[index], _data[index] & aval, (_data[index] & aval) != 0);

            return (_data[index] & aval) != 0; //and the created mask with the data element. a non-zero value indicates truth.
        }

        /**
         * @brief Sets the truth value at the x,y,z index of the table.
         *
         * @param x [in]; The x index
         * @param y [in]; The y index
         * @param z [in]; The z index
         * @param v [in[; The value to set
         *
         * @return True if successful.
         */
        inline bool set_val(int x, int y, int z, bool v=true)  
        { //shouldn't have the problem table cache in gaussian mixture had, passing in ints.
            if(z < _minz || z > _maxz) return false; //invalid vertical index. only checking due to overflow errors, which are nasty to detect.
            unsigned long int aval= 1<<(z-_minz); //set up mask, equivalent to pow(2, z). useful overflow? never!
            unsigned long int ind = (z - _minz) / 32;//get 3rd d index into _data.
            size_t index = (y-_miny) * _width * _slice + (x-_minx) * _slice + ind; //x,y ind, then up z
            if(v)  //feel me wicked evil.
            {
                _data[index] |= aval; //or mask to set desired bit true.
            }
            else
            {
                _data[index] = ~(~_data[index] | aval); //set bit false
            }
            return true;
        }

        /**
         * @brief Sets the truth value at the x,y,z index of the table,
         * checking for out of bounds conditions.
         *
         * @param x [in]; The x index
         * @param y [in]; The y index
         * @param z [in]; The z index
         * @param v [in[; The value to set
         *
         * @return True if successful.
         */
        inline bool set_val_safe(int x, int y, int z, bool v=true)  
        {
            if(x < _minx || x >= _maxx) return false; //invalid width index. 
            if(y < _miny || y >= _maxy) return false; //invalid height index.
            if(z < _minz || z >= _maxz ) return false; //invalid vertical index. 
            unsigned long int aval= 1<<(z - _minz); //set up mask, equivalent to pow(2, z)..overflizow!
            unsigned long int ind = (z - _minz) / 32;//get 3rd d index into _data.
            size_t index = (y-_miny) * _width * _slice + (x-_minx) * _slice + ind; //x,y ind, then up z

            if(v)
            {
                _data[index] |= aval; //or mask to set desired bit true.
            }
            else
            {
                _data[index] = ~(~_data[index] | aval); //set bit false
            }

//fprintf(stderr, "%d %d %d: %d is index, %u is value\n", x, y, z, index, _data[index]); 
            return true;
        }

        /**
         * @brief This function gets you a const pointer into the data of the
         * table, at point x,y, z=0.  Since the data is one continuous object,
         * you can overread if you try to read farther than z (ceil(z/32.0)
         * 32bit unsigned longs that make up each z position)
         *
         * @param x [in]; The x index
         * @param y [in]; The y index
         *
         * @return The pointer into the data table at point x,y.
         */ 
        inline const unsigned long int* get_element(int x, int y)  
        {
            //if(x < _minx || x >= _maxx) return false; //invalid width index. 
            //if(y < _miny || y >= _maxy) return false; //invalid height index. 
            size_t index = (y-_miny) * _width * _slice + (x-_minx) * _slice; //x,y ind, then up z
            return &_data[index]; //
        }

        /** 
         * @brief This function returns just 1 unsigned long.  The variable
         * 'slice' is the index of the unsigned long desired from the x,y
         * position.  It encodes the z values from (slice * 32, (slice+1) *
         * 32).
         *
         * @param x [in]; The x index
         * @param y [in]; The y index
         * @param slice [in]; The index of the unsigned long desired 
         * at the x,y position.
         *
         * @return The unsigned long value at point x,y.
         */ 
        inline const unsigned long int get_element(int x, int y, int slice)  
        {
            //if(x < _minx || x >= _maxx) return false; //invalid width index. 
            //if(y < _miny || y >= _maxy) return false; //invalid height index. 
            size_t index = (y-_miny) * _width * _slice + (x-_minx) * _slice + slice; //x,y ind, then up z
            return _data[index]; //
        }

        //this copies, unsigned long by unlong.  Assumes the array is 
        //the full length of the z dimension.

        /**
         * @brief This function copies the ulong array val of length
         * get_slice() into the table at location x,y.
         *
         * The 0th bit of the 0th index is the z=0 bit.  The last bit of last
         * index is the last z element. 
         * 
         * @param x [in]; The x index
         * @param y [in]; The y index
         * @param val [in]; The array of unsigned long values to set.
         *
         * @return True if successful.
         */
        inline bool set_element(int x, int y, unsigned long int *val)
        {
            if(val == NULL) return false; //avoid segfaults.
            //if(x < _minx || x >= _maxx) return false; //invalid width index. 
            //if(y < _miny || y >= _maxy) return false; //invalid height index.
            size_t index = (y-_miny) * _width * _slice + (x-_minx) * _slice; //x,y ind, then up z
            size_t bound = (_maxz - _minz)/32;
            memcpy( &_data[index], val, sizeof(unsigned long) * bound);
            return true;
        }

        /** 
         * @brief This function sets just 1 unsigned long.  The variable
         * 'slice' is the index of the unsigned long desired from the x,y
         * position.  It encodes the z values from (slice * 32, (slice+1) * 32]
         * 
         * @param x [in]; The x index
         * @param y [in]; The y index
         * @param slice [in]; The slice at which to set.
         * @param val [in]; The unsigned long value to set.
         *
         * @return True if successful.
         */ 
        inline bool set_element(int x, int y, int slice, unsigned long int val) //This copies val into x,y,slice index in _data.
        {
            //if(x < _minx || x >= _maxx) return false; //invalid width index. 
            //if(y < _miny || y >= _maxy) return false; //invalid height index.
            size_t index = (y-_miny) * _width * _slice + (x-_minx) * _slice + slice; //x,y ind, then up z
            _data[index ] = val;
            return true;
        }


    public: //Control.

        /**
         * @brief This function sets all slices of all indicies to 0/false.
         */
        void clear();

        /**
         * @brief This function changes the table to given width,height,depth.
         *
         * @param width [in]; The width of the table.
         * @param height [in]; The height of the table.
         * @param depth [in]; The depth of the table.
         *
         * @return True if successful.
         */
        bool realloc_memory(int width, int height, int depth); //sets to 0,width 0,height

        /**
         * @brief This function changes the table to the given width, height,
         * depth and forces memeory reallocation.
         *
         * @param width [in]; The width of the table.
         * @param height [in]; The height of the table.
         * @param depth [in]; The depth of the table.
         *
         * @return True if successful.
         */
        bool force_realloc_memory(int width, int height, int depth);

        /**
         * @brief This function changes the table to maxx-minx, maxy-miny,
         * maxz-minz, with the min/max applied appropriately.
         *
         * @param minx [in]; The lower x bound.
         * @param miny [in]; The lower y bound.
         * @param minz [in]; The lower z bound.
         * @param maxx [in]; The upper x bound.
         * @param maxy [in]; The upper y bound.
         * @param maxz [in]; The upper z bound.
         *
         * @return True if successful.
         */
        bool realloc_memory(int minx, int miny, int minz, int maxx, int maxy, int maxz); //sets to 0,width 0,height

        /**
         * @brief This function changes the table to the given maxx-minx,
         * maxy-miny, maxz-minz, with the min/max applied appropriately.  The
         * function forces a memory reallocation.
         *
         * @param minx [in]; The lower x bound.
         * @param miny [in]; The lower y bound.
         * @param minz [in]; The lower z bound.
         * @param maxx [in]; The upper x bound.
         * @param maxy [in]; The upper y bound.
         * @param maxz [in]; The upper z bound.
         *
         * @return True if successful.
         */
        bool force_realloc_memory(int minx, int miny, int minz, int maxx, int maxy, int maxz);

    public: //Display

        /**
         * @brief
         * Prints out the BinaryCacheTable.
         *
         * @param out [in]; The stream to print out the BinaryCacheTable
         */
        void print(std::ostream& out) const
        {
            out << "W: "  << _width;
            out << " H: " << _height << std::endl;
        } 

    protected: // Implementation
        int                 _width;             ///< The width of the original data. 
        int                 _height;            ///< The height of the original data. 
        int                 _depth;             ///< The height of the original data. 
        int                 _minx;              ///< lower left x
        int                 _miny;              ///< lower left y
        int                 _minz;              ///< lower left z
        int                 _maxx;              ///< upper right x
        int                 _maxy;              ///< upper right y
        int                 _maxz;              ///< upper right z

        int                 _slice;             ///< _depth/32

        unsigned long int*  _data;              ///< 32 bit height data.
    };


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_UTIL_BINARYCACHETABLE_HPP
