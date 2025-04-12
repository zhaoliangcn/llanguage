/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
*/
#include "ScpIntObject.h"
#include "ScpStringObject.h"
#include "ScpGlobalObject.h"
#include "../Common/commonutil.hpp"
#include "ScpDoubleObject.h"
#include "ScriptEngine.h"
#include "commanddefine_uni.h"
#include <math.h>
ScpIntObject::ScpIntObject()
{
	value = 0;
	objecttype = ObjInt;
	BindObjectInnerFuction(scpcommand_en_show, InnerFunction_Show);
	BindObjectInnerFuction(scpcommand_cn_show, InnerFunction_Show);

	BindObjectInnerFuction(scpcommand_cn_get, InnerFunction_Get);
	BindObjectInnerFuction(scpcommand_en_get, InnerFunction_Get);

	BindObjectInnerFuction(scpcommand_cn_transform, InnerFunction_transform);
	BindObjectInnerFuction(scpcommand_en_transform, InnerFunction_transform);

	BindObjectInnerFuction(scpcommand_square_CN, InnerFunction_square);
	BindObjectInnerFuction(scpcommand_square_EN, InnerFunction_square);

	BindObjectInnerFuction(scpcommand_squareroot_CN, InnerFunction_squareroot);
	BindObjectInnerFuction(scpcommand_squareroot_EN, InnerFunction_squareroot);


	BindObjectInnerFuction(scpcommand_cubic_CN, InnerFunction_cubic);
	BindObjectInnerFuction(scpcommand_cubic_EN, InnerFunction_cubic);

	BindObjectInnerFuction(scpcommand_cuberoot_CN, InnerFunction_cuberoot);
	BindObjectInnerFuction(scpcommand_cuberoot_EN, InnerFunction_cuberoot);

	BindObjectInnerFuction(scpcommand_abs_EN, InnerFunction_abs);
	BindObjectInnerFuction(scpcommand_abs_CN, InnerFunction_abs);

	BindObjectInnerFuction(scpcommand_acos_CN, InnerFunction_acos);
	BindObjectInnerFuction(scpcommand_acos_EN, InnerFunction_acos);


	BindObjectInnerFuction(scpcommand_cos_CN, InnerFunction_cos);
	BindObjectInnerFuction(scpcommand_cos_EN, InnerFunction_cos);


	BindObjectInnerFuction(scpcommand_asin_CN, InnerFunction_asin);
	BindObjectInnerFuction(scpcommand_asin_EN, InnerFunction_asin);

	BindObjectInnerFuction(scpcommand_sin_CN, InnerFunction_sin);
	BindObjectInnerFuction(scpcommand_sin_EN, InnerFunction_sin);

	BindObjectInnerFuction(scpcommand_atan_CN, InnerFunction_atan);
	BindObjectInnerFuction(scpcommand_atan_EN, InnerFunction_atan);

	BindObjectInnerFuction(scpcommand_tan_CN, InnerFunction_tan);
	BindObjectInnerFuction(scpcommand_tan_EN, InnerFunction_tan);

	BindObjectInnerFuction(scpcommand_pow_CN, InnerFunction_pow);
	BindObjectInnerFuction(scpcommand_pow_EN, InnerFunction_pow);

	BindObjectInnerFuction(scpcommand_add_CN, InnerFunction_add);
	BindObjectInnerFuction(scpcommand_add_EN, InnerFunction_add);

	BindObjectInnerFuction(scpcommand_sub_CN, InnerFunction_sub);
	BindObjectInnerFuction(scpcommand_sub_EN, InnerFunction_sub);
	

}
ScpIntObject::~ScpIntObject()
{
}
ScpObject * ScpIntObject::Clone(std::string strObjName)
{
	ScpObject * tmpclone=new ScpIntObject ;
	((ScpIntObject*)tmpclone)->value = this->value;
	return tmpclone;
}	
std::string ScpIntObject::ToString()
{
	return std::to_string(value);
}
void ScpIntObject::Release() 
{
	delete this;
}
bool ScpIntObject::IsInnerFunction(std::string & functionname)
{
	if (ObjectInnerFunctions.find(functionname) != ObjectInnerFunctions.end())
	{
		return true;
	}
	return false;
}
ScpObject * ScpIntObject::CallInnerFunction(std::string & functionname,VTPARAMETERS * parameters,CScriptEngine * engine)
{
	if (ObjectInnerFunctions.find(functionname) != ObjectInnerFunctions.end())
	{
		ObjectInnerFunction func = ObjectInnerFunctions[functionname];
		return func(this, parameters, engine);
	}
	return NULL;
}
ScpObject * ScpIntObject::InnerFunction_Show(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	thisObject->Show(engine);
	return thisObject;
}
ScpObject * ScpIntObject::InnerFunction_Get(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
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
		if (param0 == str_EN_ObjType || param0 == str_CN_ObjType)
		{
			ScpStringObject * tname = (ScpStringObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjString);
			tname->content = ScpGlobalObject::GetInstance()->GetTypeName(thisObject->GetType());
			tname->istemp = true;
			return tname;
		}
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_transform(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string param0 = parameters->at(0);
		StringStripQuote(param0);
		ScpObject * objparam0 = engine->GetCurrentObjectSpace()->FindObject(param0);
		if (objparam0 && objparam0->GetType() == ObjString)
		{
			if (((ScpStringObject *)objparam0)->content == ScpGlobalObject::GetInstance()->GetTypeName(ObjString))
			{
				ScpStringObject * tname = (ScpStringObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjString);
				tname->content = thisObject->ToString();
				tname->istemp = true;
				return tname;
			}
		}
		if (param0 == ScpGlobalObject::GetInstance()->GetTypeName(ObjString))
		{
			ScpStringObject * tname = (ScpStringObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjString);
			tname->content = thisObject->ToString();
			tname->istemp = true;
			return tname;
		}
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_square(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpIntObject *tint = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
		tint->value = ((ScpIntObject *)thisObject)->value*((ScpIntObject *)thisObject)->value;
		tint->istemp = true;
		return tint;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_squareroot(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpDoubleObject *td = (ScpDoubleObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjDouble);
		td->value = sqrt((double)((ScpIntObject *)thisObject)->value);
		td->istemp = true;
		return td;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_cubic(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpIntObject *tint = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
		tint->value = ((ScpIntObject *)thisObject)->value*((ScpIntObject *)thisObject)->value*((ScpIntObject *)thisObject)->value;
		tint->istemp = true;
		return tint;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_cuberoot(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpDoubleObject *td = (ScpDoubleObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjDouble);
		td->value = pow(((ScpIntObject *)thisObject)->value, 1.0 / 3);
		td->istemp = true;
		return td;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_abs(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpIntObject *tint = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
		tint->value = abs(((ScpIntObject *)thisObject)->value);
		tint->istemp = true;
		return tint;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_acos(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpDoubleObject *td = (ScpDoubleObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjDouble);
		td->value = acos((double)((ScpIntObject *)thisObject)->value);
		td->istemp = true;
		return td;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_cos(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpDoubleObject *td = (ScpDoubleObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjDouble);
		td->value = cos((double)((ScpIntObject *)thisObject)->value);
		td->istemp = true;
		return td;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_asin(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpDoubleObject *td = (ScpDoubleObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjDouble);
		td->value = asin((double)((ScpIntObject *)thisObject)->value);
		td->istemp = true;
		return td;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_sin(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpDoubleObject *td = (ScpDoubleObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjDouble);
		td->value = sin((double)((ScpIntObject *)thisObject)->value);
		td->istemp = true;
		return td;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_atan(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpDoubleObject *td = (ScpDoubleObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjDouble);
		td->value = atan((double)((ScpIntObject *)thisObject)->value);
		td->istemp = true;
		return td;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_tan(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 0)
	{
		ScpDoubleObject *td = (ScpDoubleObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjDouble);
		td->value = tan((double)((ScpIntObject *)thisObject)->value);
		td->istemp = true;
		return td;
	}
	return nullptr;
}
ScpObject * ScpIntObject::InnerFunction_pow(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (parameters->size() == 1)
	{
		std::string param0 = parameters->at(0);
		StringStripQuote(param0);
		int c = 0;
		ScpObject * objparam0 = engine->GetCurrentObjectSpace()->FindObject(param0);
		if (objparam0 && objparam0->GetType() == ObjInt)
		{
			c = ((ScpIntObject *)objparam0)->value;			
		}
		else if (IsStaticNumber(param0))
		{
			c = StringToInt(param0);
		}
		else
		{
			return nullptr;
		}
		ScpIntObject *tint = (ScpIntObject *)engine->GetCurrentObjectSpace()->AcquireTempObject(ObjInt);
		if (tint)
		{
			const int ret = ((ScpIntObject*)thisObject)->value;
			tint->value = pow(ret, c);
			tint->istemp = true;
			return tint;
		}

	}
	return nullptr;
}
ScpObject* ScpIntObject::InnerFunction_add(ScpObject* thisObject, VTPARAMETERS* parameters, CScriptEngine* engine)
{
	if (parameters->size() == 1)
	{
		std::string param0 = parameters->at(0);
		StringStripQuote(param0);
		int c = 0;
		ScpObject* objparam0 = engine->GetCurrentObjectSpace()->FindObject(param0);
		if (objparam0 && objparam0->GetType() == ObjInt)
		{
			c = ((ScpIntObject*)objparam0)->value;
		}
		else if (IsStaticNumber(param0))
		{
			c = StringToInt(param0);
		}
		else
		{
			return nullptr;
		}
		((ScpIntObject*)thisObject)->value += c;
	}
	return thisObject;
}
ScpObject* ScpIntObject::InnerFunction_sub(ScpObject* thisObject, VTPARAMETERS* parameters, CScriptEngine* engine)
{
	if (parameters->size() == 1)
	{
		std::string param0 = parameters->at(0);
		StringStripQuote(param0);
		int c = 0;
		ScpObject* objparam0 = engine->GetCurrentObjectSpace()->FindObject(param0);
		if (objparam0 && objparam0->GetType() == ObjInt)
		{
			c = ((ScpIntObject*)objparam0)->value;
		}
		else if (IsStaticNumber(param0))
		{
			c = StringToInt(param0);
		}
		else
		{
			return nullptr;
		}
		((ScpIntObject*)thisObject)->value -= c;
	}
	return thisObject;
}
void ScpIntObject::Show(CScriptEngine * engine)
{
	std::string text;
	text=STDSTRINGEXT::Format("%d",value);
	engine->PrintError(text);
}

ScpObject * __stdcall ScpIntObjectFactory(VTPARAMETERS * paramters, CScriptEngine * engine)
{
	if (paramters->size() >= 2)
	{
		const std::string &strobj = paramters->at(0);
		const std::string &userobjname = paramters->at(1);
		std::string content;
		if (paramters->size() == 3)
			content = paramters->at(2);
		ScpIntObject *obj = new ScpIntObject;
		if (obj)
		{
			if (IsStaticNumber(content))
			{
				obj->value = StringToInt(content.c_str());
			}
			else
			{
				obj->value = 0;
			}
			return obj;
		}
	}
	return NULL;
}
