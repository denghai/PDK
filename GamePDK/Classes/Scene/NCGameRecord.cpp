#include "NCGameRecord.h"
#include "NCScenceManager.h"
#include "Game/GameLib.h"
#include "../NCGameManager.h"
#include "../Game/HNPDK/PDKGameLogic.h"

FV_SINGLETON_STORAGE(NCGameRecord);

NCGameRecord::NCGameRecord()
	:m_kGameRecordMission(ScriptData<std::string>("address").Value().c_str(),ScriptData<int>("Port").Value())
{
	init();
	m_kGameRecordMission.setMissionSink(this);
}
NCGameRecord::~NCGameRecord()
{

}
bool NCGameRecord::init()
{
	if (!cocos2d::CCNode::init())
	{
		return false;
	};
	WidgetScenceXMLparse kScence1("Script/HNGameRecordScence.xml",this);
	WidgetManager::addButtonCB("Button_zhanji",this,button_selector(NCGameRecord::Button_zhanji));
	WidgetManager::addButtonCB("Button_ShowTotal",this,button_selector(NCGameRecord::Button_ShowTotal));
	WidgetManager::addButtonCB("Button_GameRecordBack",this,button_selector(NCGameRecord::Button_GameRecordBack));
	WidgetManager::addButtonCB("Button_StartGameRecord",this,button_selector(NCGameRecord::Button_StartGameRecord));
	
	return true;
}
void NCGameRecord::EnterScence()
{
	//m_kGameRecordMission.GetGameRecordList(0);
	//m_kGameRecordMission.GetPrivateRandTotalRecord(UserInfo::Instance().getUserID());
}
void NCGameRecord::hideAll()
{
	setVisible(false);
	WidgetFun::setVisible(this,"RecordListPlane",false);
	WidgetFun::setVisible(this,"RecordTotalPlane",false);
}
std::string NCGameRecord::timeToString(systemtime kSystem)
{
	std::string kStr;
	kStr = utility::toString(kStr,kSystem.wYear);
	kStr = utility::toString(kStr,"-");
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wMonth,2));
	kStr = utility::toString(kStr,"-");
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wDay,2));
	kStr = utility::toString(kStr,'\n');
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wHour,2));
	kStr = utility::toString(kStr,":");
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wMinute,2));
	kStr = utility::toString(kStr,":");
	kStr = utility::toString(kStr,utility::paseInt(kSystem.wSecond,2));
	return kStr;
}
std::string NCGameRecord::scoreToString(int iScore)
{
	if (iScore > 0)
	{
		return utility::toString("+",iScore);
	}
	return utility::toString(iScore);
}
void NCGameRecord::onGPBackGameRecordList(tagPrivateRandTotalRecordList* pNetInfo)
{
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(this,"RecordList0");
	pList->removeAllChildren();
	for (int i = 0;i<(int)pNetInfo->kList.size();i++)
	{
		tagPrivateRandTotalRecord& kInfo = pNetInfo->kList[i];
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("GameReocrdListItem",pList);
		WidgetFun::setWidgetUserInfo(pItemNode,"Button_ShowTotal","RecordID",utility::toString(kInfo.iRecordID));
		WidgetFun::setText(pItemNode,"PlayTime",timeToString(kInfo.kPlayTime));
		WidgetFun::setText(pItemNode,"RoomNum",utility::toString(kInfo.iRoomNum));
		if (kInfo.kNickName.size() != kInfo.kScore.size())
		{
			continue;
		}
		for (int m = 0;m<(int)kInfo.kNickName.size();m++)
		{
			if (kInfo.kUserID[m] == UserInfo::Instance().getUserID())
			{
				if (kInfo.kScore[m] >= 0)
				{
					WidgetFun::setVisible(pItemNode,"WinImagic",true);
					WidgetFun::setVisible(pItemNode,"LoseImagic",false);
				}
				else
				{
					WidgetFun::setVisible(pItemNode,"WinImagic",false);
					WidgetFun::setVisible(pItemNode,"LoseImagic",true);
				}
			}
		}
		for (int m = 0;m<(int)kInfo.kNickName.size();m++)
		{
			if (m >= 4)
			{
				continue;
			}
			WidgetFun::setText(pItemNode,utility::toString("PlayerTxt",m),
				utility::toString(utility::a_u8(kInfo.kNickName[m]),":",scoreToString(kInfo.kScore[m])));
		}

	}
	pList->forceDoLayout();
}
void NCGameRecord::onGPBackGameTotalRecord(tagPrivateRandTotalRecord* pNetInfo)
{
	m_kPrivateRandTotalRecord = *pNetInfo;
	cocos2d::Node* pRootPlane = WidgetFun::getChildWidget(this,"RecordTotalPlane");
	cocos2d::ListViewEx* pList = WidgetFun::getListViewWidget(pRootPlane,"RecordList1");
	pList->removeAllChildren();
	for (int i = 0;i<(int)pNetInfo->kNickName.size();i++)
	{
		if (i < 4)
		{
			WidgetFun::setText(pRootPlane,utility::toString("PlayerName",i),utility::a_u8(pNetInfo->kNickName[i]));
		}
	}
	for (int i = 0;i<(int)pNetInfo->kRecordChild.size();i++)
	{
		tagPrivateRandRecordChild& kInfo = pNetInfo->kRecordChild[i];
		cocos2d::Node* pItemNode = WidgetManager::Instance().createWidget("GameReocrdTotalItem",pList);
		WidgetFun::setText(pItemNode,"IdexTxt",utility::toString(i+1));
		WidgetFun::setText(pItemNode,"PlayTime",timeToString(kInfo.kPlayTime));
		WidgetFun::setWidgetUserInfo(pItemNode,"Button_StartGameRecord","RecordChildID",utility::toString(i));
		for (int m = 0;m<(int)kInfo.kScore.size();m++)
		{
			if (m >= 4)
			{
				continue;
			}
			WidgetFun::setText(pItemNode,utility::toString("PlayerTxt",m),scoreToString(kInfo.kScore[m]));
		}
	}
	pList->forceDoLayout();
}

