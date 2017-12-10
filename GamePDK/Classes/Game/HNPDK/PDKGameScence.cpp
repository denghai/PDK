#include "PDKGameScence.h"
#include "PDKPlayer.h"
#include "Game/GameLib.h"
#include "CMD_PDK.h"
#include "PDKPlayer.h"
#include "PDKSoundFun.h"
#include "Game/Script/ScriptXMLparse.h"

FV_SINGLETON_STORAGE(PDKGameScence);

PDKGameScence::PDKGameScence()
	:m_pLocal(NULL)
	,m_pTouchCardNode(NULL)
	,m_wRecordSelfChairID(0)
	,m_RecordTimeID(0)
	,m_RecordTime(0)
	,m_nCurrentUser(-1)
	,m_bDragHandCard(false)
	,m_nLastOutUser(INVALID_CHAIR)
	,m_nLastOutType(0)
	,GameBase(0,0)
{

	init();
}
PDKGameScence::~PDKGameScence()
{

}
bool PDKGameScence::init()
{
	if (!GameBase::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("GamePDK/xml/GameScence.xml",this);
	cocos2d::ScriptXMLparse kXml("GamePDK/xml/ScriptValueStr.xml");

    cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"GameTypeScenceNode");
    WidgetScenceXMLparse kScence2("GamePDK/xml/GameScence_HN.xml",pNode);
    
    if (WidgetFun::getChildWidget(this,"GameTalkList"))
    {
        cocos2d::ListViewEx* pTalkList = WidgetFun::getListViewWidget(this,"GameTalkList");
        pTalkList->removeAllChildren();
        for (int i=0;i<10;i++)
        {
            cocos2d::Node* pItem = WidgetManager::Instance().createWidget("HNMJGameTalkListItem",pTalkList);
            std::string kTxt = utility::getScriptString(utility::toString("GameTalkTxt",i));
            WidgetFun::setText(pItem,"TalkListItemTxt",kTxt);
            WidgetFun::setWidgetUserInfo(pItem,"Button_TalkDefine","Idex",utility::toString(i));
            WidgetFun::setWidgetUserInfo(pItem,"Button_TalkDefine","Txt",utility::toString(kTxt));
        }
        pTalkList->forceDoLayout();
        
        for (int m = 0;m<6;m++)
        {
            cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString("BiaoQing",m));
            for (int n = 0;n<8;n++)
            {
                std::string kFileName = utility::toString("GamePDK/Talk/EE",m*8+n,".png");
                WidgetFun::setButtonImagic(pNode,utility::toString("HNMJButton_BiaoQing",n),kFileName,true);
                WidgetFun::setWidgetUserInfo(pNode,utility::toString("HNMJButton_BiaoQing",n),"File",kFileName);
            }
        }
    }
    
    initPrivate();

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i] = new PDKPlayer(i,WidgetFun::getChildWidget(this,utility::toString("Player",i)));
	}
    
    m_pLocal = m_pPlayer[0];
    m_nextPlay = m_pPlayer[1];
    
	WidgetManager::addButtonCB("NCMJButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));

	initButton();
	initNet();
	initTouch();
	return true;
}
void PDKGameScence::EnterScence()
{
	WidgetFun::setVisible(this,"GamePDKPlane",true);
	if (WidgetFun::getChildWidget(this,"PrivateEndPlane"))
	{
		WidgetFun::setVisible(this,"PrivateEndPlane",false);
	}
	defaultState();
    
//
    PDKSoundFun::playBackMusic("bgm2.mp3");
}

bool PDKGameScence::IsInGame()
{
	return WidgetFun::isWidgetVisble(this,"GamePDKPlane");
}
void PDKGameScence::HideAll()
{
	WidgetFun::setVisible(this,"GamePDKPlane",false);
	defaultPrivateState();
}
void PDKGameScence::defaultState()
{
	m_pTouchCardNode = NULL;

	for (int i = 0;i<MAX_PLAYER;i++)
	{
		m_pPlayer[i]->defaultState();
	}
	WidgetFun::setVisible(this,"GameResoult2",false);
	defaultPrivateState();
	showActionNode(false);
	resetLastOutInfo();
}

void PDKGameScence::showActionNode(bool bShow)
{
	WidgetFun::setVisible(this,"ActionNode",bShow);
}

void PDKGameScence::setCurrentPlayer(word wChairID)
{
	if (wChairID == INVALID_CHAIR )
	{
		return;
	}
	m_nCurrentUser = wChairID;
	PDKPlayer* pPlayer = getPlayerByChairID(wChairID);
	if (pPlayer)
	{
		pPlayer->clearOutState();
		pPlayer->showClock(true,OUT_CARD_TIME);
		showActionNode(pPlayer == m_pLocal);
	}
}

bool PDKGameScence::onTishi()
{
    if(m_nextPlay->getIdex() != 1)
    {
        m_nextPlay = m_pPlayer[1];
    }
    
    bool isBaoDan = m_nextPlay->m_nHandCardNum==1;
    
    if(m_plyaercount == 2)
    {
        for(int i = 0;i<3; ++i)
        {
            if(m_pLocal->GetChairID() != m_pPlayer[i]->GetChairID())
            {
				
                isBaoDan = m_pPlayer[i]->m_nHandCardNum == 1;
                if(isBaoDan)
                    break;
            }
        }
    }
    
	bool bValue = m_pLocal->tishi(m_nLastOutType,m_kLastOutCardList,isBaoDan);
    if(bValue)
        WidgetFun::setButtonEnabled(this,"PDKButton_OutCard",true);
	return bValue;
}

void PDKGameScence::resetLastOutInfo()
{
	m_nLastOutUser = INVALID_CHAIR;
	m_nLastOutType = 0;
	m_kLastOutCardList.clear();
}

