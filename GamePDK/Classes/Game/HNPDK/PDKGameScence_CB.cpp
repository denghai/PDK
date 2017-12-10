#include "PDKGameScence.h"
#include "Game/GameLib.h"

#include "CMD_PDK.h"
#include "PDKPlayer.h"
#include "PDKGameLogic.h"

bool PDKGameScence::OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize)
{
	if (cbGameStatus == GS_TK_FREE)
	{
		OnFreeScence(data,dataSize);
	}
	if (cbGameStatus == GS_TK_PLAYING)
	{
		OnPlayScence(data,dataSize);
	}
	return true;
}
void PDKGameScence::OnFreeScence(void* data, int wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(PDK_CMD_S_StatusFree));
	if (wDataSize!=sizeof(PDK_CMD_S_StatusFree)) return;

	PDK_CMD_S_StatusFree* pNetInfo = (PDK_CMD_S_StatusFree*)data;
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() == US_FREE);
}
void PDKGameScence::OnPlayScence(void* data, int wDataSize)
{
	//效验数据
	datastream kStream(data,wDataSize);
	PDK_CMD_S_StatusPlay StatusPlay;
	StatusPlay.StreamValue(kStream,false);
	defaultState();

	m_nCurrentUser = StatusPlay.wCurrentUser;

	for (int i=0;i<(int)StatusPlay.kHandCardInfo.size();i++)
	{
		PDK_CardListInfo kInfo = StatusPlay.kHandCardInfo[i];

		PDKPlayer* pPlayer = getPlayerByChairID(kInfo.wChairID);
		if (pPlayer == m_pLocal)
		{
			pPlayer->setHandCard(kInfo.cbCardList);
			pPlayer->showHandCard();
		}
		else
		{
			pPlayer->showHandCardBack(kInfo.cbCardList.size());
		}
        
        //初始分
        if(StatusPlay.wlGameScores.size() > 0)
        {
            pPlayer->startGame(StatusPlay.wlGameScores[i]);
        }
        else
        {
            pPlayer->startGame(0);
        }
	}

	m_nLastOutUser = StatusPlay.wLastOutUser;
	m_kLastOutCardList = StatusPlay.cbLastOutCardList;
    
    PDKPlayer* pPlayer = getPlayerByChairID(StatusPlay.wLastOutUser);
    if (pPlayer)
    {
        pPlayer->clearOutState();
        pPlayer->showOutCard(StatusPlay.cbLastOutCardList,false,true);
    }
    
    if(pPlayer == m_pLocal  && StatusPlay.wCurrentUser == m_pLocal->GetChairID())
    {
        m_kLastOutCardList.clear();
    }
    
	m_nLastOutType = PDKGameLogic::Instance().GetCardType(StatusPlay.cbLastOutCardList);

	if ( StatusPlay.wLastOutUser != m_pLocal->GetChairID() && StatusPlay.wCurrentUser == m_pLocal->GetChairID() && !onTishi())
	{
        
        m_pLocal->clearOutState();
        m_pLocal->showMeiYouPai();
        TimeManager::Instance().addCerterTimeCB(TIME_CALLBACK(PDKGameScence::SendPass,this),2.f);
            return;
		
	}
	setCurrentPlayer(m_nCurrentUser);
}
void PDKGameScence::OnGFGameClose(int iExitCode)
{
	ExitGame();
}
void PDKGameScence::OnEventUserStatus(GamePlayer * pPlayer)
{
	WidgetFun::setVisible(this,"FreeStateNode",m_pLocal->GetUserStatus() < US_READY);
}

void PDKGameScence::SendOutCard(std::vector<BYTE> kCardList)
{
	PDK_CMD_C_OutCard kOutCard;
	kOutCard.cbOutCardList = kCardList;

	datastream kDatastream;
	kOutCard.StreamValue(kDatastream,true);
	SendSocketData(SUB_C_OutCard,&kDatastream[0],kDatastream.size());
}

void PDKGameScence::SendPass()
{
	SendSocketData(SUB_C_PASS_CARD,0,0);
}
