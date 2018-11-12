#ifndef _TESTS_H_
#define _TESTS_H_

/**
#if CC_ENABLE_BOX2D_INTEGRATION
#include "Box2DTest/Box2dTest.h"
#include "Box2DTestBed/Box2dView.h"
#endif
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE)
#include "ChipmunkTest/ChipmunkTest.h"
#include "ClippingNodeTest/ClippingNodeTest.h"
#endif
#include "NewAudioEngineTest/NewAudioEngineTest.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_EMSCRIPEN)
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MARMALADE)
 bada don't support libcurl
#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA)
#include "CurlTest/CurlTest.h"
#endif
#endif
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "JNITest/JNITest.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
#include "WindowTest/WindowTest.h"
#endif

/
**/
#include "../talktests.hpp"
#include "../Imtest.hpp"
#include "../imautotest.hpp"
#include "settings.hpp"
#endif
