#ifndef __ORDEVICE_SKELETON_HARDWARE_H__
#define __ORDEVICE_SKELETON_HARDWARE_H__

#define _USE_MATH_DEFINES 

#define BUF_SIZE 360
/***************************************************************************************
 Autodesk(R) Open Reality(R) Samples
 
 (C) 2009 Autodesk, Inc. and/or its licensors
 All rights reserved.
 
 AUTODESK SOFTWARE LICENSE AGREEMENT
 Autodesk, Inc. licenses this Software to you only upon the condition that 
 you accept all of the terms contained in the Software License Agreement ("Agreement") 
 that is embedded in or that is delivered with this Software. By selecting 
 the "I ACCEPT" button at the end of the Agreement or by copying, installing, 
 uploading, accessing or using all or any portion of the Software you agree 
 to enter into the Agreement. A contract is then formed between Autodesk and 
 either you personally, if you acquire the Software for yourself, or the company 
 or other legal entity for which you are acquiring the software.
 
 AUTODESK, INC., MAKES NO WARRANTY, EITHER EXPRESS OR IMPLIED, INCLUDING BUT 
 NOT LIMITED TO ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
 PURPOSE REGARDING THESE MATERIALS, AND MAKES SUCH MATERIALS AVAILABLE SOLELY ON AN 
 "AS-IS" BASIS.
 
 IN NO EVENT SHALL AUTODESK, INC., BE LIABLE TO ANYONE FOR SPECIAL, COLLATERAL, 
 INCIDENTAL, OR CONSEQUENTIAL DAMAGES IN CONNECTION WITH OR ARISING OUT OF PURCHASE 
 OR USE OF THESE MATERIALS. THE SOLE AND EXCLUSIVE LIABILITY TO AUTODESK, INC., 
 REGARDLESS OF THE FORM OF ACTION, SHALL NOT EXCEED THE PURCHASE PRICE OF THE 
 MATERIALS DESCRIBED HEREIN.
 
 Autodesk, Inc., reserves the right to revise and improve its products as it sees fit.
 
 Autodesk and Open Reality are registered trademarks or trademarks of Autodesk, Inc., 
 in the U.S.A. and/or other countries. All other brand names, product names, or 
 trademarks belong to their respective holders. 
 
 GOVERNMENT USE
 Use, duplication, or disclosure by the U.S. Government is subject to restrictions as 
 set forth in FAR 12.212 (Commercial Computer Software-Restricted Rights) and 
 DFAR 227.7202 (Rights in Technical Data and Computer Software), as applicable. 
 Manufacturer is Autodesk, Inc., 10 Duke Street, Montreal, Quebec, Canada, H3C 2L7.
***************************************************************************************/

/**	\file	ordeviceskeleton_hardware.h
*	Declaration of a virtual hardware class.
*	Contains the declaration of the virtual input hardware class
*	ORDeviceSkeletonHardware.
*/
#include <cstdint>
#include <string>

//--- SDK include
#include <fbsdk/fbsdk.h>
#include <fbsdk/fbmath.h>

//#include <cmath> 
#include <chrono>

using namespace std::chrono;
using namespace std;



// Test server include
//избавляемся от буфера, т.к. будем принимать данные прямо в DLL

typedef std::int32_t nsTime;

void getEulerFromQuaternion(FBRVector& pVector, const FBQuaternion& pQuaternion);

void getQuaternionFromEuler(FBQuaternion& pQuaternion , const FBRVector& pVector);

//#include "DeviceBuffer.h"
struct sSKDataBuffer
{
	// Port number to use for server
	static const int PORTNUMBER = 5555;		// Port number for communication
	static const int SIM_FPS = 60;		// Tested for 30,60,120
	static const int ChannelCount = 15;
	static const int ChildrensCount = 3;

	nsTime 	mTime;				//!< Time in Nanoseconds

