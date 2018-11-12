//
//  YMRenderTexture.h
//  YouMeVideoDemo
//
//  Created by fire on 2017/4/1.
//
//

#ifndef YMRenderTexture_h
#define YMRenderTexture_h

#include <mutex>
#include "cocos2d.h"
#include "IYouMeVideoCallback.h"
#include "IYouMeVoiceEngine.h"

class YMRenderTexture : public IYouMeVideoFrameCallback
{
public:
    YMRenderTexture();
    virtual ~YMRenderTexture();
    
private:
    typedef struct _TextureInfo { /* 纹理数据 */
        unsigned char * address;
        bool inited;
        int width;
        int height;
        int renderId;
        std::string userId;
        cocos2d::Texture2D::PixelFormat format;
		cocos2d::RenderTexture * render;
        int len;
        
        _TextureInfo() :
            address(NULL),
            width(0),
            height(0),
            renderId(0),
            format(cocos2d::Texture2D::PixelFormat::RGB888),
            render(NULL),
            len(0)
        {
            // Nothing
          
            
            
        }
        
        void checksize(int w, int h){
            if(width != w || height != h){
                inited = false;
                width = w;
                height = h;
                if (format == cocos2d::Texture2D::PixelFormat::RGB888) {
                    len = width * height * 3;
                } else if (format == cocos2d::Texture2D::PixelFormat::RGBA8888) {
                    len = width * height * 4;
                }
                if(address){
                    delete [] address;
                    address = new (std::nothrow) unsigned char[len];
                    memset((void *)address, 0x0, len);
                }
            }
        }
    } TextureInfo;

    typedef unsigned char  uint_8;
    typedef unsigned int   uint_32;

    
private:
    std::map<std::string, TextureInfo> _renders; /** 渲染源 */
    std::recursive_mutex mRenderMutex;
	IYouMeVoiceEngine* m_if = nullptr;
private:
    void yuv420p_to_rgb888(TextureInfo * _texture, const uint_8 * yplane, const uint_8 * uplane, const uint_8 * vplane, const uint_32 width, const uint_32 height);
    void yuv420p_to_rgba8888(TextureInfo * _texture, const uint_8 * yplane, const uint_8 * uplane, const uint_8 * vplane, const uint_32 width, const uint_32 height);
    
private:
    bool clean_renders(void);
    //TextureInfo getTextureInfo(int renderId) const;
    void setTextureInfo(TextureInfo texture);
public:
	void setInf(IYouMeVoiceEngine* inf);
    int create(std::string & userId, int width, int height, cocos2d::RenderTexture * texture);
    
    void removeUser( std::string& userID );
    void destory(void);

    void update();
    void frameRender(int renderId, int w, int h, int degree, int len, const void * buf);

	void onVideoFrameCallback(std::string userId, void * data, int len, int width, int height, int fmt, uint64_t timestamp) override;
	void onVideoFrameMixedCallback(void * data, int len, int width, int height, int fmt, uint64_t timestamp) override;
};


#endif /* YMRenderTexture_h */
