#pragma once
#include "stdafx.h"
#include "UIlib.h"
#include "DbgEngine/ImageInfo.h"
#include "PubLib/StrTrie.h"
#include "DbgEngine/Executor.h"
#define MS_NO_COREDLL
#include "python.h"

using namespace DuiLib;

class CPubWnd : public WindowImplBase
{
protected:
	LPCTSTR GetWindowClassName() const override { return m_className; };
	CDuiString GetSkinFile() override { return m_strXMLPath; };
private:
	CDuiString m_strXMLPath;
	TCHAR* m_className;    //类名
	static CStrTrie* m_strTrie;  //字符串前缀搜索树
	static Executor* m_executor; //调试引擎
	Disasm* disasm; //反汇编数据

public:
	explicit CPubWnd(LPCTSTR pszXMLPath);
	~CPubWnd();
	void Notify(TNotifyUI& msg) override;
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled) override;
	LRESULT OnWindowPosChanging(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled) override;

	void PeReadAttribute(LPCTSTR FileName) const;
	void PeFillGeneral(CImageInfo* tmp) const;
	void PeFillDataDir(CImageInfo* tmp) const;
	void PeFillSeg(CImageInfo* tmp) const;
	void PeFillImport(CImageInfo* tmp) const;
	void PeFillExport(CImageInfo* tmp) const;
	TCHAR* GenerateClassName();
	void UpdateWindowBackground();

	HWND hWnd;

	static void HandleCommandEvt(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandleNewConsole(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandleNewWndEvt(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandleFunction(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandleCaptionMsg(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandleQuit(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandleSubWndClose(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandleBrowserFile(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandleStartDebugger(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandlePeBrowser(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandlePeSwitchImportDll(CPubWnd * pBase, TNotifyUI& msg, int Value);
	static void HandleAsmTest(CPubWnd * pBase, TNotifyUI& msg, int Value);

	//控制台线程函数
	static void ConsoleThreadFunc(PVOID param);

	//数据窗口更新函数
	void UpdateDisasmData(HANDLE hFile); //更新反汇编窗口
	void UpdateLogData();    //更新日志窗口
	void UpdateStackData();    //更新堆栈窗口
	void UpdateRegistersData();    //更新寄存器窗口
	void UpdateModulesData();    //更新模块窗口
	void UpdateThreadsData();    //更新线程窗口
};

typedef struct s_wnd_list
{
	CPubWnd* WndClass;
	struct s_wnd_list* Next;
}SWndList;

typedef int(*SEventHandle)(CPubWnd * pBase, TNotifyUI& msg, int Value);



#define MAX_STRING_LENGTH 16
typedef struct s_eventString
{
	char m_Str[MAX_STRING_LENGTH];
	int Value;
	void(*EventHandle)(CPubWnd * pBase, TNotifyUI& msg, int Value); //事件处理函数
}SEventString;
static SEventString gEventString[] =
{
	//特殊指令
	{"command"     ,0          ,CPubWnd::HandleCommandEvt},  //执行外部程序
	{"sub_wnd"     ,0          ,CPubWnd::HandleNewWndEvt},  //打开子窗口
	{"function"    ,0          ,CPubWnd::HandleFunction}, //执行内建指令
	{"py_console"    ,0          ,CPubWnd::HandleNewConsole}, //执行内建指令

	//内建指令
	{"dbg_stop"        ,DBG_STOP          ,nullptr},
	{"dbg_suspend"     ,DBG_SUSPEND          ,nullptr},
	{"dbg_restart"     ,DBG_RESTART       ,nullptr},
	{"dbg_run"         ,DBG_RUN          ,nullptr},
	{"dbg_stepinto"    ,DBG_STEPINTO          ,nullptr},
	{"dbg_stepover"    ,DBG_STEPOVER          ,nullptr},

	//标题栏按钮事件
	{"main_close"  ,0          ,CPubWnd::HandleQuit}, //程序退出
	{"main_max"    ,SC_MAXIMIZE,CPubWnd::HandleCaptionMsg}, //最大化主窗口
	{"main_restore",SC_RESTORE ,CPubWnd::HandleCaptionMsg}, //还原主窗口大小
	{"main_min"    ,SC_MINIMIZE,CPubWnd::HandleCaptionMsg}, //最小化主窗口
	//通用窗口事件
	{"close_btn"   ,0          ,CPubWnd::HandleSubWndClose}, //子窗口隐藏
	//打开文件窗口事件
	{"browser_btn" ,0          ,CPubWnd::HandleBrowserFile}, //调试前打开文件 结果会写在browser_edit 控件上
	{"start_btn" ,0          ,CPubWnd::HandleStartDebugger}, //调试前打开文件 结果会写在browser_edit 控件上
	//pe工具事件
	{"browser_pe"  ,0          ,CPubWnd::HandlePeBrowser}, //PE工具 打开文件按钮
	{"pe_impdll"   ,0          ,CPubWnd::HandlePeSwitchImportDll}, //PE工具 切换显示 dll导入函数

	//汇编测试
	{"asm_test"  ,0          ,CPubWnd::HandleAsmTest}, //PE工具 打开文件按钮
};