	//структура без имени, сразу переменная mCannel
	struct
	{
		double		mT[3];
		double		mR[3];
		int			children[3];
		double		mdT[3];   //default position
		string		MAC;
		FBString	IP;
		int			socket = 0;
		FBTCPIP		mTCP;
		int			iR[3][3]; //направление вращения и перемешка осей
		double		cA[3]; //корректировочный угол
		bool		needToSendR;
	} mChannel[ChannelCount];

	sSKDataBuffer()
	{
		mTime = 0;
		SetupBuffer();
	}

	void SetupBuffer()
	{

		//0:Hips
		mChannel[0].mT[0] = 0.0;
		mChannel[0].mT[1] = 95.0;
		mChannel[0].mT[2] = 0.0;
		mChannel[0].mR[0] = 0.0;
		mChannel[0].mR[1] = 0.0;
		mChannel[0].mR[2] = 0.0;
		mChannel[0].children[0] = 1;
		mChannel[0].children[1] = 4;
		mChannel[0].children[2] = 7;
		mChannel[0].mdT[0] = 0.0;
		mChannel[0].mdT[1] = 0.0;
		mChannel[0].mdT[2] = 0.0;
		mChannel[0].MAC = "48:3f:da:87:9d:a9";
		//mChannel[0].MAC = "";
		mChannel[0].IP = "192.168.116.50";
		mChannel[0].iR[0][0] = -1; mChannel[0].iR[0][1] = 0; mChannel[0].iR[0][2] = 0;
		mChannel[0].iR[1][0] = 0; mChannel[0].iR[1][1] = -1; mChannel[0].iR[1][2] = 0;
		mChannel[0].iR[2][0] = 0; mChannel[0].iR[2][1] = 0; mChannel[0].iR[2][2] = 1;

		mChannel[0].cA[0] = 0; mChannel[0].cA[1] = 0; mChannel[0].cA[2] = 0;

		//1:LeftUpLeg
		mChannel[1].mT[0] = 17;
		mChannel[1].mT[1] = 95;
		mChannel[1].mT[2] = 0.0;
		mChannel[1].mR[0] = 0.0;
		mChannel[1].mR[1] = 0.0;
		mChannel[1].mR[2] = 0.0;
		mChannel[1].children[0] = 2;
		mChannel[1].children[1] = -1;
		mChannel[1].children[2] = -1;
		mChannel[1].mdT[0] = mChannel[1].mT[0] - mChannel[0].mT[0];
		mChannel[1].mdT[1] = mChannel[1].mT[1] - mChannel[0].mT[1];
		mChannel[1].mdT[2] = 0.0;
		mChannel[1].MAC = "40:f5:20:28:00:23";
		//mChannel[1].MAC = "";
		mChannel[1].IP = "192.168.116.51";
		mChannel[1].iR[0][0] = -1; mChannel[1].iR[0][1] = 0; mChannel[1].iR[0][2] = 0;
		mChannel[1].iR[1][0] = 0; mChannel[1].iR[1][1] = -1; mChannel[1].iR[1][2] = 0;
		mChannel[1].iR[2][0] = 0; mChannel[1].iR[2][1] = 0; mChannel[1].iR[2][2] = 1;
		mChannel[1].cA[0] = 0; mChannel[1].cA[1] = 0; mChannel[1].cA[2] = 0;

		//2:LeftLeg
		mChannel[2].mT[0] = 17;
		mChannel[2].mT[1] = 45;
		mChannel[2].mT[2] = 0.0;
		mChannel[2].mR[0] = 0.0;
		mChannel[2].mR[1] = 0.0;
		mChannel[2].mR[2] = 0.0;
		mChannel[2].children[0] = 3;
		mChannel[2].children[1] = -1;
		mChannel[2].children[2] = -1;
		mChannel[2].mdT[0] = mChannel[2].mT[0] - mChannel[1].mT[0];
		mChannel[2].mdT[1] = mChannel[2].mT[1] - mChannel[1].mT[1];
		mChannel[2].mdT[2] = 0.0;
		mChannel[2].MAC = "48:3f:da:87:ae:2b";
		//mChannel[2].MAC = "";
		mChannel[2].IP = "192.168.116.52";
		mChannel[2].iR[0][0] = -1; mChannel[2].iR[0][1] = 0; mChannel[2].iR[0][2] = 0;
		mChannel[2].iR[1][0] = 0; mChannel[2].iR[1][1] = -1; mChannel[2].iR[1][2] = 0;
		mChannel[2].iR[2][0] = 0; mChannel[2].iR[2][1] = 0; mChannel[2].iR[2][2] = 1;
		mChannel[2].cA[0] = 0; mChannel[2].cA[1] = 0; mChannel[2].cA[2] = 0;
		

		//3:LeftFoot
		mChannel[3].mT[0] = 17;
		mChannel[3].mT[1] = 1;
		mChannel[3].mT[2] = 0.0;
		mChannel[3].mR[0] = 0.0;
		mChannel[3].mR[1] = 0.0;
		mChannel[3].mR[2] = 0.0;
		mChannel[3].children[0] = -1;
		mChannel[3].children[1] = -1;
		mChannel[3].children[2] = -1;
		mChannel[3].mdT[0] = mChannel[3].mT[0] - mChannel[2].mT[0];
		mChannel[3].mdT[1] = mChannel[3].mT[1] - mChannel[2].mT[1];
		mChannel[3].mdT[2] = 0.0;
		mChannel[3].MAC = "48:3f:da:87:9a:f9";
		//mChannel[3].MAC = "";
		mChannel[3].IP = "192.168.116.53";
		mChannel[3].iR[0][0] = -1; mChannel[3].iR[0][1] = 0; mChannel[3].iR[0][2] = 0;
		mChannel[3].iR[1][0] = 0; mChannel[3].iR[1][1] = 0; mChannel[3].iR[1][2] = 1;
		mChannel[3].iR[2][0] = 0; mChannel[3].iR[2][1] = 1; mChannel[3].iR[2][2] = 0;
		mChannel[3].cA[0] = 0; mChannel[3].cA[1] = 0; mChannel[3].cA[2] = 0;


		//4:RightUpLeg
		mChannel[4].mT[0] = -17;
		mChannel[4].mT[1] = 95;
		mChannel[4].mT[2] = 0.0;
		mChannel[4].mR[0] = 0.0;
		mChannel[4].mR[1] = 0.0;
		mChannel[4].mR[2] = 0.0;
		mChannel[4].children[0] = 5;
		mChannel[4].children[1] = -1;
		mChannel[4].children[2] = -1;
		mChannel[4].mdT[0] = mChannel[4].mT[0] - mChannel[0].mT[0];
		mChannel[4].mdT[1] = mChannel[4].mT[1] - mChannel[0].mT[1];
		mChannel[4].mdT[2] = 0.0;
		mChannel[4].MAC = "	48:3f:da:87:9a:74";
		//mChannel[4].MAC = "";
		mChannel[4].IP = "192.168.116.54";
		mChannel[4].iR[0][0] = -1; mChannel[4].iR[0][1] = 0; mChannel[4].iR[0][2] = 0;
		mChannel[4].iR[1][0] = 0; mChannel[4].iR[1][1] = -1; mChannel[4].iR[1][2] = 0;
		mChannel[4].iR[2][0] = 0; mChannel[4].iR[2][1] = 0; mChannel[4].iR[2][2] = 1;
		mChannel[4].cA[0] = 0; mChannel[4].cA[1] = 0; mChannel[4].cA[2] = 0;

		//5:RightLeg
		mChannel[5].mT[0] = -17;
		mChannel[5].mT[1] = 45;
		mChannel[5].mT[2] = 0.0;
		mChannel[5].mR[0] = 0.0;
		mChannel[5].mR[1] = 0.0;
		mChannel[5].mR[2] = 0.0;
		mChannel[5].children[0] = 6;
		mChannel[5].children[1] = -1;
		mChannel[5].children[2] = -1;
		mChannel[5].mdT[0] = mChannel[5].mT[0] - mChannel[4].mT[0];
		mChannel[5].mdT[1] = mChannel[5].mT[1] - mChannel[4].mT[1];
		mChannel[5].mdT[2] = 0.0;
		mChannel[5].MAC = "	48:3f:da:87:7d:c7";
		//mChannel[5].MAC = "";
		mChannel[5].IP = "192.168.116.55";
		mChannel[5].iR[0][0] = -1; mChannel[5].iR[0][1] = 0; mChannel[5].iR[0][2] = 0;
		mChannel[5].iR[1][0] = 0; mChannel[5].iR[1][1] = -1; mChannel[5].iR[1][2] = 0;
		mChannel[5].iR[2][0] = 0; mChannel[5].iR[2][1] = 0; mChannel[5].iR[2][2] = 1;
		mChannel[5].cA[0] = 0; mChannel[5].cA[1] = 0; mChannel[5].cA[2] = 0;


		//6:RightFoot
		mChannel[6].mT[0] = -17;
		mChannel[6].mT[1] = 1;
		mChannel[6].mT[2] = 0.0;
		mChannel[6].mR[0] = 0.0;
		mChannel[6].mR[1] = 0.0;
		mChannel[6].mR[2] = 0.0;
		mChannel[6].children[0] = -1;
		mChannel[6].children[1] = -1;
		mChannel[6].children[2] = -1;
		mChannel[6].mdT[0] = mChannel[6].mT[0] - mChannel[5].mT[0];
		mChannel[6].mdT[1] = mChannel[6].mT[1] - mChannel[5].mT[1];
		mChannel[6].mdT[2] = mChannel[6].mT[2] - mChannel[5].mT[2];;
		mChannel[6].MAC = "48:3f:da:87:9a:65";
		//mChannel[6].MAC = "";
		mChannel[6].IP = "192.168.116.56";
		mChannel[6].iR[0][0] = -1; mChannel[6].iR[0][1] = 0; mChannel[6].iR[0][2] = 0;
		mChannel[6].iR[1][0] = 0; mChannel[6].iR[1][1] = 0; mChannel[6].iR[1][2] = 1;
		mChannel[6].iR[2][0] = 0; mChannel[6].iR[2][1] = 1; mChannel[6].iR[2][2] = 0;
		mChannel[6].cA[0] = 0; mChannel[6].cA[1] = 0; mChannel[6].cA[2] = 0;


		//7:Spine
		mChannel[7].mT[0] = 0.0;
		mChannel[7].mT[1] = 130;
		mChannel[7].mT[2] = 0.0;
		mChannel[7].mR[0] = 0.0;
		mChannel[7].mR[1] = 0.0;
		mChannel[7].mR[2] = 0.0;
		mChannel[7].children[0] = 8;
		mChannel[7].children[1] = 9;
		mChannel[7].children[2] = 12;
		mChannel[7].mdT[0] = mChannel[7].mT[0] - mChannel[0].mT[0];
		mChannel[7].mdT[1] = mChannel[7].mT[1] - mChannel[0].mT[1];
		mChannel[7].mdT[2] = 0.0;
		mChannel[7].MAC = "48:3f:da:87:ab:cb";
		//mChannel[7].MAC = "";
		mChannel[7].IP = "192.168.116.57";
		mChannel[7].iR[0][0] = -1; mChannel[7].iR[0][1] = 0; mChannel[7].iR[0][2] = 0;
		mChannel[7].iR[1][0] = 0; mChannel[7].iR[1][1] = -1; mChannel[7].iR[1][2] = 0;
		mChannel[7].iR[2][0] = 0; mChannel[7].iR[2][1] = 0; mChannel[7].iR[2][2] = 1;
		mChannel[7].cA[0] = 0; mChannel[7].cA[1] = 0; mChannel[7].cA[2] = 0;

		//8:Head
		mChannel[8].mT[0] = 0.0;
		mChannel[8].mT[1] = 167;
		mChannel[8].mT[2] = 0.0;
		mChannel[8].mR[0] = 0.0;
		mChannel[8].mR[1] = 0.0;
		mChannel[8].mR[2] = 0.0;
		mChannel[8].children[0] = -1;
		mChannel[8].children[1] = -1;
		mChannel[8].children[2] = -1;
		mChannel[8].mdT[0] = mChannel[8].mT[0] - mChannel[7].mT[0];
		mChannel[8].mdT[1] = mChannel[8].mT[1] - mChannel[7].mT[1];
		mChannel[8].mdT[2] = 0.0;
		mChannel[8].MAC = "48:3f:da:87:a9:1f";
		//mChannel[8].MAC = "";
		mChannel[8].IP = "192.168.116.58";
		mChannel[8].iR[0][0] = -1; mChannel[8].iR[0][1] = 0; mChannel[8].iR[0][2] = 0;
		mChannel[8].iR[1][0] = 0; mChannel[8].iR[1][1] = -1; mChannel[8].iR[1][2] = 0;
		mChannel[8].iR[2][0] = 0; mChannel[8].iR[2][1] = 0; mChannel[8].iR[2][2] = 1;
		mChannel[8].cA[0] = 0; mChannel[8].cA[1] = 0; mChannel[8].cA[2] = 0;

		//9:LeftArm
		mChannel[9].mT[0] = 17;
		mChannel[9].mT[1] = 150;
		mChannel[9].mT[2] = 0.0;
		mChannel[9].mR[0] = 0.0;
		mChannel[9].mR[1] = 0.0;
		mChannel[9].mR[2] = 0.0;
		mChannel[9].children[0] = 10;
		mChannel[9].children[1] = -1;
		mChannel[9].children[2] = -1;
		mChannel[9].mdT[0] = mChannel[9].mT[0] - mChannel[7].mT[0];
		mChannel[9].mdT[1] = mChannel[9].mT[1] - mChannel[7].mT[1];
		mChannel[9].mdT[2] = 0.0;
		mChannel[9].MAC = "40:f5:20:29:33:db";
		//mChannel[9].MAC = "";
		mChannel[9].IP = "192.168.116.59";
		mChannel[9].iR[0][0] = 0; mChannel[9].iR[0][1] = 1; mChannel[9].iR[0][2] = 0;
		mChannel[9].iR[1][0] = 0; mChannel[9].iR[1][1] = 0; mChannel[9].iR[1][2] = 1;
		mChannel[9].iR[2][0] = 1; mChannel[9].iR[2][1] = 0; mChannel[9].iR[2][2] = 0;
		mChannel[9].cA[0] = 0; mChannel[9].cA[1] = 0; mChannel[9].cA[2] = 0;

		//10:LeftForeArm
		mChannel[10].mT[0] = 47;
		mChannel[10].mT[1] = 150;
		mChannel[10].mT[2] = 0.0;
		mChannel[10].mR[0] = 0.0;
		mChannel[10].mR[1] = 0.0;
		mChannel[10].mR[2] = 0.0;
		mChannel[10].children[0] = 11;
		mChannel[10].children[1] = -1;
		mChannel[10].children[2] = -1;
		mChannel[10].mdT[0] = mChannel[10].mT[0] - mChannel[9].mT[0];
		mChannel[10].mdT[1] = mChannel[10].mT[1] - mChannel[9].mT[1];
		mChannel[10].mdT[2] = 0.0;
		mChannel[10].MAC = "48:3f:da:87:ab:9a";
		//mChannel[10].MAC = "";
		mChannel[10].IP = "192.168.116.60";
		mChannel[10].iR[0][0] = 0; mChannel[10].iR[0][1] = 1; mChannel[10].iR[0][2] = 0;
		mChannel[10].iR[1][0] = 0; mChannel[10].iR[1][1] = 0; mChannel[10].iR[1][2] = 1;
		mChannel[10].iR[2][0] = 1; mChannel[10].iR[2][1] = 0; mChannel[10].iR[2][2] = 0;
		mChannel[10].cA[0] = 0; mChannel[10].cA[1] = 0; mChannel[10].cA[2] = 0;

		//11:LeftHand
		mChannel[11].mT[0] = 77;
		mChannel[11].mT[1] = 150;
		mChannel[11].mT[2] = 0.0;
		mChannel[11].mR[0] = 0.0;
		mChannel[11].mR[1] = 0.0;
		mChannel[11].mR[2] = 0.0;
		mChannel[11].children[0] = -1;
		mChannel[11].children[1] = -1;
		mChannel[11].children[2] = -1;
		mChannel[11].mdT[0] = mChannel[11].mT[0] - mChannel[10].mT[0];
		mChannel[11].mdT[1] = mChannel[11].mT[1] - mChannel[10].mT[1];
		mChannel[11].mdT[2] = 0.0;
		mChannel[11].MAC = "40:f5:20:28:cd:7a";
		//mChannel[11].MAC = "";
		mChannel[11].IP = "192.168.116.61";
		mChannel[11].iR[0][0] = 0; mChannel[11].iR[0][1] = 1; mChannel[11].iR[0][2] = 0;
		mChannel[11].iR[1][0] = 0; mChannel[11].iR[1][1] = 0; mChannel[11].iR[1][2] = 1;
		mChannel[11].iR[2][0] = 1; mChannel[11].iR[2][1] = 0; mChannel[11].iR[2][2] = 0;
		mChannel[11].cA[0] = 0; mChannel[11].cA[1] = 0; mChannel[11].cA[2] = 0;

		//12:RightArm
		mChannel[12].mT[0] = -17;
		mChannel[12].mT[1] = 150;
		mChannel[12].mT[2] = 0.0;
		mChannel[12].mR[0] = 0.0;
		mChannel[12].mR[1] = 0.0;
		mChannel[12].mR[2] = 0.0;
		mChannel[12].children[0] = 13;
		mChannel[12].children[1] = -1;
		mChannel[12].children[2] = -1;
		mChannel[12].mdT[0] = mChannel[12].mT[0] - mChannel[7].mT[0];
		mChannel[12].mdT[1] = mChannel[12].mT[1] - mChannel[7].mT[1];
		mChannel[12].mdT[2] = 0.0;
		mChannel[12].MAC = "48:3f:da:87:9d:95";
	    //mChannel[12].MAC = "";
		mChannel[12].IP = "192.168.116.62";
		mChannel[12].iR[0][0] = 0; mChannel[12].iR[0][1] = -1; mChannel[12].iR[0][2] = 0;
		mChannel[12].iR[1][0] = 0; mChannel[12].iR[1][1] = 0; mChannel[12].iR[1][2] = 1;
		mChannel[12].iR[2][0] = -1; mChannel[12].iR[2][1] = 0; mChannel[12].iR[2][2] = 0;
		mChannel[12].cA[0] = 0; mChannel[12].cA[1] = 0; mChannel[12].cA[2] = 0;

		//13:RightForeArm
		mChannel[13].mT[0] = -47;
		mChannel[13].mT[1] = 150;
		mChannel[13].mT[2] = 0.0;
		mChannel[13].mR[0] = 0.0;
		mChannel[13].mR[1] = 0.0;
		mChannel[13].mR[2] = 0.0;
		mChannel[13].children[0] = 14;
		mChannel[13].children[1] = -1;
		mChannel[13].children[2] = -1;
		mChannel[13].mdT[0] = mChannel[13].mT[0] - mChannel[12].mT[0];
		mChannel[13].mdT[1] = mChannel[13].mT[1] - mChannel[12].mT[1];
		mChannel[13].mdT[2] = 0.0;
		mChannel[13].MAC = "40:f5:20:29:46:1a";
	    //mChannel[13].MAC = "";
		mChannel[13].IP = "192.168.116.63";
		mChannel[13].iR[0][0] = 0; mChannel[13].iR[0][1] = -1; mChannel[13].iR[0][2] = 0;
		mChannel[13].iR[1][0] = 0; mChannel[13].iR[1][1] = 0; mChannel[13].iR[1][2] = 1;
		mChannel[13].iR[2][0] = -1; mChannel[13].iR[2][1] = 0; mChannel[13].iR[2][2] = 0;
		mChannel[13].cA[0] = 0; mChannel[13].cA[1] = 0; mChannel[13].cA[2] = 0;

		//14:RightHand
		mChannel[14].mT[0] = -77;
		mChannel[14].mT[1] = 150;
		mChannel[14].mT[2] = 0.0;
		mChannel[14].mR[0] = 0.0;
		mChannel[14].mR[1] = 0.0;
		mChannel[14].mR[2] = 0.0;
		mChannel[14].children[0] = -1;
		mChannel[14].children[1] = -1;
		mChannel[14].children[2] = -1;
		mChannel[14].mdT[0] = mChannel[14].mT[0] - mChannel[13].mT[0];
		mChannel[14].mdT[1] = mChannel[14].mT[1] - mChannel[13].mT[1];
		mChannel[14].mdT[2] = 0.0;
		mChannel[14].MAC = "40:f5:20:2d:9d:4b";
	    //mChannel[14].MAC = "";
		mChannel[14].IP = "192.168.116.64";
		mChannel[14].iR[0][0] = 0; mChannel[14].iR[0][1] = -1; mChannel[14].iR[0][2] = 0;
		mChannel[14].iR[1][0] = 0; mChannel[14].iR[1][1] = 0; mChannel[14].iR[1][2] = 1;
		mChannel[14].iR[2][0] = -1; mChannel[14].iR[2][1] = 0; mChannel[14].iR[2][2] = 0;
		mChannel[14].cA[0] = 0; mChannel[14].cA[1] = 0; mChannel[14].cA[2] = 0;

	}




};


