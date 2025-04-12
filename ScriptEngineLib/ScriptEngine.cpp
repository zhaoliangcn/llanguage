/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
//L脚本语言解释引擎，包括脚本文件的加载，逐行解析，脚本调试支持，
//库文件导入，扩展命令的注册、扩展对象的加载和注册
*/
// ScritpEngine.cpp : Defines the entry point for the console application.
//
#include "ScriptEngine.h"
#include "ScpScriptLex.h"
#include "commanddefine_uni.h"
#include "../Common/commonutil.hpp"
#include "ScpExtendObjectMgr.h"
#include "ScpObjectMgr.h"
#include "ScpCommonObject.h"
#include "ScpObjectNammes.h"
#include "addin.h"
#include "ScpOperationMgr.h"
#include <iostream>
#include <fstream>
#ifdef _WIN32

#else
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#endif

//动作：对象,参数
//动作：参数,参数

void CScriptEngine::Init()
{
	language = 0;
	Jit = 0;
	Build = 0;
	globalcommand.InitBaseCommand();

	
	

	ScpGlobalObject::GetInstance()->SelectLanguage(language);
	ScpObjectNames::GetSingleInsatnce()->SelectLanguage(language);

	
	currentcommandline = 0;
	globalObjectSpace = new ScpObjectSpace();
	if (globalObjectSpace)
	{
		globalObjectSpace->spacename = ScpObjectNames::GetSingleInsatnce()->strGlobal;
		globalObjectSpace->ObjectSpaceType = Space_Global;
		globalObjectSpace->belongto = NULL;
		currentObjectSpace = globalObjectSpace;
	}

	vl_usercommand = vl_maxbasecommand;
	ana.Attach(this);
	lex.Attach(this);

	debugger = NULL;
	RegisterGlobalFunctions();
	obj_mgr.RegisterInnerObjects();
	//加载压缩库扩展，支持导入包命令
	extend_obj_mgr.LoadExtension("Zip", &globalcommand);

	//make an global function 
	std::string global_show = "define:string,lasterror\n\
			define:function,show\n\
			define:int, count\n\
			count = parameters.getsize()\n\
			while (count > 0)\n\
			count--\n\
			parameters[count].show()\n\
			end\n\
			end\n";	
	std::string golbal_clear = "define:function,clear\n\
			define:string,t\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			while (count > 0)\n\
			count--\n\
			parameters[count].clear()\n\
			end\n\
			end\n";
	std::string global_delete = "define:function,delete\n\
			define:string,t\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			while (count > 0)\n\
			count--\n\
			parameters[count].delete()\n\
			end\n\
			end\n";
	std::string global_open = "define:function,open\n\
			define:string,t\n\
			define:string,path\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			path = parameters[1]\n\
			#path.show()\n\
			parameters[0].open(path)\n\
			end\n\
			end\n";
	std::string global_seek = "define:function,seek\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			parameters[0].seek(parameters[1])\n\
			end\n\
			end\n";
	std::string global_copy = "define:function,copy\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			parameters[0].copy(parameters[1])\n\
			end\n\
			end\n";
	std::string global_compare = "define:function,compare\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			return(parameters[0].compare(parameters[1]))\n\
			end\n\
			end\n";
	std::string global_acquire = "define:function,acquire\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			return(parameters[0].acquire(parameters[1]))\n\
			end\n\
			end\n";
	std::string global_release = "define:function,release\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			parameters[0].release(parameters[1])\n\
			end\n\
			end\n";
	std::string global_write = "define:function,write\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			parameters[0].write(parameters[1])\n\
			end\n\
			end\n";
	std::string global_read = "define:function,read\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			return(parameters[0].read(parameters[1]))\n\
			end\n\
			end\n";
	std::string global_insert = "define:function,insert\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			parameters[0].insert(parameters[1])\n\
			end\n\
			if (count == 3)\n\
			parameters[0].insert(parameters[2], parameters[1])\n\
			end\n\
			end\n";
	std::string global_match = "define:function,match\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			return(parameters[0].find(parameters[1]))\n\
			end\n\
			end\n";
	std::string global_replace = "define:function,replace\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 3)\n\
			parameters[0].replace(parameters[1], parameters[2])\n\
			end\n\
			end\n";
	std::string global_save = "define:function,save\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			if (count == 2)\n\
			parameters[0].save(parameters[1])\n\
			end\n\
			if (count == 1)\n\
			parameters[0].save()\n\
			end\n\
			end\n";
	std::string global_print = "define:function,print\n\
			define:int,count\n\
			count = parameters.getsize()\n\
			while (count > 0)\n\
			count--\n\
			parameters[count].show()\n\
			end\n\
			end\n";
	globallib = "#scplib\n#scpeng\n" +
		global_show + "\n" +
		golbal_clear + "\n" +
		global_delete + "\n" +
		global_open + "\n" +
		global_compare + "\n" +
		global_read + "\n" +
		global_insert + "\n" +
		global_match + "\n" +
		global_replace + "\n" +
		global_save + "\n" +
		global_print + "\n";
	
	

}
BOOL CScriptEngine::FetchCommand(unsigned long commandvalue, VTPARAMETERS * vtparameters)
{
	BOOL extCommand = FALSE;
	size_t dwExtCommandCount = globalcommand.GetExtCommandFunctionCount(commandvalue);
	if (dwExtCommandCount > 0)
	{
		//多个扩展对象可能会注册同一个扩展命令
		//所以扩展命令会发送到所有注册者，注册者必须进行必要的类型检测
		for (size_t index = 0; index < dwExtCommandCount; index++)
		{
			ExtObjectCommandFunction func = globalcommand.GetExtCommandFunctionAddress(commandvalue, index);
			if (func)
			{
				extCommand = TRUE;
				func(vtparameters, this);
			}
		}
	}
	GlobalCommandFunction globalfunc = globalcommand.GetGlobalCommandFunction(commandvalue);
	if (globalfunc)
	{
		return	globalfunc(vtparameters, this);
	}
	switch (commandvalue)
	{
	case vl_invalid:
	{
		PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidScript);
	}
	break;
	case vl_reserved:
	{
		PrintError(std::wstring(L"vl_reserved"));
	}
	break;	
	case vl_do:
	{
		scriptcommand->Do_Do_Command(vtparameters, this);
	}
	break;
	case vl_define:
	{
		scriptcommand->Do_Define_Command(vtparameters, this);
	}
	break;
	case vl_compute:
	{
		scriptcommand->Do_Compute_Command(vtparameters, this);
	}
	break;
	case vl_test:
	{
		scriptcommand->Do_Test_Command(vtparameters, this);
	}
	break;
	case vl_end:
	{
		scriptcommand->Do_End_Command(vtparameters, this);
	}
	break;
	case vl_call:
	{
		scriptcommand->Do_Call_Command(vtparameters, this);
	}
	break;
	case vl_while:
	{
		scriptcommand->Do_While_Command(vtparameters, this);
	}
	break;
	case vl_register:
	{
		scriptcommand->Do_Register_Command(vtparameters, this);
	}
	break;
	case vl_import:
	{
		scriptcommand->Do_Import_Command(vtparameters, this);
	}
	break;
	case vl_wait:
	{
		scriptcommand->Do_Wait_Command(vtparameters, this);
	}
	break;
	case vl_otherwise:
	{
		scriptcommand->Do_Otherwise_Command(vtparameters, this);
	}
	break;
	case vl_inherit:
	{
		scriptcommand->Do_Inherit_Command(vtparameters, this);
	}
	break;
	case vl_load:
	{
		scriptcommand->Do_Load_Command(vtparameters, this);
	}
	break;
	case vl_exit:
	{
		scriptcommand->Do_Exit_Command(vtparameters, this);
	}
	break;
	case vl_continue:
	{
		scriptcommand->Do_Continue_Command(vtparameters, this);
	}
	break;
	case vl_break:
	{
		scriptcommand->Do_Break_Command(vtparameters, this);
	}
	break;
	case vl_loop:
	{
		scriptcommand->Do_Loop_Command(vtparameters, this);
	}
	break;
	case vl_public:
	{
		scriptcommand->Do_Public_Command(vtparameters, this);
	}
	break;
	case vl_private:
	{
		scriptcommand->Do_Private_Command(vtparameters, this);
	}
	break;
	case vl_return:
	{
		scriptcommand->Do_Return_Command(vtparameters, this);
	}
	break;
	default:
	{
		if (!extCommand)
		{
			PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidScript);
		}
	}
	break;
	}
	return TRUE;
}
ULONG CScriptEngine::QueryCommandValue(const std::string & commandstring)
{
	return globalcommand.QueryCommand(commandstring);
}
ULONG CScriptEngine::QueryCurrentUserCommand()
{
	return vl_usercommand;
}
int CScriptEngine::ParseLibBody(const char * libFileName, VTSTRINGS &LibBody, bool islib)
{
	if (LibBody.size() == 0)
	{
		return -1;
	}
	ITSTRINGS it;
	std::string headline;
	it = LibBody.begin();
	if (LibBody.size() > 0)
	{
		headline = *it;
		if (islib)
		{
			if (!CScriptFile::IsValidScriptLib(headline))
			{
				return -1;
			}
		}
		else
		{
			if (!CScriptFile::IsValidScript(headline))
			{
				return -1;
			}
		}
	}
	else
	{
		PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorLibNotExist);
	}
	int libcurrentcommandline = 0;

	while (libcurrentcommandline < LibBody.size())
	{
		std::string &wcommandline = LibBody.at(libcurrentcommandline);
		if (wcommandline.empty())
		{
			//忽略空行
			libcurrentcommandline++;
			continue;
		}
		if (CScriptFile::IsComment(wcommandline))
		{
			SwitchLanguage(wcommandline);
			libcurrentcommandline++;
			continue;
		}
		else if (CScriptFile::IsCppComment(wcommandline))
		{
			libcurrentcommandline++;
			continue;
		}
		else
		{
			ULONG value;
			VTPARAMETERS vtparameters;

			if (!lex.ParseCommandLine(wcommandline, value, vtparameters))
			{
				libcurrentcommandline++;
				continue;
			}


			if (value == vl_define && vtparameters.size() > 0 && vtparameters.at(0) == ScpObjectNames::GetSingleInsatnce()->strObjClass)
			{
				if (!ana.ParseClassDefine(libFileName,LibBody, libcurrentcommandline))
				{
					break;
				}
				else
				{
					continue;
				}
			}
			else if (value == vl_define && vtparameters.size() > 0 && vtparameters.at(0) == ScpObjectNames::GetSingleInsatnce()->strObjStruct)
			{
				if (!ana.ParseStructDefine(libFileName,LibBody, libcurrentcommandline))
				{
					break;
				}
				else
				{
					continue;
				}
			}
			else if (value == vl_define && vtparameters.size() > 0 && vtparameters.at(0) == ScpGlobalObject::GetInstance()->GetTypeName(ObjFunction))
			{
				if (!ana.ParseFunctionDefine(libFileName,LibBody, libcurrentcommandline))
				{
					break;
				}
				else
				{
					continue;
				}
			}
			if (value == vl_define || value == vl_start || value == vl_end || value == vl_register || value == vl_import || value == vl_load)
			{
				if (value == vl_define)
				{
					if (vtparameters.at(0) == ScpGlobalObject::GetInstance()->GetTypeName(ObjFunction))
					{
						if (vtparameters.at(1) == ScpObjectNames::GetSingleInsatnce()->strObjMainFunction)
						{
							libcurrentcommandline++;
							continue;
						}
					}
				}
				FetchCommand(value, &vtparameters);
				libcurrentcommandline++;
			}
			else
			{
				break;
			}
		}
	};
	return 0;
}
int CScriptEngine::ImportLibFromMemory(void * Mem, bool islib)
{
	if (!Mem)
	{
		return -1;
	}
	VTSTRINGS LibBody;
	scriptfile.ReadAllFromMemory((char *)Mem, LibBody);
	return ParseLibBody((char*)NULL,LibBody, islib);
}
int CScriptEngine::ImportLib(std::string libfilename, bool islib)
{
	int ret = -1;
	//避免重复导入
	ITSTRINGS it = importedlibs.begin();
	while (it != importedlibs.end())
	{
		if (*it == libfilename)
		{
			return 0;
		}
		it++;
	}
#ifdef _WIN32
	//win32环境下的库文件查找路径
	if (libfilename.find("\\") == std::string::npos)
	{
		//如果只给出了库文件的名字但没有给出路径
		std::string EnginePath;
		char path[MAX_PATH] = { 0 };
		GetModuleFileNameA(NULL, path, sizeof(path));
		EnginePath = CScriptFile::GetPath(path);
		std::string temp;
		temp = EnginePath;
		temp += "\\lib\\";
		temp += libfilename;
		BOOL Exist = CScriptFile::FileExist(temp);
		if (!Exist)
		{
			temp = EnginePath;
			temp += "\\";
			temp += libfilename;
			Exist = CScriptFile::FileExist(temp);
			if (!Exist)
			{
				std::string ScriptPath;
				ScriptPath = CScriptFile::GetPath(currentscriptfilename.c_str());
				temp = ScriptPath;
				temp += "\\";
				temp += libfilename;
				Exist = CScriptFile::FileExist(temp);
			}
		}
		if (Exist)
		{
			libfilename = temp;
		}
		else
		{
			return -1;
		}
	}
#endif
	if (!CScriptFile::FileExist(libfilename))
	{
		return -1;
	}
	VTSTRINGS LibBody;
	if (scriptfile.ReadAll(libfilename.c_str(), LibBody))
	{
		ret= ParseLibBody(libfilename.c_str(),LibBody, islib);
		if (ret == 0)
		{
			importedlibs.push_back(libfilename);
		}
	}
	return 0;
}

