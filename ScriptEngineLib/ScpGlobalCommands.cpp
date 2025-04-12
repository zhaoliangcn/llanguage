#include "ScpGlobalCommands.h"
#include "ScpObjectSpace.h"
#include "ScriptEngine.h"
#include "ScpFileObject.h"
#include "ScpDirectoryObject.h"
#include "ScpJsonObject.h"
#include "ScpRegExpObject.h"
#include "ScpRandomNumberObject.h"
#include "ScpDoubleObject.h"
#include "ScpStructObject.h"
#include "ScpFunctionObject.h"
#include "ScpRangeObject.h"
#include "ScpListObject.h"
#include "../Common/commonutil.hpp"
#include "ScpObjectNammes.h"
namespace GlobalCommands
{
    
	/*
	“睡眠”命令
	参数：毫秒数
	*/
	BOOL __stdcall Do_Sleep_Command(VTPARAMETERS * vtparameters, CScriptEngine * engine)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		if (vtparameters->size() == 1)
		{
			std::string strinterval = vtparameters->at(0);
			ULONG interval = StringToInt(strinterval.c_str());
#ifdef _WIN32
			Sleep(interval);
#else
			usleep(interval);
#endif
		}
		return TRUE;
	}

//
//“清空”命令
//操作对象：文件、字符串、内存
//
BOOL __stdcall Do_Clear_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string strobjtype = vtparameters->at(0);
        std::string strobjname = vtparameters->at(1);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobjtype.c_str());
        if(ObjFile==type)
        {
            ScpFileObject * fileobj=(ScpFileObject * )currentObjectSpace->FindObject(strobjname);
            if(fileobj)
            {
                ScpFileObject::Clear(fileobj->filename.c_str());
            }
            else
            {
                ScpFileObject::Clear(strobjname.c_str());
            }
        }
        else if (ObjString == type)
        {
            ScpStringObject *strobj1;
            std::string objspace;
            strobj1=(ScpStringObject *)currentObjectSpace->FindObject(strobjname);
            if(strobj1)
            {
                if(strobj1->GetType()==ObjString)
                {
                    strobj1->Clear();
                }
            }
        }
        else if (ObjMemory == type)
        {
            ScpMemoryObject *obj1=(ScpMemoryObject*)currentObjectSpace->FindObject(strobjname);
            if(obj1)
            {
                ScpObjectType type1 =obj1->GetType();
                if(ObjMemory==type1)
                {
                    obj1->Clear();
                }
            }
        }
        else if(ObjTable==type)
        {
            ScpTableObject * obj1=(ScpTableObject*)currentObjectSpace->FindObject(strobjname);
            if(obj1)
            {
                ScpObjectType type1 =obj1->GetType();
                if(ObjTable==type1)
                {
                    obj1->EmptyElement();
                }
            }
        }
        else
        {
            engine->SetLastErrorString(ScpObjectNames::GetSingleInsatnce()->scpErrorObjectDoNotSupportCommand);
        }
    }
    else if(vtparameters->size()==1)
    {
        std::string strobjname = vtparameters->at(0);
        ScpObject * obj=(ScpObject * )currentObjectSpace->FindObject(strobjname);
        if(obj)
        {
            ScpObjectType type = obj->GetType();
            if(type ==ObjFile)
            {
                ((ScpFileObject*)obj)->Clear();
            }
            else if(type == ObjString)
            {
                ((ScpStringObject *)obj)->Clear();
            }
            else if(type == ObjMemory)
            {
                ((ScpMemoryObject *)obj)->Clear();
            }
            else if(type==ObjTable)
            {
                ((ScpTableObject *)obj)->EmptyElement();
            }
            else
            {
                engine->SetLastErrorString(ScpObjectNames::GetSingleInsatnce()->scpErrorObjectDoNotSupportCommand);
            }
        }
    }
    else
    {
        engine->SetLastErrorString(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidParameter);
    }
    return TRUE;
}
/*
“关闭”命令
操作对象：计算机、进程、线程、网络连接
*/
BOOL __stdcall Do_Shutdown_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==1)
    {
        std::string text = vtparameters->at(0);
        {
            ScpObject *obj=(ScpObject *)currentObjectSpace->FindObject(text);
            if(obj)
            {
                if(obj->GetType()==ObjDirectory)
                {
                    currentObjectSpace->EraseObject(text);
                }
            }
            else
            {
                engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidParameter);
            }
        }
    }
    else
    {
        engine->SetLastErrorString(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidParameter);
    }
    return TRUE;
}
/*
“弹出”命令
操作对象：消息框
*/
BOOL __stdcall Do_Popup_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    int paramcount = vtparameters->size();
    if(paramcount == 2 || paramcount == 3)
    {
        std::string strobj = vtparameters->at(0);
        std::string text = vtparameters->at(1);
        std::string caption;
        if(vtparameters->size()==3)
            caption= vtparameters->at(2);
        ScpObjectType type =ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        if(ObjMsgBox==type)
        {
            ScpStringObject *strobjtxt=(ScpStringObject *)currentObjectSpace->FindObject(text);
            if(strobjtxt)
            {
                text=strobjtxt->content.c_str();
            }
            ScpStringObject *strobjcap=(ScpStringObject *)currentObjectSpace->FindObject(caption);
            if(strobjcap)
            {
                caption=strobjcap->content.c_str();
            }
            else
            {
                caption=ScpObjectNames::GetSingleInsatnce()->strScpMessageCaption;
            }
#ifdef _WIN32
            typedef int (WINAPI *Func_MessageBoxA)(
                 HWND hWnd,
                 LPCSTR lpText,
                 LPCSTR lpCaption,
                 UINT uType);
            static Func_MessageBoxA WinMessageBox = (Func_MessageBoxA)GetProcAddress(GetModuleHandleW(L"user32.dll"), "MessageBoxA");
            if (WinMessageBox)
            {
                WinMessageBox(NULL, STDSTRINGEXT::UToA(text).c_str(), STDSTRINGEXT::UToA(caption).c_str(), MB_OK | MB_ICONINFORMATION);
            }

#else
            std::string message;
            message=STDSTRINGEXT::Format("\"zenity --info --title'%s' --text'%s'\"",caption.c_str(),text.c_str());
            system(message.c_str());
#endif
        }
        else
        {
            engine->SetLastErrorString(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidParameter);
        }
    }
    else
    {
        engine->SetLastErrorString(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidParameter);
    }
    return TRUE;
}
/*
“运行”命令
操作对象：进程、线程
*/
BOOL __stdcall Do_Run_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();

    return TRUE;
}
/*
“显示”命令
操作对象：当前时间、静态字符串、任意对象
*/
BOOL __stdcall Do_Show_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==1)
    {
        std::string text = vtparameters->at(0);
        if(text==ScpObjectNames::GetSingleInsatnce()->scpErrorLastErrorString)
        {
            engine->PrintError(engine->GetLastErrorString());
        }
        else
        {
            ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(text.c_str());
            if(ObjCurrentTime==type)
            {
                ScpTimeObject::ShowNow(engine);
            }
            else if(ScpObjectNames::GetSingleInsatnce()->strObjCallStack==text)
            {
                ScpObjectSpace * pcurrentObjectSpace = currentObjectSpace;
                std::string Tab;
                while(pcurrentObjectSpace)
                {
                    if(pcurrentObjectSpace->ObjectSpaceType==Space_Function)
                    {
                        engine->PrintError(Tab+pcurrentObjectSpace->spacename);
                        pcurrentObjectSpace=pcurrentObjectSpace->parentspace;
                        Tab+="\t";
                    }
                    else if(pcurrentObjectSpace->ObjectSpaceType==Space_Global)
                    {
                        break;
                    }
                }
            }
            else
            {
                ScpObject *obj=currentObjectSpace->FindObject(text);
                if(obj)
                {
                    obj->Show(engine);
                }
                else
                {
                    ScpObject * tempobj = NULL;
                    std::string Expression = text;
                    ScpExpressionAnalyser ana;
                    ana.Attach(engine);
                    ScpExpressionTreeNode *root = ana.BuildExpressionTreeEx(Expression);
                    if (root)
                    {
                        tempobj = root->CalculateEx(engine);
                        if (tempobj)
                        {
                            tempobj->Show(engine);
                            //delete tempobj;
                        }
                        else
                        {
                            engine->PrintError(text);
                        }
                        root->Destroy(engine->GetCurrentObjectSpace());
                        engine->GetCurrentObjectSpace()->ReleaseTempObject(root);
                    }
                    else
                    {
                        engine->PrintError(Expression);
                    }

                }
            }
        }
    }
    if(vtparameters->size()==2)
    {
        std::string strobj = vtparameters->at(0);
        std::string pathname = vtparameters->at(1);
        ScpObject *obj=currentObjectSpace->FindObject(strobj);
        if(obj)
        {
            obj->Show(engine);
        }
        else
        {
            engine->PrintError(strobj);
        }
        ScpObject *obj1=currentObjectSpace->FindObject(pathname);
        if(obj1)
        {
            obj1->Show(engine);
        }
        else
        {
            engine->PrintError(pathname);
        }
    }
    if(vtparameters->size()>2)
    {
        for(unsigned int i=0; i<vtparameters->size(); i++)
        {
            std::string temp = vtparameters->at(i);
            ScpObject *obj=currentObjectSpace->FindObject(temp);
            if(obj)
            {
                obj->Show(engine);
            }
            else
            {
                engine->PrintError(temp);
            }
        }
    }
    return TRUE;
}
/*
“删除”命令
操作对象：文件、注册表、目录
*/
BOOL __stdcall Do_Delete_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string strobj = vtparameters->at(0);
        std::string pathname = vtparameters->at(1);
        StringStripQuote(pathname);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        if(ObjFile==type)
        {

            ScpObject* obj=currentObjectSpace->FindObject(pathname);
            if(obj && obj->GetType()==ObjFile)
            {
                ScpFileObject::Delete(((ScpFileObject*)obj)->filename);
            }
            else if(obj && obj->GetType()==ObjString)
            {
                ScpFileObject::Delete(((ScpStringObject*)obj)->content);
            }
            else
            {
                ScpFileObject::Delete(pathname);
            }
        }
        else if(ObjDirectory == type)
        {
            ScpObject* dirobj=currentObjectSpace->FindObject(pathname);
            if(dirobj && dirobj->GetType()==ObjDirectory)
            {
                ((ScpDirectoryObject*)dirobj)->Delete();
            }
            else if(dirobj && dirobj->GetType()==ObjString)
            {
                ScpDirectoryObject::Delete(((ScpStringObject*)dirobj)->content);
            }
            else
            {
                ScpDirectoryObject::Delete(pathname);
            }
        }
    }
    else if(vtparameters->size()==1)
    {
        std::string pathname = vtparameters->at(0);
        ScpObject * obj = currentObjectSpace->FindObject(pathname);
        if(obj)
        {
            ScpObjectType type = obj->GetType();
            if(ObjFile==type)
            {
                ScpFileObject::Delete(pathname.c_str());
            }
            else if(ObjDirectory == type)
            {
                ((ScpDirectoryObject*)obj)->Delete();
            }
            else if(ObjString ==type)
            {
                std::string fileorpath= ((ScpStringObject*)obj)->content;
                if (ScpDirectoryObject::IsDir(fileorpath))
                {
                    ScpDirectoryObject::Delete(fileorpath);
                }
                else
                {
                    ScpFileObject::Delete(((ScpStringObject*)obj)->content);
                }
            }
        }
    }
    else
    {
        engine->SetLastErrorString(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidParameter);
    }
    return TRUE;
}
/*
“打开”命令
操作对象：文件、目录、管道、注册表项
*/
BOOL __stdcall Do_Open_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string strobj = vtparameters->at(0);
        std::string userobjname = vtparameters->at(1);
        //ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        {
            ScpObject * obj = currentObjectSpace->FindObject(strobj);
            ScpObject * userobj = currentObjectSpace->FindObject(userobjname);
            if(userobj)
            {
                if(userobj->GetType()==ObjString)
                {
                    userobjname=((ScpStringObject*)userobj)->content;
                }
            }
            StringStripQuote(userobjname);
            if(obj && obj->GetType()==ObjJson)
            {
                ((ScpJsonObject *)obj)->Open(userobjname);
            }
            else if(obj && obj->GetType()==ObjDirectory)
            {
                ((ScpDirectoryObject * )obj)->Open(userobjname);
                ((ScpDirectoryObject * )obj)->EnumAll();;
            }
            else if(obj && obj->GetType()==ObjFile)
            {
                ((ScpFileObject * )obj)->Open(userobjname,"");
            }
        }
    }
    if(vtparameters->size()==3)
    {
        std::string strobj = vtparameters->at(0);
        std::string userobjname = vtparameters->at(1);
        std::string pathname = vtparameters->at(2);
        ScpObject * userobj = currentObjectSpace->FindObject(pathname);
        if(userobj)
        {
            if(userobj->GetType()==ObjString)
            {
                pathname=((ScpStringObject*)userobj)->content;
            }
        }
        StringStripQuote(pathname);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        if(ObjFile==type)
        {
            ScpFileObject* fileobj =new ScpFileObject;
            ScpStringObject * obj = (ScpStringObject *)currentObjectSpace->FindObject(pathname);
            if(obj)
            {
                type=obj->GetType();
                if(ObjString==type)
                {
                    fileobj->filename=obj->content;
                }
            }
            else
            {
                fileobj->filename=pathname;
            }
            fileobj->Open(pathname, str_EN_file_mode_readwrite);
            currentObjectSpace->AddObject(userobjname,fileobj);
        }
        else if(ObjDirectory==type)
        {
            ScpDirectoryObject *dirobj = new ScpDirectoryObject;
            ScpStringObject * obj = (ScpStringObject *)currentObjectSpace->FindObject(pathname);
            if(obj)
            {
                type=obj->GetType();
                if(ObjString==type)
                {
                    dirobj->Open(obj->content);
                }
            }
            else
            {
                dirobj->Open(pathname);
            }
            dirobj->EnumAll();
            if(currentObjectSpace->FindLocalObject(userobjname))
            {
                currentObjectSpace->EraseObject(userobjname);
            }
            currentObjectSpace->AddObject(userobjname,dirobj);


        }
    }
    else
    {
        engine->SetLastErrorString(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidParameter);
    }
    return TRUE;
}