struct sESPDataBuffer
{
	uint8_t id[4];
	uint8_t	w[4];
	uint8_t	x[4];
	uint8_t	y[4];
	uint8_t	z[4];
	uint8_t	mTime[4];
	
};

struct sMomentQuaternions
 {
	struct
    {
	  double w;
	  double x;
	  double y;
	  double z;
    } mChannel[sSKDataBuffer::ChannelCount];
};

struct sESPRAWBuffer
{
	int32_t id;

	uint8_t	w1;
	uint8_t	w2;
	uint8_t	w3;
	uint8_t	w4;

	uint8_t	x1;
	uint8_t	x2;
	uint8_t	x3;
	uint8_t	x4;

	uint8_t	y1;
	uint8_t	y2;
	uint8_t	y3;
	uint8_t	y4;

	uint8_t	z1;
	uint8_t	z2;
	uint8_t	z3;
	uint8_t	z4;

	nsTime 	mTime;				//!< Time in Nanoseconds


};



//! Simple hardware.
class ORDeviceSkeletonHardware
{
public:
	//! Constructor & destructor
	ORDeviceSkeletonHardware();
	~ORDeviceSkeletonHardware();

	//--- Opens and closes connection with data server. returns true if successful
	bool	Open();								//!< Open the connection.
	bool	Close();							//!< Close connection.

