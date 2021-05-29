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

/**	\file	ordeviceskeleton.cxx
*	Library declarations.
*	Contains the basic routines to declare the DLL as a loadable
*	library for FiLMBOX.
*/

//--- SDK include
#include <fbsdk/fbsdk.h>

//--- Library declaration
FBLibraryDeclare( ordeviceskeleton )
{
	FBLibraryRegister( ORDeviceSkeleton	);
	FBLibraryRegister( ORDeviceSkeletonLayout	);
}
FBLibraryDeclareEnd;

/************************************************
 *	Library functions.
 ************************************************/
bool FBLibrary::LibInit()	{ return true; }
bool FBLibrary::LibOpen()	{ return true; }
bool FBLibrary::LibReady()	{ return true; }
bool FBLibrary::LibClose()	{ return true; }
bool FBLibrary::LibRelease(){ return true; }

/**
*	\mainpage	Simple Skeleton Device
*	\section	intro	Introduction
*	This device serves as an example for any generic motion capture
*	device. This sample illustrates the use of dynamic allocation of
*	animation nodes/sensors. The UI illustrates the use of a spreadsheet.
*
*	\section	desc	Description
*	This tutorial covers a simple skeleton device, which simulates data
*	streaming in from hardware. In this example, a virtual hardware object
*	streams in the data corresponding to clock hands turning.
*	\section	walk	Walkthrough
*	Here are the steps to test the ORDeviceSkeleton.
*		<li>1. Create an instance of the device by dropping it from the asset browser onto the viewer
*		<li>2. Put the device online
*		<li>3. In the Model binding drop down choose "create ..."
*		<li>4. In the Setup tab click on the Characterize button
*		<li>5. Merge in the scene any characterized character like Mia or Aragor from the samples
*		<li>5. In the Character Controls select the input of the drop character as DeviceCharacter
*		<li>6. Go back to the Device, and put it live to see it rotating, animated by global data.
*	\subsection	func1	FBCreate()
*	<ol>
*		<li>Create a new ModelTemplate;
*		<li>Add the model template to the device model template;
*		<li>Assign the icon filename;
*		<li>Set the recording sampling mode;
*	</ol>
*	\subsection	func2	DeviceOperation()
*	<ol>
*		<li>Based on the requested operation, call the corresponding device function;
*		<li>Propagate the call to the parent class and return the result
*	</ol>
*	\subsection	func3	Stop()
*	<ol>
*		<li>Stop the data stream for the device;
*		<li>Close the hardware.
*		<li>Return false (the device is now offline).
*	</ol>
*	\subsection	func4	Start()
*	<ol>
*		<li>Set the network information (normally read from layout);
*		<li>Open hardware;
*		<li>Get the hardware setup;
*		<li>Begin channel set definition;
*		<li>Use the channels found;
*		<li>End the channel set definition;
*		<li>Define model template channel hierarchy;
*		<li>Start streaming data from device;
*		<li>Return true (device is now online).
*	</ol>
*	\subsection	func5	DefineHierarchy()
*	<ol>
*		<li>Build the hierarchy of model templates.
*	</ol>
*	\subsection	func6	Bind()
*	<ol>
*		<li>For all of the channels;
*		<ul>
*			<li>Create the translation and rotation animation nodes;
*			<li>Create and bind the model template for the channel.
*		</ul>
*	</ol>
*	\subsection	func7	UnBind()
*	<ol>
*		<li>Remove the binding and delete all animation nodes.
*	</ol>
*	\subsection	func8	AnimationNodeNotify()
*	<ol>
*		<li>Get data from hardware abstraction;
*		<li>Write data to animation nodes.
*	</ol>
*	\subsection	func9	DeviceIONotify()
*	<ol>
*		<li>For the number of packets waiting to be read;
*		<li>Read data packet (getting timestamp);
*		<li>Record packet(assigned to timestamp).
*	</ol>
*	\subsection	func10	DeviceRecordFrame()
*	<ol>
*		<li>For all the channels that are used;
*		<li>Get the animation node for recording from the node (translation or rotation);
*		<li>Get the data from the hardware abstraction;
*		<li>Based on the recording mode for the device;
*		<ul>
*			<li>Add a key for the data.
*		</ul>
*		<li>Acknowledge the receipt for statistics.
*	</ol>
*/
