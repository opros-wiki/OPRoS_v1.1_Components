//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_UTIL_COMPACTCACHETABLE_HPP
#define INCLUDE_EVOLUTION_CORE_UTIL_COMPACTCACHETABLE_HPP


/**
 * @file    evolution/core/util/CompactCacheTable.hpp
 * @brief Provies an arbitrary (x,y) size bit-addressable 3
 * dimensional data structure.  
 * 
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::CompactCacheTable
 */

#include <float.h>
#include <vector>
#include <math.h>
#include <evolution/core/base/Contract.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/ObjectImpl.hpp>

/**
 * @namespace Evolution
 * @brief   Main namespace of the evolution robotics source code.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */
namespace Evolution 
{

    /**
     * @class CompactCacheTable
     *
     * @brief This class is used as a 3d binary decision table, with z=[0,32]
     *
     * Construct this class with a height and a width, then use set_val() and
     * get_val() to set and get binary values.  Extremely compact in memory, 
     * and very fast to access.
     *
     * This table is useful for caching data
     * values in narrow (in the z dimension, size less than 32) 3
     * dimensional data spaces.  It has slightly faster access times than
     * the BinaryCacheTable, but is less general.  If you need arbitrarily
     * sized dimensions in all 3 dimensions, used the BinaryCacheTable
     * instead.
     * 
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL CompactCacheTable : public ObjectImpl
    {
    public: // Logging
        /**
         * @brief Log facilities, declaration
         */
        ERSP_LOG_CLASS_CATEGORY_DECLARE(CompactCacheTable, "Evolution.Core.Util.CompactCacheTable");

    public: // Structors
        /**
         * @brief This is the default constructor for CompactCacheTable.
         */
        CompactCacheTable ()
        {
            _width = 0; _height = 0;
            _minx = 0; _miny = 0;
            _maxx = 0; _maxy = 0;
            _data = NULL;
        };

        /**
         * @brief This constructor sets up a basic cleared table of 
         * width/height.
         *
         * @param width [in];  The table width.
         * @param height [in];  The table height.
         */
        CompactCacheTable (int width, int height)
        {
            _width = 0; _height = 0; _data = NULL;
            _minx = 0; _miny = 0;
            _maxx = 0; _maxy = 0;
            realloc_memory(width, height);
        };

        /**
         * @brief This constructor sets up a basic cleared table of 
         * specified x and y bounds.
         *
         * @param minx [in]; The lower x bound.
         * @param miny [in]; The lower y bound.
         * @param maxx [in]; The upper x bound.
         * @param maxy [in]; The upper y bound.
         */
        CompactCacheTable (int minx, int miny, int maxx, int maxy)
        {
            _width = 0; _height = 0;
            _minx = 0; _miny = 0;
            _maxx = 0; _maxy = 0;
            _data = NULL;
            realloc_memory(minx, miny, maxx, maxy);
        };

        /**
         * @brief Destructor for CompactCacheTable
         */
        ~CompactCacheTable ()
        {
            if(_width*_height > 0 && _data)
            {
                delete[] _data;
            }
        }

    public: // Queries

        /**
         * @brief This function gets the width of the table.
         *
         * @return The width of the table.
         */
        inline int get_width()  //get table width (x)
        { 
            return _width;
        } 

        /**
         * @brief This function gets the height of the table.
         *
         * @return The height of the table.
         */
        inline int get_height()  //get table height (y)
        { 
            return _height;
        } 

        /**
         * @brief This function returns the minimum valid x value of the table.
         *
         * @return The minimum x value of the table.
         */
        inline int get_minx()  //get table min x
        { 
            return _minx;
        } 

        /**
         * @brief This function returns the minimum valid y value of the table.
         *
         * @return The minimum y value of the table.
         */
        inline int get_miny()  //get table min x
        { 
            return _miny;
        } 

        /**
         * @brief This function returns the maximum valid x value of the table.
         *
         * @return The maximum x value of the table.
         */
        inline int get_maxx()  //get table max x
        { 
            return _maxx;
        } 

        /**
         * @brief This function returns the maximum valid y value of the table.
         *
         * @return The maximum y value of the table.
         */
        inline int get_maxy()  //get table max x
        { 
            return _maxy;
        } 

