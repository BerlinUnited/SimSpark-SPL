/***************************************************************************
 * Base routines of the command parser (without the HMDP commands)         *
 * Part of the Microcontroller Basimplementation of HMDP                   *
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

//

#include "base.h"
#include "hmdp_c.h"
#include "compatible.h"

Base_data *base_data; // pointer to current instantiation of the data


void sendMesg(const char *str) //! uses sendByte to make messages...
{
    while (*str)
    {
        sendByte(*str);
        str++;
    }
}

void write_int(int in) //! write out integer to master
{
    char xx[] =
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    data2hex(8, in, xx);
    sendMesg(xx);
}

void set_current_servo_pos_as_zero()
{
    int i;
    init_servo_list();
    for (i = 1; i <= base_data->servo_list_base[0]; ++i)

        base_data->zero_pos_inits_feed[base_data->servo_list_base[i]]
                = read_back_pos_set(base_data->servo_list_base[i]);
    base_data->zero_pos_inits = &(base_data->zero_pos_inits_feed[0]);
    sendMesg("Current zero pos is now zero\r\n");
}

void init_base()
{
    base_data->op_state = 0;
    base_data->echo_on = 0;
    base_data->bbase = 0;
    init_servo_list(); /*! search for all servos */
    //init_hmdl();        /*! in arm.c */
    sendMesg("# HMDP 0.3 \n");
}

int servo_list_in_hex(char *li)
{

    init_servo_list();
    int i;
    for (i = 0; i <= base_data->servo_list_base[0]; ++i)
    {
        data2hex(2, base_data->servo_list_base[i], &li[2* i ]);
    }
    li[base_data->servo_list_base[0] * 2 + 2] = 0;
    return 0;
}

void send_servo_list()
{
    char list[200];
    servo_list_in_hex(list);

    sendByte('!'); //!Data indikator

    sendMesg(list);
    sendByte(13);
    sendByte(10);
}

int init_servo_list() //!gives back
//!(size, id, id...)
{

    char i;
    int pos = 1;
    for (i = 0; i < 62; ++i)
    {
        if (read_back_id(i))
        {
            base_data->servo_list_base[pos] = i;
            pos++;
        }
    }
    base_data->servo_list_base[0] = pos - 1;
    return 0;
}
;

void eval_set_echo_on_off(const char in[])
/*! >E command one can turn on/off the echo */
{
    if (in[1] == 'N')
        base_data->echo_on = 0;
    if (in[1] == 'F')
        base_data->echo_on = -1;

}
;

void eval_send_gen_message(const char in[])
/*! >$ command allows direct access to the servos */
{
    int si = hex2data(2, in);
    int i;
    for (i = 0; i < si; i++)
    {
        char me = hex2data(2, in + 2 + 2* i );
        sendBytetoMo(me);

    };
}
;

void eval_set_position_message(const char in[])
/*! >P command  sets servo to position */
{
    int servoid, value;
    servoid = hex2data(2, in);
    value = hex2data(4, in + 2);
    value += base_data->zero_pos_inits[servoid]
            - base_data->zero_pos_inits_feed[servoid];
    send_servo_to_pos(servoid, value);
}
;

void eval_set_min_max_message(const char in[])
//! >M message -- for setting up min and max values
{
    int servoid, minvalue, maxvalue;
    servoid = hex2data(2, in);
    base_data->servo_min_val[servoid] = hex2data(4, in + 2);
    base_data->servo_max_val[servoid] = hex2data(4, in + 6);
    char eins[5];
    eins[4] = 0;
    sendMesg("\n");
    data2hex(4, base_data->servo_min_val[servoid], eins);
    sendMesg(eins);
    data2hex(4, base_data->servo_max_val[servoid], eins);
    sendMesg(eins);
    sendMesg("\n");
}
;

void eval_set_state_message(const char in[])
//! >S message
{
    base_data->op_state = hex2data(2, in);

    sendMesg("\r\n");
    write_int(base_data->op_state);
    sendMesg("\r\n");
}
;

void eval_set_gain_message(const char in[])
//! set gain for servo
{
    int servoid, value;
    servoid = hex2data(2, in);
    value = hex2data(2, &in[2]);
    send_servo_to_gain(servoid, value);

}
;

void eval_set_time(const char in[])
//! set time
{
    long time = hex2data(8, in);
    set_hmdl_time(time);
}
;

void send_time()
//! read time from HMDP and send it to the client
{
    char mes[12] =
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    data2hex(8, get_hmdl_time(), mes);
    sendMesg("!");
    sendMesg(mes);
    sendMesg("\r\n");

    data2hex(8, get_hmdl_time_inc(), mes);
    sendMesg("inc: ");
    sendMesg(mes);
    sendMesg("\r\n");

}
;

