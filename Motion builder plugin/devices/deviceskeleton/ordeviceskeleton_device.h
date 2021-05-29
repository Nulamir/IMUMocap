#ifndef __ORDEVICE_SKELETON_DEVICE_H__
#define __ORDEVICE_SKELETON_DEVICE_H__
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

/**	\file	ordeviceskeleton_device.h
*	Declaration of a simple skeleton device class.
*	Class declaration of the ORDeviceSkeleton class.
* 
* 
* более верхнего уровня работает с сущностями МБ
*/

//--- SDK include
#include <fbsdk/fbsdk.h>

//--- Class declaration
#include "ordeviceskeleton_hardware.h"

//--- Registration defines
#define ORDEVICESKELETON_CLASSNAME		ORDeviceSkeleton
#define ORDEVICESKELETON_CLASSSTR		"ORDeviceSkeleton"

//! Data channel class.
class DataChannel
{
public:
	//! Constructor.
	DataChannel()
	{
		mTAnimNode		= NULL;
		mRAnimNode		= NULL;
		mModelTemplate	= NULL;
	}

	//! Destructor.
	~DataChannel()
	{
	}

public:
	FBAnimationNode*	mTAnimNode;			//!< Position animation node.
	FBAnimationNode*	mRAnimNode;			//!< Rotation animation node.
	FBModelTemplate*	mModelTemplate;		//!< Marker model template driven by the data channel.
};


//! Simple skeleton device.
class ORDeviceSkeleton : public FBDevice
{
	//--- Declaration
	FBDeviceDeclare( ORDeviceSkeleton, FBDevice );

public:
	virtual bool FBCreate() override;
	virtual void FBDestroy() override;

    //--- Standard device virtual functions evaluation
	virtual bool DeviceOperation	( kDeviceOperations pOperation									) override;		//!< Operate device.
	virtual bool AnimationNodeNotify( FBAnimationNode* pAnimationNode,FBEvaluateInfo* pEvaluateInfo	) override;		//!< Real-time evaluation function.
	virtual void DeviceIONotify		( kDeviceIOs  pAction, FBDeviceNotifyInfo &pDeviceNotifyInfo	) override;		//!< Hardware I/O notification.
	virtual void DeviceTransportNotify( kTransportMode pMode, FBTime pTime, FBTime pSystem ) override;				//!< Transport notification.

	//--- Recording of frame information
	virtual void	RecordingDoneAnimation( FBAnimationNode* pAnimationNode) override;
	void	DeviceRecordFrame			(FBTime &pTime,FBDeviceNotifyInfo &pDeviceNotifyInfo);

	//--- Load/Save.
	virtual bool FbxStore	( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat ) override;	//!< FBX Storage.
	virtual bool FbxRetrieve( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat ) override;	//!< FBX Retrieval.

	//--- Initialisation/Shutdown
    bool	Init	();		//!< Initialize device.
    bool	Start	();		//!< Start device.
    bool	Stop	();		//!< Stop device.
    bool	Reset	();		//!< Reset device.
    bool	Done	();		//!< Device removal.

	//--- Attributes management
	void	SetNetworkUse(bool pUse)					{ mHardware.SetNetworkUse( pUse );				}
	bool	GetNetworkUse()								{ return mHardware.GetNetworkUse();				}
	void	SetNetworkAddress	(const char* pIPAddress){ mHardware.SetNetworkAddress(pIPAddress);		}
	const char*	GetNetworkAddress	()					{ return mHardware.GetNetworkAddress();			}
	void	SetNetworkPort		(int pPort)				{ mHardware.SetNetworkPort(pPort);				}
	int		GetNetworkPort		()						{ return mHardware.GetNetworkPort();			}

	//--- Get data from hardware.
	bool GetStatus(int pChannelIndex) { return mHardware.GetStatus(pChannelIndex); }
	double GetDataTX(int pChannelIndex)			{	return mHardware.GetDataTX(pChannelIndex);		}
	double GetDataTY(int pChannelIndex)			{	return mHardware.GetDataTY(pChannelIndex);		}
	double GetDataTZ(int pChannelIndex)			{	return mHardware.GetDataTZ(pChannelIndex);		}
	double GetDataRX(int pChannelIndex)			{	return mHardware.GetDataRX(pChannelIndex);		}
	double GetDataRY(int pChannelIndex)			{	return mHardware.GetDataRY(pChannelIndex);		}
	double GetDataRZ(int pChannelIndex)			{	return mHardware.GetDataRZ(pChannelIndex);		}

	//--- Marker set management.
	void	DefineHierarchy				();				//!< Define model template hierarchy.
	void	Bind();										//!< Bind models
	void	UnBind();									//!< Unbind models

	//--- Channel list manipulation.
	int		GetChannelCount()					{	return mHardware.GetChannelCount();				}
	int		GetChannelParent(int pMarkerIndex)	{	return mHardware.GetChannelParent(pMarkerIndex);}
	const char*	GetChannelName(int pMarkerIndex){	return mHardware.GetChannelName(pMarkerIndex);	}

    virtual bool ModelTemplateUnBindNotify( int pIndex, FBModelTemplate* pModelTemplate );							//!< Model Template unbinding notification callback.

	bool	ReadyForCharacterize		();				//!< Test if characterization process can be start.

	//--- Stop displaying process to local message on model unbind
	void	EventUIIdle( HISender pSender, HKEvent pEvent );

	ORDeviceSkeletonHardware	mHardware;					//!< Hardware abstraction object.
	FBModelTemplate*			mRootTemplate;				//!< Root model binding.
private:
	DataChannel					mChannels[sSKDataBuffer::ChannelCount];	//!< Data channels.
	FBPropertyBool				UseReferenceTransformation;	// !< Apply reference transformation on incoming global data.
	bool						mHierarchyIsDefined;		//!< True if the hierarchy is already defined

	FBPlayerControl				mPlayerControl;				//!< To get play mode for recording.
	FBSystem					mSystem;
	FBApplication				mApplication;
	bool						mHasAnimationToTranspose;
	bool						mPlotting;

	// Process global data on template models to local
	void		ProcessGlobalToLocal();
	void SetupLocalGlobal(bool pGlobal);
};

#endif /* __ORDEVICE_SKELETON_DEVICE_H__ */
