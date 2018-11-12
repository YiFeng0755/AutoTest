//
//  YMRenderTexture.cpp
//  YouMeVideoDemo
//
//  Created by fire on 2017/4/1.
//
//

#include "YMRenderTexture.h"
#include "IYouMeVoiceEngine.h"

#include <memory>

USING_NS_CC;


#define CLAMP(i)   (((i) < 0) ? 0 : (((i) > 255) ? 255 : (i)))

YMRenderTexture::YMRenderTexture()
{
    // TODO
}

YMRenderTexture::~YMRenderTexture()
{
    // TODO
	destory();
}

bool YMRenderTexture::clean_renders(void)
{
     std::lock_guard<std::recursive_mutex> lock(mRenderMutex);
	for (auto iter = _renders.begin(); iter != _renders.end(); iter++) {
		TextureInfo _texture = iter->second;
        
        if(_texture.renderId >=0 && m_if){
            m_if->deleteRender(iter->second.renderId);
        }
        
		if (_texture.address != NULL) {
			delete[] _texture.address;
		}

		if (_texture.render != NULL) {
            iter->second.render->getSprite()->getTexture()->releaseGLTexture();
			//_texture.render->end();
		}
	}

	_renders.clear(); /* clear */
    return true;
}

void YMRenderTexture::removeUser( std::string& userID ){
    std::lock_guard<std::recursive_mutex> lock(mRenderMutex);
    for (auto it = _renders.begin(); it != _renders.end(); ++it) {
        if(it->second.userId == userID){
            
            if(it->second.renderId >=0 && m_if){
                m_if->deleteRender(it->second.renderId);
            }
            
            if (it->second.address != NULL) {
                delete [] it->second.address;
            }
            
            if (it->second.render != NULL) {
                it->second.render->getSprite()->getTexture()->releaseGLTexture();
                //it->second.render->end();
            }
            
            _renders.erase( it );
            
            break;
        }
    }
}


void YMRenderTexture::setInf(IYouMeVoiceEngine* inf)
{
	m_if = inf;
	if (m_if){
		m_if->setVideoCallback(this);
	}
}

int YMRenderTexture::create(std::string & userId, int width, int height, RenderTexture * texture)
{
	if (!m_if){
		return -100;
	}

	m_if->setVideoCallback(this);

    Texture2D::PixelFormat format;
    
    if (texture == NULL) {
        return -1;
    }
    
    format = texture->getSprite()->getTexture()->getPixelFormat();
    
    if (format != Texture2D::PixelFormat::RGB888 && format != Texture2D::PixelFormat::RGBA8888) {
        return -2;
    }
    
    TextureInfo _texture;

    _texture.width = width;
    _texture.height = height;
    _texture.format = format;
    _texture.renderId = -1;
    _texture.userId = userId;
    _texture.inited = false;
    
    if (format == Texture2D::PixelFormat::RGB888) {
        _texture.len = width * height * 3;
    } else if (format == Texture2D::PixelFormat::RGBA8888) {
        _texture.len = width * height * 4;
    }
    
    _texture.address = new (std::nothrow) unsigned char[_texture.len];
    
    if (_texture.address == NULL) {
        return -3;
    }
    
    memset((void *)_texture.address, 0x0, _texture.len);
    
    _texture.render = texture;
    
    int renderId = m_if->createRender(userId.c_str());
    
    _texture.renderId = renderId;
	_texture.userId = userId;

    cocos2d::log("renderId: %d width=%d height=%d", renderId, width, height);
    
    std::lock_guard<std::recursive_mutex> lock(mRenderMutex);
	auto iter = _renders.find(userId);
    if (iter == _renders.end()) {
		_renders.insert(std::map<std::string, TextureInfo>::value_type(userId, _texture)); /* new */
    } else {
        if (iter->second.address != NULL) {
            delete [] iter->second.address;
        }
        
        iter->second = _texture; /* update */
    }
    
    return renderId;
}


void YMRenderTexture::destory(void)
{
	clean_renders();
    
	if (m_if){
		m_if->setVideoCallback(nullptr);
	}
}

/*
YMRenderTexture::TextureInfo YMRenderTexture::getTextureInfo(int renderId) const
{
 
    return _renders.at(renderId);
}*/

void YMRenderTexture::setTextureInfo(YMRenderTexture::TextureInfo texture)
{
    std::lock_guard<std::recursive_mutex> lock(mRenderMutex);
	_renders.insert(std::map<std::string, TextureInfo>::value_type(texture.userId, texture));
}

void YMRenderTexture::update()
{
    // cocos2d::log(" >>>>>>>>>>>>>>>>>>>>>>>>>>> fps = %d ", static_cast<int>(delta * 1000));
//    std::lock_guard<std::recursive_mutex> stateLock(mRenderMutex);
//    for (std::map<int, TextureInfo>::iterator iter = _renders.begin(); iter != _renders.end(); iter++) {
//        TextureInfo* _texture = &(iter->second);
//        Texture2D * texture = _texture->render->getSprite()->getTexture();
//        if(!_texture->inited){
//            _texture->inited = true;
//            texture->initWithData((const void *)_texture->address, _texture->len, _texture->format, _texture->width, _texture->height, texture->getContentSize());
//        } else{
//            texture->updateWithData((const void *)_texture->address, 0, 0, _texture->width, _texture->height);
//        }
//    }
}

void YMRenderTexture::frameRender(int renderId, int w, int h, int degree, int len, const void * buf)
{

}

