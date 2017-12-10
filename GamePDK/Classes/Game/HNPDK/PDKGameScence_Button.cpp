#include "PDKGameScence.h"
#include "Game/GameLib.h"
#include "CMD_PDK.h"
#include "PDKPlayer.h"
#include "GameLib/JniCross/JniFun.h"
#include "PDKSoundFun.h"

void PDKGameScence::initButton()
{
	WidgetManager::addButtonCB("PDKButton_Ready",this,button_selector(PDKGameScence::PDKButton_Ready));
	WidgetManager::addButtonCB("PDKButton_OnceAgain",this,button_selector(PDKGameScence::PDKButton_Ready));
	WidgetManager::addButtonCB("PDKButton_OutCard",this,button_selector(PDKGameScence::PDKButton_OutCard));
	WidgetManager::addButtonCB("PDKButton_TiShi",this,button_selector(PDKGameScence::PDKButton_TiShi));
	WidgetManager::addButtonCB("PDKButton_RefreshHandCard",this,button_selector(PDKGameScence::PDKButton_RefreshHandCard));
    WidgetManager::addButtonCB("Button_TalkDefine",this,button_selector(PDKGameScence::Button_TalkDefine));
    WidgetManager::addButtonCB("Button_Send_TalkStr",this,button_selector(PDKGameScence::Button_Send_TalkStr));
    
    WidgetManager::addButtonCB("HNMJButton_BiaoQing0",this,button_selector(PDKGameScence::HNMJButton_BiaoQing));
    WidgetManager::addButtonCB("HNMJButton_BiaoQing1",this,button_selector(PDKGameScence::HNMJButton_BiaoQing));
    WidgetManager::addButtonCB("HNMJButton_BiaoQing2",this,button_selector(PDKGameScence::HNMJButton_BiaoQing));
    WidgetManager::addButtonCB("HNMJButton_BiaoQing3",this,button_selector(PDKGameScence::HNMJButton_BiaoQing));
    WidgetManager::addButtonCB("HNMJButton_BiaoQing4",this,button_selector(PDKGameScence::HNMJButton_BiaoQing));
    WidgetManager::addButtonCB("HNMJButton_BiaoQing5",this,button_selector(PDKGameScence::HNMJButton_BiaoQing));
    WidgetManager::addButtonCB("HNMJButton_BiaoQing6",this,button_selector(PDKGameScence::HNMJButton_BiaoQing));
    WidgetManager::addButtonCB("HNMJButton_BiaoQing7",this,button_selector(PDKGameScence::HNMJButton_BiaoQing));
    
    WidgetManager::addButtonCBBegin("HNMJButton_Talk",this,button_selector(PDKGameScence::HNMJButton_TalkBegin));
    WidgetManager::addButtonCB("HNMJButton_Talk",this,button_selector(PDKGameScence::HNMJButton_TalkEnd));
    
    WidgetManager::addButtonCB("HNMJButton_Msg",this,button_selector(PDKGameScence::HNMJButton_Msg));
}

void PDKGameScence::HNMJButton_Msg(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    WidgetFun::setVisible(this,"GameTalkPlane",true);
}

void PDKGameScence::PDKButton_Ready(cocos2d::Ref*,WidgetUserInfo*)
{
	sendReady();
}

void PDKGameScence::PDKButton_OutCard(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	if(m_pLocal->sendOutCard())
    {
        m_pLocal->showBaoDan((m_pLocal->m_nHandCardNum == 1));
        showActionNode(false);
    }
}

void PDKGameScence::PDKButton_TiShi(cocos2d::Ref*,WidgetUserInfo*)
{
	onTishi();
}

void PDKGameScence::PDKButton_RefreshHandCard(cocos2d::Ref*,WidgetUserInfo*)
{
	m_pLocal->showHandCard();
}


void PDKGameScence::HNMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*)
{
    if (WidgetFun::isWidgetVisble(this,"TalkImagic"))
    {
        JniFun::stopSoundRecord();
        WidgetFun::setVisible(this,"TalkImagic",false);
        return;
    }
    rootNode = this;
    SoundFun::Instance().PaseBackMusic();
    WidgetFun::setVisible(this,"TalkImagic",true);
    JniFun::startSoundRecord();
    int iTimeID = TimeManager::Instance().addCerterTimeNode(TIME_CALLBACK_Node(PDKGameScence::TimeEnd_TalkEnd,this),5.0f,WidgetFun::getChildWidget(this, "TalkImagic"))->iIdex;
    WidgetFun::setWidgetUserInfo(this,"TalkImagic","TimeID",utility::toString(iTimeID));
}

void PDKGameScence::TimeEnd_TalkEnd(cocos2d::Node* node)
{
    SoundFun::Instance().OnResumeBackMusic();
    if (!node->isVisible())
    {
        return;
    }
    int iTimeID = utility::parseInt(WidgetFun::getWidgetUserInfo(node,"TimeID"));
    TimeManager::Instance().removeByID(iTimeID);
    node->setVisible(false);
    std::string kFileName = JniFun::stopSoundRecord();
    sendTalkFile(m_pLocal->GetChairID(),kFileName);
}

void PDKGameScence::HNMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*)
{
    PDKButton_TalkEnd(NULL,NULL);
}

void PDKGameScence::PDKButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*)
{
    SoundFun::Instance().OnResumeBackMusic();
    if (!WidgetFun::isWidgetVisble(rootNode,"TalkImagic"))
    {
        return;
    }
    int iTimeID = utility::parseInt(WidgetFun::getWidgetUserInfo(rootNode,"TalkImagic","TimeID"));
    TimeManager::Instance().removeByID(iTimeID);
    WidgetFun::setVisible(rootNode,"TalkImagic",false);
    std::string kFileName = JniFun::stopSoundRecord();
    sendTalkFile(m_pLocal->GetChairID(),kFileName);
}
void PDKGameScence::Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    int iUserIdex = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"Idex"))+1;
    std::string kTaskStr = WidgetFun::getUserInfoValue(pUserInfo,"Txt");
    std::string kTxt = PDKSoundFun::getDefineSound(m_pLocal->GetGender(),utility::toString(iUserIdex));
    sendTalkDefine(m_pLocal->GetChairID(),utility::toString(kTxt,":",kTaskStr));
    
    WidgetFun::setVisible(this,"GameTalkPlane",false);
}
void PDKGameScence::Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    std::string kTxt = WidgetFun::getEditeText(this,"GameTalkEdit");
    sendTalkString(m_pLocal->GetChairID(),kTxt);
    WidgetFun::setVisible(this,"GameTalkPlane",false);
    WidgetFun::setEditeText(this,"GameTalkEdit","");
}
void PDKGameScence::HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
    std::string kFile = WidgetFun::getUserInfoValue(pUserInfo,"File");
    sendTalkBiaoQing(m_pLocal->GetChairID(),kFile);
    WidgetFun::setVisible(this,"GameTalkPlane",false);
}
