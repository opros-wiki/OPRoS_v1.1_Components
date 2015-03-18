//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_RESOURCE_IBUS_HPP
#define INCLUDE_EVOLUTION_CORE_RESOURCE_IBUS_HPP


/**
 * @file   evolution/core/resource/IBus.hpp
 * @brief  Interface class that defines functions for reading and
 *         writing data to a bus device
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::IBus
 */


#include <evolution/core/base/Export.hpp>
#include <evolution/core/base/Types.hpp>

namespace Evolution
{

    /**
     * @interface IBus
     *
     * @brief This class defines a bus interface
     * which has defines functions for reading and writing
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE IBus
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.IBus"

    public: // Structors

        /**
         * @brief Constructor
         */
        IBus ()
        {
        }

        /**
         * @brief Destructor
         */
        virtual ~ IBus ()
        {
        }

    public: // Communication

        /**
         * @brief reads n bytes of data from the bus in question into data
         */

        virtual Result read_data (unsigned char *data, size_t num_to_read, size_t *num_read) = 0;

        /**
         * @brief writes n bytes of data to the bus in question
         */

        virtual Result write_data (unsigned char const *data, size_t n) = 0;

        /**
         * @brief opens the bus for reading and writing
         **/

        virtual Result open_bus () = 0;

        /**
         * @brief closes the bus
         **/

        virtual Result close_bus () = 0;

    }; // end class IBus

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_CORE_RESOURCE_IBUS_HPP
