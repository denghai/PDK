#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据（16张玩法）
const BYTE CGameLogic::m_cbCardListData16[48]=
{
	0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,// 0x0F,	//方块 3 -  A -2
	0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,// 0x1F,	//梅花 3 -  A -2
	0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,// 0x2F,	//红桃 3 -  A -2
	0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,/*0x3E,*/0x3F,	//黑桃 3 -  A -2
};

//扑克数据（15张玩法）
const BYTE CGameLogic::m_cbCardListData15[45]=
{
	0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,//0x0E,// 0x0F,	//方块 3 -  A -2
	0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,//0x1E,// 0x1F,	//梅花 3 -  A -2
	0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,/*0x2D,*/0x2E,// 0x2F,	//红桃 3 -  A -2
	0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,/*0x3E,*/0x3F,	//黑桃 3 -  A -2
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//初始化
void CGameLogic::setPrivite(int cardCounts)
{
	cardCount = cardCounts;
}

//获取类型
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount)
{
	//简单牌形
	switch (bCardCount)
	{
	case 1: //单牌
		{
			return CT_SINGLE;
		}
	case 2:	//对牌
		{
			return (GetCardLogicValue(bCardData[0])==GetCardLogicValue(bCardData[1]))?CT_DOUBLE_LINE:CT_ERROR;
		}
	}

	//分析扑克
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(bCardData,bCardCount,AnalyseResult);

	//炸弹判断
	if ((AnalyseResult.bFourCount==1)&&(bCardCount==4)) return CT_BOMB;

	//四带三判断
	if ((AnalyseResult.bFourCount==1)&&(bCardCount==7))
	{
	    return CT_FOUR_LINE_TAKE_THREE;
	}

	//三牌判断
	if (AnalyseResult.bThreeCount>0)
	{
		//连牌判断
		bool bSeriesCard=false;
		if ((AnalyseResult.bThreeCount==1)||(AnalyseResult.bThreeLogicVolue[0]!=15))
		{
			int i = 0;
			for (i=1;i<AnalyseResult.bThreeCount;i++)
			{
				if (AnalyseResult.bThreeLogicVolue[i]!=(AnalyseResult.bThreeLogicVolue[0]-i)) break;
			}
			if (i==AnalyseResult.bThreeCount) bSeriesCard=true;
		}

		//带牌判断
		if (bSeriesCard==true)
		{
			//数据定义
			BYTE bSignedCount=bCardCount-AnalyseResult.bThreeCount*3;
			BYTE bDoubleCount=AnalyseResult.bDoubleCount+AnalyseResult.bFourCount*2;

			//类型分析
			if ((AnalyseResult.bThreeCount*3)==bCardCount) return CT_THREE_LINE;
			if ((AnalyseResult.bThreeCount==bSignedCount)&&((AnalyseResult.bThreeCount*3+bSignedCount)==bCardCount)) return CT_THREE_LINE_TAKE_SINGLE;
			if ((AnalyseResult.bThreeCount==bDoubleCount)&&((AnalyseResult.bThreeCount*3+bDoubleCount*2)==bCardCount)) return CT_THREE_LINE_TAKE_DOUBLE;
		}
	}

	//两连判断
	if (AnalyseResult.bDoubleCount>0)
	{
		//连牌判断
		bool bSeriesCard=false;
		if ((AnalyseResult.bDoubleCount==1)||(AnalyseResult.bDoubleLogicVolue[0]!=15))
		{
			int i = 0;
			for (i=1;i<AnalyseResult.bDoubleCount;i++)
			{
				if (AnalyseResult.bDoubleLogicVolue[i]!=(AnalyseResult.bDoubleLogicVolue[0]-i)) break;
			}
			if (i==AnalyseResult.bDoubleCount) bSeriesCard=true;
		}

		//两连判断
		if ((bSeriesCard==true)&&(AnalyseResult.bDoubleCount*2==bCardCount)) return CT_DOUBLE_LINE;
	}

	//单连判断
	if ((AnalyseResult.bSignedCount>=5)&&(AnalyseResult.bSignedCount==bCardCount))
	{
		//变量定义
		bool bSeriesCard=false;
		BYTE bLogicValue=GetCardLogicValue(bCardData[0]);

		//连牌判断
		if (bLogicValue!=15)
		{
			int i = 0;
			for ( i=1;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(bCardData[i])!=(bLogicValue-i)) break;
			}
			if (i==AnalyseResult.bSignedCount) bSeriesCard=true;
		}

		//单连判断
		if (bSeriesCard==true) return CT_SINGLE_LINE;
	}

	return CT_ERROR;
}


