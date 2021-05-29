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

/**	\file	ordeviceskeleton_layout.cxx
*	Definition of a layout class for a simple skeleton device.
*	Definition of the functions of the ORDeviceSkeletonLayout class,
*	representing the layout for a simple skeleton device.
*/

//--- Class declarations
#include "ordeviceskeleton_device.h"
#include "ordeviceskeleton_layout.h"
#include "windows.h"

#define ORDEVICESKELETON_LAYOUT	ORDeviceSkeletonLayout

//--- FiLMBOX implementation and registration
FBDeviceLayoutImplementation(	ORDEVICESKELETON_LAYOUT		);
FBRegisterDeviceLayout		(	ORDEVICESKELETON_LAYOUT,
								ORDEVICESKELETON_CLASSSTR,
								FB_DEFAULT_SDK_ICON			);	// Icon filename (default=Open Reality icon)

/************************************************
 *	FiLMBOX constructor.
 ************************************************/
bool ORDeviceSkeletonLayout::FBCreate()
{
	// Get a handle on the device.
	mDevice = ((ORDeviceSkeleton *)(FBDevice *)Device);

	// Create/configure UI
	UICreate	();
	UIConfigure	();
	UIReset		();

	// Add device & system callbacks
	mDevice->OnStatusChange.Add	( this,(FBCallback)&ORDeviceSkeletonLayout::EventDeviceStatusChange  );
	mSystem.OnUIIdle.Add		( this,(FBCallback)&ORDeviceSkeletonLayout::EventUIIdle              );

	return true;
}


/************************************************
 *	FiLMBOX destructor.
 ************************************************/
void ORDeviceSkeletonLayout::FBDestroy()
{
	// Remove device & system callbacks
	mSystem.OnUIIdle.Remove		( this,(FBCallback)&ORDeviceSkeletonLayout::EventUIIdle              );
	mDevice->OnStatusChange.Remove	( this,(FBCallback)&ORDeviceSkeletonLayout::EventDeviceStatusChange  );
}


/************************************************
 *	Create the UI.
 ************************************************/
void ORDeviceSkeletonLayout::UICreate()
{
	int lS, lH;		// space, height
	lS = 4;
	lH = 25;

	// Create regions
	AddRegion	( "TabPanel",	"TabPanel",		0,		kFBAttachLeft,		"",			1.00,
												0,		kFBAttachTop,		"",			1.00,
												0,		kFBAttachRight,		"",			1.00,
												lH,		kFBAttachNone,		NULL,		1.00 );
	AddRegion	( "MainLayout",	"MainLayout",	lS,		kFBAttachLeft,		"TabPanel",	1.00,
												lS,		kFBAttachBottom,	"TabPanel",	1.00,
												-lS,	kFBAttachRight,		"TabPanel",	1.00,
												-lS,	kFBAttachBottom,	"",			1.00 );

	// Assign regions
	SetControl	( "TabPanel",	mTabPanel		);
	SetControl	( "MainLayout",	mLayoutMarkers	);

	// Create sub layouts
	UICreateLayout0();
	UICreateLayout1();
}


/************************************************
 *	Create the markers layout.
 ************************************************/
void ORDeviceSkeletonLayout::UICreateLayout0()
{
	// Add regions
	mLayoutMarkers.AddRegion( "SpreadMarkers", "SpreadMarkers",
													0,		kFBAttachLeft,		"",		1.00,
													0,		kFBAttachTop,		"",		1.00,
													0,		kFBAttachRight,		"",		1.00,
													0,		kFBAttachBottom,	"",		1.00 );

	// Assign regions
	mLayoutMarkers.SetControl( "SpreadMarkers", mSpreadMarkers );
}


/************************************************
 *	Create the setup layout.
 ************************************************/
