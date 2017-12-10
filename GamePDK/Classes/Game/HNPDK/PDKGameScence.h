#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/GameBase.h"
#include "Game/Game/GameDefine.h"
#include "Game/Script/TimeNode.h"

class   PDKPlayer;
struct  CMD_S_GameEnd;
struct  CMD_WeaveItem;
struct  HNMJGameRecord;

class PDKGameScence
	:public GameBase
	,public TimeNode
	,public FvSingleton<PDKGameScence>
{
public:
    Node *rootNode;
	const static int KIND_ID = 28;
	const static int VERSION_SERVER	= PROCESS_VERSION(6,0,3);				//程序版本
	const static int VERSION_CLIENT	= PROCESS_VERSION(6,0,3);	
	const static int MAX_PLAYER = 3;
	const static int OUT_CARD_TIME = 15;

public:
	PDKGameScence();
	~PDKGameScence();
public:
	virtual bool init();
	virtual bool IsInGame();
	void initPrivate();
public:
	void EnterScence();
	void HideAll();
	void defaultState();
public:
	void initButton();
	void PDKButton_Ready(cocos2d::Ref*,WidgetUserInfo*);
	void PDKButton_OutCard(cocos2d::Ref*,WidgetUserInfo*);
	void PDKButton_TiShi(cocos2d::Ref*,WidgetUserInfo*);
	void PDKButton_RefreshHandCard(cocos2d::Ref*,WidgetUserInfo*);
    void HNMJButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);
    void HNMJButton_TalkBegin(cocos2d::Ref*,WidgetUserInfo*);
    void HNMJButton_Msg(cocos2d::Ref*,WidgetUserInfo*);
    void Button_TalkDefine(cocos2d::Ref*,WidgetUserInfo*);
    void Button_Send_TalkStr(cocos2d::Ref*,WidgetUserInfo*);
    void PDKButton_TalkEnd(cocos2d::Ref*,WidgetUserInfo*);
    void TimeEnd_TalkEnd(cocos2d::Node*);
    void endButtonTalk();
    void HNMJButton_BiaoQing(cocos2d::Ref*,WidgetUserInfo*);
private:
	virtual bool OnEventSceneMessage(byte cbGameStatus, bool bLookonUser, void* data, int dataSize);
	void OnFreeScence(void* data, int dataSize);
	void OnPlayScence(void* data, int dataSize);

	//消息处理
protected:
	void initNet();
	//游戏开始
	void OnSubGameStart(void * pBuffer, int wDataSize);
	//出牌消息
	void OnSubOutCard(void * pBuffer, int wDataSize);
	//设置当前玩家
	void OnSubPassCard(void * pBuffer, int wDataSize);
	//游戏结束
	void OnSubGameEnd(void * pBuffer, int wDataSize);
public:
	void initTouch();
	bool ccTouchBegan(cocos2d::Vec2 kPos);
	void ccTouchMoved(cocos2d::Vec2 kPos);
	void ccTouchEnded(cocos2d::Vec2 kPos);
public:
	PDKPlayer* getPlayerByChairID(int iChairID);
	virtual GamePlayer* CreatePlayer(IClientUserItem * pIClientUserItem);
	virtual void DeletePlayer(GamePlayer* pPlayer);
	virtual void upSelfPlayerInfo();
	virtual void OnGFGameClose(int iExitCode);
	virtual void OnEventUserStatus(GamePlayer * pPlayer);
	virtual void LocalPlayerEnter();

	//私人场
public:
	void defaultPrivateState();
	void setPrivateInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateRoomInfo(CMD_GF_Private_Room_Info* pNetInfo);
	virtual void OnSocketSubPrivateEnd(CMD_GF_Private_End_Info* pNetInfo);
	virtual void OnSocketSubPrivateDismissInfo(CMD_GF_Private_Dismiss_Info* pNetInfo);
	void XZDDButton_WeiXinFriend(cocos2d::Ref*,WidgetUserInfo*);
	void XZDDButton_WeiXinImagic(cocos2d::Ref*,WidgetUserInfo*);
	void ButtonPlayerHeadClick(cocos2d::Ref*,WidgetUserInfo* pUserInfo);
    void NNButton_GameExit(cocos2d::Ref*,WidgetUserInfo*);
////回放
//public:
//    void initRecord();
//    void defaultRecordState();
//    bool StartRecord(datastream kDataStream);
//    void NextRecordAction();
//    virtual void onGPAccountInfoHttpIP(dword dwUserID, std::string strIP,std::string strHttp);
//    
//    void Button_GameRecordPlay(cocos2d::Ref*,WidgetUserInfo*);
//    void Button_GameRecordPase(cocos2d::Ref*,WidgetUserInfo*);
//    void Button_GameRecordLeft(cocos2d::Ref*,WidgetUserInfo*);
//    void Button_GameRecordRight(cocos2d::Ref*,WidgetUserInfo*);
//    void Button_GameRecordExit(cocos2d::Ref*,WidgetUserInfo*);
public:
	void showActionNode(bool bShow);
    void setCurrentPlayer(word wChairID);
	bool onTishi();
public:
	void SendOutCard(std::vector<BYTE> kCardList);
	void SendPass();
	void resetLastOutInfo();
public:
    void showEndInfo();
    
public:
    dword                        game_rule;
	void getGameRuleStr(std::vector<std::string>& kRuleList,dword	dwGameRuleIdex);
	static void pushGameRule(std::vector<std::string>&kRuleList, dword dwGameRule,int nRuleTag,bool bShow);
    
    int                         m_playNum;//当前局数
    int                         m_plyaercount;
    int                         m_dwPlayTotal;//总局数
protected:
    PDKPlayer*					m_nextPlay;
	PDKPlayer*					m_pLocal;
	PDKPlayer*					m_pPlayer[MAX_PLAYER];
	bool                         m_bDragHandCard;
	int							m_nCurrentUser;
	cocos2d::Node*				m_pTouchCardNode;
 
    word						    m_wRecordSelfChairID;
    int							m_iActRecordIdex;
    int							m_RecordTimeID;
    float						m_RecordTime;
    HNMJGameRecord*				m_pGameRecord;
    
	int							m_nLastOutType;
	std::vector<BYTE>				m_kLastOutCardList;
	word                         m_nLastOutUser;

};
