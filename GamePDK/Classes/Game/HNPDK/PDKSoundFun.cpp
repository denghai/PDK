#include "PDKSoundFun.h"
#include "SimpleAudioEngine.h"
#include "Game/Script/utility.h"
#include "Game/Script/SoundFun.h"
#include "PDKGameLogic.h"
namespace PDKSoundFun
{

	static const std::string LG_FLAG_PATH = "GamePDK/Sound/";

	static bool s_bMute = false;
	static float s_fSound1 = 0;
	static float s_fSound2 = 0;

	
	void playBackMusic(std::string kName)
	{
		SoundFun::Instance().playBackMusic(LG_FLAG_PATH + utility::a_u8(kName));
	}

	void stopBackMusic()
	{
		SoundFun::Instance().stopBackMusic();
	}

	void playEffect(std::string kName,bool bPreload /*= false*/)
	{
		kName = utility::a_u8(LG_FLAG_PATH + kName + RAW_KIND);

		SoundFun::Instance().playEffect(kName,bPreload);
	}

	void playCardByGender(int iGender,int nCard)
	{
		int nValue = PDKGameLogic::Instance().GetCardValue(nCard);
		std::string kName;
		if (iGender)
		{
			kName =utility::toString("woman/","pk_",nValue);
		}
		else
		{
			kName =utility::toString("man/","pk_",nValue);
		}
		playEffect(kName);
	}

	void playCardByGender_Dui(int iGender,int nCard)
	{
		int nValue = PDKGameLogic::Instance().GetCardValue(nCard);

        utility::log("the poke:%d",nValue);
		std::string kName;
		if (iGender)
		{
			kName =utility::toString("woman/","dui",nValue);
		}
		else
		{
			kName =utility::toString("man/","dui",nValue);
		}
		playEffect(kName);
	}

	void playEffectByGender(int iGender,std::string kName)
	{
		if (iGender)
		{
			kName =utility::toString("woman/",kName);
		}
		else
		{
			kName =utility::toString("man/",kName);
		}
		playEffect(kName);
	}

    std::string getDefineSound(int iGender,std::string kStr)
    {
        std::string strValue = utility::toString("fix_msg_",kStr);
        if (iGender)
        {
            strValue = utility::toString("man/",strValue);
        }
        else
        {
            strValue =utility::toString("woman/",strValue);
        }
        return utility::toString(LG_FLAG_PATH,strValue,".mp3");
    }
    
    void playEffectEX(std::string kName)
	{
		SoundFun::Instance().playEffect(kName);
	}

	void playPass(int iGender)
	{
		std::string kName;
		srand((DWORD)time(NULL));
		int nValue = rand()%5;
		if (iGender)
		{
			kName =utility::toString("woman/","buyao",nValue);
		}
		else
		{
			kName =utility::toString("man/","buyao",nValue);
		}

		playEffect(kName);
	}
} 