BYTE CGameLogic::GetCardType(const std::vector<BYTE>& cbCardList)
{
	int bCardCount = cbCardList.size();
	//简单牌形
	switch (bCardCount)
	{
	case 1: //单牌
		{
			return CT_SINGLE;
		}
	case 2:	//对牌
		{
			return (GetCardLogicValue(cbCardList[0])==GetCardLogicValue(cbCardList[1]))?CT_DOUBLE_LINE:CT_ERROR;
		}
	}

	//分析扑克
	tagAnalyseResult AnalyseResult;
	AnalysebCardData(cbCardList,AnalyseResult);

	//炸弹判断
	if ((AnalyseResult.bFourCount==1)&&(bCardCount==4)) return CT_BOMB;

	//四带三判断
	if ((AnalyseResult.bFourCount==1)&&((bCardCount==7)||(bCardCount==6)))
	{
	    return CT_FOUR_LINE_TAKE_THREE;
	}

	//三牌判断
	if (AnalyseResult.bThreeCount>0)
	{
		//连牌判断
		bool bSeriesCard=false;
		if ((AnalyseResult.bThreeCount==1)||(AnalyseResult.bThreeLogicVolue[0]!=15))
		{
			int i = 0;
			for (i=1;i<AnalyseResult.bThreeCount;i++)
			{
				if (AnalyseResult.bThreeLogicVolue[i]!=(AnalyseResult.bThreeLogicVolue[0]-i)) break;
			}
			if (i==AnalyseResult.bThreeCount) bSeriesCard=true;
            //存在飞机,存在另外三张
            if (i > 1)bSeriesCard=true;
		}

		//带牌判断
		if (bSeriesCard==true)
		{
			//数据定义
			BYTE bSignedCount=bCardCount-AnalyseResult.bThreeCount*3;
//			BYTE bDoubleCount=AnalyseResult.bDoubleCount+AnalyseResult.bFourCount*2;
            
			//类型分析
            if ((AnalyseResult.bThreeCount*3)==bCardCount) return CT_THREE_LINE;
            
            if((AnalyseResult.bThreeCount == 1) && (bSignedCount == 1) && (bCardCount == 4))return CT_THREE_LINE_TAKE_SINGLE;
            
            if(((bSignedCount+AnalyseResult.bThreeCount)==(AnalyseResult.bThreeCount*3)) && ((bSignedCount + (AnalyseResult.bThreeCount*3)) == bCardCount))
                return CT_THREE_LINE_TAKE_DOUBLE;
            if((AnalyseResult.bFourCount > 0) && (bSignedCount-(AnalyseResult.bFourCount*4) + AnalyseResult.bFourCount)==((AnalyseResult.bFourCount+AnalyseResult.bThreeCount) * 2))
                return CT_THREE_LINE_TAKE_DOUBLE;
            if(AnalyseResult.bThreeCount >= 2)
                return CT_THREE_LINE_TAKE_DOUBLE;
		}

	}

	//两连判断
	if (AnalyseResult.bDoubleCount>0)
	{
		//连牌判断
		bool bSeriesCard=false;
		if ((AnalyseResult.bDoubleCount==1)||(AnalyseResult.bDoubleLogicVolue[0]!=15))
		{
			int i = 0;
			for (i=1;i<AnalyseResult.bDoubleCount;i++)
			{
				if (AnalyseResult.bDoubleLogicVolue[i]!=(AnalyseResult.bDoubleLogicVolue[0]-i)) break;
			}
			if (i==AnalyseResult.bDoubleCount) bSeriesCard=true;
		}

		//两连判断
		if ((bSeriesCard==true)&&(AnalyseResult.bDoubleCount*2==bCardCount)) return CT_DOUBLE_LINE;
	}

	//单连判断
	if ((AnalyseResult.bSignedCount>=5)&&(AnalyseResult.bSignedCount==bCardCount))
	{
		//变量定义
		bool bSeriesCard=false;
		BYTE bLogicValue=GetCardLogicValue(cbCardList[0]);

		//连牌判断
		if (bLogicValue!=15)
		{
			int i = 0;
			for ( i=1;i<AnalyseResult.bSignedCount;i++)
			{
				if (GetCardLogicValue(cbCardList[i])!=(bLogicValue-i)) break;
			}
			if (i==AnalyseResult.bSignedCount) bSeriesCard=true;
		}

		//单连判断
		if (bSeriesCard==true) return CT_SINGLE_LINE;
	}
	return CT_ERROR;
}


