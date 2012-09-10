/***************************************************************************
 * base.h for the Harmonic Motion Description Protocol (HMDP)              *
 * N. Michael Mayer, 2007 email: nmmayer@gmail.com                         *
 * This part of the program is intended to be used in the micro-controller *
 ***************************************************************************/

/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

 this file is part of rcssserver3D

 Copyright (C) 2002,2003 Koblenz University
 Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
 Copyright (C) 2008 N. Michael Mayer, email: nmmayer@gmail.com


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

#ifndef __HMDP_BASE__
#define __HMDP_BASE__

#define MAX_ALLOWED_SERVOS 64
#define MAX_PARSER_BUFFER_SIZE 200

typedef struct
{
    /*! instantiation of the state variable

     */
    int op_state; // main state variable HMDP off=0, HMDP on =1, plastic =2
    int servo_min_val[MAX_ALLOWED_SERVOS + 1]; // sets a min and a max value for each joint
    int servo_max_val[MAX_ALLOWED_SERVOS + 1];
    char servo_list_base[MAX_ALLOWED_SERVOS + 1]; // reserved memory for the joint ids
    int *zero_pos_inits; // pointer to current zero positions
    int zero_pos_inits_feed[MAX_ALLOWED_SERVOS + 1]; // ram list of the zero positions
    int bbase; // pointer to the current position in the parser buffer
    char buff[MAX_PARSER_BUFFER_SIZE]; // parser buffer
    int echo_on; // echo flag -- for terminal communication

} Base_data; // all "global" data put for base.c


// functions in base.c


void init_base();
int parse_one_line();
void inter_routine_base();

int init_servo_list();
int servo_list_in_hex(char *li);
int eval_seq_base(const char in[]);
void send_hmdp_motion_to_servo();
void write_int(int in);

extern char jointnames[MAX_ALLOWED_SERVOS][8];

#ifndef NULL
#define NULL 0
#endif

#endif
