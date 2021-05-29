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

/**	\file	ordeviceskeleton_hardware.cxx
*	Definition of a virtual hardware class.
*	Contains the definition of the functions for the
*	ORDeviceSkeletonHardware class.
*/

//--- Class declaration
#include "ordeviceskeleton_hardware.h"
#include "windows.h"


/************************************************
 *	Constructor.
 ************************************************/
ORDeviceSkeletonHardware::ORDeviceSkeletonHardware() :
	mChannelCount(0),
	mPassCount(0),
	mNetworkUse(false),
	mNetworkPort(sSKDataBuffer::PORTNUMBER),
	mSocket(0)
{
	mNetworkAddress = "192.168.116.124";
}


/************************************************
 *	Destructor.
 ************************************************/
ORDeviceSkeletonHardware::~ORDeviceSkeletonHardware()
{
}


/************************************************
 *	Open device communications.
 ************************************************/
bool ORDeviceSkeletonHardware::Open()
{
	return true;
}

enum RotSeq { zyx, zyz, zxy, zxz, yxz, yxy, yzx, yzy, xyz, xyx, xzy, xzx };

void twoaxisrot(double r11, double r12, double r21, double r31, double r32, FBRVector& res) {
	res.mValue[0] = atan2(r11, r12);
	res.mValue[1] = acos(r21);
	res.mValue[2] = atan2(r31, r32);
}

void threeaxisrot(double r11, double r12, double r21, double r31, double r32, FBRVector& res) {
	res.mValue[0] = atan2(r31, r32) * 180 / M_PI;
	res.mValue[1] = asin(r21) * 180 / M_PI;
	res.mValue[2] = atan2(r11, r12) * 180 / M_PI;
}


struct Quaternion {

	double w;
	double x;
	double y;
	double z;
	
};

void getEulerFromQuaternion(FBRVector& pVector, const FBQuaternion& pQuaternion, RotSeq  rotSeq)
{

	//yaw = Math.Atan2(2 * (quat.W * quat.Z + quat.X * quat.Y), 1 - 2 * (quat.Y * quat.Y + quat.Z * quat.Z));
	//pitch = Math.Asin(2 * (quat.W * quat.Y - quat.Z * quat.X));
	//roll = Math.Atan2(2 * (quat.W * quat.X + quat.Y * quat.Z), 1 - 2 * (quat.X * quat.X + quat.Y * quat.Y));


 //   //	 roll (x-axis rotation)
	//double sinr_cosp = 2 * (pQuaternion.mValue[0] * pQuaternion.mValue[1] + pQuaternion.mValue[2] * pQuaternion.mValue[3]);
	//double cosr_cosp = 1 - 2 * (pQuaternion.mValue[1] * pQuaternion.mValue[1] + pQuaternion.mValue[2] * pQuaternion.mValue[2]);
	//pVector.mValue[0] = std::atan2(sinr_cosp, cosr_cosp) * 180 / M_PI;
	// //pitch (y-axis rotation) 
	//double sinp = 2 * (pQuaternion.mValue[0] * pQuaternion.mValue[2] - pQuaternion.mValue[3] * pQuaternion.mValue[1]);
	//if (std::abs(sinp) >= 1)
	//	pVector.mValue[1] = std::copysign(90, sinp); 	// use 90 degrees if out of range 
	//else pVector.mValue[1] = std::asin(sinp) * 180 / M_PI; ;
	// //yaw (z-axis rotation) 
	//double siny_cosp = 2 * (pQuaternion.mValue[0] * pQuaternion.mValue[3] + pQuaternion.mValue[1] * pQuaternion.mValue[2]);
	//double cosy_cosp = 1 - 2 * (pQuaternion.mValue[2] * pQuaternion.mValue[2] + pQuaternion.mValue[3] * pQuaternion.mValue[3]);
	//pVector.mValue[2] = std::atan2(siny_cosp, cosy_cosp) * 180 / M_PI;




	//threeaxisrot(2 * (pQuaternion.mValue[2] * pQuaternion.mValue[3] + pQuaternion.mValue[0] * pQuaternion.mValue[1]),
	//pQuaternion.mValue[0] * pQuaternion.mValue[0] - pQuaternion.mValue[1] * pQuaternion.mValue[1] + pQuaternion.mValue[2] * pQuaternion.mValue[2] - pQuaternion.mValue[3] * pQuaternion.mValue[3],
	//	-2 * (pQuaternion.mValue[1] * pQuaternion.mValue[2] - pQuaternion.mValue[0] * pQuaternion.mValue[3]),
	//	2 * (pQuaternion.mValue[1] * pQuaternion.mValue[3] + pQuaternion.mValue[0] * pQuaternion.mValue[2]),
	//	pQuaternion.mValue[0] * pQuaternion.mValue[0] + pQuaternion.mValue[1] * pQuaternion.mValue[1] - pQuaternion.mValue[2] * pQuaternion.mValue[2] - pQuaternion.mValue[3] * pQuaternion.mValue[3],
	//	pVector);

	Quaternion q;

	q.w = pQuaternion.mValue[0];
	q.x = pQuaternion.mValue[1];
	q.y = pQuaternion.mValue[2];
	q.z = pQuaternion.mValue[3];

	//return;

	switch (rotSeq) {
	case zyx:
		threeaxisrot(2 * (q.x * q.y + q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			-2 * (q.x * q.z - q.w * q.y),
			2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			pVector);
		break;

	case zyz:
		twoaxisrot(2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.z + q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.z - q.w * q.y),
			pVector);
		break;

	case zxy:
		threeaxisrot(-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			pVector);
		break;

	case zxz:
		twoaxisrot(2 * (q.x * q.z + q.w * q.y),
			-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.x * q.z - q.w * q.y),
			2 * (q.y * q.z + q.w * q.x),
			pVector);
		break;

	case yxz:
		threeaxisrot(2 * (q.x * q.z + q.w * q.y),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			-2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.y + q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			pVector);
		break;

	case yxy:
		twoaxisrot(2 * (q.x * q.y - q.w * q.z),
			2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.x * q.y + q.w * q.z),
			-2 * (q.y * q.z - q.w * q.x),
			pVector);
		break;

	case yzx:
		threeaxisrot(-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.y + q.w * q.z),
			-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			pVector);
		break;

	case yzy:
		twoaxisrot(2 * (q.y * q.z + q.w * q.x),
			-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			2 * (q.y * q.z - q.w * q.x),
			2 * (q.x * q.y + q.w * q.z),
			pVector);
		break;

	case xyz:
		threeaxisrot(-2 * (q.y * q.z - q.w * q.x),
			q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z,
			2 * (q.x * q.z + q.w * q.y),
			-2 * (q.x * q.y - q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			pVector);
		break;

	case xyx:
		twoaxisrot(2 * (q.x * q.y + q.w * q.z),
			-2 * (q.x * q.z - q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.y - q.w * q.z),
			2 * (q.x * q.z + q.w * q.y),
			pVector);
		break;

	case xzy:
		threeaxisrot(2 * (q.y * q.z + q.w * q.x),
			q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z,
			-2 * (q.x * q.y - q.w * q.z),
			2 * (q.x * q.z + q.w * q.y),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			pVector);
		break;

	case xzx:
		twoaxisrot(2 * (q.x * q.z - q.w * q.y),
			2 * (q.x * q.y + q.w * q.z),
			q.w * q.w + q.x * q.x - q.y * q.y - q.z * q.z,
			2 * (q.x * q.z + q.w * q.y),
			-2 * (q.x * q.y - q.w * q.z),
			pVector);
		break;
	default:
		
		break;
	}


};



