//
//  NodeUtil.hpp
//  AutoCaller-mobile
//
//  Created by Micheal Chen on 2017/11/10.
//

#ifndef NodeUtil_hpp
#define NodeUtil_hpp

#include "cocos2d.h"

class CNodeUtils {
public:
    static float getLeftX(cocos2d::Node * node);
    static float getRightX(cocos2d::Node * node);
    static float getTopY(cocos2d::Node * node);
    static float getBottomY(cocos2d::Node * node);

    static void  log(const char * format, ...);
    static void  log(cocos2d::Label * label, const char * format, ...);

private:
    static void  _log(cocos2d::Label * label, const char * format, va_list args);
};


#endif /* NodeUtil_hpp */
