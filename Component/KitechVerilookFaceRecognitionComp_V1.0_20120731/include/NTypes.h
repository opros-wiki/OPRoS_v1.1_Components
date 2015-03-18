/** \file NTypes.h \brief Defines types and macros used in Neurotechnology components. */

/******************************************************************************\
|*                                                                            *|
|*                          Neurotechnology Core 2.4                          *|
|*                                                                            *|
|* NTypes.h                                                                   *|
|* Types and various defines definition                                       *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef N_TYPES_H_INCLUDED
#define N_TYPES_H_INCLUDED

// C++ detection
#ifdef __cplusplus
#define N_CPP ///< Defined if compiling as C++ code.
#endif

#ifdef N_CPP
extern "C"
{
#endif

// Debug detection
#ifdef _DEBUG
#define N_DEBUG ///< Defined if compiling in debug mode.
#endif

// Static library detection
#ifdef _LIB
#define N_LIB ///< Defined if compiling static library.
#endif

// Windows detection
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
#define N_WINDOWS ///< Defined if compiling for Windows.
#endif

// Linux detection
#ifdef __linux__
#define N_LINUX ///< Defined if compiling for Linux.
#endif

// Mac detection
#ifdef __APPLE__
#define N_MAC ///< Defined if compiling for Mac OS.
#endif

#ifdef _AIX
#define N_AIX
#endif

#ifndef N_DOCUMENTATION
#ifndef N_WINDOWS
#define N_NO_UNICODE ///< Defined if compiling without Unicode support.
#endif
#endif

// Unicode detection
#if defined(_UNICODE) || defined(UNICODE)
#define N_UNICODE
#endif

// Compiler detection

#ifdef _MSC_VER
#if (_MSC_VER >= 1300)
#define N_MSVC ///< Defined if compiling with Microsoft Visual C++.
#else
#define N_NO_INT_64 ///< Defined if compiling for platform without 64-bit integer types support.
#endif
#endif

#ifdef __GNUC__
#define N_GCC ///< Defined if compiling with GCC.
#endif

#if defined(__STDC__) && !defined(N_GCC)
#define N_ANSI_C ///< Defined if ANSI C language compliance is enabled in compiler.
#endif

// Struct packing/alignment
#ifdef N_GCC
#define N_PACKED __attribute__((__packed__))
#else
#define N_PACKED
#endif

// Deprecation
#if defined(__GNUC__) && (__GNUC__ - 0 > 3 || (__GNUC__ - 0 == 3 && __GNUC_MINOR__ - 0 >= 2))
#define N_DEPRECATED(message) __attribute__ ((deprecated))
#elif defined(N_MSVC)
#define N_DEPRECATED(message) __declspec(deprecated(message))
#else
#define N_DEPRECATED(message)
#endif

// Processor detection

#if defined(__POWERPC__) || defined(_POWER)
#define N_BIG_ENDIAN ///< Defined if compiling for big-endian processor architecture.
#endif

#if defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__x86_64__) || defined(__POWERPC__)
#define N_FAST_FLOAT ///< Defined if CPU has floating-point instructions
#endif

#if defined(_M_X64) || defined(__x86_64__)
#define N_64 ///< Defined if compiling for 64-bit architecture.
#endif

//#define N_NO_FLOAT
//#define N_NO_INT_64

#if defined(N_64) && defined(N_NO_INT_64)
#error N_64 and N_NO_INT_64 defined simultaneously
#endif

// Integer types

#ifdef N_MSVC

typedef unsigned __int8  NUInt8;  ///< 8-bit unsigned integer (byte).
typedef signed   __int8  NInt8;   ///< 8-bit signed integer (signed byte).
typedef unsigned __int16 NUInt16; ///< 16-bit unsigned integer (unsigned short).
typedef signed   __int16 NInt16;  ///< 16-bit signed integer (short).
typedef unsigned __int32 NUInt32; ///< 32-bit unsigned integer (unsigned int).
typedef signed   __int32 NInt32;  ///< 32-bit signed integer (int).

#define N_UINT8_MIN 0x00ui8         ///< Minimum value for #NUInt8.
#define N_UINT8_MAX 0xFFui8         ///< Maximum value for #NUInt8.
#define N_INT8_MIN 0x80i8           ///< Minimum value for #NInt8.
#define N_INT8_MAX 0x7Fi8           ///< Maximum value for #NInt8.
#define N_UINT16_MIN 0x0000ui16     ///< Minimum value for #NUInt16.
#define N_UINT16_MAX 0xFFFFui16     ///< Maximum value for #NUInt16.
#define N_INT16_MIN 0x8000i16       ///< Minimum value for #NInt16.
#define N_INT16_MAX 0x7FFFi16       ///< Maximum value for #NInt16.
#define N_UINT32_MIN 0x00000000ui32 ///< Minimum value for #NUInt32.
#define N_UINT32_MAX 0xFFFFFFFFui32 ///< Maximum value for #NUInt32.
#define N_INT32_MIN 0x80000000i32   ///< Minimum value for #NInt32.
#define N_INT32_MAX 0x7FFFFFFFi32   ///< Maximum value for #NInt32.

#else

typedef unsigned char  NUInt8;  ///< 8-bit unsigned integer (byte).
typedef signed   char  NInt8;   ///< 8-bit signed integer (signed byte).
typedef unsigned short NUInt16; ///< 16-bit unsigned integer (unsigned short).
typedef signed   short NInt16;  ///< 16-bit signed integer (short).
typedef unsigned int   NUInt32; ///< 32-bit unsigned integer (unsigned int).
typedef signed   int   NInt32;  ///< 32-bit signed integer (int).

#define N_UINT8_MIN ((NUInt8)0x00u)     ///< Minimum value for #NUInt8.
#define N_UINT8_MAX ((NUInt8)0xFFu)     ///< Maximum value for #NUInt8.
#define N_INT8_MIN ((NInt8)0x80)        ///< Minimum value for #NInt8.
#define N_INT8_MAX ((NInt8)0x7F)        ///< Maximum value for #NInt8.
#define N_UINT16_MIN ((NUInt16)0x0000u) ///< Minimum value for #NUInt16.
#define N_UINT16_MAX ((NUInt16)0xFFFFu) ///< Maximum value for #NUInt16.
#define N_INT16_MIN ((NInt16)0x8000)    ///< Minimum value for #NInt16.
#define N_INT16_MAX ((NInt16)0x7FFF)    ///< Maximum value for #NInt16.
#define N_UINT32_MIN 0x00000000u        ///< Minimum value for #NUInt32.
#define N_UINT32_MAX 0xFFFFFFFFu        ///< Maximum value for #NUInt32.
#define N_INT32_MIN 0x80000000          ///< Minimum value for #NInt32.
#define N_INT32_MAX 0x7FFFFFFF          ///< Maximum value for #NInt32.

#endif

#ifndef N_NO_INT_64

#ifdef N_MSVC

typedef unsigned __int64 NUInt64; ///< 64-bit unsigned integer (unsigned long). Not available on some 32-bit platforms.
typedef signed   __int64 NInt64;  ///< 64-bit signed integer (long). Not available on some 32-bit platforms.

#define N_UINT64_MIN 0x0000000000000000ui64 ///< Minimum value for #NUInt64.
#define N_UINT64_MAX 0xFFFFFFFFFFFFFFFFui64 ///< Maximum value for #NUInt64.
#define N_INT64_MIN 0x8000000000000000i64   ///< Minimum value for #NInt64.
#define N_INT64_MAX 0x7FFFFFFFFFFFFFFFi64   ///< Maximum value for #NInt64.

#else

typedef unsigned long long NUInt64; ///< 64-bit unsigned integer (long). Not available on some 32-bit platforms.
typedef signed   long long NInt64;  ///< 64-bit signed integer (long). Not available on some 32-bit platforms.

#define N_UINT64_MIN 0x0000000000000000ull ///< Minimum value for #NUInt64.
#define N_UINT64_MAX 0xFFFFFFFFFFFFFFFFull ///< Maximum value for #NUInt64.
#define N_INT64_MIN 0x8000000000000000ll   ///< Minimum value for #NInt64.
#define N_INT64_MAX 0x7FFFFFFFFFFFFFFFll   ///< Maximum value for #NInt64.

#endif

#endif

// Shorthand integer types

typedef NUInt8 NByte;    ///< Same as NUInt8.
typedef NInt8 NSByte;    ///< Same as NInt8.
typedef NUInt16 NUShort; ///< Same as NUInt16.
typedef NInt16 NShort;   ///< Same as NInt16.
typedef NUInt32 NUInt;   ///< Same as NUInt32.
typedef NInt32 NInt;     ///< Same as NInt32.

#ifndef N_NO_INT_64

typedef NUInt64 NULong; ///< Same as NUInt64.
typedef NInt64 NLong;   ///< Same as NInt64.

#endif

#define N_BYTE_MIN N_UINT8_MIN    ///< Minimum value for #NByte.
#define N_BYTE_MAX N_UINT8_MAX    ///< Maximum value for #NByte.
#define N_SBYTE_MIN N_INT8_MIN    ///< Minimum value for #NSByte.
#define N_SBYTE_MAX N_INT8_MAX    ///< Maximum value for #NSByte.
#define N_USHORT_MIN N_UINT16_MIN ///< Minimum value for #NUShort.
#define N_USHORT_MAX N_UINT16_MAX ///< Maximum value for #NUShort.
#define N_SHORT_MIN N_INT16_MIN   ///< Minimum value for #NShort.
#define N_SHORT_MAX N_INT16_MAX   ///< Maximum value for #NShort.
#define N_UINT_MIN N_UINT32_MIN   ///< Minimum value for #NUInt.
#define N_UINT_MAX N_UINT32_MAX   ///< Maximum value for #NUInt.
#define N_INT_MIN N_INT32_MIN     ///< Minimum value for #NInt.
#define N_INT_MAX N_INT32_MAX     ///< Maximum value for #NInt.

#ifndef N_NO_INT_64

#define N_ULONG_MIN N_UINT64_MIN ///< Minimum value for #NULong.
#define N_ULONG_MAX N_UINT64_MAX ///< Maximum value for #NULong.
#define N_LONG_MIN N_INT64_MIN   ///< Minimum value for #NLong.
#define N_LONG_MAX N_INT64_MAX   ///< Maximum value for #NLong.

#endif

#ifndef N_NO_FLOAT

// Float types
typedef float NSingle;  ///< Single precision floating point number.
typedef double NDouble; ///< Double precision floating point number.

#define N_SINGLE_MIN 1.175494351e-38F            ///< Minimum value for #NSingle.
#define N_SINGLE_MAX 3.402823466e+38F            ///< Maximum value for #NSingle.
#define N_SINGLW_EPSILON 1.192092896e-07F        ///< Epsilon value for #NSingle.
#define N_DOUBLE_MIN 2.2250738585072014e-308     ///< Minimum value for #NDouble.
#define N_DOUBLE_MAX 1.7976931348623158e+308     ///< Maximum value for #NDouble.
#define N_DOUBLE_EPSILON 2.2204460492503131e-016 ///< Epsilon value for #NDouble.

// Shorthand float type
typedef NSingle NFloat; ///< Same as NSingle.

#define N_FLOAT_MIN N_SINGLE_MIN         ///< Minimum value for #NFloat.
#define N_FLOAT_MAX N_SINGLE_MAX         ///< Maximum value for #NFloat.
#define N_FLOAT_EPSILON N_SINGLE_EPSILON ///< Epsilon value for #NFloat.

#endif

/// 32-bit boolean value. See also #NTrue and #NFalse.
typedef int NBoolean;

// Boolean constants
#define NTrue 1  ///< True value for #NBoolean.
#define NFalse 0 ///< False value for #NBoolean.

/// Same as #NBoolean.
typedef NBoolean NBool;

// Character types

#if defined(N_NO_UNICODE) && defined(N_UNICODE)
#error "N_NO_UNICODE and N_UNICODE defined simultaneously"
#endif

typedef char NAChar; ///< ANSI character (8-bit).

#ifndef N_NO_UNICODE

/// Unicode character (16-bit).
#ifndef _WCHAR_T_DEFINED
typedef NUShort NWChar;
#else
typedef wchar_t NWChar;
#endif

#endif // !N_NO_UNICODE

// Char type

/// Character type. Either #NAChar or #NWChar (if N_UNICODE is defined).
#ifdef N_UNICODE
typedef NWChar NChar;
#else
typedef NAChar NChar;
#endif

/**
 * \brief  Makes either ANSI or Unicode (if N_UNICODE is defined) string or
 *         character constant.
 */
