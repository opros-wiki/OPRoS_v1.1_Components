/**
 * ADO database wrapper.
 *
 * Copyright (C) 2003-2008 Neurotechnology
 */

#include "Utils.h"
#include "VLDatabase.h"

using namespace neurotec::system;

namespace neurotec { namespace verilook {
namespace sample
{
	/**
	 * Class constructor, initializes member variables.
	 */
	CVLDatabase::CVLDatabase() :
		connected(false),
		opened(false),
		recordsetName(TEXT("Faces")),
		connection(NULL),
		recordset(NULL),
		recordBinding(NULL),
		allItems(NULL),
		allCount(0),
		allCapacity(0)
	{
	}

	/**
	 * Class destructor, closes database.
	 */
	CVLDatabase::~CVLDatabase()
	{
		Close();
	}

	/**
	 * Gets id.
	 */
	int CVLDatabase::ID(int index)
	{
		CheckIndex(index);
		return allItems[index]->id;
	}

	const BYTE *CVLDatabase::Features(int index)
	{
		CheckIndex(index);
		return (BYTE *)allItems[index]->features;
	}

	DWORD CVLDatabase::FeaturesSize(int index)
	{
		CheckIndex(index);
		return allItems[index]->featuresSize;
	}

	string CVLDatabase::FaceID(int index)
	{
		CheckIndex(index);
		return allItems[index]->faceId;
	}

	#define VLDBC(c, v) c <= v ? v :
	#define VLDBCapacity(c) (c <= 2 ? c : VLDBC(c, 4) VLDBC(c, 8) VLDBC(c, 16) VLDBC(c, 32) VLDBC(c, 64) VLDBC(c, 128) VLDBC(c, 256) VLDBC(c, 512) (c / 1024 + (c % 1024 ? 1 : 0) * 1024))
	#define VLDBAllCapacity(c) (c <= 2 ? c : VLDBC(c, 4) VLDBC(c, 8) VLDBC(c, 16) VLDBC(c, 32) VLDBC(c, 64) VLDBC(c, 128) VLDBC(c, 256) VLDBC(c, 512) \
		VLDBC(c, 1024) VLDBC(c, 2048) VLDBC(c, 4096) VLDBC(c, 8192) VLDBC(c, 16384) VLDBC(c, 32768) VLDBC(c, 65536) (c / 131072 + (c % 131072 ? 1 : 0) * 131072))

	#define VLDBICD(value) cp == value ? value :
	#define VLDBIncCapacityDelta(cp) (cp < 2 ? 1 : VLDBICD(2) VLDBICD(4) VLDBICD(8) VLDBICD(16) VLDBICD(32) VLDBICD(64) VLDBICD(128) VLDBICD(256) VLDBICD(512) 1024)
	#define VLDBIncAllCapacityDelta(cp) (cp < 2 ? 1 : VLDBICD(2) VLDBICD(4) VLDBICD(8) VLDBICD(16) VLDBICD(32) VLDBICD(64) VLDBICD(128) VLDBICD(256) VLDBICD(512) \
		VLDBICD(1024) VLDBICD(2048) VLDBICD(4096) VLDBICD(8192) VLDBICD(16384) VLDBICD(32768) VLDBICD(65536) 131072)

