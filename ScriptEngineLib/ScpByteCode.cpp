#include "ScpByteCode.h"
#include "ScriptEngine.h"
#include "ScpRuntime.h"
#include "ScpStringObject.h"
#include "ScpTimeObject.h"
#include "ScpClassInstanceObject.h"
#include "ScpFunctionObject.h"
#include "ScpIfStatementObject.h"
#include "ScpWhileStatementObject.h"
#include "ScpStructObject.h"
#include "ScpObjectNammes.h"
#include "commanddefine_uni.h"
#include "../Common/commonutil.hpp"
#include "../Common/stdstringext.hpp"

ScpByteCode::ScpByteCode() :ByteCodeBuffer(NULL), ByteCodeLength(0)
{
	respoolpre.pool.clear();
	respool.pool.clear();
}

ScpByteCode::~ScpByteCode()
{
	if (ByteCodeBuffer)
	{
		free(ByteCodeBuffer);
		ByteCodeBuffer = NULL;
	}
}

int ScpByteCode::Load(const char * ByteCodeFileName)
{
	if (ByteCodeBuffer)
	{
		free(ByteCodeBuffer);
		ByteCodeBuffer = NULL;
	}
#ifdef _WIN32
	HANDLE hFile = CreateFileA(ByteCodeFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		DWORD dwSize = 0;
		DWORD dwReadCount = 0;
		dwSize = GetFileSize(hFile, NULL);
		ByteCodeBuffer = (unsigned char *)malloc(dwSize);
		if (ByteCodeBuffer) {
			ReadFile(hFile, ByteCodeBuffer, dwSize, &dwReadCount, NULL);
			ByteCodeLength = dwReadCount;
		}
		CloseHandle(hFile);
	}
#else	
	uint64_t filesize = GetFileLen(ByteCodeFileName);
	int fd = ::open(ByteCodeFileName, O_RDONLY);
	if (fd > 0)
	{		
		ULONG dwReadCount = 0;
		ByteCodeBuffer = (unsigned char *)malloc(filesize);
		if (ByteCodeBuffer) {
			::lseek(fd, 0, SEEK_SET);
			dwReadCount = ::read(fd, ByteCodeBuffer, filesize);
			ByteCodeLength = dwReadCount;
		}		
		::close(fd);
	}
#endif
	return 0;
}

int ScpByteCode::LoadFromMem(const unsigned char * ByteCode, unsigned int length)
{
	if (ByteCodeBuffer)
	{
		free(ByteCodeBuffer);
		ByteCodeBuffer = NULL;
	}
	ByteCodeBuffer = (unsigned char *)malloc(length);
	if (ByteCodeBuffer) {
		memcpy(ByteCodeBuffer, ByteCode, length);
		ByteCodeLength = length;
	}
	return 0;
}

int ScpByteCode::Do()
{
	int ret = -1;
	respool.pool.clear();
	respool.ResObjMap.clear();
	std::string temp("");
	respool.AppendResource(temp);
	respool.AppendResource(std::string("show"));
	respool.AppendResource(std::string("int"));
	respool.AppendResource(std::string("int64"));
	respool.AppendResource(std::string("double"));
	respool.AppendResource(std::string("string"));
	respool.AppendResource(std::string("file"));

	ScpIntObject * obj0 = new ScpIntObject();
	obj0->value = 0;
	ScpIntObject * obj1 = new ScpIntObject();
	obj1->value = 1;
	ScpIntObject * obj2 = new ScpIntObject();
	obj2->value = 2;
	ScpIntObject * obj3 = new ScpIntObject();
	obj3->value = 3;
	ScpIntObject * obj4 = new ScpIntObject();
	obj4->value = 4;
	ScpIntObject * obj5 = new ScpIntObject();
	obj5->value = 5;
	ScpIntObject * obj6 = new ScpIntObject();
	obj6->value = 6;
	ScpIntObject * obj7 = new ScpIntObject();
	obj7->value = 7;
	ScpIntObject * obj8 = new ScpIntObject();
	obj8->value = 8;
	ScpIntObject * obj9 = new ScpIntObject();
	obj9->value = 9;
	respool.setMappedObject(residzero, obj0);
	respool.setMappedObject(residone, obj1);
	respool.setMappedObject(residtwo, obj2);
	respool.setMappedObject(residthree, obj3);
	respool.setMappedObject(residfour, obj4);
	respool.setMappedObject(residfive, obj5);
	respool.setMappedObject(residsix, obj6);
	respool.setMappedObject(residseven, obj7);
	respool.setMappedObject(resideight, obj8);
	respool.setMappedObject(residnine, obj9);
	
	{
		std::string numname = "gl_";
		numname += "zero";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj0);
		}

		numname = "gl_";
		numname += "one";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj1);
		}

		numname = "gl_";
		numname += "two";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj2);
		}

		numname = "gl_";
		numname += "three";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj3);
		}

		numname = "gl_";
		numname += "four";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj4);
		}

		numname = "gl_";
		numname += "five";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj5);
		}

		numname = "gl_";
		numname += "six";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj6);
		}

		numname = "gl_";
		numname += "seven";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj7);
		}

		numname = "gl_";
		numname += "eight";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj8);
		}

		numname = "gl_";
		numname += "nine";
		if (!engine->GetCurrentObjectSpace()->FindObject(numname))
		{
			engine->GetCurrentObjectSpace()->AddObject(numname, obj9);
		}

	}
	
	std::string timename = "currenttime";
	if (!engine->GetCurrentObjectSpace()->FindObject(timename))
	{
		//Create Global Time Object
		ScpTimeObject * currenttime = new ScpTimeObject;
		if (currenttime)
		{
			currenttime->name = timename;
			currenttime->value = currenttime->GetNow();
			engine->getScriptByteCode().resourcepool->setMappedObject(residcurrenttime, currenttime);
			engine->GetCurrentObjectSpace()->AddObject(timename, currenttime);
		}
	}

	ret = DoByteCode(ByteCodeBuffer, ByteCodeLength);
	return ret;
}

