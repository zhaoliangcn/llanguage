/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
*/
#include "ScpWhileStatementObject.h"
#include "ScpStringObject.h"
#include "commanddefine_uni.h"

ScpWhileStatementObject::ScpWhileStatementObject(CScriptEngine * eg):whilexpressionblock(NULL),ConditionResult(-1)
{
	WhileStatementObjectSpace.ObjectSpaceType = Space_While;
	WhileStatementObjectSpace.belongto = this;
	engine=eg;
	objecttype = ObjWhileStatement;
}
int ScpWhileStatementObject::ReComputeConditionResult()
{
    if (ConditionExpression.empty()) {
        return 0;
    }
	ScpObjectSpace *currentObjectSpace = engine->GetCurrentObjectSpace();
	if(ConditionExpression=="1")
	{
		ConditionResult=1;
	}
	else
	{
		ScpObject * obj =currentObjectSpace->FindObject(ConditionExpression);
		if(obj && obj->GetType()==ObjInt)
		{
			ConditionResult=((ScpIntObject *)obj)->value;
		}
		else
		{
			ScpExpressionTreeNode  *ConditionExpressionroot= engine->getExpressionAnalyser().BuildExpressionTreeEx(ConditionExpression);
			if(ConditionExpressionroot)
			{
				ScpObject * retobj=NULL;
				retobj= ConditionExpressionroot->CalculateEx(engine);
				if(retobj)
				{
					ConditionResult =((ScpIntObject *)retobj)->value;
					if (retobj->istemp)
					{
						currentObjectSpace->ReleaseTempObject(retobj);
					}
				}
				ConditionExpressionroot->Destroy(currentObjectSpace);
				engine->GetCurrentObjectSpace()->ReleaseTempObject(ConditionExpressionroot);

			}
		}
	}
	return ConditionResult;
}
bool ScpWhileStatementObject::MakeConditionByteCode()
{
	std::string name = "tempwhile";
	ULONG idwhile;
	idwhile = engine->getScriptByteCode().resourcepool->scpFindResource(name);
	while (idwhile != -1)
	{
		name += "0";
		idwhile = engine->getScriptByteCode().resourcepool->scpFindResource(name);
	}
	if (idwhile == -1)
	{
		idwhile = engine->getScriptByteCode().resourcepool->AppendResource(name);
		ByteCodeMemoryStream stream;
		engine->getScriptByteCode().GetByteCodeInitRes(name, stream, idwhile);
		engine->getScriptByteCode().bytecodemem->AppendByteCode(&stream);
		stream.Release();

		VTPARAMETERS param;
		param.push_back("int");
		param.push_back(name);
		engine->getScriptByteCode().GenByteCodeObjectDefine(ObjInt, param, stream);
		engine->getScriptByteCode().bytecodemem->AppendByteCode(&stream);
		stream.Release();
	}
	if (!ConditionExpression.empty())
	{
		if (ConditionExpression == "1")
		{
			ConditionResult = 1;
			ByteCodeMemoryStream stream;
			engine->getScriptByteCode().GetByteCodeBinaryOp(scpOperationAssign, -1, idwhile, residone, stream);
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
				engine->getScriptByteCode().GetByteCodeBinaryOp(scpOperationAssign, -1, idwhile, resid, stream);
				condition_bytecodemem.AppendByteCode(&stream);
				stream.Release();
			}
			else
			{
				ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();					
				ScpExpressionTreeNode *	ConditionExpressionroot = engine->getExpressionAnalyser().BuildExpressionTreeEx(ConditionExpression);
				if (ConditionExpressionroot)
				{
					ByteCodeMemoryStream stream;
					ScpObject * retobj = ConditionExpressionroot->MakeByteCode(engine, stream);
					condition_bytecodemem.AppendByteCode(&stream);
					stream.Release();
					name = engine->GetCurrentObjectSpace()->GetObjectNameR(retobj);
					ULONG idret = engine->getScriptByteCode().resourcepool->scpFindResource(name);
					engine->getScriptByteCode().GetByteCodeBinaryOp(scpOperationAssign, -1, idwhile, idret, stream);
					condition_bytecodemem.AppendByteCode(&stream);
					stream.Release();

					ConditionExpressionroot->Destroy(currentObjectSpace);
					engine->GetCurrentObjectSpace()->ReleaseTempObject(ConditionExpressionroot);

				}
			}
		}
	}
	return false;

}
void ScpWhileStatementObject::SetCondition(std::string condition)
{
	ScpStringObject *strobj2=(ScpStringObject *)WhileStatementObjectSpace.FindObject(condition);
	if(strobj2)
	{
		ConditionExpression = strobj2->content;
	}
	else
	{
		ConditionExpression = condition;
	}	
}
ScpWhileStatementObject::~ScpWhileStatementObject(void)
{
    if (whilexpressionblock)
    {
        delete whilexpressionblock;
        whilexpressionblock = nullptr;
    }
}
void ScpWhileStatementObject::Show(CScriptEngine * engine)
{

}
ScpObject * ScpWhileStatementObject::Clone(std::string strObjName)
{
	return NULL;
}
std::string ScpWhileStatementObject::ToString()
{
	std::string temp;
	temp+=ConditionExpression;
	temp+="\r\n";	
	return temp;
}
void ScpWhileStatementObject::Release() 
{
	delete this;
}
bool ScpWhileStatementObject::IsInnerFunction(std::string & functionname)
{
	return false;
}
ScpObject * ScpWhileStatementObject::CallInnerFunction(std::string & functionname, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	return nullptr;
}
int ScpWhileStatementObject::Do(CScriptEngine *engine)
{
	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	if(engine)
	{

	BOOL recurseCall = FALSE;
	if (currentObjectSpace->IsMyParentSpace(&WhileStatementObjectSpace))
	{
		recurseCall = TRUE;
	}
	if (!recurseCall)
	{
		engine->SetCurrentObjectSpace(&WhileStatementObjectSpace);
	}
	engine->GetCurrentObjectSpace()->breakingout = 0;
	engine->GetCurrentObjectSpace()->continuewhile = 0;
	engine->GetCurrentObjectSpace()->lastcommand = vl_compute;

		while(1)
		{
			if (!recurseCall)
			{
				engine->SetCurrentObjectSpace(WhileStatementObjectSpace.parentspace);
			}
			int condition = ReComputeConditionResult();
			if (!recurseCall)
			{
				engine->SetCurrentObjectSpace(&WhileStatementObjectSpace);
			}
			if (whilexpressionblock)
			{
				if (condition == 1)
				{
					whilexpressionblock->Run(engine);
					if (engine->GetCurrentObjectSpace()->breakingout)
					{
						break;
					}					
					continue;
				}
				else
				{
					break;
				}
			}			
		}
		ULONG ulLastCommand=engine->GetCurrentObjectSpace()->lastcommand;
		if (!recurseCall)
		{
			engine->SetCurrentObjectSpace(WhileStatementObjectSpace.parentspace);
		}
		engine->GetCurrentObjectSpace()->lastcommand=ulLastCommand;
		
	}

	return 0;
}
void ScpWhileStatementObject::ClearLocalObjects()
{
	WhileStatementObjectSpace.userobject.Destroy();
}