//
//
//  IMService.hpp
//  MyCppGame
//
//  Created by 杜红 on 2017/1/22.
//
//

#ifndef IMService_hpp
#define IMService_hpp

#include "YIM.h"
#include "YIMPlatformDefine.h"
#include <cocos2d.h>
#include <string>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <unistd.h>
#else
#include <windows.h>
#define sleep Sleep
#endif

const static XCHAR *APPKEY = __XT("YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804");

const static XCHAR *APPSECRET = __XT("3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=");

static const XCHAR *TOKEN = __XT("");
static const XCHAR *USER_ID_A = __XT("XXXXX88888");
//static const XCHAR *USER_ID_B = __XT("XXXXX99998"); //IM最常用的两个用户
static const XCHAR *USER_ID_B = __XT("1234");
static const XCHAR *ZHUBO_ID = __XT("ZXXXX10000"); //IM最常用的主播ID
static const XCHAR *PACKAGE_NAME = __XT("Test"); //目前用不着
static const XCHAR *UROOM_ID = __XT("2011235");
static const XCHAR *UserIDS[] = {
	__XT("XXXXX10001"), __XT("XXXXX10002"), __XT("XXXXX10003"),
	__XT("XXXXX10004"), __XT("XXXXX10005"), __XT("XXXXX10006"),
	__XT("XXXXX10007"), __XT("XXXXX10008"), __XT("XXXXX10009"),
	__XT("XXXXX10010"), __XT("XXXXX10011"), __XT("XXXXX10012"),
	__XT("XXXXX10013"), __XT("XXXXX10014"), __XT("XXXXX10015"),
	__XT("XXXXX10016"), __XT("XXXXX10017"), __XT("XXXXX10018")
}; //特殊场景下需要使用的用户ID

struct TestMsg{
	bool bTxt = false;               //是否文本
	bool bSelf = false;              //是否自己发送
    std::string sender ="";          //发送者
    std::string content = "";        //如果是文本，则是文本内容，如果是语音，则是语音检测出的文本
    std::string voicePath ="";       //语音的保存地址
    int voiceLen = 0 ;               //语音长度
	unsigned int createTime = 0;     //发送时间
	unsigned long long msgID = 0;    //消息ID，自己发送的没有消息ID
};


