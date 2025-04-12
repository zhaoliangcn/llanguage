/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
//文件对象
*/
#ifndef _H_SCPFILEOBJECT
#define _H_SCPFILEOBJECT
#include "ScpObject.h"
#include "ScpTableObject.h"
#include "ScpStringObject.h"
#include "ScpTimeObject.h"
#include "ScpMemoryObject.h"



const static char * str_CN_file_attribute_readonly = "只读";
const static char * str_CN_file_attribute_hide = "隐藏";
const static char * str_CN_file_attribute_normal = "平常";
const static char * str_CN_file_attribute_archive = "存档";
const static char * str_CN_file_attribute_system = "系统";
const static char * str_CN_file_attribute_temporary = "临时";
const static char * str_CN_file_mode_readonly = "只读";
const static char * str_CN_file_mode_readwrite = "读写";
const static char * str_CN_file_mode_append = "追加";
const static char * str_CN_file_pos_begin = "起始";
const static char * str_CN_file_pos_end = "结尾";
const static char * str_CN_file_create_time = "创建时间";
const static char * str_CN_file_access_time = "访问时间";
const static char * str_CN_file_modify_time = "修改时间";
const static char * scpcommand_readall_CN = "读取全部";
const static char * scpcommand_readall_line_CN ="读取所有行";
const static char * scpcommand_erase_CN = "擦除";
const static char * scpcommand_select_open_CN = "选择打开";
const static char * scpcommand_select_save_CN = "选择保存";
const static char * scpcommand_append_CN = "追加";
const static char * inner_function_exist_CN = "存在";
const static char * str_CN_ObjDrive = "驱动器";
const static char * str_CN_ObjFilename = "文件名";
const static char * str_CN_ObjPath = "路径";
const static char * str_CN_ObjExtension = "扩展名";
const static char * str_CN_pos = "位置";
const static char * str_CN_curpos = "当前位置";

const static char * str_EN_file_attribute_readonly = "readonly";
const static char * str_EN_file_attribute_hide = "hide";
const static char * str_EN_file_attribute_normal = "norma";
const static char * str_EN_file_attribute_archive = "archive";
const static char * str_EN_file_attribute_system = "system";
const static char * str_EN_file_attribute_temporary = "temporary";
const static char * str_EN_file_mode_readonly = "readonly";
const static char * str_EN_file_mode_readwrite = "readwrite";
const static char * str_EN_file_mode_append = "append";
const static char * str_EN_file_pos_begin = "begin";
const static char * str_EN_file_pos_end = "end";
const static char * str_EN_file_create_time = "createtime";
const static char * str_EN_file_access_time = "accesstime";
const static char * str_EN_file_modify_time = "modifytime";
const static char * scpcommand_readall_EN = "readal";
const static char * scpcommand_readall_line_EN = "readallline";
const static char * scpcommand_erase_EN = "erase";
const static char * scpcommand_select_open_EN = "selectopen";
const static char * scpcommand_select_save_EN = "selectsave";
const static char * scpcommand_append_EN = "append";
const static char * inner_function_exist_EN = "exist";
const static char * str_EN_ObjDrive = "drive";
const static char * str_EN_ObjFilename = "filename";
const static char * str_EN_ObjPath = "path";
const static char * str_EN_ObjExtension = "extension";
const static char * str_EN_pos = "pos";
const static char * str_EN_curpos = "curpos";

class ScpFileObject:public ScpObject
{
public:
	ScpFileObject();
	~ScpFileObject();
	virtual ScpObject * Clone(std::string strObjName);
	virtual std::string ToString();
	virtual void Release() ;
	virtual void Show(CScriptEngine * engine);

	virtual bool IsInnerFunction(std::string & functionname);
	virtual ScpObject * CallInnerFunction(std::string & functionname,VTPARAMETERS * parameters,CScriptEngine * engine);

	static ScpTimeObject * GetLastAccessTime(std::string FileName1);
	static ScpTimeObject * GetCreateTime(std::string FileName1);
	static ScpTimeObject * GetLastModifyTime(std::string FileName1);
	static int Compare(std::string FileName1,std::string FileName2);
	static ScpTableObject * GetArrtibute(std::string FileName,ScpTableObject * tableobj);
	static BOOL ChangeAttribute(std::string FileName,ScpTableObject * table);
	static BOOL GetArrtibute(std::string FileName,VTPARAMETERS & attrlist);
	static BOOL ChangeAttribute(std::string FileName,VTPARAMETERS & attrlist);
	static std::string GetFileExt(std::string FileName);
	BOOL Open(std::string FileName,std::string mode);
	BOOL Create(std::string FileName, std::string mode);
	BOOL Save(std::string newFileName);
	static BOOL FileExist(std::string FileName);
	static BOOL IsDir(std::string FileName);
	BOOL read(__int64 pos,std::string &content,unsigned int length);
	BOOL write(__int64 pos,std::string &content,unsigned int length);
	BOOL read(__int64 pos,ULONG& size,void **Buffer);
	BOOL write(__int64 pos,ULONG size,void *Buffer);
	BOOL append(ULONG size, void *Buffer);
	BOOL append(std::string &content);
	BOOL write(std::wstring &content);
	BOOL write(std::string &content);
	BOOL read(std::wstring &content);
	BOOL read(std::string &content);
	BOOL readline(std::wstring &line);
	BOOL readline(std::string &line);
	BOOL readall_line(ScpTableObject * tableobj, CScriptEngine * engine);
	BOOL writeall_line(ScpTableObject * tableobj, CScriptEngine * engine);
	BOOL readall();
	BOOL readall(ScpMemoryObject * memobj);
	void Close();
	static BOOL Clear(std::string filename);
	BOOL Clear();
	__int64 seek(std::string pos);
	__int64 seek(__int64 pos);
	__int64 GetCurrentPos();
	void Print(CScriptEngine * engine);
	static BOOL Delete(std::string filename);
	static BOOL Erase(std::string filename);
	static BOOL Encrypt(std::string filename,std::string password, std::string algorithm);
	static BOOL Decrypt(std::string filename, std::string password, std::string algorithm);
	static BOOL Move(std::string src,std::string dst);
	static BOOL Copy(std::string src,std::string dst,BOOL force );
	static __int64 GetSize(std::string filename);
	static bool IsAbsolutePath(std::string& filepathname);
	static std::string GetAbsolutePath(const std::string& relativePath);
	std::string filename;
	unsigned char * content;
	__int64 currentpos;
	std::string openmode;
	__int64 filesize;

#ifdef _WIN32
	HANDLE file;
#else
    int file;
#endif
	


	static ScpObject * InnerFunction_Show(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_Get(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);

	static ScpObject * InnerFunction_delete(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_getsize(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_clear(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_open(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_create(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_save(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_print(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_move(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_shutdown(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_close(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_compare(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_seek(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_readall(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_readall_line(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_erase(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_read(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_write(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_copy(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_select_open(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_select_save(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_append(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_exist(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
};


ScpObject * __stdcall ScpFileObjectFactory(VTPARAMETERS * paramters, CScriptEngine * engine);


#endif //_H_SCPFILEOBJECT