int ScpByteCode::DoByteCode(const unsigned char * ByteCode, unsigned int length, int *consumedlength)
{
	unsigned int codelength = length;
	int consumed = 0;
	unsigned char * pCode = (unsigned char *)ByteCode;
	while (codelength > consumed)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		if (currentObjectSpace->ObjectSpaceType == Space_While)
		{
			if (currentObjectSpace->breakingout ||
				currentObjectSpace->continuewhile)
			{
				if (consumedlength)
					*consumedlength = consumed = length;
				break;
			}
		}
		if (*pCode == BC_INITRES)
		{
			unsigned int resid;
			pCode = DoResInit(pCode, resid);
			consumed = (pCode - ByteCode);
			if (consumedlength)
			{
				*consumedlength = consumed;
			}
			continue;
		}
		else if (*pCode == BC_NOP)
		{
			//nop do nothing;
			pCode++;
		}
		else if (*pCode == BC_SWITCHLANG)
		{
			pCode = DoSwitchLang(pCode);
		}
		else if (*pCode == BC_OBJECT_DEFINE)
		{
			pCode = DoObjectDefine(pCode);
		}
		else if (*pCode == BC_CALL)
		{
			pCode = DoCall(pCode);
		}
		else if (*pCode == BC_CALL_INNER)
		{
			pCode = DoCallInner(pCode);
		}
		else if (*pCode == BC_LOAD)
		{
			pCode = DoLoad(pCode);
		}
		else if (*pCode == BC_OBJECT_ADD)
		{
			pCode = DoObjectAdd(pCode);
		}
		else if (*pCode == BC_OBJECT_SUB)
		{
			pCode = DoObjectSub(pCode);
		}
		else if (*pCode == BC_OBJECT_MUL)
		{
			pCode = DoObjectMul(pCode);
		}
		else if (*pCode == BC_OBJECT_DIV)
		{
			pCode = DoObjectDiv(pCode);
		}
		else if (*pCode == BC_OBJECT_MOD)
		{
			pCode = DoObjectMod(pCode);
		}
		else if (*pCode == BC_OBJECT_BITAND)
		{
			pCode = DoObjectBitAnd(pCode);
		}
		else if (*pCode == BC_OBJECT_BITOR)
		{
			pCode = DoObjectBitOr(pCode);
		}
		else if (*pCode == BC_OBJECT_BITXOR )
		{
			pCode = DoObjectBitXor(pCode);
		}
		else if (*pCode == BC_OBJECT_LOGICALAND)
		{
			pCode = DoObjectLogicalAnd(pCode);
		}
		else if (*pCode == BC_OBJECT_LOGICALOR)
		{
			pCode = DoObjectLogicalOr(pCode);
		}
		else if (*pCode == BC_OBJECT_BITSHIFTLEFT)
		{
			pCode = DoObjectBitShiftLeft(pCode);
		}
		else if (*pCode == BC_OBJECT_BITSHIFTRIGHT)
		{
			pCode = DoObjectBitShiftRight(pCode);
		}
		else if (*pCode == BC_OBJECT_SELFADD)
		{
			pCode = DoObjectSelfAdd(pCode);
		}
		else if (*pCode == BC_OBJECT_SELFSUB)
		{
			pCode = DoObjectSelfSub(pCode);
		}
		else if (*pCode == BC_OBJECT_ASSIGN)
		{
			pCode = DoObjectAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_ADD_AND_ASSIGN)
		{
			pCode = DoObjectAddAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_SUB_AND_ASSIGN)
		{
			pCode = DoObjectSubAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_MUL_AND_ASSIGN)
		{
			pCode = DoObjectMulAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_DIV_AND_ASSIGN)
		{
			pCode = DoObjectDivAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_MOD_AND_ASSIGN)
		{
			pCode = DoObjectModAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_BITAND_AND_ASSIGN)
		{
			pCode = DoObjectBitAndAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_BITOR_AND_ASSIGN)
		{
			pCode = DoObjectBitOrAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_BITNOT_AND_ASSIGN)
		{
			pCode = DoObjectBitNotAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_BITXOR_AND_ASSIGN)
		{
			pCode = DoObjectBitXOrAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_BITSHIFTLEFT_AND_ASSIGN)
		{
			pCode = DoObjectBitShiftLeftAndAssign(pCode);
		}
		else if (*pCode == BC_OBJECT_BITSHIFTRIGHT_AND_ASSIGN)
		{
			pCode = DoObjectBitShiftRightAndAssign(pCode);
		}
		else if (*pCode == BC_LOOP)
		{
			pCode = DoLoop(pCode);
		}
		else if (*pCode == BC_OBJECT_EQUAL)
		{
			pCode = DoObjectEqual(pCode);
		}
		else if (*pCode == BC_OBJECT_LESSTHAN)
		{
			pCode = DoObjectLessThan(pCode);
		}
		else if (*pCode == BC_OBJECT_LESSOREQUAL)
		{
			pCode = DoObjectLessOrEqual(pCode);
		}
		else if (*pCode == BC_OBJECT_BIGTHAN)
		{
			pCode = DoObjectBigThan(pCode);
		}
		else if (*pCode == BC_OBJECT_BIGOREQUAL)
		{
			pCode = DoObjectBigOrEqual(pCode);
		}
		else if (*pCode == BC_OBJECT_NOT)
		{
			pCode = DoObjectNot(pCode);
		}
		else if (*pCode == BC_OBJECT_NOTEQUAL)
		{
			pCode = DoObjectNotEqual(pCode);
		}
		else if (*pCode == BC_IF)
		{
			pCode = DoIfStatement(pCode);
		}
		else if (*pCode == BC_WHILE)
		{
			pCode = DoWhileStatement(pCode);
		}
		else if (*pCode == BC_END)
		{
			pCode = DoEnd(pCode);
		}
		else if (*pCode == BC_OBJECT_FUNCTION_DEFINE)
		{
			pCode = DoObjectFunctionDefine(pCode);
		}
		else if (*pCode == BC_RETURN)
		{
			pCode = DoReturn(pCode);
		}
		else if (*pCode == BC_BREAK)
		{
			pCode = DoBreak(pCode);
			*consumedlength = consumed = length;
			break;
		}
		else if (*pCode == BC_CONTINUE)
		{
			pCode = DoContinue(pCode);
			*consumedlength = consumed = length;
			break;
		}
		else if (*pCode == BC_OBJECT_CLASS_DEFINE)
		{
			pCode = DoObjectClassDefine(pCode);
		}
		else if (*pCode == BC_PUBLIC)
		{
			pCode = DoPublic(pCode);
		}
		else if (*pCode == BC_PRIVATE)
		{
			pCode = DoPrivate(pCode);
		}
		else if (*pCode == BC_IMPORT)
		{
			pCode = DoImport(pCode);
		}
		else
		{
			break;
		}

		consumed = (pCode - ByteCode);
		if (consumedlength)
		{
			*consumedlength = consumed;
		}
	}
	return 0;
}