void getQuaternionFromEuler(FBQuaternion& pQuaternion, const FBRVector& pVector)
{
	// yaw (Z), pitch (Y), roll (X) 
	double roll = pVector.mValue[0];
	double pitch = pVector.mValue[1];
	double yaw = pVector.mValue[2];

	double cy = cos(yaw * 0.5); 
	double sy = sin(yaw * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);

	pQuaternion.mValue[0] = cr * cp * cy + sr * sp * sy; 
	pQuaternion.mValue[1] = sr * cp * cy - cr * sp * sy;
	pQuaternion.mValue[2] = cr * sp * cy + sr * cp * sy;
	pQuaternion.mValue[3] = cr * cp * sy - sr * sp * cy;
	

}

void multiplyQuaternion(FBQuaternion& pQuaternion, const FBQuaternion& aQuaternion, FBQuaternion& bQuaternion)
{

	pQuaternion.mValue[0] = aQuaternion.mValue[0] * bQuaternion.mValue[0] - aQuaternion.mValue[1] * bQuaternion.mValue[1] - aQuaternion.mValue[2] * bQuaternion.mValue[2] - aQuaternion.mValue[3] * bQuaternion.mValue[3];

	pQuaternion.mValue[1] = aQuaternion.mValue[0] * bQuaternion.mValue[1] + aQuaternion.mValue[1] * bQuaternion.mValue[0] + aQuaternion.mValue[2] * bQuaternion.mValue[3] - aQuaternion.mValue[3] * bQuaternion.mValue[2];

	pQuaternion.mValue[2] = aQuaternion.mValue[0] * bQuaternion.mValue[2] + aQuaternion.mValue[2] * bQuaternion.mValue[0] + aQuaternion.mValue[3] * bQuaternion.mValue[1] - aQuaternion.mValue[1] * bQuaternion.mValue[3];

	pQuaternion.mValue[3] = aQuaternion.mValue[0] * bQuaternion.mValue[3] + aQuaternion.mValue[3] * bQuaternion.mValue[0] + aQuaternion.mValue[1] * bQuaternion.mValue[2] - aQuaternion.mValue[2] * bQuaternion.mValue[1];

}