void ORDeviceSkeletonLayout::UICreateLayout1()
{
	int lS, lW, lH;		// space, width, height.
	lS = 4;
	lW = 250;
	lH = 18;

	// Add regions
	mLayoutSetup.AddRegion (	"LabelSetup",	"LabelSetup",
													lS,		kFBAttachLeft,		"",		1.00,
													lS,		kFBAttachTop,		"",		1.00,
													lW,		kFBAttachNone,		NULL,	1.00,
													lH,		kFBAttachNone,		NULL,	1.00 );
	// Assign regions
	mLayoutSetup.SetControl(	"LabelSetup",	mLabelSetup );

	// Add regions
	mLayoutSetup.AddRegion (	"CharacterizeButton",	"CharacterizeButton",
													lS,		kFBAttachLeft,		"",		1.00,
													lS,		kFBAttachBottom,	"LabelSetup",		1.00,
													lW,		kFBAttachNone,		NULL,	1.00,
													lH,		kFBAttachNone,		NULL,	1.00 );
	// Assign regions
	mLayoutSetup.SetControl(	"CharacterizeButton",	mButtonCharacterize );

	// Add regions
	mLayoutSetup.AddRegion("ResetButton", "ResetButton",
		lS, kFBAttachLeft, "", 1.00,
		lS, kFBAttachBottom, "CharacterizeButton", 1.00,
		lW, kFBAttachNone, NULL, 1.00,
		lH, kFBAttachNone, NULL, 1.00);
	// Assign regions
	mLayoutSetup.SetControl("ResetButton", mButtonReset);




	int lS_y	= -15;
	lW			= 100;
	lH			= 25;
	int lHlr	= 100;
	int lWlr	= 250;
	int lWrb	= 140;
	int lSlbx	= 30;
	int lSlby	= 12;
	int lWlb	= 80;

	mLayoutSetup.AddRegion( "NetworkSetup",	"NetworkSetup",
													lS,		kFBAttachLeft,		"",						1.00,
													15,		kFBAttachBottom,	"ResetButton",	1.00,
													lWlr,	kFBAttachNone,	NULL,					1.00,
													lHlr,	kFBAttachNone,	NULL,					1.00 );

	// Add regions (network)
	mLayoutCommunication.AddRegion( "LayoutRegionNetwork",	"LayoutRegionNetwork",
													5,		kFBAttachLeft,		"",						1.00,
													15,		kFBAttachTop,		"NetworkSetup",			1.00,
													-10,	kFBAttachWidth,		"NetworkSetup",			1.00,
													-30,	kFBAttachHeight,	"NetworkSetup",			1.00 );
	mLayoutCommunication.AddRegion( "ButtonNetworkEnable",	"ButtonNetworkEnable",
													lS,		kFBAttachLeft,		"LayoutRegionNetwork",	1.00,
													lS_y,	kFBAttachTop,		"LayoutRegionNetwork",	1.00,
													lWrb,	kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "LabelNetworkAddress",		"LabelNetworkAddress",
													lSlbx,	kFBAttachLeft,		"LayoutRegionNetwork",	1.00,
													lSlby,	kFBAttachTop,		"LayoutRegionNetwork",	1.00,
													lWlb,	kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "EditNetworkAddress",		"EditNetworkAddress",
													lS,		kFBAttachRight,		"LabelNetworkAddress",	1.00,
													0,		kFBAttachTop,		"LabelNetworkAddress",	1.00,
													lW,		kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "LabelNetworkPort",	"LabelNetworkPort",
													0,		kFBAttachLeft,		"LabelNetworkAddress",	1.00,
													lS,		kFBAttachBottom,	"LabelNetworkAddress",	1.00,
													0,		kFBAttachWidth,		"LabelNetworkAddress",	1.00,
													0,		kFBAttachHeight,	"LabelNetworkAddress",	1.00 );
	mLayoutCommunication.AddRegion( "EditNetworkPort",	"EditNetworkPort",
													lS,		kFBAttachRight,		"LabelNetworkPort",		1.00,
													0,		kFBAttachTop,		"LabelNetworkPort",		1.00,
													lW,		kFBAttachNone,		NULL,					1.00,
													0,		kFBAttachHeight,	"LabelNetworkPort",		1.00 );

	// Assign regions (network)
	mLayoutCommunication.SetControl( "ButtonNetworkEnable",		mButtonNetworkEnable	);
	mLayoutCommunication.SetControl( "LabelNetworkAddress",		mLabelNetworkAddress	);
	mLayoutCommunication.SetControl( "EditNetworkAddress",		mEditNetworkAddress		);
	mLayoutCommunication.SetControl( "LabelNetworkPort",		mLabelNetworkPort		);
	mLayoutCommunication.SetControl( "EditNetworkPort",			mEditNetworkPort		);

	mLayoutSetup.SetControl	( "NetworkSetup",	mLayoutCommunication	);
}


/************************************************
 *	Configure the UI elements (main layout).
 ************************************************/
void ORDeviceSkeletonLayout::UIConfigure()
{
	SetBorder ("MainLayout", kFBStandardBorder, false,true, 1, 0,90,0);

	mTabPanel.Items.SetString("Markers~Setup");
	mTabPanel.OnChange.Add( this, (FBCallback)&ORDeviceSkeletonLayout::EventTabPanelChange );

	UIConfigureLayout0();
	UIConfigureLayout1();
}


/************************************************
 *	Configure the UI elements (marker layout).
 ************************************************/
void ORDeviceSkeletonLayout::UIConfigureLayout0()
{
}


/************************************************
 *	Configure the UI elements (setup layout).
 ************************************************/
