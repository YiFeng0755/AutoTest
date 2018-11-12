#pragma once
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
#include <YouMeCommon/XSemaphore.h>
#include <mutex>
//给测试工具使用
class CNetworkTest
{
public:
	CNetworkTest(XString strIP,int iPort,bool bSyncMode);
	void UnInit();
	~CNetworkTest();

	//开始检查，外部需要启动线程来检查
	void StartCheck();
	//获取输出，需要启动线程来获取
	XString GetOutPut();
private:
	void InsertMsg(const XString& strMsg);
	XString m_strIP;
	int m_iPort;
	youmecommon::CXSemaphore* m_sem = NULL;
	std::list<XString> m_strMsgLists;
	std::mutex* m_mutex=nullptr;
	bool m_bSyncMode;
	bool m_bExit = false;
};