/************************************************
 *	Get device setup information.
 ************************************************/
bool ORDeviceSkeletonHardware::GetSetupInfo()
{
	//This should be read from hardware.
	//Here we used simple script to generate this data from skeleton:
/*
from pyfbsdk import *

lModels = FBModelList()
FBGetSelectedModels(lModels)

def SetupHierarchy(pModel, pParentIdx, pNextIndex):
	lIdx = pNextIndex
	pNextIndex += 1
	lT = FBVector3d()
	lR = FBVector3d()
	pModel.GetVector(lT,FBModelTransformationType.kModelTranslation)
	pModel.GetVector(lR,FBModelTransformationType.kModelRotation)
	print '//'+str(lIdx)+':'+pModel.Name
	print 'mChannel['+str(lIdx)+'].mName = "' + pModel.Name + '";'
	print 'mChannel['+str(lIdx)+'].mParentChannel = ' +  str(pParentIdx) + ';'
	print 'mChannel['+str(lIdx)+'].mT[0] = ' +  str(lT[0]) + ';'
	print 'mChannel['+str(lIdx)+'].mT[1] = ' +  str(lT[1]) + ';'
	print 'mChannel['+str(lIdx)+'].mT[2] = ' +  str(lT[2]) + ';'
	print 'mChannel['+str(lIdx)+'].mR[0] = ' +  str(lR[0]) + ';'
	print 'mChannel['+str(lIdx)+'].mR[1] = ' +  str(lR[1]) + ';'
	print 'mChannel['+str(lIdx)+'].mR[2] = ' +  str(lR[2]) + ';'
	print ''
	for lChild in pModel.Children:
		pNextIndex = SetupHierarchy(lChild,lIdx,pNextIndex)
	return pNextIndex

if len(lModels) == 1:
	SetupHierarchy(lModels[0],-1,0)
*/
//mChannelCount	= 30;
//nulamir
	mChannelCount = sSKDataBuffer::ChannelCount;
	//


	//0:Hips
	mChannel[0].mName = "Hips";
	mChannel[0].mParentChannel = -1;
	mChannel[0].mT[0] = mSkDataBuffer.mChannel[0].mT[0];
	mChannel[0].mT[1] = mSkDataBuffer.mChannel[0].mT[1];
	mChannel[0].mT[2] = mSkDataBuffer.mChannel[0].mT[2];
	mChannel[0].mR[0] = mSkDataBuffer.mChannel[0].mR[0];
	mChannel[0].mR[1] = mSkDataBuffer.mChannel[0].mR[1];
	mChannel[0].mR[2] = mSkDataBuffer.mChannel[0].mR[2];
	mChannel[0].Status = false;

	//1:LeftUpLeg
	mChannel[1].mName = "LeftUpLeg";
	mChannel[1].mParentChannel = 0;
	mChannel[1].mT[0] = mSkDataBuffer.mChannel[1].mT[0];
	mChannel[1].mT[1] = mSkDataBuffer.mChannel[1].mT[1];
	mChannel[1].mT[2] = mSkDataBuffer.mChannel[1].mT[2];
	mChannel[1].mR[0] = mSkDataBuffer.mChannel[1].mR[0];
	mChannel[1].mR[1] = mSkDataBuffer.mChannel[1].mR[1];
	mChannel[1].mR[2] = mSkDataBuffer.mChannel[1].mR[2];
	mChannel[1].Status = false;

	//2:LeftLeg
	mChannel[2].mName = "LeftLeg";
	mChannel[2].mParentChannel = 1;
	mChannel[2].mT[0] = mSkDataBuffer.mChannel[2].mT[0];
	mChannel[2].mT[1] = mSkDataBuffer.mChannel[2].mT[1];
	mChannel[2].mT[2] = mSkDataBuffer.mChannel[2].mT[2];
	mChannel[2].mR[0] = mSkDataBuffer.mChannel[2].mR[0];
	mChannel[2].mR[1] = mSkDataBuffer.mChannel[2].mR[1];
	mChannel[2].mR[2] = mSkDataBuffer.mChannel[2].mR[2];
	mChannel[2].Status = false;

	//3:LeftFoot
	mChannel[3].mName = "LeftFoot";
	mChannel[3].mParentChannel = 2;
	mChannel[3].mT[0] = mSkDataBuffer.mChannel[3].mT[0];
	mChannel[3].mT[1] = mSkDataBuffer.mChannel[3].mT[1];
	mChannel[3].mT[2] = mSkDataBuffer.mChannel[3].mT[2];
	mChannel[3].mR[0] = mSkDataBuffer.mChannel[3].mR[0];
	mChannel[3].mR[1] = mSkDataBuffer.mChannel[3].mR[1];
	mChannel[3].mR[2] = mSkDataBuffer.mChannel[3].mR[2];
	mChannel[3].Status = false;


	//6:RightUpLeg
	mChannel[4].mName = "RightUpLeg";
	mChannel[4].mParentChannel = 0;
	mChannel[4].mT[0] = mSkDataBuffer.mChannel[4].mT[0];
	mChannel[4].mT[1] = mSkDataBuffer.mChannel[4].mT[1];
	mChannel[4].mT[2] = mSkDataBuffer.mChannel[4].mT[2];
	mChannel[4].mR[0] = mSkDataBuffer.mChannel[4].mR[0];
	mChannel[4].mR[1] = mSkDataBuffer.mChannel[4].mR[1];
	mChannel[4].mR[2] = mSkDataBuffer.mChannel[4].mR[2];
	mChannel[4].Status = false;

	//7:RightLeg
	mChannel[5].mName = "RightLeg";
	mChannel[5].mParentChannel = 4;
	mChannel[5].mT[0] = mSkDataBuffer.mChannel[5].mT[0];
	mChannel[5].mT[1] = mSkDataBuffer.mChannel[5].mT[1];
	mChannel[5].mT[2] = mSkDataBuffer.mChannel[5].mT[2];
	mChannel[5].mR[0] = mSkDataBuffer.mChannel[5].mR[0];
	mChannel[5].mR[1] = mSkDataBuffer.mChannel[5].mR[1];
	mChannel[5].mR[2] = mSkDataBuffer.mChannel[5].mR[2];
	mChannel[5].Status = false;

	//8:RightFoot
	mChannel[6].mName = "RightFoot";
	mChannel[6].mParentChannel = 5;
	mChannel[6].mT[0] = mSkDataBuffer.mChannel[6].mT[0];
	mChannel[6].mT[1] = mSkDataBuffer.mChannel[6].mT[1];
	mChannel[6].mT[2] = mSkDataBuffer.mChannel[6].mT[2];
	mChannel[6].mR[0] = mSkDataBuffer.mChannel[6].mR[0];
	mChannel[6].mR[1] = mSkDataBuffer.mChannel[6].mR[1];
	mChannel[6].mR[2] = mSkDataBuffer.mChannel[6].mR[2];
	mChannel[6].Status = false;


	//11:Spine
	mChannel[7].mName = "Spine";
	mChannel[7].mParentChannel = 0;
	mChannel[7].mT[0] = mSkDataBuffer.mChannel[7].mT[0];
	mChannel[7].mT[1] = mSkDataBuffer.mChannel[7].mT[1];
	mChannel[7].mT[2] = mSkDataBuffer.mChannel[7].mT[2];
	mChannel[7].mR[0] = mSkDataBuffer.mChannel[7].mR[0];
	mChannel[7].mR[1] = mSkDataBuffer.mChannel[7].mR[1];
	mChannel[7].mR[2] = mSkDataBuffer.mChannel[7].mR[2];
	mChannel[7].Status = false;


	//14:Head 
	mChannel[8].mName = "Head";
	mChannel[8].mParentChannel = 7;
	mChannel[8].mT[0] = mSkDataBuffer.mChannel[8].mT[0];
	mChannel[8].mT[1] = mSkDataBuffer.mChannel[8].mT[1];
	mChannel[8].mT[2] = mSkDataBuffer.mChannel[8].mT[2];
	mChannel[8].mR[0] = mSkDataBuffer.mChannel[8].mR[0];
	mChannel[8].mR[1] = mSkDataBuffer.mChannel[8].mR[1];
	mChannel[8].mR[2] = mSkDataBuffer.mChannel[8].mR[2];
	mChannel[8].Status = false;




	//17:LeftArm
	mChannel[9].mName = "LeftArm";
	mChannel[9].mParentChannel = 7;
	mChannel[9].mT[0] = mSkDataBuffer.mChannel[9].mT[0];
	mChannel[9].mT[1] = mSkDataBuffer.mChannel[9].mT[1];
	mChannel[9].mT[2] = mSkDataBuffer.mChannel[9].mT[2];
	mChannel[9].mR[0] = mSkDataBuffer.mChannel[9].mR[0];
	mChannel[9].mR[1] = mSkDataBuffer.mChannel[9].mR[1];
	mChannel[9].mR[2] = mSkDataBuffer.mChannel[9].mR[2];
	mChannel[9].Status = false;

	//18:LeftForeArm
	mChannel[10].mName = "LeftForeArm";
	mChannel[10].mParentChannel = 9;
	mChannel[10].mT[0] = mSkDataBuffer.mChannel[10].mT[0];
	mChannel[10].mT[1] = mSkDataBuffer.mChannel[10].mT[1];
	mChannel[10].mT[2] = mSkDataBuffer.mChannel[10].mT[2];
	mChannel[10].mR[0] = mSkDataBuffer.mChannel[10].mR[0];
	mChannel[10].mR[1] = mSkDataBuffer.mChannel[10].mR[1];
	mChannel[10].mR[2] = mSkDataBuffer.mChannel[10].mR[2];
	mChannel[10].Status = false;

	//19:LeftHand
	mChannel[11].mName = "LeftHand";
	mChannel[11].mParentChannel = 10;
	mChannel[11].mT[0] = mSkDataBuffer.mChannel[11].mT[0];
	mChannel[11].mT[1] = mSkDataBuffer.mChannel[11].mT[1];
	mChannel[11].mT[2] = mSkDataBuffer.mChannel[11].mT[2];
	mChannel[11].mR[0] = mSkDataBuffer.mChannel[11].mR[0];
	mChannel[11].mR[1] = mSkDataBuffer.mChannel[11].mR[1];
	mChannel[11].mR[2] = mSkDataBuffer.mChannel[11].mR[2];
	mChannel[11].Status = false;

	//24:RightArm
	mChannel[12].mName = "RightArm";
	mChannel[12].mParentChannel = 7;
	mChannel[12].mT[0] = mSkDataBuffer.mChannel[12].mT[0];
	mChannel[12].mT[1] = mSkDataBuffer.mChannel[12].mT[1];
	mChannel[12].mT[2] = mSkDataBuffer.mChannel[12].mT[2];
	mChannel[12].mR[0] = mSkDataBuffer.mChannel[12].mR[0];
	mChannel[12].mR[1] = mSkDataBuffer.mChannel[12].mR[1];
	mChannel[12].mR[2] = mSkDataBuffer.mChannel[12].mR[2];
	mChannel[12].Status = false;

	//25:RightForeArm
	mChannel[13].mName = "RightForeArm";
	mChannel[13].mParentChannel = 12;
	mChannel[13].mT[0] = mSkDataBuffer.mChannel[13].mT[0];
	mChannel[13].mT[1] = mSkDataBuffer.mChannel[13].mT[1];
	mChannel[13].mT[2] = mSkDataBuffer.mChannel[13].mT[2];
	mChannel[13].mR[0] = mSkDataBuffer.mChannel[13].mR[0];
	mChannel[13].mR[1] = mSkDataBuffer.mChannel[13].mR[1];
	mChannel[13].mR[2] = mSkDataBuffer.mChannel[13].mR[2];
	mChannel[13].Status = false;

	//26:RightHand
	mChannel[14].mName = "RightHand";
	mChannel[14].mParentChannel = 13;
	mChannel[14].mT[0] = mSkDataBuffer.mChannel[14].mT[0];
	mChannel[14].mT[1] = mSkDataBuffer.mChannel[14].mT[1];
	mChannel[14].mT[2] = mSkDataBuffer.mChannel[14].mT[2];
	mChannel[14].mR[0] = mSkDataBuffer.mChannel[14].mR[0];
	mChannel[14].mR[1] = mSkDataBuffer.mChannel[14].mR[1];
	mChannel[14].mR[2] = mSkDataBuffer.mChannel[14].mR[2];
	mChannel[14].Status = false;



	for (int i = 0; i < mChannelCount; i++)
	{
		memcpy(mChannel[i].mDefaultT, mChannel[i].mT, sizeof(double) * 3);
		memcpy(mChannel[i].mDefaultR, mChannel[i].mR, sizeof(double) * 3);
	}

	//mSkDataBuffer.SetupBuffer();

	return true;
}


