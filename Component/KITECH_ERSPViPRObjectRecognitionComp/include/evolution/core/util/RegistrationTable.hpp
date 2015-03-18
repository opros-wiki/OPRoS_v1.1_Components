//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_REGISTRATION_TABLE_HPP
#define INCLUDE_EVOLUTION_BASE_REGISTRATION_TABLE_HPP


/** 
 * @file    evolution/core/util/RegistrationTable.hpp
 * @brief   Implements a RegistrationTable, which is an AcquirableMap with
 *          auto-incrementing keys.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::RegistrationTable
 */


#include <evolution/core/util/AcquirableMap.hpp>
#include <evolution/core/base/Platform.hpp>
#include <evolution/core/base/IMutex.hpp>


namespace Evolution {


    /**  
     * @class RegistrationTable
     *
     * @brief This class provides an acquirable map with auto-incrementing keys.
     * 
     * @param  Val  the type of values stored in the table
     *
     * @ingroup core_util
     */
    template <class Val>
    class RegistrationTable : public AcquirableMap <unsigned long, Val>
    {
    public: // Types

        /**
         * @brief  Registration ID type
         */
        typedef unsigned long RegistrationId;

        /**
         * @brief  This value is a map which maps Registration ids to the
         * class's output values.
         */
        typedef std::map<RegistrationId, Val, std::less<RegistrationId>, std::allocator<Val> > MapType;
        
        /**
         * @brief  This value is an iterator over the map.
         */
        typedef typename MapType::iterator Iterator;

        /**
         * @brief  This value is the map's result type.
         */
        typedef typename MapType::value_type ValueType;

    public: // Construction / destruction

        /**
         * @brief This constructor initializes the registration ID counter.
         */
        RegistrationTable () : AcquirableMap<unsigned long,Val> () {
            // Basic initialization
            _last_id = 0;
        } // end RegistrationTable()

        /**
         * @brief  Standard destructor
         */
        virtual ~RegistrationTable () {}

    public: // Modification

        /**
         * @brief This inserts a value, generating a registration ID for the
         * new entry.
         *
         * @param   value [in]: The value to insert
         * @param   new_id [out]: The registration ID for the new entry
         * 
         * @return  The iterator-boolean status pair indicating the point
         *      after the insertion in the map and whether the insertion was
         *      successful.
         */
        std::pair<Iterator,bool> insert (const Val& value, RegistrationId* new_id)
        {
            *new_id = ++_last_id;

            // Insert the value
            return this->MapType::insert (ValueType (*new_id, value));
        } // end insert()

    protected: // Implementation

        /// The last ID used in the table.
        RegistrationId _last_id;

    }; // end class RegistrationTable


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE_REGISTRATION_TABLE_HPP