unsigned char * ScpByteCode::DoResInit(unsigned char * pByteCode, unsigned int& resid)
{
	unsigned int  reslen = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCELEN + reslen;
	std::string res = (char *)(pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCELEN);
	resid = respool.AppendResource(res);
	return nextByteCode;
}
unsigned char * ScpByteCode::DoSwitchLang(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_LANGID;
	unsigned int langid = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	return nextByteCode;
}
unsigned char *  ScpByteCode::DoObjectDefine(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;

	unsigned char objTypeId = *(unsigned char *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	VTPARAMETERS param;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID);
	std::string name;
	if (engine->extend_obj_mgr.IsExtendObject(objTypeId))
	{
		name = engine->extend_obj_mgr.GetExtendObjectName(objTypeId);
	}
	else
	{
		name = ScpGlobalObject::GetInstance()->GetTypeName(objTypeId);
	}
	if (objTypeId != ObjClassInstance)
		param.push_back(name);
	param.push_back(respool.scpGetResource(resid1));
	param.push_back(respool.scpGetResource(resid2));
	engine->FetchCommand(vl_define, &param);
	return nextByteCode;
}
unsigned char * ScpByteCode::DoCall(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT;
	unsigned int  paramcount = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	nextByteCode += paramcount * SIZE_OF_RESOURCEID;

	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	ScpObject * object = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
	if (object)
	{
		ScpFunctionObject * func = (ScpFunctionObject *)object;
		int i = 0;
		while (i < paramcount)
		{
			unsigned int  resid = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT + SIZE_OF_RESOURCEID * i);
			i++;
			func->RealParameters.push_back(respool.scpGetResource(resid));
		}

		BOOL bRecurseCall = FALSE;
		ScpObjectSpace * oldparentspace = func->FunctionObjectSpace->parentspace;  //解决递归调用的问题
		if (func->FunctionObjectSpace == engine->GetCurrentObjectSpace() || engine->GetCurrentObjectSpace()->IsMyParentSpace(func->FunctionObjectSpace))
		{
			bRecurseCall = TRUE;
		}
		else
		{
			func->FunctionObjectSpace->parentspace = engine->GetCurrentObjectSpace();
			engine->SetCurrentObjectSpace(func->FunctionObjectSpace);
		}
		func->Result = NULL;
		if (engine->GetCurrentObjectSpace()->lastcommand == vl_return)
		{
			engine->GetCurrentObjectSpace()->lastcommand = vl_call;
		}
		if (func->BindParameters(engine) != -1)
		{
			if (func->ByteCode && func->ByteCodeLength > 0)
			{
				DoByteCode(func->ByteCode, func->ByteCodeLength);
			}
		}
		func->UnBindParameters(engine);
		//还需要解除实际参数在资源池中的映射关系，否则下一次调用传递的参数就绑定错了
		for (int i = 0;i < func->FormalParameters.size();i++)
		{
			ULONG resid = engine->scpbytecode.respool.scpFindResource(func->FormalParameters.at(i));
			if (resid >= 0)
			{
				engine->scpbytecode.respool.unMapResource(resid);
			}
		}
		if (!bRecurseCall)
		{
			engine->SetCurrentObjectSpace(func->FunctionObjectSpace->parentspace);
			func->FunctionObjectSpace->parentspace = oldparentspace;
		}
	}
	return nextByteCode;
}
unsigned char *  ScpByteCode::DoCallInner(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT;
	unsigned int  paramcount = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	nextByteCode += paramcount * SIZE_OF_RESOURCEID;



	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * object = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
	if (object)
	{
		if (object->GetType() == ObjClassInstance)
		{
			ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
			ScpClassInstanceObject *classInstanceObj = (ScpClassInstanceObject *)object;
			ScpClassObject * classobj =(ScpClassObject * ) currentObjectSpace->FindObject(classInstanceObj->userclassname);
			classInstanceObj->UserClassObjectSpace.userobject.DeepCopy(&classobj->UserClassObjectSpace.userobject);
			classInstanceObj->memberattrmap = classobj->memberattrmap;
			classInstanceObj->ClassBody = classobj->ClassBody;
			/*classInstanceObj->UserClassObjectSpace = classobj->UserClassObjectSpace;

			for (int i = 0;i < classInstanceObj->UserClassObjectSpace.userobject.GetSize();i++)
			{
				ScpObject * obj = classInstanceObj->UserClassObjectSpace.userobject.GetObject(i);
				if (obj)
				{
					obj->AddRef();
				}
			}*/
			ScpObjectSpace* oldparent = classInstanceObj->UserClassObjectSpace.parentspace;
			
			ScpObject * objectfunc = classInstanceObj->UserClassObjectSpace.FindObject(respool.scpGetResource(resid2));
			if (objectfunc)
			{
				classInstanceObj->UserClassObjectSpace.parentspace = currentObjectSpace;
				engine->SetCurrentObjectSpace(&classInstanceObj->UserClassObjectSpace);

				ScpFunctionObject * func = (ScpFunctionObject *)objectfunc;
				int i = 0;
				while (i < paramcount)
				{
					unsigned int  resid = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT + i * SIZE_OF_RESOURCEID);
					i++;
					func->RealParameters.push_back(respool.scpGetResource(resid));
				}

				BOOL bRecurseCall = FALSE;
				ScpObjectSpace * oldparentspace = func->FunctionObjectSpace->parentspace;  //解决递归调用的问题
				if (func->FunctionObjectSpace == engine->GetCurrentObjectSpace() || engine->GetCurrentObjectSpace()->IsMyParentSpace(func->FunctionObjectSpace))
				{
					bRecurseCall = TRUE;
				}
				else
				{
					func->FunctionObjectSpace->parentspace = engine->GetCurrentObjectSpace();
					engine->SetCurrentObjectSpace(func->FunctionObjectSpace);
				}
				func->Result = NULL;
				if (engine->GetCurrentObjectSpace()->lastcommand == vl_return)
				{
					engine->GetCurrentObjectSpace()->lastcommand = vl_call;
				}
				if (func->BindParameters(engine) != -1)
				{
					if (func->ByteCode && func->ByteCodeLength > 0)
					{
						DoByteCode(func->ByteCode, func->ByteCodeLength);
					}
				}
				func->UnBindParameters(engine);
				if (!bRecurseCall)
				{
					engine->SetCurrentObjectSpace(func->FunctionObjectSpace->parentspace);
					func->FunctionObjectSpace->parentspace = oldparentspace;
				}

				engine->SetCurrentObjectSpace(currentObjectSpace);
				classInstanceObj->UserClassObjectSpace.parentspace = oldparent;
			}
			else
			{	
				//不是自定义成员函数，而是内置函数
				VTPARAMETERS param;
				for (int i = 0;i < paramcount;i++)
				{
					unsigned int  resid = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT + i * SIZE_OF_RESOURCEID);
					param.push_back(respool.scpGetResource(resid));
				}
				std::string functionname = respool.scpGetResource(resid2);
				object->CallInnerFunction(functionname, &param, engine);		
			}
			
		}
		else
		{
			VTPARAMETERS param;
			for (int i = 0;i < paramcount;i++)
			{
				unsigned int  resid = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT + i * SIZE_OF_RESOURCEID);
				param.push_back(respool.scpGetResource(resid));
			}
			std::string functionname = respool.scpGetResource(resid2);
			object->CallInnerFunction(functionname, &param, engine);
		}

	}

	return nextByteCode;
}

