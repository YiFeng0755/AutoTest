#include "YouMeTalk.h"

#include "cocos2d.h"

USING_NS_CC;

#define CLAMP(i)   (((i) < 0) ? 0 : (((i) > 255) ? 255 : (i)))

static YouMeTalk* _instance = NULL;

YouMeTalk * YouMeTalk::getInstance()
{
	if (!_instance) {
		_instance = new YouMeTalk();
	}
	return _instance;
}

void YouMeTalk::destroy()
{
	if (_instance){
		delete _instance;
		_instance = nullptr;
	}
}

YouMeTalk::YouMeTalk()
{
}


YouMeTalk::~YouMeTalk()
{
}

int YouMeTalk::init(const char* pAPPKey, const char* pAPPSecret, int serverRegionId, const char* pExtServerRegionName)
{
	return  (YouMeErrorCode)(getInf()->init(m_eventcb, pAPPKey, pAPPSecret, (YOUME_RTC_SERVER_REGION)serverRegionId, pExtServerRegionName));
}

int YouMeTalk::unInit()
{
	return  (YouMeErrorCode)(getInf()->unInit());
}

void YouMeTalk::setServerRegion(int regionId, const char* extRegionName, bool bAppend)
{
	getInf()->setServerRegion((YOUME_RTC_SERVER_REGION)regionId, extRegionName, bAppend);
}

void YouMeTalk::setSpeakerMute(bool bOn)
{
	getInf()->setSpeakerMute(bOn);
}

bool YouMeTalk::getSpeakerMute()
{
	return getInf()->getSpeakerMute();
}

bool YouMeTalk::getMicrophoneMute()
{
	return getInf()->getMicrophoneMute();
}

void YouMeTalk::setMicrophoneMute(bool mute)
{
	getInf()->setMicrophoneMute(mute);
}

unsigned int YouMeTalk::getVolume()
{
	return getInf()->getVolume();
}

void YouMeTalk::setVolume(const unsigned int &uiVolume)
{
	getInf()->setVolume(uiVolume);
}

bool YouMeTalk::getUseMobileNetworkEnabled()
{
	return getInf()->getUseMobileNetworkEnabled();
}

void YouMeTalk::setUseMobileNetworkEnabled(bool bEnabled)
{
	getInf()->setUseMobileNetworkEnabled(bEnabled);
}

int YouMeTalk::joinChannelSingleMode(const char* pUserID, const char* pChannelID, int eUserRole)
{
	return  (YouMeErrorCode)(getInf()->joinChannelSingleMode(pUserID, pChannelID, (YouMeUserRole_t)eUserRole));
}

int YouMeTalk::joinChannelMultiMode(const char* pUserID, const char* pChannelID, int eUserRole)
{
    return (YouMeErrorCode)(getInf()->joinChannelMultiMode(pUserID, pChannelID, (YouMeUserRole_t)eUserRole));
}

int YouMeTalk::speakToChannel(const char* pChannelID)
{
	return (YouMeErrorCode)(getInf()->speakToChannel(pChannelID));
}

int YouMeTalk::leaveChannelMultiMode(const char* pChannelID)
{
	return (YouMeErrorCode)(getInf()->leaveChannelMultiMode(pChannelID));
}

int YouMeTalk::leaveChannelAll()
{
	return (YouMeErrorCode)(getInf()->leaveChannelAll());
}

int YouMeTalk::avoidOtherVoiceStatus(const char* pUserID, bool status)
{
	return (YouMeErrorCode)(getInf()->setListenOtherVoice(pUserID, status));
}

int YouMeTalk::setListenOtherVoice (const char* pUserID, bool on )
{
	return (YouMeErrorCode)(getInf()->setListenOtherVoice(pUserID, on));
}

int YouMeTalk::playBackgroundMusic(const char* pFilePath, bool bRepeat)
{
	return (YouMeErrorCode)(getInf()->playBackgroundMusic(pFilePath, bRepeat));
}

int YouMeTalk::stopBackgroundMusic()
{
	return (YouMeErrorCode)(getInf()->stopBackgroundMusic());
}

int YouMeTalk::setBackgroundMusicVolume(int vol)
{
	return (YouMeErrorCode)(getInf()->setBackgroundMusicVolume(vol));
}

int YouMeTalk::setHeadsetMonitorOn(bool enabled)
{
	return (YouMeErrorCode)(getInf()->setHeadsetMonitorOn(enabled));
}

int YouMeTalk::setReverbEnabled(bool enabled)
{
	return (YouMeErrorCode)(getInf()->setHeadsetMonitorOn(enabled));
}

int YouMeTalk::pauseChannel()
{
	return (YouMeErrorCode)(getInf()->pauseChannel());
}

int YouMeTalk::resumeChannel()
{
	return (YouMeErrorCode)(getInf()->resumeChannel());
}

int YouMeTalk::getSDKVersion()
{
	return getInf()->getSDKVersion();
}

int YouMeTalk::openVideoEncoder(const char* pFilePath)
{
	return (YouMeErrorCode)(getInf()->openVideoEncoder(pFilePath));
}

int YouMeTalk::createRender(const char * userId)
{
	return (YouMeErrorCode)(getInf()->createRender(userId));
}

int YouMeTalk::deleteRender(int renderId)
{
	return (YouMeErrorCode)(getInf()->deleteRender(renderId));
}

int YouMeTalk::startCapture()
{
	return (YouMeErrorCode)(getInf()->startCapture());
}

int YouMeTalk::stopCapture()
{
	return (YouMeErrorCode)(getInf()->stopCapture());
}

