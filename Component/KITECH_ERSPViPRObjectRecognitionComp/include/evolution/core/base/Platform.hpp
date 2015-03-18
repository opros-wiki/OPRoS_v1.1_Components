//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_PLATFORM__PLATFORM_HPP
#define INCLUDE_EVOLUTION_CORE_PLATFORM__PLATFORM_HPP


/**
 * @file   evolution/core/base/Platform.hpp
 * @brief  Abstracts platform-specific facilities in a generic manner.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see    Evolution::Platform
 * @ingroup core_base
 */


#include <vector>
#include <evolution/core/base/Types.hpp>
#include <evolution/core/base/IThread.hpp>
#include <evolution/core/base/ERSPLibC.hpp>

#ifndef DOXYGEN_INTERNAL
#ifdef EVOLUTION_PLATFORM_LINUX
#define IF_LINUX(x)  x
#define IF_WIN32(x)
#else
#ifdef EVOLUTION_PLATFORM_WIN32
#define IF_LINUX(x)
#define IF_WIN32(x)  x
#else
#endif
#endif
#endif // DOXYGEN_INTERNAL

#ifndef DOXYGEN_INTERNAL
#ifndef __FILE_BASE__
#ifdef EVOLUTION_PLATFORM_WIN32
#include <string.h>
inline const char* win32_basename (const char* filename)
{
    const char* slash = strrchr (filename, '\\');
    return (slash ? slash + 1 : filename);
}
#define __FILE_BASE__  win32_basename(__FILE__)
#else
#define __FILE_BASE__  __FILE__
#endif // EVOLUTION_PLATFORM_WIN32
#endif // __FILE_BASE_
#endif // DOXYGEN_INTERNAL

/** @typedef ProcessId
 *  @brief  Abstraction for the process ID.
 *  @ingroup core_base
 */
#ifdef EVOLUTION_HAVE_PID_T

#include <sys/types.h>

typedef pid_t ProcessId;

#elif defined(EVOLUTION_PLATFORM_WIN32)

#include <evolution/core/base/Win32.hpp>

typedef DWORD ProcessId;

#else

typedef int ProcessId;

#endif


namespace Evolution
{

    class IMutex;
    class IConditionVariable;
    class ISharedLibrary;

    /// List of detected files that follow a predefined pattern.
    typedef std::vector<String> FileList;


    /**
     * @class Platform
     * @brief
     * Abstracts platform-specific facilities in a generic manner. The
     * implementation of this class will vary for each platform, and each implementation is
     * housed in a different file.
     *
     * @ingroup core_base
     * @ingroup core_base_aibo
     **/
    class EVOLUTION_EXPORT_BASE Platform
    {
    public: // Structors

        /// Constructor. Used for system-wide initialization.
        Platform();

        /// Destructor. Used for system-wide shutdown.
        virtual ~Platform();

    public: // Configuration

        /// Standard system properties.
        enum SystemProperty
        {
            INSTALL_PATH,       ///< Path to the root directory of the install.

            CONFIG_PATH,        /**< Paths to the config directories, incorporating
                                 *   any EVOLUTION_CONFIG_PATH environment setting.
                                 **/
            DATA_PATH,          /**< Paths to the data directory, incorporating
                                 *   any EVOLUTION_DATA_PATH environment setting.
                                 **/
            AUDIO_PATH,          /**< Paths to the audio directory, incorporating
                                 *   any EVOLUTION_AUDIO_PATH environment setting.
                                 **/
            IMAGES_PATH,          /**< Paths to the images directory, incorporating
                                 *   any EVOLUTION_IMAGES_PATH environment setting.
                                 **/
            FACES_PATH,         /**< Path to the face data, incorporating any
                                 *   EVOLUTION_DATA_PATH environment setting.
                                 **/
            MODELSETS_PATH,     /**< Path to the object recognition modelsets,
                                 *   incorporating any EVOLUTION_MODELSETS_PATH
                                 *   environment setting.
                                 **/
            SLAM_LANDMARK_PATH /**< Path to the slam subsystem landmark sets,
                                *   incorporating any EVOLUTION_SLAM_LANDMARK_PATH
                                *   environment setting.
                                **/
        };


