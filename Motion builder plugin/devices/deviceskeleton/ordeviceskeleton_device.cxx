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

/**	\file	ordeviceskeleton_device.cxx
*	Definition of a simple skeleton device class.
*	Function definitions for the ORDeviceSkeleton class.
*/

//--- Class declaration
#include "ordeviceskeleton_device.h"

//--- Registration defines
#define ORDEVICESKELETON_CLASS		ORDEVICESKELETON_CLASSNAME
#define ORDEVICESKELETON_NAME		ORDEVICESKELETON_CLASSSTR
#define ORDEVICESKELETON_LABEL		"OR - Skeleton Device"
#define ORDEVICESKELETON_DESC		"OR - Skeleton Device"
#define ORDEVICESKELETON_PREFIX		"SkeletonDevice"
#define ORDEVICESKELETON_ICON		"devices_body.png"

//--- FiLMBOX implementation and registration
FBDeviceImplementation	(	ORDEVICESKELETON_CLASS	);
FBRegisterDevice		(	ORDEVICESKELETON_NAME,
							ORDEVICESKELETON_CLASS,
							ORDEVICESKELETON_LABEL,
							ORDEVICESKELETON_DESC,
							ORDEVICESKELETON_ICON	);

/************************************************
 *	FiLMBOX Constructor.
 ************************************************/
bool ORDeviceSkeleton::FBCreate()
{
	// Add model templates
	mRootTemplate =  new FBModelTemplate(ORDEVICESKELETON_PREFIX, "Reference", kFBModelTemplateRoot);
	ModelTemplate.Children.Add(mRootTemplate);
	mHierarchyIsDefined = false;
	mHasAnimationToTranspose = false;
	mPlotting = false;

	// Device sampling information
	SamplingMode = kFBHardwareTimestamp;
	SamplingPeriod = FBTime(0,0,1)/sSKDataBuffer::SIM_FPS;

	FBPropertyPublish( this, UseReferenceTransformation, "UseReferenceTransformation", NULL, NULL );
	UseReferenceTransformation = false; //nulamir

	FBSystem().TheOne().OnUIIdle.Add( this,(FBCallback) &ORDeviceSkeleton::EventUIIdle );

	return true;
}


/************************************************
 *	FiLMBOX Destructor.
 ************************************************/
void ORDeviceSkeleton::FBDestroy()
{
    // Propagate to parent
    ParentClass::FBDestroy();
	FBSystem().TheOne().OnUIIdle.Remove( this,(FBCallback) &ORDeviceSkeleton::EventUIIdle );
}

/************************************************
 *	Device operation.
 ************************************************/
bool ORDeviceSkeleton::DeviceOperation( kDeviceOperations pOperation )
{
	// Must return the online/offline status of device.
	switch (pOperation)
	{
		case kOpInit:	return Init();
		case kOpStart:	return Start();
		case kOpAutoDetect:	break;
		case kOpStop:	return Stop();
		case kOpReset:	return Reset();
		case kOpDone:	return Done();
	}
    return ParentClass::DeviceOperation(pOperation);
}


/************************************************
 *	Initialize the device.
 ************************************************/
bool ORDeviceSkeleton::Init()
{
    FBProgress	lProgress;

    lProgress.Caption	= "Device";
	lProgress.Text		= "Initializing device...";

	if(!mHardware.GetSetupInfo())
	{
		Information = "Could not get channel information from device.";
		return false;
	}

	mHierarchyIsDefined = false;

	Bind();

	return true;
}


/************************************************
 *	Removal of device.
 ************************************************/
bool ORDeviceSkeleton::Done()
{
	FBProgress	lProgress;

    lProgress.Caption	= "Device";
	lProgress.Text		= "Shutting down device...";

	UnBind();
	
	/*
	*	Add device removal code here.
	*/

	return true;
}

/************************************************
 *	Reset of the device.
 ************************************************/
bool ORDeviceSkeleton::Reset()
{
    Stop();
    return Start();
}

/************************************************
 *	Device is started (online).
 ************************************************/
