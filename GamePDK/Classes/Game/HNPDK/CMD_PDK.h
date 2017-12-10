#ifndef CMD_PDK_HEAD_FILE
#define CMD_PDK_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#include "Game/Game/GameDefine.h"
#pragma pack(1)

//参数定义
#define INVALID_CHAIR				0xFFFF								//无效椅子
#define INVALID_TABLE				0xFFFF								//无效桌子

#define GAME_PLAYER						3									//游戏人数
#define MAX_COUNT						16									//最大数目

//结束原因
#define GER_NO_PLAYER					0x10								//没有玩家

//游戏状态
#define GS_TK_FREE						GAME_STATUS_FREE					//等待开始
#define GS_TK_PLAYING					GAME_STATUS_PLAY					//叫庄状态

#define  GAME_RULE_16ZHANG			      1
#define  GAME_RULE_15ZHANG			      2
#define  GAME_RULE_SHOUCHU_HEITAO3          3
#define  GAME_RULE_TWO_GAME                 5
#define  GAME_RULE_THREE_GAME               6
#define   GAME_RULE_FOUR_LINE_THREE         7

#define  MAX_GAME_RULE				      4

#define SERVER_LEN						32 

//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_GAME_START			101									//游戏开始
#define SUB_S_OUT_CARD				102									//用户出牌
#define SUB_S_PASS_CARD			103									//放弃出牌
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
		Stream_VECTOR(cbCardList);
	}
};

//游戏状态
struct PDK_CMD_S_StatusPlay
{
	WORD							wBankerUser;						//庄家用户
	WORD							wLastOutUser;						//出牌的人
	WORD				 			wCurrentUser;						//当前玩家
    std::vector<SCORE>				wlGameScores;
	std::vector<BYTE>				cbLastOutCardList;
	std::vector<PDK_CardListInfo>  kHandCardInfo;					//手牌

	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wBankerUser);
		Stream_VALUE(wLastOutUser);
		Stream_VALUE(wCurrentUser);
        Stream_VECTOR(wlGameScores);
		Stream_VECTOR(cbLastOutCardList);
		StructVecotrMember(PDK_CardListInfo,kHandCardInfo);
	}

};

//游戏开始
struct PDK_CMD_S_GameStart
{
    WORD								wBankerUser;						//庄家用户
    WORD				 				wCurrentUser;						//当前玩家
    WORD								wChairID;
    std::vector<SCORE>                   wlGameScores;
    std::vector<BYTE>					cbHandCardList;
    
    void StreamValue(datastream& kData,bool bSend)
    {
        Stream_VALUE(wBankerUser);
        Stream_VALUE(wCurrentUser);
        Stream_VALUE(wChairID);
        Stream_VECTOR(wlGameScores);
        Stream_VECTOR(cbHandCardList);
    }
};

struct PDK_CMD_S_OutCard
{
	WORD				 		wCurrentUser;						//当前玩家
	WORD						wOutCardUser;						//出牌玩家
	BYTE                     cbCardType;
	std::vector<BYTE>         cbOutCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(wCurrentUser);
		Stream_VALUE(wOutCardUser);
		Stream_VALUE(cbCardType);
		Stream_VECTOR(cbOutCardList);
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
		Stream_VECTOR(cbCardList);
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
#define SUB_C_PASS_CARD					3									//放弃出牌

struct PDK_CMD_C_OutCard
{
	BYTE                    cbCardType;
	std::vector<BYTE>        cbOutCardList;
	void StreamValue(datastream& kData,bool bSend)
	{
		Stream_VALUE(cbCardType);
		Stream_VECTOR(cbOutCardList);
	}
};

//出牌结果
struct tagOutCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbResultCard[MAX_COUNT];			    //结果扑克
	BYTE							cbCardType;
};

//////////////////////////////////////////////////////////////////////////

#endif

