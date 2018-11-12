#ifndef _NETFUND_CONVERT_H_20100313
#define _NETFUND_CONVERT_H_20100313
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
#ifdef WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif //_LINUX_OS_

#ifdef WIN32
#define H2N64(x) ((((x) & 0x00000000000000ff) << 56) | \
	(((x) & 0x000000000000ff00) << 40) | \
	(((x) & 0x0000000000ff0000) << 24) | \
	(((x) & 0x00000000ff000000) << 8) | \
	(((x) & 0x000000ff00000000) >> 8) | \
	(((x) & 0x0000ff0000000000) >> 24) | \
	(((x) & 0x00ff000000000000) >> 40) | \
	(((x) & 0xff00000000000000) >> 56))
#else
#define H2N64(x) ((((x) & 0x00000000000000ffLL) << 56) | \
	(((x) & 0x000000000000ff00LL) << 40) | \
	(((x) & 0x0000000000ff0000LL) << 24) | \
	(((x) & 0x00000000ff000000LL) << 8) | \
	(((x) & 0x000000ff00000000LL) >> 8) | \
	(((x) & 0x0000ff0000000000LL) >> 24) | \
	(((x) & 0x00ff000000000000LL) >> 40) | \
	(((x) & 0xff00000000000000LL) >> 56))
#endif
#define N2H64 H2N64

	
namespace youmecommon
{
	template <typename T>
	inline void ToHostOrder(T& v)
	{
		v.ToHostOrder();
	}

	template <>
	inline void ToHostOrder<char>(char&)
	{
		return;
	}

	template <>
	inline void ToHostOrder<unsigned char>(unsigned char&)
	{
		return;
	}

	template <>
	inline void ToHostOrder<short>(short& v)
	{
		v = ntohs(v);
	}

	template <>
	inline void ToHostOrder<unsigned short>(unsigned short& v)
	{
		v = ntohs(v);
	}

	template <>
	inline void ToHostOrder<int>(int& v)
	{
		v = ntohl(v);
	}

	template <>
	inline void ToHostOrder<unsigned int>(unsigned int& v)
	{
		v = ntohl(v);
	}

	template <>
	inline void ToHostOrder<long>(long& v)
	{
		v = ntohl(v);
	}

	template <>
	inline void ToHostOrder<unsigned long>(unsigned long& v)
	{
		v = ntohl(v);
	}

	template <>
	inline void ToHostOrder<XINT64>(XINT64& v)
	{
		v = N2H64(v);
	}

	template <>
	inline void ToHostOrder<XUINT64>(XUINT64& v)
	{
		v = N2H64(v);
	}


	template <typename T>
	inline void ToNetOrder(T& v)
	{
		v.ToNetOrder();
	}

	template <>
	inline void ToNetOrder<char>(char&)
	{
		return;
	}

	template <>
	inline void ToNetOrder<unsigned char>(unsigned char&)
	{
		return;
	}

	template <>
	inline void ToNetOrder<short>(short& v)
	{
		v = htons(v);
	}

	template <>
	inline void ToNetOrder<unsigned short>(unsigned short& v)
	{
		v = htons(v);
	}

	template <>
	inline void ToNetOrder<int>(int& v)
	{
		v = htonl(v);
	}

	template <>
	inline void ToNetOrder<unsigned int>(unsigned int& v)
	{
		v = htonl(v);
	}

	template <>
	inline void ToNetOrder<long>(long& v)
	{
		v = htonl(v);
	}

	template <>
	inline void ToNetOrder<unsigned long>(unsigned long& v)
	{
		v = htonl(v);
	}

	template <>
	inline void ToNetOrder<XINT64>(XINT64& v)
	{
		v = H2N64(v);
	}

	template <>
	inline void ToNetOrder<XUINT64>(XUINT64& v)
	{
		v = H2N64(v);
	}

	template <typename T>
	inline T ToHostOrderByVal(T v)
	{
		ToHostOrder(v);
		return v;
	}

	template <typename T>
	inline T ToNetOrderByVal(T v)
	{
		ToNetOrder(v);
		return v;
	}


};
#endif
