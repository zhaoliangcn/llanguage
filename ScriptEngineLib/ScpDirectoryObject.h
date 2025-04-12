/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
*/
#ifndef _H_SCPDIRECTORYOBJECT
#define _H_SCPDIRECTORYOBJECT

#include "ScpObject.h"
#include "ScpTableObject.h"

#ifdef _WIN32
// Windows平台特定头文件 | Windows platform specific headers
#else
#include <stdlib.h>
#include <unistd.h>	
#include <dirent.h>  // Linux/Unix平台目录操作头文件 | Linux/Unix platform directory operation header
#endif

// 目录对象类，继承自ScpObject | Directory object class, inherits from ScpObject
class ScpDirectoryObject : public ScpObject
{
public:
    ScpDirectoryObject(void);  // 构造函数 | Constructor
    ~ScpDirectoryObject(void); // 析构函数 | Destructor

    // 重写基类虚函数 | Override base class virtual functions
    virtual void Show(CScriptEngine * engine);  // 显示目录信息 | Display directory information
    virtual ScpObject * Clone(std::string strObjName);  // 克隆对象 | Clone object
    virtual std::string ToString();  // 转换为字符串 | Convert to string
    virtual void Release();  // 释放对象 | Release object

    virtual bool IsInnerFunction(std::string & functionname);  // 判断是否为内部函数 | Check if it's an inner function
    virtual ScpObject * CallInnerFunction(std::string & functionname,VTPARAMETERS * parameters,CScriptEngine * engine);  // 调用内部函数 | Call inner function

    // 目录操作函数 | Directory operation functions
    BOOL Open(std::string dir);  // 打开目录 | Open directory
    void ShowAllFiles(CScriptEngine * engine);  // 显示所有文件 | Display all files
    void ShowAllSubdir(CScriptEngine * engine);  // 显示所有子目录 | Display all subdirectories
    void ShowAll(CScriptEngine * engine);  // 显示所有内容 | Display all contents
    void EnumAll();  // 枚举所有内容 | Enumerate all contents
    BOOL FindAllFiles(ScpObjectSpace * currentObjectSpace,ScpTableObject * tableobj,std::string driectory,std::string matchrule, bool findfile);  // 查找所有文件 | Find all files
    ScpTableObject * EnumAllFiles(ScpObjectSpace * currentObjectSpace);  // 枚举所有文件 | Enumerate all files
    ScpTableObject * EnumAllFiles(ScpObjectSpace * currentObjectSpace,ScpObject * reobj);  // 枚举所有文件（带过滤） | Enumerate all files (with filter)
    ScpTableObject * EnumAllSubDir(ScpObjectSpace * currentObjectSpace);  // 枚举所有子目录 | Enumerate all subdirectories

    // 静态工具函数 | Static utility functions
    static BOOL Copy(std::string source,std::string dest);  // 复制目录 | Copy directory
    static BOOL Move(std::string source,std::string dest);  // 移动目录 | Move directory
    BOOL Delete();  // 删除当前目录 | Delete current directory
    static BOOL Delete(std::string dir);  // 删除指定目录 | Delete specified directory
    static BOOL PathOrFileExist(std::string dir);  // 检查路径或文件是否存在 | Check if path or file exists
    static BOOL IsDir(std::string dir);  // 判断是否为目录 | Check if it's a directory
    static BOOL Create(std::string dir);  // 创建目录 | Create directory

    // 数据成员 | Data members
    std::string directory;  // 当前目录路径 | Current directory path
    VTSTRINGS all;  // 所有条目 | All entries
    VTSTRINGS allfiles;  // 所有文件 | All files
    VTSTRINGS allsubdir;  // 所有子目录 | All subdirectories

    // 内部函数 | Inner functions
    static ScpObject * InnerFunction_Show(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);  // 显示内部函数 | Show inner function
    static ScpObject * InnerFunction_Get(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);  // 获取内部函数 | Get inner function

    static ScpObject * InnerFunction_delete(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);  // 删除内部函数 | Delete inner function
    static ScpObject * InnerFunction_enum(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);  // 枚举内部函数 | Enumerate inner function
    static ScpObject * InnerFunction_find(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);  // 查找内部函数 | Find inner function
    static ScpObject * InnerFunction_copy(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);  // 复制内部函数 | Copy inner function
    static ScpObject * InnerFunction_move(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);  // 移动内部函数 | Move inner function
    static ScpObject * InnerFunction_create(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);  // 创建内部函数 | Create inner function
    static ScpObject * InnerFunction_open(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);  // 打开内部函数 | Open inner function
};

// 目录对象工厂函数 | Directory object factory function
ScpObject * __stdcall ScpDirectoryObjectFactory(VTPARAMETERS * paramters, CScriptEngine * engine);

#endif //_H_SCPDIRECTORYOBJECT