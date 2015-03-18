/******************************************************************************\
|*                                                                            *|
|*                        Neurotechnology Template 1.3                        *|
|*                                                                            *|
|* NLRecord.h                                                                 *|
|* Header file for NLRecord module                                            *|
|*                                                                            *|
|* Copyright (C) 2005-2008 Neurotechnology                                    *|
|*                                                                            *|
\******************************************************************************/

#ifndef NL_RECORD_H_INCLUDED
#define NL_RECORD_H_INCLUDED

#include <NCore.h>

#ifdef N_CPP
extern "C"
{
#endif

/// Handle to face record object.
N_DECLARE_HANDLE(HNLRecord)

/**
 * \brief  Gets maximum possible face record size in bytes.
 * \param[in]  featuresSize  Size of features data in bytes.
 * \param[out] pSize         Returns number of bytes in largest possible
 *                           template of facial features (including header).
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p pSize is #NULL.</td></tr>
 *          </table>
 * \remarks  For internal use only.
 *
 * Gets maximum possible face record size in bytes.
 */
NResult N_API NLRecordGetMaxSize(NSizeType featuresSize, NSizeType *pSize);
/**
 * \brief  Checks whether serialized face record is valid.
 * \param[in]  buffer      Pointer to a buffer with serialized face record.
 * \param[in]  bufferSize  Size of buffer in bytes.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p buffer is #NULL.</td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>Not enough memory to create face record.</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>Face record is truncated.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Face record is not valid.</td></tr>
 *          </table>
 *
 * Checks whether serialized face record is valid.
 */
NResult N_API NLRecordCheck(const void *buffer, NSizeType bufferSize);

/**
 * \brief  Face record info.
 * \remarks  For internal use only.
 */
typedef struct NLRecordInfo_
{
	NByte MajorVersion; ///< Major version
	NByte MinorVersion; ///< Minor version
	NUInt Size;         ///< Size
	NByte HeaderSize;   ///< Header size
} NLRecordInfo;

/**
 * \brief  Disposes face record info.
 * \param[in]  pInfo  Pointer to a face record info structure.
 * \remarks  For internal use only.
 *
 * Disposes memory internally taken by face record info structure.
 */
void N_API NLRecordInfoDispose(NLRecordInfo *pInfo);

/**
 * \brief  Creates empty face record object.
 * \param[in]  flags     Flags that control face record object type and
 *                       purpose.
 * \param[out] pHRecord  Pointer to #HNLRecord that receives handle to the
 *                       created face record object.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p pHRecord is #NULL.</td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>Not enough memory to create face record.</td></tr>
 *          </table>
 * \remarks  For internal use only.
 * \sa HNLRecord | NLRecordCreateFromMemory | NLRecordLoadFromMemory
 *
 * Creates face record object and sets all parameters to default values.
 */
NResult N_API NLRecordCreate(NUInt flags, HNLRecord *pHRecord);
/**
 * \brief  Creates face record object from the memory buffer.
 * \param[in]  buffer      Pointer to the memory buffer containing serialized
 *                         face record.
 * \param[in]  bufferSize  Size of the memory buffer.
 * \param[in]  flags       Flags that control face record object type and
 *                         purpose.
 * \param[in]  pInfo       Pointer to #NLRecordInfo structer that is filled
 *                         from created face record object.
 * \param[out] pHRecord    Pointer to #HNLRecord that receives handle to the
 *                         created face record object.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pHRecord is #NULL.</td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>Not enough memory to create face record.</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>Face record is truncated.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Face record is not valid.</td></tr>
 *          </table>
 * \sa HNLRecord | NLRecordCreate | NLRecordLoadFromMemory
 *
 * Creates face record object from memory buffer.
 */
NResult N_API NLRecordCreateFromMemory(const void *buffer, NSizeType bufferSize,
	NUInt flags, NLRecordInfo *pInfo, HNLRecord *pHRecord);

/**
 * \brief  Deletes face record object.
 * \param[in]  hRecord  Handle to the face record.
 * \sa HNLRecord
 *
 * Deletes face record object. After the object is deleted the specified handle
 * is no longer valid.
 */
void N_API NLRecordFree(HNLRecord hRecord);

/**
 * \brief  Clones face record object.
 * \param[in]  hRecord         Handle to the face record.
 * \param[out] pHClonedRecord  Pointer to #HNLRecord that receives handle to
 *                             the cloned face record object.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord or \p pHClonedRecord is #NULL.</td></tr>
 *            <tr><td>#N_E_OUT_OF_MEMORY</td>
 *                <td>Not enough memory to create face record.</td></tr>
 *          </table>
 * \sa HNLRecord | NLRecordCreate
 *
 * Clones the created face record object to a new instance.
 */
NResult N_API NLRecordClone(HNLRecord hRecord, HNLRecord *pHClonedRecord);
/**
 * \brief  Gets size of serialized face record in bytes.
 * \param[in]  hRecord  Handle to the face record.
 * \param[in]  flags    Flags that control face record object type and purpose.
 * \param[out] pSize    Pointer to #NSizeType that receives value of serialized
 *                      face record size in bytes.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td><td>\p pSize is #NULL.</td></tr>
 *          </table>
 * \sa HNLRecord | NLRecordSaveToMemory
 *
 * Gets size of serialized face record in bytes.
 */
NResult N_API NLRecordGetSize(HNLRecord hRecord, NUInt flags, NSizeType *pSize);
/**
 * \brief  Serializes NLRecord into the specified memory buffer.
 * \param[in]  hRecord     Handle to the face record.
 * \param[in]  buffer      Pointer to the memory buffer.
 * \param[in]  bufferSize  Size of memory buffer in bytes.
 * \param[in]  flags       Flags that control face record saving.
 * \param[out] pSize       Pointer to #NSizeType that receives value of
 *                         serialized face record size in bytes.
 * \return  If the function succeeds the return value is #N_OK.
 * \return  If the function fails, the return value is one of the following
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord, \p buffer or \p pSize is #NULL.</td></tr>
 *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>Buffer is too small for the serialized face record.</td>
 *                </tr>
 *            <tr><td>#N_E_FAILED</td>
 *                <td>The memory cannot be written correctly.</td></tr>
 *          </table>
 * \sa HNLRecord
 *
 * Serializes face record to the memory buffer.
 */
NResult N_API NLRecordSaveToMemory(HNLRecord hRecord,
	void *buffer, NSizeType bufferSize, NUInt flags, NSizeType *pSize);

/**
 * \brief  Retrieves the quality of the packed #NLRecord.
 * \param[in]  buffer      Pointer to memory buffer that contains packed
 *                         #NLRecord.
 * \param[in]  bufferSize  Size of memory buffer that contains packed
 *                         #NLRecord.
 * \param[out] pValue      Pointer to #NByte that receives face quality.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL.</td></tr>
 *            <tr><td>#N_E_END_OF_STREAM</td>
 *                <td>\p bufferSize is less than expected.</td></tr>
 *            <tr><td>#N_E_FORMAT</td>
 *                <td>Data in memory buffer \p buffer points to is
 *                    inconsistent with #NLRecord format.</td></tr>
 *          </table>
 * \sa HNLRecord
 */
NResult N_API NLRecordGetQualityMem(const void *buffer, NSizeType bufferSize, NByte *pValue);

/**
 * \brief  Retrieves the quality of the #NLRecord.
 * 
 * \param[in]  hRecord  Handle to the #NLRecord object.
 * \param[out] pValue   Pointer to #NByte that receives face quality.
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p buffer or \p pValue is #NULL.</td></tr>
 *          </table>
 * \sa HNLRecord
 */
NResult N_API NLRecordGetQuality(HNLRecord hRecord, NByte *pValue);

/**
 * \brief  Sets the quality of the #NLRecord.
 * 
 * \param[in]  hRecord  Handle to the #NLRecord object.
 * \param[in]  value    New face quality value.
 * 
 * \return  If the function succeeds the return value is #N_OK. 
 * \return  If the function fails, the return value is one of the following 
 *          error codes:
 *          <table>
 *            <tr><th>Error Code</th><th>Condition</th></tr>
  *            <tr><td>#N_E_ARGUMENT</td>
 *                <td>\p value is invalid.</td></tr>
 *            <tr><td>#N_E_ARGUMENT_NULL</td>
 *                <td>\p hRecord is #NULL.</td></tr>
 *          </table>
 * 
 * \sa HNLRecord | NLRecordGetQuality 
 */
NResult N_API NLRecordSetQuality(HNLRecord hRecord, NByte value);

#ifdef N_CPP
}
#endif

#endif // !NL_RECORD_H_INCLUDED
