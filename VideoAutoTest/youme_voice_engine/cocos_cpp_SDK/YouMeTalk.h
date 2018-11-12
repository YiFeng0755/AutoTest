#ifndef cocos2d_x_sdk_YouMeTalkEngine_h
#define cocos2d_x_sdk_YouMeTalkEngine_h

#include "IYouMeVoiceEngine.h"
#include "YMRenderTexture.h"

#include <string>
#include <functional>

class YouMeTalk 
{
public:
	static YouMeTalk *getInstance();

	/**
	*  功能描述:销毁引擎实例，释放内存
	*
	*  @return 无
	*/
	static void destroy();
private:
	YouMeTalk();
	~YouMeTalk();
public:
	int init(const char* pAPPKey, const char* pAPPSecret,
		int serverRegionId, const char* pExtServerRegionName);

	/**
	*  功能描述:反初始化引擎
	*
	*  @return 错误码，详见YouMeConstDefine.h定义
	*/

	int unInit();

    /**
     *  功能描述:设置身份验证的token
     *  @param pToken: 身份验证用token，设置为NULL或者空字符串，清空token值。
     *  @return 无
     */
    void setToken( const char* pToken );
	
	/**
	*  功能描述:设置服务器区域
	*  @param region: YOUME_RTC_SERVER_REGION枚举可选的服务器区域
	*  @return 无
	*/
	void setServerRegion(int regionId, const char* extRegionName, bool bAppend);

	/**
	*  功能描述:设置扬声器静音
	*
	*  @param bOn:true——静音，false——取消静音
	*  @return 无
	*/
	void setSpeakerMute(bool bOn);

	/**
	*  功能描述:获取扬声器静音状态
	*
	*  @return true——静音，false——没有静音
	*/
	bool getSpeakerMute();

	/**
	*  功能描述:获取麦克风静音状态
	*
	*  @return true——静音，false——没有静音
	*/
	bool getMicrophoneMute();

	/**
	*  功能描述:设置麦克风静音
	*
	*  @param bOn:true——静音，false——取消静音
	*  @return 无
	*/
	void setMicrophoneMute(bool mute);

	/**
	*  功能描述:获取音量大小,此音量值为程序内部的音量，与系统音量相乘得到程序使用的实际音量
	*
	*  @return 音量值[0,100]
	*/
	unsigned int getVolume();

	/**
	*  功能描述:增加音量，音量数值加1
	*
	*  @return 无
	*/
	void setVolume(const unsigned int &uiVolume);

    /**
     *  功能描述:设置是否通知其他人自己的开关麦克风和扬声器的状态
     *
     *  @param bAutoSend:true——通知，false——不通知
     *  @return 无
     */
    void setAutoSendStatus( bool bAutoSend );
	
	/**
	*  功能描述:是否可使用移动网络
	*
	*  @return true-可以使用，false-禁用
	*/
	bool getUseMobileNetworkEnabled();

	/**
	*  功能描述:启用/禁用移动网络
	*
	*  @param bEnabled:true-可以启用，false-禁用，默认禁用
	*
	*  @return 无
	*/
	void setUseMobileNetworkEnabled(bool bEnabled);


	//---------------------多人语音接口---------------------//

	/**
	*  功能描述:加入语音频道
	*
	* @param strUserID:用户ID，要保证全局唯一
	*  @param strRoomID:频道ID，要保证全局唯一
	*
	*  @return 错误码，详见YouMeConstDefine.h定义
	*/

	/**
	*  功能描述:加入语音频道（单频道模式，每个时刻只能在一个语音频道里面）
	*
	*  @param pUserID: 用户ID，要保证全局唯一
	*  @param pChannelID: 频道ID，要保证全局唯一
	*  @param eUserRole: 用户角色，用于决定讲话/播放背景音乐等权限
	*
	*  @return 错误码，详见YouMeConstDefine.h定义
	*/
	int joinChannelSingleMode(const char* pUserID, const char* pChannelID, int eUserRole);


	/**
	*  功能描述：加入语音频道（多频道模式，可以同时在多个语音频道里面）
	*
	*  @param pUserID: 用户ID，要保证全局唯一
	*  @param pChannelID: 频道ID，要保证全局唯一
    *  @param eUserRole: 用户角色，用于决定讲话/播放背景音乐等权限
	*
	*  @return 错误码，详见YouMeConstDefine.h定义
	*/
	int joinChannelMultiMode(const char* pUserID, const char* pChannelID, int eUserRole);