	void CVLDatabase::OpenRecordset(CursorTypeEnum cursorType, bool bind,
		LockTypeEnum lockType, CursorLocationEnum cursorLocation)
	{
		CloseRecordset();
		recordset->CursorLocation = cursorLocation;
		recordset->Open((_bstr_t)recordsetName.c_str(), _variant_t((IDispatch*)connection, true),
			cursorType, lockType, adCmdTableDirect);
		opened = true;
		if (bind)
		{
			HRESULT hr = S_OK;
			hr = recordset->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&recordBinding);
			if (FAILED(hr)) _com_raise_error(hr);

			hr = recordBinding->BindToRecordset(&record);
			if (FAILED(hr)) _com_raise_error(hr);
		}
	}

	void CVLDatabase::CloseRecordset()
	{
		if (recordBinding)
		{
			recordBinding->Release();
			recordBinding = NULL;
		}
		if (opened)
		{
			recordset->Close();
			opened = false;
		}
	}

	void CVLDatabase::Open(const string &provider, const string &dataSource, const string &userId, const string &password,
		IVLDBOpenCallback * callback)
	{
		Close();
		string connectionString = Format(TEXT("Provider=%s;Data Source=%s;"), (LPCTSTR)provider.c_str(), (LPCTSTR)dataSource.c_str());

		try
		{
			HRESULT hr = E_FAIL;
			hr = connection.CreateInstance(__uuidof(Connection));
			if (FAILED(hr)) _com_raise_error(hr);

			hr = recordset.CreateInstance(__uuidof(Recordset));

			if (callback) callback->OnOpenState(string("Opening connection"));

			connection->Open(connectionString.c_str(), userId.c_str(), password.c_str(), adConnectUnspecified);
			connected = true;
			
			if (callback) callback->OnOpenState(string("Retrieving record count"));

			_RecordsetPtr r = connection->Execute((TEXT("SELECT COUNT(ID) AS RC FROM ") + recordsetName).c_str(), NULL, adCmdText);
			int rc = r->Fields->Item["RC"]->Value.lVal;

			if (callback) callback->OnOpenState(string("Allocating memory"));

			allCapacity = VLDBAllCapacity(rc);
			allItems = (VLDBItem**)malloc(sizeof(VLDBItem*) * allCapacity);

			if (callback) callback->OnOpenState(string("Opening recordset in forward only mode"));

			OpenRecordset(adOpenForwardOnly);
			if (callback)
			{
				callback->OnOpenState(string("Loading data"));
				callback->OnOpenProgress(false, rc);
			}
			DWORD dt = GetTickCount();
			while (!recordset->EndOfFile)
			{
				if (callback && GetTickCount() - dt > 300)
				{
					if (!callback->OnOpenProgress(true, allCount)) break;
					dt = GetTickCount();
				}
				if (record.idStatus != adFldOK ||
					record.featuresStatus != adFldOK ||
					record.faceIdStatus != adFldOK)
					throw exception("Can not open database\nError reading data");
				AddItem(record.id, record.features, record.featuresLength,
					record.faceId, record.faceIdLength / sizeof(TCHAR));
				recordset->MoveNext();
			}

			if (callback) callback->OnOpenState(string("Opening recordset"));
			OpenRecordset();
			
			if (callback) callback->OnOpenState(string("Done"));

			this->provider = provider;
			this->dataSource = dataSource;
		}
		catch (_com_error &e)
		{
			if (callback) callback->OnOpenState(string("Error"));
			try
			{
				Close();
			}
			catch (exception &)
			{
				// do nothing
			}
			ComErrorToException(e, string("Can not open database"));
		}
	}

	void CVLDatabase::Close()
	{
		ClearItems();
		try
		{
			CloseRecordset();
			recordset = NULL;
			if (connected)
			{
				connection->Close();
				connected = false;
			}
			connection = NULL;
		}
		catch (_com_error &e)
		{
			ComErrorToException(e, string("Can not close database"));
		}
		provider = "";
		dataSource = "";
	}

	void CVLDatabase::ClearItems()
	{
		heap.Clear();
		free(allItems);
		allItems = NULL;
		allCount = 0;
		allCapacity = 0;
	}

	void CVLDatabase::Clear()
	{
		ClearItems();
		if (opened)
		{
			try
			{
				CloseRecordset();
				connection->Execute(Format(TEXT("DELETE FROM %s"), (LPCTSTR)recordsetName.c_str()).c_str(),
					NULL, adCmdText | adExecuteNoRecords);
				OpenRecordset();
			}
			catch(_com_error &e)
			{
				ComErrorToException(e, string("Can not clear database"));
			}
		}
	}

	void CVLDatabase::CopyString(LPTSTR dst, DWORD &dstLength, LPCTSTR src, DWORD srcLength, DWORD maxLength)
	{
		if (srcLength > maxLength) srcLength = maxLength;
		dstLength = srcLength;
		::memcpy(dst, src, srcLength*sizeof(TCHAR));
		dst[srcLength] = TEXT('\0');
	}

	void CVLDatabase::CopyFaceID(LPTSTR dst, DWORD &dstLength, LPCTSTR src, DWORD srcLength)
	{
		CopyString(dst, dstLength, src, srcLength, FACE_ID_LENGTH);
	}

	int CVLDatabase::AddItem(int id, const void *features, DWORD size,
		LPCTSTR faceId, DWORD faceIdLength)
	{
		int cp = 0;
		if (allCount == allCapacity)
		{
			allCapacity += VLDBIncAllCapacityDelta(allCapacity);
			allItems = (VLDBItem**)realloc(allItems, sizeof(VLDBItem*) * allCapacity);
		}
		VLDBItem *item = (VLDBItem*)heap.Alloc((DWORD)sizeof(VLDBItem));
		item->index = allCount;
		item->id = id;
		item->features = heap.Alloc((DWORD)size);
		item->featuresSize = size;
		::memcpy(item->features, features, size);
		CopyFaceID(item->faceId, item->faceIdLength, faceId, faceIdLength);
		allItems[allCount] = item;

		return allCount++;
	}

	void CVLDatabase::SetRecord(int index)
	{
		VLDBItem * item = allItems[index];
		::memcpy(record.features, item->features, item->featuresSize);
		record.featuresLength = item->featuresSize;
		record.featuresStatus = adFldOK;
		record.faceIdLength = item->faceIdLength * sizeof(TCHAR);
		::memcpy(record.faceId, item->faceId, item->faceIdLength*sizeof(TCHAR));
		record.faceId[item->faceIdLength] = TEXT('\0');
		record.faceIdStatus = adFldOK;
	}

	bool CVLDatabase::FindRecord(int id)
	{
		try
		{
			recordset->MoveFirst();
			recordset->Find(Format(TEXT("ID=%d"), id).c_str(), 0, adSearchForward);

			return !recordset->EndOfFile;
		}
		catch(_com_error &e)
		{
			ComErrorToException(e, string("Can not find record in the database"));

			return false;
		}
	}

	void CVLDatabase::UpdateRecord(int index)
	{
		SetRecord(index);
		FindRecord(allItems[index]->id);
		try
		{
			HRESULT hr = recordBinding->Update(&record);
			if (FAILED(hr)) _com_raise_error(hr);
		}
		catch (_com_error &e)
		{
			ComErrorToException(e, string("Can not update record in the database"));
		}
	}

	void CVLDatabase::CheckIndex(int index)
	{
		if (index < 0 || index >= allCount)
			throw exception("Database record index out of bounds");
	}

	void CVLDatabase::SetFeatures(int index, const void *features, DWORD size)
	{
		CheckIndex(index);
		heap.Free(allItems[index]->features);
		allItems[index]->features = heap.Alloc((DWORD)size);
		::memcpy(allItems[index]->features, features, size);
		allItems[index]->featuresSize = size;
		if (opened)
		{
			UpdateRecord(index);
		}
	}

	void CVLDatabase::SetFaceID(int index, const string &faceId)
	{
		CheckIndex(index);
		CopyFaceID(allItems[index]->faceId, allItems[index]->faceIdLength, faceId.c_str(), (DWORD) faceId.length());
		if (opened)
		{
			UpdateRecord(index);
		}
	}

	int CVLDatabase::Add(const void *features, DWORD size, LPCTSTR faceId)
	{
		const int index = AddItem(0, features, size, faceId, (DWORD) strlen(faceId));
		if (opened)
		{
			SetRecord(index);
			try
			{
				HRESULT hr = recordBinding->AddNew(&record);
				if (FAILED(hr)) _com_raise_error(hr);
				allItems[index]->id = record.id;
			}
			catch(_com_error &e)
			{
				ComErrorToException(e, string("Can not add record to the database"));
			}
		}
		return index;
	}

	void CVLDatabase::Remove(int index)
	{
		CheckIndex(index);
		VLDBItem *item = allItems[index];
		int id = item->id;
		int k;
		int cp;
		allCount--;
		::memmove(allItems + index, allItems + index + 1, sizeof(VLDBItem*) * (allCount - index));
		for (k = index; k < allCount; k++)
		{
			allItems[k]->index--;
		}

		cp = VLDBAllCapacity(allCount);
		if (allCapacity != cp)
		{
			allItems = (VLDBItem**)realloc(allItems, sizeof(VLDBItem*) * cp);
			allCapacity = cp;
		}
		heap.Free(item->features);
		heap.Free((BYTE*)item);
		if (opened)
		{
			if (FindRecord(id))
				try
				{
					recordset->Delete(adAffectCurrent);
				}
				catch (_com_error &e)
				{
					ComErrorToException(e, string("Can not delete record from the database"));
				}
		}
	}

	const string defaultDatabaseProvider(TEXT("Microsoft.Jet.OLEDB.4.0"));
	const string defaultDatabaseFileExtension(TEXT("mdb"));

	string GetVLDBDefaultFilter()
	{
		return TEXT("*.") + defaultDatabaseFileExtension;
	}

	string GetVLDBDefaultFilterName()
	{
		return TEXT("Microsoft Access Databases");
	}

	string GetVLDBDefaultFilterString(bool filterInName)
	{
		return MakeFileFilterString(GetVLDBDefaultFilterName(), GetVLDBDefaultFilter(), filterInName);
	}

}}}