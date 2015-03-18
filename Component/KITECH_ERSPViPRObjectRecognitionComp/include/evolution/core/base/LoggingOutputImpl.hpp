//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE__LOGGING_OUTPUT_IMPL_HPP
#define INCLUDE_EVOLUTION_BASE__LOGGING_OUTPUT_IMPL_HPP

/**
 * @file   evolution/core/base/LoggingOutputImpl.hpp
 * @brief  A class used to output the logging strings.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::LoggingOutputImpl
 */

#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ILoggingLayout.hpp>
#include <evolution/core/base/ILoggingOutput.hpp>
#include <evolution/core/base/IMutex.hpp>
#include <evolution/core/base/Platform.hpp>

namespace Evolution
{
    /**
     * @class LoggingOutputImpl
     * @brief   A class used to output the logging strings.
     *
     * @ingroup core_base
     */
    class EVOLUTION_EXPORT_BASE LoggingOutputImpl : public ILoggingOutput
    {
    public: // Structors
    
        /**
         * @brief Sets the logging output.
         *
         * @param layout [in]; the layout to be used
         */
        LoggingOutputImpl(ILoggingLayout* layout=NULL)
        { 
            _layout = NULL;
            set_layout(layout); 
            _mutex = Platform::create_mutex();
        }

        /// Destructor
        virtual ~LoggingOutputImpl()
        {
            delete_layout();
            delete _mutex;
        }

    public: // Functions

        /// Frees the memory for current layout
        void delete_layout();

        /**
         * @brief Closes the logging output.
         *
         * @return Always true.
         */
        virtual bool close() {return true;}

        /**
         * @brief Reopens the logging output.
         *
         * @return Always true.
         */
        virtual bool re_open(){return true;}

    public: // Output

        /**
         * @brief Set the layout, taking care of freeing the memory.
         *
         * @param layout [in]; The logging layout
         */
        virtual void set_layout(ILoggingLayout *layout);
    
        /**
         * @brief Return the current layout.
         *
         * @return The underlying logging layout.
         */
        virtual ILoggingLayout* get_layout() {return _layout;}

        /**
         * @brief Writes the event into the log output.
         * Note that it would call format before writing into the output
         * to do the desired formatting, and then used write_method to 
         * write.
         * Mutex Lock is only acquired on this method.
         *
         * @param event [in]; The logging event
         */
        virtual void write(const LoggingEvent &event);

        /**
         * @brief Writes the string into the log output.
         * Note that it would NOT call format; instead it would be used by write.
         *
         * @param str [in]; The source string
         *
         * @return true if successful, false otherwise.
         */
        virtual bool write_method(const String &str);

    protected: // Implementation

        ILoggingLayout * _layout;  ///< Layout of the loggiing implementation.
        IMutex* _mutex;            ///< Mutex that controls logging.

    }; // end ILoggingOutput
} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE__LOGGING_OUTPUT_IMPL_HPP
