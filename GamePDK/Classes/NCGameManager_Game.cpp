#include "NCGameManager.h"

#include "Scene/NCScenceManager.h"
#include "Game/HNPDK/PDKGameScence.h"

void NCGameManager::initGame()
{
	new PDKGameScence();
	NCScenceManager::Instance().addGameNode(PDKGameScence::pInstance());
}
void NCGameManager::StartXZDD()
{
}

void NCGameManager::StartXZDDRecord(datastream& kDataStream)
{
//    NCScenceManager::Instance().HideAll(NULL);
//    PDKGameScence::Instance().EnterScence();
//    if (!PDKGameScence::Instance().StartRecord(kDataStream))
//    {
//        NCScenceManager::Instance().InHomeScence();
//        return;
//    }
}

void NCGameManager::HideAll()
{
	PDKGameScence::Instance().HideAll();
}

IClientKernelSink* NCGameManager::CreateGame(word wKindID)
{
	NCScenceManager::Instance().HideAll(NULL);
	if (PDKGameScence::KIND_ID == wKindID)
	{
		PDKGameScence::Instance().EnterScence();
        
		return PDKGameScence::pInstance();
	}
	return NULL;
}
void NCGameManager::loadGameBaseData(word wKindID)
{
	if (PDKGameScence::KIND_ID == wKindID)
	{
		DF::shared()->init(wKindID,PDKGameScence::MAX_PLAYER,PDKGameScence::VERSION_CLIENT,"Game");
	}
	
}
