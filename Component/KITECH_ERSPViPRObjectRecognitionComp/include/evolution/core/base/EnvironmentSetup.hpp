//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_BASE__ENVIRONMENT_SETUP_HPP
#define INCLUDE_EVOLUTION_CORE_BASE__ENVIRONMENT_SETUP_HPP


/**
 * @file   evolution/core/base/EnvironmentSetup.hpp
 * @brief  Utility for establishing proper paths for use of executables from
 *         within the source tree.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::EnvironmentSetup
 */


#include <stdlib.h>
#include <string.h>
#include <evolution/core/base/Types.hpp>


namespace Evolution
{

    /**
     * @class EnvironmentSetup
     * @brief
     * A class to setup the libraries and other parts of our environment during
     * development.
     * 
     * @ingroup core_base
     */
    class EVOLUTION_EXPORT_BASE EnvironmentSetup
    {
    public: // Structors

        /**
         * @brief Construct the Environment Setup class.
         * 
         * @param argc [in]; argc passed from main, used for initializing verbose
         * @param argv [in]; argv passed from main, used for initializing verbose
         */
        EnvironmentSetup (int argc, char** argv);

        /**
         * @brief Construct the Environment Setup class, need to call init
         * explicitly. (Useful for sample-code, which compiles as if it
         * were in the source directories).
         *
         * @see TestUtils.
         */
        EnvironmentSetup ();

        /**
         * @brief Standard destructor
         */
        virtual ~EnvironmentSetup ();

    public: // Configuration

        /**
         * @brief (Re)Initialize the class 
         * 
         * @param argc [in]; The argc parameter from main function
         * @param argv [in]; The argv parameter from main function (VERBOSE/NOVERBOSE)
         */
        void init (int argc, char **argv)
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
        }

    public: // Automake utilities

        /**
         * @brief         
         * For a given relative path, prepends the $(srcdir)
         * environment variable if that variable is specified.
         *
         * This method is useful when building the source tree in a
         * separate directory and running tests that require data
         * input files.
         *
         * @param path [in]; Relative path to prepend with $(srcdir).
         * The caller must free the returned memory
         *
         * @return The source prepended source dir. The return value is
         * allocated using new[] and it is the responsibility of the user to
         * release it.
         */
        static char* make_srcdir_path (const char* path);

        /** 
         * @brief
         * Determines if the executable is being run in the source tree.
         *
         * @param argc [in]; argc passed from main, used for initializing verbose
         * @param argv [in]; argv passed from main, used for initializing verbose
         *
         * @return True if in the source tree or false otherwise.
         */
        static bool in_source_tree (int argc, char* argv[]);

#ifndef DOXYGEN_INTERNAL
    private:

        EnvironmentSetup(const EnvironmentSetup& other)
        {
        }

        EnvironmentSetup operator=(const EnvironmentSetup& other)
        {
            return (*this);
        }
#endif // DOXYGEN_INTERNAL

    protected: // Implementation

        char * _env1; ///< clean up the enviornment.
        char * _env2; ///< clean up the enviornment.
        static unsigned int _argv0_ref_count;  ///< reference count for the invocation path
        static char         *_argv0; ///< store the invocation path
        
        /**
         * @brief (Re)Initialize the class (for internal use only)
         * 
         * @param argc The argc parameter from main function
         * @param argv The argv parameter from main function (VERBOSE/NOVERBOSE)
         */
        void init_internal(int argc, char **argv);

        /**
         * @brief (Re)Initialize the class (for installed source)
         * 
         * @param argc The argc parameter from main function
         * @param argv The argv parameter from main function (VERBOSE/NOVERBOSE)
         */
        void init_external(int argc, char **argv);

    }; // end class EnvironmentSetup

}

#endif  // INCLUDE_EVOLUTION_CORE_BASE__ENVIRONMENT_SETUP_HPP
