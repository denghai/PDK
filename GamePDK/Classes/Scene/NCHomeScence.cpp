#include "NCHomeScence.h"
#include "NCScenceManager.h"
#include "Game/GameLib.h"
#include "JniCross/JniFun.h"

FV_SINGLETON_STORAGE(NCHomeScence);

NCHomeScence::NCHomeScence():m_kPublicNoticMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	CServerListData::shared()->SetServerListDataSink(this);
	UserInfo::Instance().addUpPlayerInfoCB(this,QY_CALLFUNC_SELECTOR(NCHomeScence::upPlayerInfo));
	UserInfo::Instance().addLoginSucessCB(this,QY_CALLFUNC_SELECTOR(NCHomeScence::LogonSucess));
    m_kPublicNoticMission.setMissionSink(this);
}
NCHomeScence::~NCHomeScence()
{

}
bool NCHomeScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNHomeScence.xml",this);

	init_Button();

	return true;
}
void NCHomeScence::EnterScence()
{
    upPlayerInfo();
    float fEffect = cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar1",1);
    SoundFun::Instance().SetSoundEffect(fEffect);
    float fSound = cocos2d::UserDefault::getInstance()->getFloatForKey("SetBar2",1);
    SoundFun::Instance().SetSoundBackground(fSound);
    SoundFun::Instance().playBackMusic("raw/backMusic.mp3");
}
void NCHomeScence::upPlayerInfo()
{
	WidgetFun::setText(this,"HomeID",utility::paseInt(UserInfo::Instance().getUserID(),6));
	WidgetFun::setText(this,"HomeName",UserInfo::Instance().getUserNicName());
	WidgetFun::setText(this,"HomeGold",UserInfo::Instance().getUserInsure());
//	WidgetFun::setText(this,"HomeFangKa",UserInfo::Instance().getUserInsure());
	ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(this,"HeadImagic"),
		UserInfo::Instance().getHeadHttp(),UserInfo::Instance().getUserID());
}
void NCHomeScence::LogonSucess()
{
	UserInfo::Instance().reqAccountInfo();
    m_kPublicNoticMission.PublicNotice("HN_SC_NOTICE");
    m_kPublicNoticMission.PublicNotice("NC_SHOP_TXT");
}

void NCHomeScence::UpdatePaomadeng(std::string paomadeng){
    utility::runPaoMaDeng(this, "SysSpeakerTxt" ,"LayoutNode", paomadeng, NULL);
}

void NCHomeScence::UpdateChongKaNotice(std::string notice){
    
    std::string myKey = "ShopTxt";
    if (WidgetFun::getChildWidget(this,myKey) != NULL)
    {
        WidgetFun::setText(this,myKey,notice);
    }
}

void NCHomeScence::upGameSever()
{
    
}

void NCHomeScence::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
    std::string kStr = utility::a_u8(szDescription);
    if(kKey == "NC_SHOP_TXT")
    {
        std::vector<std::string> m_kMsgList = utility::split(kStr,"|");
        WidgetFun::setText(this, "ShopTxt", m_kMsgList[0]);
        WidgetFun::setText(this, "ShopTxth", m_kMsgList[1]);
        
    }
    else
    {
        m_kMsgList = utility::split(kStr,"|");
        NextSpeaker();
    }
}

void NCHomeScence::NextSpeaker()
{
    if (!m_kMsgList.size())
    {
        return;
    }
    std::string kStr = *m_kMsgList.begin();
    m_kMsgList.erase(m_kMsgList.begin());
    m_kMsgList.push_back(kStr);
    showSysSpeakerTxt(kStr,"255 255 255");
}
void NCHomeScence::showSysSpeakerTxt(std::string kTxt,std::string kColor)
{
    utility::runPaoMaDeng(this,"SysSpeakerTxt","LayoutNode",kTxt,cocos2d::CCCallFunc::create(this,callfunc_selector(NCHomeScence::NextSpeaker)));
}