#ifdef N_UNICODE
#define N_T_(text) L##text
#else
#define N_T_(text) text
#endif

#define N_T(text) N_T_(text)

#ifdef N_UNICODE
#define N_FUNC_AW(name) name##W     ///< Picks either ANSI or Unicode (if N_UNICODE is defined) version of the function (with either 'A' or 'W' suffix accordingly).
#define N_CALLBACK_AW(name) name##W ///< Picks either ANSI or Unicode (if N_UNICODE is defined) version of the callback (with either 'A' or 'W' suffix accordingly).
#define N_STRUCT_AW(name) name##W   ///< Picks either ANSI or Unicode (if N_UNICODE is defined) version of the struct (with either 'A' or 'W' suffix accordingly).
#else
#define N_FUNC_AW(name) name##A     ///< Picks either ANSI or Unicode (if N_UNICODE is defined) version of the function (with either 'A' or 'W' suffix accordingly).
#define N_CALLBACK_AW(name) name##A ///< Picks either ANSI or Unicode (if N_UNICODE is defined) version of the callback (with either 'A' or 'W' suffix accordingly).
#define N_STRUCT_AW(name) name##A   ///< Picks either ANSI or Unicode (if N_UNICODE is defined) version of the struct (with either 'A' or 'W' suffix accordingly).
#endif

