#include "ScriptByteCode.h"
#include "ScpGlobalObject.h"
#include "ScpByteCode.h"
#include "ScpScriptLex.h"
#include "ScriptEngine.h"
#include "ScpExpressionAnalyser.h"
#include "ScpStringObject.h"
#include "ScpFunctionObject.h"
#include "ScpObjectNammes.h"
#include "commanddefine_uni.h"
#include "../Common/commonutil.hpp"
#include "../Common/stdstringext.hpp"
ScriptByteCode::ScriptByteCode() :bytecodemem(NULL), resourcepool(NULL)
{
	Init();
}


ScriptByteCode::~ScriptByteCode()
{
	if (bytecodemem)
		delete bytecodemem;
	if (resourcepool)
		delete resourcepool;

}

bool ScriptByteCode::GenByteCodeFromSouceLine(std::string& wsSourceLine, ByteCodeMemoryStream& memstream)
{
	int language = 0;
	if (wsSourceLine == "#scpeng")
	{
		language = 1;
		int codelen = sizeof(unsigned char) + sizeof(unsigned int);
		unsigned char *bytecode = (unsigned char *)malloc(codelen);
		if (bytecode)
		{
			unsigned char * pcode = bytecode;
			*pcode = BC_SWITCHLANG;
			pcode++;
			memcpy(pcode, (void*)&language, sizeof(language));
			pcode += sizeof(language);
			memstream.AppendByteCode(bytecode, codelen);
			free(bytecode);
		}
	}
	else if (wsSourceLine == "#scpchs")
	{
		language = 0;
		int codelen = sizeof(unsigned char) + sizeof(unsigned int);
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			*pcode = BC_SWITCHLANG;
			pcode++;
			memcpy(pcode, (void*)&language, sizeof(language));
			pcode += sizeof(language);
			memstream.AppendByteCode(code, codelen);
			free(code);
		}
	}
	return false;
}

