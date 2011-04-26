/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

 The functions names in this header have to be implmented for each microcontroller individually

 Copyright (C) 2008 N. Michael Mayer, nmmayer@gmail.com

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; version 2 of the License.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

//!    HMDP Compatibility Layer:

#ifndef ___COMPATIBLE_RR___
#define ___COMPATIBLE_RR___

//! This is the set of functions and constants used by the hmdp_c.c and base.c functions. They have to be implemented for each robot individually

//! Interrupt related functions

// #1
void disableIRQ();
// #2
void enableIRQ();

// !Communication over with the the client
// #3
int sendByte(int data);
// #4
int readByte();
// #5
void clearBuffer();

//! possibly useful to allow direct communication between client and servos using the >$ command
// #6
int sendBytetoMo(int);

//! These commands have to be implemented to control the servos
//! For all servos it is assummed that 0 corresponds to the minimal value and 4096 corresponds to the maximal value

// #7
void send_servo_off(int ID); // ! turn servo off
// #8
void send_servo_on(int ID); // ! turn servo on
// #9
void send_servo_to_pos(int ID, int pos); // ! commands
// #10
void send_servo_to_gain(int ID, int gain);

// #11
int read_back_pos(int ID); // ! read current servo pos from poti value
// #12
int read_back_pos_set(int ID); // ! read targert position from servo
// #13
int read_back_id(int ID); // ! check if a servo with the particular ID is present


/*! for all things mentioned below this point:
 HMDP internally counts the servo from lower numbers to higher numbers thus a robot with servo
 IDs 1,3,8,15 internally has the IDs 0,1,2,3
 the arrays: jointnames[][8]; and zer_pos_inits_feed */

// #14
extern char jointnames[][8]; // ! names of the joints
//  ! can be read out by: <Nxx and used by tools e.g. for transferring motion patterns from one robot to another
/*! Naming Conventions: NNXYPD[D]  NN: Limb name that is: HE: Head, RA: right arm RL: Right leg, etc.
 X number of the major joint position counted from the body center: hip, shoulder, neck = 1;
 knee, elbow         = 2;
 foot, wrist         = 3;
 Y number of the hingjoint within the joint -- differs in dependence of the design
 P parity -- if the robot executes the same motion according to reflection symmetry, i.e. all R become L if the motion stays
 the same with parity +
 is inverted in the case of parity -
 D direction in Pitch (P), Roll (R), Yaw (Y), diagonal joints may use double letters e.g. PY
 numbering convention: IF THE ROBOT HAS GAPS BETWEEN SERVO NUMBERS:
 */

// #15
extern int zero_pos_inits_feed[];
/*! Zero positions i.e. the values that should be send to the servos of the robot to set it into an initial posture,
 a posture from which the motions deviate. In some robots it is necessary */

#endif  // ___COMPATIBLE_RR___
