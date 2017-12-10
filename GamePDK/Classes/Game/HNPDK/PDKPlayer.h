
#pragma once

#include "Game/Game/GamePlayer.h"
#include "GameLib/Game/GameLib.h"
#include "cocos2d.h"
#include "CMD_PDK.h"


//游戏逻辑
class PDKPlayer:public GamePlayer
{
public:
	const static int CARD_UP = 40;
	const static int CARD_DOWN = 0;

	PDKPlayer(int iIdex,cocos2d::Node* pSeatNode);
	~PDKPlayer();

	void init();
	int  getIdex();
	void defaultState();
	void setGameRecord(bool bRecord);
	void setZhuang();
    void startGameNoTime();
	void showStatusImagic(std::string kKey);
	cocos2d::Node* getPlayerNode();
public:
	virtual void PlayerEnter();
	virtual void PlayerLeave();
	virtual void upPlayerInfo();
	virtual void upPlayerState();
    virtual void showTalkState(CMD_GR_C_TableTalk* pNetInfo);
public:
	void showClock(bool bShow,int nTime);
	void showFangZhu(bool bShow);
	void showOffline(bool bShow);
    void showBaoDan(bool bShow);
	void showReady(bool bShow);

    void hide(bool istrue = true);
	void startGame(SCORE lScore);
public:
	void setHandCard(std::vector<BYTE> kHandCard);
	void showHandCardBack(int nCardNum);
	void showHandCard();
	void showHandCard(const std::vector<BYTE>& kHandCard);
	void setCardImgic(cocos2d::Node* pCard,int nCardData);
	void  showOutCard(std::vector<BYTE> kOutCard,bool isEnd = false,bool isReconnet = false);
	void showPass(bool bShow);
	void showMeiYouPai();

	void clearHandCard();
	void clearOutCard();
	void clearTouchList();
	void clearOutState();

	void getSendCardList();
	bool checkSendCard(const std::vector<BYTE>& kLastOutCardList,bool isBaodan = false);
	bool sendOutCard();
	bool tishi(int nLastOutType,const std::vector<BYTE>& kLastOutCardList,bool isBaodan);
public:
	cocos2d::Node*  getTouchCardNode(const cocos2d::Vec2& kPos);
	void touchCardMove(const cocos2d::Vec2& kPos);
	void touchCardEnd(const cocos2d::Vec2& kPos);

	bool insertTouchCard(BYTE cbCard);
	void setTouchCardUp();
	void blackCard(cocos2d::Node* pCard,bool bBlack);
	void blackAllHandCard(bool bBalck);
	bool isTouchCard(cocos2d::Node* pCard,const cocos2d::Vec2& kPos,float fAddWidth,bool isLastOne = false);
	void upCard(const std::vector<BYTE>& kUpCardList);
	void downAllCard();
	void clickCardNode(cocos2d::Node* pCardNode);
public:
	void runAniByCards(const std::vector<BYTE>& kCardList);
	void runBombAni();
	void runLianDuiAni();
	void runShunZiAni();
	void runFeiJiAni();
    int                   m_nHandCardNum;
    int                   lScore;//用户分数
    bool                    m_exsitUser;
public:
    void showCard();
    void showCard(BYTE* cbCardData,BYTE cbCardCount);
    void setCardImagic(cocos2d::Node* pNode,int kValue,std::string kImagicFront);
private:
	int						m_iIdex;
    
	cocos2d::Node*			m_pSeatNode;
	bool						m_bGameRecord;

	std::vector<BYTE>					m_kHandCardList;
	std::vector<BYTE>					m_kTouchCardList;
	std::vector<BYTE>					m_kSendCardList;

	std::vector<std::vector<BYTE>>		m_TishiCardListList;
	int									m_tipIndex;
	
};