static const XCHAR *MessageText[] = {
    __XT("今天，我们在这里隆重集会，纪念全世界工人阶级和劳动群众的盛大节日——“五一”国际劳动节，表彰全国劳动模范和先进工作者，目的是弘扬劳模精神，弘扬劳动精神，弘扬我国工人阶级和广大劳动群众的伟大品格。")
    __XT("首先，我代表党中央、国务院，向全国各族工人、农民、知识分子和其他各阶层劳动群众，向人民解放军指战员、武警部队官兵和公安民警，向香港同胞、澳门同胞、台湾同胞和海外侨胞，致以节日的")
	__XT("向为改革开放和社会主义现代化建设作出突出贡献的劳动模范和先进工作者，致以崇高的敬意！在这里，我代表中国工人阶级和广大劳动群众，向全世界工人阶级和广大劳动群众，致以诚挚的问候！"),
	__XT("“五一”国际劳动节，是全世界劳动人民共同的节日。从恩格斯领导成立的第二国际明确将1890年5月1日定为第一个国际劳动节起，历史车轮已走过125年。"),
	__XT("我们党历来高度重视通过纪念“五一”国际劳动节，启发职工群众觉悟，教育引导劳动群众，团结带领工人阶级和广大劳动群众为民族独立、人民解放和国家富强、人民幸福而奋斗。"),
	__XT("至如一赴绝国，讵相见期？视乔木兮故里，决北梁兮永辞，左右兮魄动，亲朋兮泪滋。可班荆兮憎恨，惟樽酒兮叙悲。值秋雁兮飞日，当白露兮下时，怨复怨兮远山曲，去复去兮长河湄。")
	__XT("同志们！"),
	__XT("*的***以来，***从坚持和发展**********全局出发，提出并形成了全面建成小康社会、全面深化改革、全面依法治国、全面从严治党的战略布局，确立了新形势下党和国家各项工作的战略目标和战略举措，"),
	__XT("历史赋予工人阶级和广大劳动群众伟大而艰巨的使命，时代召唤工人阶级和广大劳动群众谱写壮丽而崭新的篇章。我国工人阶级和广大劳动群众一定要以国家主人翁姿态")
	__XT("——在前进道路上，我们要始终弘扬劳模精神、劳动精神，为中国经济社会发展汇聚强大正能量。劳动是人类的本质活动，劳动光荣、创造伟大是对人类文明进步规律的重要诠释。“民生在勤，勤则不匮。")
	__XT("When his father's ghost visits the castle, Hamlet's suspicions are confirmed. The Ghost complains that he is unable to rest in peace because he was murdered. Claudius, says the Ghost, poured poison in King Hamlet's ear while the old king napped. Unable to confess and find salvation, King Hamlet is now consigned, for a time, to spend his days in Purgatory and walk the earth by night."),

	__XT("黯然销魂者，唯别而已矣！况秦吴兮绝国，复燕宋兮千里。或春苔兮始生，乍秋风兮暂起。是以行子肠断，百感凄恻。风萧萧而异响，云漫漫而奇色。"),
    __XT("舟凝滞于水滨，车逶迟于山侧。棹容与而讵前，马寒鸣而不息。掩金觞而谁御，横玉柱而沾轼。居人愁卧，怳若有亡。日下壁而沉彩，月上轩而飞光。见红兰之受露，望青楸之离霜。巡层楹而空掩，抚锦幕而虚凉。知离梦之踯躅，意别魂之飞扬。故别虽一绪，事乃万族。"),
    __XT("下有芍药之诗，佳人之歌，桑中卫女，上宫陈娥。春草碧色，春水渌波，送君南浦，伤如之何！至乃秋露如珠，秋月如圭，明月白露，光阴往来，与子之别，思心徘徊。"),
	__XT("是以别方不定，别理千名，有别必怨，有怨必盈。使人意夺神骇，心折骨惊，虽渊、云之墨妙，严、乐之笔精，金闺之诸彦，兰台之群英，赋有凌云之称，辨有雕龙之声，谁能摹暂离之状，写永诀之情着乎？ "),
	__XT("傥有华阴上士，服食还仙。术既妙而犹学，道已寂而未传。守丹灶而不顾，炼金鼎而方坚。驾鹤上汉，骖鸾腾天。暂游万里，少别千年。惟世间兮重别，谢主人兮依然"),
	__XT("n order to test the Ghost's sincerity, Hamlet enlists the help of a troupe of players who perform a play called The Murder of Gonzago to which Hamlet has added scenes that recreate the murder the Ghost described. Hamlet calls the revised play The Mousetrap, and the ploy proves a success. As Hamlet had hoped, Claudius' "),
	__XT("又若君居淄右，妾家河阳，同琼珮之晨照，共金炉之夕香。君结绶兮千里，惜瑶草之徒芳。惭幽闺之琴瑟，晦高台之流黄。春宫閟此青苔色，秋帐含此明月光，夏簟清兮昼不暮，冬釭凝兮夜何长！织锦曲兮泣已尽，回文诗兮影独伤"),
	__XT("——在前进道路上，我们要始终实现好、维护好、发展好最广大人民根本利益，让改革发展成果更多更公平惠及人民。人民对美好生活的向往，就是我们的奋斗目标。"),
	__XT("国家建设是全体人民共同的事业，国家发展过程也是全体人民共享成果的过程。我们一定要适应改革开放和发展社会主义市场经济的新形势，从政治、经济、社会、文化、法律、行政等各方面采取有力措施"),
	__XT("或乃边郡未和，负羽从军。辽水无极，雁山参云。闺中风暖，陌上草薰。日出天而曜景，露下地而腾文。镜朱尘之照烂，袭青气之烟煴，攀桃李兮不忍别，送爱子兮沾罗裙。"),
	__XT("乃有剑客惭恩，少年报士，韩国赵厕，吴宫燕市。割慈忍爱，离邦去里，沥泣共诀，抆血相视。驱征马而不顾，见行尘之时起。方衔感于一剑，非买价于泉里。金石震而色变，骨肉悲而心死"),
	__XT("今年是中华全国总工会成立90周年。90年来，在中国共产党领导下，中华全国总工会及其各工会组织为我国革命、建设、改革事业作出了重大贡献。"),
	__XT("Hamlet vows to affect madness — puts an antic disposition on — to wear a mask that will enable him to observe the interactions in the castle, but finds himself more confused than ever. In his persistent confusion, he questions the Ghost's trustworthiness. What if the Ghost is not a true spirit, but rather an agent of the devil sent to tempt him? What if killing Claudius results in Hamlet's having to relive his memories for all eternity? "),
	__XT("各级党委要加强和改善对工会的领导，注重发挥工会组织的作用，健全组织制度，完善工作机制，加大对工会工作的支持保障力度，及时研究解决工会工作中的重大问题，热情关心、严格要求、重视培养工会干部，为工会工作创造更加有利的条件。"),
	__XT("同志们！"),
	__XT(""),
	__XT("伟大的事业呼唤着我们，庄严的使命激励着我们。我国工人阶级和广大劳动群众要更加紧密地团结在党中央周围，勤奋劳动、扎实工作，锐意进取、勇于创造，在实现“两个一百年”奋斗目标的伟大征程上再创新的业绩，以劳动托起中国梦!")
};

