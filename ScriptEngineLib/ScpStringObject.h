/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
//字符串对象
*/
#ifndef _H_SCPSTRINGOBJECT
#define _H_SCPSTRINGOBJECT
#include "ScpObject.h"
#include <string>




const static char  * encodetypebase64 = "base64";
const static char  * encodetypeurl = "ur";
const static char  * str_CN_Length = "长度";
const static char  * str_EN_Length = "length";

class ScpStringObject:public ScpObject
{
public:

	typedef enum ScpStringCodeType
	{
		scpStringCodeTypeUNICODE,
		scpStringCodeTypeANSI,
		scpStringCodeTypeUTF8,
		scpStringCodeTypeUTF16,
		scpStringCodeTypeUTF32,
	}ScpStringCodeType;

	ScpStringObject();
	~ScpStringObject();

	virtual ScpObject * Clone(std::string strObjName);
	virtual std::string ToString();
	virtual void Release();
	virtual bool IsInnerFunction(std::string & functionname);
	virtual ScpObject * CallInnerFunction(std::string & functionname,VTPARAMETERS * parameters,CScriptEngine * engine);
	virtual void Show(CScriptEngine * engine);

	static ScpStringObject * Reverse(ScpStringObject * str1);
	static ScpStringObject * ToUpper(ScpStringObject * str1);
	static ScpStringObject * ToLower(ScpStringObject * str1);
	static ScpStringObject * ToHex(ScpStringObject * str1);
	static ScpStringObject * SubStr(ScpStringObject * org,ScpStringObject * sub,ULONG start,ULONG length);	
	template<typename... Args>
    static ScpStringObject* Format(ScpStringObject* str1, const char* strForm, Args&&... args);
	//static ScpStringObject*  Format(ScpStringObject * str1,const char  * strForm,VTPARAMETERS &parameters);
	static ScpStringObject* Connect(ScpStringObject* str1, ScpStringObject* str2, ScpStringObject* result);

	ScpStringObject* SetString(const char * str);
	
	ScpStringObject* SubStr(ULONG start, ULONG length, CScriptEngine* engine);
	ScpStringObject* SubStr(ULONG start, CScriptEngine* engine);
	void Clear();
	ULONG GetSize();
	ULONG GetLength();
	ScpStringCodeType StringCodeType;
	std::string content;
	std::string objname;


	
	static ScpObject * InnerFunction_Show(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_Get(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_transform(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_getsize(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_clear(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_getsubstring(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	//static ScpObject * InnerFunction_hash(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_format(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_replace(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_find(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_rfind(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_contains(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_insert(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	//static ScpObject * InnerFunction_encode(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	//static ScpObject * InnerFunction_decode(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_split(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	//static ScpObject * InnerFunction_encrypt(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	//static ScpObject * InnerFunction_decrypt(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_compare(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_connect(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);
	static ScpObject * InnerFunction_Length(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine);



};



ScpObject * __stdcall ScpStringObjectFactory(VTPARAMETERS * paramters, CScriptEngine * engine);

#endif //_H_SCPSTRINGOBJECT