bool ScriptByteCode::GenByteCodeFromCommand(const unsigned int & commandvalue, VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream, CScriptEngine * engine)
{
	switch (commandvalue)
	{
	case vl_define:
	{
		std::string & strobjtype = vtparameters.at(0);
		ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobjtype.c_str());
		if (engine->extend_obj_mgr.IsExtendObject(strobjtype.c_str()))
		{
			type = engine->extend_obj_mgr.GetExtendObjectId(strobjtype.c_str());
			GenByteCodeObjectDefine(type, vtparameters, memstream);
		}
		else if (ObjString == type ||
			ObjInt == type||
			ObjDouble ==type||
			ObjBigInt ==type||
			ObjRegExp == type||
			ObjJson ==type||
			ObjFile ==type||
			ObjDirectory == type||
			ObjMemory == type||
			ObjTable == type ||
			ObjStack == type ||
			ObjQueue == type ||
			ObjMap ==type||
			ObjTime == type)
		{
			GenByteCodeObjectDefine(type, vtparameters, memstream);
			ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
			if(currentObjectSpace->ObjectSpaceType!=Space_Global)
				engine->FetchCommand(commandvalue, &vtparameters);
		}
		else if (ObjFunction == type)
		{
			GenByteCodeFunctionDefine(vtparameters, memstream, engine);			
		}
		else if (ObjClass == type)
		{
			GenByteCodeClassObjectDefine(type, vtparameters, memstream);
		}
		else
		{
			ScpObject * objclass = engine->GetCurrentObjectSpace()->FindObject(strobjtype);
			if (objclass  && objclass->GetType() == ObjClass)
			{
				type = ObjClassInstance;
				GenByteCodeClassInstanceDefine(type, vtparameters, memstream);
				ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
				if (currentObjectSpace->ObjectSpaceType != Space_Global)
					engine->FetchCommand(commandvalue, &vtparameters);
			}

		}
		break;
	}
	case vl_load:
	{
		GenByteCodeLoad(vtparameters, memstream);		
		break;
	}
	case vl_import:
	{
		GenByteCodeImport(vtparameters, memstream);
		break;
	}
	case vl_public:
	{
		memstream.AppendByteCode((unsigned char *)&BC_PUBLIC, 1);
		break;
	}
	case vl_private:
	{
		memstream.AppendByteCode((unsigned char *)&BC_PRIVATE, 1);
		break;
	}
	case vl_end:
	{
		memstream.AppendByteCode((unsigned char *)&BC_END, 1);
		break;
	}
	case vl_compute:
	{
		GetByteCodeCompute(vtparameters, memstream, engine);
		break;
	}
	case vl_return:
	{
		GenByteCodeReturn(vtparameters, memstream, engine);
		break;
	}
	case vl_break:
	{
		memstream.AppendByteCode((unsigned char *)&BC_BREAK, 1);
		break;
	}
	case vl_continue:
	{
		memstream.AppendByteCode((unsigned char *)&BC_CONTINUE, 1);
		break;
	}
	case vl_call:
	{
		if (vtparameters.size() > 0)
		{
			ScpObjectSpace* objectSpace = engine->GetCurrentObjectSpace();
			ScpFunctionObject* func = (ScpFunctionObject*)engine->GetCurrentObjectSpace()->FindObject(vtparameters.at(0));
			if (func)
			{
				ScpObjectSpace* parentspaceold = func->FunctionObjectSpace->parentspace;
				func->FunctionObjectSpace->parentspace = objectSpace;
				engine->SetCurrentObjectSpace(func->FunctionObjectSpace);
				func->RealParameters.clear();
				if (func->RealParameters.size() == 0)
				{
					for (ULONG i = 1;i < vtparameters.size();i++)
					{
						ScpObject* obj = objectSpace->FindObject(vtparameters.at(i));
						if (obj)
						{
							func->RealParameters.push_back(vtparameters.at(i));
						}
						else
						{

							//这里需要考虑函数的参数是表达式的情况
							std::string Expression = vtparameters.at(i);
							ULONG residexp = resourcepool->scpFindResource(vtparameters.at(i));
							ULONG residname;
							if (residexp == -1)
							{
								ByteCodeMemoryStream stream;
								VTPARAMETERS param;
								if (IsStaticNumber(Expression))
								{
									std::string name = "tempint";
									residname = resourcepool->scpFindResource(name);
									while (residname != -1)
									{
										name += "0";
										residname = resourcepool->scpFindResource(name);
									}
									residname = resourcepool->AppendResource(name);

									GetByteCodeInitRes(name, stream, residname);
									bytecodemem->AppendByteCode(&stream);
									stream.Release();

									param.push_back("int");
									param.push_back(name);
									param.push_back(Expression);
									GenByteCodeObjectDefine(ObjInt, param, stream);

									bytecodemem->AppendByteCode(&stream);
									stream.Release();
									func->RealParameters.push_back(name);

									ScpIntObject* intobj = new  ScpIntObject;
									intobj->value = StringToInt(Expression);
									ScpObjectSpace* globalObjectSpace = engine->GetCurrentObjectSpace();
									while (globalObjectSpace->parentspace != NULL)
									{
										globalObjectSpace = globalObjectSpace->parentspace;
									}
									globalObjectSpace->AddObject(name, intobj);
								}
								else if (IsStaticString(Expression))
								{
									std::string name = "tempstring";
									residname = resourcepool->scpFindResource(name);
									while (residname != -1)
									{
										name += "0";
										residname = resourcepool->scpFindResource(name);
									}
									residname = resourcepool->AppendResource(name);

									GetByteCodeInitRes(name, stream, residname);
									bytecodemem->AppendByteCode(&stream);
									stream.Release();

									param.push_back("string");
									param.push_back(name);
									param.push_back(Expression);
									GenByteCodeObjectDefine(ObjString, param, stream);

									bytecodemem->AppendByteCode(&stream);
									stream.Release();
									func->RealParameters.push_back(name);

									ScpStringObject* StrObj = new  ScpStringObject;
									StringStripQuote(Expression);
									StrObj->content = Expression;
									ScpObjectSpace* globalObjectSpace = engine->GetCurrentObjectSpace();
									while (globalObjectSpace->parentspace != NULL)
									{
										globalObjectSpace = globalObjectSpace->parentspace;
									}
									globalObjectSpace->AddObject(name, StrObj);
								}
							}
						}
					}
				}
				if (func->BindParameters(engine) != -1)
				{
					std::string objname = objectSpace->GetObjectNameR(func);

					VTPARAMETERS param;
					for (int i = 1;i < vtparameters.size();i++)
					{
						param.push_back(vtparameters.at(i));
					}
					//函数没有调用过，没有生成函数定义和函数体的字节码
					if (func->bytecodemem_funcbody.codelength == 0)
					{

						ByteCodeMemoryStream stream;
						func->functionexpressionblock->GenByteCode(engine, stream);
						GenByteCodeFunctionBody(stream);
						func->bytecodemem_funcbody.AppendByteCode(&stream);

						bytecodemem->AppendByteCode(&func->bytecodemem_funcdef);
						bytecodemem->AppendByteCode(&func->bytecodemem_funcbody);

					}

					ByteCodeMemoryStream stream;
					GenByteCodeCallFunc(objname, param, stream);
					memstream.AppendByteCode(&stream);
					stream.Release();
					func->UnBindParameters(engine);
					func->RealParameters.clear();
				}
				engine->SetCurrentObjectSpace(objectSpace);
				func->FunctionObjectSpace->parentspace = parentspaceold;
			}
		}
		break;
	}
	case vl_loop:
	{
		GetByteCodeLoop(vtparameters, memstream, engine);
		break;
	}

	default:
		break;
	}
	return false;
}