void NCGameRecord::Button_zhanji(cocos2d::Ref*,WidgetUserInfo*)
{
	setVisible(true);
	WidgetFun::setVisible(this,"RecordListPlane",true);
	m_kGameRecordMission.GetGameRecordList(UserInfo::Instance().getUserID());
}

void NCGameRecord::Button_ShowTotal(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	WidgetFun::setVisible(this,"RecordTotalPlane",true);
	WidgetFun::setVisible(this,"RecordListPlane",false);
	m_kGameRecordMission.GetPrivateRandTotalRecord(utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RecordID")));
}

void NCGameRecord::Button_GameRecordBack(cocos2d::Ref*,WidgetUserInfo*)
{
	if (WidgetFun::isWidgetVisble(this,"RecordTotalPlane"))
	{
		WidgetFun::setVisible(this,"RecordTotalPlane",false);
		WidgetFun::setVisible(this,"RecordListPlane",true);
	}
	else
	{
		NCScenceManager::Instance().GameBackScence();
	}
}

void NCGameRecord::Button_StartGameRecord(cocos2d::Ref*,WidgetUserInfo* pUserInfo)
{
	int index = utility::parseInt(WidgetFun::getUserInfoValue(pUserInfo,"RecordChildID"));
    
    tagPrivateRandRecordChild kInfo = m_kPrivateRandTotalRecord.kRecordChild[index];
    
    for(int i=0; i<kInfo.kHandCardCost.size(); ++i)
    {
        showOutCard(kInfo.kHandCardCost[i],i,kInfo.kNickName[i]);
    }
    
    WidgetFun::setVisible(this, "showCardPlane",true);
}

void NCGameRecord::showOutCard(std::vector<BYTE> kOutCard,int m_iIdex,std::string nickName)
{
    
    cocos2d::Node* pNode = WidgetFun::getChildWidget(this,utility::toString(utility::toString("Card",m_iIdex)));
    
    if(!pNode)
        return;
    
    WidgetFun::setText(pNode,"userName", utility::a_u8(nickName));
    PDKGameLogic::Instance().SortCardList(kOutCard);
    cocos2d::Node* pOutCardNode = WidgetFun::getChildWidget(pNode,"CardNodeOut");
    pOutCardNode->removeAllChildren();
    pOutCardNode->setVisible(true);
    int nOderAdd = utility::parseInt(WidgetFun::getWidgetUserInfo(pOutCardNode,"OderAdd"));
    int nAddPosX = utility::parseInt(WidgetFun::getWidgetUserInfo(pOutCardNode,"AddPosX"));
    int nOutToPosY = utility::parseInt(WidgetFun::getWidgetUserInfo(pOutCardNode,"OutToPosY"));
    int nCardWidth = utility::parseInt(WidgetFun::getWidgetUserInfo(pOutCardNode,"CardWidth"));
    float nCardScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pOutCardNode,"CardScale"));
    int nOder = 0;
    
    cocos2d::Vec2 startPos = cocos2d::Vec2(0,0);
    int nCardNum = kOutCard.size();
    
    startPos.x = -((nCardNum-1)*nAddPosX)/2;
    startPos.y = nOutToPosY;
    
    cocos2d::Vec2 startPosTemp = startPos;
    
    cocos2d::Node* pHandNode = WidgetFun::getChildWidget(pNode,"CardNodeHand");
    for (int i=0;i<(int)kOutCard.size();i++)
    {
        cocos2d::Node* pCard = WidgetManager::Instance().createWidget("PDK_PlayerCard",pOutCardNode);
        pCard->setLocalZOrder(nOder);
        pCard->setPosition(startPosTemp);
        pCard->setScale(nCardScale);
        startPosTemp.x += nAddPosX;
        nOder += nOderAdd;
        
        setCardImgic(pCard,kOutCard[i]);
        
        pCard->setVisible(true);
        for (int j = 0;j<pHandNode->getChildrenCount();j++)
        {
            cocos2d::Node* pCardNode = pHandNode->getChildren().at(j);
            if (pCardNode == NULL)
            {
                continue;
            }
            int nCardData = utility::parseInt(WidgetFun::getWidgetUserInfo(pCardNode,"CardData"));
            if (nCardData == (int)kOutCard[i] )
            {
                int nPosX = pCard->getPositionX();
                int nPosY = pCard->getPositionY();
                WidgetFun::setWidgetUserInfo(pCardNode,"OutToPos",utility::toString(nPosX," ",nPosY));
                WidgetFun::runWidgetAction(pCardNode,"OutCardAni");
                WidgetFun::runWidgetAction(pCard,"OutAction");
                break;
            }
        }
    }
    pOutCardNode->setVisible(true);
}

