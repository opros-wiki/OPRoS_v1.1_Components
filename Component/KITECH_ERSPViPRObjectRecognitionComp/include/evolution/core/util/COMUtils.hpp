//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  This material contains trade secrets and confidential information
//  of Evolution Robotics, Inc.  Any use, reproduction, disclosure or
//  dissemination is strictly prohibited without the explicit written
//  permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_CORE_UTIL__COM_UTILS_HPP
#define INCLUDE_EVOLUTION_CORE_UTIL__COM_UTILS_HPP

#ifndef DOXYGEN_INTERNAL

/**
 * @file    evolution/core/util/COMUtils.hpp
 * @brief   Utilities to make COM usable.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @see Evolution::CoInitializeExRunnable
 * @see Evolution::CoUninitializeRunnable
 * @see Evolution::CoCreateInstanceRunnable
 **/


#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <atlbase.h>

#include <evolution/core/base/IRunnable.hpp>
#include <evolution/core/base/ResultCodes.hpp>


namespace Evolution
{

    
    /**
     * @class CoInitializeExRunnable
     * @brief
     * Runnable to initialize COM, to be run from some RunQueue.
     *
     * @see      IRunnable, RunQueue
     * @ingroup  core_util
     **/
    class EVOLUTION_EXPORT_UTIL CoInitializeExRunnable : public IRunnable
    {
    public: // Structors
        /// Constructor. Consider failed until successful run().
        CoInitializeExRunnable (DWORD flags) : _flags (flags), _result (RESULT_FAILURE) {}
    public: // Access
        Result get_result () const { return (_result); }
    public: // IRunnable
        /// Initializes COM.
        Result run ();
    protected:
        DWORD   _flags;
        Result  _result;
    }; // end class CoInitializeExRunnable


    /**
     * @class CoUninitializeRunnable
     * @brief
     * Runnable to uninitialize COM, to be run from some RunQueue.
     *
     * @see      IRunnable, RunQueue
     * @ingroup core_util
     **/
    class EVOLUTION_EXPORT_UTIL CoUninitializeRunnable : public IRunnable
    {
    public: // Structors

        /**
         * @brief The default Constructor to be considered failed until successful run().
         */
        CoUninitializeRunnable () : _result (RESULT_FAILURE) {}
    public: // Access
        Result get_result () const { return (_result); }
    public: // IRunnable

         /**
         * @brief Uninitializes COM
        */
        Result run ();
    protected:
        Result  _result;
    }; // end class CoUninitializeRunnable


    /**
     * @class CoCreateInstanceRunnable
     * @brief Runnable to create an object instance, to be run from some RunQueue.
     *
     * @see      IRunnable, RunQueue
     * @ingroup core_util
     **/
    class EVOLUTION_EXPORT_UTIL CoCreateInstanceRunnable : public IRunnable
    {
    public: // Structors
        /// Constructor. Consider failed until successful run().
        CoCreateInstanceRunnable (REFCLSID clsid,
                                  REFIID iid,
                                  LPUNKNOWN outer_unknown = NULL,
                                  DWORD cls_context = CLSCTX_ALL)
            : _clsid (clsid), _iid (iid), _outer (outer_unknown), _context (cls_context),
              _result (RESULT_FAILURE), _iface (NULL) {}
    public: // Access
        Result get_result () const { return (_result); }
        REFIID get_iid () const { return (_iid); }
        LPVOID get_iface () const { return (_iface); }
    public: // IRunnable
        /**
         * This creates an instance of COM class.
         */
        Result run ();
    protected:
        REFCLSID   _clsid;
        REFIID     _iid;
        LPUNKNOWN  _outer;
        DWORD      _context;
        Result     _result;
        LPVOID     _iface;
    }; // end class CoCreateInstanceRunnable



}

#endif


#endif  // INCLUDE_EVOLUTION_CORE_UTIL__COM_UTILS_HPP

