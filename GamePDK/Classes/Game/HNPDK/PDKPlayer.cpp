#include "PDKPlayer.h"
#include "PDKSoundFun.h"
#include "PDKGameLogic.h"

PDKPlayer::PDKPlayer(int iIdex,cocos2d::Node* pSeatNode )
	:GamePlayer(NULL)
	,m_iIdex(iIdex)
	,m_pSeatNode(pSeatNode)
	,m_bGameRecord(false)
	,m_tipIndex(0)
{
	init();
}

PDKPlayer::~PDKPlayer()
{

}

void PDKPlayer::init()
{
	PlayerLeave();
}
int  PDKPlayer::getIdex()
{
	return m_iIdex;
}
void PDKPlayer::defaultState()
{
	clearHandCard();
	clearOutCard();
	showClock(false,0);
	showFangZhu(false);
	showOffline(false);
	showReady(false);
    showBaoDan(false);
	showPass(false);
	m_kTouchCardList.clear();
	m_kSendCardList.clear();

	upPlayerState();
}

void PDKPlayer::setGameRecord(bool bRecord)
{
	m_bGameRecord = bRecord;
}


void PDKPlayer::PlayerEnter()
{
	WidgetFun::setVisible(m_pSeatNode,"Name",true);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",false);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",true);

	if (WidgetFun::getChildWidget(m_pSeatNode,"ButtonPlayerHeadClick"))
	{
		WidgetFun::setWidgetUserInfo(m_pSeatNode,"ButtonPlayerHeadClick","UserID",utility::toString((int)GetUserID()));
	}
}

void PDKPlayer::startGameNoTime()
{
    WidgetFun::runWidgetAction(m_pSeatNode,"Head","StartNoTime");
    WidgetFun::setVisible(m_pSeatNode,"ReadyState",false);
}

void PDKPlayer::PlayerLeave()
{
	defaultState();
	WidgetFun::setVisible(m_pSeatNode,"HeadImagicEmpty",true);
	WidgetFun::setVisible(m_pSeatNode,"HeadImagic",false);
	WidgetFun::setVisible(m_pSeatNode,"Name",false);
	WidgetFun::setVisible(m_pSeatNode,"GoldImagic",false);
	WidgetFun::setText(m_pSeatNode,"Name","");
	WidgetFun::setText(m_pSeatNode,"GoldTxt","");
}

cocos2d::Node* PDKPlayer::getPlayerNode()
{
	return m_pSeatNode;
}

void PDKPlayer::upPlayerInfo()
{
	WidgetFun::setText(m_pSeatNode,"Name",GetNickName());
	if (GetHeadHttp() != "")
	{
        ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(m_pSeatNode,"HeadImagic"),
			GetHeadHttp(),GetUserID());
	}
}

void PDKPlayer::hide(bool istrue)
{
    
    WidgetFun::setVisible(m_pSeatNode,"Head",!istrue);
    WidgetFun::setVisible(m_pSeatNode,"Name",!istrue);
    WidgetFun::setVisible(m_pSeatNode,"GoldImagic",!istrue);
    WidgetFun::setVisible(m_pSeatNode,"Name",!istrue);
    WidgetFun::setVisible(m_pSeatNode,"GoldTxt",!istrue);
}

void PDKPlayer::setZhuang()
{
	WidgetFun::setVisible(m_pSeatNode,"Zhuang",true);
}
void PDKPlayer::showStatusImagic(std::string kKey)
{
	WidgetFun::setPlaceImagicKey(m_pSeatNode,"ReadyState",kKey);
	showReady(true);
}

void PDKPlayer::showTalkState(CMD_GR_C_TableTalk* pNetInfo)
{
    std::string kStrNet = pNetInfo->strString;
    if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_FILE)
    {
        if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkFile"))
        {
            return;
        }
        WidgetFun::runWidgetAction(m_pSeatNode,"TalkFile","Start");
    }
    if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_WORD)
    {
        if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode"))
        {
            return;
        }
        utility::runPaoMaDeng(m_pSeatNode,"TalkTxt","LayoutNode",
                              kStrNet,NULL);
        cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode");
        pTalkNode->setVisible(true);
        pTalkNode->stopAllActions();
        pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));
    }
    if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_BIAOQING)
    {
        if (!WidgetFun::getChildWidget(m_pSeatNode,"BiaoQingNode"))
        {
            return;
        }
        WidgetFun::setImagic(m_pSeatNode,"BiaoQingPic",kStrNet);
        cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"BiaoQingNode");
        pTalkNode->setVisible(true);
        pTalkNode->stopAllActions();
        pTalkNode->runAction(cocos2d::CCVisibleAction::create(4.0f,false));
    }
    if (pNetInfo->cbType == CMD_GR_C_TableTalk::TYPE_DEFINE)
    {
        if (!WidgetFun::getChildWidget(m_pSeatNode,"TalkFile"))
        {
            return;
        }
        std::vector<std::string> kStr = utility::split(kStrNet,":");
        
        ASSERT(kStr.size()==2);
        
        WidgetFun::runWidgetAction(m_pSeatNode,"TalkFile","Start");
        
        SoundFun::Instance().playEffect(kStr[0]);
        
        utility::runPaoMaDeng(m_pSeatNode,"TalkTxt","LayoutNode",
                              kStr[1],NULL);
        cocos2d::Node* pTalkNode = WidgetFun::getChildWidget(m_pSeatNode,"TalkTxtNode");
        pTalkNode->setVisible(true);
        pTalkNode->stopAllActions();
        pTalkNode->runAction(cocos2d::CCVisibleAction::create(6.0f,false));
    }
}

