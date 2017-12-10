#include "PDKPlayer.h"
#include "PDKGameLogic.h"
#include "PDKGameScence.h"
#include "Game/FV/FvMask.h"

#define BANKER_CARD             0x33 //黑桃三

void PDKPlayer::setHandCard(std::vector<BYTE> kHandCard)
{
	m_kHandCardList = kHandCard;
	m_nHandCardNum = kHandCard.size();
}
void PDKPlayer::showHandCardBack(int nCardNum)
{
	m_nHandCardNum = nCardNum;
    
    if(nCardNum == 1)
    {
        showBaoDan(true);
    }
    
	if (m_iIdex == 0)
	{
		return;
	}
    
	WidgetFun::setVisible(m_pSeatNode,"CardNodeHand",true);
	WidgetFun::setText(m_pSeatNode,"HandCardNum",nCardNum);
    
}

void PDKPlayer::showHandCard()
{
	PDKGameLogic::Instance().SortCardList(m_kHandCardList);
	showHandCard(m_kHandCardList);
}
void PDKPlayer::showHandCard(const std::vector<BYTE>& kHandCard)
{
	if (m_iIdex != 0)
	{
		return;
	}
	cocos2d::Node* pHandCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHand");
	pHandCardNode->removeAllChildren();
	pHandCardNode->setVisible(true);

	int nOderAdd = utility::parseInt(WidgetFun::getWidgetUserInfo(pHandCardNode,"OderAdd"));
	int nAddPosX = utility::parseInt(WidgetFun::getWidgetUserInfo(pHandCardNode,"AddPosX"));
	int nCardWidth = utility::parseInt(WidgetFun::getWidgetUserInfo(pHandCardNode,"CardWidth"));
	int nOder = 0;

	cocos2d::Vec2 startPos = cocos2d::Vec2(0,0);
	int nCardNum = m_kHandCardList.size();
	startPos.x =  -((nCardNum-1)*nAddPosX)/2;

	for (int i=0;i<(int)m_kHandCardList.size();i++)
	{
		cocos2d::Node* pCard = WidgetManager::Instance().createWidget("PDK_PlayerCard",pHandCardNode);
		pCard->setLocalZOrder(nOder);
		pCard->setPosition(startPos);
		startPos.x += nAddPosX;
		nOder += nOderAdd;

		setCardImgic(pCard,m_kHandCardList[i]);
	}
}

void PDKPlayer::showCard()
{
    showCard(&m_kHandCardList[0],m_nHandCardNum);
}

void PDKPlayer::showCard(BYTE* cbCardData,BYTE cbCardCount)
{
    BYTE cbIdex = 0;
    BYTE cbChiHuCard = 0;
    
    cocos2d::Node* pRootNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNode1");
    for (int i = 0;i<pRootNode->getChildrenCount();i++)
    {
        cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(pRootNode->getChildren().at(i));
        if (pSprite == NULL || pSprite->getTag() != 1)
        {
            continue;
        }
        BYTE iCardValue = *(cbCardData+cbIdex);
        float fHuScale = utility::parseFloat(WidgetFun::getWidgetUserInfo(pSprite,"HuScale"));
        pSprite->setScale(fHuScale);
        std::string kImagic = WidgetFun::getWidgetUserInfo(pSprite,"MingImagic");
        if (cbIdex < cbCardCount)
        {
            setCardImagic(pSprite,iCardValue,kImagic);
        }
        else
        {
            setCardImagic(pSprite,0,kImagic);
        }
        cbIdex++;
    }
}

void PDKPlayer::setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront)
{
    if (kValue > 0)
    {
        BYTE cbValue= PDKGameLogic::Instance().GetCardValue(kValue);
        BYTE cbColor= PDKGameLogic::Instance().GetCardColor(kValue);
        WidgetFun::setImagic(pNode,utility::toString(kImagicFront,(int)cbColor,(int)cbValue,".png"),false);
    }
    else
    {
        WidgetFun::setImagic(pNode,utility::toString(kImagicFront,0,0,".png"),false);
    }
}

