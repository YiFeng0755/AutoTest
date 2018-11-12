#pragma once
#include <YouMeCommon/CrossPlatformDefine/PlatformDef.h>
#include <memory>
//关键词格式一行一个。不懂来问 一定记住UTF8 无 bom 头格式
//UTF8 无 bom 头格式
//UTF8 无 bom 头格式
//UTF8 无 bom 头格式

struct TrieNode
{
	//XCHAR character;
	bool bEnd;
	int iLevel;
	std::map<unsigned short, std::shared_ptr<TrieNode> >childs;
	TrieNode()
	{
		bEnd = false;
		iLevel = 0;
	}
};

class CKeywordFilter
{
public:
	CKeywordFilter(const XString& strReplaceTxt);
	~CKeywordFilter();

	//从文件加载关键词兼容以前的格式.一  abcdsfsxxxxx   二 abcdexxxxx 1(level)
	bool LoadFromFile(const XString& strFilePath);

	//全部转成大写
	void AddKeyword(const XString& strKeyWord, int iLevel);

	bool IsLoadSuccess()
	{
		return m_bLoadSuccess;
	}
	//返回一个最高级别的
	bool ContainKeyword(const XString& strSrc, int* pLevel);
	XString GetFilterResult(const XString& strSrc, int* pLevel);

private:
	bool InterSearchFilterResult(const XString& strSrc,XString& strResult,int * pLevel);
	std::shared_ptr<TrieNode> m_pRoot;
	bool m_bLoadSuccess;
	std::vector<unsigned short> m_strReplaceTextU16;
};

