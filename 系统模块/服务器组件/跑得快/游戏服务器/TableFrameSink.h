#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "HistoryScore.h"
#include "ServerControl.h"

//////////////////////////////////////////////////////////////////////////

struct PDKPlayer
{
	WORD             wChairID;
	std::vector<BYTE> kHandCard;
	std::vector<BYTE> kHandCardCost;
	SCORE            lBombScore;
	SCORE            lGameScore;
	SCORE            lGameHighScore;//单局最高分
	int              nBombCount;
	int              nBombCountConst;//炸弹个数
	int              nWinNum;//赢的局数

	PDKPlayer()
	{
		wChairID = INVALID_CHAIR;
		lBombScore = 0;
		lGameScore = 0;
		lGameHighScore = 0;
		nBombCount = 0;
		nBombCountConst = 0;
		nWinNum = 0;
	}
};


//游戏桌子类
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
public:
	BYTE							m_cbGameTypeIdex;	//游戏类型
	DWORD                           m_dwGameRule;
	//游戏变量
protected:
	WORD							m_wBankerUser;							//庄家用户
	WORD							m_wCurrentUser;							//当前用户
	WORD                            m_currentPlays;                         //当前局数 
	bool							m_bBaoPei;							    //包赔标志

	PDKPlayer                    m_kPlayers[GAME_PLAYER];
	//扑克变量
protected:
	BYTE							m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克

	WORD							m_wTurnWiner;					//胜利玩家
	BYTE							m_bOutCardType;					//出牌类型
	BYTE							m_bOutCardCount;				//出牌数目
	std::vector<BYTE>				m_bOutCardDataList;				//出牌列表
	//控制变量
protected:
	TCHAR							m_szConfigFileName[MAX_PATH];		//配置文件
	TCHAR							m_szRoomName[32];					//配置房间

	//AI变量
protected:
	//组件变量
protected:
	CGameLogic						m_GameLogic;							//游戏逻辑
	ITableFrame						* m_pITableFrame;						//框架接口
	CHistoryScore					m_HistoryScore;							//历史成绩

	tagGameServiceOption		    *m_pGameServiceOption;					//配置参数
	tagGameServiceAttrib			*m_pGameServiceAttrib;					//游戏属性

	//属性变量
protected:
	WORD				m_wPlayerCount;							//游戏人数

	//函数定义
public:
	//构造函数
	CTableFrameSink();
	//析构函数
	virtual ~CTableFrameSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() {}
	//接口查询
	virtual void * QueryInterface(const IID & Guid, DWORD dwQueryVer);

	virtual SCORE getPlayScore(int index);//返回用户总分数

	virtual WORD getChairID(int index);//返回用户ID

	virtual SCORE getHighScore(int index);//返回用户单局最高分

	virtual WORD getWinNum(int index);//返回用户赢局数量

	virtual WORD getBoombNum(int index);//返回用户打出炸弹数量
	//管理接口
public:
	//初始化
	virtual bool Initialization(IUnknownEx * pIUnknownEx);
	//复位桌子
	virtual void RepositionSink(bool resizeScore = false);

	virtual bool IsUserPlaying(WORD wChairID);
	//游戏事件
public:
	//游戏开始
	virtual bool OnEventGameStart();
	//游戏结束
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//发送场景
	virtual bool OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);


	//事件接口
public:
	//定时器事件
	virtual bool OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam);
	//游戏消息处理
	virtual bool OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//框架消息处理
	virtual bool OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//数据事件
	virtual bool OnGameDataBase(WORD wRequestID, VOID * pData, WORD wDataSize){return true;}


	//查询接口
public:
	//查询限额
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem){return 0;}
	//最少积分
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//查询服务费
	virtual bool QueryBuckleServiceCharge(WORD wChairID);
	//数据事件
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//积分事件
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason){return false;}
	//设置基数
	virtual void SetGameBaseScore(LONG lBaseScore){return;}


	//用户事件
public:
	//用户断线
	virtual bool OnActionUserOffLine(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户重入
	virtual bool OnActionUserConnect(WORD wChairID, IServerUserItem * pIServerUserItem) { return true; }
	//用户坐下
	virtual bool OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户起立
	virtual bool OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser);
	//用户同意
	virtual bool OnActionUserOnReady(WORD wChairID, IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }

	virtual void SetPrivateInfo(BYTE bGameTypeIdex,DWORD bGameRuleIdex);
	virtual void SetCreateUserID(DWORD	dwUserID);

	//游戏事件
protected:
	//读取配置
	VOID ReadConfig();

	//获取所有玩家总分
	LONGLONG GetAllUserScore(bool bAddRevenue=false);
	//写分函数
	bool TryWriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount);
	
	//获取椅子号
	WORD GetChairIDFromGameID(DWORD dwGameID);

public:
	void initPlayer(bool resizeScore);
	void shufle();
	void sendStartCard();
	bool OnUserOutCard(WORD wChairID,PDK_CMD_C_OutCard pOutCardInfo);
	bool OnUserPassCard(WORD wChairID);
	DWORD getNextPlayer(WORD wChairID);
public:
	bool hasRule(BYTE cbRule);
};

//////////////////////////////////////////////////////////////////////////

#endif