void eval_set_servo_on_off(const char in[])
//! check for >ON or >OFF message both start with the the letter O
{

    int servoid;
    sendMesg("in on off\n");
    switch (in[0])
    {
        case 'N': //! >ON##

            servoid = hex2data(2, &in[1]);
            send_servo_on(servoid);
            break;

        default: //! >OFF##
            servoid = hex2data(2, &in[2]);
            send_servo_off(servoid);
            break;
    }

}
;

void eval_get_jname(const char in[])
/*! reads the name of the servo form the preprogrammed list defined in
 compatible.h */
{

    int servoid, position, i;

    sendByte('!'); //Data indikator

    if (jointnames != NULL)
    {

        if (in[0] == 'v') //! all joints
        {
            //init_servo_list();
            for (i = 0; i < base_data->servo_list_base[0]; ++i)
            {

                sendMesg(jointnames[i]);
                sendByte(':');

            };

        }
        else
        {
            servoid = hex2data(2, &in[0]);

            sendMesg(jointnames[servoid]);

        };
    }
    sendByte(13);
    sendByte(10);

}

void eval_get_pos_message(const char in[])
/* <P command  read serv-pos from  potis */
{

    int servoid, position, i;

    sendByte('!'); //Data indikator

    if (in[0] == 'v')
    {
        //init_servo_list();
        for (i = 0; i < base_data->servo_list_base[0]; ++i)
        {
            servoid = base_data->servo_list_base[i + 1];
            position = read_back_pos(servoid);
            position -= base_data->zero_pos_inits[servoid]
                    - base_data->zero_pos_inits_feed[servoid]; //ACHTUNG

            char xx[] =
                { 0, 0, 0, 0, 0 };
            data2hex(4, position, xx);
            sendMesg(xx);

        };
        sendByte(13);
        sendByte(10);

    }
    else
    {
        servoid = hex2data(2, &in[0]);
        position = read_back_pos(servoid);
        position -= base_data->zero_pos_inits[servoid]
                - base_data->zero_pos_inits_feed[servoid]; //ACHTUNG

        char xx[] =
            { 0, 0, 0, 0, 13, 0 };
        data2hex(4, position, xx);
        sendMesg(xx);
        //uart0SendByte('*');
        sendByte(13);
        sendByte(10);
    };
}
;

void eval_get_pos_set_message(const char in[])
/*! <R get target value */
{

    int servoid, position, i;

    sendByte('!'); //Data indikator

    if (in[0] == 'v')
    {
        init_servo_list();
        for (i = 0; i < base_data->servo_list_base[0]; ++i)
        {
            servoid = base_data->servo_list_base[i + 1];
            position = read_back_pos_set(servoid);
            char xx[] =
                { 0, 0, 0, 0, 0 };
            data2hex(4, position, xx);
            sendMesg(xx);

        };
        //uart0SendByte('*');
        sendByte(13);
        sendByte(10);

    }
    else
    {

        servoid = hex2data(2, &in[0]);
        position = read_back_pos_set(servoid);
        char xx[] =
            { 0, 0, 0, 0, 13, 0 };
        data2hex(4, position, xx);
        sendMesg(xx);
        sendByte(13);
        sendByte(10);
    }
}
;

void send_zero_pos()
{

    int i;
    sendByte('!');
    for (i = 0; i < base_data->servo_list_base[0]; ++i)
    {
        unsigned char servoid = base_data->servo_list_base[i + 1];
        int zeropos = base_data->zero_pos_inits_feed[servoid]; //ACHTUNG  DEBUG

        char xx[] =
            { 0, 0, 0, 0, 0 };
        data2hex(4, zeropos, xx);
        sendMesg(xx);

    };
    sendByte(13);
    sendByte(10);

}
; // send zero pos feed


void plastic_state()
/*! timer call for plastic state */
{
    init_servo_list();
    int i;
    int j, val;
    for (i = 1; i <= base_data->servo_list_base[0]; ++i)
    {

        j = base_data->servo_list_base[i];
        val = read_back_pos(j);
        send_servo_to_pos(j, val);
    };
}
;

