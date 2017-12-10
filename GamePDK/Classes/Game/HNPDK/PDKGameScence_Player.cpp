#include "PDKGameScence.h"
#include "Game/GameLib.h"
#include "PDKPlayer.h"

PDKPlayer* PDKGameScence::getPlayerByChairID(int iChairID)
{
	if (INVALID_CHAIR == iChairID)
	{
		return NULL;
	}
	if (!m_pLocal->getUserItem(false))
	{
		return m_pLocal;
	}
    
	int iIdex = (m_pLocal->GetChairID()-iChairID+MAX_PLAYER)%MAX_PLAYER;
    
	return m_pPlayer[iIdex];
}

GamePlayer* PDKGameScence::CreatePlayer( IClientUserItem * pIClientUserItem)
{
	if (pIClientUserItem->GetUserID() == UserInfo::Instance().getUserID())
	{
		m_pLocal->setUserItem(pIClientUserItem);
		return m_pLocal;
	}
	else
	{
		GamePlayer* pPlayer = getPlayerByChairID(pIClientUserItem->GetChairID());
		pPlayer->setUserItem(pIClientUserItem);
		return pPlayer;
	}
	return NULL;
}

void PDKGameScence::DeletePlayer(GamePlayer* pPlayer)
{
    utility::log("delete plyaer");
}

void PDKGameScence::upSelfPlayerInfo()
{
}

void PDKGameScence::LocalPlayerEnter()
{
	GameBase::LocalPlayerEnter();
}