        /**
         * @brief Obtains the value of the specified system property, using
         * platform-specific configuration methods.
         *
         * @param prop  [in];  System property to obtain
         * @param value [out]; Value of the requested system property
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result get_system_property (SystemProperty prop, String* value);

        /**
         * @brief Obtains the path of a file located in a given
         * system location, a full path, or a file in the current
         * directory using platform-specific configuration methods.
         *
         * @param prop      [in];  System property (list of file directories) to
         *                         check to find the full pathname
         * @param filename  [in];  File to find full pathname of
         * @param full_path [out]; Full pathname (if found in other than current directory)
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result find_system_or_regular_file( SystemProperty prop, const char * filename, 
                                                   String* full_path );

        /**
         * @brief Obtains the full path of a place to write a file
         * based on a given system location.  It tries the first entry
         * in the path for the system property passed in, and if this
         * does not exist, it sets *full_path to a copy of filename.
         *
         * @param prop      [in];  System property (list of file directories) to
         *                         make the full pathname from.
         * @param filename  [in];  File to make full pathname of
         * @param full_path [out]; Full pathname
         */
        static void find_place_to_write( SystemProperty prop, const char * filename, 
                                         String* full_path );

        /**
         * @brief Obtains the full path of a file located in a given system
         * location using platform-specific configuration methods.
         *
         * @param prop      [in];  System property (list of file directories) to
         *                         check to find the full pathname
         * @param filename  [in];  File to find full pathname of
         * @param full_path [out]; Full pathname (if found)
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result find_system_file (SystemProperty prop,
                                        const char * filename,
                                        String* full_path);

        /**
         * @brief Obtains the full path of a file given a list of directories
         * using platform-specific configuration methods.
         *
         * @param directories [in];  List of directories to search
         * @param filename    [in];  File to find full pathname of
         * @param full_path   [out]; Full pathname (if found)
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result find_file (String directories,
                                 const char * filename,
                                 String* full_path);

        /**
         * @brief Determines whether a given file exists.
         *
         * @param filename [in]; File to check existence of
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result file_exists (const char * filename);

        /**
         * @brief Determines whether a given directory exists.
         *
         * @param dirname [in]; Directory to check existence of
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result directory_exists (const char * dirname);

        /**
         * @brief Get the first directory of a series of directories
         * using platform-specific configuration methods.
         *
         * @param directories [in];  Colon separated list of directories to search
         * @param path        [out]; First path found, if found
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result get_first_path (String &directories, String* path);

        /**
         * @brief Get the specified directory from a series of directories
         * using platform-specific configuration methods.
         *
         * @param directories [in];  Colon separated list of directories to search
         * @param sequence    [in];  0-based sequence of directory to get from list
         * @param path        [out]; First path found, if found
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result get_seq_path (String &directories, int sequence, String* path);

    public: // Time

        /**
         * @brief Sleeps for a duration in milliseconds.
         *
         * @param duration [in]; milliseconds to sleep for.
         */
        static void millisecond_sleep (unsigned long duration);

        /**
         * @brief Returns a time value in milliseconds.
         *
         * @return The current time.
         */
        static unsigned long get_millisecond_time ();

        /**
         * @brief Returns a time value in microseconds.
         *
         * @return The current time.
         */
        static double get_microsecond_time ();

        /**
         * @brief Returns a time value in seconds. The time value is the
         * current time, expressed as seconds since the Epoch.
         *
         * @see Types.hpp for Timestamp.
         *
         * @return A Timestamp.
         */
        static Timestamp get_timestamp ();

    public: // File handling

        /**
         * @brief Returns the line delimiter character(s).
         *
         * @return The line delimiter. Do not deallocate this pointer.
         */
        static const char* get_line_delimiter ();

        /**
         * @brief Returns the path delimiter character(s).
         *
         * @return The path delimiter. Do not deallocate this pointer.
         */
        static const char* get_path_delimiter();

        /**
         * @brief Returns the delimiter character(s) for a list of search paths.
         *
         * @return The path search delimiter. Do not deallocate this pointer.
         */
        static const char* get_search_path_delimiter();

        /**
         * @brief Converts a possibly relative path to absolute.
         *
         * @param path     [in];  Path to be made absolute
         * @param abs_path [out]; Value of the absolute pathname
         *
         * @return true/false: could path be converted to absolute?
         */
        static bool make_absolute_path (const char* path, String* abs_path);

