#include "NCMessageScence.h"
#include "NCScenceManager.h"
#include "Game/GameLib.h"

FV_SINGLETON_STORAGE(NCMessageScence);

NCMessageScence::NCMessageScence():m_kPublicNoticMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
    m_kPublicNoticMission.setMissionSink(this);
}
NCMessageScence::~NCMessageScence()
{

}
bool NCMessageScence::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNMessageScence.xml",this);

	WidgetManager::addButtonCB("Button_Message",this,button_selector(NCMessageScence::Button_Message));
	return true;
}
void NCMessageScence::EnterScence()
{
}
void NCMessageScence::hideAll()
{
	WidgetFun::setVisible(this,"MessagePlane",false);
}
void NCMessageScence::Button_Message(cocos2d::Ref*,WidgetUserInfo*)
{
    m_kPublicNoticMission.PublicNotice("NC_NOTICE");
	WidgetFun::setVisible(this,"MessagePlane",true);
}

void NCMessageScence::onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription)
{
    std::string kStr = utility::a_u8(szDescription);
    WidgetFun::setText(this, "MegStrText", kStr);
}

void NCMessageScence::onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr )
{
    NoticeMsg::Instance().ShowTopMsg(utility::a_u8(pStr));
}
