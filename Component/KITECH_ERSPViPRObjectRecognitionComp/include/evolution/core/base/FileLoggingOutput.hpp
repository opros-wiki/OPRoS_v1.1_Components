//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE__FILE_LOGGING_OUTPUT_HPP
#define INCLUDE_EVOLUTION_BASE__FILE_LOGGING_OUTPUT_HPP


#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ILoggingLayout.hpp>
#include <evolution/core/base/ILoggingOutput.hpp>
#include <evolution/core/base/LoggingOutputImpl.hpp>


/**
 * @file   evolution/core/base/FileLoggingOutput.hpp
 * @brief  Writes logging output to a file.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::FileLoggingOutput
 */


#include <fstream>


namespace Evolution
{


    /**
     * @class FileLoggingOutput
     * @brief Writes logging output to a file.
     *
     * @ingroup core_base
     * @internal
     */
    class EVOLUTION_EXPORT_BASE FileLoggingOutput : public LoggingOutputImpl
    {
    public: // Structors

        /**
         *@brief Set the logging output.
         *@param path [in]; The path to the log file.
         *@param layout [in]; the layout to be used
         */
        FileLoggingOutput(const char* path, ILoggingLayout* layout=NULL);
        virtual ~FileLoggingOutput();

    public: // Functions

        /**
         * @brief Free up the memory for current layout.
         */
        void delete_layout();
        
        /**
         * @brief Closes the logging output.
         *
         * @return True on success.
         */
        virtual bool close();
        
        /**
         * @brief Reopens the logging output.
         *
         * @return True on successful reoopening.
         */
        virtual bool re_open();
    
    public: // Output

        /**
         * @brief Set the layout, taking care of freeing the memory.
         *
         * @param layout [in]; The layout to set for use.
         */
        virtual void set_layout  (ILoggingLayout * layout);

        /**
         * @brief Return the current layout.
         *
         * @return the layout currently used for logging to file.
         */
        virtual ILoggingLayout * get_layout () { return _layout; }
        
        /**
         * @brief Writes the string into the log output with proper formatting.
         *
         * @param str [in]; The log string to write to file.
         *
         * @return True on success. 
         */
        virtual bool write_method(const String &str);
    
    protected: // Implementation

        /**
         * @brief opens the log file.
         */
        void open_file ();

    protected: // Implementation

        const char* _path;  ///< The log file path.
        
        std::ofstream* _stream;  ///< The output stream to write out the log.
    };

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE__LOGGING_OUTPUT_IMPL_HPP