bool ScriptByteCode::GenByteCodeObjectDefine(ScpObjectType type, VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream)
{
	if (vtparameters.size() == 3 || vtparameters.size() == 2)
	{
		ULONG residobjname;
		ULONG residobjcontent;
		std::string name = vtparameters.at(1);
		std::string content;
		unsigned int codelen = 0;
		residobjname = resourcepool->scpFindResource(name);
		if (residobjname == -1)
		{
			residobjname = resourcepool->AppendResource(name);
			std::string nameutf = name;
			int namelen = nameutf.length() + 1;
			codelen = sizeof(unsigned char) + sizeof(unsigned int) + namelen;
			unsigned char * code = (unsigned char *)malloc(codelen);
			if (code)
			{
				unsigned char * pcode = code;
				*pcode = BC_INITRES;
				pcode++;

				memcpy(pcode, (void*)&namelen, sizeof(namelen));
				pcode += sizeof(namelen);
				memcpy(pcode, nameutf.c_str(), nameutf.length());
				pcode += nameutf.length();
				*pcode = 0;
				pcode++;
				bytecodemem->AppendByteCode(code, codelen);
				free(code);
			}
		}
		if (vtparameters.size() == 3)
		{
			content = vtparameters.at(2);
			residobjcontent = resourcepool->AppendResource(content);
		}
		else if (vtparameters.size() == 2)
		{
			residobjcontent = 0;
		}
		std::string contentutf = content;
		int contentlen = contentutf.length() + 1;
		if (contentlen > 1)
		{
			codelen = sizeof(unsigned char) + sizeof(unsigned int) + contentlen;
			unsigned char * code = (unsigned char *)malloc(codelen);
			if (code)
			{
				unsigned char * pcode = code;
				*pcode = BC_INITRES;
				pcode++;

				memcpy(pcode, (void*)&contentlen, sizeof(contentlen));
				pcode += sizeof(contentlen);
				memcpy(pcode, contentutf.c_str(), contentutf.length());
				pcode += contentutf.length();
				*pcode = 0;
				pcode++;
				bytecodemem->AppendByteCode(code, codelen);
				free(code);
			}
		}
		codelen = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned int) + sizeof(unsigned int);
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			*pcode = BC_OBJECT_DEFINE;
			pcode++;
			*pcode = (unsigned char)type;
			pcode++;
			memcpy(pcode, &residobjname, sizeof(unsigned int));
			pcode += sizeof(unsigned int);
			memcpy(pcode, &residobjcontent, sizeof(unsigned int));
			memstream.AppendByteCode(code, codelen);
			free(code);
			return true;
		}
	}
	return false;
}
bool ScriptByteCode::GenByteCodeClassObjectDefine(ScpObjectType type, VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream)
{
	if (vtparameters.size() == 2)
	{
		ULONG residobjname;
		std::string name = vtparameters.at(1);
		std::string content;
		unsigned int codelen = 0;
		residobjname = resourcepool->scpFindResource(name);
		if (residobjname == -1)
		{
			residobjname = resourcepool->AppendResource(name);
			std::string nameutf = name;
			int namelen = nameutf.length() + 1;
			codelen = sizeof(unsigned char) + sizeof(unsigned int) + namelen;
			unsigned char * code = (unsigned char *)malloc(codelen);
			if (code)
			{
				unsigned char * pcode = code;
				*pcode = BC_INITRES;
				pcode++;

				memcpy(pcode, (void*)&namelen, sizeof(namelen));
				pcode += sizeof(namelen);
				memcpy(pcode, nameutf.c_str(), nameutf.length());
				pcode += nameutf.length();
				*pcode = 0;
				pcode++;
				bytecodemem->AppendByteCode(code, codelen);
				free(code);
			}
		}
		codelen = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned int);
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			*pcode = BC_OBJECT_CLASS_DEFINE;
			pcode++;
			*pcode = (unsigned char)type;
			pcode++;
			memcpy(pcode, &residobjname, sizeof(unsigned int));			
			memstream.AppendByteCode(code, codelen);
			free(code);
			return true;
		}
	}
	return false;
}

bool ScriptByteCode::GenByteCodeClassInstanceDefine(ScpObjectType type, VTPARAMETERS & vtparameters, ByteCodeMemoryStream & memstream)
{
	if (vtparameters.size() == 2)
	{
		ULONG residclassname;
		ULONG residobjname;
		std::string classname = vtparameters.at(0);
		std::string name = vtparameters.at(1);
		std::string content;
		unsigned int codelen = 0;
		residclassname = resourcepool->scpFindResource(classname);
		residobjname = resourcepool->scpFindResource(name);
		if (residobjname == -1)
		{
			residobjname = resourcepool->AppendResource(name);
			std::string nameutf = name;
			int namelen = nameutf.length() + 1;
			codelen = sizeof(unsigned char) + sizeof(unsigned int) + namelen;
			unsigned char * code = (unsigned char *)malloc(codelen);
			if (code)
			{
				unsigned char * pcode = code;
				*pcode = BC_INITRES;
				pcode++;

				memcpy(pcode, (void*)&namelen, sizeof(namelen));
				pcode += sizeof(namelen);
				memcpy(pcode, nameutf.c_str(), nameutf.length());
				pcode += nameutf.length();
				*pcode = 0;
				pcode++;
				bytecodemem->AppendByteCode(code, codelen);
				free(code);
			}
		}
		codelen = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned int)+ sizeof(unsigned int);
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			*pcode = BC_OBJECT_DEFINE;
			pcode++;
			*pcode = (unsigned char)type;
			pcode++;
			memcpy(pcode, &residclassname, sizeof(unsigned int));
			pcode += sizeof(unsigned int);
			memcpy(pcode, &residobjname, sizeof(unsigned int));
			memstream.AppendByteCode(code, codelen);
			free(code);
			return true;
		}
	}
	return false;
}

