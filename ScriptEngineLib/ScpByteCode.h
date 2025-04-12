#ifndef _H_SCPBYTECODE
#define _H_SCPBYTECODE
#include "ScpObject.h"
#ifdef WIN32
#include <windows.h>
#endif

// 字节码解析类 (Bytecode parsing class)

#define SIZE_OF_RESOURCEID (4)
#define SIZE_OF_OBJECT_TYPE_ID (1)
#define SIZE_OF_BYTECODE_COMMAND (1)
#define SIZE_OF_PARAMCOUNT (4)
#define SIZE_OF_LOOPCOUNT (4)
#define SIZE_OF_RESOURCELEN (4)
#define SIZE_OF_LANGID (4)
#define SIZE_OF_BOLCK (2) // 代码块长度，2字节 (Code block length, 2 bytes)

#ifndef MAXINT32
#define MAXINT32 ((int)(~0U>>1))
#endif

const static unsigned char BC_OBJECT_DEFINE = 0x01;
const static unsigned char BC_OBJECT_ADD = 0x02;
const static unsigned char BC_OBJECT_SELFADD = 0x03;
const static unsigned char BC_OBJECT_ASSIGN = 0x04;
const static unsigned char BC_OBJECT_INT_DEFINE = 0x05;
const static unsigned char BC_OBJECT_SUB = 0x06;
const static unsigned char BC_OBJECT_MUL = 0x07;
const static unsigned char BC_OBJECT_DIV = 0x08;
const static unsigned char BC_OBJECT_MOD = 0x09;
const static unsigned char BC_OBJECT_ADD_AND_ASSIGN = 0x0A;
const static unsigned char BC_OBJECT_SUB_AND_ASSIGN = 0x0B;
const static unsigned char BC_OBJECT_MUL_AND_ASSIGN = 0x0C;
const static unsigned char BC_OBJECT_DIV_AND_ASSIGN = 0x0D;
const static unsigned char BC_OBJECT_MOD_AND_ASSIGN = 0x0E;
const static unsigned char BC_OBJECT_EQUAL = 0x0F;
const static unsigned char BC_OBJECT_LESSTHAN = 0x10;
const static unsigned char BC_OBJECT_LESSOREQUAL = 0x11;
const static unsigned char BC_OBJECT_BIGTHAN = 0x12;
const static unsigned char BC_OBJECT_BIGOREQUAL = 0x13;
const static unsigned char BC_OBJECT_NOT = 0x14;
const static unsigned char BC_OBJECT_NOTEQUAL = 0x15;
const static unsigned char BC_OBJECT_BITAND = 0x16;
const static unsigned char BC_OBJECT_BITOR = 0x17;
const static unsigned char BC_OBJECT_BITNOT = 0x18;
const static unsigned char BC_OBJECT_BITXOR = 0x19;
const static unsigned char BC_OBJECT_LOGICALAND = 0x20;
const static unsigned char BC_OBJECT_LOGICALOR = 0x21;
const static unsigned char BC_OBJECT_BITSHIFTLEFT = 0x22;
const static unsigned char BC_OBJECT_BITSHIFTRIGHT = 0x23;
const static unsigned char BC_OBJECT_FUNCTION_DEFINE = 0x24;
const static unsigned char BC_OBJECT_CLASS_DEFINE = 0x25;//
const static unsigned char BC_OBJECT_SELFSUB = 0x26;//
const static unsigned char BC_OBJECT_BITAND_AND_ASSIGN = 0x27;//
const static unsigned char BC_OBJECT_BITOR_AND_ASSIGN = 0x28;//
const static unsigned char BC_OBJECT_BITNOT_AND_ASSIGN = 0x29;//
const static unsigned char BC_OBJECT_BITXOR_AND_ASSIGN = 0x30;//
const static unsigned char BC_OBJECT_BITSHIFTLEFT_AND_ASSIGN = 0x31;//
const static unsigned char BC_OBJECT_BITSHIFTRIGHT_AND_ASSIGN = 0x32;//

