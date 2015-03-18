//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_SHARED_LIBRARY_TABLE_HPP
#define INCLUDE_EVOLUTION_BASE_SHARED_LIBRARY_TABLE_HPP


/**
 * @file    evolution/core/util/SharedLibraryTable.hpp
 * @brief   Implements a thread-safe table of shared libraries.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::SharedLibraryTable
 **/


#include <evolution/core/base/ISharedLibrary.hpp>
#include <evolution/core/util/AcquirableMap.hpp>


namespace Evolution
{


    /**
     * @class SharedLibraryTable
     * 
     * @brief
     * Simple table of loaded shared libraries, ensuring their proper
     * cleanup and, depending on dlopen() implementations, faster
     * searching on cached libraries.
     *
     * @ingroup core_util
     */
    class EVOLUTION_EXPORT_UTIL SharedLibraryTable : public IAcquirable
    {
    public: // Structors

        /**
         * @brief
         * Constructor.
         */
        SharedLibraryTable ();

        /**
         * @brief
         * Destructor. Releases all loaded libraries.
         */
        virtual ~SharedLibraryTable ();

    public: // IAcquirable interface

        /**
         * @see Evolution::IAcquirable::acquire
         * 
         * @return True if successful.
         */
        bool acquire () { return (_table.acquire ()); }

        /**
         * @see Evolution::IAcquirable::release
         *
         * @return True if successful.
         */
        bool release () { return (_table.release ()); }

        /**
         * @see Evolution::IAcquirable::is_acquired
         *
         * @return True if successful.
         */
        bool is_acquired () const { return (_table.is_acquired ()); }

    public: // Loading

        /**
         * @brief
         * Clears the set of search paths.
         */
        void clear_search_path ();

        /**
         * @brief
         * Adds a search path.
         *
         * @param search_path [in]; The search path to add.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         */
        Result add_search_path (const char* search_path);

        /**
         * @brief
         * Returns the specified library, loading it if necessary.
         *
         * Because the table is responsible for managing the reference
         * counts of the returned library objects, they should not be
         * manipulated except through the table's interface (the
         * returned pointer is const, so the ref count cannot be
         * changed without dirty casting). One more subtle implication
         * of this is that the returned library pointers should not be
         * copied around to other contexts, unless the original
         * calling context can keep track of the copying. Other
         * contexts instead should use the table directly.
         *
         * @param  name        [in];  Name under which to search for the library
         * @param  library_ptr [out]; Pointer to the library
         *
         * @pre    name != NULL
         * @pre    library_ptr != NULL
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        Result load_library (const char* name,
                             const ISharedLibrary** library_ptr) ;

        /**
         * @brief
         * Releases a reference to the library, unloading it if no
         * references remain.
         * 
         * @param name [in]; The name of the library to release.
         *
         * @return RESULT_SUCCESS on success or an error code on failure.
         **/
        Result unload_library (const char* name);

    protected: // Implementation
        /**
         * @brief
         * The shared library table type as a map.
         */
        typedef AcquirableMap <String, ISharedLibrary*> TableType;

        String       _search_path;   ///< Search path for libraries.
        TableType    _table;         ///< The actual table.
    }; // end class SharedLibraryTable


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE_SHARED_LIBRARY_TABLE_HPP