	bool	GetSetupInfo	();						//!< Get the setup information.

	//--- Hardware communication
	bool	StartDataStream	();						//!< Put the device in streaming mode.
	bool	StopDataStream	();						//!< Take the device out of streaming mode.

	void	ResetPacketTimeOffset(FBTime &pTime);	//!< Reset time offset for recording
	bool	FetchDataPacket	(FBTime &pTime);		//!< Fetch a data packet from the computer.
	bool	ResetSensors();		//!<reset sensors

	//--- Attribute management
	void		SetNetworkUse(bool pUse)					{ mNetworkUse =  pUse;						}
	bool		GetNetworkUse()								{ return mNetworkUse;						}
	void		SetNetworkAddress	(const char *pIPAddress){ mNetworkAddress =  pIPAddress;			}
	const char*	GetNetworkAddress	()						{ return mNetworkAddress;			}
	void		SetNetworkPort		(int pPort)				{ mNetworkPort = pPort;						}
	int			GetNetworkPort		()						{ return mNetworkPort;						}

	//--- Channel & Channel data management
	int		GetChannelCount	()					{	return mChannelCount;						}
	const char*	GetChannelName	(int pChannel)	{	return mChannel[pChannel].mName;	}
	int		GetChannelParent(int pChannel)		{	return mChannel[pChannel].mParentChannel;	}
	FBVector3d GetDefaultT	(int pChannel)		{	SkeletonNodeInfo& lSI = mChannel[pChannel]; return FBVector3d(lSI.mDefaultT[0],lSI.mDefaultT[1],lSI.mDefaultT[2]);		}
	FBVector3d GetDefaultR	(int pChannel)		{	SkeletonNodeInfo& lSI = mChannel[pChannel]; return FBVector3d(lSI.mDefaultR[0],lSI.mDefaultR[1],lSI.mDefaultR[2]);		}
	bool	GetStatus		(int pChannel)		{ return mChannel[pChannel].Status; }
	double	GetDataTX		(int pChannel)		{	return mChannel[pChannel].mT[0];		}
	double	GetDataTY		(int pChannel)		{	return mChannel[pChannel].mT[1];		}
	double	GetDataTZ		(int pChannel)		{	return mChannel[pChannel].mT[2];		}
	double	GetDataRX		(int pChannel)		{	return mChannel[pChannel].mR[0];		}
	double	GetDataRY		(int pChannel)		{	return mChannel[pChannel].mR[1];		}
	double	GetDataRZ		(int pChannel)		{	return mChannel[pChannel].mR[2];		}

	


private:



