/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
//表达式块的组织管理
*/
#include "ScpExpressionBlock.h"
#include "ScriptEngine.h"
#include "ScpIfStatementObject.h"
#include "ScpWhileStatementObject.h"
#include "commanddefine_uni.h"

ScpExpressionBlock::ScpExpressionBlock():owner(NULL),result(NULL),  ulcommand(-1)
{
	objecttype = ObjExpressionBlock;
}


ScpExpressionBlock::~ScpExpressionBlock()
{
	Destroy();
}


void ScpExpressionBlock::Show(CScriptEngine * engine)
{
}

ScpObject * ScpExpressionBlock::Clone(std::string strObjName)
{
	return NULL;
}

std::string ScpExpressionBlock::ToString()
{
	return std::string();
}

void ScpExpressionBlock::Release()
{
	delete this;
}

bool ScpExpressionBlock::IsInnerFunction(std::string & functionname)
{
	return false;
}

ScpObject * ScpExpressionBlock::CallInnerFunction(std::string & functionname, VTPARAMETERS * parameters, CScriptEngine * engine)
{
	return NULL;
}

void ScpExpressionBlock::Append(std::string & expression,int line, CScriptEngine * engine)
{
	ScpExpressionBlock * block = new ScpExpressionBlock;
	if (block)
	{
		block->orgexpression = expression;
		block->linenumber = line;
		std::string & wcommandline = expression;
		ScpScriptLex lex;
		lex.Attach(engine);
		lex.ParseCommandLine(wcommandline, block->ulcommand, block->vt_command_parameters);
		Append(block);
	}

}

void ScpExpressionBlock::Append(ScpObject * block)
{
	if(block)
	expressionblocks.push_back(block);
}

void ScpExpressionBlock::Destroy()
{
	ScpObject * block = NULL;
	ITExpressionBlocks it = expressionblocks.begin();
	while (it != expressionblocks.end())
	{
		block = *it;
		it= expressionblocks.erase(it);
		if (block)
		{
			delete block;
		}
	}
}

bool ScpExpressionBlock::GenByteCode(CScriptEngine * engine, ByteCodeMemoryStream &bytecodemem)
{
	if (expressionblocks.size() > 0)
	{
		ScpObject * block = NULL;
		ITExpressionBlocks it = expressionblocks.begin();
		while (it != expressionblocks.end())
		{
			block = *it;
			if (block)
			{
				if (block->GetType() == ObjExpressionBlock)
				{
					((ScpExpressionBlock *)block)->GenByteCode(engine, bytecodemem);
				}
				else if (block->GetType() == ObjIfStatement)
				{
					ScpIfStatementObject *ifstmtobj = (ScpIfStatementObject * )block;
					ByteCodeMemoryStream stream;
					if (engine->Jit )
					{
						
						ifstmtobj->MakeConditionByteCode();
						engine->SetCurrentObjectSpace(&ifstmtobj->IfStatementObjectSpace);
						if (ifstmtobj->trueblock)
						{
							ifstmtobj->TrueBody = true;
							ifstmtobj->trueblock->GenByteCode(engine, ifstmtobj->trueblock_bytecodemem);
						}

						if (ifstmtobj->falseblock)
						{
							ifstmtobj->TrueBody = false;
							ifstmtobj->falseblock->GenByteCode(engine, ifstmtobj->falseblock_bytecodemem);
						}


						engine->getScriptByteCode().GenByteCodeIfstatement(ifstmtobj->condition_bytecodemem,
							ifstmtobj->trueblock_bytecodemem,
							ifstmtobj->falseblock_bytecodemem,
							stream);

						engine->SetCurrentObjectSpace(ifstmtobj->IfStatementObjectSpace.parentspace);
					}
					ifstmtobj->bytecodemem_ifstmt.AppendByteCode(&stream);
					stream.Release();
					bytecodemem.AppendByteCode(&((ScpIfStatementObject *)block)->bytecodemem_ifstmt);
				}
				else if (block->GetType() == ObjWhileStatement)
				{
					ScpWhileStatementObject* whileobj = (ScpWhileStatementObject*)block;
					ByteCodeMemoryStream stream;
					if (engine->Jit)
					{
						
						whileobj->MakeConditionByteCode();
						engine->SetCurrentObjectSpace(&whileobj->WhileStatementObjectSpace);
						whileobj->whilexpressionblock->GenByteCode(engine, whileobj->whileblock_bytecodemem);

						engine->getScriptByteCode().GenByteCodeWhilestatement(whileobj->condition_bytecodemem,
							whileobj->whileblock_bytecodemem,
							stream);

						engine->SetCurrentObjectSpace(whileobj->WhileStatementObjectSpace.parentspace);

						whileobj->bytecodemem_whilestmt.AppendByteCode(&stream);
					
						stream.Release();
						bytecodemem.AppendByteCode(&((ScpWhileStatementObject*)block)->bytecodemem_whilestmt);
					}

				}
			}
			ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
			if (currentObjectSpace->breakingout)
			{
				break;
			}
			else if (currentObjectSpace->continuewhile)
			{
				currentObjectSpace->continuewhile = 0;
				continue;
			}
			if (engine->GetCurrentObjectSpace()->lastcommand == vl_return)
			{
				break;
			}

			it++;
		}
	}
	else
	{
		engine->getScriptByteCode().GenByteCodeFromCommand(ulcommand, vt_command_parameters, bytecodemem, engine);
		if (ulcommand == vl_define)
		{
			engine->FetchCommand(ulcommand, &vt_command_parameters);
		}
	}
	return false;
}



ScpObject * ScpExpressionBlock::Run(CScriptEngine * engine)
{
	if (expressionblocks.size() > 0)
	{
		ScpObject * block = NULL;
		ITExpressionBlocks it = expressionblocks.begin();
		while (it != expressionblocks.end())
		{
			block = *it;			
			if (block)
			{
				if ( block->GetType()==ObjExpressionBlock)
				{
					((ScpExpressionBlock *)block)->Run(engine);
				}
				else if (block->GetType() == ObjIfStatement)
				{
					((ScpIfStatementObject *)block)->Do(engine);					
				}
				else if (block->GetType() == ObjWhileStatement)
				{
					((ScpWhileStatementObject*) block)->Do(engine);					
				}
			}
			ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
			if (currentObjectSpace->breakingout)
			{
				break;
			}
			else if (currentObjectSpace->continuewhile)
			{
				currentObjectSpace->continuewhile = 0;
				it = expressionblocks.begin();
				continue;
			}
			if (engine->GetCurrentObjectSpace()->lastcommand == vl_return)
			{
				break;
			}

			it++;
		}
	}
	else
	{
		if (ulcommand != -1)
		{
			IScriptDebugger* debugger = engine->getDebugger();
			if (debugger)
			{
				debugger->CheckDebugEvent(engine->getCurrentScriptFileName().c_str(), linenumber, INFINITE);
			}
			engine->FetchCommand(ulcommand, &vt_command_parameters);
			if (engine->GetCurrentObjectSpace()->lastcommand != vl_return)
				engine->GetCurrentObjectSpace()->lastcommand = ulcommand;
		}
	}

	return NULL;
}
