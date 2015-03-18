//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_LOGGING_LAYOUT_IMPL_HPP
#define INCLUDE_EVOLUTION_BASE_LOGGING_LAYOUT_IMPL_HPP

#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ILoggingLayout.hpp>

/**
 * @file   evolution/core/base/LoggingLayoutImpl.hpp
 * @brief  This is the class that formats the logging output.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::LoggingLayoutImpl
 */


namespace Evolution
{

    /**
     * @class LoggingLayoutImpl
     * @brief An implementation of the ILoggingLayout interface.
     *
     * LoggingLayoutImpl is responsible for converting the logging information
     * encapsulated in a LoggingEvent object into a string suitable for printing
     * out to the logging device.
     *
     * @ingroup core_base
     */
    class EVOLUTION_EXPORT_BASE LoggingLayoutImpl: public ILoggingLayout
    {
    public: // Structors

        /**
         * @brief Constructor
         */
        LoggingLayoutImpl() {}

        /**
         * @brief Destructor
         */
        virtual ~LoggingLayoutImpl() {}

    public: // format
        
        /**
         * @brief Formats the LoggingOutput data to a string that LoggingOutput
         * can log.
         *
         * @param event [in]; Contains infromation used to construct the
         *                    formatted string 
         *
         * @see Evolution::LoggingEvent
         *
         * @return A copy of the string which contains formatting information.
         */
        virtual String format(const LoggingEvent &event);

    }; // end ILoggingOutput


} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_BASE_LOGGING_LAYOUT_IMPL_HPP