	/**
	*  功能描述：对指定频道说话
	*
	*  @param pChannelID: 频道ID，要保证全局唯一
	*
	*  @return 错误码，详见YouMeConstDefine.h定义
	*/
	int speakToChannel(const char* pChannelID);

	/**
	*  功能描述:退出多频道模式下的某个语音频道
	*
	*  @param pChannelID:频道ID，要保证全局唯一
	*
	*  @return 错误码，详见YouMeConstDefine.h定义
	*/
	int leaveChannelMultiMode(const char* pChannelID);

	/**
	*  功能描述:退出所有语音频道
	*
	*  @return 错误码，详见YouMeConstDefine.h定义
	*/
	int leaveChannelAll();

    /**
     *  功能描述:查询频道的用户列表
     *  @param channelID:要查询的频道ID
     *  @param maxCount:想要获取的最大数量，-1表示获取全部
     *  @param notifyMemChagne: 其他用户进出房间时，是否要收到通知
     *  @return 错误码，详见YouMeConstDefine.h定义
     */
    int getChannelUserList( const char*  channelID, int maxCount, bool notifyMemChange );
	
	//    /**
	//     *  功能描述:控制其他人的麦克风开关
	//     *
	//     *  @param pUserID:用户ID，要保证全局唯一
	//     *  @param status:false关闭对方麦克风，true打开对方麦克风
	//     *
	//     *  @return 错误码，详见YouMeConstDefine.h定义
	//     */
	//    YouMeErrorCode setOtherMicStatus (const char* pUserID,bool status);
	//    
	//    /**
	//     *  功能描述:控制其他人的扬声器开关
	//     *
	//     *  @param pUserID:用户ID，要保证全局唯一
	//     *  @param status:false关闭对方扬声器，true打开对方扬声器
	//     *
	//     *  @return 错误码，详见YouMeConstDefine.h定义
	//     */
	//    YouMeErrorCode setOtherSpeakerStatus (const char* pUserID,bool status);
	//    
	/**
	*  功能描述:选择消除其他人的语音(NOTICE: deprecated)
	*
	*  @param pUserID:用户ID，要保证全局唯一
	*  @param status:false屏蔽对方语音，true取消屏蔽
	*
	*  @return 错误码，详见YouMeConstDefine.h定义
	*/
	int avoidOtherVoiceStatus(const char* pUserID, bool status);
	
	/**
    *  功能描述:选择消除其他人的语音
    *
    *  @param pUserID:用户ID，要保证全局唯一
    *  @param on: false屏蔽对方语音，true取消屏蔽
    *
    *  @return 错误码，详见YouMeConstDefine.h定义
    */
    int setListenOtherVoice (const char* pUserID, bool on );
	
	//    
	//    /**
	//     *  功能描述: 将提供的音频数据混合到麦克风或者扬声器的音轨里面。
	//     *  @param pBuf 指向PCM数据的缓冲区
	//     *  @param nSizeInByte 缓冲区中数据的大小，以Byte为单位
	//     *  @param nChannelNum 声道数量
	//     *  @param nSampleRateHz 采样率, 已Hz为单位
	//     *  @param nBytesPerSample 一个声道里面每个采样的字节数，典型情况下，如果数据是整型，该值为2，如果是浮点数，该值为4
	//     *  @param bFloat, true - 数据是浮点数， false - 数据是整型
	//     *  @param bLittleEndian, true - 数据是小端存储，false - 数据是大端存储
	//     *  @param bInterleaved, 用于多声道数据时，true - 不同声道采样交错存储，false - 先存储一个声音数据，再存储另一个声道数据
	//     *  @param bForSpeaker, true - 该数据要混到扬声器输出， false - 该数据要混到麦克风输入
	//     *  @return YOUME_SUCCESS - 成功
	//     *          其他 - 具体错误码
	//     */
	//    YouMeErrorCode mixAudioTrack(const void* pBuf, int nSizeInByte, int nChannelNUm,
	//                                 int nSampleRate, int nBytesPerSample, bool bFloat,
	//                                 bool bLittleEndian, bool bInterleaved, bool bForSpeaker);