        /**
         * @brief This function returns the binary value at the x,y,z 
         * index specified.
         *
         * @param x [in]; The x index into the table
         * @param y [in]; The y index into the table
         * @param z [in]; The z index into the table
         *
         * @return The binary value at the x,y,z index
         */
        inline bool get_val(int x, int y, int z)  //z 0,32 always
        {
            if(z < 0 || z > 32) return false; //invalid vertical index. only checking due to overflow errors, which are nasty to detect.
            unsigned long int aval= 1<<z; //set up mask, equivalent to pow(2, z)
            return ((_data[(y-_miny)*_width+(x-_minx)] & aval) != 0); //and the created mask with the data element. a non-zero value indicates truth.
        }

        /**
         * @brief This function returns the binary value at the x,y,z index
         * specified, checks for out of bounds calls, returns false if OOB.
         *
         * @param x [in]; The x index into the table
         * @param y [in]; The y index into the table
         * @param z [in]; The z index into the table
         *
         * @return The binary value at the x,y,z index
         */
        inline bool get_val_safe(int x, int y, int z)  //z 0,32 always
        {
            if(x < _minx || x >= _maxx) return false; //invalid width index. 
            if(y < _miny || y >= _maxy) return false; //invalid height index. 
            if(z < 0 || z > 32) return false; //invalid vertical index. 
            unsigned long int aval= 1<<z; //set up mask, equivalent to pow(2, z)
            return (_data[(y-_miny)*_width+(x-_minx)] & aval) != 0; //and the created mask with the data element. a non-zero value indicates truth.
        }

        /**
         * @brief This function sets the binary value at the x,y,z index
         * specified with the specified v.
         *
         * @param x [in]; The x index into the table
         * @param y [in]; The y index into the table
         * @param z [in]; The z index into the table
         * @param v [in]; The binary value to set.
         *
         * @return True if setting the value was successful.
         */
        inline bool set_val(int x, int y, int z, bool v=true)  //z 0,32 always
        { //shouldn't have the problem table cache in gaussian mixture had, passing in ints.
            if(z < 0 || z > 32) return false; //invalid vertical index. only checking due to overflow errors, which are nasty to detect.
            unsigned long int aval= 1<<z; //set up mask, equivalent to pow(2, z)
            v?
                _data[(y-_miny)*_width+(x-_minx)] |= aval //or mask to set desired bit true.
                :
                _data[(y-_miny)*_width+(x-_minx)] = ~(~_data[(y-_miny)*_width+(x-_minx)] | aval); //set bit false
            return true;
        }

        /**
         * @brief This function sets the binary value at the x,y,z index
         * specified with the specified v and checks for OOB.
         *
         * @param x [in]; The x index into the table
         * @param y [in]; The y index into the table
         * @param z [in]; The z index into the table
         * @param v [in]; The binary value to set.
         *
         * @return True if setting the value was successful.
         */
        inline bool set_val_safe(int x, int y, int z, bool v=true)  //z 0,32 always
        {
            if(x < _minx || x >= _maxx) return false; //invalid width index. 
            if(y < _miny || y >= _maxy) return false; //invalid height index.
            if(z < 0 || z > 32) return false; //invalid vertical index. 
            unsigned long int aval= 1<<z; //set up mask, equivalent to pow(2, z)
            if(v)
            {
                _data[(y-_miny)*_width+(x-_minx)] |= aval; //or mask to set desired bit true.
            }
            else
            {
                _data[(y-_miny)*_width+(x-_minx)] = ~(~_data[(y-_miny)*_width+(x-_minx)] | aval); //set bit false
            }
            return true;
        }

        /**
         * @brief This function returns the unsigned long that represents the
         * entire bit-table from 0->32 at the specified x,y index.
         *
         * @param x [in]; The x index into the table
         * @param y [in]; The y index into the table
         *
         * @return The unsigned long value representing the 32 bits at the
         * specified x,y index.
         */
        inline unsigned long int get_element(int x, int y)  //z 0,32 always
        {
            //if(x < _minx || x >= _maxx) return false; //invalid width index. 
            //if(y < _miny || y >= _maxy) return false; //invalid height index. 
            return _data[(y-_miny)*_width+(x-_minx)]; //and the created mask with the data element. a non-zero value indicates truth.
        }

        /**
         * @brief This function sets the unsigned long that represents the
         * entire bit-table from 0->32 at the specified x,y index.
         *
         * @param x [in]; The x index into the table
         * @param y [in]; The y index into the table
         * @param val [in]; The unsigned long value representing the 
         *      32 bits at the x,y index
         * 
         * @return True if setting the value was successful
         */
        inline bool set_element(int x, int y, unsigned long int val)
        {
            //if(x < _minx || x >= _maxx) return false; //invalid width index. 
            //if(y < _miny || y >= _maxy) return false; //invalid height index.
            _data[(y-_miny)*_width+(x-_minx)] = val; //or mask to set desired bit true.
            return true;
        }

