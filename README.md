# IMUMocap
Motion capture suit with IMU sensor 6050 and data transfer with esp8866 to Autodesk MotionBuilder.
My project is inspired by https://github.com/herzig/bewegungsfelder , https://github.com/nkeeline/KeeCap-Open-Motion-Capture-Suit and https://github.com/OlafHaag/MotionBuilder-LiveSkeleton-Plugin

My project consists of the following:
1. The mpu6050 sensors generate data about their rotation from the moment they are turned on
2. esp8866 collects this data and sends it to the BWFGserver via wifi

