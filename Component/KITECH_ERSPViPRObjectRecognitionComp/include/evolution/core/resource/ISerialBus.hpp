//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_RESOURCE_BASE_ISERIALBUS_HPP
#define INCLUDE_EVOLUTION_RESOURCE_BASE_ISERIALBUS_HPP


/**
 * @file    ISerialBus.hpp
 * @brief  Contains a class to define functions for reading and
 * writing data to a bus device
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::IBus
 */


#include <evolution/core/base/ResultCodes.hpp>
#include <evolution/core/resource/IBus.hpp>

namespace Evolution
{

    /**
     * @interface ISerialBus
     *
     * @brief This class defines a serial port interface
     * which has defines functions for reading and writing
     * open, close, and setting the parameters of the serial port
     *
     * @ingroup core_resource
     */
    class EVOLUTION_EXPORT_RESOURCE ISerialBus:public IBus
    {
    public: // Constants
        /**
         * @brief ID of this interface
         */
        static const char *const INTERFACE_ID;  // "Evolution.ISerialBus"

    public: // Structors

        /**
         * @brief Constructor, using default values
         */
        ISerialBus ()
        {
        }

        /**
         * @brief Destructor
         */
        virtual ~ ISerialBus ()
        {
        }

    public: // IBus interface

        /**
         * @brief reads n bytes from the serial port into data
         */

        virtual Result read_data (unsigned char *data, size_t num_to_read, size_t *num_read) = 0;

        /**
         *	@brief writes n bytes from data to the serial port
         */

        virtual Result write_data (unsigned char const *data, size_t n) = 0;

        /**
         * @brief reads n bytes from the serial port into data with timeout
         */

        virtual Result read_data_blocking (unsigned char *data, size_t num_to_read, size_t *num_read, int timeout ) = 0;

        /**
         * @brief meant to read a packet of data from the serial port
         *
         * In some more specific implementation, implement
         * the packet reading function.  ISerialBus will go to this
         * implementation.
         * */

        virtual Result read_packet( unsigned char *data ) 
        { 
            return RESULT_NOT_IMPLEMENTED; 
        }

        /**
         * @brief meant to write a packet of data to the serial port
         *
         * In some specific implementation of the ISerialBus interface
         * implement this function to write a packet of data
         **/

        virtual Result write_packet( unsigned char *data ) 
        {
            return RESULT_NOT_IMPLEMENTED; 
        }

        /**
         * @brief opens serial port for reading and writing
         *        if the port is already open, the bus is closed
         **/

        virtual Result open_bus () = 0;

        /**
         * @brief closes the serial port
         **/

        virtual Result close_bus () = 0;

    public: //Serial port functions

        /**
         * @brief flushes the serial port (input and output)
         **/

        virtual Result flush () 
        {
            return RESULT_NOT_IMPLEMENTED;
        }

        /**
         * @brief flushes the serial port (input)
         **/

        virtual Result iflush() 
        {
            return RESULT_NOT_IMPLEMENTED;
        }

        /**
         * @brief flushes the serial port (output)
         **/

        virtual Result oflush() 
        {
            return RESULT_NOT_IMPLEMENTED;
        }

        /**
         * @brief sets the parity
         *
         * sets the parity of the serial port (0 for none, 1 for odd, 2 for even)
         *
         **/

        virtual Result set_parity (TicketId ticket, int parity) 
        {
            return RESULT_NOT_IMPLEMENTED;
        }

        /**
         * @brief sets the speed
         *
         * sets the speed of the serial port (B300, B1200, B9600, etc. )
         **/

        virtual Result set_speed (TicketId ticket, int baud) 
        {
            return RESULT_NOT_IMPLEMENTED;
        }

        /**
         * @brief is the serial port waiting for data to send/receive
         **/

        virtual bool waiting ()  = 0;

        /**
         * @brief sets the byte size (5, 6, 7, or 8)
         */
        virtual void set_byte_size (TicketId ticket, int s) {};

        /**
         * @brief sets the number of stop bits (1 or 2)
         **/

        virtual void set_stop_bits (TicketId ticket, int n) {};

        /**
         * @brief sets the name of the port (/dev/ttyS1 or COM1, etc)
         **/

        virtual void set_port_name (TicketId ticket, char const * name) {};


    protected: //Implementation


    }; // end class ISerialBus

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_DRIVERS_BASE_ISERIALBUS_HPP