//排列扑克
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount)
{
	//转换数值
	BYTE bLogicVolue[MAX_COUNT];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);	

	//排序操作
	bool bSorted=true;
	BYTE bTempData,bLast=bCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<bLast;i++)
		{
			if ((bLogicVolue[i]<bLogicVolue[i+1])||
				((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
			{
				//交换位置
				bTempData=bCardData[i];
				bCardData[i]=bCardData[i+1];
				bCardData[i+1]=bTempData;
				bTempData=bLogicVolue[i];
				bLogicVolue[i]=bLogicVolue[i+1];
				bLogicVolue[i+1]=bTempData;
				bSorted=false;
			}	
		}
		bLast--;
	} while(bSorted==false);

	return;
}

//混乱扑克
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	int size = 0;
	BYTE *bCardData;
	//混乱准备
	if(cardCount == 15)
	{
		size = sizeof(m_cbCardListData15);
		bCardData = new BYTE[size];
		CopyMemory(bCardData,m_cbCardListData15,sizeof(m_cbCardListData15));
	}
	else
	{
	    size = sizeof(m_cbCardListData16);
		bCardData = new BYTE[size];
		CopyMemory(bCardData,m_cbCardListData16,sizeof(m_cbCardListData16));
	}

	srand((DWORD)(time(NULL)));
	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

void CGameLogic::RandCardList(std::vector<BYTE>& cbCardList)
{
	//混乱准备
	//CopyMemory(cbCardBuffer,m_cbCardListData,cbBufferCount);

	//混乱准备
	BYTE *cbCardData;
	WORD nCardCount;
	//混乱准备
	if(cardCount == 15)
	{
		cbCardData = new BYTE[CountArray(m_cbCardListData15)];
		CopyMemory(cbCardData,m_cbCardListData15,sizeof(m_cbCardListData15));
		nCardCount = CountArray(m_cbCardListData15);
	}
	else
	{

		cbCardData = new BYTE[CountArray(m_cbCardListData16)];
		CopyMemory(cbCardData,m_cbCardListData16,sizeof(m_cbCardListData16));
		nCardCount = CountArray(m_cbCardListData16);
	}

	srand(DWORD(time(NULL)));
	 
	cbCardList.clear();
	//混乱扑克
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(nCardCount-bRandCount);
		cbCardList.push_back(cbCardData[bPosition]);
		bRandCount++;
		cbCardData[bPosition]=cbCardData[nCardCount-bRandCount];
	} while (bRandCount<nCardCount);
}

//删除扑克
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	//检验数据
	ASSERT(bRemoveCount<=bCardCount);

	//定义变量
	BYTE bDeleteCount=0,bTempCardData[20];
	if (bCardCount>CountArray(bTempCardData)) return false;
	CopyMemory(bTempCardData,bCardData,bCardCount*sizeof(bCardData[0]));

	//置零扑克
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<bCardCount;j++)
		{
			if (bRemoveCard[i]==bTempCardData[j])
			{
				bDeleteCount++;
				bTempCardData[j]=0;
				break;
			}
		}
	}
	if (bDeleteCount!=bRemoveCount) return false;

	//清理扑克
	BYTE bCardPos=0;
	for (BYTE i=0;i<bCardCount;i++)
	{
		if (bTempCardData[i]!=0) bCardData[bCardPos++]=bTempCardData[i];
	}

	return true;
}


//删除扑克
bool CGameLogic::RemoveCard(const std::vector<BYTE>& kRemoveCardList, std::vector<BYTE>& kCardList)
{
	int bRemoveCount = kRemoveCardList.size();
	int bCardCount = kCardList.size();
	//检验数据
	ASSERT(bRemoveCount<=bCardCount);

	int bDeleteCount = 0;
	//置零扑克
	for (BYTE i=0;i<bRemoveCount;i++)
	{
		for (BYTE j=0;j<bCardCount;j++)
		{
			if (kRemoveCardList[i]==kCardList[j])
			{
				bDeleteCount++;
				kCardList[j]=0;
				break;
			}
		}
	}

	if (bDeleteCount!=bRemoveCount) return false;
	std::vector<BYTE>::iterator iter = kCardList.begin();
	while(iter != kCardList.end())
	{
		BYTE cbCardData = *iter;
		if (cbCardData == 0)
		{
			iter = kCardList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	return true;
}


//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	//变量定义
	BYTE cbColor=GetCardColor(cbCardData);
	BYTE cbValue=GetCardValue(cbCardData);

	//有效判断
	switch (cbColor)
	{
	case 0x00:
		{
			return ((cbValue>=0x03)&&(cbValue<=0x0D));
		}
	case 0x10:
		{
			return ((cbValue==0x01)||((cbValue>=0x03)&&(cbValue<=0x0D)));
		}
	case 0x20:
		{
			return ((cbValue==0x01)||((cbValue>=0x03)&&(cbValue<=0x0D)));
		}
	case 0x30:
		{
			return ((cbValue>=0x01)&&(cbValue<=0x0D));
		}
	}

	return false;
}

//逻辑数值
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//扑克属性
	BYTE bCardColor=GetCardColor(bCardData);
	BYTE bCardValue=GetCardValue(bCardData);

	//转换数值
	return (bCardValue<=2)?(bCardValue+13):bCardValue;
}

//对比扑克
bool CGameLogic::CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount)
{
	//获取类型
	BYTE bNextType=GetCardType(bNextList,bNextCount);
	BYTE bFirstType=GetCardType(bFirstList,bFirstCount);

	//类型判断
	if (bFirstType==CT_ERROR) return false;

	//炸弹判断
	if ((bFirstType==CT_BOMB)&&(bNextType!=CT_BOMB)) return true;
	if ((bFirstType!=CT_BOMB)&&(bNextType==CT_BOMB)) return false;

	//规则判断
	if ((bFirstType!=bNextType)||(bFirstCount!=bNextCount)) return false;

	//开始对比
	switch (bNextType)
	{
	case CT_BOMB:
	case CT_SINGLE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
		{
			BYTE bNextLogicValue=GetCardLogicValue(bNextList[0]);
			BYTE bFirstLogicValue=GetCardLogicValue(bFirstList[0]);
			return bFirstLogicValue>bNextLogicValue;
		}
	case CT_THREE_LINE:
	case CT_THREE_LINE_TAKE_SINGLE:
	case CT_THREE_LINE_TAKE_DOUBLE:
		{
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(bNextList,bNextCount,NextResult);
			AnalysebCardData(bFirstList,bFirstCount,FirstResult);
			return FirstResult.bThreeLogicVolue[0]>NextResult.bThreeLogicVolue[0];
		}
	case CT_FOUR_LINE_TAKE_THREE:
		{
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(bNextList,bNextCount,NextResult);
			AnalysebCardData(bFirstList,bFirstCount,FirstResult);
			return FirstResult.bFourLogicVolue[0]>NextResult.bFourLogicVolue[0];
		}
	}

	return false;
}

