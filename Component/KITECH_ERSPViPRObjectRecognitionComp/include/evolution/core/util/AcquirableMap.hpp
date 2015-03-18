//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_ACQUIRABLE_MAP_HPP
#define INCLUDE_EVOLUTION_BASE_ACQUIRABLE_MAP_HPP


/**
 * @file    evolution/core/util/AcquirableMap.hpp
 * @brief   Implements a thread-safe map data structure.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::AcquirableMap
 */


#include <map>
#include <evolution/core/base/AcquirableImpl.hpp>


namespace Evolution {


    /**  
     * @class AcquirableMap
     * 
     * @param  Key    the key type
     * @param  Val    the value type
     * @param  Pred   the comparison functor class
     * @param  Alloc  the allocator class
     * @ingroup core_util
     * @ingroup core_util_aibo
     *
     * @brief This provides a map that may be acquired exclusively. This
     * acquisition is completely the responsibility of the caller. That is, the
     * AcquirableMap makes no internal guarantees of exclusive access; it is
     * not innately thread-safe.
     */
    template<class Key, class Val, class Pred = std::less<Key>, class Alloc = std::allocator<Val> >
    class AcquirableMap : public std::map <Key, Val, Pred, Alloc>,
                          public AcquirableImpl
    {
    public: // Types

        typedef std::map <Key, Val, Pred, Alloc> MapType; ///< convienience typedef for the Map
        typedef typename MapType::iterator Iterator;      ///< convienience typedef for MapType iterators
        typedef typename MapType::value_type ValueType;   ///< convienience typedef for MapType values

    public: // Construction / destruction

        /**
         * @brief Default Constructor
         *
         * @param  recursive_mutex [in]; Specify if the map's
         *      associated mutex should be recursive or not.
         * @param  comp [in,opt]: Comparison predicate
         * @param  al [in,opt]: Allocator
         */
        explicit AcquirableMap (bool recursive_mutex = false,
                                const Pred& comp = Pred(),
                                const Alloc& al = Alloc())
            : std::map <Key, Val, Pred, Alloc> (comp, al),
              AcquirableImpl (recursive_mutex) {}

        /**
         * @brief Destructor
         */
        virtual ~AcquirableMap () {}

    }; // end AcquirableMap


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE_ACQUIRABLE_MAP_HPP