void YMRenderTexture::onVideoFrameCallback(std::string userId, void * data, int len, int width, int height, int fmt, uint64_t timestamp)
{
	std::lock_guard<std::recursive_mutex> lock(mRenderMutex);
	auto iter = _renders.find(userId);
	int w = width;
	int h = height;
	void* buf = data;

	if (iter != _renders.end()) {
		TextureInfo* _texture = &(iter->second);
		_texture->checksize(w, h);
		if (_texture->format == Texture2D::PixelFormat::RGB888) {
			yuv420p_to_rgb888(_texture, (const uint_8 *)buf, (const uint_8 *)buf + (w * h), (const uint_8 *)buf + (w * h * 5 / 4), w, h);
		}
		else if (_texture->format == Texture2D::PixelFormat::RGBA8888) {
			yuv420p_to_rgba8888(_texture, (const uint_8 *)buf, (const uint_8 *)buf + (w * h), (const uint_8 *)buf + (w * h * 5 / 4), w, h);
		}
		else {
			// Not support yet.
		}
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
			std::lock_guard<std::recursive_mutex> lock(mRenderMutex);
			auto iterTmp = _renders.find(userId);
			if (_renders.end() != iterTmp) {
				Texture2D * texture = iterTmp->second.render->getSprite()->getTexture();
				if (!_texture->inited){
					_texture->inited = true;
					texture->initWithData((const void *)_texture->address, _texture->len, _texture->format, _texture->width, _texture->height, texture->getContentSize());
				}
				else{
					texture->updateWithData((const void *)_texture->address, 0, 0, _texture->width, _texture->height);
				}
			}

		});
	}
	else {
		// Render not found.
	}
}

void YMRenderTexture::onVideoFrameMixedCallback(void * data, int len, int width, int height, int fmt, uint64_t timestamp)
{
	std::lock_guard<std::recursive_mutex> lock(mRenderMutex);
	auto iter = _renders.begin();
	int w = width;
	int h = height;
	void* buf = data;

	if (iter != _renders.end()) {
		TextureInfo* _texture = &(iter->second);
		_texture->checksize(w, h);
		if (_texture->format == Texture2D::PixelFormat::RGB888) {
			yuv420p_to_rgb888(_texture, (const uint_8 *)buf, (const uint_8 *)buf + (w * h), (const uint_8 *)buf + (w * h * 5 / 4), w, h);
		}
		else if (_texture->format == Texture2D::PixelFormat::RGBA8888) {
			yuv420p_to_rgba8888(_texture, (const uint_8 *)buf, (const uint_8 *)buf + (w * h), (const uint_8 *)buf + (w * h * 5 / 4), w, h);
		}
		else {
			// Not support yet.
		}
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
			std::lock_guard<std::recursive_mutex> lock(mRenderMutex);
			auto iterTmp = _renders.begin();
			if (_renders.end() != iterTmp) {
				Texture2D * texture = iterTmp->second.render->getSprite()->getTexture();
				if (!_texture->inited){
					_texture->inited = true;
					texture->initWithData((const void *)_texture->address, _texture->len, _texture->format, _texture->width, _texture->height, texture->getContentSize());
				}
				else{
					texture->updateWithData((const void *)_texture->address, 0, 0, _texture->width, _texture->height);
				}
			}

		});
	}
	else {
		// Render not found.
	}
}

/***
 * yuv420p to rgb888
 */
void YMRenderTexture::yuv420p_to_rgb888(TextureInfo * _texture, const uint_8 * yplane, const uint_8 * uplane, const uint_8 * vplane, const uint_32 width, const uint_32 height)
{
    uint_8 * ptr = _texture->address;
    int r, g, b;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint_8 yp = yplane[(y * width) + x];
            uint_8 up = uplane[((y / 2) * (width / 2)) + (x / 2)];
            uint_8 vp = vplane[((y / 2) * (width / 2)) + (x / 2)];
            
            r = ((1164 * (yp - 16) + 1596 * (vp - 128)                    ) / 1000);
            g = ((1164 * (yp - 16) -  813 * (vp - 128) -  391 * (up - 128)) / 1000);
            b = ((1164 * (yp - 16)                     + 2018 * (up - 128)) / 1000);
            
            *ptr++ = CLAMP(r);
            *ptr++ = CLAMP(g);
            *ptr++ = CLAMP(b);
        }
    }
}

/***
 * yuv420p to rgba8888 
 */
void YMRenderTexture::yuv420p_to_rgba8888(TextureInfo * _texture, const uint_8 * yplane, const uint_8 * uplane, const uint_8 * vplane, const uint_32 width, const uint_32 height)
{
    uint_8 * ptr = _texture->address;
    int r, g, b;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint_8 yp = yplane[(y * width) + x];
            uint_8 up = uplane[((y / 2) * (width / 2)) + (x / 2)];
            uint_8 vp = vplane[((y / 2) * (width / 2)) + (x / 2)];
            
            r = ((1164 * (yp - 16) + 1596 * (vp - 128)                    ) / 1000);
            g = ((1164 * (yp - 16) -  813 * (vp - 128) -  391 * (up - 128)) / 1000);
            b = ((1164 * (yp - 16)                     + 2018 * (up - 128)) / 1000);
            
            *ptr++ = CLAMP(r);
            *ptr++ = CLAMP(g);
            *ptr++ = CLAMP(b);
            *ptr++ = CLAMP(0);
        }
    }
}