    public: //Control.

        /**
         * @brief This function sets all slices of all indicies to 0/false.
         */
        void clear()
        {
            int j = 0, area = _width*_height;
            for(j=0; j < area ; j++)
            {
                _data[j] = 0; 
            }
            
        }

        /**
         * @brief This function will resize the table to (width, height).
         *
         * @param width [in]; The new width of the table
         * @param height [in]; The new height of the table
         *
         * @return True if the reallocation was successful
         */
        bool realloc_memory(int width, int height) //sets to 0,width 0,height
        {
            if(width == _width && height == _height)//if proper data size
            {
                _minx = 0;
                _miny = 0;
                _maxx = width;
                _maxy = height;
                clear(); //clear data.
                return true;
            }
            return force_realloc_memory(width, height);
        }

        /**
         * @brief This function will resize the table to (width, height),
         * forcing memory reallocation.
         *
         * @param width [in]; The new width of the table
         * @param height [in]; The new height of the table
         *
         * @return True if the reallocation was successful
         */
        bool force_realloc_memory(int width, int height)
        {
            delete[] _data; //remove memory in use.
            if( height * width == 0) {
                _minx   = 0;
                _miny   = 0;
                _maxx   = width;
                _maxy   = height;
                _width  = width; 
                _height = height;
                _data   = NULL;
                return  true; //successfully emptied table.
            }
            _data = new unsigned long int[width*height];
            if(!_data) return false; //bad alloc, oom.
            _minx = 0;
            _miny = 0;
            _maxx = width;
            _maxy = height;
            _width = width;
            _height = height;  
            clear();
            return true;
        }

        /**
         * @brief This function will resize the table to (maxx-minx,
         * maxy-miny), with maxx,minx,maxy,miny set as the min and maxes of the
         * table.
         *
         * @param minx [in]; The new lower x bound
         * @param miny [in]; The new lower y bound
         * @param maxx [in]; The new upper x bound
         * @param maxy [in]; The new upper y bound
         *
         * @return True if the reallocation was successful
         */
        bool realloc_memory(int minx, int miny, int maxx, int maxy) //sets to 0,width 0,height
        {
            if((maxx-minx) == _width && (maxy - miny) == _height)//if proper data size
            {
                //ensure that the min/max are recorded properly. data/height/width already ok.
                _minx = minx;
                _miny = miny;
                _maxx = maxx;
                _maxy = maxy;
                clear(); //clear data. no need for other manipulation
                return true;
            }
            return force_realloc_memory(minx, miny, maxx, maxy);
        }

        /**
         * @brief Use this to force-resize the table to (maxx-minx, maxy-miny),
         * with maxx,minx, maxy,miny set as the min and maxes of the table.
         *
         * @param minx [in]; The new lower x bound
         * @param miny [in]; The new lower y bound
         * @param maxx [in]; The new upper x bound
         * @param maxy [in]; The new upper y bound
         *
         * @return True if the reallocation was successful
         */
        bool force_realloc_memory(int minx, int miny, int maxx, int maxy)
        {
            delete[] _data; //remove memory in use.
            if( (maxx-minx)*(maxy - miny) == 0) {
                _minx = minx;
                _miny = miny;
                _maxx = maxx;
                _maxy = maxy;
                _width = (maxx-minx);
                _height = (maxy - miny);  
                _data = NULL;
                return true; //successfully emptied table.
            }
            _data = new unsigned long int[(maxx-minx)*(maxy - miny)]; //could save 2 subtracts. don't care.
            if(!_data) return false; //bad alloc, oom.
            _minx = minx;
            _miny = miny;
            _maxx = maxx;
            _maxy = maxy;
            _width = (maxx-minx);
            _height = (maxy - miny);  
            clear();
            return true;
        }

    public: //Display

        /**
         * @brief This function displays the table on the specified output
         * stream.
         *
         * @param out [in]; The stream to output the table
         */
        void print(std::ostream& out) const
        {
            out << "W: "  << _width;
            out << " H: " << _height << std::endl;
        } 

    protected: // Implementation
        int                 _width;            ///< The width of the original data. 
        int                 _height;           ///< The height of the original data. 
        int                 _minx;             ///< lower left x
        int                 _miny;             ///< lower left y
        int                 _maxx;             ///< upper right x
        int                 _maxy;             ///< upper right y
        unsigned long int*  _data;             ///< 32 bit height data.
    };


} // end namespace Evolution


#endif // INCLUDE_EVOLUTION_CORE_UTIL_COMPACTCACHETABLE_HPP
