/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriyption:
*/
#include "ScpIfStatementObject.h"
#include "commanddefine_uni.h"
#include "../Common/DebugUtil.hpp"

ScpIfStatementObject::ScpIfStatementObject(CScriptEngine *eg):trueblock(NULL),falseblock(NULL),ConditionResult(-1),TrueBody(true)
{
	objecttype = ObjIfStatement;
	IfStatementObjectSpace.ObjectSpaceType = Space_If;
	IfStatementObjectSpace.belongto = this;
	TrueBody = true;
	ConditionResult = -1;
	engine=eg;	
}


ScpIfStatementObject::~ScpIfStatementObject(void)
{
	
}
void ScpIfStatementObject::Show(CScriptEngine * engine)
{

}
ScpObject * ScpIfStatementObject::Clone(std::string strObjName)
{
	return NULL;
}
std::string ScpIfStatementObject::ToString()
{
	std::string temp;
	return temp;
}
void ScpIfStatementObject::Release()
{
	delete this;
}
bool ScpIfStatementObject::IsInnerFunction(std::string & functionname)
{
	return false;
}
ScpObject * ScpIfStatementObject::CallInnerFunction(std::string & functionname, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	return nullptr;
}
int ScpIfStatementObject::Do(CScriptEngine *engine)
{
	ScpObjectSpace * currentSpace = engine->GetCurrentObjectSpace();
	BOOL recurseCall = FALSE;
	if (currentSpace->IsMyParentSpace(&IfStatementObjectSpace))
	{
		recurseCall = TRUE;
	}

	ReComputeCondition();	
	if (!recurseCall)
		engine->SetCurrentObjectSpace(&IfStatementObjectSpace);
	engine->GetCurrentObjectSpace()->lastcommand = vl_compute;
	if (ConditionResult == 1)
	{
		if (trueblock)
		{
			trueblock->Run(engine);
		}
	}
	else
	{
		if (falseblock)
		{
			falseblock->Run(engine);
		}
	}
	if (!recurseCall)
		engine->SetCurrentObjectSpace(IfStatementObjectSpace.parentspace);
	return 0;
}
void ScpIfStatementObject::ClearLocalObjects()
{
	IfStatementObjectSpace.userobject.Destroy();
}
int ScpIfStatementObject::ReComputeCondition()
{
	if(!ConditionExpression.empty())
	{
		if(ConditionExpression=="1")
		{
			ConditionResult = 1;
		}
		else
		{
			ScpObject * obj =engine->GetCurrentObjectSpace()->FindObject(ConditionExpression);
			if(obj && obj->GetType()==ObjInt)
			{
				ConditionResult=((ScpIntObject *)obj)->value;
			}
			else
			{
				//需要优化，这里每次都对条件表达式进行重新解析
				ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();//IfStatementObjectSpace.parentspace;	
				
				ScpExpressionTreeNode *	ConditionExpressionroot= engine->ana.BuildExpressionTreeEx(ConditionExpression);
				if(ConditionExpressionroot)
				{
					ScpObject * retobj= ConditionExpressionroot->CalculateEx(engine);
					if(retobj)
					{
						ConditionResult = ((ScpIntObject*)retobj)->value;	
						if (retobj->istemp)
						{
							currentObjectSpace->ReleaseTempObject(retobj);
						}
					}		
					else
					{
						ConditionResult = 0 ;
					}
					ConditionExpressionroot->Destroy(currentObjectSpace);
					engine->GetCurrentObjectSpace()->ReleaseTempObject(ConditionExpressionroot);

				}			
				engine->GetCurrentObjectSpace()->lastTestResult = ConditionResult;
				DebugUtil::TraceA("ReComputeCondition ConditionExpression %s ConditionResult %d",ConditionExpression.c_str(),ConditionResult);
			}
		}
	}
	return ConditionResult;
}

bool ScpIfStatementObject::MakeConditionByteCode()
{
	std::string name = "tempif";
	ULONG idif;
	idif = engine->bytecode.resourcepool->scpFindResource(name);
	while (idif != -1)
	{
		name +="0";
		idif = engine->bytecode.resourcepool->scpFindResource(name);
	}
	if (idif == -1)
	{
		idif = engine->bytecode.resourcepool->AppendResource(name);
		ByteCodeMemoryStream stream;
		engine->bytecode.GetByteCodeInitRes(name, stream, idif);
		engine->bytecode.bytecodemem->AppendByteCode(&stream);
		stream.Release();

		VTPARAMETERS param;
		param.push_back("int");
		param.push_back(name);
		engine->bytecode.GenByteCodeObjectDefine(ObjInt, param, stream);
		engine->bytecode.bytecodemem->AppendByteCode(&stream);
		stream.Release();
	}
	if (!ConditionExpression.empty())
	{
		if (ConditionExpression == "1")
		{
			ConditionResult = 1;
			ByteCodeMemoryStream stream;
			engine->bytecode.GetByteCodeBinaryOp(scpOperationAssign, -1, idif, residone, stream);
			condition_bytecodemem.AppendByteCode(&stream);
			stream.Release();
		}
		else
		{
			ScpObject * obj = engine->GetCurrentObjectSpace()->FindObject(ConditionExpression);
			if (obj && obj->GetType() == ObjInt)
			{
				ConditionResult = ((ScpIntObject *)obj)->value;
				ULONG resid;
				if (ConditionResult == 1)
					resid = residone;
				else if (ConditionResult == 0)
					resid = residzero;
				ByteCodeMemoryStream stream;
				engine->bytecode.GetByteCodeBinaryOp(scpOperationAssign, -1, idif, resid, stream);
				condition_bytecodemem.AppendByteCode(&stream);
				stream.Release();
			}
			else
			{
				//需要优化，这里每次都对条件表达式进行重新解析
				ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();//IfStatementObjectSpace.parentspace;	
				
				ScpExpressionTreeNode *	ConditionExpressionroot = engine->ana.BuildExpressionTreeEx(ConditionExpression);
				if (ConditionExpressionroot)
				{
					ByteCodeMemoryStream stream;
					ScpObject * retobj = ConditionExpressionroot->MakeByteCode(engine, stream);
					condition_bytecodemem.AppendByteCode(&stream);
					stream.Release();

					ConditionResult = ((ScpIntObject*)retobj)->value;

					name = engine->GetCurrentObjectSpace()->GetObjectNameR(retobj);
					ULONG idret = engine->bytecode.resourcepool->scpFindResource(name);
					engine->bytecode.GetByteCodeBinaryOp(scpOperationAssign, -1, idif, idret, stream);
					condition_bytecodemem.AppendByteCode(&stream);
					stream.Release();
					
					ConditionExpressionroot->Destroy(currentObjectSpace);
					engine->GetCurrentObjectSpace()->ReleaseTempObject(ConditionExpressionroot);

				}
				//engine->GetCurrentObjectSpace()->lastTestResult = ConditionResult;
				DebugUtil::TraceA("ReComputeCondition ConditionExpression %s ConditionResult %d", ConditionExpression.c_str(), ConditionResult);
			}
		}
	}
	return false;
}
