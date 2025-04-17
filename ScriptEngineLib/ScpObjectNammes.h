#ifndef _H_SCPOBJECTNAMES
#define _H_SCPOBJECTNAMES
#include "ScpObject_uni.h"
#include "ScpErrorCode_uni.h"

class ScpObjectNames
{
public:
	static ScpObjectNames * GetSingleInsatnce();
	~ScpObjectNames();
	void SelectLanguage(int lang);
	int GetLanguage()
	{
		return languageid;
	}
	const char * strObjClass ;
	const char * strObjDll;
	const char * strObjExtObj;
	const char * strObjCurrentTime ;
	const char * strObjPicture ;	
	const char * strObjEnvironmentVariable ;	
	const char * strObjStruct ;
	
	const char * strObjWhileStatement ;
	const char * strObjType ;
	const char * strObjStaticInt ;
	const char * strObjStaticDouble ;
	const char * strObjStaticString ;
	const char * strObjIfStatement ;
	const char * strObjMainFunction ;

	const char * strScpMessageCaption ;
	const char * strHour ;
	const char * strMinute ;
	const char * strSecond ;
	const char * scpStringUpper ;
	const char * scpStringLower ;
	const char * scpStringReverse ;
	const char * scpStringHex;
	const char * strGlobal ;
	const char * strObjPid ;
	const char * strCommandParameter ;
	const char * strCurrentScriptFile ;
	const char * strCurrentScriptPath ;
	const char * strObjCurrentDirectory ;
	const char * strObjCallStack ;
	const char * strObjPackage ;
	const char * strObjFileLine ;
	const char * strScpDoubleQuoteLeftcn ;
	const char * strScpDoubleQuoteRightcn ;
	const char * strScpColoncn ;
	const char * strScpCommacn ;
	const char * strScpLeftParenthesescn ;
	const char * strScpRightParenthesescn ;
	const char * scpErrorLastErrorString ;
	const char * scpErrorInvalidScript ;
	const char * scpErrorInvalidParameter ;
	const char * scpErrorCommandSuccess ;
	const char * scpErrorCommandFailure ;
	const char * scpErrorNotCompletedFunctionDefine ;
	const char * scpErrorInvalidFunctionCall ;
	const char * scpErrorObjExist ;
	const char * scpErrorIsNotObjorStaticNumber ;
	const char * scpErrorStaticNumberCanNotBeAssign ;
	const char * scpErrorBreakpointHit ;
	const char * scpErrorLibNotExist ;
	const char * scpErrorCannotListening ;
	const char * scpErrorConnectionTimeOut ;
	const char * scpErrorConnectionClosed ;
	const char * scpErrorSendFail ;
	const char * scpErrorReceiveFail ;
	const char * scpErrorInvalidFunctionParameter;
	const char * scpErrorInvalidDividend;
	const char * scpErrorInvalidClassDefine ;
	const char * scpErrorCode ;
	const char * scpErrorGlobalComand ;
	const char * scpErrorObjNotExist ;
	const char * scpErrorInvalidClassOrFunctionDefine ;
	const char * scpErrorObjectInitializeFault ;
	const char * scpErrorObjectDoNotSupportCommand ;
	const char * scpErrorInvalidArrayElementType ;
	const char * scpErrorNestClassDefine ;
	const char * scpErrorClassDefineFault;
	const char * scpErrorInvalidStructDefine ;
	const char * scpErrorImportLibFailure ;
	const char * scpErrorInvalidExpression ;
	const char * scpErrorInvalidFunctionDefine;
	const char * scpErrorNotInt;
	const char * scpErrorNotNumber;
	const char * scpErrorIndexOutofSize;
	const char * scpErrorInvalidObjectDefine;
	const char * scpErrorInvalidLoadCommand;

	static bool IsValidObjectName(std::string& objName);
	private:
		int languageid;
	static ScpObjectNames singleton;
	ScpObjectNames();
};
#endif //_H_SCPOBJECTNAMES