bool ScriptByteCode::GenByteCodeFunctionDefine(VTPARAMETERS & vtparameters, ByteCodeMemoryStream & memstream, CScriptEngine * engine)
{
	if (vtparameters.size() >= 2)
	{
		ULONG residobjname;		
		std::string name = vtparameters.at(1);	
		ScpFunctionObject * func=(ScpFunctionObject * )engine->GetCurrentObjectSpace()->FindObject(name);
		ULONG residparentobj=0;
		if(func->ParenObject)
			residparentobj = resourcepool->scpFindResource(func->parentobjectname);
		unsigned int codelen = 0;
		residobjname = resourcepool->scpFindResource(name);
		if (residobjname == -1)
		{
			residobjname = resourcepool->AppendResource(name);
			std::string nameutf = name;
			int namelen = nameutf.length() + 1;
			codelen = sizeof(unsigned char) + sizeof(unsigned int) + namelen;
			unsigned char * code = (unsigned char *)malloc(codelen);
			if (code)
			{
				unsigned char * pcode = code;
				*pcode = BC_INITRES;
				pcode++;

				memcpy(pcode, (void*)&namelen, sizeof(namelen));
				pcode += sizeof(namelen);
				memcpy(pcode, nameutf.c_str(), nameutf.length());
				pcode += nameutf.length();
				*pcode = 0;
				pcode++;
				//memstream.AppendByteCode(code, codelen);
				//直接添加到全局字节码
				bytecodemem->AppendByteCode(code, codelen);
				free(code);
			}
		}
		int paramcount = vtparameters.size() - 2;
		
		for (int i = 2;i < vtparameters.size();i++)
		{
			std::string paramname = vtparameters.at(i);
			ULONG residparam = resourcepool->AppendResource(paramname);
			std::string paramnameutf = paramname;

			int contentlen = paramnameutf.length() + 1;
			if (contentlen > 1)
			{
				codelen = sizeof(unsigned char) + sizeof(unsigned int) + contentlen;
				unsigned char * code = (unsigned char *)malloc(codelen);
				if (code)
				{
					unsigned char * pcode = code;
					*pcode = BC_INITRES;
					pcode++;

					memcpy(pcode, (void*)&contentlen, sizeof(contentlen));
					pcode += sizeof(contentlen);
					memcpy(pcode, paramnameutf.c_str(), paramnameutf.length());
					pcode += paramnameutf.length();
					*pcode = 0;
					pcode++;
					//memstream.AppendByteCode(code, codelen);
					bytecodemem->AppendByteCode(code, codelen);
					free(code);
				}
			}
		}		
		codelen = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int);
		codelen += sizeof(unsigned int)*paramcount;
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			*pcode = BC_OBJECT_FUNCTION_DEFINE;
			pcode++;
			*pcode = (unsigned char)ObjFunction;
			pcode++;
			memcpy(pcode, &residobjname, sizeof(unsigned int));
			pcode += sizeof(unsigned int);

			memcpy(pcode, &residparentobj, sizeof(unsigned int));
			pcode += sizeof(unsigned int);


			memcpy(pcode, (unsigned char *)&paramcount, sizeof(paramcount));
			pcode += sizeof(paramcount);
			
			for (int i = 2;i < vtparameters.size();i++)
			{
				std::string paramname = vtparameters.at(i);
				ULONG paramresid = resourcepool->scpFindResource(paramname);

				memcpy(pcode, (unsigned char *)&paramresid, sizeof(paramresid));
				pcode += sizeof(paramresid);
			}			
			memstream.AppendByteCode(code, codelen);
			free(code);
			return true;
		}
	}
	return false;
}

bool ScriptByteCode::GenByteCodeLoad(VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream)
{
	std::string & strobjtype = vtparameters.at(0);
	if (strobjtype == ScpObjectNames::GetSingleInsatnce()->strObjExtObj)
	{
		if (vtparameters.size() == 2)
		{
			ULONG residobjname;
			std::string name = vtparameters.at(1);
			unsigned int codelen = 0;
			if (resourcepool->scpFindResource(name) == -1)
			{
				residobjname = resourcepool->AppendResource(name);
				std::string nameutf = name;
				int namelen = nameutf.length() + 1;
				codelen = sizeof(unsigned char) + sizeof(unsigned int) + namelen;
				unsigned char * code = (unsigned char *)malloc(codelen);
				if (code)
				{
					unsigned char * pcode = code;
					*pcode = BC_INITRES;
					pcode++;

					memcpy(pcode, (void*)&namelen, sizeof(namelen));
					pcode += sizeof(namelen);
					memcpy(pcode, nameutf.c_str(), nameutf.length());
					pcode += nameutf.length();
					*pcode = 0;
					pcode++;
					bytecodemem->AppendByteCode(code, codelen);
					free(code);
				}
			}
			codelen = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned int);
			unsigned char * code = (unsigned char *)malloc(codelen);
			if (code)
			{
				unsigned char * pcode = code;
				*pcode = BC_LOAD;
				pcode++;
				*pcode = (unsigned char)ObjExt;
				pcode++;
				memcpy(pcode, &residobjname, sizeof(unsigned int));
				pcode += sizeof(unsigned int);
				memstream.AppendByteCode(code, codelen);
				free(code);
				return true;
			}
		}
	}
	return false;
}

bool ScriptByteCode::GenByteCodeImport(VTPARAMETERS & vtparameters, ByteCodeMemoryStream & memstream)
{
	std::string & strobjtype = vtparameters.at(0);
	if (strobjtype == str_EN_ObjLib)
	{
		if (vtparameters.size() == 2)
		{
			ULONG residobjname;
			std::string name = vtparameters.at(1);
			StringStripQuote(name);
			unsigned int codelen = 0;
			if (resourcepool->scpFindResource(name) == -1)
			{
				residobjname = resourcepool->AppendResource(name);
				std::string nameutf = name;
				int namelen = nameutf.length() + 1;
				codelen = sizeof(unsigned char) + sizeof(unsigned int) + namelen;
				unsigned char * code = (unsigned char *)malloc(codelen);
				if (code)
				{
					unsigned char * pcode = code;
					*pcode = BC_INITRES;
					pcode++;

					memcpy(pcode, (void*)&namelen, sizeof(namelen));
					pcode += sizeof(namelen);
					memcpy(pcode, nameutf.c_str(), nameutf.length());
					pcode += nameutf.length();
					*pcode = 0;
					pcode++;
					bytecodemem->AppendByteCode(code, codelen);
					free(code);
				}
			}
			codelen = sizeof(unsigned char) + sizeof(unsigned char) + sizeof(unsigned int);
			unsigned char * code = (unsigned char *)malloc(codelen);
			if (code)
			{
				unsigned char * pcode = code;
				*pcode = BC_IMPORT;
				pcode++;
				*pcode = (unsigned char)ObjLib;
				pcode++;
				memcpy(pcode, &residobjname, sizeof(unsigned int));
				pcode += sizeof(unsigned int);
				memstream.AppendByteCode(code, codelen);
				free(code);
				return true;
			}
		}
	}
	return false;
}

