#pragma once
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
#include <memory>
#include <YouMeCommon/XSharedArray.h>
#include <YouMeCommon/SyncUDP.h>
#include <YouMeCommon/SyncTCP.h>
#include <YouMeCommon/SqliteOperator.h>
#include <YouMeCommon/XSemaphore.h>
#include <thread>
#include <queue>
struct DataChannelServer
{
	XString strIPOrDomain;//iP 地址，或者域名
	int iPort;
	bool bUseTcp;
	int iSerial; //tcp 的消息才会有这个序号，对应本地db 中的序号
	youmecommon::CXSharedArray<byte> buffer;
	DataChannelServer()
	{
		iSerial = -1;
	}
};

const XString s_szYoumeTableName[] = {
	__XT("report")
}; //一对多的配置



const XString s_szYoumeCreateTableSQL[] = {
	__XT("create table report (id int,ip text,port int, value  blob);")
};


class CYouMeDataChannel
{
public:
	//创建实例,缓存db 的全路径
	static CYouMeDataChannel* CreateInstance(const XString& strCachePath);
	//为了结束内部的线程，需要使用cleanup
	static void DestroyInstance(CYouMeDataChannel* pInstance);

	//直接上报数据,内部会有重复使用socket
	void SendData(const XString&strDomain, int iPort, bool bUseTcp, const byte* pBuffer, int iBufferLen);

private:
	CYouMeDataChannel(const XString& strCachePath);
	~CYouMeDataChannel();
	std::queue<std::shared_ptr<DataChannelServer> >m_dataCacheQueue;
	std::mutex m_mutex;
	//本地db
	youmecommon::CSqliteDb m_sqliteDb;
	//是否已经加载本地数据库
	bool m_bInit;
	youmecommon::CXSemaphore m_wait;
	int m_iMaxID;
	//防止多次域名解析，把解析的结果存放在一个表中
	std::map<XString, XString> m_domainIPMap;
	//上报线程句柄以及上报函数
	std::thread m_reportThread;
	void ReportProc();
	//是否退出 上报
	bool m_bExitReport;
	//缓存的db 全路径
	XString m_strCacheDBPath;
};

