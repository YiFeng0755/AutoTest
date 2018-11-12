#pragma once
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
namespace youmecommon {

class CTimeUtil
{
public:
	CTimeUtil();
	~CTimeUtil();

	static XINT64 GetTimeOfDay_MS();
	static tm * GetLocalTime(const time_t* timep, tm * result);
};

}