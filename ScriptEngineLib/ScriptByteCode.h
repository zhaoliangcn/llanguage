#ifndef _H_SCRIPTBYTECODE
#define _H_SCRIPTBYTECODE
#include "ScpAst.h"
#include <string>
#include "ScpObject.h"
#include "ScpResourcePool.h"
#include "ScpByteCode.h"

// 字节码生成类 (Bytecode generation class)
class CScriptEngine;

class ByteCodeMemoryStream
{
public:
	ByteCodeMemoryStream();
	~ByteCodeMemoryStream();
	void Release();
	int GetByteCodeCommandCount();
	unsigned char * AppendByteCode(ByteCodeMemoryStream * other);
	unsigned char * AppendByteCode(unsigned char * code,unsigned int codelen);
	unsigned char * membuf;
	unsigned int bufsize;
	unsigned char * curpos;
	unsigned int codelength;

};
class ScriptByteCode
{
public:
	friend class CScriptEngine;
	ScriptByteCode();
	~ScriptByteCode();

	// 从源代码行生成字节码 (Generate bytecode from source line)
	bool GenByteCodeFromSouceLine(std::string& wsSourceLine, ByteCodeMemoryStream& memstream);
	// 从命令生成字节码 (Generate bytecode from command)
    bool GenByteCodeFromCommand(const unsigned int & commandvalue, VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream, CScriptEngine * engine);
	// 生成对象定义的字节码 (Generate bytecode for object definition)
	bool GenByteCodeObjectDefine(ScpObjectType type,VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream);
	// 生成类对象定义的字节码 (Generate bytecode for class object definition)
	bool GenByteCodeClassObjectDefine(ScpObjectType type, VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream);
	// 生成类实例定义的字节码 (Generate bytecode for class instance definition)
	bool GenByteCodeClassInstanceDefine(ScpObjectType type, VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream);
	// 生成函数定义的字节码 (Generate bytecode for function definition)
	bool GenByteCodeFunctionDefine(VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream, CScriptEngine * engine);
	// 生成加载指令的字节码 (Generate bytecode for load command)
	bool GenByteCodeLoad(VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream);
	// 生成导入指令的字节码 (Generate bytecode for import command)
	bool GenByteCodeImport(VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream);
	// 获取初始化资源的字节码 (Get bytecode for initializing resource)
	bool GetByteCodeInitRes(std::string res, ByteCodeMemoryStream& memstream,ULONG &resid);
	// 获取二元操作的字节码 (Get bytecode for binary operation)
	bool GetByteCodeBinaryOp(std::string op, ULONG idret, ULONG idleft, ULONG idright,ByteCodeMemoryStream& memstream);
	// 生成一元操作的字节码 (Generate bytecode for unary operation)
	bool GenByteCodeUnaryOp(std::string op,ULONG idobj, ByteCodeMemoryStream& memstream);
	// 生成调用内部函数的字节码 (Generate bytecode for calling inner function)
	bool GenByteCodeCallInner(std::string objectname, std::string innerfuncname,VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream, CScriptEngine * engine);
	// 生成if语句的字节码 (Generate bytecode for if statement)
	bool GenByteCodeIfstatement(ByteCodeMemoryStream & streamcondition, ByteCodeMemoryStream & streamtrueblock, ByteCodeMemoryStream& streamfalseblock,ByteCodeMemoryStream & memstreamif);
	// 生成函数体的字节码 (Generate bytecode for function body)
	bool GenByteCodeFunctionBody(ByteCodeMemoryStream & memstreamfunc);
	// 获取计算的字节码 (Get bytecode for compute)
	bool GetByteCodeCompute(VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream, CScriptEngine * engine);
	// 生成调用函数的字节码 (Generate bytecode for calling function)
	bool GenByteCodeCallFunc(std::string funcname, VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream);
	// 生成返回指令的字节码 (Generate bytecode for return command)
	bool GenByteCodeReturn(VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream, CScriptEngine * engine);
	// 生成while语句的字节码 (Generate bytecode for while statement)
	bool GenByteCodeWhilestatement(ByteCodeMemoryStream & streamcondition, ByteCodeMemoryStream & streamwhileblock, ByteCodeMemoryStream & memstreamwhile);
	// 获取循环的字节码 (Get bytecode for loop)
	bool GetByteCodeLoop(VTPARAMETERS & vtparameters, ByteCodeMemoryStream& memstream, CScriptEngine * engine);
	// 初始化 (Initialize)
	void Init();
	// 转储到文件 (Dump to file)
	bool DumpToFile(const char * bytecodefile);

	ByteCodeMemoryStream * bytecodemem;
	ScpResourcePool * resourcepool;
	
};

#endif //_H_SCRIPTBYTECODE