	/**
	*  功能描述: 播放背景音乐，并允许选择混合到扬声器输出麦克风输入
	*  @param pFilePath 音乐文件的路径
	*  @param bRepeat 是否重复播放
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int playBackgroundMusic(const char* pFilePath, bool bRepeat);

	/**
	*  功能描述: 如果当前正在播放背景音乐的话，停止播放
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int stopBackgroundMusic();

	/**
	*  功能描述: 设置背景音乐播放的音量
	*  @param vol 背景音乐的音量，范围 0-100
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int setBackgroundMusicVolume(int vol);

	/**
	*  功能描述: 设置是否用耳机监听自己的声音，当不插耳机时，这个设置不起作用
	*  @param enabled, true 监听，false 不监听
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int setHeadsetMonitorOn(bool enabled);

	/**
	*  功能描述: 设置是否开启主播混响模式
	*  @param enabled, true 开启，false 关闭
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int setReverbEnabled(bool enabled);

    /**
     *  功能描述: 设置是否开启语音检测回调
     *  @param enabled, true 开启，false 关闭
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int setVadCallbackEnabled(bool enabled);

    /**
     *  功能描述: 设置是否开启讲话音量回调, 并设置相应的参数
     *  @param maxLevel, 音量最大时对应的级别，最大可设100。根据实际需要设置小于100的值可以减少回调的次数。
     *                   比如你只在UI上呈现10级的音量变化，那就设10就可以了。
     *                   设 0 表示关闭回调。
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int setMicLevelCallback(int maxLevel);

    /**
     *  功能描述: 设置当麦克风静音时，是否释放麦克风设备，在初始化之后、加入房间之前调用
     *  @param enabled,
     *      true 当麦克风静音时，释放麦克风设备，此时允许第三方模块使用麦克风设备录音。在Android上，语音通过媒体音轨，而不是通话音轨输出。
     *      false 不管麦克风是否静音，麦克风设备都会被占用。
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int setReleaseMicWhenMute(bool enabled);
	
    /**
     *  功能描述: 设置插入耳机时，是否自动退出系统通话模式(禁用手机硬件提供的回声消除等信号前处理)
     *          系统提供的前处理效果包括回声消除、自动增益等，有助于抑制背景音乐等回声噪音，减少系统资源消耗
     *          由于插入耳机可从物理上阻断回声产生，故可设置禁用该效果以保留背景音乐的原生音质效果
     *          注：Windows和macOS不支持该接口
     *  @param enabled,
     *      true 当插入耳机时，自动禁用系统硬件信号前处理，拔出时还原
     *      false 插拔耳机不做处理
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int setExitCommModeWhenHeadsetPlugin(bool enabled);

	/**
	*  功能描述: 暂停通话，释放麦克风等设备资源
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int pauseChannel();

	/**
	*  功能描述: 恢复通话
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int resumeChannel();

	/**
	*  功能描述:获取SDK 版本号
	*
	*
	*  @return 整形数字版本号
	*/
	int getSDKVersion();
	
	  /**
     *  功能描述:Rest API , 向服务器请求额外数据
     *  @param requestID: 回传id,回调的时候传回，标识消息
     *  @param pCommand: 请求的命令字符串
     *  @param pQueryBody: 请求需要的数据,json格式，内容参考restAPI
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int requestRestApi( const char*  pCommand , const char* pQueryBody , int* requestID = NULL  );

	//---------------------抢麦接口---------------------//
	 /**
	 * 功能描述:    抢麦相关设置（抢麦活动发起前调用此接口进行设置）
	 * @param const char * pChannelID: 抢麦活动的频道id
	 * @param int mode: 抢麦模式（1:先到先得模式；2:按权重分配模式）
	 * @param int maxAllowCount: 允许能抢到麦的最大人数
	 * @param int maxTalkTime: 允许抢到麦后使用麦的最大时间（秒）
	 * @param unsigned int voteTime: 抢麦仲裁时间（秒），过了X秒后服务器将进行仲裁谁最终获得麦（仅在按权重分配模式下有效）
	 * @return   YOUME_SUCCESS - 成功
	 *          其他 - 具体错误码
	 */
	 int setGrabMicOption(const char* pChannelID, int mode, int maxAllowCount, int maxTalkTime, unsigned int voteTime);

