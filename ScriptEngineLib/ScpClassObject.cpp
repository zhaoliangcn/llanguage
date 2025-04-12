/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
*/
#include "ScpClassObject.h"
#include "ScpStringObject.h"
#include "ScpGlobalObject.h"
#include "commanddefine_uni.h"
#include "ScriptEngine.h"
#include "ScpFunctionObject.h"
#include "../Common/commonutil.hpp"
ScpClassObject::ScpClassObject(void)
{
    // 设置对象类型为类对象
    objecttype = ObjClass;
    
    // 初始化用户类对象空间，设置空间类型为类空间
    UserClassObjectSpace.ObjectSpaceType = Space_Class;
    
    // 设置该对象空间属于当前类对象
    UserClassObjectSpace.belongto = this;
    
    // 设置成员变量的默认访问属性为私有
    MemberVariableAttribute = Attr_Private;

    // 绑定内部函数：将英文和中文的"show"命令都绑定到InnerFunction_Show函数
    BindObjectInnerFuction(scpcommand_en_show, InnerFunction_Show);
    BindObjectInnerFuction(scpcommand_cn_show, InnerFunction_Show);

    // 绑定内部函数：将英文和中文的"get"命令都绑定到InnerFunction_Get函数
    BindObjectInnerFuction(scpcommand_cn_get, InnerFunction_Get);
    BindObjectInnerFuction(scpcommand_en_get, InnerFunction_Get);
}

ScpClassObject::~ScpClassObject(void)
{
	
}
void ScpClassObject::Show(CScriptEngine * engine)
{
	for (unsigned int i = 0;i < ClassBody.size();i++)
	{
		engine->PrintError(ClassBody.at(i));
	}
	//auto it = memberattrmap.begin();
	//while (it != memberattrmap.end())
	//{
	//	std::string name = it->first;
	//	ScpObject * obj = engine->GetCurrentObjectSpace()->FindObject(name);
	//	if (obj)
	//	{
	//		obj->Show(engine);
	//	}
	//	it++;
	//}
}
ScpObject * ScpClassObject::Clone(std::string strObjName)
{
	return NULL;
}
std::string ScpClassObject::ToString()
{
	std::string temp;
	for (unsigned int i = 0;i < ClassBody.size();i++)
	{
		temp += ClassBody.at(i);
	}
	return temp;
}
void  ScpClassObject::Release()
{
	delete this;
}
bool ScpClassObject::IsInnerFunction(std::string & functionname)
{
	if (ObjectInnerFunctions.find(functionname) != ObjectInnerFunctions.end())
	{
		return true;
	}
	ScpObject * obj = FindMemberVariable(functionname);
	if (obj &&  obj->GetType()==ObjFunction)
	{
		return true;
	}
	return false;
}
ScpObject * ScpClassObject::CallInnerFunction(std::string & functionname, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	if (ObjectInnerFunctions.find(functionname) != ObjectInnerFunctions.end())
	{
		ObjectInnerFunction func = ObjectInnerFunctions[functionname];
		return func(this, parameters, engine);
	}
	if (FindMemberVariable(functionname))
	{
		VTPARAMETERS param;
		for (int i = 0;i < parameters->size();i++)
		{
			param.push_back(parameters->at(i));
		}
		return CallMemberFunction(functionname, parameters, engine);
	}
	return NULL;
}
void ScpClassObject::ClassDefine(std::string name)
{
	userclassname = name;
}
void  ScpClassObject::AddMemberVariable(std::string name, ScpObject * object)
{
	UserClassObjectSpace.AddObject(name, object);
}
void  ScpClassObject::AddMemberFunction(std::string name, ScpObject * object)
{
	UserClassObjectSpace.AddObject(name, object);
}
ScpObject * ScpClassObject::FindMemberVariable(std::string name)
{
	return UserClassObjectSpace.FindLocalObject(name);
}
ScpObject * ScpClassObject::InnerFunction_Show(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	thisObject->Show(engine);
	return thisObject;
}
ScpObject * ScpClassObject::InnerFunction_Get(ScpObject * thisObject, VTPARAMETERS * parameters, CScriptEngine * engine)
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
		if (parameters->at(0) == str_EN_ObjType || parameters->at(0) == str_CN_ObjType)
		{
			ScpStringObject * tname = new ScpStringObject;
			tname->content = ScpGlobalObject::GetInstance()->GetTypeName(thisObject->GetType());
			tname->istemp = true;
			return tname;
		}
	}
	return nullptr;
}
ScpObject * ScpClassObject::CallMemberFunction(std::string functionname, VTPARAMETERS * RealParameters, CScriptEngine * engine)
{
    // 在用户类对象空间中查找指定的函数对象
    ScpFunctionObject * func = (ScpFunctionObject*)UserClassObjectSpace.FindObject(functionname);
    
    // 如果找到函数对象
    if (func)
    {
        // 清空函数对象的实际参数列表
        func->RealParameters.clear();
        
        // 如果有传入的实际参数
        if (RealParameters)
        {
            // 将传入的参数复制到函数对象的参数列表中
            for (int i = 0;i < RealParameters->size();i++)
            {
                func->RealParameters.push_back(RealParameters->at(i));
            }
        }

        // 保存当前的对象空间和父空间
        ScpObjectSpace * space = engine->GetCurrentObjectSpace();
        ScpObjectSpace * oldparentspace = UserClassObjectSpace.parentspace;
        
        // 设置用户类对象空间的父空间为当前空间
        UserClassObjectSpace.parentspace = space;
        
        // 将引擎的当前对象空间设置为用户类对象空间
        engine->SetCurrentObjectSpace(&UserClassObjectSpace);
        
        // 执行函数
        func->Do(engine);
        
        // 恢复原来的父空间
        UserClassObjectSpace.parentspace = oldparentspace;
        
        // 恢复引擎的当前对象空间
        engine->SetCurrentObjectSpace(space);
        
        // 返回函数执行结果
        return func->Result;
    }
    // 如果未找到函数，返回NULL
    return NULL;
}