bool ORDeviceSkeleton::Start()
{
	FBProgress	Progress;

	Progress.Caption	= "Setting up device";

	// Step 1: Open device
	if(! mHardware.Open() )
	{
		Information = "Could not open device";
		return false;
	}

	// Step 2: Ask hardware to get channel information
	Progress.Text	= "Device found, scanning for channel information...";
	Information		= "Retrieving channel information";

	// Step 3: Start data stream
	if(! mHardware.StartDataStream() )
	{
		Information = "Could not start data stream.";
		return false;
	}

    return true; // if true the device is online
}

/************************************************
 *	Device is stopped (offline).
 ************************************************/
bool ORDeviceSkeleton::Stop()
{
	FBProgress	lProgress;
	lProgress.Caption	= "Shutting down device";

	if(! mHardware.StopDataStream() )
	{
		Information = "Could not stop data stream.";
		return false;
	}

	// Step 2: Close down device
	lProgress.Text		= "Closing device communication";
	Information			= "Closing device communication";
	if(! mHardware.Close() )
	{
		Information = "Could not close device";
		return false;
	}

    return true;
}

/************************************************
 *	Define model template hierarchy.
 ************************************************/
void ORDeviceSkeleton::DefineHierarchy()
{
	if( !mHierarchyIsDefined && GetChannelCount() > 0 )
	{
		int lParent;
		for(int i=0; i< GetChannelCount(); i++)
		{
			lParent = GetChannelParent(i);
			if(lParent == -1)
			{
				mRootTemplate->Children.Add(mChannels[i].mModelTemplate);
			}
			else
			{
				mChannels[lParent].mModelTemplate->Children.Add(mChannels[i].mModelTemplate);
			}
		}

		mHierarchyIsDefined = true;
	}
}

/************************************************
 *	Test if characterization process can be start
 ************************************************/
bool ORDeviceSkeleton::ReadyForCharacterize()
{
	return mHierarchyIsDefined && GetChannelCount() > 0;
}

/************************************************
 *	Process global data on template models to local.
 ************************************************/
void ORDeviceSkeleton::ProcessGlobalToLocal()
{
	SetupLocalGlobal(true);

	int i;
	FBModelList lModels;
	FBModel* lModel;
	
	FBGetSelectedModels(lModels);
	for(i = 0; i < lModels.GetCount(); i++)
		lModels[i]->Selected = false;

	lModels.Clear();
	for(i = 0; i < GetChannelCount(); i++)
	{
		if(mChannels[i].mModelTemplate && (lModel = mChannels[i].mModelTemplate->Model) != NULL)
		{
			lModel->Selected = true;
			lModels.Add(lModel);
		}
	}

	mPlotting = true;
	mSystem.CurrentTake->PlotTakeOnSelected(SamplingPeriod);
	mPlotting = false;

	for(i = 0; i < lModels.GetCount(); i++)
		lModels[i]->Selected = false;	

	SetupLocalGlobal(false);

    bool ApplyReferenceTransformation = UseReferenceTransformation && mRootTemplate->Model;

// Clear Translation and Rotation
	if(ApplyReferenceTransformation)
	{
		FBVector4d x;
		mRootTemplate->Model->Translation.SetData(&x);
		mRootTemplate->Model->Rotation.SetData(&x);
	}
}

void ORDeviceSkeleton::SetupLocalGlobal(bool pGlobal)
{
	for( int i = 0; i < GetChannelCount(); i++ )
	{
		if( mChannels[i].mTAnimNode )
		{
			mChannels[i].mTAnimNode->SetBufferType(pGlobal);
		}

		if( mChannels[i].mRAnimNode )
		{
			mChannels[i].mRAnimNode->SetBufferType(pGlobal);
		}
	}
}

/************************************************
 *	Model Template unbinding notification callback
 ************************************************/