	/**
	 * 功能描述:    发起抢麦活动
	 * @param const char * pChannelID: 抢麦活动的频道id
	 * @param const char * pContent: 游戏传入的上下文内容，通知回调会传回此内容（目前只支持纯文本格式）
	 * @return   YOUME_SUCCESS - 成功
	 *          其他 - 具体错误码
	 */
	int startGrabMicAction(const char* pChannelID, const char* pContent);

	/**
	 * 功能描述:    停止抢麦活动
	 * @param const char * pChannelID: 抢麦活动的频道id
	 * @param const char * pContent: 游戏传入的上下文内容，通知回调会传回此内容（目前只支持纯文本格式）
	 * @return   YOUME_SUCCESS - 成功
	 *          其他 - 具体错误码
	 */
	int stopGrabMicAction(const char* pChannelID, const char* pContent);

	 /**
	  * 功能描述:    发起抢麦请求
	  * @param const char * pChannelID: 抢麦的频道id
	  * @param int score: 积分（权重分配模式下有效，游戏根据自己实际情况设置）
	  * @param bool isAutoOpenMic: 抢麦成功后是否自动开启麦克风权限
	  * @param const char * pContent: 游戏传入的上下文内容，通知回调会传回此内容（目前只支持纯文本格式）
	  * @return   YOUME_SUCCESS - 成功
	  *          其他 - 具体错误码
	  */
	 int requestGrabMic(const char* pChannelID, int score, bool isAutoOpenMic, const char* pContent);

	 /**
	  * 功能描述:    释放抢到的麦
	  * @param const char * pChannelID: 抢麦活动的频道id
	  * @return   YOUME_SUCCESS - 成功
	  *          其他 - 具体错误码
	  */
	 int releaseGrabMic(const char* pChannelID);


	//---------------------连麦接口---------------------//
	 /**
	 * 功能描述:    连麦相关设置（角色是频道的管理者或者主播时调用此接口进行频道内的连麦设置）
	 * @param const char * pChannelID: 连麦的频道id
	 * @param int waitTimeout: 等待对方响应超时时间（秒）
	 * @param int maxTalkTime: 最大通话时间（秒）
	 * @return   YOUME_SUCCESS - 成功
	 *          其他 - 具体错误码
	 */
	 int setInviteMicOption(const char* pChannelID, int waitTimeout, int maxTalkTime);

	 /**
	  * 功能描述:    发起与某人的连麦请求（主动呼叫）
	  * @param const char * pUserID: 被叫方的用户id
	  * @param const char * pContent: 游戏传入的上下文内容，通知回调会传回此内容（目前只支持纯文本格式）
	  * @return   YOUME_SUCCESS - 成功
	  *          其他 - 具体错误码
	  */
	 int requestInviteMic(const char* pChannelID, const char* pUserID, const char* pContent);

	 /**
	  * 功能描述:    对连麦请求做出回应（被动应答）
	  * @param const char * pUserID: 主叫方的用户id
	  * @param bool isAccept: 是否同意连麦
	  * @param const char * pContent: 游戏传入的上下文内容，通知回调会传回此内容（目前只支持纯文本格式）
	  * @return   YOUME_SUCCESS - 成功
	  *          其他 - 具体错误码
	  */
	 int responseInviteMic(const char* pUserID, bool isAccept, const char* pContent);

	 /**
	  * 功能描述:    停止连麦
	  * @return   YOUME_SUCCESS - 成功
	  *          其他 - 具体错误码
	  */
	 int stopInviteMic();
	//---------------------连麦接口结束---------------------//

	//---------------------视频接口---------------------//	
	/**
	*  功能描述: 设置是否开启视频编码器
	*  @param pFilePath: yuv文件的绝对路径
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int openVideoEncoder(const char* pFilePath);


	/**
	*  功能描述: 创建渲染
	*  @param : userId 用户ID
	*  @return 大于等于0 - renderId
	*          小于0 - 具体错误码
	*/
	int createRender(const char * userId);

	/**
     *  功能描述: 删除渲染
     *  @param : renderId
     *  @return 等于0 - success
     *          小于0 - 具体错误码
     */
    int deleteRender(int renderId);
	