int CScriptEngine::DoString(std::string& script)
{
	if (CScriptFile::IsComment(script))
	{
		SwitchLanguage(script);
		return 1;
	}
	else
	{
		if (script.empty())
		{
			return 1;
		}

		ULONG value;
		VTPARAMETERS vtparameters;

		if (!lex.ParseCommandLine(script, value, vtparameters))
		{
			return 1;
		}
		FetchCommand(value, &vtparameters);
		if (GetCurrentObjectSpace()->lastcommand != vl_return)
		{
			GetCurrentObjectSpace()->lastcommand = value;
		}
		return 0;
	}
}
int CScriptEngine::DumpScript(std::string newscriptfilename)
{
#ifdef WIN32
	std::ofstream outfile(newscriptfilename.c_str());
#else
	std::ofstream outfile(newscriptfilename.c_str());
#endif
	std::string commandline;
	for (ITSTRINGS it = allScriptBody.begin();it != allScriptBody.end();it++)
	{
		commandline =*it;
		outfile << commandline << std::endl;
	}
	outfile.close();
	return 0;
}
int  CScriptEngine::DoloadedScript(SCRIPTRUNTYPE mode)
{
	
	runmode = mode;
	currentcommandline = 0;

	Create_Global_CommndLine_TableObject();
	Create_Global_Environment_TableObject();
	Create_Global_CurrentTimeObject();

	while (currentcommandline < allScriptBody.size())
	{
		std::string &wcommandline = allScriptBody.at(currentcommandline);		
		if (wcommandline.empty())
		{
			//忽略空行
			currentcommandline++;
			continue;
		}
		else if (CScriptFile::IsComment(wcommandline))
		{
			SwitchLanguage(wcommandline);
			currentcommandline++;
			continue;
		}
		else if (CScriptFile::IsCppComment(wcommandline))
		{
			currentcommandline++;
			continue;
		}
		{

			ULONG value;
			VTPARAMETERS vtparameters;
			if (!lex.ParseCommandLine(wcommandline, value, vtparameters))
			{			
				currentcommandline++;
				continue;
			}
			if (value == vl_define && vtparameters.size() > 0 && vtparameters.at(0) == ScpObjectNames::GetSingleInsatnce()->strObjClass)
			{				
				if (!ana.ParseClassDefine(this->currentscriptfilename.c_str(),this->allScriptBody, this->currentcommandline))
				{
					break;
				}
				else
				{
					continue;
				}
			}
			else if (value == vl_define && vtparameters.size() > 0 && vtparameters.at(0) == ScpObjectNames::GetSingleInsatnce()->strObjStruct)
			{
				if (!ana.ParseStructDefine(this->currentscriptfilename.c_str(), this->allScriptBody, this->currentcommandline))
				{
					break;
				}
				else
				{
					continue;
				}
			}
			else if (value == vl_define && vtparameters.size() > 0 && vtparameters.at(0) == ScpGlobalObject::GetInstance()->GetTypeName(ObjFunction))
			{
				if (!ana.ParseFunctionDefine(this->currentscriptfilename.c_str(), this->allScriptBody, this->currentcommandline))
				{
					break;
				}
				else
				{
					continue;
				}
			}
			else if (value == vl_while && vtparameters.size() == 1)
			{
				if (!ana.ParseWhileDefine(this->currentscriptfilename.c_str(), this->allScriptBody, this->currentcommandline, true))
				{
					break;
				}
				else
				{
					continue;
				}
			}
			else if (value == vl_test && vtparameters.size() == 1)
			{
				if (!ana.ParseIfDefine(this->currentscriptfilename.c_str(), this->allScriptBody, this->currentcommandline, true))
				{
					break;
				}
				else
				{
					continue;
				}
			}	

			if (debugger)
			{
				debugger->CheckDebugEvent(currentscriptfilename.c_str(), currentcommandline, INFINITE);
			}

			if (Jit || Build)
			{
				ByteCodeMemoryStream memstream;
				bytecode.GenByteCodeFromCommand(value, vtparameters, memstream, this);
				bytecode.bytecodemem->AppendByteCode(memstream.membuf, memstream.codelength);
				memstream.Release();

				if (value == vl_define ||
					value == vl_load||
					value == vl_import||
					value == vl_loop)
				{
					FetchCommand(value, &vtparameters);
				}
			}
			else
			{				
				FetchCommand(value, &vtparameters);
			}
			GetCurrentObjectSpace()->lastcommand = value;
		}
		currentcommandline++;		
	};
	//查找并调用“主函数”
	ScpObject * obj = currentObjectSpace->FindObject(ScpObjectNames::GetSingleInsatnce()->strObjMainFunction);
	if (obj && obj->GetType() == ObjFunction)
	{
		ULONG ulcommand = vl_call;
		VTPARAMETERS param;
		param.push_back(ScpObjectNames::GetSingleInsatnce()->strObjMainFunction);

		if (Jit || Build)
		{
			ByteCodeMemoryStream memstream;
			bytecode.GenByteCodeFromCommand(ulcommand, param, memstream, this);
			bytecode.bytecodemem->AppendByteCode(memstream.membuf, memstream.codelength);			
			memstream.Release();
		}
		else
		{
			FetchCommand(ulcommand, &param);
		}
	}
	
	//dwTimeCount = dwEnd - dwStart;
	if (Jit || Build)
	{
		bytecode.DumpToFile((currentscriptfilename + ".scpb").c_str());
		DoMemByteCode(bytecode.bytecodemem->membuf, bytecode.bytecodemem->codelength);
	}
	currentObjectSpace->userobject.Destroy();
	//printf("%d\n",dwTimeCount);
	return 0;
}

