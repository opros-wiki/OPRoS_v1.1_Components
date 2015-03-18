//  Copyright (C) 2001-2007, Evolution Robotics, Inc.
//  Any reproduction is strictly prohibited without the explicit
//  written permission of Evolution Robotics, Inc.  All rights reserved.


#ifndef INCLUDE_EVOLUTION_BASE_RESULT_CODES_HPP
#define INCLUDE_EVOLUTION_BASE_RESULT_CODES_HPP


/**
 * @file   evolution/core/base/ResultCodes.hpp
 * @brief  This file declares standard result codes for function results.
 * @author Copyright (C) 2001-2007, Evolution Robotics, Inc.
 */


#include <evolution/core/base/Export.hpp>
#include <evolution/core/base/Logging.hpp>


namespace Evolution
{
    /**
     * @ingroup core_base
     * @ingroup core_base_aibo
     */
    typedef enum
    {
        RESULT_SUCCESS,                ///< 0:  Successful result.
        RESULT_FAILURE,                ///< 1:  Generic, unspecified failure. Avoid using.
        RESULT_INTERNAL_ERROR,         ///< 2:  An internal, implementation error.
        RESULT_PRECONDITION_ERROR,     ///< 3:  A precondition failed.
        RESULT_INVALID_ARGUMENT,       ///< 4:  Invalid argument to a method.
        RESULT_INVALID_STATE,          ///< 5:  The state requirements of an object method were violated.
        RESULT_OUT_OF_MEMORY,          ///< 6:  Out of memory.
        RESULT_SYSTEM_ERROR,           ///< 7:  System error.
        RESULT_UNKNOWN_ERROR,          ///< 8:  Unknown error, generally an unexpected exception was detected.
        RESULT_NOT_IMPLEMENTED,        ///< 9:  This message is not implemented. Please try again later.
        RESULT_NOT_FOUND,              ///< 10: The requested item or information was not found.
        RESULT_ALREADY_EXISTS,         ///< 11: The item to be stored already exists.
        RESULT_NOT_EMPTY,              ///< 12: The container is not empty.
        RESULT_OUT_OF_BOUND,           ///< 13: An index or value is out of the accepted bounds.
        RESULT_INVALID_CONFIGURATION,  ///< 14: Error in configuration files.
        RESULT_NOT_ACTIVE,             ///< 15: The object is not active.
        RESULT_BUFFER_TOO_SMALL,       ///< 16: Required buffer size is too small.
        RESULT_IO_ERROR,               ///< 17: Local input/output error.
        RESULT_INSUFFICIENT_ACCESS,    ///< 18: Insufficient access to perform the requested operation.
        RESULT_BUSY,                   ///< 19: An object needed for the operation was busy with another operation.
        RESULT_ACQUISITION_ERROR,      ///< 20: Some abstract resource could not be acquired.
        RESULT_NETWORK_ERROR,          ///< 21: Generic network error.
        RESULT_CORBA_ERROR,            ///< 22: Generic CORBA error.
        RESULT_CRC_FAILURE,            ///< 23: Data set failed CRC check.
        RESULT_INVALID_TYPE,           ///< 24: Data is of invalid type.
        RESULT_TIMEOUT,                ///< 25: The function is timed out.
        RESULT_FILE_NOT_OPEN,          ///< 26: An error when trying to use a file which is not open yet.
        RESULT_UNSUPPORTED_FORMAT      ///< 27: Format not supported      
    } ResultCode;

    /**
     * @brief Returns the enum's name as a static C string.
     *
     * @param code [in]; The result code's enum value
     *
     * @return The result code name string. Do not deallocate this pointer.
     */
    EVOLUTION_EXPORT_BASE const char * result_code_get_name (ResultCode code);

    /**
     * @brief Returns the enum's description as a static C string.
     *
     * @param code [in]; The result code's enum value
     *
     * @return A description of the result code. Do not deallocate
     * this pointer
     */
    EVOLUTION_EXPORT_BASE const char * result_code_get_str (ResultCode code);

} // end namespace Evolution


#endif  // INCLUDE_EVOLUTION_BASE_RESULT_CODES_HPP
