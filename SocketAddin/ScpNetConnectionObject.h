﻿#ifndef _H_SCPNETCONNECTIONOBJECT
#define _H_SCPNETCONNECTIONOBJECT
#include "../ScriptEngineLib/ScpObject.h"
#include "ScpAddressObject.h"
#include "../ScriptEngineLib/ScpFileObject.h"
#include "../ScriptEngineLib/ScpStringObject.h"
#include "../ScriptEngineLib/ScpMemoryObject.h"
const static ScpObjectType	ObjNet = 4;
const static ScpObjectType	ObjNetConnection = 38;

const static wchar_t * str_CN_ObjNet = L"网络";
const static wchar_t * str_CN_ObjNetConnection = L"网络连接";

const static wchar_t * str_EN_ObjNet = L"network";
const static wchar_t * str_EN_ObjNetConnection = L"netconnection";
class ScpNetConnectionObject :
	public ScpObject
{
public:
	ScpNetConnectionObject(void);
	~ScpNetConnectionObject(void);

	virtual void Show(CScriptEngine * engine);
	virtual ScpObject * Clone(std::wstring strObjName);
	virtual std::wstring ToString();
	virtual void Release();
	virtual bool IsInnerFunction(std::wstring & functionname);
	virtual ScpObject * CallInnerFunction(std::wstring & functionname,VTPARAMETERS * parameters,CScriptEngine * engine);

	int Connect(ScpAddressObject * remote);
	int Watch(ScpAddressObject *local);
	int Send(ScpStringObject * strsrc);
	int Receive(ScpStringObject * strdst);
	int Send(ScpMemoryObject * memsrc);
	int Receive(ScpMemoryObject * memdst);
	int Send(ScpFileObject * srcFile);
	int Receive(ScpFileObject *destFile);
	int Send(ScpObject * srcObj);
	int Receive(ScpObject * destObj);
	int DisConnect();



	ScpAddressObject LocalAddress;
	ScpAddressObject RemoteAddress;

#ifdef _WIN32
	SOCKET sock;
	SOCKET nsock ;
#else 
	int sock;
	int nsock;
#endif
	CScriptEngine * _engine;
	std::wstring connectionType;
	static const size_t MaxNetBufferSize = 1024*32;
	   

	static ScpObject * InnerFunction_Show(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_Get(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_send(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_receive(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_watch(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_connect(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_shutdown(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);

};


ScpObject *  __stdcall NetConnectionObjFactory(VTPARAMETERS * paramters, CScriptEngine * engine);

BOOL WINAPI NetConnection_Shutdown_Command(VTPARAMETERS * vtparameters, CScriptEngine * engine);
BOOL WINAPI NetConnection_Connect_Command(VTPARAMETERS * vtparameters, CScriptEngine * engine);
BOOL WINAPI NetConnection_Send_Command(VTPARAMETERS * vtparameters, CScriptEngine * engine);
BOOL WINAPI NetConnection_Receive_Command(VTPARAMETERS * vtparameters, CScriptEngine * engine);
BOOL WINAPI NetConnection_Watch_Command(VTPARAMETERS * vtparameters, CScriptEngine * engine);
#endif //_H_SCPNETCONNECTIONOBJECT