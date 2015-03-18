//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE__LOGGING_CATEGORY_HPP
#define INCLUDE_EVOLUTION_BASE__LOGGING_CATEGORY_HPP

#include <stddef.h>
#include <stdarg.h>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ERSPLibC.hpp>
#include <evolution/core/base/LoggingTypes.hpp>
#include <evolution/core/base/ILoggingOutput.hpp>

/**
 * @file   evolution/core/base/LoggingCategory.hpp
 * @brief  A conceptual category to which to log.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::LoggingCategory
 */

/**
 * @def ERSP_FUNCTION_ATTRIBUTE_PRINTF
 * @brief Portable handling of printf attributes.
 */
#if    __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ > 4)
#define ERSP_FUNCTION_ATTRIBUTE_PRINTF( format_idx, arg_idx )    \
  __attribute__((format (printf, format_idx, arg_idx)))
#else
#define ERSP_FUNCTION_ATTRIBUTE_PRINTF( format_idx, arg_idx )
#endif


namespace Evolution
{
    /**
     * @class LoggingCategory
     * @brief A conceptual category to which to log.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    class EVOLUTION_EXPORT_BASE LoggingCategory
    {
    public: // Friends

        friend class LoggingHierarchy; ///< Friend class declaration

    public: // Constants

        static const int MAX_CATEGORY_SIZE; ///< Max size of category identifier

    protected: // Constructors

        /**
         * @brief Constructor.
         *
         * @param category_name [in]; The name of the logging category
         * @param parent        [in]; The parent of this category
         * @param priority      [in]; The priority level
         */
        explicit LoggingCategory(const String &category_name,
                                 LoggingCategory *parent = NULL,
                                 LoggingPriority::PriorityLevel priority =
                                                    LoggingPriority::LOG_NOTSET);
    public: // Destructor

        /**
         * @brief Destructor.
         */
        ~LoggingCategory() { remove_logging_output(); }

    public: // Logging methods

        /** 
         * @brief Log a message with verbose priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool verbose (const char* format, ...)
#ifndef DOXYGEN_INTERNAL
        ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3)
#endif
        ;

        /**
         * @brief Log a message with debug priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool debug (const char* format, ...)
#ifndef DOXYGEN_INTERNAL
        ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3)
#endif
        ; 

        /**
         * @brief Log a message with trace priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool trace (const char* format, ...)
#ifndef DOXYGEN_INTERNAL
        ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3)
#endif
        ;

        /**
         * @brief Log a message with info priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool info (const char* format, ...)
#ifndef DOXYGEN_INTERNAL
        ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3)
#endif
        ;

        /**
         * @brief Log a message with warning priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
        */
        bool warn (const char* format, ...)
#ifndef DOXYGEN_INTERNAL
        ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3)
#endif
        ;

        /**
         * @brief Log a message with error priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool error (const char* format, ...)
#ifndef DOXYGEN_INTERNAL
        ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3)
#endif
        ;

        /**
         * @brief Log a message with fatal_error priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
        */
        bool fatal (const char* format, ...) 
#ifndef DOXYGEN_INTERNAL
        ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3)
