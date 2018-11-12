#include "NodeUtils.hpp"

USING_NS_CC;

float CNodeUtils::getLeftX(Node * node)
{
    return node->getPositionX() - node->getContentSize().width / 2;
}


float CNodeUtils::getRightX(Node * node)
{
    return node->getPositionX() + node->getContentSize().width / 2;
}


float CNodeUtils::getTopY(Node * node)
{
    return node->getPositionY() + node->getContentSize().height / 2;
}


float CNodeUtils::getBottomY(Node * node)
{
    return node->getPositionY() - node->getContentSize().height / 2;
}


void  CNodeUtils::log(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    _log(NULL, format, args);
    va_end(args);
}

void  CNodeUtils::log(cocos2d::Label * label, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    _log(label, format, args);
    va_end(args);
}

void  CNodeUtils::_log(cocos2d::Label * label, const char * format, va_list args)
{
    int bufferSize = MAX_LOG_LENGTH;
    char * buf = nullptr;

    do
    {
        buf = new (std::nothrow) char[bufferSize];
        if (buf == nullptr)
            return; // not enough memory

        int ret = vsnprintf(buf, bufferSize - 3, format, args);
        if (ret < 0)
        {
            bufferSize *= 2;

            delete [] buf;
        }
        else
            break;

    } while (true);

    strcat(buf, "\n");

    cocos2d::log("%s", buf);

    if (label != NULL) {
        float oldwidth = label->getContentSize().width;
        label->setString(buf);
        float newwidth = label->getContentSize().width;
        label->setPositionX(label->getContentSize().width / 2 + 30);
    }

    delete [] buf;
    va_end(args);
}