#ifdef WINCE
#define N_NO_ANSI_FUNC ///< Defined if compiling for platform without ANSI versions of the functions support.
#ifndef N_UNICODE
#error "N_UNICODE must be defined under Windows CE"
#endif
#endif

#if defined(N_NO_ANSI_FUNC) && !defined(N_UNICODE)
#error "N_NO_ANSI_FUNC defined when N_UNICODE is not defined"
#endif

// Size type
#ifdef N_64

/**
 * \brief  Platform dependent size type. Unsigned 32-bit integer on 32-bit
 *         platform, unsigned 64-bit integer on 64-bit platform.
 */
typedef NUInt64 NSizeType;

#define N_SIZE_TYPE_MIN N_UINT64_MIN ///< Minimum value for #NSizeType.
#define N_SIZE_TYPE_MAX N_UINT64_MAX ///< Maximum value for #NSizeType.

#else

/**
 * \brief  Platform dependent size type. Unsigned 32-bit integer on 32-bit
 *         platform, unsigned 64-bit integer on 64-bit platform.
 */
#ifdef N_MSVC
typedef __w64 NUInt32 NSizeType;
#else
typedef NUInt32 NSizeType;
#endif

#define N_SIZE_TYPE_MIN N_UINT32_MIN ///< Minimum value for #NSizeType.
#define N_SIZE_TYPE_MAX N_UINT32_MAX ///< Maximum value for #NSizeType.