#ifndef CHECK_RETURN_CODE
#define CHECK_RETURN_CODE(code, smsg, fmsg)                                             \
do {                                                                                    \
if (code == 0) {                                                                        \
	cocos2d::log("[For you]PASS : %s", smsg);                                                   \
}                                                                                       \
else {                                                                                  \
	cocos2d::log("[For you]Failed : %s, the ret code is %d)", fmsg, code);                      \
  }                                                                                     \
} while (0)
#endif

extern "C" {
	extern void IM_SetMode(int mode);
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static XString app_log_path = __XT("/sdcard/Android/data/org.cocos2dx.ImAutoTest/yimcache");
static XString wav_save_path = __XT("/sdcard/Android/data/org.cocos2dx.ImAutoTest/yimcache");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <YouMeCommon/CrossPlatformDefine/IOS/YouMeApplication_iOS.h>
static XString app_log_path = []()->XString {
    YouMeApplication_iOS iosapp;
    return iosapp.getCachePath();
}();

static XString wav_save_path = []()->XString {
    YouMeApplication_iOS  ios;
    return ios.getCachePath();
}();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static XString app_log_path = __XT("./data/log");
static XString wav_save_path = __XT("./datas");
#else
static XString app_log_path = __XT("");
static XString wav_save_path = __XT("");
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static std::string pkg_log_name = "/sdcard/Android/data/org.cocos2dx.ImAutoTest/yimcache";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
static std::string pkg_log_name = app_log_path;

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
static std::string pkg_log_name = "c:/Users/winci/AppData/Local";
#endif


class IMService
{
public:
    static IMService * getInstance();

    virtual ~IMService();

    /** 回调 */

    //语音文件保存的路径
	std::string getSavePathName() const {
		return XStringToLocal(m_audio_path);
	}
    void setAudioPath(const XString & path) {
        m_audio_path = path;
    }
    void init();
private:
	IMService();
	void setup();

    std::string strTempDir;
    XString m_audio_path;
	static IMService* _instance;
};



#endif /* IMService_hpp */