	/**
	*  功能描述: 开始camera capture
	*  @param
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int startCapture();

	/**
	*  功能描述: 停止camera capture
	*  @param
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int stopCapture();

	/**
	*  功能描述: 设置本地视频渲染回调的分辨率
	*  @param
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int setCaptureProperty(int nFps, int nWidth, int nHeight);
    
    /**
     *  功能描述: 设置视频网络传输过程的分辨率，低分辨率
     *  @param
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int setNetCaptureProperty(int nWidth, int nHeight);

	/**
	*  功能描述: 设置是否前置摄像头
	*  @param
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int setCaptureFrontCameraEnable(bool enable);

	/**
	*  功能描述: 切换前置/后置摄像头
	*  @param
	*  @return YOUME_SUCCESS - 成功
	*          其他 - 具体错误码
	*/
	int switchCamera();

    /**
     *  功能描述: 权限检测结束后重置摄像头
     *  @param
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int resetCamera();

    /**
     *  功能描述: 屏蔽视频流
     *  @param : userId 用户ID
     *  @param : mask 1 屏蔽, 0 恢复
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */	
    int maskVideoByUserId(const char * userId, int mask);

    //----------------美颜相关-----------------//
    /**
     *  功能描述: 美颜开关，默认是关闭美颜
     *  @param open: true表示开启美颜，false表示关闭美颜
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int openBeautify(bool open) ;
    
    /**
     *  功能描述: 美颜强度参数设置
     *  @param param: 美颜参数，0.0 - 1.0 ，默认为0，几乎没有美颜效果，0.5左右效果明显
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int beautifyChanged(float param) ;
    
    /**
     *  功能描述: 瘦脸开关
     *  @param param: true 开启瘦脸，false关闭，默认 false
     *  @return YOUME_SUCCESS - 成功
     *          其他 - 具体错误码
     */
    int stretchFace(bool stretch) ;
	//---------------------视频接口结束---------------------//
    /**
     *  功能描述: 调用后同步完成麦克风释放，只是为了方便使用 IM 的录音接口时切换麦克风使用权。
     *  @param
     *  @return true - 成功
     */
    bool releaseMicSync();
    
    /**
     *  功能描述: 调用后恢复麦克风到释放前的状态，只是为了方便使用 IM 的录音接口时切换麦克风使用权。
     *  @param
     *  @return true - 成功
     */
    bool resumeMicSync();
public:
	int bindTexture(std::string & userId, int width, int height, cocos2d::RenderTexture * texture);

	void unbindTexture(std::string& userId);

	void updateTextures();

	void cleanTextures();
public:
	/**
     *  功能描述:主逻辑的事件回调消息（NOTICE:需要在init之前调用此函数）
     *  @param cb: 事件回调函数， 需要继承IYouMeEventCallback并实现其中的回调函数
     *
     *  @return None
     */
	void SetEventCallback(IYouMeEventCallback* cb){ m_eventcb = cb; }
	/**
     *  功能描述:requestRestApi的回调消息
     *  @param cb: requestRestApi的回调， 需要继承IRestApiCallback并实现其中的回调函数
     *
     *  @return None
     */
    void setRestApiCallback(IRestApiCallback* cb );
    
    /**
     *  功能描述:getChannelUserList的回调消息
     *  @param cb: getChannelUserList的回调， 需要继承IYouMeMemberChangeCallback并实现其中的回调函数
     *
     *  @return None
     */
    void setMemberChangeCallback( IYouMeMemberChangeCallback* cb );

	/**
	 * 功能描述:    设置频道内的广播消息回调(抢麦、连麦等）
	 * @param cb: 抢麦、连麦等的广播回调，需要继承IYouMeNotifyCallback并实现其中的回调函数
	 * @return   void: 
	 */
	 void setNotifyCallback(IYouMeChannelMsgCallback* cb);	
public:
	IYouMeVoiceEngine* getInf();	//NOTICE:除非特殊情况，否则尽量不要调用此接口
private:
	typedef unsigned char  uint_8;
	typedef unsigned int   uint_32;

	void yuv420p_to_rgb888(uint_8 * ptr, const uint_8 * yplane, const uint_8 * uplane, const uint_8 * vplane, const uint_32 width, const uint_32 height);
	void yuv420p_to_rgba8888(uint_8 * ptr, const uint_8 * yplane, const uint_8 * uplane, const uint_8 * vplane, const uint_32 width, const uint_32 height);
private:
	IYouMeVoiceEngine* m_if = nullptr;
	IYouMeEventCallback* m_eventcb = nullptr;
	YMRenderTexture m_texmgr;
};

#endif
