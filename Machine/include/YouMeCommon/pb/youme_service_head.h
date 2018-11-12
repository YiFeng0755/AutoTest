/*
 * youme_service_head.h
 *
 *  Created on: 20160912
 *      Author: mark
 */

#ifndef YOUME_SERVICE_HEAD_H_
#define YOUME_SERVICE_HEAD_H_

#include <string>
#include <sstream>

/**
 * In stdint.h, uint8, uint16,... and int8, int32... are defined.
 */
#include <stdint.h>
/**
 * defined size_t and ssize_t
 */
//#include <unistd.h>
#ifdef WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

#define RETURN_MINUS_ONE_IF_NULL(pointer) {if(pointer == NULL) return -1;}
#define RETURN_FALSE_IF_NULL(pointer) {if(pointer == NULL) return false;}

namespace NSYOUMEComm 
{
inline uint64_t youme_ntohll(uint64_t  v)
{
	//本地序和网络序不一样
	if (ntohl(1) != 1)
	{
		uint64_t tempT = v;

		uint32_t * p = (uint32_t *) (&v);
		uint32_t *q = (uint32_t *) (&tempT);
		*p = ntohl(*(q + 1));
		*(p + 1) = ntohl(*q);
	}
	return v;
}

inline uint64_t youme_htonll(uint64_t v)
{
	return youme_ntohll(v);
}
}

#pragma  pack(1)

struct SYoumeConnHead
{
	uint64_t m_ullReqSeq;
	uint16_t m_usLen;
	uint16_t m_usCmd;
	uint32_t m_uiServiceId;

	SYoumeConnHead()
	: m_ullReqSeq(0)
	, m_usLen(sizeof(SYoumeConnHead))
	, m_usCmd(0)
	, m_uiServiceId(0)
	{
	}
    
	std::string ToString() const
    {
        std::ostringstream oss;

		oss << "Len: " << m_usLen << ", cmd: " << m_usCmd << ", ReqSeq: " << m_ullReqSeq
			<< ", serviceID: " << m_uiServiceId;

        return oss.str();
    }

    /***********打包解包模板函数接口开始**************************/
    /**
     * 返回包头的长度，缓冲区是网络序
     */
    static size_t GetHeadLength(const uint8_t* pData, size_t uiLen)
    {
        RETURN_MINUS_ONE_IF_NULL(pData);
        return sizeof(SYoumeConnHead);
    }

    /**
     * 返回整个包的长度,缓冲区是网络序。
     * @param pData 缓冲区首地址
     * @param uiLen 缓冲区长度，至少等于GetHeadLength()返回的长度
     */
    static size_t GetTotalLength(const uint8_t* pData, size_t uiLen)
    {
        if (pData == NULL || uiLen < sizeof(SYoumeConnHead))
        {
            return 0;
        }
        return ntohs(((SYoumeConnHead*) pData)->m_usLen);
    }

    /**
     * 设置整个包的长度，应设置为网络序
     * @param pData 缓冲区首地址
     * @uiTotalLen 缓冲区总长度，包括打包后的包体和包头
     */
    static void SetTotalLength(uint8_t* pData, size_t uiTotalLen)
    {
        if (pData == NULL || uiTotalLen < sizeof(SYoumeConnHead))
        {
            return;
        }

        ((SYoumeConnHead*) pData)->m_usLen = htons(uiTotalLen);
    }

    static bool IsEncrypted()
    {
        return false;
    }

    const uint8_t* GetKey() const
    {
        return NULL;
    }

    /**
     * 打包函数
     * @param pData 缓冲区首地址
     * @param uiBufferLen缓冲区总长度，如打包成功，应返回包头长度
     * @return 打包是否成功
     */
    bool Pack(uint8_t* pData, size_t& uiBufferLen) const
    {
        if (pData == NULL || uiBufferLen < sizeof(SYoumeConnHead))
        {
            return false;
        }

        memcpy(pData, &m_ullReqSeq, sizeof(SYoumeConnHead));
        ((SYoumeConnHead*)(pData))->ToNetOrder();
        uiBufferLen = sizeof(SYoumeConnHead);
        return true;
    }

    /**
     * 解包函数，包头长度应为GetHeadLength()返回的长度
     */
    bool Unpack(const uint8_t* pData)
    {
        RETURN_FALSE_IF_NULL(pData);

        memcpy(&m_ullReqSeq, pData, sizeof(SYoumeConnHead));
        ToHostOrder();
        return true;
    }

    /***********打包解包模板函数接口结束**************************/
public:
    void ToNetOrder()
    {
		m_ullReqSeq = NSYOUMEComm::youme_htonll(m_ullReqSeq);
        m_usLen = htons(m_usLen);
		m_usCmd = htons(m_usCmd);
		m_uiServiceId = htonl(m_uiServiceId);
    }

