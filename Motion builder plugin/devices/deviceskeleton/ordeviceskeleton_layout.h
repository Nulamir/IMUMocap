#ifndef __ORDEVICE_SKELETON_LAYOUT_H__
#define __ORDEVICE_SKELETON_LAYOUT_H__
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

/**	\file	ordeviceskeleton_layout.h
*	Declaration for the layout of a simple skeleton device.
*	Contains the declaration for the layout of a simple input
*	device class, ORDeviceSkeletonLayout.
*/

//--- Class declaration
#include "ordeviceskeleton_device.h"

//! Simple device layout.
class ORDeviceSkeletonLayout : public FBDeviceLayout
{
	//--- FiLMBOX declaration.
	FBDeviceLayoutDeclare( ORDeviceSkeletonLayout, FBDeviceLayout );

public:
	//--- FiLMBOX Creation/Destruction.
	virtual bool FBCreate();			//!< FiLMBOX constructor.
	virtual void FBDestroy();			//!< FiLMBOX destructor.

	// UI Management
	void	UICreate				();		// Create & assign UI regions
	void		UICreateLayout0		();
	void		UICreateLayout1		();
	void	UIConfigure				();		// Configure UI options
	void		UIConfigureLayout0	();
	void		UIConfigureLayout1	();
	void	UIRefresh				();		// Refresh on idle
	void		UIRefreshSpreadSheet();
	void	UIReset					();		// Reset from device values
	void		UIResetSpreadSheet	();

	// Main Layout: Events
	void	EventDeviceStatusChange					( HISender pSender, HKEvent pEvent );
	void	EventUIIdle								( HISender pSender, HKEvent pEvent );
	void	EventTabPanelChange						( HISender pSender, HKEvent pEvent );

	// Setup Layout: Events
	void	EventSetupCharacter						( HISender pSender, HKEvent pEvent );
	void	EventReset								( HISender pSender, HKEvent pEvent);
	void	EventButtonNetworkEnableClick			( HISender pSender, HKEvent pEvent );
	void	EventEditNetworkAddressChange			( HISender pSender, HKEvent pEvent );
	void	EventEditNetworkPortChange				( HISender pSender, HKEvent pEvent );

private:
	FBTabPanel			mTabPanel;

	FBLayout			mLayoutMarkers;
		FBSpread			mSpreadMarkers;

	FBLayout			mLayoutSetup;
		FBLabel				mLabelSetup;
		FBButton			mButtonCharacterize;
		FBButton			mButtonReset;

	FBLayout			mLayoutCommunication;
		FBButton			mButtonNetworkEnable;
			FBLabel				mLabelNetworkAddress;
			FBEdit				mEditNetworkAddress;
			FBLabel				mLabelNetworkPort;
			FBEdit				mEditNetworkPort;

private:
	FBSystem				mSystem;					//!< System interface.
	ORDeviceSkeleton*			mDevice;					//!< Handle onto device.
};

#endif /* __ORDEVICE_SKELETON_LAYOUT_H__ */