const static unsigned char BC_IMPORT = 0x55;//
const static unsigned char BC_DO = 0x56;//
const static unsigned char BC_RETURN = 0x57;
const static unsigned char BC_LOOP = 0x58;
const static unsigned char BC_PUBLIC = 0x59;//
const static unsigned char BC_PRIVATE = 0x60;//
const static unsigned char BC_CONTINUE = 0x61;
const static unsigned char BC_BREAK = 0x62;
const static unsigned char BC_LOAD = 0x63;
const static unsigned char BC_INHERITE = 0x64;//
const static unsigned char BC_REGISTER = 0x65;//
const static unsigned char BC_WHILE = 0x66;
const static unsigned char BC_CALL = 0x67;
const static unsigned char BC_EXIT = 0x68;//
const static unsigned char BC_IF = 0x69;
const static unsigned char BC_IF2 = 0x70;
const static unsigned char BC_OTHREWISE = 0x71;//
const static unsigned char BC_WAIT = 0x72;//
const static unsigned char BC_END = 0x73;
const static unsigned char BC_SWITCHLANG = 0x74;
const static unsigned char BC_NOP = 0x75;

const static unsigned char BC_INITRES = 0x76;
const static unsigned char BC_CALL_INNER = 0x77;
const static unsigned char BC_JUMP = 0x78;

const static unsigned char BC_DEBUG = 0x99;

// 整数0到10的资源ID (Resource IDs for integers 0 to 10)
const static int residzero = (MAXINT32 - 1);
const static int residone = (MAXINT32 - 2);
const static int residtwo = (MAXINT32 - 3);
const static int residthree = (MAXINT32 - 4);
const static int residfour = (MAXINT32 - 5);
const static int residfive = (MAXINT32 - 6);
const static int residsix = (MAXINT32 - 7);
const static int residseven = (MAXINT32 - 8);
const static int resideight = (MAXINT32 - 9);
const static int residnine = (MAXINT32 - 10);
const static int residten = (MAXINT32 - 11);

const static int residcurrenttime = (MAXINT32 - 300);

//define:int,num1,16
//define:int,num2
//0x00ab00cd address_of_static_string("num1") 
//0x0012001A address_of_static_int("16")
// BC_OBJECT_DEFINE ObjInt address_of_static_string("num1") address_of_static_int("16")
// 01 00 00 00 0F 00 AB 00 CD 00 12 00 1A
// BC_OBJECT_DEFINE ObjInt address_of_static_string("num2") address_of_static_int("0")
// 定义整数这行脚本的字节码长度为 13 
// 第一个字节为动作类型，由它决定了指令的长度为13，第2-5字节为定义的对象类型，因此对象类型数的最大值为整数上限，第6-9字节为对象名称在静态字符串池中的索引，第10-13字节为整数的值在静态整数池中的索引

//define:string,str1,"helloworld"
//define:string,str2
//0x00ae00cf address_of_static_string("helloworld") 
//0x00ab00ce address_of_static_string("helloworld") 
// BC_OBJECT_DEFINE ObjString address_of_static_string("str1") address_of_static_string("helloworld")
// 01 00 00 00 0A 00 AE 00 CF 00 AB 00 CE
// BC_OBJECT_DEFINE ObjString address_of_static_string("str1") address_of_static_string("")
// 定义字符串这行脚本的字节码长度为 13
// 对于没有初始值的字符串，仍然指定一个空值 


//num1+num2
//对象的加法需要产生一个临时变量保存加法计算的结果
//如果基于栈，则字节码可以缩减为只有一个字节
//如果是无栈模式，则字节码需要包含操作数地址和计算结果地址
//先考虑无栈模式

//0x00cd00cd address_of_tempobject_int("result")
// 0x00ac0011 address_of_object_int("num1")
// 0x00ac0032 address_of_object_int("num2")
//BC_OBJECT_ADD address_of_tempobject_int("result") address_of_object_int("num1") address_of_object_int("num2") 
// 02 00 CD 00 CD 00 AC 00 11 00 AC 00 32
//整数加法这行脚本的字节码长度为13

//x = y
//BC_OBJECT_ASSIGN address_of_object_int("x") address_of_object_int("y")
//04 02 00 00 00 04 00 00 00 