void PDKPlayer::showOutCard(std::vector<BYTE> kOutCard,bool isEnd ,bool isReconnet)
{
	if (!isEnd && !isReconnet)
	{
		runAniByCards(kOutCard);
	}
	showClock(false,0);

	PDKGameLogic::Instance().SortCardList(kOutCard);

	cocos2d::Node* pOutCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeOut");
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

	if (m_iIdex == 0)
	{
		startPos.x = -((nCardNum-1)*nAddPosX)/2;
		startPos.y = nOutToPosY;
	}
	else if (m_iIdex == 1)
	{
		int iNum = nCardNum;
		if (iNum>8)
		{
			iNum=8;
		}
		startPos.x = -(iNum-1)*nAddPosX;
	}
	else if (m_iIdex == 2)
	{
		startPos.x = 0;
	}

	cocos2d::Vec2 startPosTemp = startPos;

	cocos2d::Node* pHandNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHand");
	for (int i=0;i<(int)kOutCard.size();i++)
	{
		cocos2d::Node* pCard = WidgetManager::Instance().createWidget("PDK_PlayerCard",pOutCardNode);
		pCard->setLocalZOrder(nOder);
		pCard->setPosition(startPosTemp);
		pCard->setScale(nCardScale);
		startPosTemp.x += nAddPosX;
		nOder += nOderAdd;

		setCardImgic(pCard,kOutCard[i]);

		if (m_iIdex !=0 )
		{
			if (i==7)
			{
				startPosTemp = startPos;
				startPosTemp.y = -80;
			}

			continue;
		}
		if (isReconnet)
		{
			continue;
		}
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
		//删除所出牌数据
		std::vector<BYTE>::iterator iter = m_kHandCardList.begin();
		while (iter != m_kHandCardList.end())
		{
			if (kOutCard[i] == *iter)
			{
				iter = m_kHandCardList.erase(iter);
				break;
			}
			else
			{
				iter++;
			}
		}
	}
    
	if (m_iIdex !=0)
	{
        //如果是重连则牌的数量已经是最新的。不需要减去出牌数
        int nCardCount  = isReconnet?m_nHandCardNum : m_nHandCardNum - kOutCard.size();
		showHandCardBack(nCardCount);
		if (!isReconnet)
		{
			WidgetFun::runWidgetAction(pOutCardNode,"OutCardAni");
		}
		else
		{
			pOutCardNode->setVisible(true);
		}
	}
	else
	{
		if (!isReconnet)
			WidgetFun::runWidgetAction(pHandNode,"Start");
	}
}

void PDKPlayer::setCardImgic(cocos2d::Node* pCard,int nCardData)
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

void PDKPlayer::clearHandCard()
{
	cocos2d::Node* pHandCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHand");
	if (m_iIdex  == 0)
	{
		pHandCardNode->removeAllChildren();
		m_kHandCardList.clear();
	}
	pHandCardNode->setVisible(false);
}

void PDKPlayer::clearOutCard()
{
	cocos2d::Node* pOutCardNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeOut");
	pOutCardNode->removeAllChildren();
	pOutCardNode->setVisible(false);
}

void PDKPlayer::clearTouchList()
{
	m_kTouchCardList.clear();
}


cocos2d::Node* PDKPlayer::getTouchCardNode(const cocos2d::Vec2& kPos)
{
	cocos2d::Node* pHandNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHand");
	int nAddPosX = utility::parseInt(WidgetFun::getWidgetUserInfo(pHandNode,"AddPosX"));

	for (int i = 0;i<pHandNode->getChildrenCount();i++)
	{
		cocos2d::Node* pCardNode = pHandNode->getChildren().at(i);
		if (pCardNode == NULL)
		{
			continue;
		}
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(WidgetFun::getChildWidget(pCardNode,"CardShow"));
		if (pSprite == NULL)
		{
			continue;
		}
		if (isTouchCard(pSprite,kPos,nAddPosX,i == pHandNode->getChildrenCount()-1))
		{
			int nCardData = utility::parseInt(WidgetFun::getWidgetUserInfo(pCardNode,"CardData"));
			insertTouchCard(nCardData);
			blackCard(pSprite,true);
			
			return pCardNode;
		}
	}
	return NULL;

}