	struct SkeletonNodeInfo
	{
		const char*	mName;
		int			mParentChannel;
		double		mDefaultT[3];
		double		mDefaultR[3];
		double		mT[3];
		double		mR[3];
		bool		Status;
	};

	SkeletonNodeInfo	mChannel[sSKDataBuffer::ChannelCount];//!< Channel data & info.
	int			mChannelCount;								//!< Channel count.
	sSKDataBuffer mSkDataBuffer;							//!< IO buffer
	sESPDataBuffer mESPDataBuffer;							//!< IO buffer

	//попытка получить данные через память

	sESPDataBuffer memoryArray[sSKDataBuffer::ChannelCount];


	sESPRAWBuffer mESPRAWBuffer;
	sMomentQuaternions mMomentQuaternions;

	FBQuaternion cQuaternion; // try to get quaterion
	FBQuaternion correctionQuaternion; // try to get quaterion
	FBQuaternion resultQuaternion; // try to get quaterion
	FBQuaternion resultQuaternion1; // try to get quaterion

	FBRVector cRotationOfNode;

	FBRVector correctionVector;
	

	//FBMatrix lTransformation, lChannelTransformation; // prepare martix to rotation
	


	nsTime		mPacketTimeOffset;							//!< Packet time offset for recording
	FBTime		mStartRecordingTime;						//!< Time at start of recording

	FBTime		mLastSimulationTime;						//!< Last simulation time for internal simulation
	int			mPassCount;									//!< Pass counter for internal simulation

	bool		mNetworkUse;								//!< Try to use network
	FBString	mNetworkAddress;							//!< Network address.
	int			mNetworkPort;								//!< Network port.
	FBSystem	mSystem;									//!< System interface.

	int			mSocket;									//!< Socket for communication

	//FBTCPIP		mTCP;										//!< TCP/IP communication
	milliseconds mLastFetchTimems;							// последнее время кадра

	float num_fps_limit = 30.f;
	float fps_limit_timer = 0.f;
	float fps_lim = 1 / num_fps_limit;						//столько секунд нужно подождать для 60 фпс

	float frameDeltaTime;

	high_resolution_clock::time_point nowTime = high_resolution_clock::now();  //время в начале исполнения приема данных
	high_resolution_clock::time_point thenTime = nowTime;					 // время по окончании приема данных и поворотов нод

	high_resolution_clock::time_point deltaTimeAddon;														//время которое комьютер занимался отрисовкой экрана и проч перед повторным вызовом фетча
};

#endif /* __ORDEVICE_SKELETON_HARDWARE_H__ */
