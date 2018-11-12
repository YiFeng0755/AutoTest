// 数据记录处理
//
////////////////////////////////////////////////////////////////////////////////////
#ifndef _RECORD_H_20091013_
#define _RECORD_H_20091013_

#include <string>
#include <YouMeCommon/NetConvert.h>

namespace youmecommon {


//记录数据读写操作（公共数据/记录）
class CRecord
{
public:
	enum {
		DEFAULT_BUFF_LEN = 4 * 1024	//默认缓冲区长度 
	};

	//构造函数
	//参数：iLen - 预分配缓冲区长度
	CRecord(unsigned int iLen = DEFAULT_BUFF_LEN) {
		if (iLen == 0) { //不预分配缓冲区
			m_pAllocBuf  = 0;
			m_pBuffBeg = 0;
			m_pBuffEnd = 0;
			m_pBuffInd = 0;
		}
		else {			//预分配缓冲区
			m_pAllocBuf  = new(std::nothrow) unsigned char[iLen];
			
			if (m_pAllocBuf != NULL)
			{
				m_pBuffBeg = m_pAllocBuf;
				m_pBuffEnd = m_pAllocBuf + iLen;
				m_pBuffInd = m_pAllocBuf;			
			}
			else
			{
				m_pAllocBuf  = 0;
				m_pBuffBeg = 0;
				m_pBuffEnd = 0;
				m_pBuffInd = 0;
			}
		}
	}

	//析构函数
	~CRecord() {
		delete []m_pAllocBuf;
	}

	//设置缓冲区（重置缓冲区，及读写数据指针复位）
	//参数：
	//	pBuff - 缓冲区指针
	//	iLen  - 缓冲区长度
	void SetBuff(unsigned char * pBuff, unsigned int iLen) {
		m_pBuffBeg = pBuff;
		m_pBuffEnd = pBuff + iLen;
		m_pBuffInd = pBuff;
	}

	// 获取设定的Buffer大小
	unsigned int GetBufSize()
	{
		return static_cast<unsigned int>(m_pBuffEnd - m_pBuffBeg);
	}
	//获取数据缓冲区指针
	const unsigned char * GetBuf() const {
		return m_pBuffBeg;
	}

	//获取缓冲数据长度（字节数）
	unsigned int GetDataLen() const {
		return static_cast<unsigned int>(m_pBuffInd - m_pBuffBeg);
	}

	//清空缓冲区指针
	void Clear() {
		m_pBuffBeg = 0;
		m_pBuffEnd = 0;
		m_pBuffInd = 0;
	}

	//读写指针复位，数据指针返回到缓冲区起始位置，之前的读写数据全部清空
	void ResetReadWrite() {
		m_pBuffInd = m_pBuffBeg;
	}

	////////////////////////////////////////////////////////////////
	//写入基本类型数据（如char, short, int, long, float, double等）
	template <typename T>
	bool SetData(const T& val) {
		if (CanAccept(val)) {
			*reinterpret_cast<T *>(m_pBuffInd) = ToNetOrderByVal(val);
			m_pBuffInd += sizeof(val);
			return true;
		}
		return false;
	}

	//写入字符串数据
	bool SetData(const char * val) {
		if (CanAccept(val)) {
			unsigned short usLen = static_cast<unsigned short>(strlen(val) + 1);	//字符串长度 + 结束符
			*reinterpret_cast<unsigned short *>(m_pBuffInd) = ToNetOrderByVal(usLen);	//字符串长度
			m_pBuffInd += sizeof(unsigned short);
			memcpy(m_pBuffInd, val, usLen);	//字符串数据
			m_pBuffInd += usLen;
			return true;
		}
		return false;
	}

	//写入buffer数据
	bool SetData(const void * val, unsigned short len) {
		if (CanAccept(val)) {
			*reinterpret_cast<unsigned short *>(m_pBuffInd) = ToNetOrderByVal((unsigned short)len);	//buffer长度
			m_pBuffInd += sizeof(unsigned short);
			memcpy(m_pBuffInd, val, len);	//buffer数据
			m_pBuffInd += len;
			return true;
		}
		return false;
	}

	////////////////////////////////////////////////////////////////
	//读取基本类型数据值（如char, short, int, long, float, double等）
	template <typename T>
	bool GetData(T& val) {
		if (m_pBuffInd + sizeof(val) <= m_pBuffEnd) {	//缓冲区未越界
			val = ::youmecommon::ToHostOrderByVal(*(reinterpret_cast<T *>(m_pBuffInd)));
			m_pBuffInd += sizeof(val);
			return true;
		}
		return false;
	}

	//读取字符串数据
	bool GetData(const char *& val) {
		unsigned short usLen = ToHostOrderByVal(*(reinterpret_cast<unsigned short *>(m_pBuffInd))); //字符串长度
		if (m_pBuffInd + sizeof(unsigned short) + usLen <= m_pBuffEnd) { //缓冲区未越界
			val = reinterpret_cast<const char *>(m_pBuffInd + sizeof(unsigned short));
			if (*(val + usLen - 1) != '\0') { //字符串进行最后一个字符是否 == '\0'的有效性检查
				val = 0;
				return false;
			}
			m_pBuffInd += (usLen + sizeof(unsigned short));
			return true;
		}
		return false;
	}

	//读取buffer数据
	bool GetData(unsigned char *& val, unsigned short& len) {
		len = ToHostOrderByVal(*(reinterpret_cast<unsigned short *>(m_pBuffInd)));	//长度
		if (m_pBuffInd + sizeof(unsigned short) + len <= m_pBuffEnd) { //缓冲区未越界
			val = reinterpret_cast<unsigned char *>(m_pBuffInd + sizeof(unsigned short));
			m_pBuffInd += (sizeof(unsigned short) + len);
			return true;
		}
		return false;
	}


private:

	//是否可接受该数据（基本类型）
	template <typename T>
	bool CanAccept(const T& v) const {
        if ((nullptr == m_pBuffInd) || (nullptr == m_pBuffEnd))
        {
            return false;
        }
		return m_pBuffInd + sizeof(v) < m_pBuffEnd;  // v
	}

	//是否可接受该数据（字符串）
	bool CanAccept(const char * v) const {
        if ((nullptr == m_pBuffInd) || (nullptr == m_pBuffEnd))
        {
            return false;
        }
		return m_pBuffInd + strlen(v) + 1 + sizeof(unsigned short) < m_pBuffEnd; // len + v
	}

	//是否可接受该数据（buffer）
	bool CanAccept(const unsigned char *, unsigned int len) const {
        if ((nullptr == m_pBuffInd) || (nullptr == m_pBuffEnd))
        {
            return false;
        }
		return m_pBuffInd + len + sizeof(unsigned short) < m_pBuffEnd; // len + v
	}


	unsigned char * m_pAllocBuf;	//分配的缓冲区（若有分配操作，则应析构时主动释放）

	unsigned char * m_pBuffBeg;	//数据缓冲区起始地址
	unsigned char * m_pBuffEnd;	//数据缓冲区结束地址(指向最后一个位置的后一位)
	unsigned char * m_pBuffInd;	//读(写)操作所指向的当前缓冲区位置（该指针之前的缓冲区已读(写)，新的读(写)操作从该指针位置开始）

};


} //end of namespace
#endif 