void PDKPlayer::upPlayerState()
{
    if (GetUserStatus() == US_READY)
	{
		showStatusImagic("Ready");
	}
	WidgetFun::setVisible(m_pSeatNode,"Offline",GetUserStatus() == US_OFFLINE);
}

void PDKPlayer::showClock(bool bShow,int nTime)
{
	cocos2d::Node* pClock = WidgetFun::getChildWidget(m_pSeatNode,"PlayerClock");
	pClock->setVisible(bShow);
	cocos2d::Node* pClockTime = WidgetFun::getChildWidget(pClock,"ClockTime");
	pClockTime->stopAllActions();
	pClockTime->runAction(cocos2d::MoveExTxtTime::create(nTime,nTime,0));
}

void PDKPlayer::showFangZhu(bool bShow)
{
	WidgetFun::setVisible(m_pSeatNode,"Head:FangZhu",bShow);
}

void PDKPlayer::showOffline(bool bShow)
{
	WidgetFun::setVisible(m_pSeatNode,"Head:Offline",bShow);
}

void PDKPlayer::showBaoDan(bool bShow)
{
    if(bShow)
       PDKSoundFun::playEffectByGender(GetGender(),PDKSoundFun::EFFECT_WARN);
    WidgetFun::setVisible(m_pSeatNode,"Head:BaoDan",bShow);
}

void PDKPlayer::showReady(bool bShow)
{
	WidgetFun::setVisible(m_pSeatNode,"ReadyState",bShow);
}

void PDKPlayer::startGame(SCORE lScores)
{
    lScore = (int)lScores;
    WidgetFun::setText(m_pSeatNode,"GoldTxt",lScore);
	showReady(false);
	clearOutState();
}

void PDKPlayer::showPass(bool bShow)
{
	WidgetFun::setVisible(m_pSeatNode,"PlayerPass",bShow);
	if (bShow)
	{
		showClock(false,0);
		PDKSoundFun::playPass(GetGender());
	}
}


void PDKPlayer::showMeiYouPai()
{
	if (WidgetFun::getChildWidget(m_pSeatNode,"PlayerMeiYouDaPai"))
	{
		WidgetFun::runWidgetAction(m_pSeatNode,"PlayerMeiYouDaPai","Start");
	}
}

void PDKPlayer::clearOutState()
{
	showClock(false,0);
	showPass(false);
	clearOutCard();
}

void PDKPlayer::runAniByCards(const std::vector<BYTE>& kCardList)
{
	int nType = PDKGameLogic::Instance().GetCardType(kCardList);
	int nCardNum = kCardList.size();
	switch (nType)
	{
	case CT_SINGLE:
		{
			PDKSoundFun::playCardByGender(GetGender(),(int)kCardList[0]);
			break;
		}
	case CT_SINGLE_LINE:
		{
			runShunZiAni();
			break;
		}	
	case CT_DOUBLE_LINE:
		{
			if (nCardNum==2)
			{
				PDKSoundFun::playCardByGender_Dui(GetGender(),(int)kCardList[0]);
			}
			else
			{
				runLianDuiAni();
			}
			break;
		}
	case CT_THREE_LINE:
		{
			if (nCardNum >=6 )
			{
				runFeiJiAni();
			}
			else
			{
				PDKSoundFun::playEffectByGender(GetGender(),PDKSoundFun::EFFECT_30);
			}
			break;
		}
	case CT_THREE_LINE_TAKE_SINGLE:
		{
			if (nCardNum >=8 )
			{
				runFeiJiAni();
			}
			else
			{
				PDKSoundFun::playEffectByGender(GetGender(),PDKSoundFun::EFFECT_31);
			}
			break;
		}
	case CT_THREE_LINE_TAKE_DOUBLE:
		{
            if(nCardNum <= 5)
            {
                PDKSoundFun::playEffectByGender(GetGender(),PDKSoundFun::EFFECT_32);
            }
            else
            {
                runFeiJiAni();
            }
			break;
		}
	case CT_BOMB:
		{
			runBombAni();
			break;
		}
	default:
		break;
	}
}

void PDKPlayer::runBombAni()
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("BombAni",AnimateNode);

	PDKSoundFun::playEffectByGender(GetGender(),PDKSoundFun::EFFECT_ZHADAN);
}
void PDKPlayer::runLianDuiAni()
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("LianDuiAni",AnimateNode);

	PDKSoundFun::playEffectByGender(GetGender(),PDKSoundFun::EFFECT_LIANDUI);
}
void PDKPlayer::runShunZiAni()
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("ShunZiAni",AnimateNode);

	PDKSoundFun::playEffectByGender(GetGender(),PDKSoundFun::EFFECT_SHUNZI);
}
void PDKPlayer::runFeiJiAni()
{
	cocos2d::Node* AnimateNode = WidgetFun::getChildWidget(m_pSeatNode,"AnimateNode");
	AnimateNode->removeAllChildren();
	WidgetManager::Instance().createWidget("FeiJiAni",AnimateNode);

	PDKSoundFun::playEffectByGender(GetGender(),PDKSoundFun::EFFECT_FEIJI);
}
