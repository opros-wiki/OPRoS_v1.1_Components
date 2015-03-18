//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_LOGGING_TYPES_HPP
#define INCLUDE_EVOLUTION_BASE_LOGGING_TYPES_HPP

/**
 * @file   evolution/core/base/LoggingTypes.hpp
 * @brief  Set of classes used for the logging infrastructure.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::LoggingTimeStamp
 * @see    Evolution::LoggingPriority
 * @see    Evolution::LoggingEvent
 */

#include <sstream>
#include <evolution/core/base/Types.hpp>

namespace Evolution
{
    /// Convinience typedef for std::stringstream
    typedef std::stringstream LoggingStream;

    /**
     * @class LoggingTimeStamp
     * @brief A simple timestamp type for logging.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */ 
    class EVOLUTION_EXPORT_BASE LoggingTimeStamp
    {
    public:
        /**
         * @brief Constructor.
         */
        LoggingTimeStamp();

        /**
         * @brief Set the timestamp.
         *
         * @param ts [in]; The new time value
         */
        void set_time(int ts)
        {
            _ts = ts;
        }

        /**
         * @brief Get the timestamp.
         *
         * @return The timestamp.
         */
        int get_time() const
        {
            return _ts;
        }

        /**
         * @brief Returns the textual representation of the timestamp.
         *
         * @return A copy of the string which contains the textual
         * representation of the timestamp.
         */
        String get_time_string() const;

    protected:
#ifndef DOXYGEN_INTERNAL
        int _ts; ///< The timestamp.
#endif // end DOXYGEN_INTERNAL

    }; // end class LoggingTimeStamp

    /**
     * @class LoggingPriority
     * @brief  Logging priorities.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     **/ 
    class EVOLUTION_EXPORT_BASE LoggingPriority
    {
    public:

        /// Priority levels.
        typedef enum
        {
            LOG_NOLOG   = 0,   ///< 0: No logging.
            LOG_FATAL   = 100, ///< 100: Fatal error messages.
            LOG_ERROR   = 200, ///< 200: Nonfatal error messages.
            LOG_WARN    = 300, ///< 300: Warning messages.
            LOG_INFO    = 400, ///< 400: Informational messages.
            LOG_TRACE   = 450, ///< 450: Debugging messages available in release build.
            LOG_DEBUG   = 500, ///< 500: Debugging messages.
            LOG_VERBOSE = 600, ///< 600: Really gratuitous and annoying messages.
            LOG_NOTSET  = 1000 ///< 1000: No priority set.
        } PriorityLevel;

        /**
         * @brief Returns the name of the priority.
         *
         * @param priority [in]; The enum value of the priority level
         *
         * @return The textual representation of the numeric priority level.
         */
        static const char* get_priority_name (PriorityLevel priority);

        /// A default priority --> defined in cpp file
        static const PriorityLevel _default_priority;
    }; // end class LoggingPriority

    /**
     * @class LoggingEvent
     * @brief Logging events.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     **/ 
    class EVOLUTION_EXPORT_BASE LoggingEvent
    {
    public:
        
        /**
         * @brief Constructor.
         *
         * @param __cat_name [in]; The categoray name of the logging event
         * @param __msg      [in]; The message contents of the logging event
         * @param __priority [in]; The logging priority rating
         * @param __fn       [in]; The name of the file in which the statement
         *                         originated
         * @param __line     [in]; The line of the file in which the statement
         *                         originated
         */
        LoggingEvent (const String & __cat_name, 
                      const String & __msg, 
                      LoggingPriority::PriorityLevel __priority,
                      const char* __fn,
                      const int __line)
            : category_name(__cat_name),
              message(__msg),
              priority (__priority),
              file_name(__fn),
              line(__line)
        {
        }

    public:
#ifndef DOXYGEN_INTERNAL
        const String     category_name; ///< Category name of a logging event.
        const String     message;       ///< Message contents of a logging event.
        LoggingPriority::PriorityLevel priority; ///< Priority rating of a logging event.
        const char*      file_name;     ///< Name of the file in which the statement originated.
        const int        line;          ///< Line of file in which the statement originated.
        LoggingTimeStamp time_stamp;    ///< Time stamp of the logging event.
#endif // end DOXYGEN_INTERNAL
    }; // end class LoggingEvent

} // end namespace Evolution

#endif // INCLUDE_EVOLUTION_BASE_LOGGING_TYPES_HPP