/*
“定位”命令
操作对象：文件
*/
BOOL __stdcall Do_Seek_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==3)
    {
        std::string strobj = vtparameters->at(0);
        std::string userobjname = vtparameters->at(1);
        std::string pos = vtparameters->at(2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        if(ObjFile==type)
        {
            ScpObject* fileobj =currentObjectSpace->FindObject(userobjname);
            if(fileobj)
            {
                if(fileobj->GetType()==ObjFile)
                {
                    ScpObject* posobj =currentObjectSpace->FindObject(pos);
                    if(posobj && posobj->GetType()==ObjInt)
                    {
                        ((ScpFileObject*)fileobj)->seek(((ScpIntObject*)posobj)->value);
                    }
                    else
                    {
                        if(IsStaticNumber(pos))
                            ((ScpFileObject*)fileobj)->seek(StringToInt64(pos.c_str()));
                        else
                            ((ScpFileObject*)fileobj)->seek(pos);
                    }
                }
            }
        }
    }
    else if(vtparameters->size()==2)
    {
        std::string userobjname = vtparameters->at(0);
        std::string pos = vtparameters->at(1);
        ScpObject* fileobj =currentObjectSpace->FindObject(userobjname);
        if(fileobj)
        {
            if(fileobj->GetType()==ObjFile)
            {
                ScpObject* posobj =currentObjectSpace->FindObject(pos);
                if(posobj && posobj->GetType()==ObjInt)
                {
                    ((ScpFileObject*)fileobj)->seek(((ScpIntObject*)posobj)->value);
                }
                else
                {
                    if(IsStaticNumber(pos))
                        ((ScpFileObject*)fileobj)->seek(StringToInt64(pos.c_str()));
                    else
                        ((ScpFileObject*)fileobj)->seek(pos);
                }

            }
        }
    }
    return TRUE;
}
/*
“复制”命令
操作对象：文件、字符串、内存、目录
*/
BOOL __stdcall Do_Copy_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string name1 = vtparameters->at(0);
        std::string name2 = vtparameters->at(1);
        StringStripQuote(name1);
        StringStripQuote(name2);
        ScpObject *obj1=currentObjectSpace->FindObject(name1);
        ScpObject *obj2=currentObjectSpace->FindObject(name2);
        if(obj1 && obj2)
        {
            ScpObjectType type1 =obj1->GetType();
            ScpObjectType type2 =obj2->GetType();
            if(ObjFile==type1&&ObjFile==type2)
            {
                ScpFileObject::Copy(((ScpFileObject*)obj1)->filename,((ScpFileObject*)obj2)->filename,FALSE);
            }
            else if(ObjString==type1&&ObjString==type2)
            {
                ScpStringObject *strobj1=(ScpStringObject*)obj1;
                ScpStringObject *strobj2=(ScpStringObject*)obj2;
                if(strobj2!=NULL)
                {
                    strobj2->content=strobj1->content;
                }
            }
            else if(ObjMemory==type1&&ObjMemory==type2)
            {
                ((ScpMemoryObject*)obj1)->CopyTo((ScpMemoryObject*)obj2);

            }
            else if(ObjDirectory==type1&&ObjDirectory==type2)
            {
                ScpDirectoryObject::Copy(((ScpDirectoryObject *)obj1)->directory,((ScpDirectoryObject *)obj2)->directory);
            }
            else if (ObjDirectory == type1 && ObjString == type2)
            {
                ScpDirectoryObject::Copy(((ScpDirectoryObject *)obj1)->directory, ((ScpStringObject *)obj2)->content);

            }
            else
            {
                engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorInvalidParameter);
            }
        }
        else
        {
            if(ScpFileObject::FileExist(name1))
            {
                ScpFileObject::Copy(name1,name2,FALSE);
            }
        }
    }
    else if(vtparameters->size()==3)
    {
        std::string strobj = vtparameters->at(0);
        std::string name1 = vtparameters->at(1);
        std::string name2 = vtparameters->at(2);
        StringStripQuote(name1);
        StringStripQuote(name2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        if(ObjFile==type)
        {
            ScpStringObject *strobj1=(ScpStringObject*)currentObjectSpace->FindObject(name1);
            if(strobj1!=NULL)
            {
                name1 = strobj1->content;
            }
            ScpStringObject *strobj2=(ScpStringObject*)currentObjectSpace->FindObject(name2);
            if(strobj2!=NULL)
            {
                name2=strobj2->content;
            }
            ScpFileObject::Copy(name1,name2,FALSE);
        }
        else if (ObjString == type)
        {
            ScpStringObject *strobj1=(ScpStringObject*)currentObjectSpace->FindObject(name1);
            if(strobj1!=NULL)
            {
                ScpStringObject *strobj2=(ScpStringObject*)currentObjectSpace->FindObject(name2);
                if(strobj2!=NULL)
                {
                    strobj2->content=strobj1->content;
                }
                else
                {
                    ScpStringObject *strobj2=new ScpStringObject;
                    strobj2->content =strobj1->content;
                    currentObjectSpace->AddObject(name2,strobj2);
                }
            }
        }
        else if (ObjMemory == type)
        {
            ScpObject *obj1=currentObjectSpace->FindObject(name1);
            ScpObject *obj2=currentObjectSpace->FindObject(name2);
            ScpObjectType type1 =obj1->GetType();
            ScpObjectType type2 =obj2->GetType();
            if(ObjMemory==type1&&ObjMemory==type2)
            {
                if(obj1!=NULL&&obj2!=NULL)
                {
                    ((ScpMemoryObject*)obj1)->CopyTo((ScpMemoryObject*)obj2);
                }
            }
        }
        else if(ObjDirectory == type)
        {
            ScpStringObject *strobj1=(ScpStringObject*)currentObjectSpace->FindObject(name1);
            if(strobj1!=NULL)
            {
                name1=strobj1->content;
            }
            ScpStringObject *strobj2=(ScpStringObject*)currentObjectSpace->FindObject(name2);
            if(strobj2!=NULL)
            {
                name2=strobj2->content;
            }
            ScpDirectoryObject::Copy(name1,name2);
        }
    }
    return TRUE;
}

/*
“连接”命令
操作对象：字符串，网络连接
*/
BOOL __stdcall Do_Connect_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    /*
    "连接"指令
    连接两个字符串
    或通过"网络连接"对象，连接到某个IP地址
    */
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==3)
    {
        std::string strobjtype = vtparameters->at(0);
        std::string str1 = vtparameters->at(1);
        std::string str2 = vtparameters->at(2);
        StringStripQuote(str2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobjtype.c_str());
        if(ObjString==type)
        {
            ScpObject *strobj1=currentObjectSpace->FindObject(str1);
            if(strobj1 && strobj1->GetType()==ObjString)
            {
                ScpObject *strobj2=currentObjectSpace->FindObject(str2);
                if(strobj2)
                {
                    if(strobj2->GetType()==ObjString)
                    {
                        ((ScpStringObject *)strobj1)->content +=((ScpStringObject *)strobj2)->content;
                    }
                    else
                    {
                        ((ScpStringObject *)strobj1)->content +=strobj2->ToString();
                    }
                }
                else
                {
                    ((ScpStringObject *)strobj1)->content += str2;
                }
            }


        }
    }
    else if (vtparameters->size()==2)
    {
        std::string strobj1 = vtparameters->at(0);
        std::string strobj2 = vtparameters->at(1);
        ScpObject *obj1=currentObjectSpace->FindObject(strobj1);
        if(obj1)
        {
            if(obj1->GetType()==ObjString)
            {
                ScpObject *obj2=currentObjectSpace->FindObject(strobj2);
                if(obj2)
                {
                    if(obj2->GetType()==ObjString)
                    {
                        ((ScpStringObject *)obj1)->content +=((ScpStringObject *)obj2)->content;
                    }
                    else
                    {
                        ((ScpStringObject *)obj1)->content +=obj2->ToString();
                    }
                }
                else
                {
                    ((ScpStringObject *)obj1)->content += strobj2;
                }
            }
        }
    }

    return TRUE;
}
/*
“捕获”命令
*/
BOOL __stdcall Do_Capture_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    return TRUE;
}
/*
“比较”命令
操作对象：字符串、内存、文件
*/
BOOL __stdcall Do_Compare_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==3)
    {
        std::string strobjtype = vtparameters->at(0);
        std::string str1 = vtparameters->at(1);
        std::string str2 = vtparameters->at(2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobjtype.c_str());
        ScpIntObject * compare_ret = new ScpIntObject;
        compare_ret->value = 0;
        if(ObjString==type)
        {
            ScpStringObject *strobj1=(ScpStringObject *)currentObjectSpace->FindObject(str1);
            ScpStringObject *strobj2=(ScpStringObject *)currentObjectSpace->FindObject(str2);
            if(strobj1 && strobj2)
            {
                if(strobj1->content==strobj2->content)
                {
                    compare_ret->value = 1;
                }
                else
                {
                    compare_ret->value = 0;
                }
            }
        }
        else if (ObjMemory==type)
        {
            ScpMemoryObject *obj1=(ScpMemoryObject *)currentObjectSpace->FindObject(str1);
            ScpMemoryObject *obj2=(ScpMemoryObject *)currentObjectSpace->FindObject(str2);
            if(obj1 && obj2)
            {
                if(obj1->Size==obj2->Size)
                {
                    if(memcmp(obj1->Address,obj2->Address,obj1->Size)==0)
                    {
                        compare_ret->value = 1;
                    }
                }
            }
        }
        else if(ObjFile == type)
        {
            ScpFileObject *obj1=(ScpFileObject *)currentObjectSpace->FindObject(str1);
            ScpFileObject *obj2=(ScpFileObject *)currentObjectSpace->FindObject(str2);
            if(obj1 && obj2)
            {
                if(ScpFileObject::Compare(obj1->filename,obj2->filename)==0)
                {
                    compare_ret->value = 1;
                }
            }
            else
            {
                if(ScpFileObject::FileExist(str1) && ScpFileObject::FileExist(str2))
                {
                    if(ScpFileObject::Compare(str1,str2)==0)
                    {
                        compare_ret->value = 1;
                    }
                }
                else
                {
                    engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjNotExist);
                }
            }
        }
    }
    return TRUE;
}