bool CScriptEngine::LoadAllScript(std::string scriptfilename)
{
	allScriptBody.clear();
	return scriptfile.ReadAll(scriptfilename.c_str(), allScriptBody);

}
int CScriptEngine::DoScript(std::string scriptfilename, SCRIPTRUNTYPE mode)
{
	if (LoadAllScript(scriptfilename))
	{
		if (CScriptFile::IsValidScript(allScriptBody.at(0)))
		{
			currentscriptfilename = scriptfilename;	
			ImportLibFromMemory((void *)globallib.c_str(), true);		
			return DoloadedScript(mode);
		}
		else
		{
			PrintError("ErrorInvalidScript");
		}
	}
	return -1;
}

void CScriptEngine::Create_Global_CommndLine_TableObject()
{
	if (currentObjectSpace->FindObject(ScpObjectNames::GetSingleInsatnce()->strCommandParameter))
	{
		return;
	}
#ifdef _WIN32

	LPWSTR *szArglist;
	int nArgs;
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	if (NULL == szArglist)
	{

	}
	else
	{
		ScpTableObject * tableobj = new ScpTableObject;
		if (tableobj)
		{
			for (int i = 0;i < nArgs;i++)
			{
				ScpStringObject * strobj = new ScpStringObject;
				if (strobj)
				{
					strobj->content = STDSTRINGEXT::W2UTF(szArglist[i]);
					std::string tempname = ScpObjectNames::GetSingleInsatnce()->strCommandParameter;
					tempname += IntToString(i);
					tableobj->AddElement(tempname, strobj);
				}
				if (i == 1)
				{
					std::string filepathname = STDSTRINGEXT::W2UTF(szArglist[i]);
					if (!ScpFileObject::IsAbsolutePath(filepathname))
					{
						filepathname  = ScpFileObject::GetAbsolutePath(filepathname);
					}
					//第一个参数是脚本文件名
					ScpStringObject * strobj1 = new ScpStringObject;
					if (strobj1)
					{
						strobj1->content = filepathname;
						currentObjectSpace->AddObject(ScpObjectNames::GetSingleInsatnce()->strCurrentScriptFile, strobj1);
					}
					//获取脚本文件的路径
					//如果是相对路径，需要转换为绝对路径
					ScpStringObject * strobj2 = new ScpStringObject;
					if (strobj2)
					{
						strobj2->content = PathStripFileName(filepathname);
						currentObjectSpace->AddObject(ScpObjectNames::GetSingleInsatnce()->strCurrentScriptPath, strobj2);
					}
				}
			}
			currentObjectSpace->AddObject(ScpObjectNames::GetSingleInsatnce()->strCommandParameter, tableobj);
		}
		LocalFree(szArglist);
	}
#else

	char buff[4096] = { 0 };
	int fd = open("/proc/self/cmdline", O_RDONLY);
	int rsize = read(fd, buff, sizeof(buff));
	if (rsize > 0)
	{
		char* pstart = (char*)buff;
		char* pcmdargs = (char*)buff;
		int i = 0;
		
		ScpTableObject* tableobj = new ScpTableObject;
		if (tableobj)
		{
			while (pcmdargs && ((pcmdargs - pstart) < rsize))
			{
				//printf("%s\n", pcmdargs);
				ScpStringObject* strobj = new ScpStringObject;
				if (strobj)
				{
					strobj->content =pcmdargs;
					std::string tempname = ScpObjectNames::GetSingleInsatnce()->strCommandParameter;
					tempname += IntToString(i);
					tableobj->AddElement(tempname, strobj);
					i++;
				}
				if (i == 1)
				{
					ScpStringObject* strobj1 = new ScpStringObject;
					if (strobj1)
					{
						strobj1->content =pcmdargs;
						currentObjectSpace->AddObject(ScpObjectNames::GetSingleInsatnce()->strCurrentScriptFile, strobj1);
					}
					ScpStringObject* strobj2 = new ScpStringObject;
					if (strobj2)
					{
						strobj2->content = PathStripFileName(pcmdargs);
						currentObjectSpace->AddObject(ScpObjectNames::GetSingleInsatnce()->strCurrentScriptPath, strobj2);
					}
				}
			
				pcmdargs += strlen(pcmdargs) + 1;
			}
			currentObjectSpace->AddObject(ScpObjectNames::GetSingleInsatnce()->strCommandParameter, tableobj);
		}
		
	}
	close(fd);

#endif	
}
void CScriptEngine::Create_Global_Environment_TableObject()
{
	if (currentObjectSpace->FindObject(ScpObjectNames::GetSingleInsatnce()->strObjEnvironmentVariable))
	{
		return;
	}
#ifdef _WIN32

	ScpTableObject * tableobj = new ScpTableObject;
	if (tableobj)
	{
		LPWCH lpEvn = GetEnvironmentStringsW();
		if (lpEvn)
		{
			int i = 0;
			LPWSTR lpszVariable, lpTemp;
			for (lpszVariable = (LPWSTR)lpEvn; *lpszVariable; lpszVariable++)
			{
				std::wstring envstr = lpszVariable;
				if (!envstr.empty() && !(envstr.at(0) == L'='))
				{
					ScpStringObject * strobj = new ScpStringObject;
					if (strobj)
					{
						std::string tempname;
						size_t pos = envstr.find(L"=");
						if (pos != std::wstring::npos)
						{
							//strobj->content =envstr.substr(pos+1);
							strobj->content =STDSTRINGEXT::W2UTF(envstr);
							tempname = ScpObjectNames::GetSingleInsatnce()->strObjEnvironmentVariable;
							tempname += IntToString(i);
							//tempname += L"_";
							//tempname += envstr.substr(0,pos);
							tableobj->AddElement(tempname, strobj);
						}

					}
				}
				lpTemp = lpszVariable;
				while (*lpTemp)
				{
					lpTemp++;
				}
				lpszVariable = lpTemp;
				i++;
				envstr = lpszVariable;
			}
			FreeEnvironmentStringsW(lpEvn);
		}
		currentObjectSpace->AddObject(ScpObjectNames::GetSingleInsatnce()->strObjEnvironmentVariable, tableobj);
	}
#else
	ScpTableObject* tableobj = new ScpTableObject;
	if (tableobj)
	{
		char** penv = (char**)environ;
		if (penv)
		{
			char** envir = penv;
			int i = 0;
			while (*envir)
			{
				//fprintf(stdout, "%s\n", *envir);
				std::string envstr = *envir;
				ScpStringObject* strobj = new ScpStringObject;
				if (strobj)
				{
					std::string tempname;
					size_t pos = envstr.find("=");
					if (pos != std::string::npos)
					{
						strobj->content = envstr;
						tempname = ScpObjectNames::GetSingleInsatnce()->strObjEnvironmentVariable;
						tempname += IntToString(i);
						tableobj->AddElement(tempname, strobj);
					}
					i++;
				}
				envir++;
			}
		}		
		currentObjectSpace->AddObject(ScpObjectNames::GetSingleInsatnce()->strObjEnvironmentVariable, tableobj);
	}
	
#endif	
}
void CScriptEngine::Create_Global_CurrentTimeObject()
{
	if (!currentObjectSpace->FindObject(ScpObjectNames::GetSingleInsatnce()->strObjCurrentTime))
	{
		//Create Global Time Object
		ScpTimeObject * currenttime = new ScpTimeObject;
		if (currenttime)
		{
			currenttime->name = ScpObjectNames::GetSingleInsatnce()->strObjCurrentTime;
			currenttime->value = currenttime->GetNow();
			currentObjectSpace->AddObject(ScpObjectNames::GetSingleInsatnce()->strObjCurrentTime, currenttime);
		}
	}
}
std::string CScriptEngine::GetCurrentSourceLine()
{
	std::string error = "LineNumber: " + IntToString(currentcommandline);
	error += " Source : ";
	if(currentcommandline>=0 && currentcommandline < allScriptBody.size())
		error += allScriptBody[currentcommandline];
	return error;
}
bool CScriptEngine::RegisterGlobalFunction(const std::string  chscommandstring, const std::string  engcommandstring, unsigned long commandid, GlobalCommandFunction Func)
{
	return globalcommand.RegisterGlobalFunction(chscommandstring, engcommandstring, commandid, Func);
}
void CScriptEngine::RegisterGlobalFunctions()
{
	RegisterGlobalFunction(scpcommand_cn_sleep, scpcommand_en_sleep, vl_sleep, GlobalCommands::Do_Sleep_Command);
	RegisterGlobalFunction(scpcommand_cn_clear, scpcommand_en_clear, vl_clear, GlobalCommands::Do_Clear_Command);
	RegisterGlobalFunction(scpcommand_cn_shutdown, scpcommand_en_shutdown, vl_shutdown, GlobalCommands::Do_Shutdown_Command);
	RegisterGlobalFunction(scpcommand_cn_popup, scpcommand_en_popup, vl_popup, GlobalCommands::Do_Popup_Command);
	RegisterGlobalFunction(scpcommand_cn_run, scpcommand_en_run, vl_run, GlobalCommands::Do_Run_Command);
	RegisterGlobalFunction(scpcommand_cn_show, scpcommand_en_show, vl_show, GlobalCommands::Do_Show_Command);
	RegisterGlobalFunction(scpcommand_cn_delete, scpcommand_en_delete, vl_delete, GlobalCommands::Do_Delete_Command);
	RegisterGlobalFunction(scpcommand_cn_open, scpcommand_en_open, vl_open, GlobalCommands::Do_Open_Command);
	RegisterGlobalFunction(scpcommand_cn_seek, scpcommand_en_seek, vl_seek, GlobalCommands::Do_Seek_Command);
	RegisterGlobalFunction(scpcommand_cn_copy, scpcommand_en_copy, vl_copy, GlobalCommands::Do_Copy_Command);
	RegisterGlobalFunction(scpcommand_cn_connect, scpcommand_en_connect, vl_connect, GlobalCommands::Do_Connect_Command);
	RegisterGlobalFunction(scpcommand_cn_capture, scpcommand_en_capture, vl_capture, GlobalCommands::Do_Capture_Command);
	RegisterGlobalFunction(scpcommand_cn_compare, scpcommand_en_compare, vl_compare, GlobalCommands::Do_Compare_Command);
	RegisterGlobalFunction(scpcommand_cn_acquire, scpcommand_en_acquire, vl_acquire, GlobalCommands::Do_Acquire_Command);
	RegisterGlobalFunction(scpcommand_cn_release, scpcommand_en_release, vl_replace, GlobalCommands::Do_Release_Command);
	RegisterGlobalFunction(scpcommand_cn_write, scpcommand_en_write, vl_write, GlobalCommands::Do_Write_Command);
	RegisterGlobalFunction(scpcommand_cn_read, scpcommand_en_read, vl_read, GlobalCommands::Do_Read_Command);
	RegisterGlobalFunction(scpcommand_cn_start, scpcommand_en_start, vl_start, GlobalCommands::Do_Start_Command);
	RegisterGlobalFunction(scpcommand_cn_getsubstring, scpcommand_en_getsubstring, vl_getsubstring, GlobalCommands::Do_Getsubstring_Command);
	RegisterGlobalFunction(scpcommand_cn_insert, scpcommand_en_insert, vl_insert, GlobalCommands::Do_Insert_Command);
	RegisterGlobalFunction(scpcommand_cn_match, scpcommand_en_match, vl_match, GlobalCommands::Do_Match_Command);
	RegisterGlobalFunction(scpcommand_cn_find, scpcommand_en_find, vl_find, GlobalCommands::Do_Find_Command);
	RegisterGlobalFunction(scpcommand_cn_replace, scpcommand_en_replace, vl_replace, GlobalCommands::Do_Replace_Command);
	RegisterGlobalFunction(scpcommand_cn_move, scpcommand_en_move, vl_move, GlobalCommands::Do_Move_Command);
	RegisterGlobalFunction(scpcommand_cn_getsize, scpcommand_en_getsize, vl_getsize, GlobalCommands::Do_Getsize_Command);
	RegisterGlobalFunction(scpcommand_cn_enum, scpcommand_en_enum, vl_enum, GlobalCommands::Do_Enum_Command);
	RegisterGlobalFunction(scpcommand_cn_generate, scpcommand_en_generate, vl_generate, GlobalCommands::Do_Generate_Command);
	RegisterGlobalFunction(scpcommand_cn_refresh, scpcommand_en_refresh, vl_generate, GlobalCommands::Do_Refresh_Command);
	RegisterGlobalFunction(scpcommand_cn_getelement, scpcommand_en_getelement, vl_getelement, GlobalCommands::Do_Getelement_Command);
	RegisterGlobalFunction(scpcommand_cn_traverse, scpcommand_en_traverse, vl_traverse, GlobalCommands::Do_Traverse_Command);
	RegisterGlobalFunction(scpcommand_cn_sort, scpcommand_en_sort, vl_sort, GlobalCommands::Do_Sort_Command);
	RegisterGlobalFunction(scpcommand_cn_format, scpcommand_en_format, vl_format, GlobalCommands::Do_Format_Command);
	RegisterGlobalFunction(scpcommand_cn_get, scpcommand_en_get, vl_get, GlobalCommands::Do_Get_Command);
	RegisterGlobalFunction(scpcommand_cn_set, scpcommand_en_set, vl_set, GlobalCommands::Do_Set_Command);
	RegisterGlobalFunction(scpcommand_cn_transform, scpcommand_en_transform, vl_transform, GlobalCommands::Do_Transform_Command);
	RegisterGlobalFunction(scpcommand_cn_split, scpcommand_en_split, vl_split, GlobalCommands::Do_Split_Command);
	RegisterGlobalFunction(scpcommand_cn_push, scpcommand_en_push, vl_push, GlobalCommands::Do_Push_Command);
	RegisterGlobalFunction(scpcommand_cn_print, scpcommand_en_print, vl_print, GlobalCommands::Do_Print_Command);
	RegisterGlobalFunction(scpcommand_cn_create, scpcommand_en_create, vl_create, GlobalCommands::Do_Create_Command);
	RegisterGlobalFunction(scpcommand_cn_save, scpcommand_en_save, vl_save, GlobalCommands::Do_Save_Command);


}
int CScriptEngine::DoByteCode(const char * ByteCodeFile, SCRIPTRUNTYPE mode )
{
	currentscriptfilename = ByteCodeFile;
	scpbytecode.engine = this;
	scpbytecode.Load(ByteCodeFile);
	return scpbytecode.Do();
}
int CScriptEngine::DoMemByteCode(const unsigned char * ByteCode, unsigned int length, SCRIPTRUNTYPE mode)
{
	scpbytecode.engine = this;
	scpbytecode.LoadFromMem(ByteCode, length);
	return scpbytecode.Do();
}
int CScriptEngine::DumpByteCode(const char * ByteCodeFile)
{
	if(ByteCodeFile==NULL)
		bytecode.DumpToFile((currentscriptfilename + ".scpb").c_str());
	else
		bytecode.DumpToFile(ByteCodeFile);
	return 0;
}
bool CScriptEngine::SetDebugger(IScriptDebugger * dbg)
{
	if (dbg!=NULL && debugger == NULL)
	{
		debugger = dbg;
		return true;
	}
	return false;
}
void CScriptEngine::SetScriptFileName(const char * filePathName)
{
	currentscriptfilename = filePathName;
}
CScriptEngine::CScriptEngine()
{
	Init();
}
CScriptEngine::~CScriptEngine()
{
	if (globalObjectSpace)
		delete globalObjectSpace;
	extend_obj_mgr.FreeAllExtensions();

}
BOOL CScriptEngine::RegisterUICallBack(void * uiclass, UICallBack callback)
{
	return uimessage.RegisterUICallBack(uiclass, callback);
}
BOOL CScriptEngine::RegisterUIStepCallBack(void * uiclass, UICallBack callback)
{
	return uimessage.RegisterUIStepCallBack(uiclass, callback);
}
int CScriptEngine::DebugMemoryScript(const char * memroyscript)
{
	allScriptBody.clear();
	scriptfile.ReadAllFromMemory(memroyscript, allScriptBody);
	return DoloadedScript(RUN_DEBUG);
}
void CScriptEngine::Cleanup()
{


	currentObjectSpace->userobject.Destroy();
	currentcommandline = 0;

	breakpoints.clear();
	allScriptBody.clear();

}