void PDKPlayer::touchCardMove(const cocos2d::Vec2& kPos)
{
	cocos2d::Node* pHandNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHand");
	int nAddPosX = utility::parseInt(WidgetFun::getWidgetUserInfo(pHandNode,"AddPosX"));
	for (int i = 0;i<pHandNode->getChildrenCount();i++)
	{
		cocos2d::Node* pCardNode = pHandNode->getChildren().at(i);
		if (pCardNode == NULL)
		{
			continue;
		}
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(WidgetFun::getChildWidget(pCardNode,"CardShow"));
		if (pSprite == NULL)
		{
			continue;
		}
		if (isTouchCard(pSprite,kPos,nAddPosX,i == pHandNode->getChildrenCount()-1))
		{
			int nCardData = utility::parseInt(WidgetFun::getWidgetUserInfo(pCardNode,"CardData"));
			insertTouchCard(nCardData);
			blackCard(pSprite,true);
		}
	}
}
void PDKPlayer::touchCardEnd(const cocos2d::Vec2& kPos)
{

}

bool PDKPlayer::insertTouchCard(BYTE cbCard)
{
	std::vector<BYTE>::iterator iter = m_kTouchCardList.begin();

	while (iter != m_kTouchCardList.end())
	{
		if (cbCard == *iter)
		{
			return false;
		}
		else
		{
			iter++;
		}
	}
	m_kTouchCardList.push_back(cbCard);
	return true;
}

void PDKPlayer::blackCard(cocos2d::Node* pCard,bool bBlack)
{
	if (bBlack)
	{
		pCard->setColor(cocos2d::Color3B::GRAY);
	}
	else
	{
		pCard->setColor(cocos2d::Color3B::WHITE);
	}
}

void PDKPlayer::blackAllHandCard(bool bBalck)
{
	cocos2d::Node* pHandNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHand");
	for (int i = 0;i<pHandNode->getChildrenCount();i++)
	{
		cocos2d::Node* pCardNode = pHandNode->getChildren().at(i);
		if (pCardNode == NULL)
		{
			continue;
		}
		cocos2d::Sprite* pSprite = dynamic_cast<cocos2d::Sprite*>(WidgetFun::getChildWidget(pCardNode,"CardShow"));
		if (pSprite == NULL)
		{
			continue;
		}
		blackCard(pSprite,bBalck);
	}
}


void PDKPlayer::setTouchCardUp()
{
	upCard(m_kTouchCardList);
	blackAllHandCard(false);
}

bool PDKPlayer::isTouchCard(cocos2d::Node* pCard,const cocos2d::Vec2& kPos,float fAddWidth,bool isLastOne /*= false*/)
{
	cocos2d::Vec2 kTouchPos = pCard->convertToNodeSpace(kPos);
	cocos2d::Rect kCardBox = pCard->getBoundingBox();

	kCardBox.origin = cocos2d::Vec2::ZERO;
	if (isLastOne)
	{
		return kCardBox.containsPoint(kTouchPos);
	}

	kCardBox.size.width = fAddWidth;

	return kCardBox.containsPoint(kTouchPos);
}

void PDKPlayer::upCard(const std::vector<BYTE>& kUpCardList)
{
	cocos2d::Node* pHandNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHand");
	
	for (int n=0;n<(int)kUpCardList.size();n++)
	{
		for (int i = 0;i<pHandNode->getChildrenCount();i++)
		{
			cocos2d::Node* pCardNode = pHandNode->getChildren().at(i);
			if (pCardNode == NULL)
			{
				continue;
			}
			int nCardData = utility::parseInt(WidgetFun::getWidgetUserInfo(pCardNode,"CardData"));
			if (nCardData == (int)kUpCardList[n] )
			{
				clickCardNode(pCardNode);
				break;
			}
		}
	}
}

