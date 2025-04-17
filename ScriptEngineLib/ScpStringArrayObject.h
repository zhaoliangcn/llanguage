/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
*/
#ifndef _H_SCPSTRINGARRAYOBJECT
#define _H_SCPSTRINGARRAYOBJECT

#include "ScpObject.h"
class ScpStringArrayObject :
	public ScpObject
{
public:
	ScpStringArrayObject(void);
	~ScpStringArrayObject(void);

	virtual void Show(CScriptEngine * engine);
	virtual ScpObject * Clone(std::string strObjName);
	virtual std::string ToString();
	virtual void Release() ;
	virtual bool IsInnerFunction(std::string & functionname);
	virtual ScpObject * CallInnerFunction(std::string & functionname, VTPARAMETERS * parameters, CScriptEngine * engine);
	void Add(std::string strbody);
	void Clear();
	BOOL Del(unsigned int index);
	BOOL Del(std::string strbody);
	VTSTRINGS content;

};
//字符串数组对象的工厂函数
ScpObject * __stdcall ScpStringArrayObjectFactory(VTPARAMETERS* paramters, CScriptEngine* engine);

#endif //_H_SCPSTRINGARRAYOBJECT