bool ScriptByteCode::GetByteCodeInitRes(std::string res, ByteCodeMemoryStream & memstream, ULONG &resid)
{
	std::string name = res;
	unsigned int codelen = 0;
	//if (resourcepool->scpFindResource(name) == -1)
	{
		resid = resourcepool->scpFindResource(name);
		std::string nameutf = name;
		int namelen = nameutf.length() + 1;
		codelen = sizeof(unsigned char) + sizeof(unsigned int) + namelen;
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			*pcode = BC_INITRES;
			pcode++;

			memcpy(pcode, (void*)&namelen, sizeof(namelen));
			pcode += sizeof(namelen);
			memcpy(pcode, nameutf.c_str(), nameutf.length());
			pcode += nameutf.length();
			*pcode = 0;
			pcode++;
			memstream.AppendByteCode(code, codelen);
			free(code);
			return true;
		}
	}
	return false;
}

bool ScriptByteCode::GetByteCodeBinaryOp(std::string op, ULONG idret, ULONG idleft, ULONG idright, ByteCodeMemoryStream& memstream)
{
	unsigned int codelen = 0;
	if (scpOperationLessthan == op ||
		scpOperationLessorEqual == op ||
		scpOperationBigthan == op ||
		scpOperationBigorEqual == op ||
		scpOperationEqual == op ||
		scpOperationNotEqual == op ||
		scpOperationAdd == op ||
		scpOperationSubtraction == op ||
		scpOperationMultiplication == op ||
		scpOperationDivision == op ||
		scpOperationMod == op ||
		scpOperationBitAnd == op ||
		scpOperationBitOr == op ||
		scpOperationBitXor == op ||
		scpOperationLogicalAnd == op ||
		scpOperationLogicalOr == op ||
		scpOperationBitShiftLeft == op ||
		scpOperationBitShiftRight == op)
	{
		codelen = sizeof(unsigned char) + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int);
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			if (scpOperationLessthan == op)
				*pcode = BC_OBJECT_LESSTHAN;
			else if (scpOperationLessorEqual == op)
				*pcode = BC_OBJECT_LESSOREQUAL;
			else if (scpOperationBigthan == op)
				*pcode = BC_OBJECT_BIGTHAN;
			else if (scpOperationBigorEqual == op)
				*pcode = BC_OBJECT_BIGOREQUAL;
			else if (scpOperationEqual == op)
				*pcode = BC_OBJECT_EQUAL;
			else if (scpOperationNotEqual == op)
				*pcode = BC_OBJECT_NOTEQUAL;
			else if (scpOperationAdd == op)
				*pcode = BC_OBJECT_ADD;
			else if (scpOperationSubtraction == op)
				*pcode = BC_OBJECT_SUB;
			else if (scpOperationMultiplication == op)
				*pcode = BC_OBJECT_MUL;
			else if (scpOperationDivision == op)
				*pcode = BC_OBJECT_DIV;
			else if (scpOperationMod == op)
				*pcode = BC_OBJECT_MOD;			
			else if (scpOperationBitAnd == op)
				*pcode = BC_OBJECT_BITAND;
			else if (scpOperationBitOr == op)
				*pcode = BC_OBJECT_BITOR;
			else if (scpOperationBitXor == op)
				*pcode = BC_OBJECT_BITXOR;
			else if (scpOperationLogicalAnd == op)
				*pcode = BC_OBJECT_LOGICALAND;
			else if (scpOperationLogicalOr == op)
				*pcode = BC_OBJECT_LOGICALOR;
			else if (scpOperationBitShiftLeft == op)
				*pcode = BC_OBJECT_BITSHIFTLEFT;
			else if (scpOperationBitShiftRight == op)
				*pcode = BC_OBJECT_BITSHIFTRIGHT;
			pcode++;

			memcpy(pcode, (void*)&idret, sizeof(idret));
			pcode += sizeof(idret);
			memcpy(pcode, (void*)&idleft, sizeof(idleft));
			pcode += sizeof(idleft);
			memcpy(pcode, (void*)&idright, sizeof(idright));
			pcode += sizeof(idright);
			memstream.AppendByteCode(code, codelen);
			free(code);
			return true;
		}
	}
	else if (scpOperationAssign == op ||
		scpOperationAddAndAssign == op ||
		scpOperationSubAndAssign == op ||
		scpOperationMulAndAssign == op ||
		scpOperationDivAndAssign == op ||
		scpOpeartionModAndAssign == op ||
		scpOperationBitAndAndAssign == op||
		scpOperationBitOrAndAssign == op||
		scpOperationBitNotAndAssign == op||
		scpOperationBitXorAndAssign == op||
		scpOpeartionBitShiftLeftAndAssign == op||
		scpOpeartionBitShiftRightAndAssign == op)
	{
		codelen = sizeof(unsigned char) + sizeof(unsigned int) + sizeof(unsigned int);
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			if (scpOperationAssign == op)
				*pcode = BC_OBJECT_ASSIGN;
			else if (scpOperationAddAndAssign == op)
				*pcode = BC_OBJECT_ADD_AND_ASSIGN;
			else if (scpOperationSubAndAssign == op)
				*pcode = BC_OBJECT_SUB_AND_ASSIGN;
			else if (scpOperationMulAndAssign == op)
				*pcode = BC_OBJECT_MUL_AND_ASSIGN;
			else if(scpOperationDivAndAssign ==op)
				*pcode = BC_OBJECT_DIV_AND_ASSIGN;
			else if(scpOpeartionModAndAssign == op)
				*pcode = BC_OBJECT_MOD_AND_ASSIGN;
			else if (scpOperationBitAndAndAssign == op)
				*pcode = BC_OBJECT_BITAND_AND_ASSIGN;
			else if (scpOperationBitOrAndAssign == op)
				*pcode = BC_OBJECT_BITOR_AND_ASSIGN;
			else if (scpOperationBitNotAndAssign == op)
				*pcode = BC_OBJECT_BITNOT_AND_ASSIGN;
			else if (scpOperationBitXorAndAssign == op)
				*pcode = BC_OBJECT_BITXOR_AND_ASSIGN;
			else if (scpOpeartionBitShiftLeftAndAssign == op)
				*pcode = BC_OBJECT_BITSHIFTLEFT_AND_ASSIGN;
			else if (scpOpeartionBitShiftRightAndAssign == op)
				*pcode = BC_OBJECT_BITSHIFTRIGHT_AND_ASSIGN;
			pcode++;
			memcpy(pcode, (void*)&idleft, sizeof(idleft));
			pcode += sizeof(idleft);
			memcpy(pcode, (void*)&idright, sizeof(idright));
			pcode += sizeof(idright);
			memstream.AppendByteCode(code, codelen);
			free(code);
			return true;
		}
	}

	return false;
}

