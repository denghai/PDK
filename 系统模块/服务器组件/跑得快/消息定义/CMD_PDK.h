#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE
#include <vector>

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#pragma pack(1)

#define GAME_RULE_SUIJIZHUANG		1
#define GAME_RULE_QIANGZHUANG		2
#define GAME_RULE_NOT_WAITE			3

#define KIND_ID							28									//游戏 I D
#define GAME_PLAYER						3									//游戏人数
#define GAME_NAME						TEXT("跑得快")						//游戏名字
#define MAX_COUNT						16									//最大数目

#define VERSION_SERVER					PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT					PROCESS_VERSION(6,0,3)				//程序版本

//结束原因
#define GER_NO_PLAYER					0x10								//没有玩家

//游戏状态
#define GS_TK_FREE						GAME_STATUS_FREE					//等待开始
#define GS_TK_PLAYING						GAME_STATUS_PLAY					//叫庄状态
#define  GAME_RULE_16ZHANG			      1
#define  GAME_RULE_15ZHANG			      2
#define  GAME_RULE_SHOUCHU_HEITAO3        3
#define  GAME_RULE_TWO_GAME               5
#define  GAME_RULE_THREE_GAME             6
#define  GAME_RULE_FOUR_LINE_THREE        7

#define  MAX_GAME_RULE				      4

#define SERVER_LEN						32 

//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_GAME_START			101									//游戏开始
#define SUB_S_OUT_CARD				102									//用户出牌
#define SUB_S_PASS_CARD			    103									//放弃出牌
#define SUB_S_GAME_END				104									//游戏结束

//游戏状态
struct PDK_CMD_S_StatusFree
{
	LONGLONG							lCellScore;							//基础积分

	//历史积分
	LONGLONG							lTurnScore[GAME_PLAYER];			//积分信息
	LONGLONG							lCollectScore[GAME_PLAYER];			//积分信息
	TCHAR							szGameRoomName[SERVER_LEN];			//房间名称
};
#pragma pack()

struct PDK_CardListInfo
{
	WORD  wChairID;
	std::vector<BYTE> cbCardList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChairID);
		Stream_VALUE(cbCardList);
	}
};

//游戏状态
struct PDK_CMD_S_StatusPlay
{
	WORD							wBankerUser;						//庄家用户
	WORD							wLastOutUser;						//出牌的人
	WORD				 			wCurrentUser;						//当前玩家
	std::vector<BYTE>				cbLastOutCardList;
	std::vector<SCORE>              wlGameScores;
	std::vector<PDK_CardListInfo>  kHandCardInfo;					//手牌

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wBankerUser);
		Stream_VALUE(wLastOutUser);
		Stream_VALUE(wCurrentUser);
		Stream_VALUE(wlGameScores);
		Stream_VALUE(cbLastOutCardList);
		StructVecotrMember(PDK_CardListInfo,kHandCardInfo);
	}

};

//游戏开始
struct PDK_CMD_S_GameStart
{
	WORD								wBankerUser;						//庄家用户
	WORD				 				wCurrentUser;						//当前玩家
	WORD								wChairID;
	std::vector<SCORE>                  wlGameScores;
	std::vector<BYTE>					cbHandCardList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wBankerUser);
		Stream_VALUE(wCurrentUser);
		Stream_VALUE(wChairID);
		Stream_VALUE(wlGameScores);
		Stream_VALUE(cbHandCardList);
	}
};

struct PDK_CMD_S_OutCard
{
	WORD				 		wCurrentUser;						//当前玩家
	WORD						wOutCardUser;						//出牌玩家
	BYTE                    cbCardType;
	std::vector<BYTE>        cbOutCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wCurrentUser);
		Stream_VALUE(wOutCardUser);
		Stream_VALUE(cbCardType);
		Stream_VALUE(cbOutCardList);
	}
};

//放弃出牌
struct PDK_CMD_S_PassCard
{
	BYTE							bNewTurn;							//一轮开始
	WORD				 			wPassUser;							//放弃玩家
	WORD				 			wCurrentUser;						//当前玩家

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(bNewTurn);
		Stream_VALUE(wPassUser);
		Stream_VALUE(wCurrentUser);
	}

};

struct PDK_EndInfo
{
	WORD  wChairID;
	SCORE lGameScore;
	BYTE  nBombCount;
	std::vector<BYTE> cbCardList;

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wChairID);
		Stream_VALUE(lGameScore);
		Stream_VALUE(nBombCount);
		Stream_VALUE(cbCardList);
	}
};

struct PDK_CMD_S_GameEnd
{
	SCORE lGameTax;
	std::vector<PDK_EndInfo> kEndInfoList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(lGameTax);
		StructVecotrMember(PDK_EndInfo,kEndInfoList);
	}
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_OutCard					1									//用户叫庄
#define SUB_C_PASS_CARD				3									//放弃出牌

struct PDK_CMD_C_OutCard
{
	BYTE                    cbCardType;
	std::vector<BYTE>        cbOutCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbCardType);
		Stream_VALUE(cbOutCardList);
	}
};
#endif