/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
//函数对象，函数形参和实参的绑定，解绑，函数体的调用
*/
#ifndef _H_SCPFUNCTIONOBJECT
#define _H_SCPFUNCTIONOBJECT

#include "ScpObjectSpace.h"
#include "ScriptEngine.h"
#include "NumberHelp.h"
#include "ScpExpressionBlock.h"
#include "ScpListObject.h"
class CScriptEngine;
class ScpFunctionObject:public ScpObject
{
public:



	ScpFunctionObject();
	~ScpFunctionObject();
	virtual void Show(CScriptEngine * engine);
	virtual ScpObject * Clone(std::string strObjName);
	virtual std::string ToString();
	virtual void Release() ;
	virtual bool IsInnerFunction(std::string & functionname);
	virtual ScpObject * CallInnerFunction(std::string & functionname,VTPARAMETERS * parameters,CScriptEngine * engine);

	int Do(CScriptEngine *engine);	
	int BindParameters(CScriptEngine *engine);
	int UnBindParameters(CScriptEngine *engine);
	void ClearLocalObjects();
	int GetFormalParametersCount();
	std::string GetCloneName();
	VTSTRINGS Boday;
	VTPARAMETERS FormalParameters;
	VTPARAMETERS RealParameters;
	VTSTRINGS *pBoday;
	VTPARAMETERS *pFormalParameters;
	std::string Name;
	ScpObjectSpace * FunctionObjectSpace;
	bool iscloned;
	ScpObject * Result;
	ScpExpressionBlock *functionexpressionblock;
	unsigned char * ByteCode;
	int ByteCodeLength;
	//由于函数的参数类型未知，所以需要在函数被调用时再生成函数的字节码
	//为了实现函数的多态，可以对不同的参数类型生成不同的字节码
	ByteCodeMemoryStream bytecodemem_funcdef;
	ByteCodeMemoryStream bytecodemem_funcbody;
	//对于类的成员函数，这里保存类的名字
	std::string parentobjectname;
	ScpObject * ParenObject;
	ScpListObject* paramlist;

	static ScpObject * InnerFunction_Show(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_Get(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
};
#endif //_H_SCPFUNCTIONOBJECT