/***************************************************************************
 * Header for the Harmonic Motion Description Protocol (HMDP)              *
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

#ifndef ___HMDP_C___
#define ___HMDP_C___

struct c_float /* Float Wrapper */
{
    int mantisse;
    short exponent;
};

struct c_f_hex
{
    char stri[15];
};

#define C_FLOAT_LENGTH (1+8+1+2) // SMMMMMMMMSEEY Sign, Exponent, Mantisse, Y=Y
#define HMDL_MESSAGE_START "";
/*! Header for all HMDL Messages when they are sent over the serial bus */

#define MAX_WLQ_DIGITS 6
/*! Number of hex digits reserved for the Frequencies = wavelengthquarters: WLQs */

#define MAX_TIME_DIGITS 8

#define MAX_COE_DIGITS 2
/*! Hex Digits reverved for the number of coeffieicnets */

#define MAX_ID_DIGITS 2
/*! Maximal Number of Digits of the ID; Since the ID can be up to 127 this
 means it is 2 */

#define MAX_MPS 67
#define MAX_MPS_IN_RAM 67
/*! Maximal numbers of Motion patterns that can be present
 at one time in the SH2*/
#define MAX_SERVOS 22
/*! Maximal number of servos */
#define MAX_COEFFS 11
/*!maximal number of coefficients*/
#define MAX_ALARM_POINTS 1
/*! maximal number of points were the status can be readout */

#define TM_BUFFER_SIZE 1024
/*! Size of the List of timer tasks, see @... commands */

struct motion_pattern_servo
/* Structure that contains data about the motion of one servo */
{
    struct c_float s_position[MAX_COEFFS];
    /*struct c_float s_gain [MAX_COEFFS];*/
};

struct motion_pattern_props
{
    int wlqs_top[((MAX_COEFFS - 1) / 2)];
    int wlqs_bot[((MAX_COEFFS - 1) / 2)];
    struct motion_pattern_servo sv[MAX_SERVOS];
    int ID_prop;
};

struct motion_pattern
/* Structure that contains all data necessary for a specific motion pattern  */
{
    struct motion_pattern_props *pp; //[1];
    struct c_float A_new;
    struct c_float A_old;
    // struct c_float A_curr;
    unsigned long start_0, start_1, end_0, end_1,// interpolate amplitude from start_0 to start_1
            offset, // used to offset the pattern when started
            offsetoverflow; // used to save the offset after the time wraps around
    unsigned short updated, // pattern was changed
            active; // pattern is active

};

struct message_alarm_clock
{
    unsigned long alarm_time;
    void (*callf)();
};

typedef struct
/* all data necessary for the complete harmonic motion description
 language */
{
    unsigned long time, time_max, time_inc;
    struct motion_pattern mp[MAX_MPS]; /* -> sv -> s_position */
    struct message_alarm_clock alarm[MAX_ALARM_POINTS];
    struct c_float positions_to_set[MAX_SERVOS];
    struct motion_pattern_props mpp_in_ram[MAX_MPS_IN_RAM];

} Hmdl;

struct c_f_hex c_float2hex(struct c_float in);
struct c_float float2cfloat(float a);
struct c_float mult_cc(struct c_float a, struct c_float b);
// multiply 2 c_floats
struct c_float mult_ccc(struct c_float a, struct c_float b, struct c_float d);
// multiply 3 c-fs
struct c_float mult_cc_sinus(struct c_float a, struct c_float b, int d);
// multiply 2 c-fs and the output of the sine_fixed routine (d parameter)
struct c_float mult_c_sinus(struct c_float a, int d);
// multiply c-f and the output of the sine_fixed routine (d parameter)


struct c_float c_f_sum(struct c_float a[], int size);
// sum a list of c_floats
struct c_float hex2c_float(const char in[]);
// read a cfloat from ascii with special format
int cfloat2int(struct c_float a);
// make a cloat to int (cut behind the point = floor)
/*struct c_float cfloat2int (int a);*/
// interpolation function
/* 1. parameter size, 2. Amplitude over all, 3. time, 4. Coeffs array of c_float,
 5. frequencies (frequency * time -> parameter to sine_fixed routines*/

struct c_float interpolate_rational_c_float( /*Real amplitude interpolation*/
int size_c, struct c_float amp, int time, struct c_float coeffs[],
                                            unsigned int frequencies_up[],
                                            unsigned int frequencies_bottom[]);

struct c_float interpolate_c_float(int, struct c_float, int,
                                   struct c_float coeffs[],
                                   unsigned int frequencies[]);

struct c_float interpolate_c_float_phase(int, struct c_float amps[], int,
                                         struct c_float coeffs[],
                                         unsigned int frequencies[]);

void eval_seq(const char in[]);
/*! input interpreter of HMDP*/

/* Inititalize variables, start timer, etc */
void init_hmdl();

/* Clean up */
void end_hmdl();

/*! Handle situtations when the time needs to wrap around since time_max has
 been reached. Offset values have to be saved for the motion patterns, and
 the update flag needs to be set */
void handle_overflow(void);

void motion_machine(unsigned int time);

/*! motion control - time management+*

 This function is triggered by an interrupt and call the motion machine for each time step.
 It will also set the values for the motors, take care of time wrap-arounds and offset adjustments,
 as well as keep track of alarm time requests to deliver feedback to the PC at specified times */
void motion_control(int val);

// extern int timer_running;
int calc_next_timer_value();

long get_hmdl_time(); //! wrapper for time
void set_hmdl_time(long time);
long get_hmdl_time_inc(); //! wrapper for increment
short get_hmdl_servo_out(int i); //! result of HMDP calcs


//! conversion int to hex and vice versa
int hex2data(short size_v, const char res[]);
void data2hex(short size_v, const int data, char res[]);
int c_abs(int in);

#endif

