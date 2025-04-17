/*
//author :zhaoliang
//email:zhaoliangcn@126.com
//code descriptyon:
//对象名称管理，用于实现中英文关键字的动态切换
*/
#include "ScpObjectNammes.h"
ScpObjectNames ScpObjectNames::singleton;
ScpObjectNames * ScpObjectNames::GetSingleInsatnce()
{
	return &singleton;
}

ScpObjectNames::ScpObjectNames(void)
{
	languageid = 0;
	SelectLanguage(0);
}

ScpObjectNames::~ScpObjectNames(void)
{
}
bool ScpObjectNames::IsValidObjectName(std::string& objName)
{
	bool bret=false;
	//简单的只判断了起始字符是否是数字
	bret=!isdigit(objName.at(0));
	return bret;
}
void ScpObjectNames::SelectLanguage(int lang)
{
	languageid = lang;
	if(lang==0)
	{

		strObjClass =str_CN_ObjClass;
		strObjDll = str_CN_ObjDll;
		strObjExtObj = str_CN_ObjExtObj;
		strObjCurrentTime = str_CN_ObjCurrentTime;
		strObjPicture = str_CN_ObjPicture;
		strObjEnvironmentVariable = str_CN_ObjEnvironmentVariable;
		strObjStruct = str_CN_ObjStruct;
		strObjWhileStatement = str_CN_ObjWhileStatement;
		strObjType = str_CN_ObjType;
		strObjStaticInt = str_CN_ObjStaticInt;
		strObjStaticDouble = str_CN_ObjStaticDouble;
		strObjStaticString = str_CN_ObjStaticString;
		strObjIfStatement = str_CN_ObjIfStatement;
		strObjMainFunction = str_CN_ObjMainFunction;
		strScpMessageCaption = str_CN_ScpMessageCaption;
		strHour = str_CN_Hour;
		strMinute = str_CN_Minute;
		strSecond = str_CN_Second;
		scpStringUpper = str_CN_scpStringUpper;
		scpStringLower = str_CN_scpStringLower;
		scpStringReverse = str_CN_scpStringReverse;
		scpStringHex = str_CN_scpStringHex;
		strGlobal = str_CN_Global;
		strObjPid = str_CN_ObjPid;
		strCommandParameter = str_CN_CommandParameter;
		strCurrentScriptFile = str_CN_CurrentScriptFile;
		strCurrentScriptPath = str_CN_CurrentScriptPath;
		strObjCurrentDirectory = str_CN_ObjCurrentDirectory;
		strObjCallStack = str_CN_ObjCallStack;
		strObjPackage = str_CN_ObjPackage;
		strObjFileLine = str_CN_ObjFileLine;
		strScpDoubleQuoteLeftcn = str_CN_ScpDoubleQuoteLeftcn;
		strScpDoubleQuoteRightcn = str_CN_ScpDoubleQuoteRightcn;
		strScpColoncn = str_CN_ScpColoncn;
		strScpCommacn = str_CN_ScpCommacn;
		strScpLeftParenthesescn = str_CN_ScpLeftParenthesescn;
		strScpRightParenthesescn = str_CN_ScpRightParenthesescn;
		scpErrorLastErrorString = scpErrorCnLastErrorString;
		scpErrorInvalidScript = scpErrorCnInvalidScript;
		scpErrorInvalidParameter = scpErrorCnInvalidParameter;
		scpErrorCommandSuccess = scpErrorCnCommandSuccess ;
		scpErrorCommandFailure = scpErrorCnCommandFailure;
		scpErrorNotCompletedFunctionDefine = scpErrorCnNotCompletedFunctionDefine;
		scpErrorInvalidFunctionCall = scpErrorCnInvalidFunctionCall;
		scpErrorObjExist = scpErrorCnObjExist;
		scpErrorIsNotObjorStaticNumber = scpErrorCnIsNotObjorStaticNumber;
		scpErrorStaticNumberCanNotBeAssign = scpErrorCnStaticNumberCanNotBeAssign;
		scpErrorBreakpointHit = scpErrorCnBreakpointHit;
		scpErrorLibNotExist = scpErrorCnLibNotExist ;
		scpErrorCannotListening = scpErrorCnCannotListening;
		scpErrorConnectionTimeOut = scpErrorCnConnectionTimeOut;
		scpErrorConnectionClosed = scpErrorCnConnectionClosed;
		scpErrorSendFail = scpErrorCnSendFail;
		scpErrorReceiveFail = scpErrorCnReceiveFail;
		scpErrorInvalidFunctionParameter = scpErrorCnInvalidFunctionParameter;
		scpErrorInvalidDividend = scpErrorCnInvalidDividend;
		scpErrorInvalidClassDefine = scpErrorCnInvalidClassDefine;
		scpErrorCode = scpErrorCnCode;
		scpErrorGlobalComand = scpErrorCnGlobalComand;
		scpErrorObjNotExist = scpErrorCnObjNotExist;
		scpErrorInvalidClassOrFunctionDefine = scpErrorCnInvalidClassOrFunctionDefine ;
		scpErrorObjectInitializeFault = scpErrorCnObjectInitializeFault;
		scpErrorObjectDoNotSupportCommand = scpErrorCnObjectDoNotSupportCommand;
		scpErrorInvalidArrayElementType = scpErrorCnInvalidArrayElementType ;
		scpErrorNestClassDefine = scpErrorCnNestClassDefine;
		scpErrorClassDefineFault = scpErrorCnClassDefineFault;
		scpErrorInvalidStructDefine = scpErrorCnInvalidStructDefine ;
		scpErrorImportLibFailure = scpErrorCnImportLibFailure;
		scpErrorInvalidExpression = scpErrorCnInvalidExpression;
		scpErrorInvalidFunctionDefine = scpErrorCnInvalidFunctionDefine;
		scpErrorNotInt = scpErrorCnNotInt;
		scpErrorNotNumber = scpErrorCnNotNumber;
		scpErrorIndexOutofSize = scpErrorCnIndexOutofSize;
		scpErrorInvalidObjectDefine = scpErrorCnInvalidObjectDefine;
		scpErrorInvalidLoadCommand = scpErrorCnInvalidLoadCommand;
	}
	else if(lang==1)
	{


		strObjClass =str_EN_ObjClass;		
		strObjDll = str_EN_ObjDll;
		strObjExtObj = str_EN_ObjExtObj;
		strObjCurrentTime = str_EN_ObjCurrentTime;
		strObjPicture = str_EN_ObjPicture;
		strObjEnvironmentVariable = str_EN_ObjEnvironmentVariable;
		strObjStruct = str_EN_ObjStruct;
		strObjWhileStatement = str_EN_ObjWhileStatement;
		strObjType = str_EN_ObjType;
		strObjStaticInt = str_EN_ObjStaticInt;
		strObjStaticDouble = str_EN_ObjStaticDouble;
		strObjStaticString = str_EN_ObjStaticString;
		strObjIfStatement = str_EN_ObjIfStatement;
		strObjMainFunction = str_EN_ObjMainFunction;
		strScpMessageCaption = str_EN_ScpMessageCaption;
		strHour = str_EN_Hour;
		strMinute = str_EN_Minute;
		strSecond = str_EN_Second;
		scpStringUpper = str_EN_scpStringUpper;
		scpStringLower = str_EN_scpStringLower;
		scpStringReverse = str_EN_scpStringReverse;
		scpStringHex = str_EN_scpStringHex;
		strGlobal = str_EN_Global;
		strObjPid = str_EN_ObjPid;
		strCommandParameter = str_EN_CommandParameter;
		strCurrentScriptFile = str_EN_CurrentScriptFile;
		strCurrentScriptPath = str_EN_CurrentScriptPath;
		strObjCurrentDirectory = str_EN_ObjCurrentDirectory;
		strObjCallStack = str_EN_ObjCallStack;
		strObjPackage = str_EN_ObjPackage;
		strObjFileLine = str_EN_ObjFileLine;
		strScpDoubleQuoteLeftcn = str_EN_ScpDoubleQuoteLeftcn;
		strScpDoubleQuoteRightcn = str_EN_ScpDoubleQuoteRightcn;
		strScpColoncn = str_EN_ScpColoncn;
		strScpCommacn = str_EN_ScpCommacn;
		strScpLeftParenthesescn = str_EN_ScpLeftParenthesescn;
		strScpRightParenthesescn = str_EN_ScpRightParenthesescn;
		scpErrorLastErrorString = scpErrorEnLastErrorString;
		scpErrorInvalidScript = scpErrorEnInvalidScript;
		scpErrorInvalidParameter = scpErrorEnInvalidParameter;
		scpErrorCommandSuccess = scpErrorEnCommandSuccess ;
		scpErrorCommandFailure = scpErrorEnCommandFailure;
		scpErrorNotCompletedFunctionDefine = scpErrorEnNotCompletedFunctionDefine;
		scpErrorInvalidFunctionCall = scpErrorEnInvalidFunctionCall;
		scpErrorObjExist = scpErrorEnObjExist;
		scpErrorIsNotObjorStaticNumber = scpErrorEnIsNotObjorStaticNumber;
		scpErrorStaticNumberCanNotBeAssign = scpErrorEnStaticNumberCanNotBeAssign;
		scpErrorBreakpointHit = scpErrorEnBreakpointHit;
		scpErrorLibNotExist = scpErrorEnLibNotExist ;
		scpErrorCannotListening = scpErrorEnCannotListening;
		scpErrorConnectionTimeOut = scpErrorEnConnectionTimeOut;
		scpErrorConnectionClosed = scpErrorEnConnectionClosed;
		scpErrorSendFail = scpErrorEnSendFail;
		scpErrorReceiveFail = scpErrorEnReceiveFail;
		scpErrorInvalidFunctionParameter = scpErrorEnInvalidFunctionParameter;
		scpErrorInvalidDividend = scpErrorEnInvalidDividend;
		scpErrorInvalidClassDefine = scpErrorEnInvalidClassDefine;
		scpErrorCode = scpErrorEnCode;
		scpErrorGlobalComand = scpErrorEnGlobalComand;
		scpErrorObjNotExist = scpErrorEnObjNotExist;
		scpErrorInvalidClassOrFunctionDefine = scpErrorEnInvalidClassOrFunctionDefine ;
		scpErrorObjectInitializeFault = scpErrorEnObjectInitializeFault;
		scpErrorObjectDoNotSupportCommand = scpErrorEnObjectDoNotSupportCommand;
		scpErrorInvalidArrayElementType = scpErrorEnInvalidArrayElementType ;
		scpErrorNestClassDefine = scpErrorEnNestClassDefine;
		scpErrorClassDefineFault = scpErrorEnClassDefineFault;
		scpErrorInvalidStructDefine = scpErrorEnInvalidStructDefine ;
		scpErrorImportLibFailure = scpErrorEnImportLibFailure;
		scpErrorInvalidExpression = scpErrorEnInvalidExpression;
		scpErrorInvalidFunctionDefine = scpErrorEnInvalidFunctionDefine;
		scpErrorNotInt = scpErrorEnNotInt;
		scpErrorNotNumber = scpErrorEnNotNumber;
		scpErrorIndexOutofSize = scpErrorEnIndexOutofSize;
		scpErrorInvalidObjectDefine = scpErrorEnInvalidObjectDefine;
		scpErrorInvalidLoadCommand = scpErrorEnInvalidLoadCommand;
	}
}