bool ORDeviceSkeleton::ModelTemplateUnBindNotify( int pIndex, FBModelTemplate* pModelTemplate)
{
	// Reset skeleton by reevaluating a candidate because of Local/Global animation conversion

	bool ObjectInProcess = GetObjectStatus(kFBStatusCreating) || GetObjectStatus(kFBStatusDestroying) || GetObjectStatus(kFBStatusMerging);
	if(pModelTemplate->Model && pModelTemplate->Model == mRootTemplate->Model && !ObjectInProcess)
	{
		FBPlayerControl().TheOne().EvaluationPause();
		for( int i = 0; i < GetChannelCount(); i++ )
		{
			// Create new translation and rotation animation nodes if necessary
			if(mChannels[i].mTAnimNode)
			{
				mChannels[i].mModelTemplate->Model->Translation.SetAnimated(true);
				FBAnimationNode* lNode = mChannels[i].mModelTemplate->Model->Translation.GetAnimationNode();
				FBVector3d  lVector(mChannels[i].mModelTemplate->Model->Translation);
				lNode->SetCandidate(lVector.mValue);
			}

			if(mChannels[i].mRAnimNode)
			{
				mChannels[i].mModelTemplate->Model->Rotation.SetAnimated(true);
				FBAnimationNode* lNode = mChannels[i].mModelTemplate->Model->Rotation.GetAnimationNode();
				FBVector3d  lVector(mChannels[i].mModelTemplate->Model->Rotation);
				lNode->SetCandidate(lVector.mValue);
			}
		}
		FBPlayerControl().TheOne().EvaluationResume();
	}
	return true;
}

/************************************************
 *	End the channel set definition.
 ************************************************/
void ORDeviceSkeleton::Bind()
{
	int i;
	// Exit edit mode:
	// All used channels: if already defined, don't touch, if new: create animation node and model template
	// All unused channels: delete animation nodes and associated model template
    for( i = 0; i < GetChannelCount(); i++ )
	{
		// Create new translation and rotation animation nodes if necessary
		if( !mChannels[i].mTAnimNode )
		{
            // We must use a unique name for our connector.
            FBString lName( GetChannelName(i), " T" );
			mChannels[i].mTAnimNode = AnimationNodeOutCreate( 10000+i, lName, ANIMATIONNODE_TYPE_LOCAL_TRANSLATION	);
		}

		if( !mChannels[i].mRAnimNode )
		{
            // We must use a unique name for our connector.
            FBString lName( GetChannelName(i), " R" );
			mChannels[i].mRAnimNode = AnimationNodeOutCreate( 10000+i, lName, ANIMATIONNODE_TYPE_LOCAL_ROTATION	);
		}

		// Create new model templates
		if(!mChannels[i].mModelTemplate)
		{
			mChannels[i].mModelTemplate = new FBModelTemplate(ORDEVICESKELETON_PREFIX, GetChannelName(i), kFBModelTemplateSkeleton);
			// Bind model template to T and R animation nodes
			mChannels[i].mModelTemplate->Bindings.Add(mChannels[i].mTAnimNode);
			mChannels[i].mModelTemplate->Bindings.Add(mChannels[i].mRAnimNode);

			// Setting global values makes weird value when not live
			// mChannels[i].mModelTemplate->DefaultTranslation = mHardware.GetDefaultT(i);
			// mChannels[i].mModelTemplate->DefaultRotation = mHardware.GetDefaultR(i);
		}
    }

	//Define hierarchy if needed
	DefineHierarchy();
}

void ORDeviceSkeleton::UnBind()
{
	int i;
	for( i = 0; i < GetChannelCount(); i++ )
	{
		// Unbind model templates from T and R animation nodes
		if( mChannels[i].mTAnimNode )
		{
			if( mChannels[i].mModelTemplate )
			{
				mChannels[i].mModelTemplate->Bindings.Remove(mChannels[i].mTAnimNode);
			}
		}

		if( mChannels[i].mRAnimNode )
		{
			if( mChannels[i].mModelTemplate )
			{
				mChannels[i].mModelTemplate->Bindings.Remove(mChannels[i].mRAnimNode);
			}
		}

		// Remove as child of root template
		if( mRootTemplate->Children.Find(mChannels[i].mModelTemplate) >= 0 )
		{
			mRootTemplate->Children.Remove(mChannels[i].mModelTemplate);
		}

		// Destroy unused animation nodes
        if( mChannels[i].mTAnimNode )
		{
			AnimationNodeDestroy(mChannels[i].mTAnimNode);
		}

        if( mChannels[i].mRAnimNode )
		{
			AnimationNodeDestroy(mChannels[i].mRAnimNode);
		}

        mChannels[i].mTAnimNode = NULL;
        mChannels[i].mRAnimNode = NULL;

		if( mChannels[i].mModelTemplate )
		{
			mChannels[i].mModelTemplate->Children.RemoveAll();
		}
	}
	
    for( i = 0; i < GetChannelCount(); i++ )
	{
		// Delete model template
		delete mChannels[i].mModelTemplate;
		mChannels[i].mModelTemplate = NULL;
	}
}

