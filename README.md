# IMUMocap
https://youtu.be/pLOBQtpoAnA

I decided to create мotion capture suit with IMU sensor 6050 and data transfer with esp8866 to Autodesk MotionBuilder.
My project is inspired by https://github.com/herzig/bewegungsfelder , https://github.com/nkeeline/KeeCap-Open-Motion-Capture-Suit and https://github.com/OlafHaag/MotionBuilder-LiveSkeleton-Plugin

My project consists of the following:
1. The mpu6050 sensors generate data about their rotation from the moment they are turned on
2. esp8866 collects this data and sends it to the BWFGscanner via wifi
3. BWFGscanner collect data from all sensors and put it in the memory mapped file
4. ORdeviceskeleton read this data, use them to rotate and positioning bones
5. motionbuilder can display and capture motion troth ORdeviceskeleton


![promo](https://user-images.githubusercontent.com/50498587/120082315-e56d3b80-c0ca-11eb-8b7f-8e5a20e0032e.jpg)



Hello, how do you convert the sensor coordinate system to the model node coordinate system?I'm doing something similar to you, but there are some problems with coordinate system conversion.—

unswer

Hi,
IMU sensors generate quaternions and only rotation information, not position information.Autodesk Motion builder wait for euler angle and mdT (position of node)
Wee need to convert this data
Facts:1. Sensor initial state is start from lying on the table chip Z axis look in to roof. X and Y lying on the table. But on the motionbuilder model. Skeleton is faced to you. Z axis look on you.
2 Sensor physical implementation (crafted hard device) a unified (they are all likes each on enover), But on you body you need to hang sensor on stomach legs head vertically, and in left side of body sensor hanged one method on right side another
3 there is a little deviation then you hang sensors 10-15 degrees from strightly vertical or horizontal position 

All calculation made in ordeviceskeleton_hardware.cxx ORDeviceSkeletonHardware::FetchDataPacket(FBTime& pTime)
To get 1,2 Facts you need to remix and redirect axes of quaternion.
cQuaternion.mValue[1] = mSkDataBuffer.mChannel[i].iR[0][0] * tempx + mSkDataBuffer.mChannel[i].iR[0][1] * tempy + mSkDataBuffer.mChannel[i].iR[0][2] * tempz; // -tempz
			cQuaternion.mValue[2] = mSkDataBuffer.mChannel[i].iR[1][0] * tempx + mSkDataBuffer.mChannel[i].iR[1][1] * tempy + mSkDataBuffer.mChannel[i].iR[1][2] * tempz;//-tempx;
			cQuaternion.mValue[3] = mSkDataBuffer.mChannel[i].iR[2][0] * tempx + mSkDataBuffer.mChannel[i].iR[2][1] * tempy + mSkDataBuffer.mChannel[i].iR[2][2] * tempz;//tempy;

Data for how to remix axes stored in ordeviceskeleton_hardware .h void SetupBuffer()
in example of left foot^
		mChannel[3].iR[0][0] = -1; mChannel[3].iR[0][1] = 0; mChannel[3].iR[0][2] = 0; x axis reversed
		mChannel[3].iR[1][0] = 0; mChannel[3].iR[1][1] = 0; mChannel[3].iR[1][2] = 1;  y,z a swapped
		mChannel[3].iR[2][0] = 0; mChannel[3].iR[2][1] = 1; mChannel[3].iR[2][2] = 0;  y,z a swapped
Now you may hang your sensor on you comfort

To get 3 Factyou need to multiply correction quaternion on sensor quaternion
multiplyQuaternion(resultQuaternion, correctionQuaternion, cQuaternion);

Now we get Euler angels
getEulerFromQuaternion(cRotationOfNode, resultQuaternion, zyx);




Code below  get transformation data mT (position of node)from rotation of bones. code use sceleton data. length of bones they initial position.

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

						memcpy(&mSkDataBuffer.mChannel[children].mT, &lChannelTransformation[0], sizeof(mSkDataBuffer.mChannel[children].mT));  // here is updating transfers

						lIter = &mChannel[children];

						memcpy(lIter->mT, mSkDataBuffer.mChannel[children].mT, sizeof(lIter->mT));// значение ноды

					}

				}
			}







