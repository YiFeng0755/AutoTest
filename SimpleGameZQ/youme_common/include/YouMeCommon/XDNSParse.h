#pragma once
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
#include <string>
#include <vector>
#include <XCondWait.h>
//同步的DNS 解析
namespace youmecommon {

class CXDNSParse
{
public:
	//超时单位 毫秒
	static bool ParseDomain2(const XString& strDomain, std::vector<XString>& ipList, int iTimeOut = 2000);
	static bool IsIPAddress(const XString& strValue);
};
}