int YouMeTalk::setCaptureProperty(int nFps, int nWidth, int nHeight)
{
	YouMeErrorCode n = (YouMeErrorCode)(getInf()->setVideoLocalResolution(nWidth, nHeight));
	if(n != YOUME_SUCCESS)
		return n;
	return (YouMeErrorCode)(getInf()->setVideoFps(nFps));
}

int YouMeTalk::setNetCaptureProperty(int nWidth, int nHeight)
{
    return (YouMeErrorCode)(getInf()->setVideoNetResolution(nWidth, nHeight));
}

int YouMeTalk::setCaptureFrontCameraEnable(bool enable)
{
	return (YouMeErrorCode)(getInf()->setCaptureFrontCameraEnable(enable));
}

int YouMeTalk::switchCamera()
{
	return (YouMeErrorCode)(getInf()->switchCamera());
}

int YouMeTalk::resetCamera()
{
	return (YouMeErrorCode)(getInf()->resetCamera());
}

int YouMeTalk::maskVideoByUserId(const char * userId, int mask)
{
    return (YouMeErrorCode)(getInf()->maskVideoByUserId(userId, mask));
}

int YouMeTalk::bindTexture(std::string & userId, int width, int height, cocos2d::RenderTexture * texture)
{
	return m_texmgr.create(userId, width, height, texture);
}

void YouMeTalk::unbindTexture(std::string& userId)
{
	m_texmgr.removeUser(userId);
}

void YouMeTalk::updateTextures()
{
	m_texmgr.update();
}

void YouMeTalk::cleanTextures()
{
	m_texmgr.destory();
}

IYouMeVoiceEngine* YouMeTalk::getInf()
{
	if (!m_if){
		m_if = IYouMeVoiceEngine::getInstance();
		m_if->setVideoHardwareCodeEnable(false);
		m_texmgr.setInf(m_if);
		m_if->setVideoCallback(&m_texmgr);
	}

	return m_if;
}

void YouMeTalk::setToken( const char* pToken ){
	getInf()->setToken(pToken);
}

void YouMeTalk::setAutoSendStatus( bool bAutoSend ){
	getInf()->setAutoSendStatus(bAutoSend);
}

int YouMeTalk::getChannelUserList( const char*  channelID, int maxCount, bool notifyMemChange ){
	return getInf()->getChannelUserList(channelID, maxCount, notifyMemChange);
}

int YouMeTalk::setVadCallbackEnabled(bool enabled){
	return getInf()->setVadCallbackEnabled(enabled);
}

int YouMeTalk::setMicLevelCallback(int maxLevel){
	return getInf()->setMicLevelCallback(maxLevel);
}

int YouMeTalk::setReleaseMicWhenMute(bool enabled){
	return getInf()->setReleaseMicWhenMute(enabled);
}

int YouMeTalk::setExitCommModeWhenHeadsetPlugin(bool enabled){
	return getInf()->setExitCommModeWhenHeadsetPlugin(enabled);
}

int YouMeTalk::requestRestApi( const  char* strCommand , const  char* strQueryBody , int* requestID){
	return getInf()->requestRestApi(strCommand, strQueryBody, requestID);
}

int YouMeTalk::setGrabMicOption(const char* pChannelID, int mode, int maxAllowCount, int maxTalkTime, unsigned int voteTime){
	return getInf()->setGrabMicOption(pChannelID, mode, maxAllowCount, maxTalkTime, voteTime);
}

int YouMeTalk::startGrabMicAction(const char* pChannelID, const char* pContent){
	return getInf()->startGrabMicAction(pChannelID, pContent);
}

int YouMeTalk::stopGrabMicAction(const char* pChannelID, const char* pContent){
	return getInf()->stopGrabMicAction(pChannelID, pContent);
}

int YouMeTalk::requestGrabMic(const char* pChannelID, int score, bool isAutoOpenMic, const char* pContent){
	return getInf()->requestGrabMic(pChannelID, score, isAutoOpenMic, pContent);
}

int YouMeTalk::releaseGrabMic(const char* pChannelID){
	return getInf()->releaseGrabMic(pChannelID);
}

int YouMeTalk::setInviteMicOption(const char* pChannelID, int waitTimeout, int maxTalkTime){
	return getInf()->setInviteMicOption(pChannelID, waitTimeout, maxTalkTime);
}

int YouMeTalk::requestInviteMic(const char* pChannelID, const char* pUserID, const char* pContent){
	return getInf()->requestInviteMic(pChannelID, pUserID, pContent);
}

int YouMeTalk::responseInviteMic(const char* pUserID, bool isAccept, const char* pContent){
	return getInf()->responseInviteMic(pUserID, isAccept, pContent);
}

int YouMeTalk::stopInviteMic(){
	return getInf()->stopInviteMic();
}

bool YouMeTalk::releaseMicSync(){
    return getInf()->releaseMicSync();
}

bool YouMeTalk::resumeMicSync(){
    return getInf()->resumeMicSync();
}

int YouMeTalk::openBeautify(bool open){
    return getInf()->openBeautify(open);
}

int YouMeTalk::beautifyChanged(float param){
    return getInf()->beautifyChanged(param);
}

int YouMeTalk::stretchFace(bool stretch){
    return getInf()->stretchFace(stretch);
}

void YouMeTalk::setRestApiCallback(IRestApiCallback* cb ){
	getInf()->setRestApiCallback(cb);
}

void YouMeTalk::setMemberChangeCallback( IYouMeMemberChangeCallback* cb ){
	getInf()->setMemberChangeCallback(cb);
}

void YouMeTalk::setNotifyCallback(IYouMeChannelMsgCallback* cb){
	getInf()->setNotifyCallback(cb);
}