//num1-num2
//BC_OBJECT_SUB address_of_tempobject_int("result") address_of_object_int("num1") address_of_object_int("num2") 


//num1*num2
//BC_OBJECT_MUL  address_of_tempobject_int("result") address_of_object_int("num1") address_of_object_int("num2")

//num1/num2
//BC_OBJECT_DIV  address_of_tempobject_int("result") address_of_object_int("num1") address_of_object_int("num2")

//num1%num2
//BC_OBJECT_MOD  address_of_tempobject_int("result") address_of_object_int("num1") address_of_object_int("num2")

//num1+=num2
//BC_OBJECT_ADD_AND_ASSIGN address_of_object_int("num1") address_of_object_int("num2")

//num1==num2
//BC_OBJECT_EQUAL  address_of_tempobject_int("result") address_of_object_int("num1") address_of_object_int("num2")

//num1++
//BC_OBJECT_SELFADD address_of_object_int("num1")
// 0x00ac0011 address_of_object_int("num1")
// 02 00 AC 00 11
// 对象自增这行脚本的字节码长度为5

//import:"lib","1.scp"
//BC_IMPORT address_of_static_string("lib"),address_of_static_string("1.scp")

//do:"number.show()"
//do:"1.scp"
//BC_DO address_of_static_string("number.show()")
//BC_DO address_of_static_string("1.scp")

//return:number1
//BC_RETURN  address_of_static_string("number1")

//loop:100,function1
//BC_LOOP 00 00 00 64  
//BC_CALL address_of_static_string("function1")
//这里会把loop语句改写为两条语句
//loop语句后面的循环次数参数表示下一条语句执行多少次

//public:
//BC_PUBLIC 

//private:
//BC_PRIVATE

//continue
//BC_CONTINUE

//break
//BC_BREAK

//load:extobj,OS
//BC_LOAD address_of_static_string("extobj") address_of_static_string("OS")

//inherite:class,abc,def
//BC_INHERITE address_of_static_string("class") address_of_static_string("abc") address_of_static_string("def")

//register:function,xxx,400
//BC_REGISTER address_of_static_string("function") address_of_static_string("xxx") address_of_static_string("400")

//while(x>100)
//BC_WHILE address_of_expression_string("x>100") int_value_of_whileblocksize;

//function1(x,y)
//BC_CALL address_of_static_string("function1") address_of_static_string("x") address_of_static_string("y") 

//number.show()
//BC_CALL_INNER address_of_static_string("number") address_of_static_string("show") 

//exit
//BC_EXIT 

//if(x>0)
//BC_IF  address_of_expression_string("x>0")

//if:x>0,function1(x,y)
//BC_IF2  address_of_expression_string("x>0")  address_of_static_string("function1(x,y)")

//otherwise:function2(a,b)
//BC_OTHREWISE  address_of_static_string("function2(a,b)")

//wait:userinput,x
//BC_WAIT address_of_static_string("userinput") address_of_static_string("x")

//end
//BC_END 

//#scpeng 
//BC_SWITCHLANG 00 00 00 01

//#scp
//BC_NOP

//BC_INITRES int_value_of_resource_string_length utf8_encoded_resource_string

typedef struct _tag_ByteCodeHeader
{
	unsigned char Signature[16];
	unsigned char ScriptFileHash[16];
}ByteCodeHeader, *PByteCodeHeader;

#define ByteCodeHeaderSize sizeof(ByteCodeHeader)

#include "ScpResourcePool.h"
class CScriptEngine;
class ScpByteCode
{
public:
	ScpByteCode();
	~ScpByteCode();
	int Load(const char * ByteCodeFileName);
	int LoadFromMem(const unsigned char * ByteCode, unsigned int length);
	int Do();
	int DoByteCode(const unsigned char * ByteCode, unsigned int length, int *consumedlength = NULL);

