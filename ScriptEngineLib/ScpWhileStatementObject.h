/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
//循环语句对象
*/
#ifndef _H_WHILESTATEMENTOBJECT
#define _H_WHILESTATEMENTOBJECT
#include "ScpObject.h"
#include "ScpObjectSpace.h"
#include "ScriptEngine.h"
#include "ScpExpressionBlock.h"
class CScriptEngine;
class ScpWhileStatementObject :
	public ScpObject
{
public:


	ScpWhileStatementObject(CScriptEngine * eg);
	~ScpWhileStatementObject(void);


	virtual void Show(CScriptEngine * engine);
	virtual ScpObject * Clone(std::string strObjName);
	virtual std::string ToString();
	virtual void Release() ;
	virtual bool IsInnerFunction(std::string & functionname);
	virtual ScpObject * CallInnerFunction(std::string & functionname, VTPARAMETERS * parameters, CScriptEngine * engine);
	int Do(CScriptEngine *engine);
	void ClearLocalObjects();
	void SetCondition(std::string condition);
	int ReComputeConditionResult();

	
	std::string ConditionExpression;
	std::string Name;
	ScpObjectSpace WhileStatementObjectSpace;
	CScriptEngine * engine;
	ScpExpressionBlock *whilexpressionblock;
	int ConditionResult;

	bool MakeConditionByteCode();

	ByteCodeMemoryStream condition_bytecodemem;
	ByteCodeMemoryStream whileblock_bytecodemem;
	ByteCodeMemoryStream bytecodemem_whilestmt;
	
};

#endif //_H_WHILESTATEMENTOBJECT