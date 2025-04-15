/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
*/
#include "ScpArrayObject.h"
#include "ScpStringObject.h"
#include "ScpGlobalObject.h"
#include "ScpNullObject.h"
#include "ScpDoubleObject.h"
#include "ScriptEngine.h"
#include "ScpObjectNammes.h"
#include "commanddefine_uni.h"
#include "../Common/commonutil.hpp"

ScpArrayObject::ScpArrayObject(void) 
{
	objecttype = ObjArray;
	BindObjectInnerFuction(scpcommand_en_show, InnerFunction_Show);
	BindObjectInnerFuction(scpcommand_cn_show, InnerFunction_Show);

	BindObjectInnerFuction(scpcommand_cn_get, InnerFunction_Get);
	BindObjectInnerFuction(scpcommand_en_get, InnerFunction_Get);
}


ScpArrayObject::~ScpArrayObject(void)
{
	arrayobjects.Destroy();
}
ScpObject * ScpArrayObject::Clone(std::string strObjName)
{
	ScpArrayObject * obj = new ScpArrayObject;
	if (obj)
	{
		obj->arrayobjects.DeepCopy(&arrayobjects);
	}
	return obj;
}
void ScpArrayObject::Release() 
{
	delete this;
}
std::string ScpArrayObject::ToString()
{
	std::string temp;
	ULONG size = GetSize();
	for(ULONG i=0;i<size;i++)
	{
		ScpObject * element = (ScpObject *)GetElement(i);
		if(element)
		{
			temp+=element->ToString();
		}
		else
		{
			temp+=GetElementName(i);
		}
	}
	return temp;
}

ULONG ScpArrayObject::GetSize()
{
	return arrayobjects.GetSize();
}
BOOL ScpArrayObject::AddElement(std::string elementname,ScpObject * obj)
{
	return arrayobjects.MapObject(elementname,obj);
}
BOOL ScpArrayObject::AddElement(std::string elementname,ScpObjectSpace * objectSpace)
{

	ScpObject * obj = objectSpace->FindObject(elementname);
	if(obj)
	{
		if(obj!=this)
			return AddElement(elementname,obj);
	}

	return FALSE;
}
BOOL ScpArrayObject::SetElement(std::string elementname,ULONG elementindex,ScpObject * obj)
{
	if(Elementtype==ObjUnknown && Max!=0)
	{
		if(elementindex>=0 && elementindex<Max)
			return arrayobjects.SetObject(elementindex,elementname,obj);
	}
	return FALSE;
}
BOOL ScpArrayObject::SetElement(std::string elementname,ULONG elementindex,ScpObjectSpace * objectSpace)
{

	ScpObject * obj = objectSpace->FindObject(elementname);
	if(obj)
	{
		if(obj!=this)
			return SetElement(elementname,elementindex,obj);
	}

	return FALSE;
}
BOOL ScpArrayObject::DeleteElement(std::string elementname)
{
	if(arrayobjects.IsInMap(elementname))
	{
		arrayobjects.UnMapObject(elementname);
		return TRUE;
	}
	return FALSE;
}
BOOL ScpArrayObject::ReplaceElementObj(ScpObject * oldobj,ScpObject * obj)
{
	BOOL bret = FALSE;
	if(oldobj->GetType()==ObjNull)
	{
		std::string name = arrayobjects.GetObjectName(oldobj);
		ULONG elementindex = arrayobjects.GetObjectIndex(name);			
		bret = SetElement(name,elementindex,obj);
		delete oldobj;
	}	
	return bret;
}
BOOL ScpArrayObject::ReplaceElementObj(ULONG elementindex,ScpObject * obj)
{
	BOOL bret = FALSE;
	ScpObject * temp = arrayobjects.GetObject(elementindex);
	if(temp->GetType()==ObjNull)
	{
		bret = SetElement(arrayobjects.GetObjectName(elementindex),elementindex,obj);
		delete temp;
	}	
	return bret;
}
BOOL ScpArrayObject::ReplaceElementObj(std::string elementname,ScpObject * obj)
{
	BOOL bret = FALSE;
	ScpObject * temp = arrayobjects.GetObject(elementname);
	if(temp->GetType()==ObjNull)
	{
		ULONG elementindex = arrayobjects.GetObjectIndex(elementname);		
		bret = SetElement(elementname,elementindex,obj);
		delete temp;
	}	
	return bret;

}
ScpObject * ScpArrayObject::GetElement(std::string elementname)
{
	if(arrayobjects.IsInMap(elementname))
	{
		return arrayobjects.GetObject(elementname);
	}
	return NULL;
}
ScpObjectType ScpArrayObject::GetElementType(std::string elementname)
{
	return arrayobjects.GetType(elementname);
}
BOOL ScpArrayObject::DeleteElement(ULONG elementindex)
{
	return FALSE;
}
ScpObject * ScpArrayObject::GetElement(ULONG elementindex)
{

	if(elementindex<Max)
		return arrayobjects.GetObject(elementindex);
	return NULL;
}
ScpObjectType ScpArrayObject::GetElementType(ULONG elementindex)
{
	return ObjUnknown;
}
std::string ScpArrayObject::GetElementName(ULONG elementindex)
{
	std::string temp;
	temp = arrayobjects.GetObjectName(elementindex);
	return temp;
	
}
ScpObject * ScpArrayObject::InnerFunction_Show(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	thisObject->Show(engine);
	return thisObject;

}
ScpObject * ScpArrayObject::InnerFunction_Get(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
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
		if (parameters->at(0) == str_EN_ObjType || 
			parameters->at(0) == str_CN_ObjType)
		{
			ScpStringObject * tname = new ScpStringObject;
			tname->content = ScpGlobalObject::GetInstance()->GetTypeName(thisObject->GetType());
			tname->istemp = true;
			return tname;
		}
	}
	return nullptr;
}
void ScpArrayObject::Show(CScriptEngine * engine)
{
	ULONG size = GetSize();
	for(ULONG i=0;i<size;i++)
	{
		ScpObject * element = (ScpObject *)GetElement(i);
		if(element)
		{
			element->Show(engine);
		}
		else
		{
			engine->PrintError(GetElementName(i));
		}
	}
}
bool ScpArrayObject::IsInnerFunction(std::string & functionname)
{
	if (ObjectInnerFunctions.find(functionname) != ObjectInnerFunctions.end())
	{
		return true;
	}
	return false;
}
ScpObject * ScpArrayObject::CallInnerFunction(std::string & functionname,VTPARAMETERS * parameters,CScriptEngine * engine)
{
	if (ObjectInnerFunctions.find(functionname) != ObjectInnerFunctions.end())
	{
		ObjectInnerFunction func = ObjectInnerFunctions[functionname];
		return func(this, parameters, engine);
	}
	return NULL;
}

