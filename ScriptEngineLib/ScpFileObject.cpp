/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
*/
#include "ScpFileObject.h"

#include "ScpBigIntObject.h"
#include "ScpStringObject.h"
#include "ScpGlobalObject.h"
#include "ScriptEngine.h"
#include "ScpMemoryObject.h"
#include "ScpObjectNammes.h"
#include "commanddefine_uni.h"
#include "../Common/commonutil.hpp"
#ifdef _WIN32
#include "../Common/IsValidFileName.h"
#else
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#endif

#include <iostream>
#include <fstream>
#ifndef WIN32

int copyFile(const char* src, const char* des)
{
	int ret = -1;
	int fd, fd2;
	uint64_t buffersize = 4096;
	uint64_t size = 0;
	uint64_t size2 = 0;
	uint64_t filefullsize;
	filefullsize = GetFileLen(src);
	fd = open(src, O_RDONLY);
	fd2 = open(des, O_RDWR | O_CREAT, 00700);
	if (fd < 0 || fd2 < 0)
	{
		return ret;
	}
	ret = 0;
	unsigned char * buffer = (unsigned char *)malloc(buffersize);
	if (buffer)
	{
		while (filefullsize > 0)
		{
			size = read(fd, buffer, buffersize);
			size2 = write(fd2, buffer, size);
			if (size != size2)
			{
				ret = -1;
				break;
			}
			filefullsize -= size;
		}
		free(buffer);
	}

	close(fd);
	close(fd2);

/*
	//keep last accesstime and last modifytime;
	struct utimbuf orgutim;
	struct stat st;
	stat(src, &st);
	orgutim.actime = st.st_atime;
	orgutim.modtime = st.st_mtime;
	utime(des, &orgutim);
*/
	return ret;
}
#endif
BOOL GetLastWriteTime(HANDLE hFile, wchar_t * lpszString, DWORD dwSize);
BOOL comparefile(const wchar_t *FileName1, const wchar_t *FileName2);
BOOL GetLastWriteTime(HANDLE hFile, wchar_t * lpszString, DWORD dwSize)
{
#ifdef _WIN32
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return FALSE;
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	dwRet = _swprintf(lpszString,
		L"%02d/%02d/%d  %02d:%02d",
		stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		stLocal.wHour, stLocal.wMinute);

	if (dwRet > 0)
		return TRUE;
	else return FALSE;
#else 
	return TRUE;
#endif
}
int comparefile(const wchar_t *FileName1, const wchar_t *FileName2)
{
	int ret = -1;
#ifdef _WIN32
	HANDLE hFile1, hFile2;
	// TCHAR szBuf[MAX_PATH];  

	hFile1 = CreateFileW(FileName1, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile1 == INVALID_HANDLE_VALUE)
	{
		//printf("CreateFile failed with %d\n", GetLastError());
		return ret;
	}
	FILETIME ftCreate1, ftAccess1, ftWrite1;
	if (!GetFileTime(hFile1, &ftCreate1, &ftAccess1, &ftWrite1))
		return ret;

	hFile2 = CreateFileW(FileName2, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile2 == INVALID_HANDLE_VALUE)
	{
		//printf("CreateFile failed with %d\n", GetLastError());
		return ret;
	}
	FILETIME ftCreate2, ftAccess2, ftWrite2;
	if (!GetFileTime(hFile2, &ftCreate2, &ftAccess2, &ftWrite2))
		return ret;
	//比较文件最后修改时间    
	ret = CompareFileTime(&ftWrite1, &ftWrite2);
	if (ret == 1)
	{
		//printf("%sis newer than %s\n", FileName1,FileName2);
	}
	else if (ret == 0)
	{
		//printf("%s's last write time is same as %s's last write time\n", FileName1,FileName2);
	}
	else if (ret == -1)
	{
		//printf("%sis older than %s\n", FileName1,FileName2);
	}
	//比较文件大小 
	ULARGE_INTEGER liFile1, liFile2;
	liFile1.LowPart = GetFileSize(hFile1, &liFile1.HighPart);
	liFile2.LowPart = GetFileSize(hFile2, &liFile2.HighPart);
	if (liFile1.QuadPart < liFile2.QuadPart)
	{
		//printf("%sis small than %s\n", FileName1,FileName2);
	}
	else if (liFile1.QuadPart == liFile2.QuadPart)
	{
		ret = 0;
		//printf("%sis equal to %s\n", FileName1,FileName2);
	}
	else if (liFile1.QuadPart > liFile2.QuadPart)
	{
		//printf("%sis big than %s\n", FileName1,FileName2);
	}
	CloseHandle(hFile1);
	CloseHandle(hFile2);
#else
	struct stat st;
	struct stat st2;
	stat(STDSTRINGEXT::W2UTF(FileName1).c_str(), &st);
	stat(STDSTRINGEXT::W2UTF(FileName2).c_str(), &st2);
	if (st.st_mtime == st2.st_mtime)
	{
		ret = 0;
	}
	else
	{
		int fd, fd2;
		uint64_t buffersize = 4096;
		uint64_t size = 0;
		uint64_t size2 = 0;
		uint64_t filefullsize;
		uint64_t filefullsize2;
		filefullsize = GetFileLen(STDSTRINGEXT::W2UTF(FileName1).c_str());
		filefullsize2 = GetFileLen(STDSTRINGEXT::W2UTF(FileName2).c_str());
		if (filefullsize != filefullsize2)
		{
			return ret;
		}
		fd = open(STDSTRINGEXT::W2UTF(FileName1).c_str(), O_RDONLY);
		fd2 = open(STDSTRINGEXT::W2UTF(FileName2).c_str(), O_RDONLY);
		if (fd < 0 || fd2 < 0)
		{
			return ret;
		}
		ret = 0;
		unsigned char * buffer = (unsigned char *)malloc(buffersize);
		unsigned char * buffer2 = (unsigned char *)malloc(buffersize);
		if (buffer && buffer2)
		{
			while (filefullsize > 0)
			{
				size = read(fd, buffer, buffersize);
				size2 = read(fd2, buffer2, buffersize);
				if (size != size2)
				{
					ret = -1;
					break;
				}
				filefullsize -= size;
				if (memcmp(buffer, buffer2, size) != 0)
				{
					ret = -1;
					break;
				}
			}

			free(buffer2);
			free(buffer);
		}
		close(fd);
		close(fd2);
	}
#endif
	return ret;
}
#ifdef _WIN32
time_t systime_to_timet(const SYSTEMTIME& st)
{
	struct tm gm = { st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth - 1, st.wYear - 1900, st.wDayOfWeek, 0, 0 };
	return mktime(&gm);
}
SYSTEMTIME Time_tToSystemTime(time_t t)
{
	tm temptm = *localtime(&t);
	SYSTEMTIME st = { 1900 + temptm.tm_year,
								   1 + temptm.tm_mon,
								   temptm.tm_wday,
								   temptm.tm_mday,
								   temptm.tm_hour,
								   temptm.tm_min,
								   temptm.tm_sec,
								   0 };
	return st;
}
#endif
ScpFileObject::ScpFileObject()
{
	objecttype = ObjFile;
	filename = "";
	currentpos = 0;
	filesize = 0;
	content = NULL;
	file = NULL;

	BindObjectInnerFuction(scpcommand_en_show, InnerFunction_Show);
	BindObjectInnerFuction(scpcommand_cn_show, InnerFunction_Show);

	BindObjectInnerFuction(scpcommand_cn_get, InnerFunction_Get);
	BindObjectInnerFuction(scpcommand_en_get, InnerFunction_Get);

	BindObjectInnerFuction(scpcommand_cn_delete, InnerFunction_delete);
	BindObjectInnerFuction(scpcommand_en_delete, InnerFunction_delete);

	BindObjectInnerFuction(scpcommand_cn_getsize, InnerFunction_getsize);
	BindObjectInnerFuction(scpcommand_en_getsize, InnerFunction_getsize);

	BindObjectInnerFuction(scpcommand_cn_clear, InnerFunction_clear);
	BindObjectInnerFuction(scpcommand_en_clear, InnerFunction_clear);

	BindObjectInnerFuction(scpcommand_cn_open, InnerFunction_open);
	BindObjectInnerFuction(scpcommand_en_open, InnerFunction_open);

	BindObjectInnerFuction(scpcommand_cn_create, InnerFunction_create);
	BindObjectInnerFuction(scpcommand_en_create, InnerFunction_create);

	BindObjectInnerFuction(scpcommand_cn_save, InnerFunction_save);
	BindObjectInnerFuction(scpcommand_en_save, InnerFunction_save);

	BindObjectInnerFuction(scpcommand_cn_print, InnerFunction_print);
	BindObjectInnerFuction(scpcommand_en_print, InnerFunction_print);

	BindObjectInnerFuction(scpcommand_cn_move, InnerFunction_move);
	BindObjectInnerFuction(scpcommand_en_move, InnerFunction_move);

	BindObjectInnerFuction(scpcommand_cn_shutdown, InnerFunction_shutdown);
	BindObjectInnerFuction(scpcommand_en_shutdown, InnerFunction_shutdown);

	BindObjectInnerFuction(scpcommand_cn_close, InnerFunction_close);
	BindObjectInnerFuction(scpcommand_en_close, InnerFunction_close);

	BindObjectInnerFuction(scpcommand_cn_compare, InnerFunction_compare);
	BindObjectInnerFuction(scpcommand_en_compare, InnerFunction_compare);

	BindObjectInnerFuction(scpcommand_cn_seek, InnerFunction_seek);
	BindObjectInnerFuction(scpcommand_en_seek, InnerFunction_seek);

	BindObjectInnerFuction(scpcommand_readall_CN, InnerFunction_readall);
	BindObjectInnerFuction(scpcommand_readall_EN, InnerFunction_readall);

	BindObjectInnerFuction(scpcommand_readall_line_CN, InnerFunction_readall_line);
	BindObjectInnerFuction(scpcommand_readall_line_EN, InnerFunction_readall_line);

	BindObjectInnerFuction(scpcommand_erase_CN, InnerFunction_erase);
	BindObjectInnerFuction(scpcommand_erase_EN, InnerFunction_erase);

	BindObjectInnerFuction(scpcommand_cn_read, InnerFunction_read);
	BindObjectInnerFuction(scpcommand_en_read, InnerFunction_read);

	BindObjectInnerFuction(scpcommand_cn_write, InnerFunction_write);
	BindObjectInnerFuction(scpcommand_en_write, InnerFunction_write);

	BindObjectInnerFuction(scpcommand_cn_copy, InnerFunction_copy);
	BindObjectInnerFuction(scpcommand_en_copy, InnerFunction_copy);

	BindObjectInnerFuction(scpcommand_select_open_CN, InnerFunction_select_open);
	BindObjectInnerFuction(scpcommand_select_open_EN, InnerFunction_select_open);

	BindObjectInnerFuction(scpcommand_select_save_CN, InnerFunction_select_save);
	BindObjectInnerFuction(scpcommand_select_save_EN, InnerFunction_select_save);

	BindObjectInnerFuction(scpcommand_append_CN, InnerFunction_append);
	BindObjectInnerFuction(scpcommand_append_EN, InnerFunction_append);

	BindObjectInnerFuction(inner_function_exist_CN, InnerFunction_exist);
	BindObjectInnerFuction(inner_function_exist_EN, InnerFunction_exist);

}
ScpFileObject::~ScpFileObject()
{

#ifdef _WIN32
	if (file)
	{
		CloseHandle(file);
	}
#else
#endif
	
}
ScpObject * ScpFileObject::Clone(std::string strObjName)
{
	ScpFileObject * obj = new ScpFileObject;
	obj->filename = filename;
	return NULL;
}
std::string ScpFileObject::ToString()
{
	std::string temp;
	temp = filename;
	return temp;
}
void ScpFileObject::Release()
{
	delete this;
}
int  ScpFileObject::Compare(std::string FileName1, std::string FileName2)
{
	return comparefile(STDSTRINGEXT::UTF2W(FileName1).c_str(), STDSTRINGEXT::UTF2W(FileName2).c_str());
}
ScpTimeObject * ScpFileObject::GetLastAccessTime(std::string FileName1)
{

#ifdef _WIN32
	HANDLE hFile;
	hFile = CreateFileA(FileName1.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return NULL;
	FileTimeToSystemTime(&ftAccess, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	ScpTimeObject * temp = new ScpTimeObject;
	temp->value = systime_to_timet(stLocal);
	return temp;
#else
	ScpTimeObject * temp = NULL;
	struct stat buf;
	int fd = open(FileName1.c_str(), O_RDWR);
	if (fd != -1)
	{
		if (fstat(fd, &buf) != -1)
		{
			temp = new ScpTimeObject;
			temp->value = buf.st_atime;
		}
		close(fd);
	}
	return temp;
#endif
}
ScpTimeObject * ScpFileObject::GetCreateTime(std::string FileName1)
{

#ifdef _WIN32
	HANDLE hFile;
	hFile = CreateFileA(FileName1.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return NULL;
	FileTimeToSystemTime(&ftCreate, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	ScpTimeObject * temp = new ScpTimeObject;
	temp->value = systime_to_timet(stLocal);
	return temp;
#else 
	ScpTimeObject * temp = NULL;
	struct stat buf;
	int fd = open(FileName1.c_str(), O_RDWR);
	if (fd != -1)
	{
		if (fstat(fd, &buf) != -1)
		{
			temp = new ScpTimeObject;
			temp->value = buf.st_ctime;
		}
		close(fd);
	}
	return temp;
#endif
}
ScpTimeObject * ScpFileObject::GetLastModifyTime(std::string FileName1)
{

#ifdef _WIN32
	HANDLE hFile;
	hFile = CreateFileA(FileName1.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return NULL;
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	ScpTimeObject * temp = new ScpTimeObject;
	temp->value = systime_to_timet(stLocal);
	return temp;
#else 
	ScpTimeObject * temp = NULL;
	struct stat buf;
	int fd = open(FileName1.c_str(), O_RDWR);
	if (fd != -1)
	{
		if (fstat(fd, &buf) != -1)
		{
			temp = new ScpTimeObject;
			temp->value = buf.st_mtime;
		}
		close(fd);
	}
	return temp;
#endif
}
ScpTableObject * ScpFileObject::GetArrtibute(std::string FileName, ScpTableObject * tableobj)
{
	ScpTableObject * table = NULL;
	VTPARAMETERS attrlist;
	GetArrtibute(FileName, attrlist);
	if (attrlist.size() > 0)
	{
		if (tableobj == NULL)
			table = new ScpTableObject;
		else
			table = tableobj;
		for (VTPARAMETERS::iterator it = attrlist.begin(); it != attrlist.end(); it++)
		{
			ScpStringObject * tempstring = new ScpStringObject;
			tempstring->content = *it;
			table->AddElement(tempstring->content, tempstring);
		}
	}
	return table;
}
BOOL ScpFileObject::ChangeAttribute(std::string FileName, ScpTableObject * table)
{
	BOOL ret = FALSE;

	if (table->GetSize() > 0)
	{
		VTPARAMETERS attrlist;

		for (unsigned int i = 0; i < table->GetSize(); i++)
		{
			ScpStringObject * tempstring = (ScpStringObject *)table->GetElement(i);
			attrlist.push_back(tempstring->content);
		}
		ret = ChangeAttribute(FileName, attrlist);
	}

	return ret;
}
BOOL ScpFileObject::GetArrtibute(std::string FileName, VTPARAMETERS & attrlist)
{
	attrlist.clear();
#ifdef WIN32	
	DWORD attributes = GetFileAttributesA(FileName.c_str());
	if (ScpObjectNames::GetSingleInsatnce()->GetLanguage() == 0)
	{

		if ((attributes&FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		{
			attrlist.push_back(str_CN_file_attribute_readonly);
		}
		if ((attributes&FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)
		{
			attrlist.push_back(str_CN_file_attribute_hide);
		}
		if ((attributes&FILE_ATTRIBUTE_NORMAL) == FILE_ATTRIBUTE_NORMAL)
		{
			attrlist.push_back(str_CN_file_attribute_normal);
		}
		if ((attributes&FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE)
		{
			attrlist.push_back(str_CN_file_attribute_archive);
		}
		if ((attributes&FILE_ATTRIBUTE_SYSTEM) == FILE_ATTRIBUTE_SYSTEM)
		{
			attrlist.push_back(str_CN_file_attribute_system);
		}
		if ((attributes&FILE_ATTRIBUTE_TEMPORARY) == FILE_ATTRIBUTE_TEMPORARY)
		{
			attrlist.push_back(str_CN_file_attribute_temporary);
		}
	}
	else if (ScpObjectNames::GetSingleInsatnce()->GetLanguage() == 1)
	{
		if ((attributes&FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		{
			attrlist.push_back(str_EN_file_attribute_readonly);
		}
		if ((attributes&FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)
		{
			attrlist.push_back(str_EN_file_attribute_hide);
		}
		if ((attributes&FILE_ATTRIBUTE_NORMAL) == FILE_ATTRIBUTE_NORMAL)
		{
			attrlist.push_back(str_EN_file_attribute_normal);
		}
		if ((attributes&FILE_ATTRIBUTE_ARCHIVE) == FILE_ATTRIBUTE_ARCHIVE)
		{
			attrlist.push_back(str_EN_file_attribute_archive);
		}
		if ((attributes&FILE_ATTRIBUTE_SYSTEM) == FILE_ATTRIBUTE_SYSTEM)
		{
			attrlist.push_back(str_EN_file_attribute_system);
		}
		if ((attributes&FILE_ATTRIBUTE_TEMPORARY) == FILE_ATTRIBUTE_TEMPORARY)
		{
			attrlist.push_back(str_EN_file_attribute_temporary);
		}
	}
#else
	struct stat st;
	stat(FileName.c_str(), &st);
#endif
	return TRUE;
}
std::string ScpFileObject::GetFileExt(std::string FileName)
{
	char Ext[MAX_PATH];
#ifdef _WIN32
	_splitpath_s(FileName.c_str(), NULL, 0, NULL, 0, NULL, 0, Ext, MAX_PATH);
#else
	char utfExt[MAX_PATH];
	_splitpath(FileName.c_str(), NULL, NULL, NULL, utfExt);
    strcpy(Ext, utfExt);
#endif
	return Ext;
}
BOOL ScpFileObject::ChangeAttribute(std::string FileName, VTPARAMETERS & attrlist)
{
#ifdef WIN32

	DWORD attributes = FILE_ATTRIBUTE_NORMAL;
	attributes = GetFileAttributesA(FileName.c_str());
	for (VTPARAMETERS::iterator it = attrlist.begin(); it != attrlist.end(); it++)
	{
		if (*it == str_EN_file_attribute_readonly|| *it == str_CN_file_attribute_readonly)
		{
			attributes |= FILE_ATTRIBUTE_READONLY;
		}
		else if (*it == str_EN_file_attribute_hide|| *it == str_CN_file_attribute_hide)
		{
			attributes |= FILE_ATTRIBUTE_HIDDEN;
		}
		else if (*it == str_EN_file_attribute_normal|| *it == str_CN_file_attribute_normal)
		{
			attributes |= FILE_ATTRIBUTE_NORMAL;
		}
		else if (*it == str_EN_file_attribute_archive|| *it == str_CN_file_attribute_archive)
		{
			attributes |= FILE_ATTRIBUTE_ARCHIVE;
		}
		else if (*it == str_EN_file_attribute_system|| *it == str_CN_file_attribute_system)
		{
			attributes |= FILE_ATTRIBUTE_SYSTEM;
		}
		else if (*it == str_EN_file_attribute_temporary|| *it == str_CN_file_attribute_temporary)
		{
			attributes |= FILE_ATTRIBUTE_TEMPORARY;
		}
	}
	return SetFileAttributesA(FileName.c_str(), attributes);
#else
	return FALSE;
#endif 

}
BOOL ScpFileObject::Save(std::string newFileName)
{
	BOOL ret = FALSE;
	if (newFileName.empty())
	{
		if (this->content)
			ret = write(0,filesize, content);
	}
	else
	{
#ifdef WIN32
		ret = CopyFileA(this->filename.c_str(), newFileName.c_str(), FALSE);
		if (ret)
			this->filename = newFileName;
#else 
		if(copyFile(this->filename.c_str(), newFileName.c_str()) ==0)
		{
			this->filename = newFileName;
			ret = TRUE;
		}
#endif
	}
	return ret;
}
BOOL ScpFileObject::Open(std::string FileName, std::string mode)
{
#ifdef _WIN32
    if (!file)
    {
        filename = FileName;
        openmode = mode;
        filesize = GetSize(FileName);
        file = CreateFileA(FileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    if (file && file!=INVALID_HANDLE_VALUE)
        return TRUE;
    else
        return FALSE;
#else
	file = open(FileName.c_str(), O_RDWR);
	if (file < 0) {
		return FALSE;
	}
	seek(0);
	return TRUE;
#endif
    return FALSE;
}
BOOL ScpFileObject::Create(std::string FileName, std::string mode)
{
	BOOL bRet = FALSE;
#ifdef _WIN32
    if (!file)
    {
        filename = FileName;
        openmode = mode;
        filesize = GetSize(FileName);
        file = CreateFileA(FileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }
	if (file)
		bRet = TRUE;
	else
		bRet = FALSE;
#else
	file = open(FileName.c_str(), O_RDWR | O_CREAT, 00700);
	if (file < 0) {
		bRet = FALSE;
	}
	bRet = TRUE;
#endif
	return bRet;
    
}
BOOL ScpFileObject::IsDir(std::string FileName)
{
#ifdef _WIN32
    DWORD dwAttr = GetFileAttributesA(FileName.c_str());
    if(dwAttr!=-1)
        if ((dwAttr&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
        {
            return TRUE;
        }
#else
	struct stat st;
	stat(FileName.c_str(), &st);
	if(st.st_mode&S_IFDIR == S_IFDIR)
	{
		return TRUE;
	}
#endif
    return FALSE;
}
BOOL ScpFileObject::FileExist(std::string FileName)
{
	BOOL ret = FALSE;
#ifdef WIN32
	HANDLE hFile = CreateFileA(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		ret = TRUE;
		CloseHandle(hFile);
	}
#else
	if (access(FileName.c_str(), F_OK) == 0)
	{
		ret = TRUE;
	}
#endif
	return ret;
}
BOOL ScpFileObject::read(__int64 pos, std::string &content, unsigned int length)
{
	BOOL ret = FALSE;
	if (file)
	{
		char* buffer = (char*)malloc(length);
		memset(buffer, 0, length);
		if (buffer)
		{
			seek(pos);
#ifdef _WIN32
			if (file)
			{
				ret = ReadFile(file, buffer, length, NULL, NULL);
			}
#else
			if (file>0)
			{
				if(::read(file, buffer, length)>0)
				{
					ret = TRUE;
				}
			}
#endif
			if(ret)
				content = (char*)buffer;
			free(buffer);
		}
	}	
	return ret;
}
BOOL ScpFileObject::read(std::wstring &content)
{
	BOOL ret = FALSE;
	DWORD size = 0;
	if (file)
	{
		char* buffer = (char*)malloc(filesize + 1);
		memset(buffer, 0, filesize + 1);
		if (buffer)
		{
			seek(currentpos);
#ifdef _WIN32
			if (file)
			{
				ret = ReadFile(file, buffer, filesize, NULL, NULL);
			}
#else
			if (file > 0)
			{
				if (::read(file, buffer, filesize) > 0)
				{
					ret = TRUE;
				}
			}
#endif
			if(ret)
			{
				buffer[filesize] = '\0';
				content = STDSTRINGEXT::UTF2W((char*)buffer);
			}
			free(buffer);
			ret = TRUE;
		}
	}
	return ret;
}
BOOL ScpFileObject::readline(std::string &line)
{
	BOOL bRet = FALSE;
	static size_t posend = 0;
	static size_t linesize = 0;
	static char * pstart = NULL;
	static char * pend = NULL;
	if (!content)
	{
		readall();
		pstart =  (char *)content;
		pend =  strchr(pstart, '\n');
		linesize = pend - pstart;
		posend = pend -(char*) content;
	}	
	while (posend <=filesize)
	{
		char * l = (char*)malloc(linesize +2);
		if (l)
		{
			memset(l, 0, linesize + 2);
			memcpy(l, pstart, linesize +1);
			line = l;
			free(l);
			if (!pend)
			{
				bRet = TRUE;
				posend = filesize + 1;
				break;
			}
			pstart = pend + 1;
			pend = strchr(pstart, '\n');
			if (pend)
			{
				posend = pend - (char*)content;
				linesize = pend - pstart;
			}
			else
			{
				linesize = filesize - posend;
				posend = filesize;				
			}
			bRet = TRUE;
			break;
		}
		
	}
	return bRet;
}
BOOL ScpFileObject::readall_line(ScpTableObject * tableobj, CScriptEngine * engine)
{
	BOOL bRet = FALSE;
	std::string line;
	int i = 0;
	while (readline(line))
	{
		ScpStringObject * obj = new ScpStringObject;
		STDSTRINGEXT::trim(line);
		obj->content = line;
		std::string linename = STDSTRINGEXT::Format("line%d", i);
		engine->GetCurrentObjectSpace()->AddObject(linename, obj);
		tableobj->AddElement(linename, obj);
		i++;
	}
	return bRet;
}
BOOL ScpFileObject::writeall_line(ScpTableObject * tableobj, CScriptEngine * engine)
{
	BOOL bRet = FALSE;
	std::string line;
	std::string filenamea =filename;
	std::fstream infile;
	infile.open(filenamea.c_str(), std::ios::in | std::ios::out | std::ios::app);
	if (infile.is_open())
	{
		int length = 0;
		for (int i = 0; i < tableobj->GetSize(); i++)
		{
			ScpObject * obj = tableobj->GetElement(i);
			line = ((ScpStringObject*)obj)->content;
			length = line.length() * sizeof(wchar_t);
			seek(GetCurrentPos());
			infile.seekg((std::streamoff)currentpos, std::ios::beg);
			infile.write((const char *)line.c_str(), length);
		}
		seek(GetCurrentPos() + length);
		filesize = infile.tellg();
		infile.close();
		bRet = TRUE;
	}
	return bRet;
}
BOOL ScpFileObject::readline(std::wstring &line)
{
	BOOL bRet = FALSE;
	std::string aline;
	bRet = readline(aline);
	if (bRet)
	{
		line = STDSTRINGEXT::UTF2W(aline);
	}
	else
	{
		line = L"";
	}
	return bRet;
}
BOOL ScpFileObject::read(std::string &content)
{
	
	BOOL ret = FALSE;
	if (file)
	{
		std::string contenta;
		size_t size = 0;
		unsigned char* buffer = (unsigned char*)malloc(filesize + 1);
		if (buffer)
		{
			memset(buffer, 0, filesize);
#ifdef _WIN32
			if (file)
			{
				ret = ReadFile(file, buffer, filesize, NULL, NULL);
				size = filesize;
			}
#else
			if (file > 0)
			{
				size = ::read(file, buffer, filesize);
				if (size > 0)
				{
					ret = TRUE;
				}
			}
#endif
			buffer[size] = 0;
			contenta = (char*)buffer;
			content = contenta;
			free(buffer);
			ret = TRUE;
		}
	}	
	return ret;
}
BOOL ScpFileObject::write(std::string &content)
{
	BOOL ret = FALSE;
	if (file)
	{
		DWORD size = content.length();
		seek(currentpos);
		DWORD WriteCount;
#ifdef _WIN32
		if (file)
		{
			
			ret = WriteFile(file, content.c_str(), size, &WriteCount, NULL);
		}
#else
		if(file > 0)
		{
			WriteCount = ::write(file, content.c_str(), size);
			if(WriteCount>0)
			{
				ret = TRUE;
			}
		}
#endif
	}
	return ret;
}
BOOL ScpFileObject::write(std::wstring &content)
{	
	BOOL ret = FALSE;
	if (file)
	{
		std::string contenta = STDSTRINGEXT::W2UTF(content);
		DWORD size = contenta.length();
		write(contenta);
		ret = TRUE;
	}
	return ret;

}
BOOL ScpFileObject::Clear(std::string filename)
{
	BOOL ret = FALSE;
#ifdef WIN32
	HANDLE handle = CreateFileA(filename.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != handle)
	{
		ret = CloseHandle(handle);
	}
#else
	int fd = open(filename.c_str(), O_RDWR);
	if (fd != -1)
	{
		if (ftruncate(fd, 0) != -1)
		{
			ret = TRUE;
		}
		close(fd);
	}
#endif
	return ret;
}
BOOL ScpFileObject::Clear()
{
	return Clear(this->filename);
}
BOOL ScpFileObject::write(__int64 pos, std::string &content, unsigned int length)
{
	BOOL ret = FALSE;
	if (file)
	{
		std::string contenta =content;
		char* buffer = (char*)malloc(contenta.length());
		memset(buffer, 0, contenta.length());
		DWORD WriteCount;
		if (buffer)
		{
			seek(0);
#ifdef _WIN32
			if (file)
			{
				
				WriteFile(file, contenta.c_str(), contenta.length(), &WriteCount, NULL);
			}
#else
			if (file > 0)
			{
				WriteCount = ::write(file, contenta.c_str(), contenta.length());
				if (WriteCount > 0)
				{
					ret = TRUE;
				}
			}
#endif
			ret = TRUE;
			free(buffer);
		}
	}
	return ret;
}
BOOL ScpFileObject::read(__int64 pos, ULONG& size, void **Buffer)
{
	BOOL ret = FALSE;
	if (file)
	{

		if (Buffer && (*Buffer))
		{
			seek(pos);
#ifdef _WIN32
			if (file)
			{
				ReadFile(file, *Buffer, size, NULL, NULL);
			}
#else
			if (file > 0)
			{
				size = ::read(file, *Buffer, size);
			}
#endif
			if (size > 0)
				ret = TRUE;
		}
	}
	return ret;
}
BOOL ScpFileObject::write(__int64 pos, ULONG size, void *Buffer)
{
	BOOL ret = FALSE;
	if (file)
	{
		ULONG sizenew = 0;
		DWORD WriteCount;
		if (Buffer)
		{
			seek(pos);
#ifdef _WIN32
			if (file)
			{
				
				WriteFile(file, Buffer, size, &WriteCount, NULL);
				sizenew = GetFileSize(file, NULL);				
			}
#else
			if (file > 0)
			{
				WriteCount = ::write(file, Buffer, size);
				struct stat st;
				fstat(file, &st);
				sizenew = st.st_size;
				if (WriteCount > 0)
				{
					ret = TRUE;
				}
			}
#endif
			
			if (sizenew > filesize)
			{
				filesize = sizenew;
			}
			ret = TRUE;
			
		}
	}
	return ret;
}
BOOL ScpFileObject::append(ULONG size, void * Buffer)
{
	BOOL ret = FALSE;
	if (file)
	{
		if (Buffer)
		{
			ULONG writesize=0;
#ifdef _WIN32
			if (file)
			{
				SetFilePointer(file, 0, NULL, FILE_END);
				if (WriteFile(file, Buffer, size, &writesize, NULL))
				{
					filesize += writesize;
					ret = TRUE;
				}				
			}
#else
			if (file > 0)
			{
				lseek(file, 0, SEEK_END);
				writesize = ::write(file, Buffer, size);
				filesize += writesize;
				if (writesize > 0)
				{
					ret = TRUE;
				}
			}
#endif
			
		}
	}
	return ret;
}
BOOL ScpFileObject::append(std::string & content)
{
	std::string contenta = content;
	BOOL ret = FALSE;
	DWORD size = contenta.length();	
	return append(size,(void *)contenta.c_str());
}
BOOL ScpFileObject::readall()
{
	BOOL ret = FALSE;
	if (content)
	{
		free(content);
		content = NULL;
	}
#ifdef _WIN32
	if (file)
	{
		CloseHandle(file);
		file = NULL;
	}
	filesize = GetSize(this->filename);
	file = CreateFileA(this->filename.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file)
	{
		content = (unsigned char*)malloc(filesize+1);
		ULONG readcount = 0;
		if (content)
		{
			memset(content, 0, filesize + 1);

			if (file)
			{
				SetFilePointer(file, 0, NULL, FILE_BEGIN);
				ReadFile(file, content, filesize, &readcount, NULL);

			}

		}
		if (readcount > 0)
			ret = TRUE;
		CloseHandle(file);
		file = NULL;
	}	
#else
	if (file > 0)
	{
		close(file);
		file = NULL;
		filesize = GetSize(this->filename);
		file = open(this->filename.c_str(), O_RDWR);
		if (file > 0)
		{
			content = (unsigned char*)malloc(filesize + 1);
			ULONG readcount = 0;
			if (content)
			{
				memset(content, 0, filesize + 1);

				if (file)
				{
					lseek(file, 0, SEEK_SET);
					readcount = ::read(file, content, filesize);
				}
			}
			if (readcount > 0)
				ret = TRUE;
			close(file);
			file = NULL;
		}
	}
#endif	
	return ret;
}
BOOL ScpFileObject::readall(ScpMemoryObject * memobj)
{
	BOOL bRet = FALSE;
	if (memobj)
	{
		ULONG size = GetSize(this->filename);
		memobj->Acquare(GetSize(this->filename));
		bRet = this->read(0, size,&memobj->Address);
	}
	return bRet;
}
__int64  ScpFileObject::seek(std::string pos)
{

	if (pos == str_CN_file_pos_begin|| pos == str_EN_file_pos_begin)
	{
		return currentpos = 0;
	}
	else if (pos == str_CN_file_pos_end|| pos == str_EN_file_pos_end)
	{
		return currentpos = filesize;
	}
	else
	{

		return -1;
	}
}
__int64 ScpFileObject::seek(__int64 pos)
{
#ifdef _WIN32
	LARGE_INTEGER li;
	li.QuadPart = pos;
	SetFilePointerEx(file,li,NULL, FILE_BEGIN);

	li.QuadPart = 0;
    SetFilePointerEx(file, li, &li, FILE_CURRENT);
    currentpos = li.QuadPart;

#else
    errno=0;
	currentpos =lseek(file, pos, SEEK_SET);
#endif
	return currentpos ;
}
__int64 ScpFileObject::GetCurrentPos()
{
#ifdef _WIN32
    LARGE_INTEGER li;
    li.QuadPart = 0;
    SetFilePointerEx(file, li, &li, FILE_CURRENT);
    currentpos = li.QuadPart;
#else
    errno=0;
	currentpos = lseek(file, 0, SEEK_CUR);
#endif
    return currentpos;
}
void ScpFileObject::Print(CScriptEngine * engine)
{
	if(readall())
		engine->PrintError((char *)content);
}
void ScpFileObject::Show(CScriptEngine * engine)
{
	Print(engine);
}
bool ScpFileObject::IsInnerFunction(std::string & functionname)
{
	if (ObjectInnerFunctions.find(functionname) != ObjectInnerFunctions.end())
	{
		return true;
	}
	return false;	
}
ScpObject * ScpFileObject::CallInnerFunction(std::string & functionname, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (ObjectInnerFunctions.find(functionname) != ObjectInnerFunctions.end())
	{
		ObjectInnerFunction func = ObjectInnerFunctions[functionname];
		return func(this, parameters, engine);
	}
	return NULL;	
}
void ScpFileObject::Close()
{
	filename = "";
	currentpos = 0;
	openmode = "";
	filesize = 0;
	if (content)
		free(content);
	content = NULL;
#ifdef WIN32
	if (file)
		CloseHandle(file);
#else
    remove(filename.c_str()) ;
#endif
	file = NULL;
}
BOOL ScpFileObject::Delete(std::string filename)
{
#ifdef WIN32
	return DeleteFileA(filename.c_str());
#else
	return (remove(filename.c_str()) != -1);
#endif
}
BOOL ScpFileObject::Erase(std::string filename)
{
	BOOL ret = FALSE;
	__int64 size = GetSize(filename);
	if (size > 0)
	{
		unsigned char * mem = (unsigned char *)malloc(size);
		if (mem)
		{
			memset(mem, 0, size);

#ifdef WIN32
			HANDLE handle = CreateFileA(filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (INVALID_HANDLE_VALUE != handle)
			{
				DWORD WriteCount = 0;
				ret = WriteFile(handle, mem, size, &WriteCount, NULL);
				CloseHandle(handle);
			}
#else
			int handle = ::open(filename.c_str(), O_RDWR, 007000);
			if (handle != -1)
			{
                lseek(handle, 0, SEEK_SET);
				if (::write(handle, mem, size) > 0)
					ret = TRUE;
				close(handle);
			}
#endif
		}
		if (ret)
		{
			ret = Delete(filename);
		}
	}
	return ret;
}
BOOL ScpFileObject::Encrypt(std::string filename, std::string password, std::string algorithm)
{
	//ScpEncryptLib encryptlib;
	//return (0 == encryptlib.scpEncryptFile(filename, password, EncryptTypeAES));
	return FALSE;
}
BOOL ScpFileObject::Decrypt(std::string filename, std::string password, std::string algorithm)
{
	//ScpEncryptLib encryptlib;
	//return (0 == encryptlib.scpDecryptFile(filename, password, EncryptTypeAES));
	return FALSE;
}
BOOL ScpFileObject::Move(std::string src, std::string dst)
{
#ifdef WIN32
	return MoveFileA(src.c_str(), dst.c_str());
#else 
	return (rename(src.c_str(), dst.c_str()) != -1);
#endif
}
BOOL ScpFileObject::Copy(std::string src, std::string dst,BOOL force)
{
#ifdef WIN32
	if (IsDir(dst))
	{

		if (dst.at(dst.length() - 1) != '\\')
		{
			dst += "\\";
		}
		dst += MyPathStripPath(src.c_str());
	}
	return CopyFileA(src.c_str(), dst.c_str(), !force);
#else
	if (IsDir(dst))
	{

		if (dst.at(dst.length() - 1) != '/')
		{
			dst += "/";
		}
		char utfDrive[MAX_PATH];
		char utfPath[MAX_PATH];
		char utfName[MAX_PATH];
		char utfExt[MAX_PATH];
		_splitpath(src.c_str(), utfDrive, utfPath, utfName, utfExt);
		dst += utfName;
		dst += utfExt;
	}
	return copyFile(src.c_str(), dst.c_str());

#endif
}
__int64 ScpFileObject::GetSize(std::string FileName)
{
#ifdef WIN32

	LARGE_INTEGER size;
	size.QuadPart = 0;
	HANDLE hFile = CreateFileA(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hFile, &size);
		CloseHandle(hFile);
	}
	return size.QuadPart;
#else
	struct stat buf;
	if (stat(FileName.c_str(), &buf) != -1)
	{
		return buf.st_size;
	}
#endif

}

bool ScpFileObject::IsAbsolutePath(std::string& filepathname)
{
	if (filepathname.empty()) {
		return false;
	}

#ifdef _WIN32
	// Windows: 绝对路径以驱动器号（如 C:\）或反斜杠（如 \）开头
	if (filepathname.size() > 1 && filepathname[1] == ':') {
		return true;
	}
	if (filepathname[0] == '\\') {
		return true;
	}
#else
	// Unix: 绝对路径以斜杠（如 /）开头
	if (filepathname[0] == '/') {
		return true;
	}
#endif

	return false;
	
}

std::string ScpFileObject::GetAbsolutePath(const std::string& relativePath)
{
	char absolutePath[4096];

#ifdef _WIN32
	if (_fullpath(absolutePath, relativePath.c_str(), 4096) != nullptr) {
		return std::string(absolutePath);
	}
#else
	if (realpath(relativePath.c_str(), absolutePath) != nullptr) {
		return std::string(absolutePath);
	}
#endif

	// 如果转换失败，返回原始路径
	return relativePath;
}

ScpObject * __stdcall ScpFileObjectFactory(VTPARAMETERS * paramters, CScriptEngine * engine)
{
	if (paramters->size() >= 2)
	{
		std::string &strobj = paramters->at(0);
		std::string &userobjname = paramters->at(1);
		std::string content;
		if (paramters->size() == 3)
			content = paramters->at(2);
		StringStripQuote(content);
		ScpFileObject* fileobj = new ScpFileObject;
		if (fileobj)
		{
			ScpStringObject * obj = (ScpStringObject *)engine->GetCurrentObjectSpace()->FindObject(content);
			if (obj)
			{
				ScpObjectType type = obj->GetType();
				if (ObjString == type)
				{
					fileobj->filename = obj->content;
				}
			}
			else
			{
				fileobj->filename = content;
			}
			return fileobj;
		}
	}
	return NULL;
}

ScpObject * ScpFileObject::InnerFunction_Show(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	thisObject->Show(engine);
	return thisObject;
}

ScpObject * ScpFileObject::InnerFunction_Get(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string param0 = parameters->at(0);
		StringStripQuote(param0);
		ScpObject * objparam0 = engine->GetCurrentObjectSpace()->FindObject(param0);
		if (objparam0 && objparam0->GetType() == ObjString)
		{
			if (((ScpStringObject *)objparam0)->content == str_EN_ObjType ||
				((ScpStringObject *)objparam0)->content == str_CN_ObjType)
			{
				ScpStringObject * tname = (ScpStringObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjString);
				tname->content = ScpGlobalObject::GetInstance()->GetTypeName(thisObject->GetType());
				tname->istemp = true;
				return tname;
			}
		}
		if (param0 == str_EN_ObjType ||
			param0 == str_CN_ObjType)
		{
			ScpStringObject * tname = new ScpStringObject;
			tname->content = ScpGlobalObject::GetInstance()->GetTypeName(thisObject->GetType());
			tname->istemp = true;
			return tname;
		}
		else if (param0 == str_EN_pos ||
			param0 == str_CN_pos)
		{
			ScpIntObject * ipos = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
			if (ipos)
			{
				ipos->value = ((ScpFileObject*)thisObject)->currentpos;
				ipos->istemp = true;
			}
			return ipos;
		}
		else
		{
			char Drv[MAX_PATH];
			char Dir[MAX_PATH];
			char Name[MAX_PATH];
			char Ext[MAX_PATH];
#ifdef _WIN32
			_splitpath_s(((ScpFileObject*)thisObject)->filename.c_str(), Drv, MAX_PATH, Dir, MAX_PATH, Name, MAX_PATH, Ext, MAX_PATH);
#endif
			if (param0 == str_EN_ObjDrive ||
				param0 == str_CN_ObjDrive)
			{
				ScpStringObject * tname = new ScpStringObject;
				tname->content = Drv;
				tname->istemp = true;
				return tname;
			}
			else if (param0 == str_EN_ObjExtension || 
				param0 == str_CN_ObjExtension)
			{
				ScpStringObject * tname = new ScpStringObject;
				tname->content = Ext;
				tname->istemp = true;
				return tname;
			}
			else if (param0 == str_EN_ObjFilename || 
				param0 == str_CN_ObjFilename)
			{
				ScpStringObject * tname = new ScpStringObject;
				tname->content = Name;
				tname->istemp = true;
				return tname;
			}
			else if (param0 == str_EN_ObjPath || 
				param0 == str_CN_ObjPath)
			{
				ScpStringObject * tname = new ScpStringObject;
				tname->content = Dir;
				tname->istemp = true;
				return tname;

			}
			else if (param0 == str_CN_file_create_time ||
				param0 == str_EN_file_create_time)
			{
				ScpTimeObject *  timeobj = NULL;
				timeobj = ScpFileObject::GetCreateTime(((ScpFileObject*)thisObject)->filename.c_str());
				return timeobj;
			}
			else if (param0 == str_CN_file_access_time || 
				param0 == str_EN_file_access_time)
			{
				ScpTimeObject *  timeobj = NULL;
				timeobj = ScpFileObject::GetLastAccessTime(((ScpFileObject*)thisObject)->filename.c_str());
				return timeobj;
			}
			else if (param0 == str_CN_file_modify_time || 
				param0 == str_EN_file_modify_time)
			{
				ScpTimeObject *  timeobj = NULL;
				timeobj = ScpFileObject::GetLastModifyTime(((ScpFileObject*)thisObject)->filename.c_str());
				return timeobj;
			}
		}
	}

	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_delete(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	((ScpFileObject*)thisObject)->Delete(((ScpFileObject*)thisObject)->filename);
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_getsize(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	ScpBigIntObject * size = new ScpBigIntObject;
	if (size)
	{
		size->value = GetSize(((ScpFileObject*)thisObject)->filename);
		size->istemp = true;
	}
	return size;
}

ScpObject * ScpFileObject::InnerFunction_clear(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	Clear(((ScpFileObject*)thisObject)->filename);
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_open(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string wsfilename = parameters->at(0);
		StringStripQuote(wsfilename);
		ScpStringObject * obj = (ScpStringObject *)engine->GetCurrentObjectSpace()->FindObject(wsfilename);
		if (obj && obj->GetType() == ObjString)
		{
			wsfilename = obj->content;
			StringStripQuote(wsfilename);
		}		
		ScpIntObject * iexist = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
		if (iexist)
		{
			iexist->value = ((ScpFileObject*)thisObject)->Open(wsfilename, "");
			iexist->istemp = true;
		}
		return iexist;
	}
	else if (parameters->size() == 0)
	{
		ScpIntObject * iexist = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
		if (iexist)
		{
			iexist->value = ((ScpFileObject*)thisObject)->Open(((ScpFileObject*)thisObject)->filename, "");
			iexist->istemp = true;
		}
		return iexist;
	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_create(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string wsfilename = parameters->at(0);
		StringStripQuote(wsfilename);
		ScpStringObject * obj = (ScpStringObject *)engine->GetCurrentObjectSpace()->FindObject(wsfilename);
		if (obj && obj->GetType() == ObjString)
		{
			wsfilename = obj->content;
			StringStripQuote(wsfilename);
		}		
		ScpIntObject * iexist = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
		if (iexist)
		{
			iexist->value = ((ScpFileObject*)thisObject)->Create(wsfilename, "");
			iexist->istemp = true;
		}

		return iexist;
	}
	else if (parameters->size() == 0)
	{
		ScpIntObject * iexist = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
		if (iexist)
		{
			iexist->value = ((ScpFileObject*)thisObject)->Create(((ScpFileObject*)thisObject)->filename, "");
			iexist->istemp = true;
		}
		return iexist;
	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_save(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		((ScpFileObject*)thisObject)->Save("");
	}
	else if (parameters->size() == 1)
	{
		std::string wsobjectname1 = parameters->at(0);
		StringStripQuote(wsobjectname1);
		ScpObject * obj1 = engine->GetCurrentObjectSpace()->FindObject(wsobjectname1);
		if (obj1 && obj1->GetType() == ObjString)
		{
			wsobjectname1 = ((ScpStringObject *)obj1)->content;
		}
		else
		{
			if (IsStaticString(wsobjectname1))
			{
				StringStripQuote(wsobjectname1);

			}
		}
		#ifdef _WIN32
		if (IsValidFileName(wsobjectname1.c_str()))
		#else
		#endif
		{
			((ScpFileObject*)thisObject)->Save(wsobjectname1);

		}
	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_print(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	((ScpFileObject*)thisObject)->Print(engine);
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_move(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string wsdestname = parameters->at(1);
		StringStripQuote(wsdestname);
		ScpObject * obj1 = engine->GetCurrentObjectSpace()->FindObject(wsdestname);
		if (obj1 && obj1->GetType() == ObjString)
		{
			wsdestname = ((ScpStringObject *)obj1)->content;
			StringStripQuote(wsdestname);
		}
		((ScpFileObject*)thisObject)->Move(((ScpFileObject*)thisObject)->filename, wsdestname);
	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_shutdown(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	((ScpFileObject*)thisObject)->Close();
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_close(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	((ScpFileObject*)thisObject)->Close();
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_compare(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string str1 = parameters->at(0);
		StringStripQuote(str1);
		ScpIntObject * compare_ret = new ScpIntObject;
		compare_ret->istemp = true;
		compare_ret->value = 0;
		ScpFileObject *obj1 = (ScpFileObject *)engine->GetCurrentObjectSpace()->FindObject(str1);
		if (obj1)
		{
			if (ScpFileObject::Compare(((ScpFileObject*)thisObject)->filename, obj1->filename) == 0)
			{
				compare_ret->value = 1;
			}
		}
		else
		{
			if (ScpFileObject::FileExist(str1))
			{
				if (ScpFileObject::Compare(((ScpFileObject*)thisObject)->filename, str1) == 0)
				{
					compare_ret->value = 1;
				}
			}
			else
			{
				engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjNotExist);
			}
		}
		return compare_ret;
	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_seek(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string pos = parameters->at(0);
		StringStripQuote(pos);
		ScpObject* posobj = engine->GetCurrentObjectSpace()->FindObject(pos);
		if (posobj && posobj->GetType() == ObjInt)
		{
			((ScpFileObject*)thisObject)->seek(((ScpIntObject*)posobj)->value);
		}
		else if (posobj && posobj->GetType() == ObjString)
		{
			pos = ((ScpStringObject*)posobj)->content;
			if (pos == str_EN_curpos ||
				pos == str_CN_curpos)
			{

			}
			else if (pos == str_CN_file_pos_begin || pos == str_EN_file_pos_begin)
			{
				((ScpFileObject*)thisObject)->currentpos = 0;
			}
			else if (pos == str_CN_file_pos_end || pos == str_EN_file_pos_end)
			{
				((ScpFileObject*)thisObject)->currentpos = ((ScpFileObject*)thisObject)->filesize;
			}
			else
			{
				return nullptr;
			}
			ScpIntObject * ipos = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
			if (ipos)
			{
				ipos->value = ((ScpFileObject*)thisObject)->currentpos;
				ipos->istemp = true;
			}
			return ipos;
		}
		else
		{
			if (IsStaticNumber(pos))
				((ScpFileObject*)thisObject)->seek(StringToInt64(pos.c_str()));
			else if (pos == str_EN_curpos ||
				pos == str_CN_curpos)
			{
				ScpIntObject * ipos = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
				if (ipos)
				{
					ipos->value = ((ScpFileObject*)thisObject)->currentpos;
					ipos->istemp = true;
				}
				return ipos;
			}
			else
				((ScpFileObject*)thisObject)->seek(pos);
		}
	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_readall(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string memobjname = parameters->at(0);
		StringStripQuote(memobjname);
		ScpMemoryObject * obj = (ScpMemoryObject*)engine->GetCurrentObjectSpace()->FindObject(memobjname);
		if (obj)
		{
			((ScpFileObject*)thisObject)->readall(obj);
			return obj;
		}
	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_readall_line(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string tableobjname = parameters->at(0);
		StringStripQuote(tableobjname);
		ScpTableObject * obj = (ScpTableObject*)engine->GetCurrentObjectSpace()->FindObject(tableobjname);
		if (obj)
		{
			((ScpFileObject*)thisObject)->readall_line(obj, engine);
			return obj;
		}
	}
	return NULL;
}

ScpObject * ScpFileObject::InnerFunction_erase(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	((ScpFileObject*)thisObject)->Erase(((ScpFileObject*)thisObject)->filename);
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_read(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	if (parameters->size() == 1)
	{
		std::string objname = parameters->at(0);
		StringStripQuote(objname);
		ScpObject *obj1 = currentObjectSpace->FindObject(objname);
		if (obj1)
		{
			ScpObject  * readcount = currentObjectSpace->AcquireTempObject(ObjInt);
			if (obj1->GetType() == ObjString)
			{
				if (((ScpFileObject*)thisObject)->read(((ScpStringObject *)obj1)->content))
				{
					((ScpIntObject*)readcount)->value = ((ScpStringObject *)obj1)->content.size();
				}
			}				
			else if (obj1->GetType() == ObjMemory)
			{
				if (((ScpFileObject*)thisObject)->read(((ScpFileObject*)thisObject)->GetCurrentPos(), ((ScpMemoryObject*)obj1)->Size, &((ScpMemoryObject*)obj1)->Address))
				{
					((ScpIntObject*)readcount)->value = ((ScpMemoryObject *)obj1)->GetSize();
				}
			}
			else if (obj1->GetType() == ObjInt)
			{
				//void * pint = (void *)&((ScpIntObject*)obj1)->value;
				int value = 0;
				ULONG size = sizeof(value);
				void* pint = (void*)&value;
				if (((ScpFileObject*)thisObject)->read(((ScpFileObject*)thisObject)->GetCurrentPos(), size, &pint))
				{
					((ScpIntObject*)obj1)->value = value;
					((ScpIntObject*)readcount)->value = size;
				}
			}
			else if (obj1->GetType() == ObjBigInt)
			{
				void * pint = (void *)&((ScpBigIntObject*)obj1)->value;
				ULONG size = sizeof(__int64);
				if (((ScpFileObject*)thisObject)->read(((ScpFileObject*)thisObject)->GetCurrentPos(), size, &pint))
				{
					((ScpIntObject*)readcount)->value = size;
				}
			}
		}
	}
	else if (parameters->size() == 2)
	{
		ScpObject  * readcount = currentObjectSpace->AcquireTempObject(ObjInt);
		std::string &param0 = parameters->at(0);
		std::string &objname = parameters->at(1);
		StringStripQuote(param0);
		StringStripQuote(objname);
		ScpObject * objparam0 = engine->GetCurrentObjectSpace()->FindObject(param0);
		if (objparam0 && objparam0->GetType() == ObjString)
		{
			param0 = ((ScpStringObject *)objparam0)->content;
		}
		if (param0 == ScpObjectNames::GetSingleInsatnce()->strObjFileLine)
		{
			ScpObject *obj1 = currentObjectSpace->FindObject(objname);
			if (obj1)
			{
				if (obj1->GetType() == ObjString)
				{
					if (((ScpFileObject*)thisObject)->readline(((ScpStringObject *)obj1)->content))
					{
						((ScpIntObject*)readcount)->value = ((ScpStringObject *)obj1)->content.size();
					}
					return readcount;
				}
			}
		}
		if (objparam0 && objparam0->GetType() == ObjMemory)
		{
			ULONG size = 0;
			ScpObject *obj1 = currentObjectSpace->FindObject(objname);
			if (obj1)
			{
				if (obj1->GetType() == ObjInt)
				{
					size = ((ScpIntObject*)obj1)->value;
					if (size <= ((ScpMemoryObject*)objparam0)->Size)
					{
						if (((ScpFileObject*)thisObject)->read(((ScpFileObject*)thisObject)->GetCurrentPos(), size, &((ScpMemoryObject*)objparam0)->Address))
						{
							((ScpIntObject*)readcount)->value = size;
						}
						return readcount;
					}
				}
			}
		}
	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_write(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	if (parameters->size() == 1)
	{
		std::string objname = parameters->at(0);
		StringStripQuote(objname);
		ScpObject *obj1 = currentObjectSpace->FindObject(objname);
		if (obj1)
		{
			if (obj1->GetType() == ObjString)
				((ScpFileObject*)thisObject)->write(((ScpStringObject *)obj1)->content);
			else if (obj1->GetType() == ObjMemory)
				((ScpFileObject*)thisObject)->write(((ScpFileObject*)thisObject)->GetCurrentPos(), ((ScpMemoryObject*)obj1)->Size, ((ScpMemoryObject*)obj1)->Address);
			else if (obj1->GetType() == ObjInt)
			{
				void * pint = (void *)&((ScpIntObject*)obj1)->value;
				ULONG size = sizeof(int);
				((ScpFileObject*)thisObject)->write(((ScpFileObject*)thisObject)->GetCurrentPos(), size, pint);
			}
			else if (obj1->GetType() == ObjBigInt)
			{
				void* pint = (void*) & ((ScpBigIntObject*)obj1)->value;
				ULONG size = sizeof(__int64);
				((ScpFileObject*)thisObject)->write(((ScpFileObject*)thisObject)->GetCurrentPos(), size, pint);
			}
		}
		else
		{
			IsStaticString(objname);
			StringStripQuote(objname);
			((ScpFileObject*)thisObject)->write(objname);
		}

	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_copy(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	ScpIntObject* retval = (ScpIntObject*)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
	retval->value = 0;
	if (parameters->size() == 1)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		std::string destname = parameters->at(0);
		StringStripQuote(destname);
		ScpObject *obj1 = currentObjectSpace->FindObject(destname);
		if (obj1)
		{
			if (obj1->GetType() == ObjString)
			{
				destname = ((ScpStringObject *)obj1)->content;
			}
		}	
		if (Copy(((ScpFileObject*)thisObject)->filename, destname,FALSE))
		{
			retval->value = 1;
		}
	}
	else if (parameters->size() == 2)
	{
		ScpObjectSpace* currentObjectSpace = engine->GetCurrentObjectSpace();
		std::string destname = parameters->at(0);
		std::string param1= parameters->at(1);
		StringStripQuote(destname);
		StringStripQuote(param1);
		ScpObject* obj1 = currentObjectSpace->FindObject(destname);
		if (obj1)
		{
			if (obj1->GetType() == ObjString)
			{
				destname = ((ScpStringObject*)obj1)->content;
			}
		}
		ScpObject* obj2 = currentObjectSpace->FindObject(param1);
		if (obj2)
		{
			if (obj2->GetType() == ObjString)
			{
				param1 = ((ScpStringObject*)obj2)->content;
			}
		}
		BOOL bForce = FALSE;
#ifdef _WIN32
		if (stricmp(param1.c_str(), "force") == 0)
		{
			bForce = TRUE;
		}
#else 
		if (strcasecmp(param1.c_str(), "force") == 0)
		{
			bForce = TRUE;
		}
#endif
		if (Copy(((ScpFileObject*)thisObject)->filename, destname, bForce))
		{
			retval->value = 1;
		}
	}
	return retval;
}

ScpObject * ScpFileObject::InnerFunction_select_open(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	//去掉对c:\windows\system32\COMDLG32.DLL的依赖
//#ifdef WIN32
//		OPENFILENAME ofn;
//		wchar_t szFile[MAX_PATH];
//		ZeroMemory(&ofn, sizeof(ofn));
//		ofn.lStructSize = sizeof(ofn);
//		ofn.hwndOwner = NULL;
//		ofn.lpstrFile = szFile;
//		ofn.lpstrFile[0] = '\0';
//		ofn.nMaxFile = sizeof(szFile);
//		ofn.lpstrFilter = _T("all files (*.*)\0*.*\0\0");
//		ofn.lpstrDefExt = L".txt";
//		ofn.nFilterIndex = 1;
//		ofn.lpstrFileTitle = NULL;
//		ofn.nMaxFileTitle = 0;
//		ofn.lpstrInitialDir = NULL;
//		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//		ofn.lpstrTitle = _T("打开");
//		if (GetOpenFileName(&ofn))
//		{
//			ScpStringObject * tname = new ScpStringObject;
//			tname->content = szFile;
//			tname->istemp = true;
//			Open(szFile, L"");
//			return tname;
//		}
//#endif
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_select_save(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	//#ifdef WIN32
//		OPENFILENAME ofn;
//		wchar_t szFile[MAX_PATH];
//		ZeroMemory(&ofn, sizeof(ofn));
//		ofn.lStructSize = sizeof(ofn);
//		ofn.hwndOwner = NULL;
//		ofn.lpstrFile = szFile;
//		ofn.lpstrFile[0] = '\0';
//		ofn.nMaxFile = sizeof(szFile);
//		ofn.lpstrFilter = _T("all files (*.*)\0*.*\0\0");
//		ofn.lpstrDefExt = L"txt";
//		ofn.nFilterIndex = 1;
//		ofn.lpstrFileTitle = NULL;
//		ofn.nMaxFileTitle = 0;
//		ofn.lpstrInitialDir = NULL;
//		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//		ofn.lpstrTitle = _T("保存");
//		if (GetSaveFileName(&ofn))
//		{
//			ScpStringObject * tname = new ScpStringObject;
//			tname->content = szFile;
//			tname->istemp = true;
//			Save(szFile);
//			return tname;
//		}
//#endif
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_append(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string content = parameters->at(0);
		StringStripQuote(content);
		ScpObject * obj1 = engine->GetCurrentObjectSpace()->FindObject(content);
		if (obj1)
		{
			if (obj1->GetType() == ObjString)
			{
				((ScpFileObject*)thisObject)->append(((ScpStringObject*)obj1)->content);
			}
			else if (obj1->GetType() == ObjMemory)
			{
				ScpMemoryObject* mem = (ScpMemoryObject*)obj1;
				((ScpFileObject*)thisObject)->append(mem->Size, mem->Address);
			}
		}
		else
		{
			StringStripQuote(content);
			((ScpFileObject*)thisObject)->append(content);
		}
	}
	return nullptr;
}

ScpObject * ScpFileObject::InnerFunction_exist(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	ScpIntObject * iexist = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
	if (iexist)
	{
		iexist->value = FileExist(((ScpFileObject*)thisObject)->filename);
		iexist->istemp = true;
	}
	return iexist;
}
