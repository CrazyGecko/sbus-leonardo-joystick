//!*****************************************************************************
//!  \file      WirelessJoystick.cpp
//!*****************************************************************************
//!
//!  \brief		Balluff BNI0088 Smartlight specific driver, not implemented yet
//!
//!  \author    Janik Lehmann (CrazyGecko)
//!
//!  \date      2020-10-19
//!
//! Copyright (C) 2020 Janik Lehmann
//! 
//! This program is free software: you can redistribute it and/or modify it under 
//! the terms of the GNU General Public License as published by the Free Software
//! Foundation, either version 3 of the License, or (at your option) any later
//! version.
//! 
//! This program is distributed in the hope that it will be useful, but WITHOUT
//! ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
//! FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//! 
//! You should have received a copy of the GNU General Public License along with this
//! program. If not, see <https://www.gnu.org/licenses/>.
//!
//!*****************************************************************************

//#define DEBUG
//#define READ_CHANNEL_OUT

#include "SBUS.h"
#include "Joystick.h"
#ifdef DEBUG
#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX
#endif
SBUS sbus(Serial1);

#define CHANNEL_NUMB  16
#define MIN_VALUE 172
#define MAX_VALUE 1811
#define CALC_255 6.42 //(MAX_VALUE - MIN_VALUE) / 255
#define CALC_360 4.56 //(MAX_VALUE - MIN_VALUE) / 359


int channel[CHANNEL_NUMB];
#define X_ACHSE 1
#define Y_ACHSE 2
#define Z_ACHSE 3
#define X_ROTATION 4
#define Y_ROTATION 5
#define Z_ROTATION 6
#define THROTTLE 7
#define RUDDER 8
#define BUTTON_1 9
#define BUTTON_2 10
#define BUTTON_3 11
#define BUTTON_4 12
#define BUTTON_5 13
#define BUTTON_6 14
#define BUTTON_7 15
#define BUTTON_8 16

void setup() {
  sbus.begin(false);
 
  #ifdef DEBUG
  mySerial.begin(4800);
  #endif
  
  // Initialize Joystick Library
  Joystick.begin(false);
}

void loop() {
  sbus.process();
  for(unsigned char i=0; i<CHANNEL_NUMB; i++)
  {
    channel[i] = sbus.getChannel(i+1);
  }
  #ifndef READ_CHANNEL_OUT
  Joystick.setXAxis(get_channel_127(X_ACHSE));
  Joystick.setYAxis(get_channel_127(Y_ACHSE));
  Joystick.setZAxis(get_channel_127(Z_ACHSE));
  
  Joystick.setXAxisRotation(get_channel_360(X_ROTATION));
  Joystick.setYAxisRotation(get_channel_360(Y_ROTATION));
  Joystick.setZAxisRotation(get_channel_360(Z_ROTATION));
  
  Joystick.setThrottle(get_channel_255(THROTTLE));
  Joystick.setRudder(get_channel_255(RUDDER));
  
  Joystick.setButton(0, get_channel_1(BUTTON_1));
  Joystick.setButton(1, get_channel_1(BUTTON_2));
  Joystick.setButton(2, get_channel_1(BUTTON_3));
  Joystick.setButton(3, get_channel_1(BUTTON_4));
  Joystick.setButton(4, get_channel_1(BUTTON_5));
  Joystick.setButton(5, get_channel_1(BUTTON_6));
  Joystick.setButton(6, get_channel_1(BUTTON_7));
  Joystick.setButton(7, get_channel_1(BUTTON_8));


  Joystick.sendState();
  #else
  mySerial.println(channel[0]);
  delay(100);
  #endif

  #ifdef DEBUG
  mySerial.print(channel[BUTTON_1]);
  mySerial.print("    ");
  mySerial.println(get_channel_1(BUTTON_1));
  delay(100);
  #endif
 
}

int get_channel_255(int channel_number)
{
   return (int) lround((channel[channel_number - 1]-MIN_VALUE) / CALC_255);
}
int get_channel_360(int channel_number)
{
   return (int) lround((channel[channel_number - 1]-MIN_VALUE) / CALC_360);
}
int get_channel_127(int channel_number)
{
   return (int) get_channel_255(channel_number) - 127;
}
int get_channel_1(int channel_number)
{
  if(channel[channel_number-1]>1000)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