#endif
        ;

    public:
    
        /**
         * @brief Checking if a specific logging type is enabled.
         *
         * @return true if enabled, false otherwise
         */
        bool logging_enabled () { return (_priority != LoggingPriority::LOG_NOLOG); }

        /**
         * @brief Checking if a specific logging type is disabled.
         *
         * @return true if diabled, false otherwise
         */
        bool logging_disabled () { return (_priority == LoggingPriority::LOG_NOLOG); }

        /**
         * @brief Specifies whether to log a message with verbose priority.
         *
         * @return true if enabled, false otherwise
         */
        bool verbose_enabled () { return (priority_satisfied(LoggingPriority::LOG_VERBOSE)); }

        /**
         * @brief Specifies whether to log a message with debug priority.
         *
         * @return true if enabled, false otherwise
         */
        bool debug_enabled () { return (priority_satisfied(LoggingPriority::LOG_DEBUG)); }

        /**
         * @brief Specifies whether to log a message with trace priority.
         *
         * @return true if enabled, false otherwise
         */
        bool trace_enabled () { return (priority_satisfied(LoggingPriority::LOG_TRACE)); }

        /**
         * @brief Specifies whether to log a message with info priority.
         *
         * @return true if enabled, false otherwise
         */
        bool info_enabled () { return (priority_satisfied(LoggingPriority::LOG_INFO)); }

        /**
         * @brief Specifies whether to log a message with warning priority.
         *
         * @return true if enabled, false otherwise
         */
        bool warn_enabled () { return (priority_satisfied(LoggingPriority::LOG_WARN)); }

        /**
         * @brief Specifies whether to log a message with error priority.
         *
         * @return true if enabled, false otherwise
         */
        bool error_enabled () { return (priority_satisfied(LoggingPriority::LOG_ERROR)); }

        /**
         * @brief Specifies whether to log a message with fatal_error priority.
         *
         * @return true if enabled, false otherwise
         */
        bool fatal_enabled () { return (priority_satisfied(LoggingPriority::LOG_FATAL)); }

    public:
    
        /**
         * @brief Returns the root category.
         *
         * @return The root logging category.
         */
        static LoggingCategory& get_root_category () ;
        
        /**
         * @brief Gets the instance.
         *
         * @param category_name [in]; identifier string of LoggingCategory to get
         * instance of
         *
         * @return Returns a reference to the first LoggingCategory that matches
         * category_name.
         */
        static LoggingCategory& get_instance (const char* category_name);

    public:

        /**
         * @brief Sets the priority of current category.
         *
         * @param priority [in]; priority to set current category to
         */
        void set_priority(LoggingPriority::PriorityLevel priority);

        /**
         * @brief Sets the priority for the root category.
         *
         * @param priority [in]; priority to set root category to
         *
         * @return returns an ILoggingOutput pointer, this category's logging.
         */
        static void set_root_priority(LoggingPriority::PriorityLevel priority);

        /**
         * @brief Returns the logging for this category. If NULL, it returns
         * the parents.
         *
         * @return returns an ILoggingOutput pointer, this categories logging.
         */
        ILoggingOutput * get_logging_output() ;

        /**
         * @brief Sets the logging for this category.
         *
         * @param logging_output [in]; ILoggingOutput interface to use for this
         * categories output
         */
        void set_logging_output(ILoggingOutput * logging_output);

        ///Removes the logging output.
        void remove_logging_output ()
        {
            if(_logging_output != NULL)
            {
                delete _logging_output;
            }
        }

    public:

        /**
         * @brief Gets the priority.
         *
         * @return The priority level of this instance.
         */
        LoggingPriority::PriorityLevel get_priority() { return _priority; }

        /**
         * @brief Gets the granted priority's name (_priority name if set,
         * else parent's granted priority).
         *
         * @return The identifier string for this category's granted priority.
         */
        const char* get_granted_priority_name () { return (LoggingPriority::get_priority_name(granted_priority()));}

        /**
         * @brief Returns the name for the category
         *
         * @return The category name of this category.
         */
        const char* get_category_name()
        {
            return _category_name.c_str();
        }

        /**
         * @brief Returns the priority of set, or else returns the nearest
         * parent's priority which is set.
         *
         * @return The granted priority of set or nearest parent's set priority.
         */
        LoggingPriority::PriorityLevel granted_priority();

    public:

        /**
         * @brief Checks if priority level is satisfied in the current priority.
         *
         * @param priority [in]; priority to check for satisfaction
         *
         * @return priority satisfied or not.
         */
        bool priority_satisfied(const LoggingPriority::PriorityLevel priority)
        {
            return (granted_priority() >= priority);
        }

        /**
         * @brief Given a string that looks like
         * "Category[,Priority][;Category[,Priority]]*" this function parses
         * out the next category and priority.
         *
         * @param specs    [in]; The input string to parse
         * @param start    [in]; The starting point in the spec string
         * @param category [out]; The parsed category
         * @param priority [out]; The parsed priority
         *
         * @return The index at which to begin parsing the next spec.
         */
        static String::size_type parse_logging_spec(String specs,
                                                    String::size_type start,
                                                    String* category,
                                                    String* priority);

    protected: // Helpers

        /**
         * @brief Writes to the log output.
         *
         * @param priority  [in]; The logging priority
         * @param file_name [in]; The name of the file being logged
         * @param line      [in]; A line number in the file being logged
         * @param format    [in]; The format string
         * @param args      [in]; The argumen list
         */
        void log_write(LoggingPriority::PriorityLevel priority,
                       const char *file_name,
                       int line,
                       const char *format,
                       va_list args);
    private:
        void log_write_helper(const char *msg,
            LoggingPriority::PriorityLevel priority,
            const char *file_name,
            int line
            );
            

    protected: // Implementation

        LoggingPriority::PriorityLevel _priority; ///< priority level for this category
        LoggingCategory                *_parent; ///< parent logging category
        ILoggingOutput                 *_logging_output; ///< logging output interface pointer
        const String                   _category_name; ///< category name
    }; // end class LoggingCategory

