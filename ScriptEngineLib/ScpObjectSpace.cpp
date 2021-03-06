/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
//对象作用域空间管理，用于实现名字空间的隔离
*/
#include "ScpObjectSpace.h"
#include "ScpTableObject.h"
#include "ScpArrayObject.h"
#include "ScpClassObject.h"
#include "ScpClassInstanceObject.h"
#include "ScpStructObject.h"
#include "ScpObjectFactory.h"
#include "ScpCommonObject.h"
#include "../Common/stdstringext.hpp"
ScpObjectSpace::ScpObjectSpace(void):breakingout(0),continuewhile(0),parentspace(NULL),enddepths(0),lastTestResult(-1),LastTempObjAppendix(0),lastcommand(0),LastTempObjectName(L"@@tempobj")
{
	breakingout = 0  ;
	continuewhile =  0 ;
	parentspace = NULL;
	LastTempObjectName = L"@@tempobj";
	enddepths = 0;
	lastTestResult =-1;
	LastTempObjAppendix =0;
	lastcommand = 0;
}

ScpObjectSpace::~ScpObjectSpace(void)
{
	DestroyTempObjects();
}
ScpObject * ScpObjectSpace::FindStructMemberVariable(const std::wstring& objname)
{
	size_t pos = objname.find(scpOperationObjectRefrence);
	if(pos!=std::wstring::npos)
	{
		std::wstring parent;
		std::wstring children;
		parent=objname.substr(0,pos);
		children=objname.substr(pos+1,objname.length()-pos-1);
		ScpObject* parentobj = FindObject(parent);
		if(parentobj)
		{
			if(parentobj->GetType()==ObjStructInstance)
			{
				ScpStructInstanceObject* structobj=(ScpStructInstanceObject* )parentobj;
				if(structobj)
				{
					ScpObject * childobj = structobj->StructObjectSpace.FindLocalObject(children);
					return childobj;
				}
			}
		}
	}
	return NULL;
}
ScpObject * ScpObjectSpace::FindClassOrStructMemberVariable(const std::wstring & objname)
{
	size_t pos = objname.find(scpOperationObjectRefrence);
	if (pos != std::wstring::npos)
	{
		std::wstring parent;
		std::wstring children;
		parent = objname.substr(0, pos);
		children = objname.substr(pos + 1, objname.length() - pos - 1);
		ScpObject* parentobj = FindObject(parent);
		if (parentobj)
		{
			if (parentobj->GetType() == ObjClassInstance)
			{
				ScpClassObject* classobj = (ScpClassObject*)parentobj;
				if (classobj)
				{
					ScpObject * childobj = classobj->UserClassObjectSpace.FindLocalObject(children);
					if (childobj)
					{
						if (classobj->memberattrmap[children] == ScpClassObject::Attr_Public)
						{
							return childobj;
						}
						else
						{
							return NULL;
						}
					}
				}
			}
			else
			{
				if (parentobj->GetType() == ObjStructInstance)
				{
					ScpStructInstanceObject* structobj = (ScpStructInstanceObject*)parentobj;
					if (structobj)
					{
						ScpObject * childobj = structobj->StructObjectSpace.FindLocalObject(children);
						return childobj;
					}
				}
			}
		}
	}
	return NULL;
}
ScpObject * ScpObjectSpace::FindClassMemberVariable(const std::wstring& objname)
{
	//size_t pos = objname.find(scpColon);
	//if(pos==std::wstring::npos)
	//{
	//	pos=objname.find(ScpObjectNames::GetSingleInsatnce()->strScpColoncn);
	//}
	//if(pos==std::wstring::npos)
	//{
	//	pos = objname.find(scpOperationObjectRefrence);
	//}
	size_t pos = objname.find(scpOperationObjectRefrence);
	if(pos!=std::wstring::npos)
	{
		std::wstring parent;
		std::wstring children;
		parent=objname.substr(0,pos);
		children=objname.substr(pos+1,objname.length()-pos-1);
		ScpObject* parentobj = FindObject(parent);
		if(parentobj)
		{
			if(parentobj->GetType()==ObjClassInstance)
			{
				ScpClassObject* classobj=(ScpClassObject* )parentobj;
				if(classobj)
				{
					ScpObject * childobj = classobj->UserClassObjectSpace.FindLocalObject(children);
					if(childobj)
					{
						if(classobj->memberattrmap[children]==ScpClassObject::Attr_Public)
						{
							return childobj;
						}
						else
						{
							return NULL;
						}						
					}
				}
			}
		}
	}
	return NULL;
}
ScpObject * ScpObjectSpace::FindTableElement(const std::wstring& objname)
{
	size_t pos1 = objname.find(scpLeftBracket);
	if((pos1!=std::wstring::npos))
	{
		size_t pos2=objname.length()-1;
		if(scpRightBracket==objname.substr(pos2,1))
		{			
			std::wstring name = objname.substr(0,pos1);
			std::wstring element = objname.substr(pos1+1,pos2-pos1-1);			
			if(element.substr(element.length()-1,1)==scpRightBracket)
			{
				return FindTableElement(element);
			}
			else
			{
				if(IsStaticNumber(element))
				{
					int Index = StringToInt(element.c_str());
					ScpObject* tableobj=FindObject(name);
					if(tableobj)
					{
						if(tableobj->GetType()==ObjTable)
						{
							ScpObject * childobj = ((ScpTableObject* )tableobj)->GetElement(Index);
							if(childobj)
							{
								return childobj;
							}
						}
					}
				}	
				else
				{
					ScpObject* tableobj=FindObject(name);
					if(tableobj)
					{
						if(tableobj->GetType()==ObjTable)
						{							
							ScpObject * childobj = ((ScpTableObject* )tableobj)->GetElement(element);
							if(childobj)
							{
								return childobj;
							}
							ScpObject * obj = FindObject(element);
							if(obj && obj->GetType()==ObjInt)
							{
								int Index =((ScpIntObject *)obj)->value;
								return ((ScpTableObject* )tableobj)->GetElement(Index);
							}
						}
					}
				}
			}
		}
	}	
	return NULL;
}
ScpObject * ScpObjectSpace::FindArrayElement(const std::wstring& objname)
{
	size_t pos1 = objname.find(scpLeftBracket);
	if((pos1!=std::wstring::npos))
	{
		size_t pos2=objname.length()-1;
		if(scpRightBracket==objname.substr(pos2,1))
		{
			std::wstring arrayname = objname.substr(0,pos1);
			std::wstring arrayelement = objname.substr(pos1+1,pos2-pos1-1);
			if(arrayelement.substr(arrayelement.length()-1,1)==scpRightBracket)
			{
				return FindArrayElement(arrayelement);
			}
			else
			{
				if(IsStaticNumber(arrayelement))
				{
					int Index = StringToInt(arrayelement.c_str());
					ScpObject * arrayobj = FindObject(arrayname);
					if(arrayobj)
					{
						if(arrayobj->GetType()==ObjArray)
						{
							ScpObject * childobj = ((ScpArrayObject *)arrayobj)->GetElement(Index);
							if(childobj)
							{
								return childobj;
							}
						}
					}
				}			
				else
				{
					ScpObject * arrayobj = FindObject(arrayname);
					if(arrayobj)
					{
						if(arrayobj->GetType()==ObjArray)
						{							
							ScpObject * childobj = ((ScpArrayObject *)arrayobj)->GetElement(arrayelement);
							if(childobj)
							{
								return childobj;
							}
							ScpObject * obj = FindObject(arrayelement);
							if(obj && obj->GetType()==ObjInt)
							{
								int Index =((ScpIntObject *)obj)->value;
								return ((ScpArrayObject *)arrayobj)->GetElement(Index);
							}
						}
					}
				}
			}
		}
	}	
	return NULL;
}
ScpObject * ScpObjectSpace::FindLocalObject(const std::wstring& objname)
{
	ScpObject* temp =NULL;
	/*size_t pos1 = objname.rfind(scpRightBracket);
	if(pos1==(objname.length()-1))
	{
		temp=FindTableElement(objname);
		if(temp)
		{
			return temp;
		}
		temp = FindArrayElement(objname);
		if(temp)
		{
			return temp;
		}		
	}*/
	//temp = FindClassMemberVariable(objname);
	//if(temp)
	//{
	//	return temp;
	//}
	//temp = FindStructMemberVariable(objname);
	//if(temp)
	//{
	//	return temp;
	//}
	//temp = FindClassOrStructMemberVariable(objname);
	//if (temp)
	//{
	//	return temp;
	//}
	temp = userobject.GetObject(objname);
	return temp;
}
ScpObject * ScpObjectSpace::FindObject(IN const std::wstring& objname)
{
	ScpObject* temp =NULL;
	//size_t pos1 = objname.rfind(scpRightBracket);
	//if(pos1==(objname.length()-1))
	//{
	//	temp=FindTableElement(objname);
	//	if(temp)
	//	{
	//		return temp;
	//	}
	//	temp = FindArrayElement(objname);
	//	if(temp)
	//	{
	//		return temp;
	//	}		
	//}
	//temp = FindClassMemberVariable(objname);
	//if(temp)
	//{
	//	return temp;
	//}
	//temp = FindStructMemberVariable(objname);
	//if(temp)
	//{
	//	return temp;
	//}
	//temp = FindClassOrStructMemberVariable(objname);
	//if (temp)
	//{
	//	return temp;
	//}
	temp=userobject.GetObject(objname);
	if(temp)
	{			
		return temp;
	}
	else
	{
		if(parentspace)
		{
			return parentspace->FindObject(objname);
		}
		else
		{
			return NULL;
		}
	}		
}
ScpObject * ScpObjectSpace::FindGlobalObject(const std::wstring & objname)
{
	if (parentspace)
	{
		return parentspace->FindGlobalObject(objname);
	}
	else
	{
		return FindObject(objname);
	}
}
std::wstring ScpObjectSpace::GetObjectSpace(IN const std::wstring objname)
{
	ScpObject* temp=userobject.GetObject(objname);
	if(temp)
	{
		return spacename;
	}
	else
	{
		if(parentspace)
		{
			return parentspace->GetObjectSpace(objname);
		}
		else
		{
			return L"";
		}
	}
}
BOOL ScpObjectSpace::AddObject(std::wstring strObjname,ScpObject *obj,std::wstring scope)
{
	if(ObjectSpaceType==Space_Class)
	{
		ScpClassObject * classObj =(ScpClassObject * )belongto;
		if(classObj)
		{
			classObj->memberattrmap[strObjname]=classObj->MemberVariableAttribute;
		}
	}
	//被映射的对象不能时临时对象
	obj->istemp = false;
	//if (obj->GetType() == ObjExpressionNode)
	//{
	//	if (obj->RefCount >=1)
	//	{
	//		obj->RefCount--;
	//	}
	//	//printf("node %S refcount %d %0x\n", strObjname.c_str(), obj->RefCount, obj);
	//}
	return userobject.MapObject(strObjname,obj,scope);
}
void ScpObjectSpace::EraseObject(std::wstring strObjname)
{
	userobject.UnMapObject(strObjname);
}
void ScpObjectSpace::EraseObject(ScpObject * Obj)
{
    std::wstring strObjname = userobject.GetObjectName(Obj);
	if (strObjname != L"")
	{
		userobject.UnMapObject(strObjname);
	}
}
ScpObjectType ScpObjectSpace::GetType(std::wstring strObjname)
{
	ScpObject * temp = FindObject(strObjname);
	if(temp)
		return temp->GetType();
	else
		return ObjUnknown;
}
std::wstring ScpObjectSpace::GetObjectName(ScpObject * obj)
{
	return userobject.GetObjectName(obj);
}
std::wstring ScpObjectSpace::GetObjectNameR(ScpObject * obj)
{
	std::wstring name = userobject.GetObjectName(obj);
	if (name.empty())
	{
		ScpObjectSpace * pobjectSpace = parentspace;
		while (pobjectSpace)
		{
			name = pobjectSpace->GetObjectName(obj);
			if (!name.empty())
			{
				break;
			}
			pobjectSpace = pobjectSpace->parentspace;
		}
	}
	return name;
}
ScpObject * ScpObjectSpace::AcquireTempObject(ScpObjectType type)
{
	ScpObject * tmp = NULL;
	if (type == ObjInt)
	{
		tmp = AcquireTempObjectUni(UnusedTemp_Int_Objects);		
	}
	else if (type == ObjDouble)
	{
		tmp = AcquireTempObjectUni(UnusedTemp_Double_Objects);
	}
	else if (type == ObjBigInt)
	{
		tmp = AcquireTempObjectUni(UnusedTemp_BigInt_Objects);
	}
	else if (type == ObjString)
	{
		tmp = AcquireTempObjectUni(UnusedTemp_String_Objects);
	}
	else if (type ==  ObjExpressionNode)
	{
		tmp = AcquireTempObjectUni(UnusedTemp_ExpressionNode_Objects);
	}
	else
	{
		ITPOBJECTS it = UnusedTempObjects.begin();
		while (it != UnusedTempObjects.end())
		{
			tmp = *it;
			if (tmp->RefCount < 0)
			{
				it = UnusedTempObjects.erase(it);
				continue;
			}
			if (tmp->GetType() == type)
			{
				UnusedTempObjects.erase(it);
				tmp->istemp = true;
				tmp->AddRef();
				return tmp;
			}
			it++;
		}
	}
	if (!tmp)
	{
		tmp = BaseObjectFactory(type);
		tmp->istemp = true;
		tmp->AddRef();
	}	
	return tmp;
}
void ScpObjectSpace::ReleaseTempObject(ScpObject * tempobj)
{
	if (tempobj->istemp)
	{
		//std::wstring name = GetObjectName(tempobj);
		//if (!name.empty())
		//{
		//	//说明已经被映射到了对象名字空间，这里不能放进临时对象缓冲区
		//	return;
		//}
		if (tempobj->RefCount > 0)
		{
			tempobj->RefCount--;
		}
		if(tempobj->RefCount==0)
		{			
			if (tempobj->GetType() == ObjInt)
			{
				ReleaseTempObjectUni(tempobj, UnusedTemp_Int_Objects);
			}
			else if (tempobj->GetType() == ObjDouble)
			{
				ReleaseTempObjectUni(tempobj, UnusedTemp_Double_Objects);
			}
			else if (tempobj->GetType() == ObjBigInt)
			{
				ReleaseTempObjectUni(tempobj, UnusedTemp_BigInt_Objects);
			}
			else if (tempobj->GetType() == ObjString)
			{
				ReleaseTempObjectUni(tempobj, UnusedTemp_String_Objects);
			}
			else if (tempobj->GetType() == ObjExpressionNode)
			{
				ReleaseTempObjectUni(tempobj, UnusedTemp_ExpressionNode_Objects);
			}
			else
			{
				ITPOBJECTS result = find(UnusedTempObjects.begin(), UnusedTempObjects.end(), tempobj);
				if (result == UnusedTempObjects.end()) //没找到
				{					
					tempobj->RefCount = 0;
					//UnusedTempObjects.push_back(tempobj);
					UnusedTempObjects.insert(UnusedTempObjects.begin(), tempobj);

				}
			}			
		}		
	}
}
void ScpObjectSpace::DestroyTempObjects()
{	
	DestroyTempObjectsUni(UnusedTempObjects);
	DestroyTempObjectsUni(UnusedTemp_Int_Objects);	
	DestroyTempObjectsUni(UnusedTemp_Double_Objects);
	DestroyTempObjectsUni(UnusedTemp_BigInt_Objects);
	DestroyTempObjectsUni(UnusedTemp_String_Objects);
	DestroyTempObjectsUni(UnusedTemp_ExpressionNode_Objects);
}
ScpObject * ScpObjectSpace::AcquireTempObjectUni(VTPOBJECTS & TempObjects)
{
	ScpObject * tmp = NULL;
	ITPOBJECTS it = TempObjects.begin();
	while (it != TempObjects.end())
	{
		tmp = *it;
		if (tmp->RefCount < 0)
		{
			it = TempObjects.erase(it);
			continue;
		}
		else
		{
			TempObjects.erase(it);
			tmp->istemp = true;
			tmp->AddRef();
			return tmp;
		}
		it++;
	}
	return tmp;
}
void ScpObjectSpace::ReleaseTempObjectUni(ScpObject * tempobj, VTPOBJECTS & TempObjects)
{
	ITPOBJECTS result = find(TempObjects.begin(), TempObjects.end(), tempobj);
	if (result == TempObjects.end()) //没找到
	{		
		if (tempobj->GetType() == ObjInt)
		{
			((ScpIntObject *)tempobj)->value = 0;
		}
		else if (tempobj->GetType() == ObjDouble)
		{
			((ScpDoubleObject *)tempobj)->value = 0;
		}
		else if (tempobj->GetType() == ObjBigInt)
		{
			((ScpBigIntObject *)tempobj)->value = 0;
		}
		else if (tempobj->GetType() == ObjString)
		{
			((ScpStringObject *)tempobj)->Clear();
		}
		tempobj->RefCount = 0;
		TempObjects.insert(TempObjects.begin(), tempobj);
	}
}
void ScpObjectSpace::DestroyTempObjectsUni(VTPOBJECTS & TempObjects)
{
	ScpObject * tmp = NULL;
	ITPOBJECTS it = TempObjects.begin();
	while (it != TempObjects.end())
	{
		tmp = *it;
		tmp->DelRef();
		it++;
	}
}
BOOL ScpObjectSpace::IsMyParentSpace(ScpObjectSpace * space)
{
	BOOL bret = FALSE;
	ScpObjectSpace * pspace =this->parentspace;
	while(pspace!=NULL)
	{
		if(pspace==space)
		{
			bret = TRUE;
			break;
		}
		pspace=pspace->parentspace;
	}
	return bret;
}
//void ScpObjectSpace::AddTempObject(ScpObject * tempobj)
//{
//	if(tempobj->istemp)
//	{
//		ITPOBJECTS result = find( TempObjects.begin( ), TempObjects.end( ), tempobj ); 
//		if ( result == TempObjects.end( ) ) //没找到
//		{
//			tempobj->AddRef();
//			TempObjects.push_back(tempobj);
//		}
//	}
//	
//}
//void ScpObjectSpace::ClearTempObjects()
//{
//	ITPOBJECTS it = TempObjects.begin();	
//	while(it!=TempObjects.end())
//	{
//		ScpObject * tmp = *it;
//		if(tmp)
//			tmp->DelRef();
//		it = TempObjects.erase(it);
//	}
//}
std::wstring ScpObjectSpace::GetNewTempObjectName()
{
	std::wstring tempobjname ;	
	do
	{
		tempobjname = STDSTRINGEXT::Format(L"@@tempobj%d", ++LastTempObjAppendix);
	}while (FindLocalObject(tempobjname));
	LastTempObjectName = tempobjname;
	return tempobjname;
}

bool ScpObjectSpace::MoveToSpace(ScpObject * obj, ScpObjectSpace * space)
{
	if (obj && space)
	{
		if (space->AddObject(this->GetObjectName(obj), obj))
		{
			this->EraseObject(obj);
			return true;
		}		
	}
	return false;
}

ScpObjectSpace * ScpObjectSpace::GetGlobalSpace()
{
	ScpObjectSpace * g = this;
	while (g->parentspace)
	{
		g = g->parentspace;
	}
	if (g->ObjectSpaceType == Space_Global)
	{
		return g;
	}
	return nullptr;
}

ScpObjectSpace * ScpObjectSpace::FindObject_ObjectSpace(ScpObject * obj)
{
	std::wstring name = userobject.GetObjectName(obj);
	if (name.empty())
	{
		ScpObjectSpace * pobjectSpace = parentspace;
		while (pobjectSpace)
		{
			name = pobjectSpace->GetObjectName(obj);
			if (!name.empty())
			{
				return pobjectSpace;
			}
			pobjectSpace = pobjectSpace->parentspace;
		}
	}
	else
	{
		return this;
	}
	return nullptr;
}
