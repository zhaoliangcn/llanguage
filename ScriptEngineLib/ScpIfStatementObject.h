/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
//条件语句对象 | Conditional statement object
*/
#ifndef _H_SCPIFSTATEMENTOBJECT
#define _H_SCPIFSTATEMENTOBJECT
#include "ScpObject.h"
#include "ScpObjectSpace.h"
#include "ScriptEngine.h"
#include "ScpExpressionBlock.h"
class CScriptEngine;
class ScpIfStatementObject : public ScpObject
{
public:
    friend class CScriptEngine;  // 声明CScriptEngine为友元类 | Declare CScriptEngine as a friend class
    ScpIfStatementObject(CScriptEngine *eg);  // 构造函数 | Constructor
    ~ScpIfStatementObject(void);  // 析构函数 | Destructor

    // 重写基类虚函数 | Override base class virtual functions
    virtual void Show(CScriptEngine * engine);  // 显示对象信息 | Display object information
    virtual ScpObject * Clone(std::string strObjName);  // 克隆对象 | Clone object
    virtual std::string ToString();  // 转换为字符串 | Convert to string
    virtual void Release();  // 释放对象 | Release object
    virtual bool IsInnerFunction(std::string & functionname);  // 判断是否为内部函数 | Check if it's an inner function
    virtual ScpObject * CallInnerFunction(std::string & functionname, VTPARAMETERS * parameters, CScriptEngine * engine);  // 调用内部函数 | Call inner function

    int Do(CScriptEngine *engine);  // 执行IF语句 | Execute IF statement
    void ClearLocalObjects();  // 清除本地对象 | Clear local objects
    int ReComputeCondition();  // 重新计算条件 | Recompute condition

    // IF语句的条件表达式 | Condition expression of IF statement
    std::string ConditionExpression;  // 条件表达式字符串 | Condition expression string
    int ConditionResult;  // 条件表达式结果 | Condition expression result
    bool TrueBody;  // 是否为真分支 | Whether it's the true branch
    std::string Name;  // 对象名称 | Object name
    ScpObjectSpace IfStatementObjectSpace;  // IF语句的对象空间 | Object space for IF statement
    CScriptEngine *engine;  // 脚本引擎指针 | Pointer to script engine
    ScpExpressionBlock *trueblock;  // 真分支表达式块 | True branch expression block
    ScpExpressionBlock *falseblock;  // 假分支表达式块 | False branch expression block

    bool MakeConditionByteCode();  // 生成条件字节码 | Generate condition bytecode

    // 字节码内存流 | Bytecode memory streams
    ByteCodeMemoryStream condition_bytecodemem;  // 条件表达式字节码 | Condition expression bytecode
    ByteCodeMemoryStream trueblock_bytecodemem;  // 真分支字节码 | True branch bytecode
    ByteCodeMemoryStream falseblock_bytecodemem;  // 假分支字节码 | False branch bytecode

    ByteCodeMemoryStream bytecodemem_ifstmt;  // IF语句字节码 | IF statement bytecode
};

#endif //_H_SCPIFSTATEMENTOBJECT