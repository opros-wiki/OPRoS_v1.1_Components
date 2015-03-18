//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_IDOCK_HPP
#define INCLUDE_EVOLUTION_CORE_IDOCK_HPP


/**
 * @file    IDock.hpp
 * @brief   Defines an interface for docking.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::IDock
 */


#include <evolution/core/base/Export.hpp>
#include <evolution/core/base/Config.hpp>
#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/base/Types.hpp>

namespace Evolution
{
    /**
     * @interface IDock
     *
     * @brief This class define the interface for docking.
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IDock
    {
    public:

        /** 
        * @brief Possible docking states.
        * @ingroup core_resource
        */
        typedef enum DockingState
        {
            UNDOCKED,
            PENDING,
            DOCKED,
            CHARGED,
            DOCK_FAILED
        };

      public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char * const INTERFACE_ID;  // "Evolution.IDock"

      public: // Structors

        /**
         * @brief Constructor
         */
        IDock ()
        {
        }

        /**
         * @brief Destructor
         */
        virtual ~ IDock ()
        {
        }

      public: // Communication

        /**
         * @brief Engages the docking mechanism
         *
         * @result RESULT_SUCCESS if successful
         **/
        virtual Result dock() = 0;

        /**
         * @brief Disengages the docking mechanism
         *
         * @result RESULT_SUCCESS if successful
         **/
        virtual Result undock() = 0;

        /**
         * @brief Retrieves the status of the docking device
         *
         * @result the docking state.
         **/
        virtual IDock::DockingState get_status() = 0;

    };  // end class IDock

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_IDOCK_HPP