/*
“申请”命令
操作对象：内存
*/
BOOL  __stdcall Do_Acquire_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==3)
    {
        int size = 0;
        std::string memobj = vtparameters->at(0);
        std::string memname = vtparameters->at(1);
        std::string memsize = vtparameters->at(2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(memobj.c_str());
        if(ObjMemory==type)
        {
            ScpObject * obj=currentObjectSpace->FindObject(memname);
            if(obj)
            {

            }
            else
            {
                ScpIntObject * intobj=(ScpIntObject *)currentObjectSpace->FindObject(memsize);
                if(intobj && intobj->GetType()==ObjInt)
                {
                    size = intobj->value;
                }
                else
                {
                    if(IsStaticNumber(memsize))
                    {
                        size = StringToInt(memsize.c_str());
                    }
                }
                ScpMemoryObject * mem=new ScpMemoryObject;
                mem->Acquare(size);

                currentObjectSpace->AddObject(memname,mem);
            }
        }
    }
    return TRUE;
}
/*
“释放”命令
操作对象：内存
*/
BOOL  __stdcall Do_Release_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string memobj = vtparameters->at(0);
        std::string memname = vtparameters->at(1);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(memobj.c_str());
        if(ObjMemory==type)
        {
            ScpMemoryObject * mem=(ScpMemoryObject*)currentObjectSpace->FindObject(memname);
            if(mem)
            {
                if(ObjMemory==mem->GetType())
                {
                    mem->ReleaseMem();
                }
            }
            else
            {
                engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjNotExist);
            }
        }
    }
    else if(vtparameters->size()==1)
    {
        std::string memname = vtparameters->at(0);
        ScpMemoryObject * mem=(ScpMemoryObject*)currentObjectSpace->FindObject(memname);
        if(mem)
        {
            if(ObjMemory==mem->GetType())
            {
                mem->ReleaseMem();
            }
        }
        else
        {
            engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjNotExist);
        }
    }
    return  TRUE;
}
/*
“写入”命令
操作对象：内存、文件、注册表、管道
*/
BOOL __stdcall Do_Write_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==3)
    {
        std::string memobj = vtparameters->at(0);
        std::string name = vtparameters->at(1);
        std::string objname = vtparameters->at(2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(memobj.c_str());
        if(ObjMemory==type)
        {
            ScpMemoryObject * mem=(ScpMemoryObject*)currentObjectSpace->FindObject(name);
            if(mem)
            {
                ScpObject *obj1=currentObjectSpace->FindObject(objname);
                if(obj1)
                {
                    if(obj1->GetType()==ObjString)
                        mem->Write(((ScpStringObject *)obj1)->content);
                    else if(obj1->GetType()==ObjInt)
                    {
                        mem->Write(&((ScpIntObject*)obj1)->value,sizeof(int));
                    }
                }
            }
        }
        else if(ObjFile==type)
        {

            ScpFileObject *file=(ScpFileObject*)currentObjectSpace->FindObject(name);
            if(file)
            {
                if(file->GetType()==ObjFile)
                {
                    ScpObject *obj1=currentObjectSpace->FindObject(objname);
                    if(obj1)
                    {
                        if(obj1->GetType()==ObjString)
                            file->write(((ScpStringObject *)obj1)->content);
                        else if(obj1->GetType()==ObjMemory)
                            file->write(file->GetCurrentPos(),((ScpMemoryObject*)obj1)->Size,((ScpMemoryObject*)obj1)->Address);
                        else if(obj1->GetType()==ObjInt)
                            file->write(file->GetCurrentPos(),sizeof(int),&((ScpIntObject*)obj1)->value);
                    }
                }
            }
        }

    }
    else if(vtparameters->size()==2)
    {
        std::string name = vtparameters->at(0);
        std::string objname = vtparameters->at(1);
        ScpObject * obj = currentObjectSpace->FindObject(name);
        if(obj)
        {
            ScpObjectType type = obj->GetType();
            if(ObjMemory==type)
            {
                ScpMemoryObject * mem=(ScpMemoryObject*)currentObjectSpace->FindObject(name);
                if(mem)
                {
                    ScpObject *obj1=currentObjectSpace->FindObject(objname);
                    if(obj1)
                    {
                        if(obj1->GetType()==ObjString)
                            mem->Write(((ScpStringObject *)obj1)->content);
                        else if(obj1->GetType()==ObjInt)
                        {
                            mem->Write(&((ScpIntObject*)obj1)->value,sizeof(int));
                        }
                    }
                }
            }
            else if(ObjFile==type)
            {

                ScpFileObject *file=(ScpFileObject*)currentObjectSpace->FindObject(name);
                if(file)
                {

                    ScpObject *obj1=currentObjectSpace->FindObject(objname);
                    if(obj1)
                    {
                        if(obj1->GetType()==ObjString)
                            file->write(((ScpStringObject *)obj1)->content);
                        else if(obj1->GetType()==ObjMemory)
                            file->write(file->GetCurrentPos(),((ScpMemoryObject*)obj1)->Size,((ScpMemoryObject*)obj1)->Address);
                        else if(obj1->GetType()==ObjInt)
                            file->write(file->GetCurrentPos(),sizeof(int),&((ScpIntObject*)obj1)->value);
                    }
                    else
                    {
                        file->write(objname);
                    }

                }
            }

        }
    }

    return TRUE;
}
/*
“读取”命令
操作对象：当前时间、内存、文件、管道
*/
BOOL __stdcall Do_Read_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string strtimeobj = vtparameters->at(0);
        std::string timename = vtparameters->at(1);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strtimeobj.c_str());
        if(ObjCurrentTime==type)
        {
            ScpTimeObject * timeobj=(ScpTimeObject *)currentObjectSpace->FindObject(timename);
            if(timeobj)
            {
                if(timeobj->GetType()==ObjTime)
                    timeobj->value=ScpTimeObject::GetNow();
            }
        }
        else
        {
            std::string name = vtparameters->at(0);
            std::string objname = vtparameters->at(1);
            ScpObject * obj = currentObjectSpace->FindObject(name);
            if(obj)
            {
                ScpObjectType type = obj->GetType();
                if(ObjMemory==type)
                {
                    ScpMemoryObject * mem=(ScpMemoryObject*)currentObjectSpace->FindObject(name);
                    if(mem)
                    {
                        ScpStringObject *strobj1=(ScpStringObject *)currentObjectSpace->FindObject(objname);
                        if(strobj1)
                        {
                            if(strobj1->GetType()==ObjString)
                                mem->Read(strobj1->content);
                        }
                    }
                }
                else if(ObjFile==type)
                {
                    ScpFileObject *file=(ScpFileObject*)currentObjectSpace->FindObject(name);
                    if(file)
                    {
                        ScpObject *obj1=currentObjectSpace->FindObject(objname);
                        if(obj1)
                        {
                            if(obj1->GetType()==ObjString)
                                file->read(((ScpStringObject *)obj1)->content);
                            else if(obj1->GetType()==ObjMemory)
                                file->read(file->GetCurrentPos(),((ScpMemoryObject*)obj1)->Size,&((ScpMemoryObject*)obj1)->Address);
                            else if(obj1->GetType()==ObjInt)
                            {
                                void * pint=(void *)&((ScpIntObject*)obj1)->value;
                                ULONG size= sizeof(int);
                                file->read(file->GetCurrentPos(),size,&pint);
                            }
                        }
                    }
                }
            }
        }
    }
    else if(vtparameters->size()==3)
    {
        std::string memobj = vtparameters->at(0);
        std::string memname = vtparameters->at(1);
        std::string objname = vtparameters->at(2);
        if (memobj == ScpObjectNames::GetSingleInsatnce()->strObjFileLine)
        {
            ScpFileObject *file = (ScpFileObject*)currentObjectSpace->FindObject(memname);
            if (file)
            {
                ScpObject *obj1 = currentObjectSpace->FindObject(objname);
                if (obj1)
                {
                    if (obj1->GetType() == ObjString)
                        file->readline(((ScpStringObject *)obj1)->content);
                }
            }
        }
        else
        {
            ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(memobj.c_str());
            if (ObjMemory == type)
            {
                ScpMemoryObject * mem = (ScpMemoryObject*)currentObjectSpace->FindObject(memname);
                if (mem)
                {
                    if (mem->GetType() == ObjMemory)
                    {
                        ScpStringObject *strobj1 = (ScpStringObject *)currentObjectSpace->FindObject(objname);
                        if (strobj1)
                        {
                            if (strobj1->GetType() == ObjString)
                                mem->Read(strobj1->content);
                        }
                    }
                }
            }
            else if (ObjFile == type)
            {
                ScpFileObject *file = (ScpFileObject*)currentObjectSpace->FindObject(memname);
                if (file)
                {
                    ScpObject *obj1 = currentObjectSpace->FindObject(objname);
                    if (obj1)
                    {
                        if (obj1->GetType() == ObjString)
                            file->read(((ScpStringObject *)obj1)->content);
                        else if (obj1->GetType() == ObjMemory)
                            file->read(file->GetCurrentPos(), ((ScpMemoryObject*)obj1)->Size, &((ScpMemoryObject*)obj1)->Address);
                        else if (obj1->GetType() == ObjInt)
                        {
                            void * pint = (void *)&((ScpIntObject*)obj1)->value;
                            ULONG size = sizeof(int);
                            file->read(file->GetCurrentPos(), size, &pint);
                        }
                    }
                }
            }

        }
    }
    return TRUE;
}
/*
“开始”命令
操作对象：函数、类
*/
BOOL __stdcall Do_Start_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    return TRUE;
}
/*
“取子串”命令
操作对象：字符串
*/
BOOL __stdcall Do_Getsubstring_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==4)
    {
        std::string substrname= vtparameters->at(0);
        std::string strobjname = vtparameters->at(1);
        std::string strstart= vtparameters->at(2);
        std::string strlength = vtparameters->at(3);
        ScpStringObject *substrobj = (ScpStringObject *)currentObjectSpace->FindObject(substrname);
        if(!substrobj)
        {
            ScpStringObject *obj = (ScpStringObject *)currentObjectSpace->FindObject(strobjname);
            if(obj)
            {
                if(obj->GetType()==ObjString)
                {
                    ScpIntObject *objs = (ScpIntObject *)currentObjectSpace->FindObject(strstart);
                    ScpIntObject *objl = (ScpIntObject *)currentObjectSpace->FindObject(strlength);
                    if(objs && objl)
                    {
                        ScpStringObject *subobj=obj->SubStr(objs->value,objl->value, engine);
                        currentObjectSpace->AddObject(substrname,subobj);
                    }
                    else
                    {
                        ScpStringObject *subobj=obj->SubStr(StringToInt(strstart.c_str()),StringToInt(strlength.c_str()), engine);
                        currentObjectSpace->AddObject(substrname,subobj);
                    }
                }
            }
        }
        else
        {
            ScpStringObject *obj = (ScpStringObject *)currentObjectSpace->FindObject(strobjname);
            if(obj)
            {
                if(obj->GetType()==ObjString)
                {
                    ScpIntObject *objs = (ScpIntObject *)currentObjectSpace->FindObject(strstart);
                    ScpIntObject *objl = (ScpIntObject *)currentObjectSpace->FindObject(strlength);
                    if(objs && objl)
                    {
                        obj->SubStr(obj,substrobj,objs->value,objl->value);
                    }
                    else
                    {
                        obj->SubStr(obj,substrobj,StringToInt(strstart.c_str()),StringToInt(strlength.c_str()));
                    }
                }
            }
        }
    }
    return TRUE;

}
/*
“插入”命令
操作对象：表对象、字符串
*/
BOOL __stdcall Do_Insert_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string name= vtparameters->at(0);
        std::string elementname = vtparameters->at(1);
        ScpObject * obj = (ScpObject * )currentObjectSpace->FindObject(name);
        if(obj)
        {
            if(obj->GetType()==ObjTable)
            {
                ((ScpTableObject * )obj)->AddElement(elementname,currentObjectSpace);
            }
        }
    }
    else if(vtparameters->size()==3)
    {
        std::string objname= vtparameters->at(0);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(objname.c_str());
        if(ObjTable==type)
        {
            std::string name = vtparameters->at(1);
            std::string elementname = vtparameters->at(2);
            ScpTableObject * tableobj = (ScpTableObject * )currentObjectSpace->FindObject(name);
            if(tableobj)
            {
                if(tableobj->GetType()==ObjTable)
                {
                    tableobj->AddElement(elementname,currentObjectSpace);
                }
            }
        }
        else
        {
            std::string name = vtparameters->at(0);
            std::string substrname = vtparameters->at(1);
            std::string posname = vtparameters->at(2);
            ScpStringObject * strobj = (ScpStringObject * )currentObjectSpace->FindObject(name);
            if(strobj)
            {
                if(strobj->GetType()==ObjString)
                {
                    ScpStringObject * substrobj = (ScpStringObject * )currentObjectSpace->FindObject(substrname);
                    ScpIntObject * intobj=  (ScpIntObject * )currentObjectSpace->FindObject(posname);
                    if(intobj)
                    {
                        if(substrobj )
                        {
                            strobj->content.insert(intobj->value,substrobj->content);
                        }
                        else
                        {
                            strobj->content.insert(intobj->value,substrname);
                        }
                    }
                }

            }
        }
    }
    else if (vtparameters->size()==4)
    {
        std::string objname= vtparameters->at(0);
        std::string name = vtparameters->at(1);
        std::string substrname = vtparameters->at(2);
        std::string posname = vtparameters->at(3);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(objname.c_str());
        if(ObjString==type)
        {
            ScpStringObject * strobj = (ScpStringObject * )currentObjectSpace->FindObject(name);
            if(strobj)
            {
                if(strobj->GetType()==ObjString)
                {
                    ScpStringObject * substrobj = (ScpStringObject * )currentObjectSpace->FindObject(substrname);
                    ScpIntObject * intobj=  (ScpIntObject * )currentObjectSpace->FindObject(posname);
                    if(intobj)
                    {
                        if(substrobj)
                        {
                            strobj->content.insert(intobj->value,substrobj->content);
                        }
                        else
                        {
                            strobj->content.insert(intobj->value,substrname);
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}
/*
“匹配”命令
操作对象：正则表达式对象
*/
BOOL __stdcall Do_Match_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()>=3)
    {
        std::string objname= vtparameters->at(0);
        std::string name = vtparameters->at(1);
        std::string text = vtparameters->at(2);
        std::string posname ;
        ScpIntObject* intobj = NULL;
        if(vtparameters->size()==4)
        {
            posname =  vtparameters->at(3);
            intobj = (ScpIntObject *)currentObjectSpace->FindObject(posname);
        }

        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(objname.c_str());
        if(ObjRegExp==type)
        {

            ScpRegExpObject *regexpobj =(ScpRegExpObject *)currentObjectSpace->FindObject(name);
            if(regexpobj)
            {
                if(regexpobj->GetType()==ObjRegExp)
                {

                    if(!intobj)
                    {
                        intobj= new ScpIntObject;
                    }
                    ScpStringObject *obj = (ScpStringObject *)currentObjectSpace->FindObject(text);
                    if(obj)
                    {
                        if(obj->GetType()==ObjString)
                            text=obj->content;
                    }

                    if(regexpobj->Find(text))
                    {
                        intobj->value = 1;
                    }
                    else
                    {
                        intobj->value = 0;
                    }
                    //delete matchret;
                }
            }
        }
    }
    return TRUE;
}
/*
“查找”命令
操作对象：字符串 目录
*/
BOOL __stdcall Do_Find_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==3)
    {
        std::string objname = vtparameters->at(0);
        std::string strobjname = vtparameters->at(1);
        std::string substrname= vtparameters->at(2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(objname.c_str());
        if(ObjString==type)
        {
            ScpStringObject *obj = (ScpStringObject *)currentObjectSpace->FindObject(strobjname);
            if(obj)
            {
                if(obj->GetType()==ObjString)
                {
                    ScpStringObject *subobj=(ScpStringObject *)currentObjectSpace->FindObject(substrname);
                    ScpIntObject * findret = new ScpIntObject;

                    if(obj->content.find(subobj->content)!=std::string::npos)
                    {
                        findret->value = 1;
                    }
                    else
                    {
                        findret->value  = 0;
                    }
                }
            }
        }
    }
    else if(vtparameters->size()==4)
    {
        std::string objname = vtparameters->at(0);
        std::string strobjname = vtparameters->at(1);
        std::string substrname= vtparameters->at(2);
        std::string posname = vtparameters->at(3);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(objname.c_str());
        if(ObjString==type)
        {
            ScpStringObject *obj = (ScpStringObject *)currentObjectSpace->FindObject(strobjname);
            if(obj)
            {
                if(obj->GetType()==ObjString)
                {
                    ScpStringObject *subobj=(ScpStringObject *)currentObjectSpace->FindObject(substrname);
                    if(subobj)
                    {
                        ScpIntObject *intobj=(ScpIntObject *)currentObjectSpace->FindObject(posname);
                        size_t pos =obj->content.find(subobj->content);
                        if(pos!=std::string::npos)
                        {
                            intobj->value=pos;
                        }
                        else
                        {
                            intobj->value=-1;
                        }

                    }
                }
            }
        }
        else if(ObjFile==type)
        {
            ScpDirectoryObject * obj =(ScpDirectoryObject*)currentObjectSpace->FindObject(strobjname);
            if(obj)
            {
                if(obj->GetType()==ObjDirectory)
                {
                    ScpTableObject * tableobj = (ScpTableObject *)currentObjectSpace->FindObject(substrname);
                    if(tableobj)
                    {
                        if(tableobj->GetType()==ObjTable)
                        {
                            obj->FindAllFiles(currentObjectSpace,tableobj,obj->directory,posname,true);
                        }
                    }
                }
            }
            else
            {
                ScpTableObject * tableobj = (ScpTableObject *)currentObjectSpace->FindObject(substrname);
                if(tableobj)
                {
                    if(tableobj->GetType()==ObjTable)
                    {
                        obj->FindAllFiles(currentObjectSpace,tableobj,strobjname,posname,true);
                    }
                }
            }
        }
    }

    return TRUE;
}
/*
“替换”命令
操作对象：字符串
*/
BOOL __stdcall Do_Replace_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==4)
    {
        std::string objname = vtparameters->at(0);
        std::string strobjname = vtparameters->at(1);
        std::string substrname= vtparameters->at(2);
        std::string substrname2= vtparameters->at(3);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(objname.c_str());
        if(ObjString==type)
        {

            ScpStringObject *obj = (ScpStringObject *)currentObjectSpace->FindObject(strobjname);
            if(obj)
            {
                if(obj->GetType()==ObjString)
                {
                    ScpStringObject *subobj=(ScpStringObject *)currentObjectSpace->FindObject(substrname);
                    ScpStringObject *subobj2 = (ScpStringObject *)currentObjectSpace->FindObject(substrname2);
                    if(subobj2)
                        substrname2=subobj2->content;
                    ScpIntObject * replaceret = new ScpIntObject;

                    size_t pos =obj->content.find(subobj->content);
                    if(pos!=std::string::npos)
                    {
                        obj->content.replace(pos,substrname2.length(),substrname2);
                        replaceret->value= 1;
                    }
                    else
                    {
                        replaceret->value = 0;
                    }
                }
            }
        }
    }
    return TRUE;
}
/*
“移动”命令
操作对象：文件、目录、字符串
*/
BOOL __stdcall Do_Move_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    BOOL bRet = FALSE;
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==3)
    {
        std::string strobj = vtparameters->at(0);
        std::string name1 = vtparameters->at(1);
        std::string name2 = vtparameters->at(2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        if(ObjFile==type)
        {
            bRet =ScpFileObject::Move(name1.c_str(),name2.c_str());

        }
        else if(ObjDirectory == type)
        {
            bRet =ScpDirectoryObject::Move(name1.c_str(),name2.c_str());
        }
        else if (ObjString == type)
        {
            ScpStringObject *strobj1=(ScpStringObject*)currentObjectSpace->FindObject(name1);
            if(strobj1!=NULL)
            {
                ScpStringObject *strobj2=(ScpStringObject*)currentObjectSpace->FindObject(name2);
                if(strobj2!=NULL)
                {
                    strobj2->content=strobj1->content;
                }
                else
                {
                    ScpStringObject *strobj2=new ScpStringObject;
                    if (strobj2)
                    {
                        strobj2->content = strobj1->content;
                        currentObjectSpace->AddObject(name2, strobj2);
                    }
                }
                bRet = TRUE;
            }
        }
    }
    return bRet;
}
/*
“取大小”命令
操作对象：文件、字符串、表对象、内存
*/
BOOL __stdcall Do_Getsize_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==3)
    {
        std::string strobj = vtparameters->at(0);
        std::string name1 = vtparameters->at(1);
        std::string name2 = vtparameters->at(2);
        StringStripQuote(name1);
        StringStripQuote(name2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        if(ObjFile==type)
        {
            ScpFileObject * fileobj=(ScpFileObject*)currentObjectSpace->FindObject(name1);
            if(fileobj && fileobj->GetType()==ObjFile)
            {
                ScpIntObject * intobj =(ScpIntObject *)currentObjectSpace->FindObject(name2);
                if(intobj && intobj->GetType()==ObjInt)
                {
                    intobj->value=(int)fileobj->GetSize(fileobj->filename.c_str());
                }
            }
        }
        else if (ObjString == type)
        {
            ScpStringObject * strobj=(ScpStringObject*)currentObjectSpace->FindObject(name1);
            if(strobj && strobj->GetType()==ObjString)
            {
                ScpIntObject * intobj =(ScpIntObject *)currentObjectSpace->FindObject(name2);
                if(intobj && intobj->GetType()==ObjInt)
                {
                    intobj->value=strobj->GetSize();
                }
            }
        }
        else if(ObjMemory == type)
        {
            ScpMemoryObject * memeobj=(ScpMemoryObject*)currentObjectSpace->FindObject(name1);
            if(memeobj && memeobj->GetType()==ObjMemory)
            {
                ScpIntObject * intobj =(ScpIntObject *)currentObjectSpace->FindObject(name2);
                if(intobj && intobj->GetType()==ObjInt)
                {
                    intobj->value=memeobj->GetSize();
                }
            }
        }
        else if(ObjTable == type)
        {
            ScpTableObject * tableobj =(ScpTableObject*)currentObjectSpace->FindObject(name1);
            if(tableobj && tableobj->GetType()==ObjTable)
            {
                ScpIntObject * intobj =(ScpIntObject *)currentObjectSpace->FindObject(name2);
                if(intobj && intobj->GetType()==ObjInt)
                {
                    intobj->value=tableobj->GetSize();
                }
            }
        }
        else if(ObjStruct ==type)
        {
            ScpStructObject * structobj =(ScpStructObject*)currentObjectSpace->FindObject(name1);
            if(structobj && structobj->GetType()==ObjStruct)
            {
                ScpIntObject * intobj =(ScpIntObject *)currentObjectSpace->FindObject(name2);
                if(intobj && intobj->GetType()==ObjInt)
                {
                    intobj->value = structobj->GetStructMemorySize();
                }
            }
        }
        else if(ObjStructInstance ==type)
        {
            ScpStructInstanceObject * structobj =(ScpStructInstanceObject*)currentObjectSpace->FindObject(name1);
            if(structobj && structobj->GetType()==ObjStructInstance)
            {
                ScpIntObject * intobj =(ScpIntObject *)currentObjectSpace->FindObject(name2);
                if(intobj && intobj->GetType()==ObjInt)
                {
                    intobj->value = structobj->GetStructMemorySize();
                }
            }
        }
    }
    else if(vtparameters->size()==2)
    {
        std::string name1 = vtparameters->at(0);
        std::string name2 = vtparameters->at(1);
        ScpObject * obj=(ScpObject*)currentObjectSpace->FindObject(name1);
        ScpIntObject * intobj =(ScpIntObject *)currentObjectSpace->FindObject(name2);
        if(obj && intobj)
        {
            ScpObjectType type = obj->GetType();
            if(ObjFile == type)
            {
                ScpFileObject * fileobj=(ScpFileObject*)obj;
                intobj->value=(int)fileobj->GetSize(fileobj->filename.c_str());
            }
            else if (ObjString == type)
            {
                ScpStringObject * strobj=(ScpStringObject*)obj;
                intobj->value=strobj->GetSize();
            }
            else if(ObjMemory == type)
            {
                ScpMemoryObject * memeobj=(ScpMemoryObject*)obj;
                intobj->value=memeobj->GetSize();
            }
            else if(ObjTable == type)
            {
                ScpTableObject * tableobj =(ScpTableObject*)obj;
                intobj->value=tableobj->GetSize();
            }
            else if(ObjStruct ==type)
            {
                ScpStructObject * structobj =(ScpStructObject*)obj;
                intobj->value = structobj->GetStructMemorySize();
            }
            else if(ObjStructInstance ==type)
            {
                ScpStructInstanceObject * structobj =(ScpStructInstanceObject*)obj;
                intobj->value = structobj->GetStructMemorySize();
            }
        }
        else
        {
            engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjNotExist);
        }
    }
    return TRUE;
}
/*
“枚举”命令
操作对象：目录、进程
*/
BOOL __stdcall Do_Enum_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string strobj = vtparameters->at(0);
        std::string name1 = vtparameters->at(1);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        if(ObjFile==type)
        {
            ScpDirectoryObject * dirobj=(ScpDirectoryObject*)currentObjectSpace->FindObject(name1);
            if(dirobj)
            {
                dirobj->ShowAllFiles(engine);
            }
        }
        else if(ObjSubDir ==type)
        {
            ScpDirectoryObject * dirobj=(ScpDirectoryObject*)currentObjectSpace->FindObject(name1);
            if(dirobj)
            {
                dirobj->ShowAllSubdir(engine);
            }
        }
    }
    else if(vtparameters->size()==3)
    {
        std::string strobj = vtparameters->at(0);
        std::string name1 = vtparameters->at(1);
        std::string name2 = vtparameters->at(2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(strobj.c_str());
        if(ObjFile==type)
        {
            ScpDirectoryObject * dirobj=(ScpDirectoryObject*)currentObjectSpace->FindObject(name1);
            if(dirobj)
            {
                ScpTableObject * tableobj=dirobj->EnumAllFiles(currentObjectSpace);
                if(currentObjectSpace->FindLocalObject(name2))
                {
                    currentObjectSpace->EraseObject(name2);
                }
                currentObjectSpace->AddObject(name2,tableobj);
            }
        }
        else if(ObjSubDir ==type)
        {
            ScpDirectoryObject * dirobj=(ScpDirectoryObject*)currentObjectSpace->FindObject(name1);
            if(dirobj)
            {
                ScpTableObject * tableobj=dirobj->EnumAllSubDir(currentObjectSpace);
                if(currentObjectSpace->FindLocalObject(name2))
                {
                    currentObjectSpace->EraseObject(name2);
                }
                currentObjectSpace->AddObject(name2,tableobj);
            }
        }
    }
    return TRUE;
}
/*
“生成”命令
操作对象：随机数
*/
BOOL __stdcall Do_Generate_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string randobj = vtparameters->at(0);
        std::string name1 = vtparameters->at(1);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(randobj.c_str());
        if(ObjRandomNumber==type)
        {
            ScpObject * obj=currentObjectSpace->FindObject(name1);
            if(!obj)
            {
                ScpRandomNumberObject * randomobj=new ScpRandomNumberObject;
                randomobj->SetValue(randomobj->GetRandom());

                currentObjectSpace->AddObject(name1,randomobj);
            }
        }
    }
    else if(vtparameters->size()==3)
    {
        std::string randobj = vtparameters->at(0);
        std::string tableobjname = vtparameters->at(1);
        std::string randcount = vtparameters->at(2);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(randobj.c_str());
        if(ObjRandomNumber==type)
        {
            ScpObject * tableobj=currentObjectSpace->FindObject(tableobjname);
            ScpObject * intobj=currentObjectSpace->FindObject(randcount);
            int count ;
            if(intobj && intobj->GetType()==ObjInt)
            {
                count =((ScpIntObject *)intobj)->value;
            }
            else
            {
                if(IsStaticNumber(randcount))
                {
                    count = StringToInt(randcount.c_str());
                }
            }
            if(tableobj)
            {
                if(tableobj->GetType()==ObjTable )
                {
                    ScpIntObject last;
                    for(int i=0; i<count; i++)
                    {
                        std::string temp = tableobjname;
                        temp+=STDSTRINGEXT::Format("%s%d",randobj.c_str(),i);
                        ScpRandomNumberObject * randomobj=new ScpRandomNumberObject;
                        randomobj->SetValue(randomobj->Refresh(last));
                        last=randomobj->GetValue();
                        currentObjectSpace->AddObject(temp,randomobj);
                        ((ScpTableObject *)tableobj)->AddElement(temp,randomobj);
                    }

                }
            }
        }
    }
    return TRUE;
}
/*
“刷新”命令
操作对象：随机数
*/
BOOL __stdcall Do_Refresh_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==1)
    {
        std::string randomobj_name = vtparameters->at(0);
        ScpRandomNumberObject * randomobj=(ScpRandomNumberObject * )currentObjectSpace->FindObject(randomobj_name);
        if(randomobj)
        {
            if(randomobj->GetType()==ObjRandomNumber)
            {
                randomobj->SetValue(randomobj->Refresh(randomobj->value));
            }
            else
            {
                engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjectDoNotSupportCommand);
            }
        }
        else
        {
            engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjNotExist);
        }
    }
    else if(vtparameters->size()==2)
    {
        std::string objtypename = vtparameters->at(0);
        std::string randomobj_name = vtparameters->at(1);
        ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(objtypename.c_str());
        if(ObjRandomNumber==type)
        {
            ScpRandomNumberObject * randomobj=(ScpRandomNumberObject * )currentObjectSpace->FindObject(randomobj_name);
            if(randomobj)
            {
                if(randomobj->GetType()==ObjRandomNumber)
                {
                    randomobj->SetValue(randomobj->Refresh(randomobj->value));
                }
                else
                {
                    engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjectDoNotSupportCommand);
                }
            }
            else
            {
                engine->PrintError(ScpObjectNames::GetSingleInsatnce()->scpErrorObjNotExist);
            }
        }
    }
    return TRUE;
}
/*
“取元素”命令
操作对象：表对象
*/
BOOL __stdcall  Do_Getelement_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==3)
    {
        std::string tablename = vtparameters->at(0);
        std::string element=vtparameters->at(1);
        std::string indexstr=vtparameters->at(2);
        int index=0;
        ScpTableObject * tableobj=(ScpTableObject*)currentObjectSpace->FindObject(tablename);
        if(tableobj)
        {
            ScpIntObject * intobj=(ScpIntObject*)currentObjectSpace->FindObject(indexstr);
            if(intobj)
            {
                index =intobj->value;
                ScpObject * elementobj = tableobj->GetElement(index);
                if(elementobj)
                {
                    ScpObjectSpace * globalspace =currentObjectSpace->parentspace;
                    while(globalspace->parentspace!=NULL)
                    {
                        globalspace=globalspace->parentspace;
                    }
                    if(globalspace->FindObject(element)!=NULL)
                    {
                        globalspace->EraseObject(element);
                    }
                    globalspace->AddObject(element,elementobj);
                }
            }
        }
    }
    return TRUE;
}
/*
“遍历”命令
操作对象：表对象 列表对象
*/
BOOL __stdcall Do_Traverse_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
{
    ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
    if(vtparameters->size()==2)
    {
        std::string tablename = vtparameters->at(0);
        std::string funcname=vtparameters->at(1);
        ScpObject *obj = currentObjectSpace->FindObject(tablename);
        if(obj &&obj->GetType() ==ObjTable )
        {
            ScpTableObject * tableobj = (ScpTableObject *)obj;
            ScpFunctionObject * func=(ScpFunctionObject*)currentObjectSpace->FindObject(funcname);
            if(func && (func->FormalParameters.size()==1))
            {
                BOOL Clone = FALSE;
                std::string OldName;
                if (currentObjectSpace->IsMyParentSpace(func->FunctionObjectSpace) || currentObjectSpace == func->FunctionObjectSpace)
                {
                    //	//说明是递归的函数调用
                    Clone = TRUE;
                }

                int count =0;
                count=tableobj->GetSize();
                for(int index=0; index<count; index++)
                {
                    ScpObject * elementobj = tableobj->GetElement(index);
                    if(elementobj)
                    {

                        std::string elementname =currentObjectSpace->userobject.GetObjectName(elementobj);
                        ScpObjectSpace * OldObjectSpace = NULL;
                        VTPARAMETERS OldRealParameters;
                        if (Clone)
                        {
                            OldObjectSpace = func->FunctionObjectSpace;
                            OldName=func->Name;
                            func->FunctionObjectSpace = new ScpObjectSpace;
                            func->FunctionObjectSpace->belongto = func;
                            func->FunctionObjectSpace->parentspace = currentObjectSpace;
                            func->FunctionObjectSpace->ObjectSpaceType = Space_Function;
                            OldRealParameters = func->RealParameters;
                            func->RealParameters.clear();
                            func->Name=func->GetCloneName();
                        }
                        if (func->RealParameters.size() == 0)
                        {
                            func->RealParameters.push_back(elementname);
                        }
                        func->Do(engine);
                        if (Clone)
                        {
                            ScpObjectSpace * tempObjectSpace = func->FunctionObjectSpace;
                            func->FunctionObjectSpace = OldObjectSpace;
                            delete tempObjectSpace;
                            func->RealParameters = OldRealParameters;
                            func->Name = OldName;
                        }

                    }
                }

            }
        }
        else if (obj &&obj->GetType() == ObjList)
        {
            ScpListObject * listobj = (ScpListObject *)obj;
            ScpFunctionObject * func = (ScpFunctionObject*)currentObjectSpace->FindObject(funcname);
            if (func && (func->FormalParameters.size() == 1))
            {
                BOOL Clone = FALSE;
                std::string OldName;
                if (currentObjectSpace->IsMyParentSpace(func->FunctionObjectSpace) || currentObjectSpace == func->FunctionObjectSpace)
                {
                    //	//说明是递归的函数调用
                    Clone = TRUE;
                }

				int count = 0;
				count = listobj->GetSize();
				for (int index = 0;index < count;index++)
				{
					ScpObject * elementobj = listobj->GetElement(index);
					if (elementobj)
					{

						std::string elementname = currentObjectSpace->userobject.GetObjectName(elementobj);
						ScpObjectSpace * OldObjectSpace = NULL;
						VTPARAMETERS OldRealParameters;
						if (Clone)
						{
							OldObjectSpace = func->FunctionObjectSpace;
							OldName = func->Name;
							func->FunctionObjectSpace = new ScpObjectSpace;
							func->FunctionObjectSpace->belongto = func;
							func->FunctionObjectSpace->parentspace = currentObjectSpace;
							func->FunctionObjectSpace->ObjectSpaceType = Space_Function;
							OldRealParameters = func->RealParameters;
							func->RealParameters.clear();
							func->Name = func->GetCloneName();
						}
						if (func->RealParameters.size() == 0)
						{
							func->RealParameters.push_back(elementname);
						}
						func->Do(engine);
						if (Clone)
						{
							ScpObjectSpace * tempObjectSpace = func->FunctionObjectSpace;
							func->FunctionObjectSpace = OldObjectSpace;
							delete tempObjectSpace;
							func->RealParameters = OldRealParameters;
							func->Name = OldName;
						}
					}
				}
			}				
		}
		else if (obj &&obj->GetType() == ObjRange)
		{
			ScpRangeObject * rangeobject = (ScpRangeObject *)obj;
			ScpFunctionObject * func = (ScpFunctionObject*)currentObjectSpace->FindObject(funcname);
			if (func && (func->FormalParameters.size() == 1))
			{
				BOOL Clone = FALSE;
				std::string OldName;
				if (currentObjectSpace->IsMyParentSpace(func->FunctionObjectSpace) || currentObjectSpace == func->FunctionObjectSpace)
				{
					//	//说明是递归的函数调用		
					Clone = TRUE;
				}

				int count = 0;
				count = rangeobject->Elements.size();
				for (int index = 0;index < count;index++)
				{
					ScpObject * elementobj = rangeobject->Elements.at(index);
					if (elementobj)
					{

						std::string elementname = currentObjectSpace->userobject.GetObjectName(elementobj);
						elementname = "element"+currentObjectSpace->GetNewTempObjectName();						
						currentObjectSpace->AddObject(elementname, elementobj);
						ScpObjectSpace * OldObjectSpace = NULL;
						VTPARAMETERS OldRealParameters;
						if (Clone)
						{
							OldObjectSpace = func->FunctionObjectSpace;
							OldName = func->Name;
							func->FunctionObjectSpace = new ScpObjectSpace;
							func->FunctionObjectSpace->belongto = func;
							func->FunctionObjectSpace->parentspace = currentObjectSpace;
							func->FunctionObjectSpace->ObjectSpaceType = Space_Function;
							OldRealParameters = func->RealParameters;
							func->RealParameters.clear();
							func->Name = func->GetCloneName();
						}
						if (func->RealParameters.size() == 0)
						{
							func->RealParameters.push_back(elementname);
						}
						func->Do(engine);
						if (Clone)
						{
							ScpObjectSpace * tempObjectSpace = func->FunctionObjectSpace;
							func->FunctionObjectSpace = OldObjectSpace;
							delete tempObjectSpace;
							func->RealParameters = OldRealParameters;
							func->Name = OldName;
						}
						currentObjectSpace->EraseObject(elementname);
					}
				}
			}
		}
	}
    else if (vtparameters->size() > 2)
    {
        size_t paramcount = vtparameters->size();
        std::string tablename = vtparameters->at(0);
        std::string funcname = vtparameters->at(1);
        ScpObject* obj = currentObjectSpace->FindObject(tablename);
        if (obj && obj->GetType() == ObjTable)
        {
            ScpTableObject* tableobj = (ScpTableObject*)obj;
            ScpFunctionObject* func = (ScpFunctionObject*)currentObjectSpace->FindObject(funcname);
            if (func && (func->FormalParameters.size() == paramcount - 1))
            {
                BOOL Clone = FALSE;
                std::string OldName;
                if (currentObjectSpace->IsMyParentSpace(func->FunctionObjectSpace) || currentObjectSpace == func->FunctionObjectSpace)
                {
                    //	//说明是递归的函数调用
                    Clone = TRUE;
                }
                int count = 0;
                count = tableobj->GetSize();
                for (int index = 0; index < count; index++)
                {
                    ScpObject* elementobj = tableobj->GetElement(index);
                    if (elementobj)
                    {
                        std::string elementname = currentObjectSpace->userobject.GetObjectName(elementobj);
                        ScpObjectSpace* OldObjectSpace = NULL;
                        VTPARAMETERS OldRealParameters;
                        if (Clone)
                        {
                            OldObjectSpace = func->FunctionObjectSpace;
                            OldName = func->Name;
                            func->FunctionObjectSpace = new ScpObjectSpace;
                            func->FunctionObjectSpace->belongto = func;
                            func->FunctionObjectSpace->parentspace = currentObjectSpace;
                            func->FunctionObjectSpace->ObjectSpaceType = Space_Function;
                            OldRealParameters = func->RealParameters;
                            func->RealParameters.clear();
                            func->Name = func->GetCloneName();
                        }
                        if (func->RealParameters.size() == 0)
                        {
                            func->RealParameters.push_back(elementname);
                        }
                        for (size_t i = 2; i < paramcount; i++)
                        {
                            std::string paramname = vtparameters->at(i);
                            func->RealParameters.push_back(paramname);
                        }
                        func->Do(engine);
                        if (Clone)
                        {
                            ScpObjectSpace* tempObjectSpace = func->FunctionObjectSpace;
                            func->FunctionObjectSpace = OldObjectSpace;
                            delete tempObjectSpace;
                            func->RealParameters = OldRealParameters;
                            func->Name = OldName;
                        }
                    }
                }
            }
        }
        else if (obj && obj->GetType() == ObjList)
        {
            ScpListObject* listobj = (ScpListObject*)obj;
            ScpFunctionObject* func = (ScpFunctionObject*)currentObjectSpace->FindObject(funcname);
            if (func && (func->FormalParameters.size() == paramcount - 1))
            {
                BOOL Clone = FALSE;
                std::string OldName;
                if (currentObjectSpace->IsMyParentSpace(func->FunctionObjectSpace) || currentObjectSpace == func->FunctionObjectSpace)
                {
                    //	//说明是递归的函数调用
                    Clone = TRUE;
                }
                int count = 0;
                count = listobj->GetSize();
                for (int index = 0; index < count; index++)
                {
                    ScpObject* elementobj = listobj->GetElement(index);
                    if (elementobj)
                    {
                        std::string elementname = currentObjectSpace->userobject.GetObjectName(elementobj);
                        ScpObjectSpace* OldObjectSpace = NULL;
                        VTPARAMETERS OldRealParameters;
                        if (Clone)
                        {
                            OldObjectSpace = func->FunctionObjectSpace;
                            OldName = func->Name;
                            func->FunctionObjectSpace = new ScpObjectSpace;
                            func->FunctionObjectSpace->belongto = func;
                            func->FunctionObjectSpace->parentspace = currentObjectSpace;
                            func->FunctionObjectSpace->ObjectSpaceType = Space_Function;
                            OldRealParameters = func->RealParameters;
                            func->RealParameters.clear();
                            func->Name = func->GetCloneName();
                        }
                        if (func->RealParameters.size() == 0)
                        {
                            func->RealParameters.push_back(elementname);
                        }
                        for (size_t i = 2; i < paramcount; i++)
                        {
                            std::string paramname = vtparameters->at(i);
                            func->RealParameters.push_back(paramname);
                        }
                        func->Do(engine);
                        if (Clone)
                        {
                            ScpObjectSpace* tempObjectSpace = func->FunctionObjectSpace;
                            func->FunctionObjectSpace = OldObjectSpace;
                            delete tempObjectSpace;
                            func->RealParameters = OldRealParameters;
                            func->Name = OldName;
                        }
                    }
                }
            }
        }
        else if (obj && obj->GetType() == ObjRange)
        {
            ScpRangeObject* rangeobject = (ScpRangeObject*)obj;
            ScpFunctionObject* func = (ScpFunctionObject*)currentObjectSpace->FindObject(funcname);
            if (func && (func->FormalParameters.size() == paramcount - 1))
            {
                BOOL Clone = FALSE;
                std::string OldName;
                if (currentObjectSpace->IsMyParentSpace(func->FunctionObjectSpace) || currentObjectSpace == func->FunctionObjectSpace)
                {
                    //	//说明是递归的函数调用
                    Clone = TRUE;
                }
                int count = 0;
                count = rangeobject->Elements.size();
                for (int index = 0; index < count; index++)
                {
                    ScpObject* elementobj = rangeobject->Elements.at(index);
                    if (elementobj)
                    {
                        std::string elementname = currentObjectSpace->userobject.GetObjectName(elementobj);
                        elementname = "element" + currentObjectSpace->GetNewTempObjectName();
                        currentObjectSpace->AddObject(elementname, elementobj);
                        ScpObjectSpace* OldObjectSpace = NULL;
                        VTPARAMETERS OldRealParameters;
                        if (Clone)
                        {
                            OldObjectSpace = func->FunctionObjectSpace;
                            OldName = func->Name;
                            func->FunctionObjectSpace = new ScpObjectSpace;
                            func->FunctionObjectSpace->belongto = func;
                            func->FunctionObjectSpace->parentspace = currentObjectSpace;
                            func->FunctionObjectSpace->ObjectSpaceType = Space_Function;
                            OldRealParameters = func->RealParameters;
                            func->RealParameters.clear();
                            func->Name = func->GetCloneName();
                        }
                        if (func->RealParameters.size() == 0)
                        {
                            func->RealParameters.push_back(elementname);
                        }
                        for (size_t i = 2; i < paramcount; i++)
                        {
                            std::string paramname = vtparameters->at(i);
                            func->RealParameters.push_back(paramname);
                        }
                        func->Do(engine);
                        if (Clone)
                        {
                            ScpObjectSpace* tempObjectSpace = func->FunctionObjectSpace;
                            func->FunctionObjectSpace = OldObjectSpace;
                            delete tempObjectSpace;
                            func->RealParameters = OldRealParameters;
                            func->Name = OldName;
                        }
                    }
                }
            }
        }
    }
    return TRUE;
}
	/*
	“排序”命令
	*/
	BOOL  __stdcall Do_Sort_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		return TRUE;
	}
	/*
	“格式化”命令
	操作对象：字符串
	*/
	BOOL  __stdcall Do_Format_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		if(vtparameters->size()>=2)
		{
			std::string str = vtparameters->at(0);				
			std::string name1= vtparameters->at(1);
			std::string wstrForm;
			if(vtparameters->size()>=3)
			 wstrForm = vtparameters->at(2);
			ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(str.c_str());
			if (ObjString == type)
			{
				ScpStringObject * strobj=(ScpStringObject*)currentObjectSpace->FindObject(name1);
				if(strobj)
				{
					VTPARAMETERS param;
					if(vtparameters->size()>3)
					{
						for(unsigned int i=3;i<vtparameters->size();i++)
						{
							ScpObject * obj = (ScpObject*)currentObjectSpace->FindObject(vtparameters->at(i));
							if(obj)
							{				
								std::string temp = obj->ToString();
								param.push_back(temp);						
							}
							else
							{
								param.push_back(vtparameters->at(i));
							}
						}
					}
					strobj->Format(strobj,wstrForm.c_str(),param);
				}
			}	
			else
			{
				ScpStringObject * strobj=(ScpStringObject*)currentObjectSpace->FindObject(str);
				if(strobj)
				{
					VTPARAMETERS param;
					if(vtparameters->size()>2)
					{
						for(unsigned int i=2;i<vtparameters->size();i++)
						{
							ScpObject * obj = (ScpObject*)currentObjectSpace->FindObject(vtparameters->at(i));
							if(obj)
							{				
								std::string temp = obj->ToString();
								param.push_back(temp);						
							}
							else
							{
								param.push_back(vtparameters->at(i));
							}
						}
					}
					strobj->Format(strobj,name1.c_str(),param);
				}
			}
		}
	
		return TRUE;
	}
	/*
	“获取”命令
	操作对象：
	*/
	BOOL __stdcall Do_Get_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
	 	if(vtparameters->size()==3)
		{
			std::string objname1 = vtparameters->at(0);				
			std::string objname2= vtparameters->at(1);
			std::string objname3= vtparameters->at(2);
			if(objname1==ScpObjectNames::GetSingleInsatnce()->strObjType)
			{
				ScpStringObject * obj= (ScpStringObject*)currentObjectSpace->FindObject(objname3);
				if(obj && obj->GetType()==ObjString)
				{
					ScpObject *temp = currentObjectSpace->FindObject(objname2);
					if(temp)
					{
						obj->content = ScpGlobalObject::GetInstance()->GetTypeName(temp->GetType());
					}
					else 
					{
						if(IsStaticNumber(objname2))
						{
							obj->content = ScpObjectNames::GetSingleInsatnce()->strObjStaticInt;
						}
						else if(IsStaticDoubleNumber(objname2))
						{
							obj->content = ScpObjectNames::GetSingleInsatnce()->strObjStaticDouble;
						}
						else 
						{
							obj->content = ScpObjectNames::GetSingleInsatnce()->strObjStaticString;
						}		
					}
				}
			}
			else if(objname1== str_CN_ObjAttribute||
				objname1 == str_EN_ObjAttribute)
			{		
				ScpFileObject * fileobj=(ScpFileObject*)currentObjectSpace->FindObject(objname2);
				if(fileobj)
				{
					ScpTableObject * obj=(ScpTableObject*)currentObjectSpace->FindObject(objname3);
					if(obj)
					{					
						obj->EmptyElement();
					}
					obj = fileobj->GetArrtibute(fileobj->filename,obj);
					currentObjectSpace->AddObject(objname3,obj);
				}
			}	
			else if(objname1==ScpObjectNames::GetSingleInsatnce()->strObjEnvironmentVariable)
			{
				ScpStringObject * obj= (ScpStringObject*)currentObjectSpace->FindObject(objname2);
				if(obj)
				{
					objname2=obj->content;
				}			
				ScpStringObject * obj2= (ScpStringObject*)currentObjectSpace->FindObject(objname3);
				if(obj2)
				{		
					char Value[2048];
	#ifdef _WIN32	
					GetEnvironmentVariableA(objname2.c_str(),Value,2048);
	#endif
					obj2->content=Value;
				}
			}		
			else 
			{
				ScpObject * obj=(ScpObject*)currentObjectSpace->FindObject(objname1);
				if(obj)			
				{				
					if(obj->GetType()==ObjFile)
					{
						ScpFileObject *fileobj = (ScpFileObject*)obj;
						ScpStringObject * obj2= (ScpStringObject*)currentObjectSpace->FindObject(objname3);
						if(obj2 && obj2->GetType()==ObjString)
						{	
	
								char Drv[MAX_PATH];
								char Dir[MAX_PATH];
								char Name[MAX_PATH];
								char Ext[MAX_PATH];
	#ifdef _WIN32
								_splitpath_s(fileobj->filename.c_str(),Drv,MAX_PATH,Dir,MAX_PATH,Name,MAX_PATH,Ext,MAX_PATH);
	#endif
								if(objname2== str_CN_ObjDrive|| objname2 == str_EN_ObjDrive)
								{
									obj2->content=Drv;
								}
								else if(objname2 == str_EN_ObjFilename || objname2 == str_CN_ObjFilename)
								{
									obj2->content=Name;
									obj2->content+=Ext;
								}
								else if(objname2 == str_EN_ObjPath || objname2 == str_CN_ObjPath)
								{
									obj2->content=Drv;
									obj2->content+=Dir;
								}
								else if(objname2 == str_EN_ObjExtension || objname2 == str_CN_ObjExtension)
								{
									obj2->content=Ext;
								}
							
						}
						else
						{			
							ScpTimeObject *  timeobj =NULL;
							if(objname2== str_CN_file_create_time|| objname2 == str_EN_file_create_time)
							{
								timeobj = ScpFileObject::GetCreateTime(fileobj->filename.c_str());							
							}
							else if(objname2== str_CN_file_access_time|| objname2 == str_EN_file_access_time)
							{
								timeobj = ScpFileObject::GetLastAccessTime(fileobj->filename.c_str());							
							}
							else if(objname2== str_CN_file_modify_time|| objname2 == str_EN_file_modify_time)
							{
								timeobj = ScpFileObject::GetLastModifyTime(fileobj->filename.c_str());							
							}	
							ScpTimeObject * obj3= (ScpTimeObject*)currentObjectSpace->FindObject(objname3);
							if(timeobj)
							{
								if(obj3)
								{
									obj3->value = timeobj->value;
									delete timeobj;
								}
								else
									currentObjectSpace->AddObject(objname3,timeobj);
							}
						}
					}
					else if(obj->GetType()==ObjString)
					{
						ScpStringObject * stringobj = (ScpStringObject *)obj;
	
						ScpStringObject * obj2= (ScpStringObject*)currentObjectSpace->FindObject(objname3);
						if(obj2)
						{	
							if(obj2->GetType()==ObjString)
							{
								char Drv[MAX_PATH];
								char Dir[MAX_PATH];
								char Name[MAX_PATH];
								char Ext[MAX_PATH];
	#ifdef _WIN32
								_splitpath_s(stringobj->content.c_str(),Drv,MAX_PATH,Dir,MAX_PATH,Name,MAX_PATH,Ext,MAX_PATH);
	#endif
								if(objname2== str_EN_ObjDrive|| objname2 == str_CN_ObjDrive)
								{
									obj2->content=Drv;
								}
								else if(objname2 == str_EN_ObjFilename || objname2 == str_CN_ObjFilename)
								{
									obj2->content=Name;
									obj2->content+=Ext;
								}
								else if(objname2 == str_EN_ObjPath || objname2 == str_CN_ObjPath)
								{
									obj2->content=Drv;
									obj2->content+=Dir;
								}
								else if(objname2 == str_EN_ObjExtension || objname2 == str_CN_ObjExtension)
								{
									obj2->content=Ext;
								}
							}
						}
					}
					else if(obj->GetType()==ObjTime)
					{
						ScpTimeObject * timeobj =(ScpTimeObject *)obj;
						ScpIntObject * intobj =(ScpIntObject * )currentObjectSpace->FindObject(objname3);
						if(intobj)
						{
							if(intobj->GetType()==ObjInt)
							{
								if(objname2==ScpObjectNames::GetSingleInsatnce()->strHour)
								{
									intobj->value=timeobj->GetHour();
								}
								else if(objname2==ScpObjectNames::GetSingleInsatnce()->strMinute)
								{
									intobj->value=timeobj->GetMinute();
								}
								else if(objname2==ScpObjectNames::GetSingleInsatnce()->strSecond)
								{
									intobj->value=timeobj->GetSecond();
								}
							}
						}
					}
					
				}
			}
		}
		else if(vtparameters->size()==2)
		{	
			std::string objname1 = vtparameters->at(0);				
			std::string objname2= vtparameters->at(1);
			if(objname1 == ScpObjectNames::GetSingleInsatnce()->strObjCurrentDirectory)
			{
				ScpObject * obj= currentObjectSpace->FindObject(objname2);
				if(obj && obj->GetType()==ObjString)
				{
					char CurrDir[MAX_PATH]={0};
	#ifdef _WIN32
					GetCurrentDirectoryA(MAX_PATH,CurrDir);
	#else
	
	#endif
					((ScpStringObject*)obj)->content = CurrDir;
				}
			}
		}
		else if(vtparameters->size()==4)
		{
			std::string objname1 = vtparameters->at(0);				
			std::string objname2= vtparameters->at(1);
			std::string objname3= vtparameters->at(2);
			std::string objname4= vtparameters->at(3);
			ScpObject * obj=(ScpObject*)currentObjectSpace->FindObject(objname1);
			if(obj)			
			{	
				if (obj->GetType()==ObjJson)
				{
					if(objname2== str_CN_ObjValue||
						objname2 == str_EN_ObjValue)
					{
						StringStripQuote(objname3);
						ScpObject * obj3= currentObjectSpace->FindObject(objname3);
						if(obj3 && obj3->GetType()==ObjString)
						{
							objname3 = ((ScpStringObject*)obj3)->content;				
						}
	
						ScpObject * obj4= currentObjectSpace->FindObject(objname4);
						if(obj4 && obj4->GetType()==ObjString)
						{
							((ScpJsonObject *)obj)->GetValue(objname3,((ScpStringObject*)obj4)->content);		
						}	
						else if(obj4 && obj4->GetType()==ObjInt)
						{
							((ScpJsonObject *)obj)->GetValue(objname3,((ScpIntObject*)obj4)->value);		
						}	
					}
				}
			}		
		}	
		return TRUE;
	}
	/*
	“设置”命令
	操作对象：
	*/
	BOOL __stdcall Do_Set_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		BOOL ret = FALSE;
		if(vtparameters->size()==3)
		{
			std::string objname1 = vtparameters->at(0);				
			std::string objname2= vtparameters->at(1);
			std::string objname3= vtparameters->at(2);
			if(objname1== str_CN_ObjAttribute||
				objname1 == str_EN_ObjAttribute)
			{		
				ScpFileObject * fileobj=(ScpFileObject*)currentObjectSpace->FindObject(objname2);
				if(fileobj)
				{
					ScpTableObject * table = (ScpTableObject*)currentObjectSpace->FindObject(objname3);
					if(table)
					{
						ret = fileobj->ChangeAttribute(fileobj->filename,table);
					}
				}			
			}	
			else if(objname1==ScpObjectNames::GetSingleInsatnce()->strObjEnvironmentVariable)
			{
				ScpStringObject * obj= (ScpStringObject*)currentObjectSpace->FindObject(objname2);
				if(obj)
				{
					objname2=obj->content;
				}			
				ScpStringObject * obj2= (ScpStringObject*)currentObjectSpace->FindObject(objname3);
				if(obj2)
				{	
	#ifdef _WIN32
					SetEnvironmentVariableA(objname2.c_str(),(char *)obj2->content.c_str());
	#endif	
				}
			}		
		}
		else if(vtparameters->size()==2)
		{	
			std::string objname1 = vtparameters->at(0);				
			std::string objname2= vtparameters->at(1);
			if(objname1 == ScpObjectNames::GetSingleInsatnce()->strObjCurrentDirectory)
			{
				ScpObject * obj= currentObjectSpace->FindObject(objname2);
				std::string curdir;
				if(obj && obj->GetType()==ObjString)
				{
					curdir = ((ScpStringObject*)obj)->content;				
				}
				else
				{
					curdir = objname2;
				}
	#ifdef _WIN32
				SetCurrentDirectoryA(curdir.c_str());
	#else
	
	#endif
			}
		}	
		else if(vtparameters->size()==4)
		{
			std::string objname1 = vtparameters->at(0);				
			std::string objname2= vtparameters->at(1);
			std::string objname3= vtparameters->at(2);
			std::string objname4= vtparameters->at(3);
			ScpObject * obj = (ScpObject *)currentObjectSpace->FindObject(objname1);
			if (obj && obj->GetType()==ObjJson)
			{
				if(objname2== str_CN_ObjValue||
					objname2 == str_EN_ObjValue)
				{
					StringStripQuote(objname3);
					ScpObject * obj3= currentObjectSpace->FindObject(objname3);
					if(obj3 && obj3->GetType()==ObjString)
					{
						objname3 = ((ScpStringObject*)obj3)->content;				
					}
					ScpObject * obj4= currentObjectSpace->FindObject(objname4);
					if(obj4 && obj4->GetType()==ObjString)
					{
						((ScpJsonObject *)obj)->SetValue(objname3,((ScpStringObject*)obj4)->content);		
					}	
					else if(obj4 && obj4->GetType()==ObjInt)
					{
						((ScpJsonObject *)obj)->SetValue(objname3,((ScpIntObject*)obj4)->value);		
					}			
				}
			}	
		}
		return ret;
	}
	/*
	“转换”命令
	操作对象：
	*/
	BOOL __stdcall Do_Transform_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		if(vtparameters->size()==2)
		{
			std::string objname1 = vtparameters->at(0);				
			std::string transtype= vtparameters->at(1);
			ScpObject * obj1=(ScpObject*)currentObjectSpace->FindObject(objname1);
			if (obj1)
			{
				ScpObjectType objtype =obj1->GetType(); 
				if(objtype==ObjString)
				{
					if(transtype==ScpObjectNames::GetSingleInsatnce()->scpStringUpper)
					{
						ScpStringObject::ToUpper((ScpStringObject*)obj1);
					}
					else if (transtype == ScpObjectNames::GetSingleInsatnce()->scpStringLower)
					{
						ScpStringObject::ToLower((ScpStringObject*)obj1);
					}
					else if(transtype == ScpObjectNames::GetSingleInsatnce()->scpStringReverse)
					{
						ScpStringObject::Reverse((ScpStringObject*)obj1);
					}
					else if (transtype == ScpObjectNames::GetSingleInsatnce()->scpStringReverse)
					{
						ScpStringObject::ToHex((ScpStringObject*)obj1);
					}
					else 
					{
						ScpObject * obj2=(ScpObject*)currentObjectSpace->FindObject(transtype);
						if(obj2)
						{
							((ScpStringObject*)obj1)->content = obj2->ToString();
						}
					}
				}
				else if(objtype==ObjInt)
				{
					ScpObject * obj2=(ScpObject*)currentObjectSpace->FindObject(transtype);
					if(obj2  && obj2->GetType()==ObjString)
					{
						if(IsStaticNumber(((ScpStringObject*)obj2)->content))
						{
							((ScpIntObject*)obj1)->value = StringToInt(((ScpStringObject*)obj2)->content);						
						}
						else
						{
							std::string ErrorMessage=STDSTRINGEXT::Format("%s %s",((ScpStringObject*)obj2)->content.c_str(),ScpObjectNames::GetSingleInsatnce()->scpErrorNotInt);
							engine->PrintError(ErrorMessage);
						}
					}				
				}
				else if(objtype==ObjDouble)
				{
					ScpObject * obj2=(ScpObject*)currentObjectSpace->FindObject(transtype);
					if(obj2  && obj2->GetType()==ObjString)
					{
						if(IsStaticDoubleNumber(((ScpStringObject*)obj2)->content))
						{
							((ScpDoubleObject*)obj1)->value = StringToDouble(((ScpStringObject*)obj2)->content);						
						}
						else if(IsStaticNumber(((ScpStringObject*)obj2)->content))
						{
							((ScpDoubleObject*)obj1)->value = StringToDouble(((ScpStringObject*)obj2)->content);						
						}
						else
						{
							std::string ErrorMessage=STDSTRINGEXT::Format("%s %s",((ScpStringObject*)obj2)->content.c_str(),ScpObjectNames::GetSingleInsatnce()->scpErrorNotNumber);
							engine->PrintError(ErrorMessage);
						}
					}				
				}
			}	
		}
		return TRUE;
	}
	/*
	“分割”命令
	操作对象：字符串
	*/
	BOOL __stdcall Do_Split_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		if(vtparameters->size()==3)
		{
			std::string objname1 = vtparameters->at(0);				
			std::string objname2= vtparameters->at(1);
			std::string objname3= vtparameters->at(2);
			ScpStringObject * strobj1=(ScpStringObject*)currentObjectSpace->FindObject(objname1);
			if (strobj1)
			{
				objname1=strobj1->content;
				ScpStringObject * strobj2=(ScpStringObject*)currentObjectSpace->FindObject(objname2);
				if(strobj2)
				{
					objname2 = strobj2->content;
	
					ScpTableObject * tableobj=(ScpTableObject*)currentObjectSpace->FindObject(objname3);
					if(!tableobj)
					{
						tableobj = new ScpTableObject;
						size_t pos = objname1.find(objname2);
						while(pos!=std::string::npos)
						{
							std::string temp = objname1.substr(0,pos);
							ScpStringObject *  tempstrobj = new ScpStringObject;
							tempstrobj->content = temp;
							currentObjectSpace->AddObject(temp,tempstrobj);
							tableobj->AddElement(temp,tempstrobj);
							objname1=objname1.substr(pos+1,objname1.length()-pos-1);
							pos = objname1.find(objname2);
						}
						if(!objname1.empty())
						{
							std::string temp = objname1;
							ScpStringObject *  tempstrobj = new ScpStringObject;
							tempstrobj->content = temp;
							currentObjectSpace->AddObject(temp,tempstrobj);
							tableobj->AddElement(temp,tempstrobj);
						}
						currentObjectSpace->AddObject(objname3,tableobj);
					}
				}			
			}	
		}
		else if(vtparameters->size()==4)
		{
			std::string objnametype = vtparameters->at(0);				
			std::string objname1= vtparameters->at(1);
			std::string objname2= vtparameters->at(2);
			std::string objname3= vtparameters->at(3);
			ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(objnametype.c_str());
			if (ObjString == type)
			{
				ScpStringObject * strobj1=(ScpStringObject*)currentObjectSpace->FindObject(objname1);
				if (strobj1)
				{
					objname1=strobj1->content;
					ScpStringObject * strobj2=(ScpStringObject*)currentObjectSpace->FindObject(objname2);
					if(strobj2)
					{
						objname2 = strobj2->content;
	
						ScpTableObject * tableobj=(ScpTableObject*)currentObjectSpace->FindObject(objname3);
						if(!tableobj)
						{
							tableobj = new ScpTableObject;
							size_t pos = objname1.find(objname2);
							while(pos!=std::string::npos)
							{
								std::string temp = objname1.substr(0,pos);
								ScpStringObject *  tempstrobj = new ScpStringObject;
								tempstrobj->content = temp;
								currentObjectSpace->AddObject(temp,tempstrobj);
								tableobj->AddElement(temp,tempstrobj);
								objname1=objname1.substr(pos+1,objname1.length()-pos-1);
								pos = objname1.find(objname2);
							}
							if(!objname1.empty())
							{
								std::string temp = objname1;
								ScpStringObject *  tempstrobj = new ScpStringObject;
								tempstrobj->content = temp;
								currentObjectSpace->AddObject(temp,tempstrobj);
								tableobj->AddElement(temp,tempstrobj);
							}
							currentObjectSpace->AddObject(objname3,tableobj);
						}
					}			
				}	
			}
		}
		return TRUE;
	}
	/*
	“压入”命令
	操作对象：
	*/
	BOOL __stdcall Do_Push_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		return TRUE;
	}
	/*
	"打印”命令
	操作对象：文件
	*/
	BOOL __stdcall Do_Print_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		if(vtparameters->size()==1)
		{
			std::string objname = vtparameters->at(0);
			ScpObject * obj = currentObjectSpace->FindObject(objname);
			if(obj)
			{
				if(obj->GetType()==ObjFile)
				{
					((ScpFileObject * )obj)->Print(engine);
				}
			}
	
		}
		return TRUE;
	}
	BOOL __stdcall Do_Create_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		BOOL bRet = FALSE;
		DWORD dwError;
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		if(vtparameters->size()==2)
		{
			std::string objtype = vtparameters->at(0);
			std::string objname = vtparameters->at(1);
			std::string wsdir;
			ScpObjectType type = ScpGlobalObject::GetInstance()->GetType(objtype.c_str());
			if(type==ObjDirectory)
			{		
				ScpObject * obj = currentObjectSpace->FindObject(objname);
				if(obj)
				{
					if(obj->GetType()==ObjString)
					{
						wsdir = ((ScpStringObject * )obj)->content;
	
					}
				}
				else
				{
					wsdir = objname;
				}
	#ifdef _WIN32
						bRet = CreateDirectoryA(wsdir.c_str(),NULL);
						dwError = GetLastError();
	#else
	
	#endif
			}
	
		}
		return bRet;
	}
	BOOL __stdcall  Do_Save_Command(VTPARAMETERS * vtparameters,CScriptEngine * engine)
	{
		BOOL bRet = FALSE;
		ScpObjectSpace * currentObjectSpace = engine->GetCurrentObjectSpace();
		if(vtparameters->size()==1)
		{
			std::string wsobjectname=vtparameters->at(0);
			ScpObject * obj = currentObjectSpace->FindObject(wsobjectname);
			if(obj && obj->GetType()==ObjFile)
			{
				((ScpFileObject *)obj)->Save("");
			}
		}
		else if(vtparameters->size()==2)
		{
			std::string wsobjectname=vtparameters->at(0);
			std::string wsobjectname1=vtparameters->at(1);
			ScpObject * obj = currentObjectSpace->FindObject(wsobjectname);
			if(obj && obj->GetType()==ObjFile)
			{
				ScpObject * obj1 = currentObjectSpace->FindObject(wsobjectname1);
				if(obj1 && obj1->GetType()==ObjString)
				{
					wsobjectname1=((ScpStringObject *)obj1)->content;
				}
				((ScpFileObject *)obj)->Save(wsobjectname1);
			}
		}
		return bRet;
	}




}