bool ScriptByteCode::GenByteCodeUnaryOp(std::string op, ULONG idobj, ByteCodeMemoryStream & memstream)
{
	unsigned int codelen = 0;
	codelen = sizeof(unsigned char) + sizeof(unsigned int);
	unsigned char * code = (unsigned char *)malloc(codelen);
	if (code)
	{
		unsigned char * pcode = code;
		if (scpOperationSelfAdd == op)
			*pcode = BC_OBJECT_SELFADD;
		else if (scpOperationSelfSub == op)
			*pcode = BC_OBJECT_SELFSUB;
		pcode++;
		memcpy(pcode, (void*)&idobj, sizeof(idobj));
		pcode += sizeof(idobj);
		memstream.AppendByteCode(code, codelen);
		free(code);
		return true;
	}
	return false;
}

bool ScriptByteCode::GenByteCodeCallInner(std::string objectname, std::string innerfuncname, VTPARAMETERS & vtparameters, ByteCodeMemoryStream & memstream, CScriptEngine * engine)
{
	unsigned int codelen = 0;
	int paramcount = vtparameters.size();
	{
		codelen = SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID;
		codelen += SIZE_OF_RESOURCEID * paramcount;
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			*pcode = BC_CALL_INNER;
			pcode++;
			ULONG id = resourcepool->scpFindResource(objectname);
			memcpy(pcode, (void*)&id, sizeof(id));
			pcode += sizeof(id);

			id = resourcepool->scpFindResource(innerfuncname);
			memcpy(pcode, (void*)&id, sizeof(id));
			pcode += sizeof(id);

			memcpy(pcode, (void*)&paramcount, sizeof(paramcount));
			pcode += sizeof(paramcount);

			for (int i = 0;i < paramcount;i++)
			{
				std::string param = vtparameters.at(i);
				ScpObject * objparam = engine->GetCurrentObjectSpace()->FindObject(param);
				if (objparam &&objparam->GetType() == ObjString)
				{
					param = ((ScpStringObject *)objparam)->content;
				}
				id = resourcepool->scpFindResource(param);
				memcpy(pcode, (void*)&id, sizeof(id));
				pcode += sizeof(id);
			}


			memstream.AppendByteCode(code, codelen);
			free(code);
			return true;
		}
	}
	return false;
}

bool ScriptByteCode::GenByteCodeIfstatement(ByteCodeMemoryStream & streamcondition, ByteCodeMemoryStream & streamtrueblock, ByteCodeMemoryStream & streamfalseblock, ByteCodeMemoryStream & memstreamif)
{
	short conditionblockCommandCount = streamcondition.GetByteCodeCommandCount();
	short trueblockCommandCount = streamtrueblock.GetByteCodeCommandCount();
	short falseblockCommandCount = streamfalseblock.GetByteCodeCommandCount();
	int codelength = SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK + SIZE_OF_BOLCK + SIZE_OF_BOLCK;
	unsigned char * code = (unsigned char *)malloc(codelength);
	if (code)
	{
		unsigned char * pcode = code;
		*pcode = BC_IF;
		pcode++;
		memcpy(pcode, &conditionblockCommandCount, sizeof(conditionblockCommandCount));
		pcode += sizeof(conditionblockCommandCount);
		memcpy(pcode, &trueblockCommandCount, sizeof(trueblockCommandCount));
		pcode += sizeof(trueblockCommandCount);
		memcpy(pcode, &falseblockCommandCount, sizeof(falseblockCommandCount));
		pcode += sizeof(falseblockCommandCount);
		memstreamif.AppendByteCode(code, codelength);
		free(code);
	}
	memstreamif.AppendByteCode(&streamcondition);
	memstreamif.AppendByteCode(&streamtrueblock);
	memstreamif.AppendByteCode(&streamfalseblock);
	memstreamif.AppendByteCode((unsigned char *)&BC_END, 1);
	return false;
}