int CScriptEngine::GetCurrentCommandLine()
{
	return currentcommandline;
}

VTSTRINGS CScriptEngine::EnumObjects()
{
	return currentObjectSpace->userobject.EnumObjects();
}
void CScriptEngine::DumpObject(std::string objectname)
{
	return currentObjectSpace->userobject.DumpObject(objectname, this);
}
ScpObjectSpace * CScriptEngine::GetCurrentObjectSpace()
{
	return currentObjectSpace;
}
void CScriptEngine::SetCurrentObjectSpace(ScpObjectSpace * objspace)
{
	if (objspace)
	{
		currentObjectSpace = objspace;
	}
}
std::string CScriptEngine::GetLastErrorString()
{
	return LastErrorString;
}
void CScriptEngine::SetLastErrorString(const char * error)
{
	LastErrorString = error;
	ScpStringObject* obj = (ScpStringObject*)currentObjectSpace->FindObject("lasterror");
	if (obj && obj->GetType()==ObjString)
	{
		obj->content = error;
	}
}

void CScriptEngine::SwitchLanguage(std::string &comment)
{
	if (comment == "#scpeng")
	{
		language = 1;
		if (Jit || Build)
		{

			ByteCodeMemoryStream memstream;
			bytecode.GenByteCodeFromSouceLine(comment, memstream);
			bytecode.bytecodemem->AppendByteCode(memstream.membuf, memstream.codelength);
			memstream.Release();
		}

		ScpObjectNames::GetSingleInsatnce()->SelectLanguage(language);
		ScpGlobalObject::GetInstance()->SelectLanguage(language);
		Create_Global_CurrentTimeObject();
		Create_Global_CommndLine_TableObject();
		Create_Global_Environment_TableObject();
	}
	else if (comment == "#scpchs")
	{
		language = 0;
		if (Jit || Build)
		{
			ByteCodeMemoryStream memstream;
			bytecode.GenByteCodeFromSouceLine(comment, memstream);
			bytecode.bytecodemem->AppendByteCode(memstream.membuf, memstream.codelength);
			memstream.Release();
		}

		ScpObjectNames::GetSingleInsatnce()->SelectLanguage(language);
		ScpGlobalObject::GetInstance()->SelectLanguage(language);
		Create_Global_CurrentTimeObject();
		Create_Global_CommndLine_TableObject();
		Create_Global_Environment_TableObject();
	}

}

int CScriptEngine::GetLanguge()
{
	return language;
}




