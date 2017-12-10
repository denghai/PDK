
#include "PDKGameScence.h"
#include "Game/GameLib.h"
#include "CMD_PDK.h"
#include "PDKPlayer.h"

void PDKGameScence::initNet()
{
	addNetCBDefine(SUB_S_GAME_START,this,PDKGameScence::OnSubGameStart);
	addNetCBDefine(SUB_S_OUT_CARD,this,PDKGameScence::OnSubOutCard);
	addNetCBDefine(SUB_S_PASS_CARD,this,PDKGameScence::OnSubPassCard);
	addNetCBDefine(SUB_S_GAME_END,this,PDKGameScence::OnSubGameEnd);
}
//游戏开始
void PDKGameScence::OnSubGameStart(void * pBuffer, int wDataSize)
{
	datastream kStream(pBuffer,wDataSize);
	PDK_CMD_S_GameStart kGameStart;
	kGameStart.StreamValue(kStream,false);
	for (int i=0;i<MAX_PLAYER;i++)
	{
		PDKPlayer* pPlayer = getPlayerByChairID(i);
		if (pPlayer == m_pLocal && i == kGameStart.wChairID)
		{
			pPlayer->setHandCard(kGameStart.cbHandCardList);
			pPlayer->showHandCard();
		}
		else
		{
			pPlayer->showHandCardBack(kGameStart.cbHandCardList.size());
		}
	}
    
    for(int i = 0;i<MAX_PLAYER;++i)
    {
        PDKPlayer* pPlayer = getPlayerByChairID(i);
        pPlayer->startGame(kGameStart.wlGameScores[i]);
        pPlayer->showBaoDan(false);
    }
	resetLastOutInfo();
    if(getPlayerByChairID(kGameStart.wCurrentUser) == m_pLocal)
    {
        WidgetFun::setButtonEnabled(this,"PDKButton_OutCard",false);
    }
	setCurrentPlayer(kGameStart.wCurrentUser);
}

//出牌消息
void PDKGameScence::OnSubOutCard(void * pBuffer, int wDataSize)
{
	datastream kStream(pBuffer,wDataSize);
	PDK_CMD_S_OutCard kOutCard;
	kOutCard.StreamValue(kStream,false);

	PDKPlayer* pPlayer = getPlayerByChairID(kOutCard.wOutCardUser);
	if (pPlayer)
	{
		pPlayer->clearOutState();
        pPlayer->showOutCard(kOutCard.cbOutCardList);
	}

	m_nLastOutUser = kOutCard.wOutCardUser;
	m_nLastOutType = kOutCard.cbCardType;
	m_kLastOutCardList = kOutCard.cbOutCardList;
    
    if(pPlayer == m_pLocal  && kOutCard.wOutCardUser == m_pLocal->GetChairID() && kOutCard.wCurrentUser == m_pLocal->GetChairID())
    {
        m_kLastOutCardList.clear();
        m_nLastOutType = 0;
    }
    
	if (kOutCard.wCurrentUser == m_pLocal->GetChairID() && !onTishi())
	{
        m_pLocal->clearOutState();
        m_pLocal->showMeiYouPai();
        TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(PDKGameScence::SendPass,this),2.f);
        return;
	}
    
    WidgetFun::setButtonEnabled(this,"PDKButton_OutCard",true);
	setCurrentPlayer(kOutCard.wCurrentUser);
}

void PDKGameScence::OnSubPassCard(void * pBuffer, int wDataSize)
{
	datastream kStream(pBuffer,wDataSize);
	PDK_CMD_S_PassCard kPassCard;
	kPassCard.StreamValue(kStream,false);

	PDKPlayer* pPlayer = getPlayerByChairID(kPassCard.wPassUser);
	if (pPlayer)
	{
		pPlayer->clearOutState();
		pPlayer->showPass(true);
	}
	if (kPassCard.bNewTurn)
	{
		for (int i=0;i<MAX_PLAYER;i++)
		{
			PDKPlayer* pPlayer = getPlayerByChairID(i);
			pPlayer->clearOutState();
		}
		resetLastOutInfo();
        WidgetFun::setVisible(this,"ActionNode",false);
	}
    
    if(pPlayer == m_pLocal  && kPassCard.wCurrentUser == m_pLocal->GetChairID())
    {
        m_kLastOutCardList.clear();
        m_nLastOutType = 0;
    }
    
    else if(!kPassCard.bNewTurn &&kPassCard.wCurrentUser == m_pLocal->GetChairID() && !onTishi())
    {
        m_pLocal->clearOutState();
        m_pLocal->showMeiYouPai();
        TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(PDKGameScence::SendPass,this),2.f);
        return;
	}
    
//    WidgetFun::setButtonEnabled(this,"PDKButton_OutCard",false);
    setCurrentPlayer(kPassCard.wCurrentUser);
}

//游戏结束
void PDKGameScence::OnSubGameEnd(void * pBuffer, int wDataSize)
{
	datastream kStream(pBuffer,wDataSize);
	PDK_CMD_S_GameEnd kGameEnd;
	kGameEnd.StreamValue(kStream,false);

	cocos2d::ListViewEx* pEndList = WidgetFun::getListViewWidget(this,"EndInfoListView");
	pEndList->removeAllChildren();
	
	int nSelfScore =0;
    
    int length = kGameEnd.kEndInfoList.size()>0?m_plyaercount:0;
    
	for (int i=0;i<length;i++)
	{
		cocos2d::Node* pEndInfo = WidgetManager::Instance().createWidget("PDKGameResoultNode",pEndList);
		PDKPlayer* pPlayer = getPlayerByChairID(kGameEnd.kEndInfoList[i].wChairID);
		WidgetFun::setText(pEndInfo,"Name",pPlayer->GetNickName());
		WidgetFun::setText(pEndInfo,"LeftNum",(int)kGameEnd.kEndInfoList[i].cbCardList.size());
		WidgetFun::setText(pEndInfo,"BombNum",(int)kGameEnd.kEndInfoList[i].nBombCount);
        
		int nGoldNum = (int)(kGameEnd.kEndInfoList[i].lGameScore);
        
		if (nGoldNum >=0 )
		{
			WidgetFun::setText(pEndInfo,"GoldTxt",utility::toString("+",nGoldNum));
		}
		else
		{
			WidgetFun::setText(pEndInfo,"GoldTxt",utility::toString(nGoldNum));
		}
		if (pPlayer == m_pLocal)
		{
			nSelfScore = nGoldNum;
		}
		else
		{
            if(kGameEnd.kEndInfoList[i].cbCardList.size() > 0)
            {
                pPlayer->showOutCard(kGameEnd.kEndInfoList[i].cbCardList,true);
            }
		}
	}
	pEndList->forceDoLayout();

	WidgetFun::setVisible(this,"WinTitle",nSelfScore>0);
	WidgetFun::setVisible(this,"LoseTitle",nSelfScore<=0);
    
    TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(PDKGameScence::showEndInfo,this),2.f);
}

void PDKGameScence::showEndInfo()
{
    if(!WidgetFun::isWidgetVisble(this, "PrivateEndPlane"))
    {
        WidgetFun::setVisible(this,"GameResoult2",true);
    }
    
}