bool ScriptByteCode::GenByteCodeFunctionBody(ByteCodeMemoryStream & memstreamfunc)
{
	memstreamfunc.AppendByteCode((unsigned char *)&BC_END, 1);
	return false;
}

bool ScriptByteCode::GetByteCodeCompute(VTPARAMETERS & vtparameters, ByteCodeMemoryStream & memstream, CScriptEngine * engine)
{
	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	for (int i = 0;i < vtparameters.size();i++)
	{
		std::string expression = vtparameters.at(i);
		ScpObject *obj2 = currentObjectSpace->FindObject(expression);
		if (obj2 && obj2->GetType() == ObjString)
		{
			expression = ((ScpStringObject*)obj2)->content;
		}
		
			ScpExpressionTreeNode *root = engine->getExpressionAnalyser().BuildExpressionTreeEx(expression);
			if (root)
			{
				ByteCodeMemoryStream stream;
				ScpObject * retobj = root->MakeByteCode(engine, stream);
				memstream.AppendByteCode(&stream);
				stream.Release();
				/*if (retobj)
				{
					if (retobj->istemp)
					{
						currentObjectSpace->ReleaseTempObject(retobj);
					}
				}*/
				root->Destroy(currentObjectSpace);
				engine->GetCurrentObjectSpace()->ReleaseTempObject(root);
			}		
	}
	return false;
}

bool ScriptByteCode::GenByteCodeCallFunc(std::string funcname, VTPARAMETERS & vtparameters, ByteCodeMemoryStream & memstream)
{
	unsigned int codelen = 0;
	int paramcount = vtparameters.size();
	{
		codelen = SIZE_OF_BYTECODE_COMMAND + SIZE_OF_RESOURCEID + SIZE_OF_RESOURCEID ;
		codelen += SIZE_OF_RESOURCEID * paramcount;
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			*pcode = BC_CALL;
			pcode++;
			ULONG id = resourcepool->scpFindResource(funcname);
			memcpy(pcode, (void*)&id, sizeof(id));
			pcode += sizeof(id);


			memcpy(pcode, (void*)&paramcount, sizeof(paramcount));
			pcode += sizeof(paramcount);

			for (int i = 0;i < paramcount;i++)
			{
				id = resourcepool->scpFindResource(vtparameters.at(i));
				memcpy(pcode, (void*)&id, sizeof(id));
				pcode += sizeof(id);
			}


			memstream.AppendByteCode(code, codelen);
			free(code);
			return true;
		}
	}
	return false;
}

bool ScriptByteCode::GenByteCodeReturn(VTPARAMETERS & vtparameters, ByteCodeMemoryStream & memstream, CScriptEngine * engine)
{
	ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	if (vtparameters.size() == 1)
	{
		std::string expression = vtparameters.at(0);
		ScpStringObject *strobjexpression = (ScpStringObject *)currentObjectSpace->FindObject(expression);
		if (strobjexpression)
		{
			if (strobjexpression->GetType() == ObjString)
			{
				expression = strobjexpression->content;
			}
		}
		
			ScpExpressionTreeNode *root = engine->getExpressionAnalyser().BuildExpressionTreeEx(expression);
			if (root)
			{
				ByteCodeMemoryStream stream;
				ScpObject * retobj = root->MakeByteCode(engine, stream);
				memstream.AppendByteCode(&stream);
				stream.Release();
				if (retobj)
				{
					std::string name = currentObjectSpace->GetObjectNameR(retobj);
					ULONG resid = resourcepool->scpFindResource(name);
					memstream.AppendByteCode((unsigned char*)&BC_RETURN, 1);
					memstream.AppendByteCode((unsigned char*)&resid, sizeof(resid));
					if (currentObjectSpace->ObjectSpaceType == Space_Function)
					{
						ScpFunctionObject * func = (ScpFunctionObject*)currentObjectSpace->belongto;
						func->Result = retobj;
					}
				}
				root->Destroy(engine->GetCurrentObjectSpace());
				engine->GetCurrentObjectSpace()->ReleaseTempObject(root);
			}
			
	}
	return false;
}

bool ScriptByteCode::GenByteCodeWhilestatement(ByteCodeMemoryStream & streamcondition, ByteCodeMemoryStream & streamwhileblock, ByteCodeMemoryStream & memstreamwhile)
{
	short conditionblockCommandCount = streamcondition.GetByteCodeCommandCount();
	short blockCommandCount = streamwhileblock.GetByteCodeCommandCount();
	int codelength = SIZE_OF_BYTECODE_COMMAND + SIZE_OF_BOLCK + SIZE_OF_BOLCK ;
	unsigned char * code = (unsigned char *)malloc(codelength);
	if (code)
	{
		unsigned char * pcode = code;
		*pcode = BC_WHILE;
		pcode++;
		memcpy(pcode, &conditionblockCommandCount, sizeof(conditionblockCommandCount));
		pcode += sizeof(conditionblockCommandCount);
		memcpy(pcode, &blockCommandCount, sizeof(blockCommandCount));
		pcode += sizeof(blockCommandCount);		
		memstreamwhile.AppendByteCode(code, codelength);
		free(code);
	}
	memstreamwhile.AppendByteCode(&streamcondition);
	memstreamwhile.AppendByteCode(&streamwhileblock);
	memstreamwhile.AppendByteCode((unsigned char *)&BC_END, 1);
	return false;
}