void ORDeviceSkeletonLayout::UIConfigureLayout1()
{
	mLabelSetup.Caption = "Character setup according to device template";
	mButtonCharacterize.Caption = "Characterize";
	mButtonCharacterize.OnClick.Add( this, (FBCallback)&ORDeviceSkeletonLayout::EventSetupCharacter );

	mButtonReset.Caption = "Reset";
	mButtonReset.OnClick.Add(this, (FBCallback)&ORDeviceSkeletonLayout::EventReset);


    mLayoutCommunication.SetBorder( "LayoutRegionNetwork",	kFBEmbossBorder,false,true,2,1,90.0,0);

	mButtonNetworkEnable.Caption	= "Use skeleton test server";
	mButtonNetworkEnable.Style		= kFBCheckbox;
	mButtonNetworkEnable.State		= mDevice->GetNetworkUse();

	mButtonNetworkEnable.OnClick.Add( this,(FBCallback) &ORDeviceSkeletonLayout::EventButtonNetworkEnableClick );

	mLayoutCommunication.SetBorder( "EditNetworkAddress",	kFBStandardBorder, false,true, 1, 0,90,0);
	mLayoutCommunication.SetBorder( "EditNetworkPort",		kFBStandardBorder, false,true, 1, 0,90,0);


	mLabelNetworkAddress.Caption	= "Address :";
	mEditNetworkAddress.Text		= mDevice->GetNetworkAddress();
	mEditNetworkAddress.OnChange.Add( this, (FBCallback) &ORDeviceSkeletonLayout::EventEditNetworkAddressChange );

	mLabelNetworkPort.Caption		= "Port :";

	char	lBuffer[40];
	sprintf(lBuffer, "%d", mDevice->GetNetworkPort() );
	mEditNetworkPort.Text = lBuffer;
	mEditNetworkPort.OnChange.Add( this, (FBCallback) &ORDeviceSkeletonLayout::EventEditNetworkPortChange );
}


/************************************************
 *	Refresh the UI.
 ************************************************/
void ORDeviceSkeletonLayout::UIRefresh()
{
	UIRefreshSpreadSheet();
}


/************************************************
 *	Refresh the spreadsheet content.
 ************************************************/
void ORDeviceSkeletonLayout::UIRefreshSpreadSheet()
{
  	for(int i=0; i<mDevice->GetChannelCount();i++)
	{
		mSpreadMarkers.SetCell(i, 0, mDevice->GetStatus(i));
		mSpreadMarkers.SetCell( i, 1, mDevice->GetDataTX(i) );
		mSpreadMarkers.SetCell( i, 2, mDevice->GetDataTY(i) );
		mSpreadMarkers.SetCell( i, 3, mDevice->GetDataTZ(i) );
		mSpreadMarkers.SetCell( i, 4, mDevice->GetDataRX(i) );
		mSpreadMarkers.SetCell( i, 5, mDevice->GetDataRY(i) );
		mSpreadMarkers.SetCell( i, 6, mDevice->GetDataRZ(i) );
	}
}


/************************************************
 *	Reset the UI values from device.
 ************************************************/
void ORDeviceSkeletonLayout::UIReset()
{
	UIResetSpreadSheet();
}


/************************************************
 *	Re-build the spreadsheet.
 ************************************************/
void ORDeviceSkeletonLayout::UIResetSpreadSheet()
{
	int i;

	mSpreadMarkers.Clear();

	// Spreadsheet
	int lColumnIndex = -1;
	mSpreadMarkers.GetColumn(lColumnIndex).Width = 200;

	// column 0: Translation X
	mSpreadMarkers.ColumnAdd("Online");
	lColumnIndex++;
	mSpreadMarkers.GetColumn(lColumnIndex).Width = 60;
	mSpreadMarkers.GetColumn(lColumnIndex).Style = kFBCellStyleDouble;
	
	// column 0: Translation X
	mSpreadMarkers.ColumnAdd ("PosX");
	lColumnIndex++;
	mSpreadMarkers.GetColumn(lColumnIndex).Width = 60;
	mSpreadMarkers.GetColumn(lColumnIndex).Style = kFBCellStyleDouble;

	// column 1: Translation Y
	mSpreadMarkers.ColumnAdd ("PosY");
	lColumnIndex++;
	mSpreadMarkers.GetColumn(lColumnIndex).Width = 60;
 	mSpreadMarkers.GetColumn(lColumnIndex).Style = kFBCellStyleDouble;

 	// column 2: Translation Z
	mSpreadMarkers.ColumnAdd ("PosZ");
	lColumnIndex++;
	mSpreadMarkers.GetColumn(lColumnIndex).Width = 60;
	mSpreadMarkers.GetColumn(lColumnIndex).Style = kFBCellStyleDouble;

	// column 3: Rotation X
	mSpreadMarkers.ColumnAdd ("RotX");
	lColumnIndex++;
	mSpreadMarkers.GetColumn(lColumnIndex).Width = 60;
	mSpreadMarkers.GetColumn(lColumnIndex).Style = kFBCellStyleDouble;

	// column 4: Rotation Y
	mSpreadMarkers.ColumnAdd ("RotY");
	lColumnIndex++;
	mSpreadMarkers.GetColumn(lColumnIndex).Width = 60;
 	mSpreadMarkers.GetColumn(lColumnIndex).Style = kFBCellStyleDouble;

 	// column 5: Rotation Z
	mSpreadMarkers.ColumnAdd ("RotZ");
	lColumnIndex++;
	mSpreadMarkers.GetColumn(lColumnIndex).Width = 60;
	mSpreadMarkers.GetColumn(lColumnIndex).Style = kFBCellStyleDouble;

	for (i=0;i<mDevice->GetChannelCount();i++)
	{
 		mSpreadMarkers.RowAdd( mDevice->GetChannelName(i), i );
	 	mSpreadMarkers.GetCell(i,lColumnIndex).ReadOnly = true;
	}
}


