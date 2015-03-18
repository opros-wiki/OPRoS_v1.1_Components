//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE__ILOGGING_LAYOUT_HPP
#define INCLUDE_EVOLUTION_BASE__ILOGGING_LAYOUT_HPP

#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/LoggingTypes.hpp>

/**
 * @file   evolution/core/base/ILoggingLayout.hpp
 * @brief  A conceptual category to log to.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ILoggingLayout
 */

namespace Evolution
{

    /**
     * @class ILoggingLayout
     *
     * @brief An interface for formatting logging information
     * into a logging message string.
     *
     * Logging information is stored in a LoggingEvent object.  This
     * information can be printed out in any number of ways.  The
     * ILoggingLayout is responsible for converting the information
     * in a LoggingEvent object into a string suitable for printing
     * out to the logging device.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE ILoggingLayout
    {
    public: // Structors

        /**
         * @brief Constructor
         */
        ILoggingLayout() {}

        /**
         * @brief Destructor
         */
        virtual ~ILoggingLayout() {}

    public: // Format

        /**
         * @brief Formats the LoggingOutput data to a string that LoggingOutput
         * can log.
         *
         * This method formats the information contained in LoggingEvent
         * into a string suitable for logging. 
         * 
         * @param event [in]; Contains information used to construct the
         * formatted string 
         *
         * @see Evolution::LoggingEvent
         *
         * @return A copy of the string which contains formatting information.
         */
        virtual String format (const LoggingEvent &event) = 0;
    }; // end ILoggingOutput

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_BASE__ILOGGING_LAYOUT_HPP