/************************************************
 *	Close device communications.
 ************************************************/
bool ORDeviceSkeletonHardware::Close()
{
	// TODO: Close network connection to server

//	for (int i = 0; i < GetChannelCount(); i++)
//	{
//		if (mSkDataBuffer.mChannel[i].socket != 0)
//		{

//			if (mSkDataBuffer.mChannel[i].socket) mSkDataBuffer.mChannel[i].mTCP.CloseSocket(mSocket);
//			mSkDataBuffer.mChannel[i].socket = 0;

//		}

//	}
//
	return true;
}

/************************************************
 *	Take current DataBuffer time as a time offset
 ************************************************/
void ORDeviceSkeletonHardware::ResetPacketTimeOffset(FBTime& pTime)
{
	mPacketTimeOffset = mSkDataBuffer.mTime; //
	mStartRecordingTime = pTime;
}

/************************************************
 *	Fetch a data packet from the device.
 ************************************************/
bool ORDeviceSkeletonHardware::FetchDataPacket(FBTime& pTime)
{

	HANDLE hMapFile;
	LPBYTE M_File;


	int32_t id, w, x, y, z;
	nsTime 	mTime;
	double len;

	nowTime = high_resolution_clock::now(); // замеряем время на входе

	duration<double> time_span = nowTime - thenTime; // определяем разницу после отрисовок и вычислений

	//Sleep(10);

	while (time_span.count() < fps_lim)   // разница должна быть не меньше 1/60 сек. дожигаем время до этого значения
	{
		nowTime = high_resolution_clock::now();  // время увеличивается 
		time_span = nowTime - thenTime;    // разница тоже
	}

	thenTime = high_resolution_clock::now();  //ставим метку и все что пройдет после пойдет в зачет времени между кадрами


	mLastSimulationTime = mSystem.SystemTime;
	mSkDataBuffer.mTime = mLastSimulationTime.GetMilliSeconds() * 1000; //нужно для корректной работы записи анимации

	//открываем файл
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		"MemoryFile");               // name of mapping object

	if (hMapFile == NULL)
	{
		return false;
	}

	M_File = (LPBYTE)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		BUF_SIZE);

	if (M_File == NULL)
	{
		CloseHandle(hMapFile);
		return false;
	}

	memcpy(memoryArray, M_File, BUF_SIZE);


	for (int i = 0; i < mChannelCount; i++)
		//int i = 14;
	{

		//проблемы две: 1 при вращении по продольной оси кости возникает асцилляция думаю изза замка, 2 при повороле родительской кости меняется направление вращения потомка (напирмер вниз плечо на правой руке 12-13 датчики)
			// разбираем буфер
		id = (uint32_t)(uint32_t)memoryArray[i].id[0] | (uint32_t)memoryArray[i].id[1] << 8 | (uint32_t)memoryArray[i].id[2] << 16 | (uint32_t)memoryArray[i].id[3] << 24;
		w = (uint32_t)(uint32_t)memoryArray[i].w[0] | (uint32_t)memoryArray[i].w[1] << 8 | (uint32_t)memoryArray[i].w[2] << 16 | (uint32_t)memoryArray[i].w[3] << 24;
		x = (uint32_t)(uint32_t)memoryArray[i].x[0] | (uint32_t)memoryArray[i].x[1] << 8 | (uint32_t)memoryArray[i].x[2] << 16 | (uint32_t)memoryArray[i].x[3] << 24;
		y = (uint32_t)(uint32_t)memoryArray[i].y[0] | (uint32_t)memoryArray[i].y[1] << 8 | (uint32_t)memoryArray[i].y[2] << 16 | (uint32_t)memoryArray[i].y[3] << 24;
		z = (uint32_t)(uint32_t)memoryArray[i].z[0] | (uint32_t)memoryArray[i].z[1] << 8 | (uint32_t)memoryArray[i].z[2] << 16 | (uint32_t)memoryArray[i].z[3] << 24;
		mTime = (uint32_t)(uint32_t)memoryArray[i].mTime[0] | (uint32_t)memoryArray[i].mTime[1] << 8 | (uint32_t)memoryArray[i].mTime[2] << 16 | (uint32_t)memoryArray[i].mTime[3] << 24;

		if (mTime != 0) {

			//w - uint32_t   потом преобразуется в float сразу нельзя
			cQuaternion.mValue[0] = w;
			cQuaternion.mValue[1] = x;
			cQuaternion.mValue[2] = y;
			cQuaternion.mValue[3] = z;

			//нормализация 
			len = FBQLength(cQuaternion);
			cQuaternion.mValue[0] = cQuaternion.mValue[0] / len;
			cQuaternion.mValue[1] = cQuaternion.mValue[1] / len;
			cQuaternion.mValue[2] = cQuaternion.mValue[2] / len;
			cQuaternion.mValue[3] = cQuaternion.mValue[3] / len;

			//сначала свопим оси
			double tempx, tempy, tempz, tempw;
			tempw = cQuaternion.mValue[0];
			tempx = cQuaternion.mValue[1];
			tempy = cQuaternion.mValue[2];
			tempz = cQuaternion.mValue[3];


			//cQuaternion.mValue[0] = mSkDataBuffer.mChannel[i].iR[0] * tempw;
			//cQuaternion.mValue[1] = mSkDataBuffer.mChannel[i].iR[1] * -tempy;
			//cQuaternion.mValue[2] = mSkDataBuffer.mChannel[i].iR[2] * tempz;
			//cQuaternion.mValue[3] = mSkDataBuffer.mChannel[i].iR[3] *- tempx;


			cQuaternion.mValue[1] = mSkDataBuffer.mChannel[i].iR[0][0] * tempx + mSkDataBuffer.mChannel[i].iR[0][1] * tempy + mSkDataBuffer.mChannel[i].iR[0][2] * tempz; // -tempz
			cQuaternion.mValue[2] = mSkDataBuffer.mChannel[i].iR[1][0] * tempx + mSkDataBuffer.mChannel[i].iR[1][1] * tempy + mSkDataBuffer.mChannel[i].iR[1][2] * tempz;//-tempx;
			cQuaternion.mValue[3] = mSkDataBuffer.mChannel[i].iR[2][0] * tempx + mSkDataBuffer.mChannel[i].iR[2][1] * tempy + mSkDataBuffer.mChannel[i].iR[2][2] * tempz;//tempy;


			//cQuaternion.mValue[0] = mSkDataBuffer.mChannel[i].iR[0] * tempw;
			//cQuaternion.mValue[1] = mSkDataBuffer.mChannel[i].iR[1] * tempx;
			//cQuaternion.mValue[2] = mSkDataBuffer.mChannel[i].iR[2] * tempy;
			//cQuaternion.mValue[3] = mSkDataBuffer.mChannel[i].iR[3] * tempz;


			//идеальный случай когда оси вращения датчика совпадают с мировыми
			//но ось z датчика направлена вверх а у от нас,
			//поэтому повернув датчик на 90 градусов по х (умножением кватерниона вращения х 90 градусов на вращение датчика) мы приведем его к нужному результату
			//это подходит для всех вертикально навешенных датчиков 
			//для горизонтально навешенных датчиков нужно менять местами компоненты кватерниона z->y y->x x->z  
			//для понимания берем два датчика и выкладываем оси 
			//для зеркалирования вращения используется минус перед компонентой кватерниона но только в паре, одны оси инвертить нельзя
			//сначала перезагружаем датчики чтобы сбросить значения осей 
			//потом нажимаем ресет чтобы внести корректировочный угол в положение датчика
			//что касается лап - не возможно задать угол коррекции по оси, совпадающей с гравитацией, поэтому вписываем его заранее
			//а потом вроде уже можно корректировать обычным образом. Нужны тесты при полной навеске и вообще нужны ли плавающие углы корректировки или жесткие
			//на лапах нашел что можно менять две компоненты кватерниона, но третья идет с обратным знаком
			//для чего понадобился поворот вертикального датчика: он знает что его повернули по х из за гравитации и этот поворот не сбросить в ноль, прихдится его учитывать
			
			FBRVector correctionVector;

			correctionVector.mValue[0] = mSkDataBuffer.mChannel[i].cA[0] * (M_PI / 180);
			correctionVector.mValue[1] = mSkDataBuffer.mChannel[i].cA[1] * (M_PI / 180);
			correctionVector.mValue[2] = mSkDataBuffer.mChannel[i].cA[2] *(M_PI / 180);

	
			getQuaternionFromEuler(correctionQuaternion, correctionVector);

			multiplyQuaternion(resultQuaternion, correctionQuaternion, cQuaternion);

			//получаем Эйлера
			getEulerFromQuaternion(cRotationOfNode, resultQuaternion, zyx);
	

			mSkDataBuffer.mChannel[i].mR[0] = cRotationOfNode.mValue[0];// +correctionVector.mValue[0];
			mSkDataBuffer.mChannel[i].mR[1] = cRotationOfNode.mValue[1];// -correctionVector.mValue[1];
			mSkDataBuffer.mChannel[i].mR[2] = cRotationOfNode.mValue[2];// -correctionVector.mValue[2];

			// Copy data to head
			SkeletonNodeInfo* lIter;
			lIter = &mChannel[i]; // значение ноды
				//memcpy(lIter->mT, mSkDataBuffer.mChannel[i].mT, sizeof(lIter->mT));
			memcpy(lIter->mR, mSkDataBuffer.mChannel[i].mR, sizeof(lIter->mR));

			if (true) {
				FBMatrix lTransformation, lChannelTransformation, gTransformatilon; // prepare martix to rotation

				//FBQuaternionToMatrix(lTransformation, cQuaternion);

				FBRotationToMatrix(lTransformation, mSkDataBuffer.mChannel[i].mR);

				for (int j = 0; j < mSkDataBuffer.ChildrensCount; j++)
				{
					int children = mSkDataBuffer.mChannel[i].children[j];
					if (children != -1) // 0 = hips
					{

						memcpy(&lChannelTransformation[0], mSkDataBuffer.mChannel[children].mdT, sizeof(mSkDataBuffer.mChannel[children].mT)); //дельту от родителя
						//берем 
						FBMatrixMult(lChannelTransformation, lTransformation, lChannelTransformation);// поворачиваем на кватернион родителя


						memcpy(&gTransformatilon[0], mSkDataBuffer.mChannel[i].mT, sizeof(mSkDataBuffer.mChannel[i].mT)); //достаем родительскую ноду текущую


						lChannelTransformation = lChannelTransformation + gTransformatilon; //вернуть нужно в глобальных координатах, то есть сложить с родительской нодой

						memcpy(&mSkDataBuffer.mChannel[children].mT, &lChannelTransformation[0], sizeof(mSkDataBuffer.mChannel[children].mT));  // here is updating transfers

						lIter = &mChannel[children];

						memcpy(lIter->mT, mSkDataBuffer.mChannel[children].mT, sizeof(lIter->mT));// значение ноды

					}

				}
			}

			mESPDataBuffer.w[0] = 0; mESPDataBuffer.w[1] = 0; mESPDataBuffer.w[2] = 0; mESPDataBuffer.w[3] = 1;
			mESPDataBuffer.x[0] = 0; mESPDataBuffer.x[1] = 0; mESPDataBuffer.x[2] = 0; mESPDataBuffer.x[3] = 0;
			mESPDataBuffer.y[0] = 0; mESPDataBuffer.y[1] = 0; mESPDataBuffer.y[2] = 0; mESPDataBuffer.y[3] = 0;
			mESPDataBuffer.z[0] = 0; mESPDataBuffer.z[1] = 0; mESPDataBuffer.z[2] = 0; mESPDataBuffer.z[3] = 0;

		}


	}

	// Update packet relative time
	pTime.SetMilliSeconds((mSkDataBuffer.mTime - mPacketTimeOffset) / 1000);
	pTime += mStartRecordingTime;


	UnmapViewOfFile(M_File);

	CloseHandle(hMapFile);


	return true;
}