/************************************************
 *	Tab panel change callback.
 ************************************************/
void ORDeviceSkeletonLayout::EventTabPanelChange( HISender pSender, HKEvent pEvent )
{
	switch( mTabPanel.ItemIndex )
	{
		case 0:	SetControl("MainLayout", mLayoutMarkers			);	break;
		case 1:	SetControl("MainLayout", mLayoutSetup		);	break;
	}
}


/************************************************
 *	Server port change callback.
 ************************************************/
void ORDeviceSkeletonLayout::EventButtonNetworkEnableClick( HISender pSender, HKEvent pEvent )
{
	mDevice->SetNetworkUse(((int)mButtonNetworkEnable.State)==1);
}

/************************************************
 *	Server port change callback.
 ************************************************/
void ORDeviceSkeletonLayout::EventEditNetworkPortChange( HISender pSender, HKEvent pEvent )
{
	int		lPort;
	char	lBuffer[40];

	sscanf(mEditNetworkPort.Text.AsString(), "%d", &lPort);
	mDevice->SetNetworkPort(lPort);
	lPort = mDevice->GetNetworkPort();
	sprintf(lBuffer, "%d", lPort );
	mEditNetworkPort.Text = lBuffer;
}


/************************************************
 *	Server port address callback.
 ************************************************/
void ORDeviceSkeletonLayout::EventEditNetworkAddressChange( HISender pSender, HKEvent pEvent )
{
	mDevice->SetNetworkAddress(mEditNetworkAddress.Text.AsString());
	mEditNetworkAddress.Text = mDevice->GetNetworkAddress();
}

/************************************************
 *	Device status change callback.
 ************************************************/
void ORDeviceSkeletonLayout::EventDeviceStatusChange( HISender pSender, HKEvent pEvent )
{
	UIReset();
}


/************************************************
 *	UI Idle callback.
 ************************************************/
void ORDeviceSkeletonLayout::EventUIIdle( HISender pSender, HKEvent pEvent )
{
	if( mDevice->Online )
	{
		UIRefresh();
	}
}

//simple name mapping.
static void SetupMapping(FBCharacter* pCharacter, FBModel* pParent)
{
    FBString lName = (const char*)pParent->Name; lName += "Link";
	FBProperty* lLink = pCharacter->PropertyList.Find(lName,false);
	if(lLink && lLink->IsList())
	{
		((FBPropertyListComponent*)lLink)->Add(pParent);
	}
	
	//do the same for children
	for(int i = 0; i < pParent->Children.GetCount(); i++)
	{
		SetupMapping(pCharacter,pParent->Children[i]);
	}
}

/************************************************
 *	Characterize binding models.
 ************************************************/
void ORDeviceSkeletonLayout::EventSetupCharacter( HISender pSender, HKEvent pEvent )
{
	if(mDevice && mDevice->ReadyForCharacterize())
	{
		FBCharacter* lCharacter = new FBCharacter("DeviceCharacter");

		FBModel* lReference = mDevice->mRootTemplate->Model;
		
		if(lReference)
		{
			SetupMapping(lCharacter,lReference);
			lCharacter->SetCharacterizeOn(true);
		}
	}
}

/************************************************
 *	Reset sensors
 ************************************************/
void ORDeviceSkeletonLayout::EventReset(HISender pSender, HKEvent pEvent)
{
	Sleep(10);


//	int nnelCount = mDevice->mHardware.mChannelCount;

	if (mDevice)
	{
		Sleep(10);
		mDevice->mHardware.ResetSensors();

	}

}
