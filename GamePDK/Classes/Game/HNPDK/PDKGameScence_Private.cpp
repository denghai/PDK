#include "PDKGameScence.h"
#include "Game/GameLib.h"
#include "PDKPlayer.h"
#include "Game/Game/MissionWeiXin.h"
#include "Game/FV/FvMask.h"
#include "../../Scene/NCScenceManager.h"

void PDKGameScence::initPrivate()
{
//	m_bEnterGetUserInfo = true;
	WidgetScenceXMLparse kScence("GamePDK/xml/GamePrivateScence.xml",this);

    WidgetManager::addButtonCB("PrivateEndPlane_GameExit",this,button_selector(PDKGameScence::NNButton_GameExit));
    WidgetManager::addButtonCB("NNButton_GameExit",this,button_selector(GameBase::Button_ExitGameBase));
	WidgetManager::addButtonCB("NNButton_WeiXinFriend",this,button_selector(PDKGameScence::XZDDButton_WeiXinFriend));
	WidgetManager::addButtonCB("NNButton_PrivateWeiXinShare",this,button_selector(PDKGameScence::XZDDButton_WeiXinImagic));
	WidgetManager::addButtonCB("ButtonPlayerHeadClick",this,button_selector(PDKGameScence::ButtonPlayerHeadClick));
}
void PDKGameScence::defaultPrivateState()
{
    for (int i  = 0;i<MAX_PLAYER;i++)
    {
        PDKPlayer* pPlayer = m_pPlayer[i];
        pPlayer->lScore = 0;
    }
    WidgetFun::setVisible(this,"PrivateDismissPlane",false);
}
void PDKGameScence::OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setVisible(this,"PrivateInfo",true);
	setPrivateInfo(pNetInfo);
	bool bShowDismiss = false;
	if (pNetInfo->dwCreateUserID == m_pLocal->GetUserID())
	{
		if (pNetInfo->bStartGame || pNetInfo->dwPlayCout == 0)
		{
			bShowDismiss = true;
		}
	}
	if (pNetInfo->bStartGame)
	{
		bShowDismiss = true;
	}
    if(FvMask::HasAny(pNetInfo->bGameRuleIdex,_MASK_((dword)GAME_RULE_THREE_GAME)))
    {
        m_plyaercount = 3;
    }
    else
    {
        m_plyaercount = 2;
        int next = (m_pLocal->getIdex() - 1 + 3 ) % 3;
        m_nextPlay = m_pPlayer[next];
    }
    
 	WidgetFun::setVisible(this,"NNButton_WeiXinFriend",!pNetInfo->bStartGame );
 	WidgetFun::setVisible(this,"Button_DismissPrivate",bShowDismiss);
}

void PDKGameScence::setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo)
{
	WidgetFun::setText(this,"RoomID",utility::toString(pNetInfo->dwRoomNum));
	if (pNetInfo->bPlayCoutIdex == 0)
	{
		WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString((int)pNetInfo->dwPlayCout,"/",pNetInfo->dwPlayTotal));
	}
	else
	{
		WidgetFun::setPlaceTextKey(this,"RoomNum","Txt",utility::toString((int)pNetInfo->dwPlayCout,"/",pNetInfo->dwPlayTotal));
	}
    
    m_playNum = pNetInfo->dwPlayCout;
    m_dwPlayTotal = pNetInfo->dwPlayTotal;
    game_rule = pNetInfo->bGameRuleIdex;
    
	std::vector<std::string> kRuleList;
	getGameRuleStr(kRuleList,pNetInfo->bGameRuleIdex);
    
	for (int i=0;i<MAX_GAME_RULE;i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),false);
	}
	for (int i=0;i<(int)kRuleList.size() && i < MAX_GAME_RULE;i++)
	{
		WidgetFun::setVisible(this,utility::toString("RoomGameRule",i),true);
		WidgetFun::setText(this,utility::toString("RoomGameRule",i),kRuleList[i]);
	}
}