ScpObject * __stdcall ScpArrayObjectFactory(VTPARAMETERS * paramters, CScriptEngine * engine)
{
	std::string arrayobjname = paramters->at(1);
	if (paramters->size() == 2)
	{
		const size_t pos1 = arrayobjname.find(scpLeftBracket);
		const size_t pos2 = arrayobjname.find(scpRightBracket);
		if ((pos1 != std::string::npos) && (pos2 != std::string::npos))
		{
			std::string arrayname = arrayobjname.substr(0, pos1);
			std::string arraylen = arrayobjname.substr(pos1 + 1, pos2 - pos1 - 1);
			if (IsStaticNumber(arraylen))
			{
				ScpArrayObject * obj = (ScpArrayObject *)engine->GetCurrentObjectSpace()->FindObject(arrayname);
				if (!obj)
				{
					obj = new ScpArrayObject;
					if (obj)
					{
						obj->arrayname = arrayname;
						obj->Max = StringToInt(arraylen.c_str());
						obj->Elementtype = ObjUnknown;
						for (unsigned int i = 0;i < obj->Max;i++)
						{
							std::string elementname = STDSTRINGEXT::Format("nullobj%d", i);
							ScpNullObject * nullobj = new ScpNullObject;
							nullobj->parentObj = obj;
							obj->AddElement(elementname, nullobj);
						}
						return obj;
					}
				}
			}
		}
	}
	else if (paramters->size() > 2)
	{
		ScpArrayObject * obj = (ScpArrayObject *)engine->GetCurrentObjectSpace()->FindObject(arrayobjname);
		if (!obj)
		{
			obj = new ScpArrayObject;
			if (obj)
			{
				obj->arrayname = arrayobjname;
				for (ULONG i = 2;i < paramters->size();i++)
				{
					std::string elementname = paramters->at(i);
					ScpObject * tempobj = engine->GetCurrentObjectSpace()->FindObject(elementname);
					if (tempobj)
					{
						if (obj->GetSize() == 0)
						{
							obj->Elementtype = tempobj->GetType();
						}
						else
						{
							if (obj->Elementtype != tempobj->GetType())
							{
								engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidArrayElementType);
								break;
							}
						}
						obj->AddElement(elementname, tempobj);
					}
					else
					{
						std::string elementtype;
						ScpObject * elementobj;
						if (IsStaticNumber(elementname))
						{
							elementtype = ScpObjectNames::GetSingleInsatnce()->strObjStaticInt;
							elementobj = new ScpIntObject;
							((ScpIntObject*)elementobj)->value = StringToInt(elementname);
						}
						else if (IsStaticDoubleNumber(elementname))
						{
							elementtype = ScpObjectNames::GetSingleInsatnce()->strObjStaticDouble;
							elementobj = new ScpDoubleObject;
							((ScpDoubleObject*)elementobj)->value = StringToDouble(elementname);
						}
						else
						{
							elementtype = ScpObjectNames::GetSingleInsatnce()->strObjStaticString;
							elementobj = new ScpStringObject;
							((ScpStringObject*)elementobj)->content = elementname;
						}
						const ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(elementtype.c_str());
						if (obj->GetSize() == 0)
						{
							obj->Elementtype = type;
						}
						else
						{
							if (obj->Elementtype != type)
							{
								if (obj->Elementtype == ObjStaticString)
								{
									ScpStringObject * temp = new ScpStringObject;
									temp->content = elementobj->ToString();
									delete elementobj;
									elementobj = (ScpObject *)temp;
								}
								else
								{
									engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidArrayElementType);
									break;
								}
							}
						}
						obj->AddElement(elementname, (ScpObject *)elementobj);
					}
				}
				obj->Max = obj->GetSize();
				return obj;
			}
		}
		else
		{
			engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjExist);
		}
	}
	return NULL;
}
