#ifndef _H_SCRIPTENGINE
#define _H_SCRIPTENGINE
#ifdef WIN32
#include <WinSock2.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#endif
#include <string>
#include <iostream>

#define LangCn (0)
#define LangEn (1)

#include "ScriptFile.h"
#include "ScpScriptCommand.h"
#include "ScpGlobalCommands.h"
#include "ScpObject_uni.h"
#include "ScpRuntimeStack.h"
#include "ScriptScanner.h"
#include "ScpAst.h"
#include "ScriptByteCode.h"
#include "ScpObject.h"
//语法对象 (Syntax objects)

#include "ScpGlobalCommandObject.h"
#include "ScpObjectSpace.h"
#include "ScpUserObject.h"
#include "ScpScriptLex.h"
#include "ScpExpressionAnalyser.h"
#include "ScpExpressionsObject.h"
#include "ScpExtendObjectMgr.h"
#include "ScpObjectMgr.h"
#include "ScpByteCode.h"
#include "ScriptByteCode.h"

#include "ScriptDebugger.h"
#include "UIMessage.h"

class CScriptCommand;
class ScpClassObject;
class ScpObject;

class CScriptEngine
{
public:
	CScriptEngine();
	~CScriptEngine();
	
	/*
	注册界面消息回调 (Register UI callback)
	*/
	BOOL RegisterUICallBack(void * uiclass,UICallBack callback);
	BOOL RegisterUIStepCallBack(void * uiclass,UICallBack callback);
	/*
	以调试模式执行序列化为字符串的脚本 (Execute serialized script in debug mode)
	*/
	int DebugMemoryScript(const char * memroyscript);
	/*
	导入库文件 (Import library file)
	*/
	int ImportLib(std::string libfilename,bool islib=true);
	int ImportLibFromMemory(void * Mem,bool islib=true);
	int ParseLibBody(const char * libFileName, VTSTRINGS &LibBody, bool islib);
	/*
	将当前脚本转储为文件 (Dump current script to file)
	*/
	int DumpScript(std::string newscriptfilename);
	/*
	执行脚本文件 (Execute script file)
	*/
	int DoScript(std::string scriptfilename,SCRIPTRUNTYPE mode=RUN_NORMAL); //0 normal run 1 debug 2 singlestep
	/*
	将一行字符串作为脚本执行 (Execute a line of string as script)
	*/
	int DoString(std::string &script);
	/*
	将函数中的一行脚本预编译为内部格式 (Precompile a line of script in function to internal format)
	*/

	/*
	清理脚本引擎 (Cleanup script engine)
	*/
	void Cleanup();
	/*
	获得当前执行的脚本的行号 (Get current executing script line number)
	*/
	int GetCurrentCommandLine();

	/*
	枚举全局名字空间中的对象名 (Enumerate object names in global namespace)
	*/
	VTSTRINGS EnumObjects();
	/*
	显示指定对象的内部信息 (Display internal information of specified object)
	*/
	void DumpObject(std::string objectname);
	
	/*
	获得当前名字空间 (Get current namespace)
	*/
	ScpObjectSpace * GetCurrentObjectSpace();
	/*
	切换当前名字空间 (Switch current namespace)
	*/
	void SetCurrentObjectSpace(ScpObjectSpace * objspace);
	/*
	获得上一条错误信息 (Get last error message)
	*/
	std::string GetLastErrorString();
	/*
	设置最后一条错误消息 (Set last error message)
	*/
	void SetLastErrorString(const char * error);

	/*
	根据命令名映射命令 (Map command by command name)
	*/
	ULONG QueryCommandValue(const std::string & commandstring);
	/*
	查询当前允许注册的用户命令值 (Query current allowed user command value)
	*/
	ULONG QueryCurrentUserCommand();

	int GetLanguge();
	
	std::string GetCurrentSourceLine();

    bool RegisterGlobalFunction(const std::string chscommandstring, const std::string   engcommandstring, unsigned long commandid, GlobalCommandFunction Func);
    void  RegisterGlobalFunctions();
    ScpGlobalCommandObject globalcommand;

	int DoByteCode(const char * ByteCodeFile, SCRIPTRUNTYPE mode = RUN_NORMAL);
	int DoMemByteCode(const unsigned char* ByteCode, unsigned int length, SCRIPTRUNTYPE mode = RUN_NORMAL);
	ScpByteCode scpbytecode;
	int Jit;
	int Build;
	int DumpByteCode(const char *ByteCodeFile);
	/*
	设置调试器 实例 (Set debugger instance)
	*/
	bool SetDebugger(IScriptDebugger * dbg);
	void SetScriptFileName(const char*filePathName);
	std::string GetCurrentScriptfilename() {
		return currentscriptfilename;;
	}
	std::string globallib;


