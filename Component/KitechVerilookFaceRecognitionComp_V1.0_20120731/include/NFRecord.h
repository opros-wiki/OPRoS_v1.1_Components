/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Template 1.3                        *|
|*                                                                            *|
|* NFRecord.h                                                                 *|
|* Header file for NFRecord module                                            *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NF_RECORD_H_INCLUDED
#define NF_RECORD_H_INCLUDED

#include <NCore.h>

#ifdef N_CPP
extern "C"
{
#endif

/**
 * \brief Specifies the finger position
 * 
 * \remark This enumeration is implemented according to ANSI/NIST-ITL 1-2000 and ANSI INCITS
 *  378-2004 standards.
 */
typedef enum NFPosition_
{
	nfpUnknown = 0, ///< Unknown finger
	nfpRightThumb = 1, ///< Right thumb
	nfpRightIndex = 2, ///< Right index finger
	nfpRightMiddle = 3, ///< Right middle finger
	nfpRightRing = 4, ///< Right ring finger
	nfpRightLittle = 5, ///< Right little finger
	nfpLeftThumb = 6, ///< Left thumb finger
	nfpLeftIndex = 7, ///< Left index finger
	nfpLeftMiddle = 8, ///< Left middle finger
	nfpLeftRing = 9, ///< Left ring finger
	nfpLeftLittle = 10, ///< Left little finger
	nfpPlainRightThumb = 11, ///< Plain right thumb
	nfpPlainLeftThumb = 12, ///< Plain left thumb
	nfpPlainRightFourFingers = 13, ///< Plain right four fingers
	nfpPlainLeftFourFingers = 14 ///< Plain left four fingers
} NFPosition;

/**
* \brief Finger impression type
 * 
 * \remark This enumeration is implemented according to ANSI/NIST-ITL 1-2000 and ANSI INCITS
 * 378-2004 standards.
 */
typedef enum NFImpressionType_
{
	nfitLiveScanPlain = 0, ///< Live-scan plain
	nfitLiveScanRolled = 1, ///< Live-scan rolled
	nfitNonliveScanPlain = 2, ///< Nonlive-scan plain
	nfitNonliveScanRolled = 3, ///< Nonlive-scan rolled
	nfitLatentImpression = 4, ///< Latent impression
	nfitLatentTracing = 5, ///< Latent tracing
	nfitLatentPhoto = 6, ///< Latent photo
	nfitLatentLift = 7, ///< Latent lift
	nfitSwipe = 8, ///< Live-scanned fingerprint by sliding the finger across a "swipe" sensor.
	nfitLiveScanContactless = 9 ///< Live-scanned fingerprint using contactless device.
} NFImpressionType;

/**
 * \brief Pattern class of the fingerprint
 * 
 * \remark This enumeration is implemented according to ANSI/NIST-ITL 1-2000 standard.
 */
typedef enum NFPatternClass_
{
	nfpcUnknown = 0, ///< Unknown pattern class.
	nfpcPlainArch = 1, ///< Plain arch pattern class.
	nfpcTentedArch = 2, ///< Tented arch pattern class.
	nfpcRadialLoop = 3, ///< Radial loop pattern class.
	nfpcUlnarLoop = 4, ///< Ulnar loop pattern class.
	nfpcPlainWhorl = 5, ///< Plain whorl pattern class.
	nfpcCentralPocketLoop = 6, ///< Central pocket loop pattern class.
	nfpcDoubleLoop = 7, ///< Double loop pattern class.
	nfpcAccidentalWhorl = 8, ///< Accidental whorl pattern class.

	nfpcWhorl = 9, ///< Whorl pattern class.
	nfpcRightSlantLoop = 10, ///< Right slant loop pattern class.
	nfpcLeftSlantLoop = 11, ///< Left slant loop pattern class.
	nfpcScar = 12, ///< Scar. Pattern class is not available.
	nfpcAmputation = 15 ///< Pattern class is not available.
} NFPatternClass;

#define NFR_RESOLUTION 500  /**< \brief The resolution of minutiae, cores, deltas and double cores coordinates in a #NFRecord. */

#define NFR_MAX_DIMENSION 2047 /**< \brief The maximum value for x and y coordinates of a minutia, core, delta or double core in a #NFRecord. */
#define NFR_BLOCK_SIZE 16 /**< \brief For internal use. */
#define NFR_MAX_BLOCKED_ORIENTS_DIMENSION 128 /**< \brief For internal use. */

#define NFR_MAX_MINUTIA_COUNT 255  /**< \brief The maximum number of minutiae #NFRecord can contain. */
#define NFR_MAX_CORE_COUNT 15  /**< \brief The maximum number of cores #NFRecord can contain. */
#define NFR_MAX_DELTA_COUNT 15  /**< \brief The maximum number of deltas #NFRecord can contain. */
#define NFR_MAX_DOUBLE_CORE_COUNT 15  /**< \brief The maximum number of double cores #NFRecord can contain. */

/**
 * \brief Specifies the minutia format.
 * 
 * \remark This enumeration allows a bitwise combination of its member values.
 */
typedef enum NFMinutiaFormat_ // Flags
{
	nfmfNone = 0,
	nfmfHasQuality = 1, ///< Indicates that #NFMinutia. Quality field contains meaningful value and is preserved during unpacking/packing of #NFRecord.
	nfmfHasCurvature = 2, ///< Indicates that #NFMinutia. Curvature field contains meaningful value and is preserved during unpacking/packing of #NFRecord.
	nfmfHasG = 4 ///< Indicates that #NFMinutia. G field contains meaningful value and is preserved during unpacking/packing of #NFRecord.
} NFMinutiaFormat;

/**
 * \brief Minutia type
 */
typedef enum NFMinutiaType_
{
	nfmtUnknown = 0, ///< The type of the minutia is unknown.
	nfmtEnd = 1, ///< The minutia that is an end of a ridge.
	nfmtBifurcation = 2 ///< The minutia that is a bifurcation of a ridge.
} NFMinutiaType;

/**
 *\brief Represents a minutia in a #NFRecord.
 * 
 * NFMinutia::X X coordinate of this #NFMinutia. The x coordinate of the minutia is specified in pixels at #NFR_RESOLUTION and X *
 * [#NFRecord horizontal resolution] / #NFR_RESOLUTION can not be greater than #NFR_MAX_DIMENSION or #NFRecord width minus one.
 * 
 * NFMinutia::Y Field Y coordinate of this NFMinutia. The y coordinate of the minutia is specified in pixels at #NFR_RESOLUTION and Y *
 * [#NFRecord vertical resolution] / #NFR_RESOLUTION can not be greater than #NFR_MAX_DIMENSION or #NFRecord height minus one.
 * 
 * NFMinutia::Type #NFMinutiaType of this #NFMinutia.
 * 
 * NFMinutia::Angle The angle of the minutia is specified in 180/128 degrees units in counterclockwise order and
 * can not be greater than 256 minus one.
 * 
 * NFMinutia::Quality Quality of this #NFMinutia. The quality of the minutia must be in the range [0, 100]. The higher it is, the better the quality
 * of the minutia is. If quality of the minutia is unknown it must be set to zero.
 * 
 * NFMinutia::Curvature Ridge curvature near this #NFMinutia. If curvature of the minutia is unknown it must be set to 255.
 * 
 * NFMinutia::G G (ridge density) near this #NFMinutia. If G of the minutia is unknown it must be set to 255.
 */
typedef struct NFMinutia_
{
	NUShort X; ///< X coordinate of this #NFMinutia.
	NUShort Y; ///< Y coordinate of this #NFMinutia.
	NFMinutiaType Type; ///< #NFMinutiaType of this #NFMinutia.
	NByte Angle; ///< Angle of this #NFMinutia.
	NByte Quality; ///< Quality of this #NFMinutia.
	NByte Curvature; ///< Ridge curvature this #NFMinutia.
	NByte G; ///< G (ridge density) of this #NFMinutia.
} NFMinutia;

/**
 * \brief Type of ridge counts contained in a #NFRecord
 */
typedef enum NFRidgeCountsType_
{
	nfrctNone = 0, ///< The #NFRecord does not contain ridge counts. 
	nfrctFourNeighbors = 1, ///< The #NFRecord contains ridge counts to closest minutia in each of the four sectors of each minutia. First sector starts at minutia angle. 
	nfrctEightNeighbors = 2, ///< The #NFRecord contains ridge counts to closest minutia in each of the eight sectors of each minutia. First sector starts at minutia angle. 
	nfrctFourNeighborsWithIndexes = 5, ///< The #NFRecord contains ridge counts to four neighbors of each minutia. 
	nfrctEightNeighborsWithIndexes = 6, ///< The #NFRecord contains ridge counts to eight neighbors of each minutia. 
	nfrctUnspecified = 128 + 4 ///< For internal use.
} NFRidgeCountsType;

/** 
 * \brief Minutia neighbor in #NFRecord.
 * 
 */
typedef struct NFMinutiaNeighbor_
{
	NInt Index; ///< Index of neighbor minutia. 
	NByte RidgeCount; ///< Ridge count to neighbor minutia. 
} NFMinutiaNeighbor;

/**
 * \brief Represents a core in a #NFRecord.
 * 
 * NFCore::Angle 
 * The angle of the core is specified in 180/128 degrees units in counterclockwise order and can
 * not be less than zero or greater than 256 minus one.
 * The value of -1 can be specified if the angle of the core is unknown.
 * 
 * NFCore::X 
 * The x coordinate of the core is specified in pixels at #NFR_RESOLUTION and X *
 * [#NFRecord horizontal resolution] / #NFR_RESOLUTION can not be greater than
 * #NFR_MAX_DIMENSION or #NFRecord width minus one.
 * 
 * NFCore::Y
 * The y coordinate of the core is specified in pixels at #NFR_RESOLUTION and Y *
 * [#NFRecord vertical resolution] / #NFR_RESOLUTION can not be greater than
 * #NFR_MAX_DIMENSION or #NFRecord height minus one.
 */
typedef struct NFCore_
{
	NUShort X; ///< X coordinate of this #NFCore.
	NUShort Y; ///< Y coordinate of this #NFCore.
	NInt Angle; ///< Angle of this #NFCore.
} NFCore;

/** 
 * \brief Represents a delta in a #NFRecord.
 * 
 * NFDelta::Angle1 First angle of this #NFDelta.
 * 
 * NFDelta::Angle2 Second angle of this #NFDelta.
 * 
 * NFDelta::Angle3 Third angle of this #NFDelta.
 * 
 * NFDelta::X X coordinate of this #NFDelta The x coordinate of the delta is specified in pixels at #NFR_RESOLUTION and X *
 * [#NFRecord horizontal resolution] / #NFR_RESOLUTION can not be greater than
 * #NFR_MAX_DIMENSION or #NFRecord width minus one.
 * 
 * NFDelta::Y Y coordinate of this #NFDelta. The y coordinate of the delta is specified in pixels at #NFR_RESOLUTION and Y *
 * [#NFRecord vertical resolution] / #NFR_RESOLUTION can not be greater than
 * #NFR_MAX_DIMENSION or #NFRecord height minus one.
 * 
 * \remark The angles of the delta are specified in 180/128 degrees units in counterclockwise order and can
 * not be less than zero or greater than 256 minus one.
 * The value of -1 can be specified if specific angle of the delta is unknown.
 */
typedef struct NFDelta_
{
	NUShort X; ///< X coordinate of this #NFDelta.
	NUShort Y; ///< Y coordinate of this #NFDelta.
	NInt Angle1; ///< First angle of this #NFDelta.
	NInt Angle2; ///< Second angle of this #NFDelta.
	NInt Angle3; ///< Third angle of this #NFDelta.
} NFDelta;

/**
 * \brief Represents a double core in a #NFRecord.
 * 
 * NFDoubleCore::X The x coordinate of the double core is specified in pixels at #NFR_RESOLUTION and X *
 * [#NFRecord horizontal resolution] / #NFR_RESOLUTION can not be greater than #NFR_MAX_DIMENSION or #NFRecord width minus one.
 * 
 * NFDoubleCore::Y The y coordinate of the double core is specified in pixels at #NFR_RESOLUTION and Y *
 * [#NFRecord vertical resolution] / #NFR_RESOLUTION can not be greater than #NFR_MAX_DIMENSION or #NFRecord height minus one.
 * 
 */
typedef struct NFDoubleCore_
{
	NUShort X; ///< X coordinate of this #NFDoubleCore.
	NUShort Y; ///< Y coordinate of this #NFDoubleCore.
} NFDoubleCore;

/**
 \brief Handle to #NFRecord object
*/
N_DECLARE_HANDLE(HNFRecord)

/** 
 * \brief Retrieves the maximal size of a packed #NFRecord containing the specified number of minutiae, cores, deltas and double cores and the specified ridge counts type.
 * 
 * \param[in] minutiaFormat The minutia format. 
 * \param[in] minutiaCount The number of minutiae. 
 * \param[in] ridgeCountsType The type of ridge counts. 
 * \param[in] coreCount The number of cores. 
 * \param[in] deltaCount The number of deltas. 
 * \param[in] doubleCoreCount The number of double cores. 
 * \param[in] boWidth The width of blocked orientations. 
 * \param[in] boHeight The height of blocked orientations. 
 * \param[out] pSize Pointer to #NSizeType that receives maximal size of packed #NFRecord. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p minutiaFormat is invalid. - or - \p ridgeCountsType is invalid. .</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pSize is NULL. is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p minutiaCount is less than zero or greater than or equal to #NFR_MAX_MINUTIA_COUNT <br>
*                   \p coreCount is less than zero or greater than or equal to #NFR_MAX_CORE_COUNT <br>
 *                  \p deltaCount is less than zero or greater than or equal to #NFR_MAX_DELTA_COUNT <br>
 *                  \p doubleCoreCount is less than zero or greater than or equal to #NFR_MAX_DOUBLE_CORE_COUNT <br>
 *                  \p boWidth or \p boHeight is less than zero or greater than or equal to #NFR_MAX_BLOCKED_ORIENTS_DIMENSION.  </td></tr>
 *          </table>
 * \remark This is a low-level function and can be changed in future version of the library.<br>
 *         The function calculates current (2.0) version packed size of NFRecord.<br>
 *         \p boWidth and \p boHeight parameters are for compatibility only. If one of them or both is zero, blocked orientations are ignored. 
 * 
 * \sa NFRecord Module | NFMinutiaFormat | NFMinutia | NFRidgeCountsType | NFCore | NFDelta | NFDoubleCore | NFRecordSaveToMemory 
 */
NResult N_API NFRecordGetMaxSize(NFMinutiaFormat minutiaFormat, NInt minutiaCount, NFRidgeCountsType ridgeCountsType,
	NInt coreCount, NInt deltaCount, NInt doubleCoreCount, NInt boWidth, NInt boHeight, NSizeType * pSize);

/** 
 * \brief Checks if format of the packed #NFRecord is correct.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * 
 * If the function succeeds, the return value is N_OK.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer is NULL. </td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected. </td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format. </td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td> There was not enough memory. </td></tr>
 *          </table>
 * 
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats.
 * 
 * \sa NFRecord Module 
 */
NResult N_API NFRecordCheck(const void * buffer, NSizeType bufferSize);

/**
 * \brief Retrieves the width of the image the packed #NFRecord is made from.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to #NUShort that receives width of fingerprint image.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats. <br>
 *         Always returns 1 for version 1.0 format.
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetWidthMem(const void * buffer, NSizeType bufferSize, NUShort * pValue);

/**
 * \brief Retrieves the height of the image the packed #NFRecord is made from.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to #NUShort that receives height of fingerprint image.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats. <br>
 *         Always returns 1 for version 1.0 format.
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetHeightMem(const void * buffer, NSizeType bufferSize, NUShort * pValue);

/**
 * \brief Retrieves the horizontal resolution of the image the packed #NFRecord is made from.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to #NUShort that receives horizontal resolution of fingerprint image.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats. <br>
 *         Always returns 500 for version 1.0 format.
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetHorzResolutionMem(const void * buffer, NSizeType bufferSize, NUShort * pValue);

/**
 * \brief Retrieves the vertical resolution of the image the packed #NFRecord is made from.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to #NUShort that receives vertical resolution of fingerprint image.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats. <br>
 *         Always returns 500 for version 1.0 format.
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetVertResolutionMem(const void * buffer, NSizeType bufferSize, NUShort * pValue);

/**
 * \brief Retrieves the finger position of the packed #NFRecord 
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to #NFPosition that receives finger position.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats. <br>
 *         Always returns nfpUnknown for version 1.0 format.
 * \sa NFRecord Module | NFPosition
 */
NResult N_API NFRecordGetPositionMem(const void * buffer, NSizeType bufferSize, NFPosition * pValue);

/**
 * \brief Retrieves the impression type of the packed #NFRecord.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to #NFImpressionType that receives finger inpression.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats. <br>
 *         Always returns nfitLiveScanPlain  for version 1.0 format.
 * \sa NFRecord Module | NFImpressionType
 */
NResult N_API NFRecordGetImpressionTypeMem(const void * buffer, NSizeType bufferSize, NFImpressionType * pValue);

/**
 * \brief Retrieves the pattern class of the packed #NFRecord.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to #NFPatternClass that receives finger inpression.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats. <br>
 *         Always returns nfpcUnknown for version 1.0 format.
 * \sa NFRecord Module | NFPatternClass 
 */
NResult N_API NFRecordGetPatternClassMem(const void * buffer, NSizeType bufferSize, NFPatternClass * pValue);

/**
 * \brief Retrieves the quality of the packed #NFRecord.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to NByte that receives fingerprint quality.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats. <br>
 *         Always returns 0 for version 1.0 format.
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetQualityMem(const void * buffer, NSizeType bufferSize, NByte * pValue);

/**
 * \brief Retrieves the G of the packed #NFRecord.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to NByte that receives G. 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats. <br>
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetGMem(const void * buffer, NSizeType bufferSize, NByte * pValue);

/**
 * \brief Retrieves the Cbeff product type of the packed #NFRecord.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[out] pValue Pointer to NUShort that receives Cbeff product type.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>\p Data in memory buffer \p buffer points to is inconsistent with #NFRecord format </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFRecordSetCbeffProductType NFRecordGetCbeffProductType 
 */
NResult N_API NFRecordGetCbeffProductTypeMem(const void * buffer, NSizeType bufferSize, NUShort * pValue);

/** 
 * /brief For internal use
 */
typedef struct NFRecordInfo_
{
	NByte MajorVersion;
	NByte MinorVersion;
	NUShort Size;
	NByte HeaderSize;
} NFRecordInfo;

/** 
 * /brief For internal use
 */
void N_API NFRecordInfoDispose(NFRecordInfo * pInfo);

typedef enum NFMinutiaOrder_
{
	nfmoAscending = 0x01,
	nfmoDescending = 0x02,
	nfmoCartesianXY = 0x04,
	nfmoCartesianYX = 0x08,
	nfmoAngle = 0x0C,
	nfmoPolar = 0x10,
} NFMinutiaOrder;

/**
 * \brief Skip ridge counts while unpacking or packing
 * 
 * The flag indicating whether ridge counts should be skipped while unpacking or packing #NFRecord.
 */
#define NFR_SKIP_RIDGE_COUNTS    0x00010000 // Load/Save

/**
 * \brief Skip singular points while unpacking or packing
 * 
 * The flag indicating whether singular points (cores, deltas and double cores) should be skipped 
 * while unpacking or packing #NFRecord.
 * \remark Can be used when packing in 1.0 format #NFRecord
 */
#define NFR_SKIP_SINGULAR_POINTS 0x00020000 // Load/Save/SaveV1

/**
 * \brief Skip blocked orients while unpacking
 * 
 * The flag indicating whether blocked orientations should be skipped while unpacking #NFRecord.
 */
#define NFR_SKIP_BLOCKED_ORIENTS 0x00040000 // Load

/**
 * \brief Save blocked orients while packing
 * 
 * The flag indicating whether blocked orientations should be packed in #NFRecord.
 * \remark Can be used when packing in 1.0 format #NFRecord
 */
#define NFR_SAVE_BLOCKED_ORIENTS 0x00040000 // Save/SaveV1

/**
 * \brief Skip minutiae qualities while unpacking or packing
 * 
 * The flag indicating whether minutiae qualities should be skipped while unpacking or packing #NFRecord.
 */
#define NFR_SKIP_QUALITIES       0x00100000 // Load/Save

/**
 * \brief Skip minutiae curvatures while unpacking or packing
 * 
 * The flag indicating whether minutiae curvatures should be skipped while unpacking or packing #NFRecord.
 * \remark Can be used when packing in 1.0 format #NFRecord
 */
#define NFR_SKIP_CURVATURES      0x00200000 // Load/Save/SaveV1

/**
 * \brief Skip minutiae gs while unpacking or packing
 * 
 * The flag indicating whether minutiae gs should be skipped while unpacking or packing #NFRecord.
 * \remark Can be used when packing in 1.0 format #NFRecord
 */
#define NFR_SKIP_GS              0x00400000 // Load/Save/SaveV1

/**
 * \brief Creates an empty #NFRecord.
 * Parameters
 * \param[in] width Specifies width of fingerprint image. 
 * \param[in] height Specifies height of fingerprint image. 
 * \param[in] horzResolution Specifies horizontal resolution in pixels per inch of fingerprint image. 
 * \param[in] vertResolution Specifies vertical resolution in pixels per inch of fingerprint image. 
 * \param[in] flags Bitwise combination of zero or more flags that controls behavior of the function. This parameter is reserved, must be zero. 
 * \param[out] pHRecord Pointer to #HNFRecord that receives handle to created #NFRecord object.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p width or \p height is zero.- or - \p horzResolution or \p vertResolution is zero.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pHRecord is NULL.</td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>There was not enough memory. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFRecordFree 
 */
NResult N_API NFRecordCreate(NUShort width, NUShort height,
	NUShort horzResolution, NUShort vertResolution, NUInt flags, HNFRecord * pHRecord);

/**
 * \brief Unpacks a #NFRecord from the specified memory buffer.
 * 
 * \param[in] buffer Pointer to memory buffer that contains packed #NFRecord. 
 * \param[in] bufferSize Size of memory buffer that contains packed #NFRecord. 
 * \param[in] flags Bitwise combination of zero or more flags that controls behavior of the function. 
 * \param[out] pInfo For internal use. Must be #NULL. 
 * \param[out] pHRecord Pointer to #HNFRecord that receives handle to newly created #NFRecord object.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pHRecord or \p buffer is NULL </td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected </td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Data in memory buffer buffer points to is inconsistent with #NFRecord format. </td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>There was not enough memory. </td></tr>
 *          </table>
 * 
 * \remark The following flags are supported:
 * \remark #NFR_SKIP_BLOCKED_ORIENTS
 * \remark #NFR_SKIP_CURVATURES
 * \remark #NFR_SKIP_GS
 * \remark #NFR_SKIP_QUALITIES
 * \remark #NFR_SKIP_RIDGE_COUNTS
 * \remark #NFR_SKIP_SINGULAR_POINTS
 * \remark This function supports both #NFRecord version 1.0 and 2.0 formats.
 * \remark Created object must be deleted using #NFRecordFree function. 
 * 
 * \sa NFRecord Module | HNFRecord | NFRecordInfo | NFRecordFree | NFRecordSaveToMemory 
 */
NResult N_API NFRecordCreateFromMemory(const void * buffer, NSizeType bufferSize,
	NUInt flags, NFRecordInfo * pInfo, HNFRecord * pHRecord);

/**
 * \brief Deletes the #NFRecord. After the object is deleted the specified handle is no longer valid.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * 
 * \remark If hRecord is #NULL, does nothing.
 * 
 * \sa NFRecord Module | HNFRecord 
 */
void N_API NFRecordFree(HNFRecord hRecord);

/**
 * \brief Retrieves the number of minutiae in the #NFRecord.
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] pValue Pointer to #NInt that receives number of minutiae. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * \remark Minutia capacity is the number of minutiae that the #NFRecord can store.<br>
 *          Minutia count (see #NFRecordGetMinutiaCount function) is the number of minutiae that are actually in the #NFRecord. <br>
 *          Capacity is always greater than or equal to count. <br>
 *          If count exceeds capacity while adding minutiae the capacity is automatically increased by reallocating the internal array 
 *          before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordGetMinutiaCapacity | NFRecordSetMinutiaCapacity 
 */
NResult N_API NFRecordGetMinutiaCount(HNFRecord hRecord, NInt * pValue);

/** \brief Retrieves the minutia at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index of minutia to retrieve. 
 * \param[out] pValue Pointer to #NFMinutia that receives minutia. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p index is less than zero or greater than or equal to minutia count
                        obtained using #NFRecordGetMinutiaCount function.  </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFMinutia | NFRecordGetMinutiaCount | NFRecordSetMinutiae 
 */
NResult N_API NFRecordGetMinutia(HNFRecord hRecord, NInt index, NFMinutia * pValue);

/** \brief Sets a #NFMinutia at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index of minutia to set. 
 * \param[in] pValue Pointer to the #NFMinutia to set. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p index is less than zero or greater than or equal to minutia count
 *                        obtained using #NFRecordGetMinutiaCount function.  </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFMinutia | NFRecordGetMinutiaCount | NFRecordGetMinutia 
 */
NResult N_API NFRecordSetMinutia(HNFRecord hRecord, NInt index, const NFMinutia * pValue);

/** \brief Copies all minutiae of #NFRecord to the specified array.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] arMinutiae Pointer to array of #NFMinutia that receives minutiae.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p arMinutiae is #NULL</td></tr>
 *          </table>
 * 
 * \remark Array \p arMinutiae points to must be large enough to receive all #NFRecord minutiae.
 *  See #NFRecordGetMinutiaCount function. 
 * \sa NFRecord Module | HNFRecord | NFMinutia | NFRecordGetMinutiaCount
 */
NResult N_API NFRecordGetMinutiae(HNFRecord hRecord, NFMinutia * arMinutiae);

/** \brief Retrieves the number of minutiae that the #NFRecord can contain.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] pValue Pointer to #NInt that receives number of minutiae #NFRecord can contain.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * 
 * \remark Minutia capacity is the number of minutiae that the #NFRecord can store.<br>
 *          Minutia count (see #NFRecordGetMinutiaCount function) is the number of minutiae that are actually in the #NFRecord. <br>
 *          Capacity is always greater than or equal to count. <br>
 *          If count exceeds capacity while adding minutiae the capacity is automatically increased by reallocating the internal array 
 *          before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordSetMinutiaCapacity | NFRecordGetMinutiaCount 
 */
NResult N_API NFRecordGetMinutiaCapacity(HNFRecord hRecord, NInt * pValue);

/** \brief Sets the number of minutiae that the #NFRecord can contain.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] value New number of minutiae #NFRecord can contain.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p value is less than minutia count obtained using #NFRecordGetMinutiaCount function. </td></tr>
 *          </table>
 * 
 * \remark Minutia capacity is the number of minutiae that the #NFRecord can store.<br>
 *          Minutia count (see #NFRecordGetMinutiaCount function) is the number of minutiae that are actually in the #NFRecord. <br>
 *          Capacity is always greater than or equal to count. <br>
 *          If count exceeds capacity while adding minutiae the capacity is automatically increased by reallocating the internal array 
 *          before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordGetMinutiaCapacity | NFRecordGetMinutiaCount 
 */
NResult N_API NFRecordSetMinutiaCapacity(HNFRecord hRecord, NInt value);

/** \brief Adds a #NFMinutia to the end of #NFRecord minutiae.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] pValue Pointer to the #NFMinutia to add. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>Number of minutiae in #NFRecord (see #NFRecordGetMinutiaCount) is equal to #NFR_MAX_MINUTIA_COUNT. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFMinutia 
 */
NResult N_API NFRecordAddMinutia(HNFRecord hRecord, const NFMinutia * pValue);

/** \brief Inserts a #NFMinutia into the #NFRecord at the specified index.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index at which minutia is inserted. 
 * \param[in] pValue Pointer to the #NFMinutia to insert. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>index is less than zero or greater than minutia count obtained using #NFRecordGetMinutiaCount function. </td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>Number of minutia in #NFRecord (see #NFRecordGetMinutiaCount) is equal to #NFR_MAX_MINUTIA_COUNT. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFMinutia | NFRecordGetMinutiaCount
 */
NResult N_API NFRecordInsertMinutia(HNFRecord hRecord, NInt index, const NFMinutia * pValue);

/** \brief Removes minutia at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index at which minutia is removed. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>index is less than zero or greater than minutia count obtained using #NFRecordGetMinutiaCount function. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFRecordGetMinutiaCount 
 */
NResult N_API NFRecordRemoveMinutia(HNFRecord hRecord, NInt index);

/** \brief Removes all minutiae from the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord
 */
NResult N_API NFRecordClearMinutiae(HNFRecord hRecord);

/**
 * \brief Retrieves the number of minutia neighbors in the minutia at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] minutiaIndex The index of minutia. 
 * \param[out] pValue Pointer to #NInt that receives number of minutia neighbors. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td> \p index is less than zero or greater than or equal to minutia count obtained using #NFRecordGetMinutiaCount function. </td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord  
 */
NResult N_API NFRecordGetMinutiaNeighborCount(HNFRecord hRecord, NInt minutiaIndex, NInt * pValue);

/**
 * \brief Retrieves the minutia neighbor at the specified index of the minutia at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] minutiaIndex The index of minutia. 
 * \param[in] index Index of minutia neighbor to retrieve. 
 * \param[out] pValue Pointer to #NFMinutiaNeighbor that receives minutia neighbor.  
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td> \p minutiaIndex is less than zero or greater than or equal to minutia count obtained using #NFRecordGetMinutiaCount function 
 *  - or - \p index is less than zero or greater than or equal to minutia neighbor count obtained using #NFRecordGetMinutiaNeighborCount function. </td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord | NFMinutiaNeighbor | NFRecordGetMinutiaCount | NFRecordGetMinutiaNeighborCount | NFRecordSetMinutiaNeighbor 
 */
NResult N_API NFRecordGetMinutiaNeighbor(HNFRecord hRecord, NInt minutiaIndex, NInt index, NFMinutiaNeighbor * pValue);

/**
 * \brief Sets a #NFMinutiaNeighbor at the specified index of the minutia at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] minutiaIndex The index of minutia. 
 * \param[in] index Index of minutia neighbor to retrieve. 
 * \param[in] pValue Pointer to #NFMinutiaNeighbor to set
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td> Value \p pValue points to is invalid..</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td> \p minutiaIndex is less than zero or greater than or equal to minutia count obtained using #NFRecordGetMinutiaCount function
 *                      - or - \p index is less than zero or greater than or equal to minutia neighbor count obtained using #NFRecordGetMinutiaNeighborCount function.  </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFMinutiaNeighbor | NFRecordGetMinutiaCount | NFRecordGetMinutiaNeighborCount | NFRecordGetMinutiaNeighbor 
 */
NResult N_API NFRecordSetMinutiaNeighbor(HNFRecord hRecord, NInt minutiaIndex, NInt index, const NFMinutiaNeighbor * pValue);

/**
 * \brief Copies all minutia neighbors of the minutia at the specified index of the #NFRecord to the specified array.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] minutiaIndex The index of minutia. 
 * \param[out] arMinutiaNeighbors Pointer to array of #NFMinutiaNeighbor that receives minutia neighbors.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or  \p arMinutiaNeighbors is #NULL.</td></tr>
 *          </table>
 * \remark Array \p arMinutiaNeighbors points to must be large enough to receive all minutia neighbors. See #NFRecordGetMinutiaNeighborCount function.
 * \sa NFRecord Module | HNFRecord | NFMinutiaNeighbor | NFRecordGetMinutiaCount | NFRecordGetMinutiaNeighborCount 
 */
NResult N_API NFRecordGetMinutiaNeighbors(HNFRecord hRecord, NInt minutiaIndex, NFMinutiaNeighbor * arMinutiaNeighbors);

/**
 * \brief Retrieves the number of cores in the #NFRecord.
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] pValue Pointer to #NInt that receives number of cores. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * \remark Core capacity (see #NFRecordGetCoreCapacity and #NFRecordSetCoreCapacity functions) is the number of cores that the #NFRecord can store.
 *          Core count is the number of cores that are actually in the #NFRecord. Capacity is always greater than or equal to count. 
 *          If count exceeds capacity while adding cores, the capacity is automatically increased by reallocating the internal array 
 *          before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordGetCoreCapacity | NFRecordSetCoreCapacity 
 */
NResult N_API NFRecordGetCoreCount(HNFRecord hRecord, NInt * pValue);

/** \brief Retrieves the core at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index of core to retrieve. 
 * \param[out] pValue Pointer to #NFCore that receives core. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p index is less than zero or greater than or equal to core count
                        obtained using #NFRecordGetCoreCount function.  </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFCore | NFRecordGetCoreCount | NFRecordSetCore 
 */
NResult N_API NFRecordGetCore(HNFRecord hRecord, NInt index, NFCore * pValue);

/** \brief Sets a #NFCore at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index of core to set. 
 * \param[in] pValue Pointer to the #NFCore to set. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p index is less than zero or greater than or equal to core count
                        obtained using #NFRecordGetCoreCount function.  </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFCore | NFRecordGetCoreCount | NFRecordGetCore 
 */
NResult N_API NFRecordSetCore(HNFRecord hRecord, NInt index, const NFCore * pValue);

/** \brief Copies all cores of #NFRecord to the specified array.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] arCores Pointer to array of #NFCore that receives cores.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or arCores is #NULL</td></tr>
 *          </table>
 * \remark Array \p arCores points to must be large enough to receive all #NFCore cores.
 *  See #NFRecordGetCoreCount function. 
 * \sa NFRecord Module | HNFRecord | NFCore | NFRecordGetCoreCount
 */
NResult N_API NFRecordGetCores(HNFRecord hRecord, NFCore * arCores);

/** \brief Retrieves the number of cores that the #NFRecord can contain.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] pValue Pointer to #NInt that receives number of cores #NFRecord can contain.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * 
 * \remark Core capacity (see #NFRecordGetCoreCapacity and #NFRecordSetCoreCapacity functions) is the number of cores that the #NFRecord can store.
 *          Core count is the number of cores that are actually in the #NFRecord. Capacity is always greater than or equal to count. 
 *          If count exceeds capacity while adding core,  the capacity is automatically increased by reallocating the internal array 
 *          before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordSetCoreCapacity | NFRecordGetCoreCount 
 */
NResult N_API NFRecordGetCoreCapacity(HNFRecord hRecord, NInt * pValue);

/** \brief Sets the number of cores that the #NFRecord can contain.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] value New number of core #NFRecord can contain.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p value is less than core count obtained using #NFRecordGetCoreCount function. </td></tr>
 *          </table>
 * 
 * \remark Core capacity is the number of cores that the #NFRecord can store.
           Core count (see #NFRecordGetCoreCount function) is the number of cores that are actually in the #NFRecord. 
           Capacity is always greater than or equal to count. 
           If count exceeds capacity while adding cores, the capacity is automatically increased by reallocating the internal array 
           before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordGetCoreCapacity | NFRecordGetCoreCount 
 */
NResult N_API NFRecordSetCoreCapacity(HNFRecord hRecord, NInt value);

/** \brief Adds #NFCore to the end of #NFRecord cores.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] pValue Pointer to the #NFCore to add. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>Number of cores in #NFRecord (see #NFRecordGetCoreCount) is equal to #NFR_MAX_CORE_COUNT. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFCore 
 */
NResult N_API NFRecordAddCore(HNFRecord hRecord, const NFCore * pValue);

/** \brief Inserts a #NFCore into the #NFRecord at the specified index.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index at which core is inserted. 
 * \param[in] pValue Pointer to the #NFMinutia to insert. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>index is less than zero or greater than core count obtained using #NFRecordGetCoreCount function. </td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>Number of cores in #NFRecord (see #NFRecordGetCoreCount) is equal to #NFR_MAX_CORE_COUNT. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFCore | NFRecordGetCoreCount 
 */
NResult N_API NFRecordInsertCore(HNFRecord hRecord, NInt index, const NFCore * pValue);

/** \brief Removes core at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index at which core is removed. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>index is less than zero or greater than core count obtained using #NFRecordGetCoreCount function. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFRecordGetCoreCount 
 */
NResult N_API NFRecordRemoveCore(HNFRecord hRecord, NInt index);

/** \brief Removes all cores from the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord
 */
NResult N_API NFRecordClearCores(HNFRecord hRecord);

/**
 * \brief Retrieves the number of deltas in the #NFRecord.
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] pValue Pointer to #NInt that receives number of deltas. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * \remark Delta capacity (see #NFRecordGetDeltaCapacity and #NFRecordSetDeltaCapacity functions) is the number of cores that the #NFRecord can store.
 *          Delta count is the number of cores that are actually in the #NFRecord. Capacity is always greater than or equal to count. 
 *          If count exceeds capacity while adding deltas, the capacity is automatically increased by reallocating the internal array 
 *          before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordGetDeltaCapacity | NFRecordSetDeltaCapacity 
 */
NResult N_API NFRecordGetDeltaCount(HNFRecord hRecord, NInt * pValue);

/** \brief Retrieves the delta at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index of delta to retrieve. 
 * \param[out] pValue Pointer to #NFDelta that receives delta. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p index is less than zero or greater than or equal to delta count
                        obtained using #NFRecordGetDeltaCount function.  </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFDelta | NFRecordGetDeltaCount | NFRecordSetDelta 
 */
NResult N_API NFRecordGetDelta(HNFRecord hRecord, NInt index, NFDelta * pValue);

/** \brief Sets a #NFDelta at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index of delta to set. 
 * \param[in] pValue Pointer to the #NFDelta to set. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p index is less than zero or greater than or equal to delta count
                        obtained using #NFRecordGetDeltaCount function.  </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFDelta | NFRecordGetDeltaCount | NFRecordGetDelta 
 */
NResult N_API NFRecordSetDelta(HNFRecord hRecord, NInt index, const NFDelta * pValue);

/** \brief Copies all deltas of #NFRecord to the specified array.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] arDeltas Pointer to array of #NFDelta that receives deltas.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or arDeltas is #NULL</td></tr>
 *          </table>
 * \remark Array \p arDeltas points to must be large enough to receive all #NFDelta deltas.
 *  See #NFRecordGetDeltaCount function. 
 * \sa NFRecord Module | HNFRecord | NFDelta | NFRecordGetDeltaCount
 */
NResult N_API NFRecordGetDeltas(HNFRecord hRecord, NFDelta * arDeltas);

/** \brief Retrieves the number of deltas that the #NFRecord can contain.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] pValue Pointer to #NInt that receives number of deltas #NFRecord can contain.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * 
 * \remark Delta capacity (see #NFRecordGetDeltaCapacity and #NFRecordSetDeltaCapacity functions) is the number of delta that the #NFRecord can store.
 *          Delta count is the number of deltas that are actually in the #NFRecord. Capacity is always greater than or equal to count. 
 *          If count exceeds capacity while adding delta,  the capacity is automatically increased by reallocating the internal array 
 *          before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordSetDeltaCapacity | NFRecordGetDeltaCount 
 */
NResult N_API NFRecordGetDeltaCapacity(HNFRecord hRecord, NInt * pValue);

/** \brief Sets the number of deltas that the #NFRecord can contain.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] value New number of deltas #NFRecord can contain.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p value is less than delta count obtained using #NFRecordGetDeltaCount function. </td></tr>
 *          </table>
 * 
 * \remark Delta capacity is the number of deltas that the #NFRecord can store.
           Delta count (see #NFRecordGetDeltaCount function) is the number of deltas that are actually in the #NFRecord. 
           Capacity is always greater than or equal to count. 
           If count exceeds capacity while adding delta, the capacity is automatically increased by reallocating the internal array 
           before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordGetDeltaCapacity | NFRecordGetDeltaCount 
 */
NResult N_API NFRecordSetDeltaCapacity(HNFRecord hRecord, NInt value);

/** \brief Adds #NFDelta to the end of #NFRecord deltas.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] pValue Pointer to the #NFDelta to add. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>Number of deltas in #NFRecord (see #NFRecordGetDeltaCount) is equal to #NFR_MAX_DELTA_COUNT. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFDelta 
 */
NResult N_API NFRecordAddDelta(HNFRecord hRecord, const NFDelta * pValue);

/** \brief Inserts a #NFDelta into the #NFRecord at the specified index.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index at which delta is inserted. 
 * \param[in] pValue Pointer to the #NFMinutia to insert. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>index is less than zero or greater than delta count obtained using #NFRecordGetDeltaCount function. </td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>Number of deltas in #NFRecord (see #NFRecordGetDeltaCount) is equal to #NFR_MAX_DELTA_COUNT. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFDelta | NFRecordGetDeltaCount 
 */
NResult N_API NFRecordInsertDelta(HNFRecord hRecord, NInt index, const NFDelta * pValue);

/** \brief Removes dleta at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index at which delta is removed. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>index is less than zero or greater than deltas count obtained using #NFRecordGetDeltaCount function. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFRecordGetDeltaCount 
 */
NResult N_API NFRecordRemoveDelta(HNFRecord hRecord, NInt index);

/** \brief Removes all deltas from the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord
 */
NResult N_API NFRecordClearDeltas(HNFRecord hRecord);

/**
 * \brief Retrieves the number of double cores in the #NFRecord.
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] pValue Pointer to #NInt that receives number of double cores. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * \remark Double core capacity (see #NFRecordGetDeltaCapacity and #NFRecordSetDeltaCapacity functions) is the number of double cores that the #NFRecord can store.
 *          Double cores count is the number of cores that are actually in the #NFRecord. Capacity is always greater than or equal to count. 
 *          If count exceeds capacity while adding deltas, the capacity is automatically increased by reallocating the internal array 
 *          before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordGetDoubleCoreCapacity | NFRecordSetDoubleCoreCapacity 
 */
NResult N_API NFRecordGetDoubleCoreCount(HNFRecord hRecord, NInt * pValue);

/** \brief Retrieves the double core at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index of double core to retrieve. 
 * \param[out] pValue Pointer to #NFDoubleCore that receives double core. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p index is less than zero or greater than or equal to double cores count
                        obtained using #NFRecordGetDoubleCoreCount function.  </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFDoubleCore | NFRecordGetDoubleCoreCount | NFRecordSetDoubleCore 
 */
NResult N_API NFRecordGetDoubleCore(HNFRecord hRecord, NInt index, NFDoubleCore * pValue);

/** \brief Sets a #NFDoubleCore at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index of double core to set. 
 * \param[in] pValue Pointer to the #NFDoubleCore to set. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p index is less than zero or greater than or equal to double core count
                        obtained using #NFRecordGetDoubleCoreCount function.  </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFDoubleCore | NFRecordGetDoubleCoreCount | NFRecordGetDoubleCore 
 */
NResult N_API NFRecordSetDoubleCore(HNFRecord hRecord, NInt index, const NFDoubleCore * pValue);

/** \brief Copies all double cores of #NFRecord to the specified array.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] arDoubleCores Pointer to array of #NFDoubleCore that receives double cores.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or arDoubleCores is #NULL</td></tr>
 *          </table>
 * \remark Array \p arDoubleCores points to must be large enough to receive all #NFDoubleCore double cores.
 *  See #NFRecordGetDoubleCoreCount function. 
 * \sa NFRecord Module | HNFRecord | NFDoubleCore | NFRecordGetDoubleCoreCount
 */
NResult N_API NFRecordGetDoubleCores(HNFRecord hRecord, NFDoubleCore * arDoubleCores);

/** \brief Retrieves the number of double cores that the #NFRecord can contain.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] pValue Pointer to #NInt that receives number of double cores #NFRecord can contain.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * 
 * \remark Double Core capacity (see #NFRecordGetDoubleCoreCapacity and #NFRecordSetDoubleCoreCapacity functions) is the number of double cores that the #NFRecord can store.
 *          Double Core count is the number of double cores that are actually in the #NFRecord. Capacity is always greater than or equal to count. 
 *          If count exceeds capacity while adding double core,  the capacity is automatically increased by reallocating the internal array 
 *          before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordSetDoubleCoreCapacity | NFRecordGetDoubleCoreCount 
 */
NResult N_API NFRecordGetDoubleCoreCapacity(HNFRecord hRecord, NInt * pValue);

/** \brief Sets the number of double cores that the #NFRecord can contain.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] value New number of double cores #NFRecord can contain.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>\p value is less than double core count obtained using #NFRecordGetDoubleCoreCount function. </td></tr>
 *          </table>
 * 
 * \remark Double Core capacity is the number of double cores that the #NFRecord can store.
           Double Core count (see #NFRecordGetDoubleCoreCount function) is the number of double cores that are actually in the #NFRecord. 
           Capacity is always greater than or equal to count. 
           If count exceeds capacity while adding double cores, the capacity is automatically increased by reallocating the internal array 
           before copying the old elements and adding the new elements. 
 * \sa NFRecord Module | HNFRecord | NFRecordGetDoubleCoreCapacity | NFRecordGetDoubleCoreCount 
 */
NResult N_API NFRecordSetDoubleCoreCapacity(HNFRecord hRecord, NInt value);

/** \brief Adds #NFDoubleCore to the end of #NFRecord double cores.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] pValue Pointer to the #NFDoubleCore to add. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>Number of double cores in #NFRecord (see #NFRecordGetDoubleCoreCount) is equal to #NFR_MAX_DOUBLE_CORE_COUNT. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFDoubleCore 
 */
NResult N_API NFRecordAddDoubleCore(HNFRecord hRecord, const NFDoubleCore * pValue);

/** \brief Inserts a #NFDoubleCore into the #NFRecord at the specified index.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index at which double core is inserted. 
 * \param[in] pValue Pointer to the #NFDoubleCore to insert. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p pValue points to is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>index is less than zero or greater than double core count obtained using #NFRecordGetDoubleCoreCount function. </td></tr>
 *            <tr><td>#N_E_INVALID_OPERATION</td>
 *                <td>Number of double cores in #NFRecord (see #NFRecordGetDoubleCoreCount) is equal to #NFR_MAX_DOUBLE_CORE_COUNT. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFDoubleCore | NFRecordGetDoubleCoreCount 
 */
NResult N_API NFRecordInsertDoubleCore(HNFRecord hRecord, NInt index, const NFDoubleCore * pValue);

/** \brief Removes double core at the specified index of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[in] index Index at which double core is removed. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *            <tr><td>#N_E_ARGUMENT_OUT_OF_RANGE</td>
 *                <td>index is less than zero or greater than double core count obtained using #NFRecordGetDoubleCoreCount function. </td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFRecordGetDoubleCoreCount 
 */
NResult N_API NFRecordRemoveDoubleCore(HNFRecord hRecord, NInt index);

/** \brief Removes all double cores from the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord
 */
NResult N_API NFRecordClearDoubleCores(HNFRecord hRecord);

/**
 * \brief Creates a copy of the #NFRecord.
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] pHClonedRecord Pointer to a #HNFRecord that receives handle to newly created #NFRecord object.  
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or pHClonedRecord is #NULL. . </td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td> There was not enough memory. </td></tr>
 *          </table>
 * \remark Created object must be deleted using #NFRecordFree function. 
 * \sa #NFRecord Module | #HNFRecord | #NFRecordFree 
 */
NResult N_API NFRecordClone(HNFRecord hRecord, HNFRecord * pHClonedRecord);

/**
 * \brief Calculates packed size of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object 
 * \param[in] flags Bitwise combination of zero or more flags that controls behavior of the function. 
 * \param[out] pSize Pointer to #NSizeType that receives size of packed #NFRecord. 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pSize is #NULL</td></tr>
 *          </table>
 * \remark The function calculates current (2.0) version packed size of #NFRecord.
 * \remark For the list of flags that are supported see #NFRecordSaveToMemory function.
 * \sa #NFRecord Module | #HNFRecord | #NFRecordSaveToMemory 
 */
NResult N_API NFRecordGetSize(HNFRecord hRecord, NUInt flags, NSizeType * pSize);

/**
 * \brief Packs the #NFRecord into the specified memory buffer.
 * 
 * \param[in] hRecord Handle to the #NFRecord object. 
 * \param[out] buffer Pointer to memory buffer to store packed #NFRecord. Can be NULL. 
 * \param[in] bufferSize Size of memory buffer to store packed #NFRecord. 
 * \param[in] flags Bitwise combination of zero or more flags that controls behavior of the function. 
 * \param[out] pSize Pointer to #NSizeType that receives size of packed #NFRecord.  
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p buffer is not #NULL and \p bufferSize is less than size required to store packed #NFRecord.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pSize is #NULL or  \p buffer is #NULL and \p bufferSize is not zero. </td></tr>
 *          </table>
 * 
 * \remark The function packs #NFRecord in current (2.0) version format.<br>
 * If \p buffer is #NULL and \p bufferSize is zero the function only calculates the size of the buffer needed and has the same effect as #NFRecordGetSize function. <br>
 * If \p buffer is not #NULL, \p bufferSize must not be less than value calculated with #NFRecordGetSize function. <br>
 * Note that blocked orientations are not packed by default.<br>
 * The following flags are supported:<br>
 * #NFR_SAVE_BLOCKED_ORIENTS<br>
 * #NFR_SKIP_CURVATURES<br>
 * #NFR_SKIP_GS<br>
 * #NFR_SKIP_QUALITIES<br>
 * #NFR_SKIP_RIDGE_COUNTS<br>
 * #NFR_SKIP_SINGULAR_POINTS<br>
 * 
 * \sa #NFRecord Module | #HNFRecord | #NFRecordGetSize | #NFRecordCreateFromMemory 
 */
NResult N_API NFRecordSaveToMemory(HNFRecord hRecord, void * buffer, NSizeType bufferSize, NUInt flags, NSizeType * pSize);

NResult N_API NFRecordSortMinutiae(HNFRecord hRecord, NFMinutiaOrder order);
NResult N_API NFRecordTruncateMinutiaeByQuality(HNFRecord hRecord, NByte threshold, NInt maxCount);
NResult N_API NFRecordTruncateMinutiae(HNFRecord hRecord, NInt maxCount);

/**
 * \brief Retrieves the width of the image the #NFRecord is made from.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NUShort that receives width of fingerprint image.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetWidth(HNFRecord hRecord, NUShort * pValue);

/**
 * \brief Retrieves the height of the image the #NFRecord is made from.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NUShort that receives height of fingerprint image.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetHeight(HNFRecord hRecord, NUShort * pValue);

/**
 * \brief Retrieves the horizontal resolution of the image the #NFRecord is made from.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NUShort that receives horizontal resolution of fingerprint image.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetHorzResolution(HNFRecord hRecord, NUShort * pValue);

/**
 * \brief Retrieves the vertical resolution of the image the #NFRecord is made from.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NUShort that receives vertical resolution of fingerprint image.  
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetVertResolution(HNFRecord hRecord, NUShort * pValue);

/**
 * \brief Retrieves the finger position of the #NFRecord 
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NFPosition that receives finger position.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module | NFPosition
 */
NResult N_API NFRecordGetPosition(HNFRecord hRecord, NFPosition * pValue);

/**
 * \brief Retrieves the impression type of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NFImpressionType that receives finger inpression.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module | NFImpressionType
 */
NResult N_API NFRecordGetImpressionType(HNFRecord hRecord, NFImpressionType * pValue);

/**
 * \brief Retrieves the pattern class of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NFPatternClass that receives finger inpression.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module | NFPatternClass 
 */
NResult N_API NFRecordGetPatternClass(HNFRecord hRecord, NFPatternClass * pValue);

/**
 * \brief Retrieves the quality of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NByte that receives fingerprint quality.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetQuality(HNFRecord hRecord, NByte * pValue);

/**
 * \brief Retrieves the G of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NByte that receives G. 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module 
 */
NResult N_API NFRecordGetG(HNFRecord hRecord, NByte * pValue);

/**
 * \brief Retrieves the Cbeff product type of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NUShort that receives Cbeff product type.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * \sa NFRecord Module | HNFRecord | NFRecordSetCbeffProductType NFRecordGetCbeffProductType 
 */
NResult N_API NFRecordGetCbeffProductType(HNFRecord hRecord, NUShort * pValue);

/**
 * \brief Sets the impression type of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[in] value New impression type value.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
  *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord | NFImpressionType | NFRecordGetImpressionType 
 */
NResult N_API NFRecordSetImpressionType(HNFRecord hRecord, NFImpressionType value);

/**
 * \brief Sets the finger position of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[in] value New finger position value.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
  *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord | NFPosition | NFRecordGetPosition 
 */
NResult N_API NFRecordSetPosition(HNFRecord hRecord, NFPosition value);

/**
 * \brief Sets the pattern class of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[in] value New fingerprint pattern class value.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
  *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord | NFPatternClass | NFRecordGetPatternClass 
 */
NResult N_API NFRecordSetPatternClass(HNFRecord hRecord, NFPatternClass value);

/**
 * \brief Sets the quality of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[in] value New fingerprint quality value.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
  *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord | NFRecordGetQuality 
 */
NResult N_API NFRecordSetQuality(HNFRecord hRecord, NByte value);

/**
 * \brief Sets the G of the #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[in] value New G value.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
  *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * 
 * \remark G is a global fingerprint feature that reflects ridge density. It can have values from 0 to 255, so it occupies one byte. The bigger is value the bigger is ridge density.
 * 
 * \sa NFRecord Module | HNFRecord | NFRecordGetQuality 
 */
NResult N_API NFRecordSetG(HNFRecord hRecord, NByte value);

/**
 * \brief Sets the Cbeff product type.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[in] value Cbeff product type.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
  *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * 
 * \remark G is a global fingerprint feature that reflects ridge density. It can have values from 0 to 255, so it occupies one byte. The bigger is value the bigger is ridge density.
 * 
 * \sa NFRecord Module | HNFRecord | NFRecordGetQuality 
 */
NResult N_API NFRecordSetCbeffProductType(HNFRecord hRecord, NUShort value);

/**
 * \brief Retrieves the ridge counts type the #NFRecord contains
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[in] pValue Pointer to #NFRidgeCountsType that receives ridge counts type stored in #NFRecord. 
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord | NFRecordGetCbeffProductType NFRecordGetCbeffProductTypeMem 
 */
NResult N_API NFRecordGetRidgeCountsType(HNFRecord hRecord, NFRidgeCountsType * pValue);

/**
 * \brief Sets the ridge counts type the #NFRecord contains.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] value New ridge counts type value.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
  *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord | NFRidgeCountsType | NFRecordSetRidgeCountsType 
 */
NResult N_API NFRecordSetRidgeCountsType(HNFRecord hRecord, NFRidgeCountsType value);

/**
 * \brief Retrieves the format of the minutiae in #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[out] pValue Pointer to #NFMinutiaFormat that receives format of minutiae in the #NFRecord. 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pValue is #NULL</td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord | NFRidgeCountsType | NFRecordGetRidgeCountsType 
 */
NResult N_API NFRecordGetMinutiaFormat(HNFRecord hRecord, NFMinutiaFormat * pValue);

/**
 * \brief Sets the format of the minutiae in #NFRecord.
 * 
 * \param[in] hRecord Handle to the #NFRecord object.
 * \param[in] value New minutia format value.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
  *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL</td></tr>
 *          </table>
 * 
 * \sa NFRecord Module | HNFRecord | NFMinutiaFormat | NFRecordGetMinutiaFormat 
 */
NResult N_API NFRecordSetMinutiaFormat(HNFRecord hRecord, NFMinutiaFormat value);

#ifdef N_CPP
}
#endif

#endif // !NF_RECORD_H_INCLUDED