unsigned char * ScpByteCode::DoLoad(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID;
	unsigned char objtype = *(unsigned char *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID);
	if (objtype == ObjExt)
	{
		engine->extend_obj_mgr.LoadExtension(respool.scpGetResource(resid2).c_str(), &engine->globalcommand);
	}

	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectAdd(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = Add(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectSub(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = Sub(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectMul(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = Mul(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectDiv(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = Div(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectMod(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = Mod(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
	ScpObject* obj2 = respool.getMappedObject(resid2);
	if (obj2 == NULL)
	{
		obj2= engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
	}	
	if (obj1 && obj2)
	{
		if (obj2->GetType() == ObjFunction)
		{
			Assign(obj1, ((ScpFunctionObject*)obj2)->Result, engine);
		}
		else
		{
			Assign(obj1, obj2, engine);
		}
	}
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectAddAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	AddAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectSubAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	SubAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectMulAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	MulAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectDivAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	DivAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectModAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ModAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitAndAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	BitAndAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitOrAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	BitOrAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitNotAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	BitNotAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitXOrAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	BitXorAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitShiftLeftAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	BitShiftLeftAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitShiftRightAndAssign(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	BitShiftRightAndAssign(obj1, obj2, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoLoop(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_LOOPCOUNT;
	unsigned char * nextLine = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_LOOPCOUNT;
	unsigned int  loopcount = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	int consumed = 0;//获得下一条指令的实际长度
	while (loopcount > 0)
	{
		//执行下一条指令,nextLine指针没有变化，仍指向下一条指令
		DoByteCode(nextLine, 1, &consumed);
		loopcount--;
	}
	//返回下下一条指令的地址
	nextByteCode += consumed;
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectFunctionDefine(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT;
	unsigned int  paramcount = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	nextByteCode += paramcount * SIZE_OF_RESOURCEID;

	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID);
	unsigned int residparentobj= *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID+ SIZE_OF_RESOURCEID);
	ScpObject * object = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
	if (!object)
	{
		VTPARAMETERS param;
		for (int i = 0;i < paramcount;i++)
		{
			unsigned int  resid = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT + i * SIZE_OF_RESOURCEID);
			param.push_back(respool.scpGetResource(resid));
		}
		std::string functionname = respool.scpGetResource(resid1);
		std::string parentobjectname = respool.scpGetResource(residparentobj);

		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();		
		ScpFunctionObject * funcobj = new ScpFunctionObject;

		if (!parentobjectname.empty())
		{
			ScpObject * parentobj = currentObjectSpace->FindObject(parentobjectname);
			if (parentobj->GetType() == ObjClass || parentobj->GetType() == ObjClassInstance)
			{
				currentObjectSpace = &((ScpClassObject *)parentobj)->UserClassObjectSpace;
				funcobj->parentobjectname = parentobjectname;
				funcobj->ParenObject = parentobj;
			}
		}
		funcobj->Name = functionname;
		for (size_t i = 0;i < param.size();i++)
		{
			std::string wparam = param.at(i);
			funcobj->FormalParameters.push_back(wparam);
		}
		funcobj->FunctionObjectSpace->spacename = ScpGlobalObject::GetInstance()->GetTypeName(ObjFunction) + functionname;
		currentObjectSpace->AddObject(functionname, funcobj);
		funcobj->FunctionObjectSpace->parentspace = currentObjectSpace;
		engine->SetCurrentObjectSpace(funcobj->FunctionObjectSpace);


		unsigned char * temp = nextByteCode;
		int functionlength_byte = 0;
		//这里需要判断名字空间，考虑函数中有if语句 while语句 嵌套的问题		
		//while (*temp != BC_END)
		//{
		//	functionlength_byte = GetBlockByteCodeLength(&temp);
		//	temp += functionlength_byte;				
		//}
		functionlength_byte = GetBlockByteCodeLength(temp);
		temp += functionlength_byte;
		functionlength_byte = (temp - nextByteCode);
		funcobj->ByteCode = (unsigned char *)malloc(functionlength_byte);
		memcpy(funcobj->ByteCode, nextByteCode, functionlength_byte);
		funcobj->ByteCodeLength = functionlength_byte;
		nextByteCode += functionlength_byte;
	}
	return nextByteCode;
}

unsigned char * ScpByteCode::DoReturn(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);

	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	ScpObjectSpace * FuncObjectSpace = NULL;
	//当前名字空间有可能是函数内部的条件语句或者循环语句
	//这里找到最内层的函数调用的名字空间
	if (currentObjectSpace->ObjectSpaceType != Space_Function)
	{
		if (currentObjectSpace->ObjectSpaceType == Space_While)
		{
			currentObjectSpace->breakingout = 1;
		}
		ScpObjectSpace * parentObjectSpace = currentObjectSpace->parentspace;
		while (parentObjectSpace && parentObjectSpace->ObjectSpaceType != Space_Global)
		{
			if (parentObjectSpace->ObjectSpaceType == Space_Function)
			{
				break;
			}
			if (parentObjectSpace->ObjectSpaceType == Space_While)
			{
				parentObjectSpace->breakingout = 1;
			}
			parentObjectSpace = parentObjectSpace->parentspace;
		}
		if (parentObjectSpace != NULL && parentObjectSpace->ObjectSpaceType == Space_Function)
			FuncObjectSpace = parentObjectSpace;
		else
			FuncObjectSpace = currentObjectSpace;

	}
	else
	{
		FuncObjectSpace = currentObjectSpace;
	}

	if (FuncObjectSpace->ObjectSpaceType == Space_Function)
	{
		ScpObject * object = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		ScpFunctionObject * Func = (ScpFunctionObject *)engine->GetCurrentObjectSpace()->belongto;
		if (Func && object)
		{
			Func->Result = object;
		}
	}
	FuncObjectSpace->lastcommand = vl_return;
	currentObjectSpace->lastcommand = vl_return;
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectClassDefine(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID);
	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	std::string classname = respool.scpGetResource(resid1);
	ScpClassObject *classobj = new ScpClassObject;
	classobj->ClassDefine(classname);
	currentObjectSpace->AddObject(classname, classobj);
	classobj->UserClassObjectSpace.parentspace = engine->GetCurrentObjectSpace();
	engine->SetCurrentObjectSpace(&classobj->UserClassObjectSpace);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoPublic(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND;
	ScpClassObject * classobj = (ScpClassObject *)engine->GetCurrentObjectSpace()->belongto;
	if (classobj)
	{
		classobj->MemberVariableAttribute = ScpClassObject::Attr_Public;
	}
	return nextByteCode;
}

unsigned char * ScpByteCode::DoPrivate(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND;
	ScpClassObject * classobj = (ScpClassObject *)engine->GetCurrentObjectSpace()->belongto;
	if (classobj)
	{
		classobj->MemberVariableAttribute = ScpClassObject::Attr_Private;
	}
	return nextByteCode;
}

unsigned char * ScpByteCode::DoInherite(unsigned char * pByteCode)
{
	return nullptr;
}

unsigned char * ScpByteCode::DoWhileStatement(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK + SIZE_OF_BOLCK;
	unsigned short  condlength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned short  blocklength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK);
	unsigned char * condByteCode = nextByteCode;
	unsigned char * whileBlockByteCode = NULL;
	unsigned char * temp = nextByteCode;
	int condlength_byte = 0;
	while (condlength > 0)
	{
		condlength_byte = GetByteCodeLength(temp);
		temp += condlength_byte;
		condlength--;
	}
	condlength_byte = temp - nextByteCode;
	whileBlockByteCode = condByteCode + condlength_byte;
	//计算whileblock的大小
	temp = whileBlockByteCode;
	int whilelenth_byte = 0;
	while (blocklength > 0)
	{
		whilelenth_byte = GetByteCodeLength(temp);
		temp += whilelenth_byte;
		blocklength--;
	}
	whilelenth_byte = temp - whileBlockByteCode;
	nextByteCode = whileBlockByteCode + whilelenth_byte;

	ScpObjectSpace *currentObjectSpace = engine->GetCurrentObjectSpace();
	ScpWhileStatementObject * whileobj = new ScpWhileStatementObject(engine);
	if (whileobj)
	{
		std::string userobjname = ScpObjectNames::GetSingleInsatnce()->strObjWhileStatement;
		ScpWhileStatementObject * temp = (ScpWhileStatementObject *)currentObjectSpace->FindObject(userobjname);
		while (temp)
		{
			userobjname += "@";
			temp = (ScpWhileStatementObject *)currentObjectSpace->FindObject(userobjname);
		}
		whileobj->Name = userobjname;
		whileobj->WhileStatementObjectSpace.spacename = userobjname;
		currentObjectSpace->AddObject(userobjname, whileobj);
		whileobj->WhileStatementObjectSpace.parentspace = currentObjectSpace;
		engine->SetCurrentObjectSpace(&whileobj->WhileStatementObjectSpace);
	}
	while (1)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		if (currentObjectSpace->breakingout)
		{
			break;
		}
		DoContitionBlock(condByteCode, condlength_byte);
		//约定条件语句返回的临时对象为tempresultif

		ScpObject * obj1 = currentObjectSpace->FindObject(std::string("tempwhile"));
		if (obj1 && obj1->GetType() == ObjInt)
		{
			if (((ScpIntObject*)obj1)->value == 1)
			{
				DoWhileBlock(whileBlockByteCode, whilelenth_byte);
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	return nextByteCode;
}

unsigned char * ScpByteCode::DoWhileBlock(unsigned char * pByteCode, int size)
{
	int consumed = 0;
	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	if (currentObjectSpace->ObjectSpaceType == Space_While)
	{
		currentObjectSpace->breakingout = 0;
		currentObjectSpace->continuewhile = 0;
	}
	DoByteCode(pByteCode, size, &consumed);
	unsigned char * nextByteCode = pByteCode + consumed;
	return nextByteCode;
}

unsigned char * ScpByteCode::DoEnd(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND;

	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	if (currentObjectSpace->ObjectSpaceType == Space_Function || currentObjectSpace->ObjectSpaceType == Space_While)
	{
		if (currentObjectSpace->belongto)
		{
			if (currentObjectSpace->belongto->GetType() == ObjFunction)
			{
				ScpFunctionObject * funcobj = (ScpFunctionObject *)currentObjectSpace->belongto;
				engine->SetCurrentObjectSpace(funcobj->FunctionObjectSpace->parentspace);
			}
			else if (currentObjectSpace->belongto->GetType() == ObjWhileStatement)
			{
				ScpWhileStatementObject * whileobj = (ScpWhileStatementObject *)currentObjectSpace->belongto;
				engine->SetCurrentObjectSpace(whileobj->WhileStatementObjectSpace.parentspace);
				ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
				currentObjectSpace->EraseObject(whileobj->Name);
			}
			else
			{
				engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidScript);
			}
		}
	}
	else if (currentObjectSpace->ObjectSpaceType == Space_Class)
	{
		ScpClassObject *classobj = (ScpClassObject *)currentObjectSpace->belongto;
		engine->SetCurrentObjectSpace(classobj->UserClassObjectSpace.parentspace);
	}
	else if (currentObjectSpace->ObjectSpaceType == Space_If)
	{
		ScpIfStatementObject * ifstmtobj = (ScpIfStatementObject *)currentObjectSpace->belongto;
		if (ifstmtobj->GetType() == ObjIfStatement)
		{
			engine->SetCurrentObjectSpace(ifstmtobj->IfStatementObjectSpace.parentspace);
			ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
			currentObjectSpace->EraseObject(ifstmtobj->Name);
		}
	}
	else if (currentObjectSpace->ObjectSpaceType == Space_Struct)
	{
		ScpStructObject * structobj = (ScpStructObject*)currentObjectSpace->belongto;
		if (structobj->GetType() == ObjStruct)
		{
			structobj->GetStructMemorySize();
			engine->SetCurrentObjectSpace(structobj->StructObjectSpace.parentspace);
		}
	}
	return nextByteCode;
}

unsigned char * ScpByteCode::DoBreak(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND;
	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	while (currentObjectSpace->ObjectSpaceType != Space_While && currentObjectSpace->ObjectSpaceType != Space_Global)
	{
		currentObjectSpace = currentObjectSpace->parentspace;
	}
	if (currentObjectSpace->ObjectSpaceType == Space_While)
	{
		currentObjectSpace->breakingout = 1;
	}
	return nextByteCode;
}

unsigned char * ScpByteCode::DoContinue(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND;
	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	while (currentObjectSpace->ObjectSpaceType != Space_While && currentObjectSpace->ObjectSpaceType != Space_Global)
	{
		currentObjectSpace = currentObjectSpace->parentspace;
	}
	if (currentObjectSpace->ObjectSpaceType == Space_While)
	{
		currentObjectSpace->continuewhile = 1;
	}
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectEqual(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = Equal(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectLessThan(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = Lessthan(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectLessOrEqual(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = LessorEqual(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBigThan(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = Bigthan(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBigOrEqual(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = BigorEqual(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectNot(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * objtemp = Not(obj2, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectNotEqual(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = NotEqual(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitAnd(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = BitAnd(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitOr(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = BitOr(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitNot(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * objtemp = BitNot(obj2, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitXor(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = BitXor(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectLogicalAnd(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = LogicalAnd(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectLogicalOr(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = LogicalOr(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitShiftLeft(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = BitShiftLeft(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectBitShiftRight(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned int  resid2 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
	unsigned int  resid3 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	ScpObject * obj2 = respool.getMappedObject(resid2);
	if (!obj2)
	{
		obj2 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid2));
		respool.setMappedObject(resid2, obj2);
	}
	ScpObject * obj3 = respool.getMappedObject(resid3);
	if (!obj3)
	{
		obj3 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid3));
		respool.setMappedObject(resid3, obj3);
	}
	ScpObject * objtemp = BitShiftRight(obj2, obj3, engine);
	Assign(obj1, objtemp, engine);
	engine->GetCurrentObjectSpace()->ReleaseTempObject(objtemp);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectSelfAdd(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND );
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	if(obj1)
		PostSelfAdd(obj1, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoObjectSelfSub(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND );
	ScpObject * obj1 = respool.getMappedObject(resid1);
	if (!obj1)
	{
		obj1 = engine->GetCurrentObjectSpace()->FindObject(respool.scpGetResource(resid1));
		respool.setMappedObject(resid1, obj1);
	}
	if (obj1)
		PostSelfSub(obj1, engine);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoImport(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID;
	unsigned int  resid1 = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND+ SIZE_OF_OBJECT_TYPE_ID);
	std::string libname = respool.scpGetResource(resid1);
	engine->ImportLib(libname);
	return nextByteCode;
}

unsigned char * ScpByteCode::DoIfStatement(unsigned char * pByteCode)
{
	unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK + SIZE_OF_BOLCK + SIZE_OF_BOLCK;
	unsigned short  condlength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
	unsigned short  tlength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK);
	unsigned short  flength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK + SIZE_OF_BOLCK);
	unsigned char * condByteCode = nextByteCode;
	unsigned char * temp = nextByteCode;
	int condlength_byte = 0;
	while (condlength > 0)
	{
		condlength_byte = GetByteCodeLength(temp);
		temp += condlength_byte;
		condlength--;
	}
	condlength_byte = temp - condByteCode;


	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	std::string userobjname = ScpObjectNames::GetSingleInsatnce()->strObjIfStatement;
	userobjname = ScpObjectNames::GetSingleInsatnce()->strObjIfStatement;
	ScpIfStatementObject * ifstmtObj = new ScpIfStatementObject(engine);
	if (ifstmtObj)
	{
		while (currentObjectSpace->FindObject(userobjname))
		{
			userobjname += "@";
		}
		ifstmtObj->Name = userobjname;
		ifstmtObj->IfStatementObjectSpace.spacename = userobjname;
		currentObjectSpace->AddObject(userobjname, ifstmtObj);
		ifstmtObj->IfStatementObjectSpace.parentspace = currentObjectSpace;
		engine->SetCurrentObjectSpace(&ifstmtObj->IfStatementObjectSpace);
	}


	nextByteCode = DoContitionBlock(nextByteCode, condlength_byte);

	//约定条件语句返回的临时对象为tempresultif
	ScpObject * obj1 = engine->GetCurrentObjectSpace()->FindObject(std::string("tempif"));
	if (obj1 && obj1->GetType() == ObjInt)
	{
		//计算trueblock的大小
		//temp = nextByteCode;
		unsigned char * tByteCode = temp;
		int tlenth_byte = 0;
		while (tlength > 0)
		{
			tlenth_byte = GetByteCodeLength(temp);
			temp += tlenth_byte;
			tlength--;
		}
		//计算falseblock的大小
		tlenth_byte = temp - tByteCode;
		unsigned char * fByteCode = temp;
		int flenth_byte = 0;
		while (flength > 0)
		{
			flenth_byte = GetByteCodeLength(temp);
			temp += flenth_byte;
			flength--;
		}
		flenth_byte = temp - fByteCode;

		if (((ScpIntObject*)obj1)->value == 1)
		{
			DoTrueBlock(nextByteCode, tlenth_byte);
		}
		else
		{
			DoFalseBlock(nextByteCode + tlenth_byte, flenth_byte);
		}
		nextByteCode += tlenth_byte + flenth_byte;
	}
	return nextByteCode;
}

unsigned char * ScpByteCode::DoContitionBlock(unsigned char * pByteCode, int size)
{
	int consumed = 0;
	DoByteCode(pByteCode, size, &consumed);
	unsigned char * nextByteCode = pByteCode + consumed;
	return nextByteCode;
}

unsigned char * ScpByteCode::DoTrueBlock(unsigned char * pByteCode, int size)
{
	int consumed = 0;
	DoByteCode(pByteCode, size, &consumed);
	unsigned char * nextByteCode = pByteCode + consumed;
	return nextByteCode;
}

unsigned char * ScpByteCode::DoFalseBlock(unsigned char * pByteCode, int size)
{
	int consumed = 0;
	DoByteCode(pByteCode, size, &consumed);
	unsigned char * nextByteCode = pByteCode + consumed;
	return nextByteCode;
}

int ScpByteCode::GetByteCodeLength(unsigned char * pByteCode)
{
	int length = 0;
	if (*pByteCode == BC_INITRES)
	{
		unsigned int  reslen = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCELEN + reslen;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_NOP)
	{
		length = 1;
	}
	else if (*pByteCode == BC_SWITCHLANG)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_LANGID;
		unsigned int langid = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_OBJECT_DEFINE)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_CALL)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT;
		unsigned int  paramcount = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID);
		nextByteCode += paramcount * SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_CALL_INNER)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT;
		unsigned int  paramcount = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID);
		nextByteCode += paramcount * SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_LOAD)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_OBJECT_ADD ||
		*pByteCode == BC_OBJECT_SUB ||
		*pByteCode == BC_OBJECT_MUL ||
		*pByteCode == BC_OBJECT_DIV ||
		*pByteCode == BC_OBJECT_MOD)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_OBJECT_ASSIGN ||
		*pByteCode == BC_OBJECT_ADD_AND_ASSIGN ||
		*pByteCode == BC_OBJECT_SUB_AND_ASSIGN ||
		*pByteCode == BC_OBJECT_MUL_AND_ASSIGN ||
		*pByteCode == BC_OBJECT_DIV_AND_ASSIGN ||
		*pByteCode == BC_OBJECT_MOD_AND_ASSIGN ||
		*pByteCode == BC_OBJECT_BITAND_AND_ASSIGN ||
		*pByteCode == BC_OBJECT_BITOR_AND_ASSIGN||
		*pByteCode == BC_OBJECT_BITNOT_AND_ASSIGN ||
		*pByteCode == BC_OBJECT_BITXOR_AND_ASSIGN||
		*pByteCode == BC_OBJECT_BITSHIFTLEFT_AND_ASSIGN ||
		*pByteCode == BC_OBJECT_BITSHIFTRIGHT_AND_ASSIGN)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_LOOP)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_LOOPCOUNT;
		int nextcommandlen = GetByteCodeLength(nextByteCode);
		nextByteCode += nextcommandlen;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_OBJECT_EQUAL ||
		*pByteCode == BC_OBJECT_LESSTHAN ||
		*pByteCode == BC_OBJECT_LESSOREQUAL ||
		*pByteCode == BC_OBJECT_BIGTHAN ||
		*pByteCode == BC_OBJECT_BIGOREQUAL ||
		*pByteCode == BC_OBJECT_NOTEQUAL ||
		*pByteCode == BC_OBJECT_BITAND ||
		*pByteCode == BC_OBJECT_BITOR ||
		*pByteCode == BC_OBJECT_BITXOR ||
		*pByteCode == BC_OBJECT_LOGICALAND ||
		*pByteCode == BC_OBJECT_LOGICALOR ||
		*pByteCode == BC_OBJECT_BITSHIFTLEFT ||
		*pByteCode == BC_OBJECT_BITSHIFTRIGHT)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_OBJECT_NOT ||
		*pByteCode == BC_OBJECT_BITNOT
		)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}

	else if (*pByteCode == BC_IF)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK + SIZE_OF_BOLCK + SIZE_OF_BOLCK;
		unsigned short  condlength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
		unsigned short  tlength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK);
		unsigned short  flength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK + SIZE_OF_BOLCK);
		unsigned char * condByteCode = nextByteCode;
		unsigned char * temp = nextByteCode;
		//计算条件语句块的大小
		int condlength_byte = 0;
		while (condlength > 0)
		{
			condlength_byte = GetByteCodeLength(temp);
			temp += condlength_byte;
			condlength--;
		}
		condlength_byte = temp - condByteCode;
		unsigned char * tByteCode = temp;
		//计算trueblock的大小
		//temp现在指向trueblock
		int tlenth_byte = 0;
		while (tlength > 0)
		{
			tlenth_byte = GetByteCodeLength(temp);
			temp += tlenth_byte;
			tlength--;
		}
		tlenth_byte = temp - tByteCode;
		unsigned char * fByteCode = temp;
		//计算falseblock的大小
		int flenth_byte = 0;
		while (flength > 0)
		{
			flenth_byte = GetByteCodeLength(temp);
			temp += flenth_byte;
			flength--;
		}
		flenth_byte = temp - fByteCode;
		nextByteCode += condlength_byte + tlenth_byte + flenth_byte;
		//长度包含end语句
		length = nextByteCode - pByteCode + 1;
	}
	else if (*pByteCode == BC_RETURN)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_OBJECT_FUNCTION_DEFINE)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT;
		unsigned int  paramcount = *(unsigned int *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_OBJECT_TYPE_ID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_PARAMCOUNT);
		nextByteCode += paramcount * SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_WHILE)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK + SIZE_OF_BOLCK;
		unsigned short  condlength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND);
		unsigned short  blocklength = *(unsigned short *)((unsigned char *)pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK);
		unsigned char * condByteCode = nextByteCode;
		unsigned char * whileBlockByteCode = NULL;
		unsigned char * temp = nextByteCode;
		int condlength_byte = 0;
		while (condlength > 0)
		{
			condlength_byte = GetByteCodeLength(temp);
			temp += condlength_byte;
			condlength--;
		}
		condlength_byte = temp - nextByteCode;
		whileBlockByteCode = condByteCode + condlength_byte;
		//计算whileblock的大小
		temp = whileBlockByteCode;
		int whilelenth_byte = 0;
		while (blocklength > 0)
		{
			whilelenth_byte = GetByteCodeLength(temp);
			temp += whilelenth_byte;
			blocklength--;
		}
		whilelenth_byte = temp - whileBlockByteCode;
		nextByteCode = whileBlockByteCode + whilelenth_byte;
		//长度包含end语句
		length = nextByteCode - pByteCode + 1;
	}
	else if (*pByteCode == BC_BREAK ||
		*pByteCode == BC_CONTINUE ||
		*pByteCode == BC_PUBLIC ||
		*pByteCode == BC_PRIVATE)
	{
		length = 1;
	}
	else if (*pByteCode == BC_IMPORT)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	else if (*pByteCode == BC_OBJECT_SELFADD ||
	*pByteCode == BC_OBJECT_SELFSUB)
	{
		unsigned char * nextByteCode = pByteCode + SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID;
		length = nextByteCode - pByteCode;
	}
	return length;
}

int ScpByteCode::GetBlockByteCodeLength(unsigned char * ppByteCode)
{
	int blocklength=0;
	if (ppByteCode )
	{
		unsigned char * temp = ppByteCode;
		while (*temp != BC_END)
		{
			int subblocklength = 0;
			if (*temp == BC_IF||*temp==BC_WHILE)
			{
				subblocklength = GetByteCodeLength(temp);
			}
			else
			{
				subblocklength = GetByteCodeLength(temp);
			}		
			temp += subblocklength;
			blocklength += subblocklength;
		}
	}
	return blocklength;
}
