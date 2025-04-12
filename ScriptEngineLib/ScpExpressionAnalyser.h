/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
*/
#ifndef _H_SCPEXPRESSIONANALYSER
#define _H_SCPEXPRESSIONANALYSER

#include "ScpObject.h"
#include "ScpObjectSpace.h"
#include "ScpScriptLex.h"
#include "ScriptByteCode.h"
#include <stack>
class ScpExpressionTreeNode;
typedef std::stack<std::string> ScpObjStack;
typedef std::stack<ScpExpressionTreeNode*> TheNodeStack;
typedef std::stack<ScpObject*> TheObjectStack;

/*
表达式语法树节点类
*/
class ScpExpressionTreeNode:public ScpObject
{
public:
	ScpExpressionTreeNode();
	~ScpExpressionTreeNode();

	virtual void Show(CScriptEngine * engine)
	{
		
	}
	virtual ScpObject * Clone(std::string strObjName)
	{
		return NULL;
	}
	virtual std::string ToString()
	{
		return "";
	}
	virtual void Release()
	{
		delete this;
	}
	virtual bool IsInnerFunction(std::string & functionname)
	{
		if (nodeobject && InnerFunctionName.empty())
		{
			//如果这个节点是一个数组、列表、表成员引用节点，那需要先引用其成员，再对其成员调用IsInnerFunction
			if (!ArrayItem.empty())
			{
				return true;
			}
			else
			return nodeobject->IsInnerFunction(functionname);
		}
		else
		{
			return true;
		}
		return false;
	}
	virtual ScpObject * CallInnerFunction(std::string & functionname, VTPARAMETERS * parameters, CScriptEngine * engine)
	{
		if (nodeobject)
		{
			return nodeobject->CallInnerFunction(functionname, parameters,engine);
		}
		else
		{
			return ScpObject::CallInnerFunction(functionname, parameters, engine);
		}
		return NULL;
	}
	void Destroy(ScpObjectSpace * currentObjectSpace);


	ScpExpressionTreeNode * LeftChild;
	ScpExpressionTreeNode * RightChild;
	
	ScpObject *  CalculateEx(CScriptEngine * engine);
	ScpObject *  CalcStackEx(CScriptEngine * engine);
	ScpObject * MakeByteCode(CScriptEngine * engine, ByteCodeMemoryStream &memstream);
	ScpObject * MakeByteCodeStack(CScriptEngine * engine, ByteCodeMemoryStream &memstream);
	ScpObject * MakeByteCodePerformFunctionCall(CScriptEngine * engine, ByteCodeMemoryStream &memstream);
	ULONG GenTempObjectByteCode(ScpObject* obj, std::string &name, CScriptEngine* engine,bool retvalue);

	ScpObject * nodeobject;  //当前节点中保存的对象
	std::string Expression;
	ScpObjStack nodestackobj;
	ScpObjStack nodestackop;
	TheObjectStack NodeObjectStack;
	ScpObject * PerformFunctionCall(CScriptEngine * engine);
	ScpObject * DoArrayItemRef(CScriptEngine * engine);
	VTPARAMETERS vtFuncparameters;   //函数调用参数
	std::string InnerFunctionName;  //本节点表示对象成员函数的调用
	std::string ArrayItem;
	int GenTempObjectByteCodeOfExpression(std::string Expression, CScriptEngine* engine);
};



/*
表达式解析器类
*/
class ScpExpressionAnalyser
{
public:
	ScpExpressionAnalyser();
	ScpExpressionAnalyser(CScriptEngine * eg);
	~ScpExpressionAnalyser();
	void Attach(CScriptEngine * eng);
	


	ScpExpressionTreeNode *  BuildExpressionTreeEx(std::string Expression);   //构建表达式的语法树
	ScpExpressionTreeNode *  PostFixExpressionToTree(std::string PostFixExpression);   //通过后缀表达式构建语法树
	void EmptyStack(std::string &PostFixExpression,ScpObjStack &OperationStack);
	void PopStack(std::string &PostFixExpression,ScpObjStack &OperationStack);
	/*
	由中缀表达式转换为后缀表达式的同时生成表达式树
	*/
	ScpExpressionTreeNode * InFixToPostFixEx(std::string &InFixExpression,std::string &PostFixExpression,ScpObjectSpace * objectSpace, size_t &startpos);
	

	std::string GetATokenFromPostFix(std::string& PostFixExpression, size_t &startpos); //从后缀表达式中获得一个token
	ScpExpressionTreeNode * BuildExressionTreeNode(std::string Expression, size_t &startpos);
	ScpExpressionTreeNode * ParseFunctionCall(ScpObject * obj,std::string token,ScpObjectSpace * objectSpace);
	ScpExpressionTreeNode * ParseArrayItemRefrence(ScpObject * arrayObj,ScpObjectSpace * objectSpace);
	std::string ParseObjectRefrence(ScpObject * obj, ScpObjectSpace * objectSpace);
	bool ParseClassDefine(const char * ScriptFile,VTSTRINGS & ScriptBody,int & currentcommandline);
	bool ParseStructDefine(const char * ScriptFile, VTSTRINGS & ScriptBody,int & currentcommandline);
	bool ParseFunctionDefine(const char * ScriptFile, VTSTRINGS & ScriptBody,int & currentcommandline);
	ScpObject * ParseWhileDefine(const char * ScriptFile, VTSTRINGS & ScriptBody,int & currentcommandline,bool doimmediately);
	ScpObject * ParseIfDefine(const char * ScriptFile, VTSTRINGS & ScriptBody,int & currentcommandline, bool doimmediately);

	void ClearNodeStack();
	TheNodeStack thenodestack;
	ScpScriptLex  lex; 
	CScriptEngine * engine;
	
};
#endif //_H_SCPEXPRESSIONANALYSER