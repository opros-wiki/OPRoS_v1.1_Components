//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE__ILOGGING_OUTPUT_HPP
#define INCLUDE_EVOLUTION_BASE__ILOGGING_OUTPUT_HPP

#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/LoggingTypes.hpp>
#include <evolution/core/base/ILoggingLayout.hpp>


/**
 * @file   evolution/core/base/ILoggingOutput.hpp
 * @brief  An abstraction for logging output.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ILoggingOutput
 */

namespace Evolution
{

    /**
     * @interface ILoggingOutput
     * 
     * @brief   An interface for generating logging outputs to
     *          an arbitrary output source.
     *
     * This interface is responsible for writing logging information
     * to a logging output device, which could be a standard output
     * stream, a text window, a file, etc.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     **/
    class EVOLUTION_EXPORT_BASE ILoggingOutput
    {
    public: // Structors

        /**
         * @brief Constructor
         */
        ILoggingOutput() {}

        /**
         * @brief Destructor
         */
        virtual ~ILoggingOutput(){}

    public: // Functions

        /**
         * @brief Closes the logging output.
         *
         * Closes the logging output.
         *
         * @return Returns true if successful, false otherwise.
         */
        virtual bool close() = 0;

        /**
         * @brief Reopens the logging output.
         *
         * Reopens the logging output.
         *
         * @return Always true.
         */
        virtual bool re_open() = 0;

    public: // Output

        /**
         * @brief Set the layout, taking care of freeing the memory.
         *
         * Set the layout, taking care of freeing the memory.
         *
         * @param layout [in]; The logging layout
         *
         * @see Evolution::ILoggingLayout
         */
        virtual void set_layout(ILoggingLayout *layout) = 0;

        /**
         * @brief Return the current layout.
         *
         * Return the current layout.
         *
         * @return The underlying logging layout.
         *
         * @see Evolution::ILoggingLayout
         */
        virtual ILoggingLayout * get_layout() = 0;

        /**
         * @brief Writes the string into the log output.
         *
         * This method writes the logging message string into an 
         * arbitrary logging output.  Depending on the implementation
         * of this interface, the logging output could be a file,
         * an output stream (like stdout or stderr), a log window,
         * a system logging mechanism, etc.
         *
         * @param str [in]; The source string
         *
         * @return true if successful, false otherwise.
         */
        virtual bool write_method(const String &str) = 0;

        /**
         * @brief Writes the event into the log output.
         *
         * Writes the event into the log output by first
         * formatting the logging event into a logging 
         * message string and calling write_method to
         * write the logging string to the logging output.
         *
         * @param event [in]; The logging event
         *
         * @see Evolution::LoggingEvent
         */
        virtual void write(const LoggingEvent &event) = 0;
    }; // end ILoggingOutput

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_BASE__ILOGGING_OUTPUT_HPP