bool CGameLogic::CompareCard(const std::vector<BYTE>& kFirstList , const std::vector<BYTE>& kNextList)
{
	//获取类型
	BYTE bFirstType=GetCardType(kFirstList);
	BYTE bNextType=GetCardType(kNextList);

	BYTE bFirstCount = kFirstList.size();
	BYTE bNextCount = kNextList.size();

	//类型判断
	if (bFirstType==CT_ERROR) return false;

	//炸弹判断
	if ((bFirstType==CT_BOMB)&&(bNextType!=CT_BOMB)) return true;
	if ((bFirstType!=CT_BOMB)&&(bNextType==CT_BOMB)) return false;

	//规则判断
	if ((bFirstType!=bNextType)||(bFirstCount!=bNextCount)) return false;

	//开始对比
	switch (bNextType)
	{
	case CT_BOMB:
	case CT_SINGLE:
	case CT_SINGLE_LINE:
	case CT_DOUBLE_LINE:
		{
			BYTE bNextLogicValue=GetCardLogicValue(kNextList[0]);
			BYTE bFirstLogicValue=GetCardLogicValue(kFirstList[0]);
			return bFirstLogicValue>bNextLogicValue;
		}
	case CT_THREE_LINE:
	case CT_THREE_LINE_TAKE_SINGLE:
	case CT_THREE_LINE_TAKE_DOUBLE:
		{
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(kNextList,NextResult);
			AnalysebCardData(kFirstList,FirstResult);
			return FirstResult.bThreeLogicVolue[0]>NextResult.bThreeLogicVolue[0];
		}
	case CT_FOUR_LINE_TAKE_THREE:
		{
			tagAnalyseResult NextResult;
			tagAnalyseResult FirstResult;
			AnalysebCardData(kNextList,NextResult);
			AnalysebCardData(kFirstList,FirstResult);
			return FirstResult.bFourLogicVolue[0]>NextResult.bFourLogicVolue[0];
		}
	}

	return false;
}