/* first level parser for base.c commands (<..., >...., @....)*/
int eval_seq_base(const char in[])
{

    int found_command = 0;
    found_command = -1;
    switch (in[0])
    {
        case '>':
            disableIRQ();

            switch (in[1])
            {
                case '$':
                    eval_send_gen_message(&in[2]); /* >$ (gen messg command */
                case 'S':
                    eval_set_state_message(&in[2]); /* >S command */
                    break;
                case 'P':
                    eval_set_position_message(&in[2]); /* >P command */
                    break;
                case 'G':
                    eval_set_gain_message(&in[2]); /* >G command */
                    break;
                case 'O':
                    eval_set_servo_on_off(&in[2]); /* >O command */
                    break;
                case 'M':
                    eval_set_min_max_message(&in[2]); /* >M command */
                    break;
                case 'T':
                    eval_set_time(&in[2]); /* >T command */
                    break;
                case 'E':
                    eval_set_echo_on_off(&in[2]); /* >E command */
                    break;
                case 'Z':
                    set_current_servo_pos_as_zero();
                    break;
            }
            enableIRQ();
            break;
        case '<':

            switch (in[1])
            {
                case 'N':
                    disableIRQ();
                    eval_get_jname(&in[2]); /* <N command: get the names of the servos */
                    break;
                case 'P':
                    disableIRQ();
                    eval_get_pos_message(&in[2]); /* <P command */
                    break;
                case 'R':
                    disableIRQ();
                    eval_get_pos_set_message(&in[2]); /* <R command */
                    break;
                case 'L':
                    disableIRQ();
                    send_servo_list(); /* <L command servo list*/
                    break;
                case 'T':
                    disableIRQ();
                    send_time();
                    break;
                case 'Z':
                    send_zero_pos(); // send zero pos feed
                    break;

            }
            enableIRQ();
        default:
            found_command = -1;
            break;
    }
    return found_command;
}
;

/* sending motion to servo ...*/

void send_hmdp_motion_to_servo()
{
    int i;
    for (i = 0; i < base_data->servo_list_base[0]; ++i)
    {

        int ID = base_data->servo_list_base[i + 1];
        int pos = base_data->zero_pos_inits[(int) base_data->servo_list_base[i
                + 1]] + get_hmdl_servo_out(i);

        send_servo_to_pos((int) base_data->servo_list_base[i + 1], pos);
    };

}
;

/*! interrupt routine for plastic state */
void inter_routine_state_2()
{
    plastic_state();
}
;

/*! interrupt routine for HMDP runnung */
void inter_routine_state_1()
{
    // if ((get_hmdl_time_inc()!=0))
    // {
    calc_next_timer_value();
    send_hmdp_motion_to_servo();
    motion_machine(calc_next_timer_value());

}
;

/*! main interrupt routine */
void inter_routine_base()
{
    if (base_data->op_state == 1)
        inter_routine_state_1();
    if (base_data->op_state == 2)
        inter_routine_state_2();

}
;

int main_eval(char *buff)
{

    if (eval_seq_base(buff) == -1)
    {
        //sendMesg ("\n");
        eval_seq(buff); // HMDP  parser in hmdp_c.c

    }
    return 0;
}

//! main parser
int parse_one_line()
{
    char in[3];
    in[0] = 0;
    in[1] = 0;
    int buff_c = 0;
    int a = 0;

    while (a != 13)
    {
        //	led1_off();
        a = readByte();
        ;
        if (a != -1)
        {
            //	    led1_on();
            base_data->buff[base_data->bbase + buff_c] = a;
            buff_c++;
            in[0] = a;
            if (base_data->echo_on == 0)
                sendByte(a);

        };
    };

    if (buff_c > 5) //! Checksum feature
    {
        if ((base_data->buff[base_data->bbase + buff_c - 4] == 'C')
                && (base_data->buff[base_data->bbase + buff_c - 3] == 'S'))
        {
            int sum = 0;
            int i;
            for (i = 0; i < buff_c - 4; ++i)
                sum += base_data->buff[base_data->bbase + i];
            sum %= 15;
            int sum2 = hex2data(1, &(base_data->buff[base_data->bbase + buff_c
                    - 2]));
            if (sum == sum2)
            {
                sendMesg("\r\n*\r\n");
            }
            else
            {
                sendMesg("\r\nE\r\n");
            };
            buff_c -= 3;

        };
    };
    if (buff_c > 1) //! Long message feature
    {
        if (base_data->buff[base_data->bbase + buff_c - 2] == '&')
        {
            base_data->bbase += buff_c - 2;
            sendMesg("add line \n");
        }
        else
        {
            base_data->buff[base_data->bbase + buff_c] = 13;
            base_data->buff[base_data->bbase + buff_c + 1] = 0;

            base_data->bbase = 0;

        }

    }
    else
    {

        base_data->buff[base_data->bbase + buff_c] = 13;
        base_data->buff[base_data->bbase + buff_c + 1] = 0;
        base_data->bbase = 0;
    }
    if (base_data->bbase == 0)
    {
        sendMesg("\r\n");

        main_eval(base_data->buff); // see base.c
        clearBuffer();
    };
    return 0;
}