    void ToHostOrder()
    {
		m_ullReqSeq = NSYOUMEComm::youme_ntohll(m_ullReqSeq);
		m_usLen = ntohs(m_usLen);
		m_usCmd = ntohs(m_usCmd);
		m_uiServiceId = ntohl(m_uiServiceId);	
    }

};

struct RelayHead
{
	uint16_t usLen;
	int32_t iSocket;//for tcp client
	uint32_t uiIp;//for udp client
	uint16_t usPort;//for udp client
	uint32_t uiTime;
	uint32_t uiSeq;
	RelayHead()
	{
		usLen = sizeof(RelayHead);
		uiIp = 0;
		usPort = 0;
		iSocket = 0;
	}

	void ToNetOrder()
	{
		usLen = htons(usLen);
		uiIp = htonl(uiIp);
		usPort = htons(usPort);
		iSocket = htonl(iSocket);
		uiTime = htonl(uiTime);
		uiSeq = htonl(uiSeq);
	}

	void ToHostOrder()
	{
		usLen = ntohs(usLen);
		uiIp = ntohl(uiIp);
		usPort = ntohs(usPort);
		iSocket = ntohl(iSocket);
		uiTime = ntohl(uiTime);
		uiSeq = ntohl(uiSeq);
	}

	std::string ToString() const
	{
		std::ostringstream oss;

		oss << "RelayHead : " << std::endl << "     " << "usLen = " << usLen
			<< std::endl << "       " << "uiIp = " << uiIp << std::endl << "        "
			<< "usPort = " << usPort << std::endl << "      " << "iSocket = "
			<< iSocket << std::endl << "        " << "uiTime = " << uiTime
			<< std::endl << "       " << "uiSeq = " << uiSeq << std::endl;

		return oss.str();
	}
};

struct SYoumeServiceHead
{
    RelayHead m_oRelayHead;
    SYoumeConnHead m_oConnHead;

    std::string ToString() const
    {
        std::ostringstream oss;

        oss << m_oRelayHead.ToString() << m_oConnHead.ToString();

        return oss.str();
    }

    /***********打包解包模板函数接口开始**************************/
    /**
     * 返回包头的长度，缓冲区是网络序
     */
    static size_t GetHeadLength(const uint8_t* pData, size_t uiLen)
    {
        RETURN_MINUS_ONE_IF_NULL(pData);
        return sizeof(SYoumeServiceHead);
    }

    /**
     * 返回整个包的长度,缓冲区是网络序。
     * @param pData 缓冲区首地址
     * @param uiLen 缓冲区长度，至少等于GetHeadLength()返回的长度
     */
    static size_t GetTotalLength(const uint8_t* pData, size_t uiLen)
    {
        if (pData == NULL || uiLen < sizeof(SYoumeServiceHead))
        {
            return 0;
        }
        return ntohs(((SYoumeServiceHead*) pData)->m_oRelayHead.usLen);
    }

    /**
     * 设置整个包的长度，应设置为网络序
     * @param pData 缓冲区首地址
     * @uiTotalLen 缓冲区总长度，包括打包后的包体和包头
     */
    static void SetTotalLength(uint8_t* pData, size_t uiTotalLen)
    {
        if (pData == NULL || uiTotalLen < sizeof(SYoumeServiceHead))
        {
            return;
        }

		((SYoumeServiceHead*) pData)->m_oRelayHead.usLen = htons(uiTotalLen);
        ((SYoumeServiceHead*) pData)->m_oConnHead.m_usLen = htons(uiTotalLen - sizeof(RelayHead));
    }

    static bool IsEncrypted()
    {
        return false;
    }

    const uint8_t* GetKey() const
    {
        return NULL;
    }

    /**
     * 打包函数
     * @param pData 缓冲区首地址
     * @param uiBufferLen缓冲区总长度，如打包成功，应返回包头长度
     * @return 打包是否成功
     */
    bool Pack(uint8_t* pData, size_t& uiBufferLen) const
    {
        if (pData == NULL || uiBufferLen < sizeof(SYoumeServiceHead))
        {
            return false;
        }

        memcpy(pData, &m_oRelayHead, sizeof(SYoumeServiceHead));
        ((SYoumeServiceHead*)(pData))->ToNetOrder();
        uiBufferLen = sizeof(SYoumeServiceHead);
        return true;
    }

    /**
     * 解包函数，包头长度应为GetHeadLength()返回的长度
     */
    bool Unpack(const uint8_t* pData)
    {
        RETURN_FALSE_IF_NULL(pData);

        memcpy(&m_oRelayHead, pData, sizeof(SYoumeServiceHead));
        ToHostOrder();
        return true;
    }

    /***********打包解包模板函数接口结束**************************/
public:
    void ToNetOrder()
    {
        m_oRelayHead.ToNetOrder();
        m_oConnHead.ToNetOrder();
    }

    void ToHostOrder()
    {
        m_oRelayHead.ToHostOrder();
        m_oConnHead.ToHostOrder();
    }

};

#pragma pack()

#endif // end for YOUME_SERVICE_HEAD_H_