//分析扑克
void CGameLogic::AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult)
{
	//设置结果
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//扑克分析
	for (BYTE i=0;i<bCardCount;i++)
	{
		//变量定义
		BYTE bSameCount=1;
		BYTE bSameCardData[4]={bCardData[i],0,0,0};
		BYTE bLogicValue=GetCardLogicValue(bCardData[i]);

		//获取同牌
		for (int j=i+1;j<bCardCount;j++)
		{
			//逻辑对比
			if (GetCardLogicValue(bCardData[j])!=bLogicValue) break;

			//设置扑克
			bSameCardData[bSameCount++]=bCardData[j];
		}

		//保存结果
		switch (bSameCount)
		{
		case 1:		//单张
			{
				AnalyseResult.bSignedLogicVolue[AnalyseResult.bSignedCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bSignedCardData[(AnalyseResult.bSignedCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		case 2:		//两张
			{
				AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bDoubleCardData[(AnalyseResult.bDoubleCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		case 3:		//三张
			{
				AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bThreeCardData[(AnalyseResult.bThreeCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		case 4:		//四张
			{
				AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bFourCardData[(AnalyseResult.bFourCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		}

		//设置递增
		i+=bSameCount-1;
	}

	return;
}

//分析扑克
void CGameLogic::AnalysebCardData(const std::vector<BYTE>& cbCardList, tagAnalyseResult & AnalyseResult)
{
	//设置结果
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	int bCardCount = cbCardList.size();
	//扑克分析
	for (BYTE i=0;i<bCardCount;i++)
	{
		//变量定义
		BYTE bSameCount=1;
		BYTE bSameCardData[4]={cbCardList[i],0,0,0};
		BYTE bLogicValue=GetCardLogicValue(cbCardList[i]);

		//获取同牌
		for (int j=i+1;j<bCardCount;j++)
		{
			//逻辑对比
			if (GetCardLogicValue(cbCardList[j])!=bLogicValue) break;

			//设置扑克
			bSameCardData[bSameCount++]=cbCardList[j];
		}

		//保存结果
		switch (bSameCount)
		{
		case 1:		//单张
			{
				AnalyseResult.bSignedLogicVolue[AnalyseResult.bSignedCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bSignedCardData[(AnalyseResult.bSignedCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		case 2:		//两张
			{
				AnalyseResult.bDoubleLogicVolue[AnalyseResult.bDoubleCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bDoubleCardData[(AnalyseResult.bDoubleCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		case 3:		//三张
			{
				AnalyseResult.bThreeLogicVolue[AnalyseResult.bThreeCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bThreeCardData[(AnalyseResult.bThreeCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		case 4:		//四张
			{
				AnalyseResult.bFourLogicVolue[AnalyseResult.bFourCount]=bLogicValue;
				CopyMemory(&AnalyseResult.bFourCardData[(AnalyseResult.bFourCount++)*bSameCount],bSameCardData,bSameCount);
				break;
			}
		}

		//设置递增
		i+=bSameCount-1;
	}

	return;
}

//出牌判断
bool CGameLogic::SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, tagOutCardResult & OutCardResult)
{
	//设置结果
	OutCardResult.cbCardCount=0;

	//类型搜索
	switch (bTurnOutType)
	{
	case CT_ERROR:					//错误类型
		{
			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bCardData[bCardCount-1]);

			//多牌判断
			BYTE cbSameCount=1;
			for (BYTE i=1;i<bCardCount;i++)
			{
				if (GetCardLogicValue(bCardData[bCardCount-i-1])==bLogicValue) cbSameCount++;
				else break;
			}

			//完成处理
			if (cbSameCount>1)
			{
				OutCardResult.cbCardCount=cbSameCount;
				for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=bCardData[bCardCount-1-j];
				return true;
			}

			//单牌处理
			OutCardResult.cbCardCount=1;
			OutCardResult.cbResultCard[0]=bCardData[bCardCount-1];

			return true;
		}
	case CT_SINGLE:					//单牌类型
		{
			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//对比大小
			for (BYTE i=0;i<bCardCount;i++)
			{
				if (GetCardLogicValue(bCardData[bCardCount-i-1])>bLogicValue)
				{
					OutCardResult.cbCardCount=1;
					OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
					return true;
				}
			}

			break;
		}
	case CT_SINGLE_LINE:			//单连类型
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//搜索连牌
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue>=15) break;
				if (bHandLogicValue<=bLogicValue) continue;

				//搜索连牌
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<bCardCount;j++)
				{
					if ((GetCardLogicValue(bCardData[j])+bLineCount)==bHandLogicValue) 
					{
						//增加连数
						OutCardResult.cbResultCard[bLineCount++]=bCardData[j];

						//完成判断
						if (bLineCount==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_DOUBLE_LINE:	//对连类型
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//搜索连牌
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue<=bLogicValue) continue;
				if ((bTurnCardCount>2)&&(bHandLogicValue>=15)) break;

				//搜索连牌
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<(bCardCount-1);j++)
				{
					if (((GetCardLogicValue(bCardData[j])+bLineCount)==bHandLogicValue)
						&&((GetCardLogicValue(bCardData[j+1])+bLineCount)==bHandLogicValue))
					{
						//增加连数
						OutCardResult.cbResultCard[bLineCount*2]=bCardData[j];
						OutCardResult.cbResultCard[(bLineCount++)*2+1]=bCardData[j+1];

						//完成判断
						if (bLineCount*2==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_THREE_LINE:				//三连类型
	case CT_THREE_LINE_TAKE_SINGLE:	//三带一单
	case CT_THREE_LINE_TAKE_DOUBLE:	//三带一对
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=0;
			for (BYTE i=0;i<bTurnCardCount-2;i++)
			{
				bLogicValue=GetCardLogicValue(bTurnCardData[i]);
				if (GetCardLogicValue(bTurnCardData[i+1])!=bLogicValue) continue;
				if (GetCardLogicValue(bTurnCardData[i+2])!=bLogicValue) continue;
				break;
			}

			//属性数值
			BYTE bTurnLineCount=0;
			if (bTurnOutType==CT_THREE_LINE_TAKE_SINGLE) bTurnLineCount=bTurnCardCount/4;
			else if (bTurnOutType==CT_THREE_LINE_TAKE_DOUBLE) bTurnLineCount=bTurnCardCount/5;
			else bTurnLineCount=bTurnCardCount/3;

			//搜索连牌
			for (BYTE i=bTurnLineCount*3-1;i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue<=bLogicValue) continue;
				if ((bTurnLineCount>1)&&(bHandLogicValue>=15)) break;

				//搜索连牌
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<(bCardCount-2);j++)
				{
					//三牌判断
					if ((GetCardLogicValue(bCardData[j])+bLineCount)!=bHandLogicValue) continue;
					if ((GetCardLogicValue(bCardData[j+1])+bLineCount)!=bHandLogicValue) continue;
					if ((GetCardLogicValue(bCardData[j+2])+bLineCount)!=bHandLogicValue) continue;

					//增加连数
					OutCardResult.cbResultCard[bLineCount*3]=bCardData[j];
					OutCardResult.cbResultCard[bLineCount*3+1]=bCardData[j+1];
					OutCardResult.cbResultCard[(bLineCount++)*3+2]=bCardData[j+2];

					//完成判断
					if (bLineCount==bTurnLineCount)
					{
						//连牌设置
						OutCardResult.cbCardCount=bLineCount*3;

						//构造扑克
						BYTE bLeftCardData[MAX_COUNT];
						BYTE bLeftCount=bCardCount-OutCardResult.cbCardCount;
						CopyMemory(bLeftCardData,bCardData,sizeof(BYTE)*bCardCount);
						RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bLeftCardData,bCardCount);

						//分析扑克
						tagAnalyseResult AnalyseResultLeft;
						AnalysebCardData(bLeftCardData,bLeftCount,AnalyseResultLeft);

						//单牌处理
						if (bTurnOutType==CT_THREE_LINE_TAKE_SINGLE)
						{
							//提取单牌
							for (BYTE k=0;k<AnalyseResultLeft.bSignedCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=AnalyseResultLeft.bSignedCount-k-1;
								BYTE bSignedCard=AnalyseResultLeft.bSignedCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}

							//提取对牌
							for (BYTE k=0;k<AnalyseResultLeft.bDoubleCount*2;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bDoubleCount*2-k-1);
								BYTE bSignedCard=AnalyseResultLeft.bDoubleCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}

							//提取三牌
							for (BYTE k=0;k<AnalyseResultLeft.bThreeCount*3;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bThreeCount*3-k-1);
								BYTE bSignedCard=AnalyseResultLeft.bThreeCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}

							//提取四牌
							for (BYTE k=0;k<AnalyseResultLeft.bFourCount*4;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bFourCount*4-k-1);
								BYTE bSignedCard=AnalyseResultLeft.bFourCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}
						}

						//对牌处理
						if (bTurnOutType==CT_THREE_LINE_TAKE_DOUBLE)
						{
							//提取对牌
							for (BYTE k=0;k<AnalyseResultLeft.bDoubleCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bDoubleCount-k-1)*2;
								BYTE bCardData1=AnalyseResultLeft.bDoubleCardData[bIndex];
								BYTE bCardData2=AnalyseResultLeft.bDoubleCardData[bIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData2;
							}

							//提取三牌
							for (BYTE k=0;k<AnalyseResultLeft.bThreeCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bThreeCount-k-1)*3;
								BYTE bCardData1=AnalyseResultLeft.bThreeCardData[bIndex];
								BYTE bCardData2=AnalyseResultLeft.bThreeCardData[bIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData2;
							}

							//提取四牌
							for (BYTE k=0;k<AnalyseResultLeft.bFourCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bFourCount-k-1)*4;
								BYTE bCardData1=AnalyseResultLeft.bFourCardData[bIndex];
								BYTE bCardData2=AnalyseResultLeft.bFourCardData[bIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData2;
							}
						}

						//完成判断
						if (OutCardResult.cbCardCount!=bTurnCardCount)
						{
							OutCardResult.cbCardCount=0;
							return false;
						}

						return true;
					}
				}
			}

			break;
		}
	case CT_BOMB:						//炸弹类型
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=GetCardLogicValue(bTurnCardData[0]);

			//搜索炸弹
			for (BYTE i=3;i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue<=bLogicValue) continue;

				//炸弹判断
				BYTE bTempLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

				int j = 0;
				for ( j=1;j<4;j++)
				{
					if (GetCardLogicValue(bCardData[bCardCount+j-i-1])!=bTempLogicValue) break;
				}
				if (j!=4) continue;

				//完成处理
				OutCardResult.cbCardCount=bTurnCardCount;
				OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
				OutCardResult.cbResultCard[1]=bCardData[bCardCount-i];
				OutCardResult.cbResultCard[2]=bCardData[bCardCount-i+1];
				OutCardResult.cbResultCard[3]=bCardData[bCardCount-i+2];

				return true;
			}

			return false;
		}
	}

	//炸弹搜索
	for (BYTE i=3;i<bCardCount;i++)
	{
		//获取数值
		BYTE bHandLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

		//炸弹判断
		BYTE bTempLogicValue=GetCardLogicValue(bCardData[bCardCount-i-1]);

		int j = 0;
		for (j=1;j<4;j++)
		{
			if (GetCardLogicValue(bCardData[bCardCount+j-i-1])!=bTempLogicValue) break;
		}
		if (j!=4) continue;

		//完成处理
		OutCardResult.cbCardCount=4;
		OutCardResult.cbResultCard[0]=bCardData[bCardCount-i-1];
		OutCardResult.cbResultCard[1]=bCardData[bCardCount-i];
		OutCardResult.cbResultCard[2]=bCardData[bCardCount-i+1];
		OutCardResult.cbResultCard[3]=bCardData[bCardCount-i+2];

		return true;
	}

	return false;
}

bool CGameLogic::SearchOutCard(const std::vector<BYTE>& cbCardList, const std::vector<BYTE>& cbTurnCardList, BYTE bTurnOutType, tagOutCardResult & OutCardResult)
{
	//设置结果
	OutCardResult.cbCardCount=0;

	int bCardCount = cbCardList.size();
	int bTurnCardCount = cbTurnCardList.size();
	//类型搜索
	switch (bTurnOutType)
	{
	case CT_ERROR:					//错误类型
		{
			//获取数值
			BYTE bLogicValue=GetCardLogicValue(cbCardList[bCardCount-1]);

			//多牌判断
			BYTE cbSameCount=1;
			for (BYTE i=1;i<bCardCount;i++)
			{
				if (GetCardLogicValue(cbCardList[bCardCount-i-1])==bLogicValue) cbSameCount++;
				else break;
			}

			//完成处理
			if (cbSameCount>1)
			{
				OutCardResult.cbCardCount=cbSameCount;
				for (BYTE j=0;j<cbSameCount;j++) OutCardResult.cbResultCard[j]=cbCardList[bCardCount-1-j];
				return true;
			}

			//单牌处理
			OutCardResult.cbCardCount=1;
			OutCardResult.cbResultCard[0]=cbCardList[bCardCount-1];

			return true;
		}
	case CT_SINGLE:					//单牌类型
		{
			//获取数值
			BYTE bLogicValue=GetCardLogicValue(cbTurnCardList[0]);

			//对比大小
			for (BYTE i=0;i<bCardCount;i++)
			{
				if (GetCardLogicValue(cbCardList[bCardCount-i-1])>bLogicValue)
				{
					OutCardResult.cbCardCount=1;
					OutCardResult.cbResultCard[0]=cbCardList[bCardCount-i-1];
					return true;
				}
			}

			break;
		}
	case CT_SINGLE_LINE:			//单连类型
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=GetCardLogicValue(cbTurnCardList[0]);

			//搜索连牌
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(cbCardList[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue>=15) break;
				if (bHandLogicValue<=bLogicValue) continue;

				//搜索连牌
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<bCardCount;j++)
				{
					if ((GetCardLogicValue(cbCardList[j])+bLineCount)==bHandLogicValue) 
					{
						//增加连数
						OutCardResult.cbResultCard[bLineCount++]=cbCardList[j];

						//完成判断
						if (bLineCount==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_DOUBLE_LINE:	//对连类型
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=GetCardLogicValue(cbTurnCardList[0]);

			//搜索连牌
			for (BYTE i=(bTurnCardCount-1);i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(cbCardList[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue<=bLogicValue) continue;
				if ((bTurnCardCount>2)&&(bHandLogicValue>=15)) break;

				//搜索连牌
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<(bCardCount-1);j++)
				{
					if (((GetCardLogicValue(cbCardList[j])+bLineCount)==bHandLogicValue)
						&&((GetCardLogicValue(cbCardList[j+1])+bLineCount)==bHandLogicValue))
					{
						//增加连数
						OutCardResult.cbResultCard[bLineCount*2]=cbCardList[j];
						OutCardResult.cbResultCard[(bLineCount++)*2+1]=cbCardList[j+1];

						//完成判断
						if (bLineCount*2==bTurnCardCount)
						{
							OutCardResult.cbCardCount=bTurnCardCount;
							return true;
						}
					}
				}
			}

			break;
		}
	case CT_THREE_LINE:				//三连类型
	case CT_THREE_LINE_TAKE_SINGLE:	//三带一单
	case CT_THREE_LINE_TAKE_DOUBLE:	//三带一对
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=0;
			for (BYTE i=0;i<bTurnCardCount-2;i++)
			{
				bLogicValue=GetCardLogicValue(cbTurnCardList[i]);
				if (GetCardLogicValue(cbTurnCardList[i+1])!=bLogicValue) continue;
				if (GetCardLogicValue(cbTurnCardList[i+2])!=bLogicValue) continue;
				break;
			}

			//属性数值
			BYTE bTurnLineCount=0;
			if (bTurnOutType==CT_THREE_LINE_TAKE_SINGLE) bTurnLineCount=bTurnCardCount/4;
			else if (bTurnOutType==CT_THREE_LINE_TAKE_DOUBLE) bTurnLineCount=bTurnCardCount/5;
			else bTurnLineCount=bTurnCardCount/3;

			//搜索连牌
			for (BYTE i=bTurnLineCount*3-1;i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(cbCardList[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue<=bLogicValue) continue;
				if ((bTurnLineCount>1)&&(bHandLogicValue>=15)) break;

				//搜索连牌
				BYTE bLineCount=0;
				for (BYTE j=(bCardCount-i-1);j<(bCardCount-2);j++)
				{
					//三牌判断
					if ((GetCardLogicValue(cbCardList[j])+bLineCount)!=bHandLogicValue) continue;
					if ((GetCardLogicValue(cbCardList[j+1])+bLineCount)!=bHandLogicValue) continue;
					if ((GetCardLogicValue(cbCardList[j+2])+bLineCount)!=bHandLogicValue) continue;

					//增加连数
					OutCardResult.cbResultCard[bLineCount*3]=cbCardList[j];
					OutCardResult.cbResultCard[bLineCount*3+1]=cbCardList[j+1];
					OutCardResult.cbResultCard[(bLineCount++)*3+2]=cbCardList[j+2];

					//完成判断
					if (bLineCount==bTurnLineCount)
					{
						//连牌设置
						OutCardResult.cbCardCount=bLineCount*3;

						//构造扑克
						BYTE bLeftCardData[MAX_COUNT];
						BYTE bLeftCount=bCardCount-OutCardResult.cbCardCount;

						for (int i=0;i<bCardCount;i++)
						{
							bLeftCardData[i] = cbCardList[i];
						}
						RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,bLeftCardData,bCardCount);

						//分析扑克
						tagAnalyseResult AnalyseResultLeft;
						AnalysebCardData(bLeftCardData,bLeftCount,AnalyseResultLeft);

						//单牌处理
						if (bTurnOutType==CT_THREE_LINE_TAKE_SINGLE)
						{
							//提取单牌
							for (BYTE k=0;k<AnalyseResultLeft.bSignedCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=AnalyseResultLeft.bSignedCount-k-1;
								BYTE bSignedCard=AnalyseResultLeft.bSignedCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}

							//提取对牌
							for (BYTE k=0;k<AnalyseResultLeft.bDoubleCount*2;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bDoubleCount*2-k-1);
								BYTE bSignedCard=AnalyseResultLeft.bDoubleCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}

							//提取三牌
							for (BYTE k=0;k<AnalyseResultLeft.bThreeCount*3;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bThreeCount*3-k-1);
								BYTE bSignedCard=AnalyseResultLeft.bThreeCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}

							//提取四牌
							for (BYTE k=0;k<AnalyseResultLeft.bFourCount*4;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bFourCount*4-k-1);
								BYTE bSignedCard=AnalyseResultLeft.bFourCardData[bIndex];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bSignedCard;
							}
						}

						//对牌处理
						if (bTurnOutType==CT_THREE_LINE_TAKE_DOUBLE)
						{
							//提取对牌
							for (BYTE k=0;k<AnalyseResultLeft.bDoubleCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bDoubleCount-k-1)*2;
								BYTE bCardData1=AnalyseResultLeft.bDoubleCardData[bIndex];
								BYTE bCardData2=AnalyseResultLeft.bDoubleCardData[bIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData2;
							}

							//提取三牌
							for (BYTE k=0;k<AnalyseResultLeft.bThreeCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bThreeCount-k-1)*3;
								BYTE bCardData1=AnalyseResultLeft.bThreeCardData[bIndex];
								BYTE bCardData2=AnalyseResultLeft.bThreeCardData[bIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData2;
							}

							//提取四牌
							for (BYTE k=0;k<AnalyseResultLeft.bFourCount;k++)
							{
								//中止判断
								if (OutCardResult.cbCardCount==bTurnCardCount) break;

								//设置扑克
								BYTE bIndex=(AnalyseResultLeft.bFourCount-k-1)*4;
								BYTE bCardData1=AnalyseResultLeft.bFourCardData[bIndex];
								BYTE bCardData2=AnalyseResultLeft.bFourCardData[bIndex+1];
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData1;
								OutCardResult.cbResultCard[OutCardResult.cbCardCount++]=bCardData2;
							}
						}

						//完成判断
						if (OutCardResult.cbCardCount!=bTurnCardCount)
						{
							OutCardResult.cbCardCount=0;
							return false;
						}

						return true;
					}
				}
			}

			break;
		}
	case CT_BOMB:						//炸弹类型
		{
			//长度判断
			if (bCardCount<bTurnCardCount) break;

			//获取数值
			BYTE bLogicValue=GetCardLogicValue(cbTurnCardList[0]);

			//搜索炸弹
			for (BYTE i=3;i<bCardCount;i++)
			{
				//获取数值
				BYTE bHandLogicValue=GetCardLogicValue(cbCardList[bCardCount-i-1]);

				//构造判断
				if (bHandLogicValue<=bLogicValue) continue;

				//炸弹判断
				BYTE bTempLogicValue=GetCardLogicValue(cbCardList[bCardCount-i-1]);

				int j = 0;
				for ( j=1;j<4;j++)
				{
					if (GetCardLogicValue(cbCardList[bCardCount+j-i-1])!=bTempLogicValue) break;
				}
				if (j!=4) continue;

				//完成处理
				OutCardResult.cbCardCount=bTurnCardCount;
				OutCardResult.cbResultCard[0]=cbCardList[bCardCount-i-1];
				OutCardResult.cbResultCard[1]=cbCardList[bCardCount-i];
				OutCardResult.cbResultCard[2]=cbCardList[bCardCount-i+1];
				OutCardResult.cbResultCard[3]=cbCardList[bCardCount-i+2];

				return true;
			}

			return false;
		}
	}

	//炸弹搜索
	for (BYTE i=3;i<bCardCount;i++)
	{
		//获取数值
		BYTE bHandLogicValue=GetCardLogicValue(cbCardList[bCardCount-i-1]);

		//炸弹判断
		BYTE bTempLogicValue=GetCardLogicValue(cbCardList[bCardCount-i-1]);

		int j = 0;
		for (j=1;j<4;j++)
		{
			if (GetCardLogicValue(cbCardList[bCardCount+j-i-1])!=bTempLogicValue) break;
		}
		if (j!=4) continue;

		//完成处理
		OutCardResult.cbCardCount=4;
		OutCardResult.cbResultCard[0]=cbCardList[bCardCount-i-1];
		OutCardResult.cbResultCard[1]=cbCardList[bCardCount-i];
		OutCardResult.cbResultCard[2]=cbCardList[bCardCount-i+1];
		OutCardResult.cbResultCard[3]=cbCardList[bCardCount-i+2];

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