#ifndef DOXYGEN_INTERNAL
    // internal use only
    /**
     * @class LoggingCategoryPrefixed
     */
    class EVOLUTION_EXPORT_BASE LoggingCategoryPrefixed
    {
    public: // Structors

        /**
         * @brief Constructor.
         *
         * @param category      [in]; The logging category
         * @param prefix_format [in]; The format string
         * @param ...           [in]; The argument list
         */
        LoggingCategoryPrefixed(LoggingCategory *category,
                                const char *prefix_format,
                                ...);
       
        /// Destructor
        ~LoggingCategoryPrefixed() { delete [] _prefix; }

    public:

        /** 
         * @brief Log a message with verbose priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool verbose (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);

        /**
         * @brief Log a message with debug priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool debug (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);

        /**
         * @brief Log a message with trace priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool trace (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
        
        /**
         * @brief Log a message with info priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool info (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
        
        /**
         * @brief Log a message with warning priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
        */
        bool warn (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
        
        /**
         * @brief Log a message with error priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool error (const char* format, ...) 
#ifndef DOXYGEN_INTERNAL
        ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
#endif
        
        /**
         * @brief Log a message with fatal_error priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool fatal (const char* format, ...) 
#ifndef DOXYGEN_INTERNAL
        ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
#endif

    private:

        LoggingCategory *_category;
        char            *_prefix;
    }; // end class LoggingCategoryPrefixed
 
    // internal use only
    class EVOLUTION_EXPORT_BASE LoggingCategoryPostfixed
    {
    public: // Structors

        /**
         * @brief Constructor.
         *
         * @param category       [in]; The logging category
         * @param postfix_format [in]; The format string
         * @param ...            [in]; The argument list
         */
        LoggingCategoryPostfixed(LoggingCategory *category,
                                 const char *postfix_format,
                                 ...);

        /**
         * @brief Constructor.
         *
         * @param category [in]; The logging category
         */
        LoggingCategoryPostfixed(LoggingCategory *category);

        /// Destructor
        ~LoggingCategoryPostfixed() { delete [] _postfix; }

    public:

        /** 
         * @brief Log a message with verbose priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool verbose (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
        
        /**
         * @brief Log a message with debug priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool debug (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
        
        /**
         * @brief Log a message with trace priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool trace (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
        
        /**
         * @brief Log a message with info priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool info (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
        
        /**
         * @brief Log a message with warning priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
        */
        bool warn (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
        
        /**
         * @brief Log a message with error priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool error (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);
        
        /**
         * @brief Log a message with fatal_error priority.
         *
         * @param format [in]; Input format string
         * @param ...    [in]; The argument list
         *
         * @return true if successful, false otherwise.
         */
        bool fatal (const char* format, ...) ERSP_FUNCTION_ATTRIBUTE_PRINTF(2, 3);

    private:
    
        LoggingCategory *_category;
        char            *_postfix;
    }; // end class LoggingCategoryPostfixed
#endif // DOXYGEN_INTERNAL

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE__LOGGING_CATEGORY_HPP
