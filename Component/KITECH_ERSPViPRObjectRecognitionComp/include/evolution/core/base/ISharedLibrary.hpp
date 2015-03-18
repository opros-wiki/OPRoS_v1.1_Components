//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_ISHARED_LIBRARY_HPP
#define INCLUDE_EVOLUTION_BASE_ISHARED_LIBRARY_HPP


/**
 * @file   evolution/core/base/ISharedLibrary.hpp
 * @brief  Wrapper of a shared library.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see  Evolution::ISharedLibrary
 */


#include <evolution/core/base/IObject.hpp>


namespace Evolution
{


    /**
     * @interface ISharedLibrary
     * @brief
     * Shared library abstraction.
     *
     * The library should search first in the platform-specific
     * library path, then in EVOLUTION_LIBRARY_PATH, and finally in
     * the paths added manually to the search path via add_search_path().
     * Use the Platform::create_shared_library() method to obtain a shared
     * library instance.
     * @see Platform::create_shared_library()
     * @ingroup core_base
     **/
    class EVOLUTION_EXPORT_BASE ISharedLibrary : public IObject
    {
    public: // Constants

        /// Interface ID.
        static const char* const INTERFACE_ID; // "Evolution.ISharedObject";

    public: // Structors

        /// Constructor.
        ISharedLibrary () {}

        /**
         * @brief
         * Destructor. Implementations should unload the library in
         * the destructor.
         **/
        virtual ~ISharedLibrary () {}

    public: // Access

        /** @brief Obtains the name under which the library was searched.
         * @return The name under which the library was search for.
         */
        virtual const char* get_name () const = 0;

        /**
         * @brief
         * Obtains a pointer to the specified symbol in the library.
         *
         * @param  symbol_name [in];  This is the name of the symbol.
         * @param  symbol_ptr  [out]; This is the pointer to the symbol.
         *
         * @pre    is_loaded()
         **/
        virtual Result get_symbol (const char* symbol_name, void** symbol_ptr) const = 0;

    public: // Search

        /**
         * @brief
         * Sets the name for which to search in the library.
         *
         * This name need not include the platform-specific "dot"
         * suffix for shared libraries, but if a suffix is given, it
         * will be searched literally.
         *
         * @param name [in]; name to use as library identifier.
         * @return Result indicating success or failure.
         * @pre  !is_loaded()
         **/
        virtual Result set_name (const char* name) = 0;

        /// Clears the set of search paths.
        virtual void clear_search_path () = 0;

        /** 
         * @brief Adds a search path.
         * @param search_path [in]; path to search through.
         * @return Result indicating success or failure.
         */ 
        virtual Result add_search_path (const char* search_path) = 0;

    public: // Loading

        /** @brief Indicates if the library is loaded.
         * @return bool indicating if library is loaded.
         */ 
        virtual bool is_loaded () const = 0;

        /** @brief Loads the shared library if not loaded.
         * @return Result indicating success or failure.
         */ 
        virtual Result load () = 0;

        /** @brief Unloads the shared library if loaded.
         * @return Result indicating success or failure.
         */ 
        virtual Result unload () = 0;

    }; // end class ISharedLibrary


} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE_ISHARED_LIBRARY_HPP