#endif

// Pos type

#ifndef N_NO_INT_64

/**
 * \brief  Platform dependent position type. Signed 64-bit (or 32-bit on some
 *         platforms) integer on 32-bit platform, signed 64-bit integer on
 *         64-bit platform).
 */
typedef NInt64 NPosType;

#define N_POS_TYPE_MIN N_INT64_MIN ///< Minimum value for #NPosType.
#define N_POS_TYPE_MAX N_INT64_MAX ///< Maximum value for #NPosType.

#else

/**
 * \brief Platform dependent position type. Signed 64-bit (or 32-bit on some
 *        platforms) integer on 32-bit platform, signed 64-bit integer on
 *        64-bit platform).
 */
#ifdef N_MSVC
typedef __w64 NInt32 NPosType;
#else
typedef NInt32 NPosType;
#endif

#define N_POS_TYPE_MIN N_INT32_MIN ///< Minimum value for #NPosType.
#define N_POS_TYPE_MAX N_INT32_MAX ///< Maximum value for #NPosType.

#endif

#ifndef NULL
	#define NULL 0 ///< Null value for pointer.
#endif

/// Result of a function (same as NInt). See also NErrors.h.
typedef NInt NResult;

// API and callback functions' calling convention

#ifdef N_WINDOWS


#define N_API __stdcall      ///< Defines functions calling convention (stdcall on Windows).
#define N_CALLBACK __stdcall ///< Defined callbacks calling convention (stdcall on Windows).

#else

#define N_API      ///< Defines functions calling convention (stdcall on Windows).
#define N_CALLBACK ///< Defined callbacks calling convention (stdcall on Windows).

#endif

// Handle

typedef void * NHandle; ///< Pointer to unspecified data (same as void *).

/// Declares handle with specified name.
#define N_DECLARE_HANDLE(name) typedef struct name##_ { int unused; } * name;

// Rational

/// Represents an unsigned rational number.
typedef struct NURational_
{
	NUInt Numerator;   ///< Numerator of this NURational.
	NUInt Denominator; ///< Denominator of this NURational.
} NURational;

/// Represents a signed rational number.
typedef struct NRational_
{
	NInt Numerator;   ///< Numerator of this NRational.
	NInt Denominator; ///< Denominator of this NRational.
} NRational;

/// Specifies access to a file.
typedef enum NFileAccess_ // Flags
{
	nfaRead = 1,                      ///< Read access to the file.
	nfaWrite = 2,                     ///< Write access to the file.
	nfaReadWrite = nfaRead | nfaWrite ///< Read and write access to the file.
} NFileAccess;

/// Specifies byte order.
typedef enum NByteOrder_
{
	nboLittleEndian = 0,        ///< Little-endian byte order.
	nboBigEndian = 1,           ///< Big-endian byte order.
#ifdef N_BIG_ENDIAN
	nboSystem = nboBigEndian    ///< System-dependent byte order (either little-endian or big-endian).
#else
	nboSystem = nboLittleEndian ///< System-dependent byte order (either little-endian or big-endian).
#endif
} NByteOrder;

/// Checks if specified byte order is reverse to system byte order.
#define NIsReverseByteOrder(byteOrder) ((byteOrder) != nboSystem)

/// Represents a pair of indexes.
typedef struct NIndexPair_
{
	NInt Index1; ///< First index of this #NIndexPair.
	NInt Index2; ///< Second index of this #NIndexPair.
} NIndexPair;

#ifdef N_CPP
}
#endif

#endif // !N_TYPES_H_INCLUDED