	unsigned char * DoResInit(unsigned char * pByteCode, unsigned int& resid);
	unsigned char * DoSwitchLang(unsigned char * pByteCode);
	unsigned char * DoObjectDefine(unsigned char * pByteCode);
	unsigned char * DoCall(unsigned char * pByteCode);
	unsigned char * DoCallInner(unsigned char * pByteCode);
	unsigned char * DoLoad(unsigned char * pByteCode);
	unsigned char * DoObjectAdd(unsigned char * pByteCode);
	unsigned char * DoObjectSub(unsigned char * pByteCode);
	unsigned char * DoObjectMul(unsigned char * pByteCode);
	unsigned char * DoObjectDiv(unsigned char * pByteCode);
	unsigned char * DoObjectMod(unsigned char * pByteCode);
	unsigned char * DoObjectAssign(unsigned char * pByteCode);
	unsigned char * DoObjectAddAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectSubAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectMulAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectDivAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectModAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectBitAndAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectBitOrAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectBitNotAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectBitXOrAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectBitShiftLeftAndAssign(unsigned char * pByteCode);
	unsigned char * DoObjectBitShiftRightAndAssign(unsigned char * pByteCode);

	unsigned char * DoObjectEqual(unsigned char * pByteCode);
	unsigned char * DoObjectLessThan(unsigned char * pByteCode);
	unsigned char * DoObjectLessOrEqual(unsigned char * pByteCode);
	unsigned char * DoObjectBigThan(unsigned char * pByteCode);
	unsigned char * DoObjectBigOrEqual(unsigned char * pByteCode);
	unsigned char * DoObjectNot(unsigned char * pByteCode);
	unsigned char * DoObjectNotEqual(unsigned char * pByteCode);
	unsigned char * DoObjectBitAnd(unsigned char * pByteCode);
	unsigned char * DoObjectBitOr(unsigned char * pByteCode);
	unsigned char * DoObjectBitNot(unsigned char * pByteCode);
	unsigned char * DoObjectBitXor(unsigned char * pByteCode);
	unsigned char * DoObjectLogicalAnd(unsigned char * pByteCode);
	unsigned char * DoObjectLogicalOr(unsigned char * pByteCode);
	unsigned char * DoObjectBitShiftLeft(unsigned char * pByteCode);
	unsigned char * DoObjectBitShiftRight(unsigned char * pByteCode);
	unsigned char * DoObjectSelfAdd(unsigned char * pByteCode);
	unsigned char * DoObjectSelfSub(unsigned char * pByteCode);

	unsigned char * DoImport(unsigned char * pByteCode);

	// 实现循环语句 (Implement loop statement)
	unsigned char * DoLoop(unsigned char * pByteCode);

	// 实现函数定义 (Implement function define)
	unsigned char * DoObjectFunctionDefine(unsigned char * pByteCode);
	unsigned char * DoReturn(unsigned char * pByteCode);

	// 实现类定义 (Implement class define)
	unsigned char * DoObjectClassDefine(unsigned char * pByteCode);
	unsigned char * DoPublic(unsigned char * pByteCode);
	unsigned char * DoPrivate(unsigned char * pByteCode);
	unsigned char * DoInherite(unsigned char * pByteCode);

	// 实现while语句 (Implement while statement)
	unsigned char * DoWhileStatement(unsigned char * pByteCode);
	unsigned char * DoWhileBlock(unsigned char * pByteCode, int size);
	unsigned char * DoEnd(unsigned char * pByteCode);
	unsigned char * DoBreak(unsigned char * pByteCode);
	unsigned char * DoContinue(unsigned char * pByteCode);

	// 实现if语句 (Implement if statement)
	unsigned char * DoIfStatement(unsigned char * pByteCode);
	unsigned char * DoContitionBlock(unsigned char * pByteCode, int size);
	unsigned char * DoTrueBlock(unsigned char * pByteCode, int size);
	unsigned char * DoFalseBlock(unsigned char * pByteCode, int size);

	// 获取字节码实际长度，用于相对定位 (Get actual length of bytecode for relative positioning)
	int GetByteCodeLength(unsigned char * pByteCode);

	int GetBlockByteCodeLength(unsigned char * ppByteCode);

	unsigned char * ByteCodeBuffer;
	unsigned int ByteCodeLength;
	ScpResourcePool respool;
	ScpResourcePool respoolpre;

	CScriptEngine * engine;
};
#endif //_H_SCPBYTECODE