/************************************************
 *	Real-time Evaluation Engine.
 ************************************************/
bool ORDeviceSkeleton::AnimationNodeNotify(FBAnimationNode* pAnimationNode ,FBEvaluateInfo* pEvaluateInfo)
{
	kReference lReference = pAnimationNode->Reference;

	if (lReference>=10000)
	{
	    FBTVector	Pos;
	    FBRVector	Rot;
	    FBSVector	Scal;
	    FBMatrix	GlobalNodeTransformation, GlobalReferenceTransformation;
	    bool		ApplyReferenceTransformation = UseReferenceTransformation && mRootTemplate->Model;
    
	    if(ApplyReferenceTransformation)
	    {
			    mRootTemplate->Model->GetMatrix(GlobalReferenceTransformation,kModelTransformation,true,pEvaluateInfo);
	    }
    
        for(int i=0;i<GetChannelCount();i++)
	    {
		    if(mChannels[i].mTAnimNode && mChannels[i].mRAnimNode)
		    {
			    bool lDontWrite = false;
			    if ((!pAnimationNode->Live || mPlotting) && mChannels[i].mModelTemplate->Model)
			    {
				    mChannels[i].mModelTemplate->Model->Translation.GetAnimationNode()->Evaluate(Pos.mValue,pEvaluateInfo->GetLocalTime(),false);
				    mChannels[i].mModelTemplate->Model->Rotation.GetAnimationNode()->Evaluate(Rot.mValue,pEvaluateInfo->GetLocalTime(),false);
			    } else if (pAnimationNode->Live)
			    {
				    Pos[0] = mHardware.GetDataTX(i);
				    Pos[1] = mHardware.GetDataTY(i);
				    Pos[2] = mHardware.GetDataTZ(i);
				    Rot[0] = mHardware.GetDataRX(i);
				    Rot[1] = mHardware.GetDataRY(i);
				    Rot[2] = mHardware.GetDataRZ(i);
			    } else
			    {
				    lDontWrite = true; // Nothing to do 
			    }
    
			    if(ApplyReferenceTransformation)
			    {
				    FBTRSToMatrix(GlobalNodeTransformation,Pos,Rot,Scal);
				    FBGetGlobalMatrix(GlobalNodeTransformation,GlobalReferenceTransformation,GlobalNodeTransformation);
				    FBMatrixToTranslation(Pos,GlobalNodeTransformation);
				    FBMatrixToRotation(Rot,GlobalNodeTransformation);
			    }
    
			    if(!lDontWrite)
			    {
				    if (!pAnimationNode->Live || mPlotting)
				    {
			            mChannels[i].mRAnimNode->WriteData( Rot.mValue, pEvaluateInfo);
			            mChannels[i].mTAnimNode->WriteData( Pos.mValue, pEvaluateInfo);
			        }  else
			        {
				        mChannels[i].mRAnimNode->WriteGlobalData( Rot.mValue, pEvaluateInfo );
				        mChannels[i].mTAnimNode->WriteGlobalData( Pos.mValue, pEvaluateInfo );
				    }
			    }
			}
		}
	}
	return ParentClass::AnimationNodeNotify( pAnimationNode , pEvaluateInfo);
}

/************************************************
 *	Transport notication.
 *	Useful to compute time offset between Hardware time and System time
 *  PreparePlay is called just before play
 ************************************************/
void ORDeviceSkeleton::DeviceTransportNotify( kTransportMode pMode, FBTime pTime, FBTime pSystem )
{
	if(pMode==kPreparePlay)
	{
		mHardware.ResetPacketTimeOffset(pTime);
	}
}

