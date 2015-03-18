/**
 * ADO database wrapper.
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */

#pragma once

#include <OleDB.h>
#include "msado15.tlh"
#include <icrsint.h>

using namespace ADODB;
using std::exception;


namespace neurotec { namespace verilook {
namespace sample
{
	#define SOHEAP_MAX_BLOCK_COUNT 1024

	class CSOHeap
	{
	private:
		DWORD blockSize;
		int blockCount;
		BYTE * blocks[SOHEAP_MAX_BLOCK_COUNT];
		DWORD sizes[SOHEAP_MAX_BLOCK_COUNT];

	public:
		CSOHeap(DWORD blockSize = 1048576)
			: blockCount(0)
		{
			this->blockSize = blockSize;
		}

		~CSOHeap()
		{
			Clear();
		}

		void Clear()
		{
			for(int i = 0; i < blockCount; i++)
				free(blocks[i]);
			blockCount = 0;
		}

		BYTE* Alloc(DWORD size)
		{
			if(!size) return NULL;
			DWORD memSize = size + sizeof(DWORD);
			BYTE *block = NULL;
			int j = -1;
			for(int i = 0; i < blockCount; i++)
			{
				if(blockSize - sizes[i] >= memSize)
				{
					block = blocks[i];
					j = i;
					break;
				}
			}
			if(!block)
			{
				if(blockCount == SOHEAP_MAX_BLOCK_COUNT) throw exception("SOHeap: max block count riched");
				block = new BYTE[blockSize];
				blocks[blockCount] = block;
				sizes[blockCount] = 0;
				j = blockCount;
				blockCount++;
			}
			block += sizes[j];
			*(DWORD*)block = size; block += sizeof(DWORD);
			sizes[j] += memSize;
			return block;
		}

		void Free(BYTE * /*object*/)
		{
			// Do nothing - memory will be allocated until heap is cleared
		}

	};

	#define FACE_ID_LENGTH 50

	class CFacesRs: public CADORecordBinding
	{

	BEGIN_ADO_BINDING(CFacesRs)
	  
		ADO_FIXED_LENGTH_ENTRY(1, adInteger, id, idStatus, FALSE)

		ADO_VARIABLE_LENGTH_ENTRY(2, adBinary, features, sizeof(features), featuresStatus, featuresLength, TRUE)

	#ifdef _UNICODE
		ADO_VARIABLE_LENGTH_ENTRY(3, adWChar, faceId, sizeof(faceId), faceIdStatus, faceIdLength, TRUE)
	#else
		ADO_VARIABLE_LENGTH_ENTRY(3, adChar, faceId, sizeof(faceId), faceIdStatus, faceIdLength, TRUE)
	#endif

	END_ADO_BINDING()

	public:
		int id;
		ULONG idStatus;
		BYTE features[131072];
		ULONG featuresStatus;
		DWORD featuresLength;
		TCHAR faceId[FACE_ID_LENGTH + 1];
		ULONG faceIdStatus;
		DWORD faceIdLength;
	};

	struct VLDBItem
	{
		int index;
		int id;
		BYTE *features;
		DWORD featuresSize;
		DWORD faceIdLength;
		TCHAR faceId[FACE_ID_LENGTH + 1];
	};

	interface IVLDBOpenCallback
	{
		virtual void OnOpenState(const string &state) = 0;
		virtual bool OnOpenProgress(bool started, int value) = 0;
	};

	struct VLDBFindData
	{
		int testG;
		int g;
		int index;
	};

	class CVLDatabase
	{
	private:
		CSOHeap heap;

		bool connected;
		bool opened;

		string recordsetName;

		_ConnectionPtr connection;

		_RecordsetPtr recordset;

		CFacesRs record;

		IADORecordBinding * recordBinding;

		VLDBItem  **allItems;
		int allCapacity;
		int allCount;

		string provider;
		string dataSource;

		void OpenRecordset(CursorTypeEnum cursor_type = adOpenKeyset, bool bind = true,
			ADODB::LockTypeEnum lockType = adLockOptimistic, CursorLocationEnum cursorLocation = adUseServer);
		void CloseRecordset();

		static void CopyString(LPTSTR dst, DWORD &dstLength, LPCTSTR src, DWORD srcLength, DWORD maxLength);
		static void CopyFaceID(LPTSTR dst, DWORD &dstLength, LPCTSTR src, DWORD srcLength);

		void ClearItems();
		int AddItem(int id, const void *features, DWORD size,
			const LPCTSTR faceId, DWORD faceIdLength);

		void CheckIndex(int index);

		void SetRecord(int index);
		bool FindRecord(int id);
		void UpdateRecord(int index);

	public:
		CVLDatabase();
		~CVLDatabase();

		void Open(const string &provider, const string &dataSource,
			const string &userId = "", const string &password = "",
			IVLDBOpenCallback * callback = NULL);
		void Close();
		bool Opened() { return opened; };

		const string & Provider() { return provider; }
		const string & DataSource() { return dataSource; }

		int get_Count() { return allCount; }
		__declspec(property(get = get_Count)) int Count;
		
		bool get_IsEmpty() { return allCount != 0; }
		__declspec(property(get = get_IsEmpty)) bool IsEmpty;

		int ID(int index);
		const BYTE * Features(int index);
		DWORD FeaturesSize(int index);
		string FaceID(int index);

		void SetFeatures(int index, const void *features, DWORD size);
		void SetFaceID(int index, const string &faceId);

		int Add(const void *features, DWORD size, LPCTSTR faceId);
		void Remove(int index);
		void Clear();

		void FindStart(const void *features, VLDBFindData &fd);
		int FindNext(VLDBFindData &fd);
		void FindEnd(VLDBFindData &fd);
	};

	extern const string defaultDatabaseProvider;
	extern const string defaultDatabaseFileExtension;
	string GetVLDBDefaultFilter();
	string GetVLDBDefaultFilterName();
	string GetVLDBDefaultFilterString(bool filterInName = true);
}}}