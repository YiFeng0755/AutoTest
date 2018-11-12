#pragma once
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
#include <YouMeCommon/XFile.h>
namespace youmecommon
{
	struct WAVHeadInfo
	{
		// RIFF chunk
		char riffID[4];					//"RIFF"
		unsigned int fileSize;
		char riffFormat[4];				//"WAVE"
		// format chunk
		char fmtID[4];					//"fmt "
		unsigned int fmtSize;
		unsigned short formatTag;		//编码方式
		unsigned short channels;		//声道数
		unsigned int sampleFrequency;	//采样频率
		unsigned int byteRate;			//比特率
		unsigned short blockAlign;		//块对齐大小
		unsigned short sampleBitSize;	//样本宽度
		// data chunk
		char dataID[4];					//"data"
		unsigned int dataChunkSize;
	};
	class CPCMResampleTo8K
	{
	public:

		//返回是否已经转换，如果传入的本来就是8K 的，不需要转换，返回false
		static bool ConvertTo8K(const XString& strSrcPath, const XString& strDestPath);
	private:
		static bool PcmToWav(const XString& srcPath, unsigned short sampleBitSize, unsigned short channels, unsigned long sampleFrequency, int format, const XString desPath);
		static int ReadPCMFrame(short speech[], youmecommon::CXFile& fpwave, int nChannels, int nBitsPerSample);
	};

}