/************************************************
 *	Real-Time Synchronous Device IO.
 ************************************************/
void ORDeviceSkeleton::DeviceIONotify( kDeviceIOs pAction,FBDeviceNotifyInfo &pDeviceNotifyInfo)
{
    switch (pAction)
	{
		case kIOPlayModeWrite:
		case kIOStopModeWrite:
		{
			// Output devices
		}
		break;

		case kIOStopModeRead:
		case kIOPlayModeRead:
		{
			// Skeleton devices
	        FBTime		lEvalTime;
        
	        lEvalTime = pDeviceNotifyInfo.GetLocalTime();
	        while(mHardware.FetchDataPacket(lEvalTime))
	        {
				//mDevice.UIRefresh();
		        DeviceRecordFrame(lEvalTime,pDeviceNotifyInfo);
				AckOneSampleReceived();
	        }
			//AckOneSampleReceived();
		}
		break;
	}
}

void ORDeviceSkeleton::RecordingDoneAnimation( FBAnimationNode* pAnimationNode )
{
	// Parent call
	FBDevice::RecordingDoneAnimation( pAnimationNode );
	mHasAnimationToTranspose = true;
}

/************************************************
 *	Record a frame of the device (recording).
 ************************************************/
void ORDeviceSkeleton::DeviceRecordFrame(FBTime &pTime,FBDeviceNotifyInfo &pDeviceNotifyInfo)
{
	if( mPlayerControl.GetTransportMode() == kFBTransportPlay )
	{
		int i;
		FBAnimationNode* Data;

		FBTVector	Pos;
		FBRVector	Rot;
		bool		ApplyReferenceTransformation = UseReferenceTransformation && mRootTemplate->Model;
		
		FBMatrix	GlobalReferenceTransformation;
		if(ApplyReferenceTransformation)
			mRootTemplate->Model->GetMatrix(GlobalReferenceTransformation,kModelTransformation,true);

		for (i=0; i<GetChannelCount(); i++)
		{
			// Translation information.
			if (mChannels[i].mTAnimNode)
			{
				Data = mChannels[i].mTAnimNode->GetAnimationToRecord();
				if (Data)
				{
					Pos[0] = mHardware.GetDataTX(i);
					Pos[1] = mHardware.GetDataTY(i);
					Pos[2] = mHardware.GetDataTZ(i);

					if(ApplyReferenceTransformation)
						FBVectorMatrixMult(Pos,GlobalReferenceTransformation,Pos);

					Data->KeyAdd(pTime, Pos);
				}
			}

			// Rotation information.
			if (mChannels[i].mRAnimNode)
			{
				Data = mChannels[i].mRAnimNode->GetAnimationToRecord();
				if (Data)
				{
					Rot[0] = mHardware.GetDataRX(i);
					Rot[1] = mHardware.GetDataRY(i);
					Rot[2] = mHardware.GetDataRZ(i);

					if(ApplyReferenceTransformation)
					{
						FBMatrix GRM;
						FBRotationToMatrix(GRM,Rot);
						FBGetGlobalMatrix(GRM,GlobalReferenceTransformation,GRM);
						FBMatrixToRotation(Rot,GRM);
					}

					Data->KeyAdd(pTime, Rot);
				}
			}
		}
	}
}

/************************************************
 *	Store data in FBX.
 ************************************************/
bool ORDeviceSkeleton::FbxStore(FBFbxObject* pFbxObject,kFbxObjectStore pStoreWhat)
{
	if (pStoreWhat & kAttributes)
	{
	}
	return true;
}


/************************************************
 *	Retrieve data from FBX.
 ************************************************/
bool ORDeviceSkeleton::FbxRetrieve(FBFbxObject* pFbxObject,kFbxObjectStore pStoreWhat)
{
	if( pStoreWhat & kAttributes )
	{
	}
	return true;
}

void ORDeviceSkeleton::EventUIIdle( HISender pSender, HKEvent pEvent )
{
	if(mHasAnimationToTranspose)
	{
		mHasAnimationToTranspose = false;

		// Put the animation back on skeleton
		ProcessGlobalToLocal();
	}
}