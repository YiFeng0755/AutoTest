//
//  talkservice.cpp
//  AutoCaller
//
//  Created by Micheal Chen on 2017/7/17.
//
//

#include "talkservice.hpp"

TalkService* TalkService::getInstance()
{
    static TalkService inst;
    return &inst;
}

void TalkService::initialize()
{
}

TalkService::TalkService()
{
}
TalkService::~TalkService()
{

}
