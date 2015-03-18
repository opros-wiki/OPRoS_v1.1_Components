/* Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * Any reproduction is strictly prohibited without the explicit
 * written permission of Evolution Robotics, Inc.  All rights reserved.
 */

#ifndef INCLUDE_EVOLUTION_BASE_BASE_C_H
#define INCLUDE_EVOLUTION_BASE_BASE_C_H


#include <evolution/core/base/Export.hpp>


/**
 * @file    evolution/core/base/BaseC.h
 * @brief   C interface layer.
 * @author  Copyright (C) 2001-2007, Evolution Robotics, Inc.
 * @ingroup core_base
 */


#ifdef  __cplusplus
extern "C" {
#endif

#include <stddef.h>


    /*** C TYPES ***/

    /**
     * Q: Why are these all structs instead of typedefs to void*?
     *
     * A: To have some modicum of type safety. With structs you can't
     *    accidentally assign a pointer of one type to another.
     */

    /** C mapping for Evolution::Result. */
    typedef enum 
    {
        ERSP_RESULT_SUCCESS,                ///< 0:  Successful result.
        ERSP_RESULT_FAILURE,                ///< 1:  Generic, unspecified failure. Avoid using.
        ERSP_RESULT_INTERNAL_ERROR,         ///< 2:  An internal, implementation error.
        ERSP_RESULT_PRECONDITION_ERROR,     ///< 3:  A precondition failed.
        ERSP_RESULT_INVALID_ARGUMENT,       ///< 4:  Invalid argument to a method.
        ERSP_RESULT_INVALID_STATE,          ///< 5:  The state requirements of an object method were violated.
        ERSP_RESULT_OUT_OF_MEMORY,          ///< 6:  Out of memory.
        ERSP_RESULT_SYSTEM_ERROR,           ///< 7:  System error.
        ERSP_RESULT_UNKNOWN_ERROR,          ///< 8:  Unknown error, generally an unexpected exception was caught.
        ERSP_RESULT_NOT_IMPLEMENTED,        ///< 9:  This message is not implemented. Please try again later.
        ERSP_RESULT_NOT_FOUND,              ///< 10: The requested item or information was not found.
        ERSP_RESULT_ALREADY_EXISTS,         ///< 11: The item to be stored already exists.
        ERSP_RESULT_NOT_EMPTY,              ///< 12: The container is not empty.
        ERSP_RESULT_OUT_OF_BOUND,           ///< 13: An index or value is out of the accepted bounds.
        ERSP_RESULT_INVALID_CONFIGURATION,  ///< 14: Error in configuration files.
        ERSP_RESULT_NOT_ACTIVE,             ///< 15: The object is not active.
        ERSP_RESULT_BUFFER_TOO_SMALL,       ///< 16: Required buffer size is too small.
        ERSP_RESULT_IO_ERROR,               ///< 17: Local input/output error.
        ERSP_RESULT_INSUFFICIENT_ACCESS,    ///< 18: Insufficient access to perform the requested operation.
        ERSP_RESULT_BUSY,                   ///< 19: An object needed for the operation was busy with another operation.
        ERSP_RESULT_ACQUISITION_ERROR,      ///< 20: Some abstract resource could not be acquired.
        ERSP_RESULT_NETWORK_ERROR,          ///< 21: Generic network error.
        ERSP_RESULT_CORBA_ERROR,            ///< 22: Generic CORBA error.
        ERSP_RESULT_CRC_FAILURE,            ///< 23: Data set failed CRC check.
        ERSP_RESULT_INVALID_TYPE,           ///< 24: Data is of invalid type.
        ERSP_RESULT_TIMEOUT,                ///< 25: The function is timed out
        ERSP_RESULT_FILE_NOT_OPEN,          ///< 26: An error when Trying to use a file which is not open yet
        ERSP_RESULT_UNSUPPORTED_FORMAT      ///< 27: Format not supported
    } ERSPResult;


    /** C mapping for the BehaviorDataType. */
    typedef enum 
    {
        ERSP_DATA_TYPE_UNKNOWN,
        ERSP_DATA_TYPE_BOOLEAN,
        ERSP_DATA_TYPE_ENUM,
        ERSP_DATA_TYPE_DOUBLE,
        ERSP_DATA_TYPE_STRING,
        ERSP_DATA_TYPE_MULTIVALUE,
        ERSP_DATA_TYPE_CHAR_ARRAY,
        ERSP_DATA_TYPE_DOUBLE_ARRAY,
        ERSP_DATA_TYPE_IMAGE,
        ERSP_DATA_TYPE_POINT_SET,
        ERSP_DATA_TYPE_IOBJECT,
        ERSP_DATA_TYPE_POINTER
    } ERSPTaskValueType;

    /** C mappings for logging. */
    typedef enum 
    {
        ERSP_LOG_LEVEL_NONE = 0,
        ERSP_LOG_LEVEL_FATAL  = 100,
        ERSP_LOG_LEVEL_ERROR  = 200,
        ERSP_LOG_LEVEL_WARN   = 300,
        ERSP_LOG_LEVEL_INFO   = 400,
        ERSP_LOG_LEVEL_DEBUG  = 500,
        ERSP_LOG_LEVEL_VERBOSE = 600,
        ERSP_LOG_LEVEL_NOTSET = 1000
    } ERSPLogPriority;

    /** C mappings for default units. */
    typedef enum 
    {
        ERSP_UNIT_DISTANCE = 0,
        ERSP_UNIT_ANGLE    = 1,
        ERSP_UNIT_TIME     = 2
    } ERSPUnitType;

    /** C mappings for colorspaces. */
    typedef enum 
    {
        ERSP_COLORSPACE_UNSPECIFIED = 0x00,                         ///< Unspecified type.
        ERSP_COLORSPACE_GRAYSCALE   = 1 << (sizeof(int) * 8 - 2),   ///< Grayscale.
        ERSP_COLORSPACE_RGB         = 1 << (sizeof(int) * 8 - 3),   ///< RGB format.
        ERSP_COLORSPACE_YUV         = 1 << (sizeof(int) * 8 - 4),   ///< YUV format.
        ERSP_COLORSPACE_YCBCR       = 1 << (sizeof(int) * 8 - 5),   ///< YCbCr format.
        ERSP_COLORSPACE_MASK        = (ERSP_COLORSPACE_GRAYSCALE | ERSP_COLORSPACE_YCBCR
                                       | ERSP_COLORSPACE_YUV | ERSP_COLORSPACE_RGB)
    } ERSPColorspace;

    /** C mappings for color formats. */
    typedef enum 
    {
        ERSP_COLOR_FORMAT_UNSPECIFIED = ERSP_COLORSPACE_UNSPECIFIED,     ///< Default value.

        // YCbCr
        ERSP_COLOR_FORMAT_IYUV      = ERSP_COLORSPACE_YCBCR | 0x00,      ///< Planar YUV 4:2:2
        ERSP_COLOR_FORMAT_I420      =   ERSP_COLOR_FORMAT_IYUV,          ///< Same as IYUV
        ERSP_COLOR_FORMAT_IYU1      = ERSP_COLORSPACE_YCBCR | 0x01,      ///< Packed YUV 4:1:1 horizontal (UYYVYY)
        ERSP_COLOR_FORMAT_IYU2      = ERSP_COLORSPACE_YCBCR | 0x02,      ///< Packed YUV 4:4:4 (UYV)
        ERSP_COLOR_FORMAT_UYVY      = ERSP_COLORSPACE_YCBCR | 0x03,      ///< Packed YUV 4:2:2 horizontal
        ERSP_COLOR_FORMAT_YVU9      = ERSP_COLORSPACE_YCBCR | 0x04,      ///< Planar YUV 4:1:1 (Y, V, U)
        ERSP_COLOR_FORMAT_YV12      = ERSP_COLORSPACE_YCBCR | 0x05,      ///< Planar YUV 4:2:2 (Y, U, V)

        // YUV
        ERSP_COLOR_FORMAT_YUV       = ERSP_COLORSPACE_YUV | 0x00,        ///< Real YUV planar format (not YCbCr) 4:4:4

        // RGB
        ERSP_COLOR_FORMAT_RGB24     = ERSP_COLORSPACE_RGB       | 0x00,  ///< RGB 3bpp interleaved.
        ERSP_COLOR_FORMAT_RGB32     = ERSP_COLORSPACE_RGB       | 0x01,  ///< 4-byte aligned 24-bit RGB

        // Mono.
        ERSP_COLOR_FORMAT_GRAYSCALE = ERSP_COLORSPACE_GRAYSCALE | 0x00,  ///< Friendly name for grayscale.
        ERSP_COLOR_FORMAT_Y800      =   ERSP_COLOR_FORMAT_GRAYSCALE,     ///< Single-byte Luminance (mono)

        // Number of distinct color values (e.g COLOR_FORMAT_GRAYSCALE
        // and COLOR_FORMAT_Y800 count as 1).
        ERSP_COLOR_FORMAT_COUNT     = 11

    } ERSPColorFormat;

    /** C mapping for Timestamp. */
    typedef double ERSPTimestamp;

    /** C mapping for logging. */
    typedef struct 
    {
        void* dummy;  ///< Internal member variable.
    } ERSPLog;

    /** C mapping for Evolution::Array. */
    typedef struct
    {
        void* dummy;  ///< Internal member variable.
    } ERSPArray;

    /** C mapping for Evolution::DoubleArray. */
    typedef struct 
    {
        void* dummy;  ///< Internal member variable.
    } ERSPDoubleArray;

    /**
     * @brief Gets the instance.
     *
     * @param name [in]; identifier string of the LoggingCategory to get
     * an instance of
     *
     * @return Returns the first logging category that matches name.
     */
    extern EVOLUTION_EXPORT
    ERSPLog* ersp_log__get_instance(const char* name);

    /**
     * @brief Sets the logging priority.
     *
     * @param log      [in]; the logging instance
     * @param priority [in]; priority to set current category to
     */
    extern EVOLUTION_EXPORT
    void ersp_log__set_priority(ERSPLog* log, ERSPLogPriority priority);

    /**
     * @brief Log a message with debug priority.
     *
     * @param category  [in]; The logging category
     * @param format    [in]; The format string
     * @param file_name [in]; The name of the file being logged
     * @param line_num  [in]; A line number in the file being logged
     * @param ...       [in]; The argument list
     */
    extern EVOLUTION_EXPORT
    void ersp_log__debug(const char* category,
                         const char* format,
                         const char* file_name,
                         const int line_num,
                         ...);

    /**
     * @brief Log a message with info priority.
     *
     * @param category [in]; The logging category
     * @param format   [in]; Input format string
     * @param ...      [in]; The argument list
     */
    extern EVOLUTION_EXPORT
    void ersp_log__info(const char* category, const char* format, ...);

    /**
     * @brief Log an IO profiling message.
     *
     * @param category [in]; The logging category
     * @param format   [in]; Input format string
     * @param ...      [in]; The argument list
     */
    extern EVOLUTION_EXPORT
    void ersp_log__io_prof(const char* category, const char* format, ...);

    /**
     * @brief Log a message with warn priority.
     *
     * @param category  [in]; The logging category
     * @param format    [in]; The format string
     * @param file_name [in]; The name of the file being logged
     * @param line_num  [in]; A line number in the file being logged
     * @param ...       [in]; The argument list
     */
    extern EVOLUTION_EXPORT
    void ersp_log__warn(const char* category,
                        const char* format,
                        const char* file_name,
                        const int line_num,
                        ...);

    /**
     * @brief Log a message with warn priority.
     *
     * @param category  [in]; The logging category
     * @param format    [in]; The format string
     * @param ...       [in]; The argument list
     */
    extern EVOLUTION_EXPORT
    void ersp_log__warn_no_debug(const char* category,
                                 const char* format,
                                 ...);
    /**
     * @brief Returns the number of elements in the array.
     *
     * @param array [in]; The source array
     *
     * @return The number of elements in the array.
     */
    extern EVOLUTION_EXPORT
    size_t ersp_array__get_size (const ERSPArray* array);

    /**
     * @brief Returns the array's current allocated capacity in number of elements.
     *
     * @param array [in]; The source array
     *
     * @return The capacity of the array.
     */
    extern EVOLUTION_EXPORT
    size_t ersp_array__get_capacity (const ERSPArray* array);

    /**
     * @brief Return the array data buffer pointer.
     *
     * @param array [in]; The source array
     *
     * @return The data bufer.
     */
    extern EVOLUTION_EXPORT
    const void* ersp_array__get_data (const ERSPArray* array);

    /**
     * @brief Guarantees that ersp_array__get_size() returns a specified size.
     *
     * @param array [in]; The array
     * @param size  [in]; The size of the array
     */
    extern EVOLUTION_EXPORT
    void ersp_array__set_size (ERSPArray* array, size_t size);

    /**
     * @brief Clears the array.
     * 
     * @param array [in]; The source array
     */
    extern EVOLUTION_EXPORT
    void ersp_array__clear (ERSPArray* array);

    /**
     * @brief Removes one or more elements from the array.
     *
     * @param array [in]; The array
     * @param start [in]; index to begin removal
     * @param count [in]; number of elements to delete
     *
     * @return  ERSP_RESULT_SUCCESS on success, or an error result on failure.
     */
    extern EVOLUTION_EXPORT
    ERSPResult ersp_array__remove (ERSPArray* array, size_t start, size_t count);

    /**
     * @brief Constucts an ERSPDoubleArray from a buffer of double data.
     *
     * @pre data is a valid pointer.
     *
     * @param data [in]; The double data buffer
     * @param size [in]; Number of double elements in data
     *
     * @return An instance of ERSPDoubleArray.
     */
    extern EVOLUTION_EXPORT
    ERSPDoubleArray* ersp_doublearray__create (const double* data, size_t size);
 
    /**
     * @brief Deallocates an ERSPDoubleArray.
     *
     * @pre array is a valid pointer.
     *
     * @param array [in]; The ERSPDoubleArray to delete
     */
    extern EVOLUTION_EXPORT
    void ersp_doublearray__delete (ERSPDoubleArray* array);

    /**
     * @brief Returns the indexed element.
     *
     * @pre array is a valid pointer.
     * @pre a valid index
     *
     * @param array [in]; The ERSPDoubleArray which contains the element
     * @param index [in]; The 0-based indexed of the desired element
     *
     * @return  The indexed element if found, or 0 if
     *          the index is out of bound.
     */
    extern EVOLUTION_EXPORT
    double ersp_doublearray__get (const ERSPDoubleArray* array, size_t index);

    /**
     * @brief Modifies the indexed element.
     *
     * @pre array is a valid pointer.
     * @pre a valid index
     *
     * @param array [in]; The ERSPDoubleArray which contains the element
     * @param index [in]; The 0-based indexed of the element to modify
     * @param val   [in]; The value to set
     */
    extern EVOLUTION_EXPORT
    void ersp_doublearray__set (ERSPDoubleArray* array, size_t index, double val);

    /**
     * @brief Push a single new element to the back of the array,
     * expand if necessary.
     *
     * @pre array is a valid pointer.
     *
     * @param array [in]; The ERSPDoubleArray which contains the element
     * @param val   [in]; A value to be inserted into the array
     *
     * @return ERSP_RESULT_SUCCESS on success, or an error result on failure.
     */
    extern EVOLUTION_EXPORT
    ERSPResult ersp_doublearray__push_back (ERSPDoubleArray* array, double val);

    /**
     * @brief Returns the ERSPResult as a static C string.
     *
     * @param result [in]; The result code
     *
     * @return The result code name as a string. Do not deallocate this pointer.
     */
    extern EVOLUTION_EXPORT
    const char* ersp_result__get_name (ERSPResult result);

    /**
     * @brief Returns the ERSPResult description as a static C string.
     *
     * @param result [in]; The result code
     *
     * @return A description of the result code. Do not deallocate
     * this pointer
     */
    extern EVOLUTION_EXPORT
    const char* ersp_result__get_description (ERSPResult result);

    /**
     * @brief Sets a key to the specified value and, optionally, saves it to a file.
     *
     * @param key         [in]; The config key name
     * @param value       [in]; The user config value to set
     * @param write_flag  [in]; Save the new value to permanent data store
     *
     * @return ERSP_RESULT_SUCCESS on success or an error code on failure.
     */
    extern EVOLUTION_EXPORT
    ERSPResult ersp_user_config__set_value (const char* key, const char* value, int write_flag);

    /**
     * @brief Retrieves the value given the specified key.
     *
     * @ingroup core_base
     *
     * @param key       [in];  The user config key name
     * @param value_buf [out]; The returned value
     * @param max_len   [in];  The length of the buffer
     *
     * @return ERSP_RESULT_SUCCESS on success or an error code on failure.
     */
    extern EVOLUTION_EXPORT
    ERSPResult ersp_user_config__get_value (const char* key,
                                            char* value_buf,
                                            unsigned int max_len);

    /**
     * @brief Sets the default units.
     *
     * @param type  [in]; The unit type to set
     * @param units [in]; The string identifying the default unit for the
     *                    specified type
     *
     * @return ERSP_RESULT_SUCCESS on success or an error code on failure.
     */
    extern EVOLUTION_EXPORT
    ERSPResult ersp_units__set_default (ERSPUnitType type, const char* units);

    /**
     * @brief Gets the default units.
     *
     * @param type     [in];  The type of default unit to get
     * @param unit_buf [out]; The string identifying the default unit for the
     *                        specified type
     * @param max_len  [in];  The length of the buffer
     *
     * @return ERSP_RESULT_SUCCESS on success or an error code on failure.
     */
    extern EVOLUTION_EXPORT
    ERSPResult ersp_units__get_default (ERSPUnitType type,
                                        char* unit_buf,
                                        unsigned int max_len);

    /**
     * @brief Converts value associated w/specified units into default units.
     *
     * @param type        [in];  The type of unit to convert
     * @param input_units [in];  The unit to convert from
     * @param input       [in];  The unit value to convert
     * @param output      [out]; The converted value in default units   
     *
     * @return ERSP_RESULT_SUCCESS on success, or error code on failure.
     */
    extern EVOLUTION_EXPORT
    ERSPResult ersp_units__convert_to_default (ERSPUnitType type,
                                               const char* input_units,
                                               double input,
                                               double* output);

    /**
     * @brief Converts value associated w/default units into specified units
     *
     * @param type         [in];  The type of unit to convert
     * @param output_units [in];  The unit to which to convert
     * @param input        [in];  The unit value to convert
     * @param output       [out]; The converted value in default units     
     *
     * @return ERSP_RESULT_SUCCESS on success, or error code on failure.
     */
    extern EVOLUTION_EXPORT
    ERSPResult ersp_units__convert_from_default (ERSPUnitType type,
                                                 const char* output_units,
                                                 double input,
                                                 double* output);

    /**
     * @brief Convert the first specified units into the second specified units.
     *
     * @param type         [in];  The type of unit to convert
     * @param input_units  [in];  The unit to convert from
     * @param input        [in];  The unit value to convert
     * @param output_units [in];  The unit to convert to
     * @param output       [out]; The converted value in output units
     *
     * @return ERSP_RESULT_SUCCESS on success, or error code on failure.
     */
    extern EVOLUTION_EXPORT
    ERSPResult ersp_units__convert (ERSPUnitType type, const char* input_units, double input,
                                    const char* output_units, double* output);

#ifndef DOXYGEN_INTERNAL

#ifdef EVOLUTION_PLATFORM_WIN32
    extern EVOLUTION_EXPORT
    const char* ersp_win32_basename(const char* filename);
#endif // EVOLUTION_PLATFORM_WIN32
    
#ifndef __FILE_BASE__

#ifdef EVOLUTION_PLATFORM_WIN32    
#define __FILE_BASE__  ersp_win32_basename(__FILE__)
#else
#define __FILE_BASE__  __FILE__
#endif
    
#endif // __FILE_BASE__

#ifdef  __cplusplus
} /* end extern "C" */
#endif
#endif // DOXYGEN_INTERNAL


#endif  /* INCLUDE_EVOLUTION_BASE_BASE_C_H */