void PDKPlayer::downAllCard()
{
	cocos2d::Node* pHandNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHand");
	for (int i = 0;i<pHandNode->getChildrenCount();i++)
	{
		cocos2d::Node* pCardNode = pHandNode->getChildren().at(i);
		if (pCardNode == NULL)
		{
			continue;
		}
		pCardNode->setPositionY(CARD_DOWN);

	}
}

void PDKPlayer::getSendCardList()
{
	m_kSendCardList.clear();
	cocos2d::Node* pHandNode = WidgetFun::getChildWidget(m_pSeatNode,"CardNodeHand");
	for (int i = 0;i<pHandNode->getChildrenCount();i++)
	{
		cocos2d::Node* pCardNode = pHandNode->getChildren().at(i);
		if (pCardNode == NULL)
		{
			continue;
		}
		int nCardPosY = pCardNode->getPositionY();
		if (nCardPosY > CARD_DOWN)
		{
			int nCardData = utility::parseInt(WidgetFun::getWidgetUserInfo(pCardNode,"CardData"));
			m_kSendCardList.push_back(nCardData);
		}
	}
}

bool PDKPlayer::checkSendCard(const std::vector<BYTE>& kLastOutCardList,bool isBaodan)
{
	getSendCardList();
    
    BYTE nCardType = PDKGameLogic::Instance().GetCardType(m_kSendCardList);
	if (kLastOutCardList.empty())
	{
        //判断黑桃三是否先出
        if(FvMask::HasAny(PDKGameScence::Instance().game_rule, _MASK_((dword)GAME_RULE_SHOUCHU_HEITAO3)) && PDKGameScence::Instance().m_playNum == 1)
        {
            for(int i=0; i<m_kHandCardList.size(); ++i)
            {
                
                if(m_kHandCardList[i] == BANKER_CARD)
                {
                    for(int j=0; j<m_kSendCardList.size(); ++j)
                    {
                        if(m_kSendCardList[j] == BANKER_CARD)
                        {
                            break;
                        }
                        if(j == m_kSendCardList.size()-1)
                             nCardType = CT_ERROR;
                    }
                }
            }
        }
        
        //如果是单牌且有人报单则必须打最大的牌
        if(nCardType == CT_SINGLE && isBaodan && m_kSendCardList.size() == 1)
        {
            PDKGameLogic::Instance().SortCardList(m_kHandCardList);
            BYTE sendCard = PDKGameLogic::Instance().GetCardLogicValue(m_kSendCardList[0]);
            BYTE handCard = PDKGameLogic::Instance().GetCardLogicValue(m_kHandCardList[0]);
            if(sendCard == handCard)
            {
                return true;
            }
            else
                return false;
        }
        
        //如果是三带0则必须是只剩三张牌
        else if(nCardType == CT_THREE_LINE)
        {
            //分析扑克
            tagAnalyseResult AnalyseResult;
            PDKGameLogic::Instance().AnalysebCardData(m_kSendCardList,AnalyseResult);
            if(AnalyseResult.bThreeCount*3 == m_kHandCardList.size())
                return true;
            else
                return false;
        }
        //如果是三带1则必须是只剩四张牌
        else if(nCardType == CT_THREE_LINE_TAKE_SINGLE)
        {
            //分析扑克
            tagAnalyseResult AnalyseResult;
            PDKGameLogic::Instance().AnalysebCardData(m_kSendCardList,AnalyseResult);
            if((AnalyseResult.bThreeCount*3 + AnalyseResult.bThreeCount) == m_kHandCardList.size())
                return true;
            else
                return false;
        }
        //判断飞机
        else if(nCardType == CT_THREE_LINE_TAKE_DOUBLE)
        {
            tagAnalyseResult AnalyseResult;
            PDKGameLogic::Instance().AnalysebCardData(m_kSendCardList,AnalyseResult);
            //如果飞机最后少了带牌则可以直接打出
            if(AnalyseResult.bThreeCount >= 2)
            {
                if(m_kSendCardList.size() < (AnalyseResult.bThreeCount*3+AnalyseResult.bThreeCount*2))
                {
                    if(m_kHandCardList.size() == m_kSendCardList.size())
                    {
                        return true;
                    }
                    //如果是2个三个带一个三个加一张
                    else if(((AnalyseResult.bThreeCount-1)*3+(AnalyseResult.bThreeCount-1)*2) == m_kSendCardList.size())
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else if(m_kSendCardList.size() > (AnalyseResult.bThreeCount*3+AnalyseResult.bThreeCount*2))
                {
                    return false;
                }
            }
        }
        //如果是4带3则判断是否有选4带3规则
        else if(nCardType == CT_FOUR_LINE_TAKE_THREE)
        {
            if(FvMask::HasAny(PDKGameScence::Instance().game_rule, _MASK_((dword)GAME_RULE_FOUR_LINE_THREE)))
            {
                //如果最后只剩6张牌则可以4带2
                if(m_kSendCardList.size() == 6)
                {
                    if(m_kHandCardList.size() == 6)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }
		return nCardType != CT_ERROR;
	}
	else
	{
        //如果是单牌且有人报单则必须打最大的牌
        if(nCardType == CT_SINGLE && isBaodan)
        {
            PDKGameLogic::Instance().SortCardList(m_kHandCardList);
            if(m_kSendCardList[0] == m_kHandCardList[0])
            {
                return true;
            }
            else
            {
                return false;
            }
        }
		return PDKGameLogic::Instance().CompareCard(m_kSendCardList,kLastOutCardList);
	 }
}

bool PDKPlayer::sendOutCard()
{
	getSendCardList();
	if(m_kSendCardList.size()<=0)
	{
		return false;
	}

	m_tipIndex = 0;
	m_TishiCardListList.clear();
	PDKGameScence::Instance().SendOutCard(m_kSendCardList);
    return true;
}

bool  PDKPlayer::tishi(int nLastOutType,const std::vector<BYTE>& kLastOutCardList,bool isbaodan)
{
    //扑克分析
	if (m_TishiCardListList.empty())
	{
		std::vector<tagOutCardResult>		m_OutCardResultList;
		PDKGameLogic::Instance().SearchOutCard(m_kHandCardList, kLastOutCardList, nLastOutType, m_OutCardResultList);

		for (auto &OutCardResult : m_OutCardResultList)
		{
			std::vector<BYTE> m_TishiCardList;
			//放弃判断
			if (OutCardResult.cbCardCount == 0)
			{
				return false;
			}

			if (isbaodan && (kLastOutCardList.size() <= 1))
			{
				PDKGameLogic::Instance().SortCardList(m_kHandCardList);
				m_TishiCardList.push_back(m_kHandCardList[0]);
			}
			else
			{
				ASSERT(OutCardResult.cbCardCount <= m_kHandCardList.size());
				for (int i = 0; i < OutCardResult.cbCardCount; i++)
				{
					m_TishiCardList.push_back(OutCardResult.cbResultCard[i]);
				}
			}
			if (!m_TishiCardList.empty())
			{
				m_TishiCardListList.push_back(m_TishiCardList);
			}
		}
	}
   
	if (m_TishiCardListList.empty())
	{
		downAllCard();
		return false;
	}

	auto size = m_TishiCardListList.size();
	if (m_tipIndex >= size)
	{
		m_tipIndex = 0;
	}

    downAllCard();
	upCard(m_TishiCardListList.at(m_tipIndex++));
	return true;
}

void PDKPlayer::clickCardNode(cocos2d::Node* pCardNode)
{
	if (!pCardNode)
	{
		return;
	}
	int nPosY = pCardNode->getPositionY();
	if (nPosY == CARD_DOWN)
	{
		pCardNode->setPositionY(CARD_UP);
	}
	else
	{
		pCardNode->setPositionY(CARD_DOWN);
	}

}
