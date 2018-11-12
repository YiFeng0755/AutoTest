#pragma once
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
class CSoundTouchConfig
{
public:
	CSoundTouchConfig();
	~CSoundTouchConfig();

	bool LoadConfi(const XString& strConfigPath);
	bool SaveConfig(const XString& strConfigPath);

	/*
	int iTempo = m_tempo.GetPos();
	soundTouch.setTempoChange(iTempo);

	int iTones = m_tones.GetPos();
	soundTouch.setPitchSemiTones(iTones);

	int iRateChang = m_rateChange.GetPos();
	soundTouch.setRateChange(iRateChang);
	*/
	int m_iTempo=0;
	int m_iTones=0;
	int m_iRateChang=0;

};

