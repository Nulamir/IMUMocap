#include <cstdint>


//определяем новое имя для типа int64_t
typedef std::int64_t nsTime;

//123

struct sSKDataBuffer
{
	// Port number to use for server
	static const int PORTNUMBER		= 8889;		// Port number for communication
	static const int SIM_FPS		= 60;		// Tested for 30,60,120
	
	static const int ChannelCount	= 15;
	//static const int ChannelCount = 2; //nulamir
	//


	nsTime 	mTime;				//!< Time in Nanoseconds
	int		mCounter;
	
	//структура без имени, сразу переменная mCannel
	struct  
	{
		double		mT[3];
		double		mR[3];
	} mChannel[ChannelCount];

	sSKDataBuffer()
	{
		mTime		  = 0;
		mCounter	  = 0;
		SetupBuffer();
	}

	void Simulate(nsTime pTime)
	{
		mTime = pTime;

		// Send last data + 0.5 degree global rotation in the Y direction

		FBMatrix lTransformation, lChannelTransformation;
		FBRotationToMatrix(lTransformation,FBRVector(0,0.5,0));

// скелет поедет за костью? 	 нет кость 	деформируется
		//	mChannel[0].mT[0]++ ;

		
		
		for(int i = 0; i < 15; i++)
		{
			FBRotationToMatrix(lChannelTransformation, mChannel[i].mR);
			memcpy(&lChannelTransformation[12], mChannel[i].mT,sizeof(mChannel[i].mT));
			
			FBMatrixMult(lChannelTransformation,lTransformation,lChannelTransformation);
			
			FBMatrixToRotation((FBRVector&)*mChannel[i].mR,lChannelTransformation);
			memcpy(&mChannel[i].mT,&lChannelTransformation[12],sizeof(mChannel[i].mT));
		}
		
		mCounter++;
	}

	void SetupBuffer()
	{
		
		
		//0:Hips
		mChannel[0].mT[0] = 5.52507944791e-15;
		mChannel[0].mT[1] = 90.2313919067;
		mChannel[0].mT[2] = 0.0;
		mChannel[0].mR[0] = 0.0;
		mChannel[0].mR[1] = 0.0;
		mChannel[0].mR[2] = 0.0;
		mChannel[0]..Status = false;

		//1:LeftUpLeg
		mChannel[1].mT[0] = 10.8919000626;
		mChannel[1].mT[1] = 90.2313919067;
		mChannel[1].mT[2] = 0.0;
		mChannel[1].mR[0] = 0.0;
		mChannel[1].mR[1] = 0.0;
		mChannel[1].mR[2] = 0.0;

		//2:LeftLeg
		mChannel[2].mT[0] = 10.8919000626;
		mChannel[2].mT[1] = 46.1967926025;
		mChannel[2].mT[2] = 0.0;
		mChannel[2].mR[0] = 0.0;
		mChannel[2].mR[1] = 0.0;
		mChannel[2].mR[2] = 0.0;

		//3:LeftFoot
		mChannel[3].mT[0] = 10.8919000626;
		mChannel[3].mT[1] = 4.74909210205;
		mChannel[3].mT[2] = 0.0;
		mChannel[3].mR[0] = 14.9999777807;
		mChannel[3].mR[1] = 0.0;
		mChannel[3].mR[2] = 0.0;

		
		//4:RightUpLeg
		mChannel[4].mT[0] = -10.8919000626;
		mChannel[4].mT[1] = 90.2313919067;
		mChannel[4].mT[2] = 0.0;
		mChannel[4].mR[0] = 0.0;
		mChannel[4].mR[1] = 0.0;
		mChannel[4].mR[2] = 0.0;

		//5:RightLeg
		mChannel[5].mT[0] = -10.8919000626;
		mChannel[5].mT[1] = 46.1967926025;
		mChannel[5].mT[2] = 0.0;
		mChannel[5].mR[0] = 0.0;
		mChannel[5].mR[1] = 0.0;
		mChannel[5].mR[2] = 0.0;

		//6:RightFoot
		mChannel[6].mT[0] = -10.8919000626;
		mChannel[6].mT[1] = 4.74909210205;
		mChannel[6].mT[2] = 0.0;
		mChannel[6].mR[0] = 14.9999777807;
		mChannel[6].mR[1] = 0.0;
		mChannel[6].mR[2] = 0.0;

		
		//7:Spine
		mChannel[7].mT[0] = 5.52507944791e-15;
		mChannel[7].mT[1] = 135.3313919082;
		mChannel[7].mT[2] = 0.0;
		mChannel[7].mR[0] = 0.0;
		mChannel[7].mR[1] = 0.0;
		mChannel[7].mR[2] = 0.0;


		//8:Head
		mChannel[8].mT[0] = 5.52507944791e-15;
		mChannel[8].mT[1] = 145.887693025;
		mChannel[8].mT[2] = 0.0;
		mChannel[8].mR[0] = 0.0;
		mChannel[8].mR[1] = 0.0;
		mChannel[8].mR[2] = 0.0;

		//9:LeftArm
		mChannel[9].mT[0] = 21.7077007292;
		mChannel[9].mT[1] = 133.325613014;
		mChannel[9].mT[2] = 0.0;
		mChannel[9].mR[0] = 0.0;
		mChannel[9].mR[1] = 0.0;
		mChannel[9].mR[2] = -90.0002104591;

		//10:LeftForeArm
		mChannel[10].mT[0] = 48.0103015896;
		mChannel[10].mT[1] = 133.325516399;
		mChannel[10].mT[2] = 0.0;
		mChannel[10].mR[0] = 0.0;
		mChannel[10].mR[1] = 0.0;
		mChannel[10].mR[2] = -90.0002104591;

		//11:LeftHand
		mChannel[11].mT[0] = 75.5387020106;
		mChannel[11].mT[1] = 133.325415281;
		mChannel[11].mT[2] = 0.0;
		mChannel[11].mR[0] = 0.0;
		mChannel[11].mR[1] = 0.0;
		mChannel[11].mR[2] = -90.0002104591;


		//12:RightArm
		mChannel[12].mT[0] = -21.7077007292;
		mChannel[12].mT[1] = 133.325613014;
		mChannel[12].mT[2] = 0.0;
		mChannel[12].mR[0] = 0.0;
		mChannel[12].mR[1] = 0.0;
		mChannel[12].mR[2] = 90.0002104591;

		//13:RightForeArm
		mChannel[13].mT[0] = -48.0103015896;
		mChannel[13].mT[1] = 133.325516399;
		mChannel[13].mT[2] = 0.0;
		mChannel[13].mR[0] = 0.0;
		mChannel[13].mR[1] = 0.0;
		mChannel[13].mR[2] = 90.0002104591;

		//14:RightHand
		mChannel[14].mT[0] = -75.5387020106;
		mChannel[14].mT[1] = 133.325415281;
		mChannel[14].mT[2] = 0.0;
		mChannel[14].mR[0] = 0.0;
		mChannel[14].mR[1] = 0.0;
		mChannel[14].mR[2] = 90.0002104591;

	
	}


};
