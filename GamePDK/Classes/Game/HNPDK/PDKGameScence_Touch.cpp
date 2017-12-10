#include "PDKGameScence.h"
#include "PDKPlayer.h"
#include "Game/GameLib.h"
#include "CMD_PDK.h"
#include "PDKPlayer.h"

class NNTouchScence
	:public cocos2d::Layer
{
public:
	virtual bool init()
	{
		if ( !cocos2d::Layer::init() )
		{
			return false;
		}
		setTouchEnabled(true);
		setTouchMode(cocos2d::Touch::DispatchMode::ONE_BY_ONE);
		return true;
	}
public:
	CREATE_FUNC(NNTouchScence);
public:
	virtual bool onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return false;
		}
		if (pTouch->getID() != 0)
		{
			return false;
		}
		return PDKGameScence::Instance().ccTouchBegan(pTouch->getLocation());
	}
	virtual void onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return;
		}
		if (pTouch->getID() != 0)
		{
			return;
		}
		return PDKGameScence::Instance().ccTouchMoved(pTouch->getLocation());
	}
	virtual void onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)
	{
		if (!utility::isAllVisible(this))
		{
			return;
		}
		if (pTouch->getID() != 0)
		{
			return;
		}
		return PDKGameScence::Instance().ccTouchEnded(pTouch->getLocation());
	}
};

void PDKGameScence::initTouch()
{
	NNTouchScence* pTouchScence = new NNTouchScence;
	pTouchScence->init();
	addChild(pTouchScence,-1);
}

bool PDKGameScence::ccTouchBegan(cocos2d::Vec2 kPos)
{
	if (!IsInGame()|| m_nCurrentUser != m_pLocal->GetChairID())
	{
		return false;
	}
	m_pTouchCardNode = m_pLocal->getTouchCardNode(kPos);
	m_bDragHandCard = m_pTouchCardNode;
	m_pLocal->clearTouchList();
	return true;
}
void PDKGameScence::ccTouchMoved(cocos2d::Vec2 kPos)
{
	if (m_bDragHandCard)
	{
		m_pLocal->touchCardMove(kPos);
	}
}
void PDKGameScence::ccTouchEnded(cocos2d::Vec2 kPos)
{
	cocos2d::Node* pEndNode = m_pLocal->getTouchCardNode(kPos);
	if (pEndNode == NULL)
	{
		m_pLocal->downAllCard();
	}
	else if (pEndNode != m_pTouchCardNode)
	{
		m_pLocal->setTouchCardUp();
	}
	else
	{
		m_pLocal->clickCardNode(pEndNode);
	}
	m_pLocal->clearTouchList();
	m_pLocal->blackAllHandCard(false);
    
    bool isbaodan = m_nextPlay->m_nHandCardNum==1;
    
    if(m_plyaercount == 2)
    {
        for(int i = 0;i<3; ++i)
        {
           if(m_pLocal->GetChairID() != m_pPlayer[i]->GetChairID())
           {
               isbaodan = m_pPlayer[i]->m_nHandCardNum == 1;
               if(isbaodan)
                   break;
           }
        }
    }
    
    
    bool istrue = m_pLocal->checkSendCard(m_kLastOutCardList,isbaodan);
	if( istrue)
	{
        WidgetFun::setButtonEnabled(this,"PDKButton_OutCard",true);
	}
	else
	{
		WidgetFun::setButtonEnabled(this,"PDKButton_OutCard",false);
	}
}