void PDKGameScence::OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo)
{
    
    WidgetFun::setVisible(this,"GameResoult2",false);
    
	cocos2d::Node* pRootNode = WidgetFun::getChildWidget(this,"PrivateEndPlane");
    
    WidgetFun::setVisible(this,utility::toString(utility::toString("PrivateEndPlayer",2)),true);
    
    int ii = 0;
	for (int i  = 0;i<MAX_PLAYER;i++)
	{
		PDKPlayer* pPlayer = m_pPlayer[i];
        
		if (!pPlayer || !pPlayer->getUserItem(false))
		{WidgetFun::setVisible(this,utility::toString(utility::toString("PrivateEndPlayer",2)),false);
			continue;
		}
        
		cocos2d::Node* pNode = WidgetFun::getChildWidget(pRootNode,utility::toString(utility::toString("PrivateEndPlayer",ii)));
		WidgetFun::setText(pNode,"Name",pPlayer->GetNickName());
		WidgetFun::setText(pNode,"ID",(int)pPlayer->GetUserID());
        
        for(int j = 0;j<pNetInfo->lChairIDs.size();++j)
        {
            if(pNetInfo->lChairIDs[j] == pPlayer->GetChairID())
            {
                WidgetFun::setText(pNode,"singlesocre",pNetInfo->lGameHighScore[j]);
                WidgetFun::setText(pNode,"boomnum",pNetInfo->lBombCount[j]);
                int win = pNetInfo->lWinNum[j];
                int lose = m_dwPlayTotal - win;
                WidgetFun::setText(pNode,"winlose", utility::toString(win,"/",lose));
                WidgetFun::setText(pNode,"TotalScore",pNetInfo->lPlayerWinLose[j]);
            }
        }
        
    ImagicDownManager::Instance().addDown(WidgetFun::getChildWidget(pNode,"Head"),pPlayer->GetUserInfo()->szHeadHttp,pPlayer->GetUserID());
        ++ii;
	}
	WidgetFun::setVisible(this,"PrivateEndPlane",true);
    
    defaultPrivateState();
}
void PDKGameScence::OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo)
{
	cocos2d::Node* pNode = WidgetFun::getChildWidget(this,"PrivateDismissPlane");
	pNode->setVisible(true);
	if (pNetInfo->dwDissUserCout == 0)
	{
		pNode->runAction(cocos2d::CCVisibleAction::create(3.0f,false));
		return;
	}
	int kChairID[4] = {1,1,1,1};

	bool bShowSelfAction = true;
	int iIdex = 0;
	for (int i = 0;i<(int)pNetInfo->dwDissUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwDissChairID[i]);
		kChairID[pNetInfo->dwDissChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text0",kName);
		iIdex++;
	}
	for (int i = 0;i<(int)pNetInfo->dwNotAgreeUserCout;i++)
	{
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(pNetInfo->dwNotAgreeChairID[i]);
		kChairID[pNetInfo->dwNotAgreeChairID[i]] = 0;
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		if (pPlayer == m_pLocal)
		{
			bShowSelfAction = false;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		WidgetFun::setPlaceTextKey(pNode,utility::toString("DissPrivateState",iIdex),"Text1",kName);
		iIdex++;
	}
	for (int i = 0;i<3;i++)
	{
		if (kChairID[i] == 0)
		{
			continue;
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),"");
		WidgetFun::setText(pNode,utility::toString("DissPrivateState",iIdex),"");
		std::string kName = "";
		GamePlayer* pPlayer = getPlayerByChairID(i);
		if (!pPlayer)
		{
			continue;
		}
		if (pPlayer)
		{
			kName = pPlayer->GetNickName();
		}
		WidgetFun::setText(pNode,utility::toString("DissPrivateName",iIdex),kName);
		iIdex++;
	}
	WidgetFun::setVisible(pNode,"PrivateDismissAction",bShowSelfAction);
}
void PDKGameScence::XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*)
{
    std::string four = "";
    if (FvMask::HasAny(game_rule,_MASK_((dword)GAME_RULE_FOUR_LINE_THREE)))
    {
        four = "炸弹4带3";
    }
   MissionWeiXin::Instance().shareUrlWeiXin(utility::getScriptString("NNWeiXinSharUrl"),utility::getScriptReplaceValue("NNPrivateTxt1",WidgetFun::getText(this,"RoomID"),WidgetFun::getText(this, "RoomGameRule1"),four),utility::getScriptReplaceValue("NNPrivateTxt1",WidgetFun::getText(this,"RoomID"),WidgetFun::getText(this, "RoomGameRule1"),four));
}
void PDKGameScence::XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*)
{
	static PDKGameScence* pTemp = NULL;
	pTemp = this;
	cocos2d::utils::captureScreen([](bool b, std::string name){
		if (b)
		{
			MissionWeiXin::Instance().shareScreenWeiXin(name);
			pTemp->Button_ExitGameBase(NULL,NULL);
			pTemp->ExitGame();
			WidgetFun::setVisible(pTemp,"PrivateEndPlane",false); 
		}
	},"screenshot.png");

}
void PDKGameScence::NNButton_GameExit(cocos2d::Ref*,WidgetUserInfo*)
{
    for (int i  = 0;i<MAX_PLAYER;i++)
    {
        PDKPlayer* pPlayer = m_pPlayer[i];
        pPlayer->PlayerLeave();
    }
    WidgetFun::setVisible(this, "PrivateEndPlane",false);
    NCScenceManager::Instance().InHomeScence();
}

void PDKGameScence::ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	GamePlayer* pPlayer = getBasePlayerByUserID(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"UserID")));
	if (!pPlayer)
	{
		return;
	}
	PopScence::Instance().showIPInfo(pPlayer->GetNickName(),
		utility::paseInt(pPlayer->GetUserID(),6),
		pPlayer->GetUserInfo()->szLogonIP,
		pPlayer->GetUserInfo()->szHeadHttp);
}

void PDKGameScence::pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow)
{
	if (FvMask::HasAny(dwGameRule,_MASK_((dword)nRuleTag))&&bShow)
	{
		kRuleList.push_back(ScriptData<std::string>(utility::toString("PDKGameRule",(int)nRuleTag)).Value());
	}
}

void PDKGameScence::getGameRuleStr(std::vector<std::string>& kRuleList,dword	dwGameRuleIdex)
{
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_RULE_16ZHANG,			!HaveOptionRule(GAME_RULE_16ZHANG));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_RULE_15ZHANG,			!HaveOptionRule(GAME_RULE_15ZHANG));
    pushGameRule(kRuleList,dwGameRuleIdex,GAME_RULE_TWO_GAME,			!HaveOptionRule(GAME_RULE_TWO_GAME));
    pushGameRule(kRuleList,dwGameRuleIdex,GAME_RULE_THREE_GAME,			!HaveOptionRule(GAME_RULE_THREE_GAME));
	pushGameRule(kRuleList,dwGameRuleIdex,GAME_RULE_SHOUCHU_HEITAO3,		!HaveOptionRule(GAME_RULE_SHOUCHU_HEITAO3));
    pushGameRule(kRuleList,dwGameRuleIdex,GAME_RULE_FOUR_LINE_THREE,		!HaveOptionRule(GAME_RULE_FOUR_LINE_THREE));
}
