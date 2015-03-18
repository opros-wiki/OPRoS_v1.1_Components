//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_BASE__TEST_UTILS_HPP
#define INCLUDE_EVOLUTION_CORE_BASE__TEST_UTILS_HPP


/**
 * @file    evolution/core/base/TestUtils.hpp
 * @brief   Utilities for unit tests.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::fdoutbuf
 * @see Evolution::fdostream
 * @see Evolution::TestUtils
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <streambuf>
#include <evolution/core/base/EnvironmentSetup.hpp>
#include <evolution/core/base/Logging.hpp>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/ERSPLibC.hpp>

namespace Evolution
{

    /**
     * @class fdoutbuf
     * @brief
     * A streambuf-based output buffer that can be associated with an open
     * file descriptor. Used by the ostream-based file descriptor output 
     * stream class: fdostream.
     * 
     * @ingroup core_base
     */
    class fdoutbuf : public std::streambuf 
    {
    public:

        /**
         * @brief Construct file descriptor output buffer.
         */
        fdoutbuf () { _fd = -1; }

        /**
         * @brief Assign file descriptor to buffer.
         *
         * @param fd [in]; File descriptor to associate
         */
        void open (int fd) { _fd = fd; }

    protected:
        /**
         * @brief Handle extra character output.
         *
         * @param c [in]; Character to display
         *
         * @return The character successfully written, EOF otherwise.
         */
        virtual int_type overflow (int_type c)
        {
            if (_fd != -1 && c != EOF)
            {
                char z = c;
                if (ersp_write (_fd, &z, 1) != 1)
                {
                    return EOF; 
                }
            }

            return c;
        }

        /**
         * @brief Handle normal stream output.
         *
         * @param s   [in]; A string to display
         * @param num [in]; Size of the string to display
         *
         * @return The number of bytes successfully written, EOF otherwise.
         */
        virtual std::streamsize xsputn (const char *s, std::streamsize num)
        {
            if (_fd != -1)
            {
                return ersp_write(_fd, s, num);
            }
            else
            {
                return EOF;
            }
        }

    protected:
        int _fd; ///< file descriptor
    };

    /**
     * @class fdostream
     * @brief
     * An ostream-based file descriptor output stream. Can open and write to
     * Unix/Windows file descriptors (ie: stderr).
     * 
     * @ingroup core_base
     */
    class fdostream : public std::ostream 
    {
    public:

        /**
         * @brief Construct the file descriptor output stream class.
         */
        fdostream () :std::ostream(&_buf) { }

        /**
         * @brief Associate the file descriptor with the output stream.
         * It is OK to stream to this class before calling this function, 
         * but you won't get any output.
         *
         * @param fd [in]; Numeric file descriptor
         */
        void open (int fd) 
        { 
            _buf.open(fd); 
        }

    protected:
        fdoutbuf _buf; ///< output fd buffer
    };

    /**
     * @class TestUtils
     * @brief
     * A unit testing utility class.
     * 
     * @ingroup core_base
     */
    class EVOLUTION_EXPORT_BASE TestUtils
    {
    public: // Structors

        /**
         * @brief Construct the testing utility class
         *        If environment variable EVOLUTION_TEST_HARDWARE
         *        is set to "yes" or the test is executed directly
         *        (ie: not using make check), then this need is
         *        satisfied. 
         * 
         * @param requires_hardware [in];  True if the test must interact with hardware
         * @param autoexit [in]; True if autoexit is going to be selected 
         *        (default is false, i.e., no autoexit).
         * @param argc [in]; argc passed from main, used for initializing verbose
         * @param argv [in]; argv passed from main, used for initializing verbose
         */
        TestUtils (int argc,
                   char** argv,
                   bool requires_hardware,
                   bool autoexit = false);

        /**
         * @brief Construct the testing utility class
         *        If environment variable EVOLUTION_TEST_HARDWARE
         *        is set to "yes" or the test is executed directly
         *        (ie: not using make check), then this need is
         *        satisfied. 
         * 
         * @param requires_hardware [in];  True if the test must interact with hardware
         * @param autoexit [in]; True if autoexit is going to be selected 
         *        (default is false, i.e., no autoexit)
         */
        TestUtils (bool requires_hardware,
                   bool autoexit = false);
        /**
         * @brief Standard destructor
         */
        virtual ~TestUtils ();

    public: // Testing


        /**
         * @brief Prints a message describing the next test to
         * standard error.
         * 
         * Takes a standard printf-style format string and subsequent
         * arguments.  A call to describe should be followed by a call
         * to check, fatal_check or ignore.
         *
         * @param fmt [in]; The format string
         * @param ... [in]; Optional arguments
         */
        void describe(const char* fmt, ...);

        /**
         * @brief Tell the testing utility if the current test passed
         * so it can output and record this information.  If autoexit
         * mode was selected, and a test fails, the test program will
         * be terminated with an error result.
         *
         * @param truth_test [in]; True if the test passed and false if the
         * test failed
         * 
         * @return Returns true if all tests have passed so far, false
         * otherwise.
         */
        bool check (bool truth_test);

        /**
         * @brief Test a and b inputs for equality.  Calls check() with
         * the results and prints out the a and b values if they differ.
         *
         * @param a [in]; Left hand side value for equality test
         * @param b [in]; Right hand side value for equality test
         *
         * @return Returns true if all tests have passed so far, false
         * otherwise.
         */
        bool check_equal (double a, double b);

        /**
         * @brief Same as check_equal(), but with epsilon tolerance.
         *
         * @param a [in]; Left hand side value for equality test
         * @param b [in]; Right hand side value for equality test
         * @param epsilon [in]; The tolerance value
         *
         * @return Returns true if all tests have passed so far, false
         * otherwise.
         */
        bool check_close (double a, double b, double epsilon);

        /**
         * @brief When called via ERSP_TEST_CHECK, this method will
         * check truth_test and display the file name and line number
         * where a failure occurred. Then check will be called and the
         * description and FAIL indicator will be on the next line.
         * If success, just calls check which may or may not display 
         * data.
         *
         * @param truth_test [in]; True if the test passed and false if the
         * test failed
         * @param file [in]; The filename used when printing the warning
         * @param line [in]; The line number used when printing the warning
         *
         * @return Returns true if all tests have passed so far, false
         * otherwise.
         */
        bool check_line (bool truth_test, const char *file, int line);

        /**
         * @brief Call check method via check_line method using macro
         * in order to record the current file and line where a failure
         * may occur
         */
        #define ERSP_TEST_CHECK(test_obj,test_cond) \
            test_obj.check_line(test_cond, __FILE_BASE__, __LINE__)

        /**
         * @brief Like check, but used for tests that *must* succeed
         * in order for the test program to continue.
         * 
         * If the test failed, an error message will be displayed and
         * the test program will be terminated.
         *
         * @param truth_test [in]; True is the test passed, false if the
         * test failed
         * @param err [in]; The error message to display if the test fails
         *
         * @return True if successfull, false otherwise.
         */
        bool fatal_check (bool truth_test,
                          const char* err = NULL);

        /**
         * @brief Tells the test utility to ignore the outcome of the
         * current test.
         */
        void ignore ();

    public: // Output

        /**
         * @brief Displays the message if verbose is enabled.
         * 
         * Takes a standard printf-style format string and subsequent
         * arguments.
         *
         * @param fmt [in]; The format string
         * @param ... [in]; Optional arguments
         */
        void msg(const char* fmt, ...);

        /**
         * @brief Writes a message to standard error and then exits
         * the program.
         * 
         * @param err [in]; The error message to be written
         */
        void fatal_error (const char* err = NULL);

        /**
         * @brief Terminates the program.
         * Exits with an error code if all tests did not pass.
         */
        void exit ();

    public: // Status report

        /**
         * @brief  Checks whether all previous tests have passed.
         * 
         * @return  Returns true if all previous tests have passed.
         */
        bool is_ok () const;

        /**
         * @brief Returns the result of the test.
         *
         * @return 0 for success, 1 otherwise
         */
        int result () const;

        /**
         * @brief  Checks if verbose is enabled
         *
         * @return  Returns true if verbose is enabled
         */
        bool verbose_enabled () const { return _verbose; }

        /**
         * @brief Writes the specified error code to stderr in
         * hexadecimal form.
         *
         * If the error code is 0, the message "(no error)" will be
         * written.
         *
         * @param err_code [in]; The error code.
         *
         * @return Returns true if the error code is 0, false otherwise.
         */
        bool verbose_err_code (int err_code);

    public: // Configuration
        /**
         * @brief  Checks if the autoexit mode is selected.
         * 
         * @return  Returns true if autoexit mode is on.
         */
        bool autoexit_selected () const { return _autoexit; }

        /**
         * @brief Turns autoexit on/off.
         * 
         * @param autoexit [in]; If true the autoexit mode is selected
         */
        void set_autoexit (bool autoexit) { _autoexit = autoexit;}

        /**
         * @brief (Re)Initialize the class 
         * 
         * @param argc [in]; The argc parameter from the main function
         * @param argv [in]; The argv parameter from the main function. (VERBOSE/NOVERBOSE)
         */
        inline void init (int argc, char **argv)
        {
#ifndef EVOLUTION_MODULE_SAMPLE_CODE
#ifndef EVOLUTION_MODULE_APPS
            init_internal(argc, argv);
#else
            init_external(argc, argv);
#endif
#else
            init_external(argc, argv);
#endif
        };

        /**
         * @brief  Enables verbose mode.
         */
        void enable_verbose () { _verbose = true; }

        /**
         * @brief  Disables verbose mode.
         */
        void disable_verbose () { _verbose = false; }

    public: // Automake utilities

        /**
         * @brief Indicates if the test is being run
         * interactively (as opposed to batch).
         *
         * @return true if test is being run interactively, false otherwise.
         */
        static bool is_interactive ();

    public: // output buffer

        /**
         * @brief Use this for any stream test debug output to be controlled by
         * the verbose flag.
         */
        fdostream msg_stream;

    protected: // Implementation

        /**
         * @brief  True if all tests have passed so far.
         */
        bool _is_ok;

        /**
         * @brief  True if the test requires hardware.
         *         If environment variable EVOLUTION_TEST_HARDWARE
         *         is set to "yes" or the test is executed directly
         *         (ie: not using make check), then this need is
         *         satisfied. 
         */
        bool _requires_hardware;

        /**
         * @brief  Number of the next test that will be run
         */
        unsigned _current_test;

        /**
         * @brief  True if autoexit is selected.
         */
        bool _autoexit;

        /**
         * @brief  True if verbose is enabled.
         */
        bool _verbose;

        char _describe_buffer[2048]; ///< Buffer to hold the description of the test

        /**
         * @brief  Sets up environment variables and provides functions for
         *         assisting execution of binaries within the source tree
         */
        EnvironmentSetup *_env_setup;

        /**
         * @brief (Re)Initialize the class (when called from a development source tree)
         * 
         * @param argc [in]; The argc parameter from the main function
         * @param argv [in]; The argv parameter from the main function (VERBOSE/NOVERBOSE)
         */
        void init_internal (int argc, char **argv);

        /**
         * @brief (Re)Initialize the class (when called from an installed/distribution tree)
         * 
         * @param argc [in]; The argc parameter from the main function
         * @param argv [in]; The argv parameter from the main function (VERBOSE/NOVERBOSE)
         */
        void init_external (int argc, char **argv);
    }; // end class TestUtils

    /**
     * @brief Convert a value to it's char hex representation.
     *
     * @param x [in]; The value to convert
     *
     * @return The char hex value
     */
    inline char to_hex (int x)
    {
        return ((x > 9) ? x + ('A'-10) : '0' + x);
    }

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_BASE__TEST_UTILS_HPP
