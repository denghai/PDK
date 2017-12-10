#pragma once

#include "cocos2d.h"

namespace PDKSoundFun
{
	const static int EFFECT_NUM     = 3;

	const static int AN_GANG_NUM_W    = 2;

	const static std::string RAW_KIND= ".wav";

    const static std::string EFFECT_WARN    = "baojing1";
	const static std::string EFFECT_FEIJI	 = "feiji";
	const static std::string EFFECT_30		 = "sange";
	const static std::string EFFECT_31	     = "sandaiyi";
	const static std::string EFFECT_32	     = "sandaiyidui";
	const static std::string EFFECT_SHUNZI  = "shunzi";
	const static std::string EFFECT_ZHADAN     = "zhadan";
	const static std::string EFFECT_LIANDUI     = "liandui";
	
	const static std::string EX_BACKGRUAND = "bgm2.mp3";


	void playBackMusic(std::string kName);
	void stopBackMusic();
	void playEffect(std::string kName,bool bPreload = false);
	void playCardByGender(int iGender,int nCard);
	void playCardByGender_Dui(int iGender,int nCard);
    std::string getDefineSound(int iGender,std::string kStr);
	void playEffectByGender(int iGender,std::string kName);
	void playEffectEX(std::string kName);

	void playPass(int iGender);
} 

