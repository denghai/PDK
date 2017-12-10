#include "ScenceManagerBase.h"
#include "Kernel/kernel/server/IServerItem.h"

#include "Game/GameLib.h"
#include "GameScriptNet.h"

FV_SINGLETON_STORAGEBASE(ScenceManagerBase);

ScenceManagerBase::ScenceManagerBase()
{
	new GameScriptNet();
}
ScenceManagerBase::~ScenceManagerBase()
{

}