#pragma once
#include "cocos2d.h"
#include "Game/FV/FvSingleton.h"
#include "Game/Widget/WidgetDefine.h"
#include "Game/Game/MissionWeiXin.h"
#include "Platform/PFKernel/CGPGameRecord.h"
#include "Platform/PFKernel/CGPublicNoticMission.h"

class NCMessageScence
	:public cocos2d::Node
    ,public CGPublicNoticMissionSink
	,public FvSingleton<NCMessageScence> 
{
public:
	NCMessageScence();
	~NCMessageScence();
public:
	virtual bool init();
public:
	void EnterScence();
	void hideAll();
public:
    virtual void onGPNoticeResult(tagGameMatch&	pGameMatchInfo,bool bSucess,const char* pStr );
    virtual void onGPPublicNoticeResult(std::string kKey,int iError,const char* szDescription);
    CGPublicNoticMission m_kPublicNoticMission;
public:
	void Button_Message(cocos2d::Ref*,WidgetUserInfo*);
};