        /**
         * @brief Checks whether a path is an absolute path. 
         *        A path that starts with ./ is also considered an
         *        absolute path.
         *
         * @param path [in]; The path to check
         *
         * @return true if the path is absolute, false otherwise.
         */
        static bool is_absolute_path (const char* path);

        /**
         * @brief Returns in 'list' a list of files in the directory 'path',
         * with optional extension filter 'filter'.
         *
         * @param path   [in];  Path to look for files in
         * @param filter [in];  Filter to filter files with
         * @param list   [out]; List of files in dir path w/filter filter
         */
        static Result get_file_list(const char* path,
                                    const char* filter,
                                    FileList* list);

        /**
         * @brief Returns in 'list' a list of subdirectories in the
         * directory 'path'.
         *
         * @param path [in];  Directory to find subdirs of
         * @param list [out]; List of subdirs in path
         */
        static Result get_dir_list(const char* path, FileList* list);

    public: // I/O

        /**
         * @brief Returns true if input is ready to read on stream.
         *
         * @param fstruct [in]; File structure to check
         *
         * @return true/false: file read to be read?
         */
        static bool input_ready(FILE *fstruct);

        /**
         * @brief Returns a character from stdin.
         *
         * @return A char.
         */
        static char get_char();

        /**
         * @brief Returns a line from stdin.
         *
         * @param line [out]; Line read from stdin
         *
         * @return true on success, or false otherwise.
         */
        static bool get_line(String * line);

    public: // Directory manipulation

        /**
         * @brief Creates a directory with the specified mode in Unix.
         *
         * @param path [in]; Directory to create
         * @param mode [in]; Unix permissions mode to set directory to
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result mkdir( const char *path, int mode = 0777 );

        /**
         * @brief Get the current working directory.
         *
         * @param cwd [out]; The string to contain the current directory.
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result get_current_directory(String* cwd);

    public: // Process control

        /**
         * @brief Obtains the ID of the current process.
         *
         * @return The process id.
         */
        static ProcessId get_current_process_id ();

        /**
         * @brief Obtains the ID of the current thread.
         *
         * @return The thread id.
         */
        static ThreadId get_current_thread_id ();

        /// Yields the CPU to the next ready thread.
        static void yield ();

        /**
         * @brief Spawns a new process.
         *
         * @param command [in];  Name of command to execute
         * @param pid     [out]; Pid of command when run
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result spawn(const char* command, int* pid);

        /**
         * @brief Kill a process given the process ID.
         *
         * @param pid [in]; The process id
         *
         * @return RESULT_SUCCESS on success, or an error result on failure.
         */
        static Result kill(int pid);

    public: // Factory

        /**
         * @brief Creates a thread object.
         * @see  Evolution::IThread
         *
         * @param proc     [in]; Procedure to run as thread
         * @param param    [in]; Parameters to pass to thread on startup
         * @param detached [in]; Detach thread or not
         *
         * @return NULL on failure, IThread * to thread on success.
         */
        static IThread* create_thread (ThreadProc proc,
                                       ThreadProcParam param,
                                       bool detached);

        /**
         * @brief  Creates a mutex.
         *
         * Creates a mutex for the current platform.  Mutex could
         * be recursive, in which case successive locks of the mutex
         * in the same thread with no intervening unlocks does not
         * cause a deadlock.  The mutex keeps a lock count in each
         * thread.  If a mutex is not recursive, successive locks of
         * the mutex in the same thread will cause a deadlock.
         *
         * @param recursive [in]; True if the mutex should be recursive
         *
         * @return NULL on failure, IMutex * to mutex on success.
         */
        static IMutex* create_mutex (bool recursive = true);

        /**
         * @brief Creates a condition variable.
         *
         * @return NULL on failure, IConditionVariable * to condition variable on success.
         */
        static IConditionVariable* create_condition_variable ();

        /**
         * @brief Creates a shared library wrapper.
         *
         * @return NULL on failure, ISharedLibrary * to shared lib wrapper on success.
         */
        static ISharedLibrary* create_shared_library ();

    }; // end class Platform

} // end namespace Evolution

#endif  // INCLUDE_EVOLUTION_CORE_PLATFORM__PLATFORM_HPP