	/*
	指令派发，一行脚本对应一个指令 (Command dispatch, one line of script corresponds to one command)
	*/
    BOOL FetchCommand(unsigned long commandvalue,VTPARAMETERS * vtparameters =NULL);


	ScpExpressionAnalyser& getExpressionAnalyser()
	{
		return ana;
	}

	int getLanguage()
	{
		return language;
	}
	ULONG get_usercommand()
	{
		return vl_usercommand;
	}
	void increment_usercommand()
	{
		vl_usercommand++;
	}
	ScriptByteCode & getScriptByteCode()
	{
		return bytecode;
	}
	IScriptDebugger * getDebugger()
	{
		return debugger;
	}
	std::string & getCurrentScriptFileName()
	{
		return currentscriptfilename;
	}
	//切换语言 (Switch language)
	void SwitchLanguage(std::string &comment);
private:
	/*
	创建存储当前命令行参数的表对象 (Create table object to store current command line arguments)
	*/
	void Create_Global_CommndLine_TableObject();
	void Create_Global_Environment_TableObject();
	void Create_Global_CurrentTimeObject();
	/*
	初始化脚本引擎 (Initialize script engine)
	*/
	void Init();
	

	
	/*
	加载脚本 (Load script)
	*/
	bool LoadAllScript(std::string scriptfilename);
	/*
	执行已加载的脚本 (Execute loaded script)
	逐行解析并执行脚本 (Parse and execute script line by line)
	*/
	int DoloadedScript(SCRIPTRUNTYPE mode=RUN_NORMAL);

	//当前语言标记 (Current language flag)
	int language;
	
	
	
	ULONG vl_usercommand;
	DWORD dwTimeCount;
	                                                                                                  
	//指向全局名字空间 (Pointer to global namespace)
	ScpObjectSpace * globalObjectSpace;
	//指向当前名字空间 (Pointer to current namespace)
	ScpObjectSpace * currentObjectSpace;
	CScriptCommand * scriptcommand;
		
	//当前运行到的脚本行号 (Current executing script line number)
	int currentcommandline;
	//保存所有断点的行号 (Save all breakpoint line numbers)
	VTINT breakpoints;

	//当前脚本文件名 (Current script file name)
	std::string currentscriptfilename;
	//当前脚本完整内容 (Full content of current script)
	VTSTRINGS allScriptBody;
	//保存已导入的库文件名 (Save imported library file names)
	VTSTRINGS importedlibs;

	CScriptFile scriptfile;

	std::string LastErrorString;

	//保存当前脚本的字节码 (Save bytecode of current script)
	ScriptByteCode bytecode;
	
	ScpScriptLex lex;
	ScpExpressionAnalyser ana;

	SCRIPTRUNTYPE runmode;
	IScriptDebugger * debugger;
	
public:

	CScpObjectMgr obj_mgr;
	//扩展模块相关 (Related to extension modules)
	ScpExtendObjectMgr extend_obj_mgr;

	//输出重定向 (Output redirection)
	CUIMessage uimessage;
    void PrintError(const std::wstring& ErrorMessage, bool withnewline = true)
	{
		uimessage.PostUIMessage(ErrorMessage.c_str(), withnewline);
	}
    void PrintError(const std::string& ErrorMessage, bool withnewline = true)
	{
		uimessage.PostUIMessage(ErrorMessage.c_str(), withnewline);
	}
	void PrintError(const wchar_t * ErrorMessage, bool withnewline = true)
	{
		uimessage.PostUIMessage(ErrorMessage,withnewline);
	}
	void PrintError(const char * ErrorMessage, bool withnewline = true)
	{
		uimessage.PostUIMessage(ErrorMessage,withnewline);
	}
	void PrintErrorWithNewLine(const wchar_t * Format, ...)
	{
#ifdef _WIN32
		va_list pArgs;
		va_start(pArgs, Format);
		const static DWORD BufferSize = 4096*2;
		wchar_t * szMessageBuffer = (wchar_t *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, BufferSize);
		if (szMessageBuffer)
		{
			memset(szMessageBuffer, 0, BufferSize);
			StringCbVPrintfW(szMessageBuffer, BufferSize, Format, pArgs);
			uimessage.PostUIMessage(szMessageBuffer, TRUE);
			HeapFree(GetProcessHeap(), 0, szMessageBuffer);
		}
		va_end(pArgs);
#else
#endif
	}
};

#endif //_H_SCRIPTENGINE