/************************************************
 *	Reset sensors
 ************************************************/
bool ORDeviceSkeletonHardware::ResetSensors()
{

	Sleep(5000);

	for (int i = 0; i < mChannelCount; i++)

	{

		mSkDataBuffer.mChannel[i].cA[0] = - mSkDataBuffer.mChannel[i].mR[0] + mSkDataBuffer.mChannel[i].cA[0];
		mSkDataBuffer.mChannel[i].cA[1] = - mSkDataBuffer.mChannel[i].mR[1] + mSkDataBuffer.mChannel[i].cA[1];
		mSkDataBuffer.mChannel[i].cA[2] = - mSkDataBuffer.mChannel[i].mR[2] + mSkDataBuffer.mChannel[i].cA[2];

	}


	return true;

}




/************************************************
 *	Start data streaming from device.
 ************************************************/
bool ORDeviceSkeletonHardware::StartDataStream()
{
	// TODO: ask server to start sending data
//	bool succes = false;

	//if (mNetworkUse)
	//{
//		for (int i = 0; i < GetChannelCount(); i++)
//		{
	//		if (mSkDataBuffer.mChannel[i].MAC != "")  // не задан мас - нет датчика
	//		{
//
//				if (mSkDataBuffer.mChannel[i].socket == 0) {  //если ещё не подключен
//
//					if (mSkDataBuffer.mChannel[i].mTCP.CreateSocket(mSkDataBuffer.mChannel[i].socket, kFBTCPIP_Stream))
//					{
//						if (!mSkDataBuffer.mChannel[i].mTCP.Connect(mSkDataBuffer.mChannel[i].socket, mSkDataBuffer.mChannel[i].IP, mNetworkPort))
//						{
//							if (mSkDataBuffer.mChannel[i].socket) mSkDataBuffer.mChannel[i].mTCP.CloseSocket(mSkDataBuffer.mChannel[i].socket);
//							mSkDataBuffer.mChannel[i].socket = 0;
//
//						}
//						else
//						{
//							mSkDataBuffer.mChannel[i].needToSendR = true;
//						}
//
//					}
//
//				}
//
//			}
//
//		}
//
		//если хотя бы один не подключился - возвращаем ложь
//		for (int i = 0; i < GetChannelCount(); i++)
//		{
//			succes = true;
//			if ((mSkDataBuffer.mChannel[i].MAC != "") && (mSkDataBuffer.mChannel[i].socket == 0))
//
//			{
//				succes = false;
//				break;
//			}
//		}
//
//	}
	return true;
}

/************************************************
 *	Stop data streaming from device.
 ************************************************/
bool ORDeviceSkeletonHardware::StopDataStream()
{
	// TODO: ask server to stop sending data
	// нужно опустошить буфер принимаемых данных

	//bool nothingToRead = false;



//	for (int i = 0; i < mChannelCount; i++)

//	{
//
//		nothingToRead = true;
//
//		if (mSkDataBuffer.mChannel[i].socket) // нулевые сокеты будут отброшены
//		{
//
//			int tmpByteCount = 0;
//
//
//			if (mSkDataBuffer.mChannel[i].mTCP.Read(mSkDataBuffer.mChannel[i].socket, (uint8_t*)&mESPDataBuffer, sizeof(mESPDataBuffer), &tmpByteCount));
//			{
//
//				nothingToRead = false;
//				//break;
//
//			}
//
//		}
//
//	}



	return true;
}