void NCGameRecord::setCardImgic(cocos2d::Node* pCard,int nCardData)
{
    WidgetFun::setWidgetUserInfo(pCard,"CardData",utility::toString(nCardData));
    
    std::string kImgPath = WidgetFun::getWidgetUserInfo(pCard,"CardImgPath");
    if (nCardData <=0)
    {
        WidgetFun::setVisible(pCard,"CardShow",false);
        WidgetFun::setVisible(pCard,"CardBack",true);
        return;
    }
    WidgetFun::setVisible(pCard,"CardShow",true);
    WidgetFun::setVisible(pCard,"CardBack",false);
    
    int nValue = PDKGameLogic::Instance().GetCardValue(nCardData);
    int nColor = PDKGameLogic::Instance().GetCardColor(nCardData);
    
    ASSERT(nValue>=3&& nValue<=15);
    ASSERT(nColor>=0&& nColor<=3);
    
    std::string kColorTemp;
    if (nColor%2==0)
    {
        kColorTemp = "red_";
    }
    else
    {
        kColorTemp = "black_";
    }
    std::string kNumPath = utility::toString(kImgPath,kColorTemp,nValue,"_big",".png");
    std::string kColorPath = utility::toString(kImgPath,"color_",nColor,".png");
    
    WidgetFun::setImagic(pCard,"Card_Number1",kNumPath,false);
    WidgetFun::setImagic(pCard,"Card_Number2",kNumPath,false);
    WidgetFun::setImagic(pCard,"Card_Color1",kColorPath,false);
    WidgetFun::setImagic(pCard,"Card_Color2",kColorPath,false);
    
    WidgetFun::setVisible(pCard,"Card_BigImg",false);
    
    std::string kBigImgPath;
    if (nValue>=11 && nValue<=13)
    {
        kBigImgPath = utility::toString(kImgPath,kColorTemp,"p",nValue,"_big.png");
        WidgetFun::setImagic(pCard,"Card_BigImg",kBigImgPath,false);
        WidgetFun::setVisible(pCard,"Card_BigImg",true);
    }
}