bool ScriptByteCode::GetByteCodeLoop(VTPARAMETERS & vtparameters, ByteCodeMemoryStream & memstream, CScriptEngine * engine)
{
	//LOOP指令的生成实际上会生成多条指令，如果BC_LOOP调用的函数此时没有被调用过，那么
	//要先生成函数的定义和函数体，后跟BC_LOOP 和 BC_CALL
	if (vtparameters.size() == 2)
	{
		std::string funcname = vtparameters.at(1);
		
		ScpFunctionObject* func = (ScpFunctionObject*)engine->GetCurrentObjectSpace()->FindObject(funcname);
		if (func)
		{
			//函数没有调用过，没有生成函数定义和函数体的字节码
			if (func->bytecodemem_funcbody.codelength == 0)
			{
				ByteCodeMemoryStream stream;
				func->functionexpressionblock->GenByteCode(engine, stream);
				GenByteCodeFunctionBody(stream);
				func->bytecodemem_funcbody.AppendByteCode(&stream);

				memstream.AppendByteCode(&func->bytecodemem_funcdef);
				memstream.AppendByteCode(&func->bytecodemem_funcbody);

			}
		}
		

		std::string strloopcount= vtparameters.at(0);
		unsigned int codelen = 0;
		
		codelen = SIZE_OF_BYTECODE_COMMAND + SIZE_OF_LOOPCOUNT;
		unsigned char * code = (unsigned char *)malloc(codelen);
		if (code)
		{
			unsigned char * pcode = code;
			*pcode = BC_LOOP;
			pcode++;
			
			ULONG loopcount = StringToInt(strloopcount);
			memcpy(pcode, (void*)&loopcount, sizeof(loopcount));
			pcode += sizeof(loopcount);
			
			VTPARAMETERS vtparameters_empty;
			ByteCodeMemoryStream  memstreamfunccall;
			GenByteCodeCallFunc(funcname, vtparameters_empty, memstreamfunccall);

			memstream.AppendByteCode(code, codelen);
			memstream.AppendByteCode(&memstreamfunccall);
			free(code);
			return true;
		}
	}
	return false;
}





void ScriptByteCode::Init()
{
	if (bytecodemem == NULL)
	{
		bytecodemem = new ByteCodeMemoryStream();
	}
	if (resourcepool == NULL)
	{
		resourcepool = new ScpResourcePool();
	}
	std::string temp("");
	resourcepool->AppendResource(temp);
	resourcepool->AppendResource(std::string("show"));
	resourcepool->AppendResource(std::string("int"));
	resourcepool->AppendResource(std::string("int64"));
	resourcepool->AppendResource(std::string("double"));
	resourcepool->AppendResource(std::string("string"));
	resourcepool->AppendResource(std::string("file"));
	
}

bool ScriptByteCode::DumpToFile(const char * bytecodefile)
{
	bool bret = false;
#ifdef _WIN32
    HANDLE hFile = CreateFileA(bytecodefile, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE != hFile)
    {
        DWORD dwWriteCount = 0;
		if (WriteFile(hFile, bytecodemem->membuf, bytecodemem->codelength, &dwWriteCount, NULL))
		{
			bret = true;
		}
        CloseHandle(hFile);
    }
#else
	int file = ::open(bytecodefile, O_WRONLY | O_CREAT,  00700);
	if (file > 0)
	{
		::lseek(file, 0, SEEK_SET);
		DWORD dwWriteCount = 0;
		dwWriteCount = ::write(file, bytecodemem->membuf, bytecodemem->codelength);
		if (dwWriteCount > 0)
		{
			bret = true;
		}
		::close(file);
	}
#endif // _WIN32
    return bret;
}

ByteCodeMemoryStream::ByteCodeMemoryStream() :membuf(NULL), bufsize(0), curpos(NULL), codelength(NULL)
{

}

ByteCodeMemoryStream::~ByteCodeMemoryStream()
{
	Release();
}

void ByteCodeMemoryStream::Release()
{
	if (membuf)
	{
		free(membuf);
		membuf = NULL;
	}
	bufsize = 0;
	curpos = NULL;
	codelength = 0;
}

int ByteCodeMemoryStream::GetByteCodeCommandCount()
{
	int count = 0;
	ScpByteCode bytecodereader;
	if (membuf)
	{
		unsigned char * pbytecode = (unsigned char *)membuf;
		int total = codelength;
		while (total > 0)
		{
			int length = bytecodereader.GetByteCodeLength(pbytecode);
			pbytecode += length;
			total -= length;
			count++;
		}	
	}
	return count;
}

unsigned char * ByteCodeMemoryStream::AppendByteCode(ByteCodeMemoryStream * other)
{
	return AppendByteCode(other->membuf, other->codelength);
}


unsigned char * ByteCodeMemoryStream::AppendByteCode(unsigned char * code, unsigned int  codelen)
{
	if (membuf == NULL)
	{
		bufsize = 1024 * 10;
		membuf = (unsigned char *)malloc(bufsize);
		if (membuf)
		{
			memset(membuf, 0, bufsize);
			curpos = membuf;
			memcpy(curpos, code, codelen);
			curpos += codelen;
			codelength += codelen;
			return curpos;
		}

	}
	else
	{
		size_t dwlen = curpos - membuf;
		if ((bufsize - dwlen) < codelen)
		{
			bufsize *= 2;
			membuf = (unsigned char *)realloc(membuf, bufsize);
		}
		memcpy(curpos, code, codelen);
		curpos += codelen;
		codelength += codelen;
		return curpos;
	}
	return NULL;
}
