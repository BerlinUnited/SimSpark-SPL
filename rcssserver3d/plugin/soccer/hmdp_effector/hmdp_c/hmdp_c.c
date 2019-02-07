#include "hmdp_c.h"
#include "sine_fixed.h"

Hmdl *hmdl; // hook for a pointer to all data


/*! Util routines to convert integers to hex strings  and vice versa */

int hex2data(short size_v, const char res[])
{

    int k;
    int data;
    data = 0;

    for (k = 0; k < size_v; k++)
    {

        data *= 16;
        switch (res[k])

        {
            case '0':
                break;
            case '1':
                data += 1;
                break;
            case '2':
                data += 2;
                break;
            case '3':
                data += 3;
                break;
            case '4':
                data += 4;
                break;
            case '5':
                data += 5;
                break;
            case '6':
                data += 6;
                break;
            case '7':
                data += 7;
                break;
            case '8':
                data += 8;
                break;
            case '9':
                data += 9;
                break;
            case 'a':
                data += 10;
                break;
            case 'b':
                data += 11;
                break;
            case 'c':
                data += 12;
                break;
            case 'd':
                data += 13;
                break;
            case 'e':
                data += 14;
                break;
            case 'f':
                data += 15;
                break;

        }

    };
    return data;
}
;

void data2hex(short size_v, const int dat, char res[])
{

    int k;
    int data = dat;
    for (k = size_v - 1; k >= 0; k--)
    {

        switch (data % 16)

        {
            case 0:
                res[k] = '0';
                break;
            case 1:
                res[k] = '1';
                break;
            case 2:
                res[k] = '2';
                break;
            case 3:
                res[k] = '3';
                break;
            case 4:
                res[k] = '4';
                break;
            case 5:
                res[k] = '5';
                break;
            case 6:
                res[k] = '6';
                break;
            case 7:
                res[k] = '7';
                break;
            case 8:
                res[k] = '8';
                break;
            case 9:
                res[k] = '9';
                break;
            case 10:
                res[k] = 'a';
                break;
            case 11:
                res[k] = 'b';
                break;
            case 12:
                res[k] = 'c';
                break;
            case 13:
                res[k] = 'd';
                break;
            case 14:
                res[k] = 'e';
                break;
            case 15:
                res[k] = 'f';
                break;

        }
        data /= 16;
    };

}
;

void set_hmdl_time(long time)
{
    hmdl->time = time;
}
;

long get_hmdl_time()
{
    return hmdl->time;
}
;

long get_hmdl_time_inc()
{
    return hmdl->time_inc;
}
;

short get_hmdl_servo_out(int i)
{
    return cfloat2int(hmdl->positions_to_set[i]);
}
;

void write_cfloat(struct c_float *in)
{

    struct c_f_hex in_hex = c_float2hex(*in);
    sendMesg(in_hex.stri);

}

#define INTERVAL 50             /* num of ms for the timer to go off */

/* function prototype */
void motion_control(int);

/*
 * global variables
 */

/* struct Hmdl hmdl;*/

struct c_f_hex c_float2hex(struct c_float in)
{
    short sign, esign, i;
    struct c_f_hex ret;
    for (i = 0; i < 15; ++i)
        ret.stri[i] = 0;

    if (in.mantisse < 0)
    {
        ret.stri[0] = '-';
        sign = -1;
    }
    else
    {
        sign = 1;
        ret.stri[0] = '+';
    };

    if (in.exponent < 0)
    {
        ret.stri[9] = '-';
        esign = -1;
    }
    else
    {
        esign = 1;
        ret.stri[9] = '+';
    };

    data2hex(8, sign * in.mantisse, &ret.stri[1]);
    /*ret.stri[9]='^';*/

    data2hex(2, esign * in.exponent, &ret.stri[10]);

    return ret;
}
;

struct c_float mult_cc(struct c_float a, struct c_float b)
{
    struct c_float c;
    int sign_c = 1;

    if (a.mantisse < 0)
    {
        sign_c = -1;
        a.mantisse = -a.mantisse;
    }

    if (b.mantisse < 0)
    {
        sign_c = -sign_c;
        b.mantisse = -b.mantisse;
    }

    a.mantisse = a.mantisse >> (INT_MAXE / 2);
    b.mantisse = b.mantisse >> (INT_MAXE / 2);

    c.exponent = a.exponent + b.exponent;
    c.mantisse = a.mantisse * b.mantisse * sign_c;

    if (c_abs(c.mantisse) < INT_MAXX)
    {
        c.exponent--;
        c.mantisse = c.mantisse << 1;
    };

    return c;
}
;

int cfloat2int(struct c_float a)
{
    int r;
    if (a.mantisse > 0)
        r = a.mantisse >> (INT_MAXE - a.exponent);
    else
    {
        r = -1* ( (-a.mantisse)>>(INT_MAXE-a.exponent));
    };
    return r;
};

struct c_float hex2c_float(const char in[])
{
    struct c_float ret;
    short sign, esign;
    if (in[0] == '-')
        sign = -1;
    else if (in[0] == '+')
        sign = +1;

    if (in[9] == '-')
        esign = -1;
    else if (in[9] == '+')
        esign = +1;

    ret.mantisse = sign * hex2data(8, &in[1]);
    ret.exponent = esign * hex2data(2, &in[10]);
    return ret;
}

int lo2(int in)
//! returns rough integer logarithm of in
{
    if (in >= 2048)
        return 11;
    if (in >= 1024)
        return 10;
    if (in >= 512)
        return 9;
    if (in >= 256)
        return 8;
    if (in >= 128)
        return 7;
    if (in >= 64)
        return 6;
    if (in >= 32)
        return 5;
    if (in >= 16)
        return 4;
    if (in >= 8)
        return 3;
    if (in >= 4)
        return 2;
    if (in >= 2)
        return 1;
    if (in < 2)
        return 0;
}
;

struct c_float c_f_sum(struct c_float a[], int size)
//! sum of size c_floats
{
    int i, max, lsize;
    lsize = lo2(size);
    struct c_float result;
    max = 0;
    result.mantisse = 0;

    for (i = 0; i < size; ++i)
        if (a[i].exponent > max)
            max = a[i].exponent;

    for (i = 0; i < size; ++i)
    {
        result.mantisse += (a[i].mantisse >> (max - a[i].exponent + lsize));
    };

    result.exponent = max + lsize;
    return result;
}
;

int c_abs(int in)
//! absolut value of integer
{
    if (in > 0)
        return in;
    else
        return -in;
}
;

struct c_float c_f_add(struct c_float s1, struct c_float s2)
//! add 2 c_floats
{
    int i, max, lsize;
    struct c_float result;
    max = 0;
    result.mantisse = 0;

    max = s1.exponent;
    if (s2.exponent > max)
        max = s2.exponent;

    if (s1.mantisse < 0)
        result.mantisse -= ((-s1.mantisse) >> (max - s1.exponent + 1));
    else
        result.mantisse += ((s1.mantisse) >> (max - s1.exponent + 1));

    if (s2.mantisse < 0)
        result.mantisse -= (-s2.mantisse >> (max - s2.exponent + 1));
    else
        result.mantisse += (s2.mantisse >> (max - s2.exponent + 1));

    result.exponent = max + 1;
    if (c_abs(result.mantisse) < INT_MAXX)
    {
        result.exponent--;
        if (result.mantisse > 0)
            result.mantisse = result.mantisse << 1;
        else
            result.mantisse = -1* ( (-result.mantisse) <<1);
        };

        return result;
    };

struct c_float mult_c_sinus(struct c_float a, int d)
//! multiply routine for sinus input c_float * sinus
{
    struct c_float c;

    int sign_c = 1;

    if (a.mantisse < 0)
    {
        sign_c = -1;
        a.mantisse = -a.mantisse;
    }

    if (d < 0)
    {
        sign_c = -sign_c;
        d = -d;
    }

    a.mantisse = a.mantisse >> (INT_MAXE / 2);
    d = d >> (INT_MAXE / 2);

    c.exponent = a.exponent;
    c.mantisse = a.mantisse * d * sign_c;

    return c;
}
;

struct c_float mult_cc_sinus(struct c_float a, struct c_float b, int d)
//! multiply 2 c_floats with one sinus
{
    struct c_float c;
    int sign_c = 1;

    if (a.mantisse < 0)
    {
        sign_c = -1;
        a.mantisse = -a.mantisse;
    }

    if (d < 0)
    {
        sign_c = -sign_c;
        d = -d;
    }

    if (b.mantisse < 0)
    {
        sign_c = -sign_c;
        b.mantisse = -b.mantisse;
    }

    a.mantisse = a.mantisse >> (2*INT_MAXE / 3);
    b.mantisse = b.mantisse >> (2*INT_MAXE / 3);
    d = d >> (2*INT_MAXE / 3);

    c.exponent = a.exponent + b.exponent;
    c.mantisse = a.mantisse * b.mantisse * d * sign_c;

    if (c_abs(c.mantisse) < INT_MAXX)
    {
        c.exponent--;
        if (c.mantisse > 0)
            c.mantisse = c.mantisse << 1;
        else
            c.mantisse = -1* ( (-c.mantisse) <<1);
        };

        return c;
    };

struct c_float set_c_float_zero()
// returns a c_float = 0
{
    struct c_float a;
    a.mantisse = 0;
    a.exponent = 0;
    return a;
}

struct c_float fade_in(struct c_float a, struct c_float b, long t_a, long t_b,
                       long t)
//! provides fading in for motion patterns (in a linear -- ramp like manner)
{

    int max_man;
    struct c_float c;
    max_man = a.exponent;
    int sign_a, sign_b;
    if (a.mantisse >= 0)
    {
        sign_a = 1;
    }
    else
    {
        sign_a = -1;
        a.mantisse = -a.mantisse;
    };

    if (b.mantisse >= 0)
    {
        sign_b = 1;
    }
    else
    {
        sign_b = -1;
        b.mantisse = -b.mantisse;
    };

    if (max_man < b.exponent)
        max_man = b.exponent;

    long long diff = sign_b * (b.mantisse >> (max_man - b.exponent)) - sign_a
            * (a.mantisse >> (max_man - a.exponent));
    diff *= (t - t_a);
    diff /= (t_b - t_a);

    c.exponent = max_man;
    c.mantisse = sign_a * a.mantisse + diff;

    return c;
}
;

void motion_machine(unsigned int time)
/*! interpolation using direct access to the hmdl structure */
{

    int i, k, l;
    struct c_float y, sine_amp, cos_amp;
    struct c_float amp;
    struct c_float *coeffs;

    /*! iterate over the servos and sum all values for the active motion patterns
     -> superposition */
    for (i = 0; i < MAX_SERVOS; ++i)
    {
        hmdl->positions_to_set[i] = set_c_float_zero(); // init servo_list

        /*! loop over all motion patterns, see which ones should be active */
        for (k = 0; k < MAX_MPS; ++k)
        {
            /*! skip pattern if amplitude is 0 or not active */
            if (!(hmdl->mp[k].active))

            {
                continue;
            }

            coeffs = hmdl->mp[k].pp->sv[i].s_position;

            if (time < hmdl->mp[k].start_0)
                amp = hmdl->mp[k].A_old;
            else if (time >= hmdl->mp[k].start_1)
                amp = hmdl->mp[k].A_new;
            else
                amp = fade_in(hmdl->mp[k].A_old, hmdl->mp[k].A_new,
                    hmdl->mp[k].start_0, hmdl->mp[k].start_1, time);

            /*if (hmdl->mp[k].end_0!=0)
             {
             if (time > hmdl->mp[k].start_1)
             {
             hmdl->mp[k].start_0=hmdl->mp[k].end_0;
             hmdl->mp[k].start_1=hmdl->mp[k].end_1;

             hmdl->mp[k].end_0=0;
             hmdl->mp[k].end_1=0;
             hmdl->mp[k].A_old=hmdl->mp[k].A_new;
             hmdl->mp[k].A_new=set_c_float_zero();
             };
             };*/

            if (hmdl->mp[k].end_0 != 0)
            {
                if (time > hmdl->mp[k].end_0)
                {
                    if (time > hmdl->mp[k].end_1)
                    {
                        //
                        amp = set_c_float_zero();
                        // hmdl->mp[k].active=0;
                    }

                    else
                    {
                        amp = fade_in(hmdl->mp[k].A_new, set_c_float_zero(),
                            hmdl->mp[k].end_0, hmdl->mp[k].end_1, time);

                    };

                };
            };

            //amp = hmdl->mp[k].A_curr;
            //if (i==20)
            //  {write_int(cfloat2int(amp)); DMESG("  :   ");}

            y = mult_cc(amp, coeffs[0]);
            //y = coeffs[0];
            //if (i==20) {
            //  write_int(cfloat2int(y)); DMESG("  :   ");}


            for (l = 0; l < (MAX_COEFFS - 1) / 2; ++l)
            {
                y = c_f_add(y, mult_cc_sinus(amp, coeffs[2* l + 1], sin_fixed(
                    ((time - hmdl->mp[k].offset) * hmdl->mp[k].pp->wlqs_top[l])
                            / hmdl->mp[k].pp->wlqs_bot[l])));

                y = c_f_add(y, mult_cc_sinus(amp, coeffs[2* l + 2], cos_fixed(
                    ((time - hmdl->mp[k].offset) * hmdl->mp[k].pp->wlqs_top[l])
                            / hmdl->mp[k].pp->wlqs_bot[l])));
            };

            hmdl->positions_to_set[i] = c_f_add(y, hmdl->positions_to_set[i]);

        };
    };
}
;

int calc_next_timer_value()
//! wrapper for time
{
    hmdl->time += hmdl->time_inc;
    return hmdl->time;
}

void eval_set_time_message(const char in[])
// TS HMDP command similar to >T command but also increment can be set
{
    hmdl->time = hex2data(MAX_TIME_DIGITS, in);
    hmdl->time_max = hex2data(MAX_TIME_DIGITS, &in[MAX_TIME_DIGITS]);
    hmdl->time_inc = hex2data(MAX_TIME_DIGITS, &in[2*MAX_TIME_DIGITS]);
    //timer_running=0;
    write_int(hmdl->time);
    sendMesg("\r\n");

}
;

void eval_set_alarm_point(const char in[])
// not implelemented so far
{

}
;

void eval_use_pattern_message(const char in[])
//! PU command
{
    int patternid, curr;
    patternid = hex2data(MAX_ID_DIGITS, in);

    hmdl->mp[patternid].start_0 = hex2data(MAX_TIME_DIGITS, &in[MAX_ID_DIGITS]);
    hmdl->mp[patternid].start_1 = hex2data(MAX_TIME_DIGITS, &in[MAX_ID_DIGITS
            + MAX_TIME_DIGITS]);
    hmdl->mp[patternid].offset = hex2data(MAX_TIME_DIGITS,
        &in[MAX_ID_DIGITS + 2*MAX_TIME_DIGITS]);

    hmdl->mp[patternid].end_0 = 0;
    hmdl->mp[patternid].end_1 = 0;

    curr = MAX_ID_DIGITS + 3*MAX_TIME_DIGITS;

    hmdl->mp[patternid].A_old = hmdl->mp[patternid].A_new;
    hmdl->mp[patternid].A_new = hex2c_float(&in[curr]);

    hmdl->mp[patternid].active = 1;

}
;

void eval_use_short_pattern_message(const char in[])
//! PP command - shorter PU message
//
{
    int patternid, curr;
    patternid = hex2data(MAX_ID_DIGITS, in);

    hmdl->mp[patternid].start_0 = 0;
    hmdl->mp[patternid].start_1 = 0;
    hmdl->mp[patternid].end_0 = 0;
    hmdl->mp[patternid].end_1 = 0;
    hmdl->mp[patternid].offset = 0;

    hmdl->mp[patternid].A_old = hmdl->mp[patternid].A_new;
    hmdl->mp[patternid].A_new.mantisse = 0x40000000;
    hmdl->mp[patternid].A_new.exponent = 0;
    hmdl->mp[patternid].active = 1;

}
;

void eval_use_pattern_message_with_end(const char in[])
//! PY sending a pattern with the ramp end
{
    int patternid, curr;
    patternid = hex2data(MAX_ID_DIGITS, in);

    hmdl->mp[patternid].A_old = set_c_float_zero();
    hmdl->mp[patternid].A_new = set_c_float_zero();

    hmdl->mp[patternid].start_0 = hex2data(MAX_TIME_DIGITS, &in[MAX_ID_DIGITS]);
    hmdl->mp[patternid].start_1 = hex2data(MAX_TIME_DIGITS, &in[MAX_ID_DIGITS
            + MAX_TIME_DIGITS]);
    hmdl->mp[patternid].end_0 = hex2data(MAX_TIME_DIGITS,
        &in[MAX_ID_DIGITS + 2*MAX_TIME_DIGITS]);
    hmdl->mp[patternid].end_1 = hex2data(MAX_TIME_DIGITS,
        &in[MAX_ID_DIGITS + 3*MAX_TIME_DIGITS]);
    hmdl->mp[patternid].offset = hex2data(MAX_TIME_DIGITS,
        &in[MAX_ID_DIGITS + 4*MAX_TIME_DIGITS]);
    /*printf (" on sh 2 :%d %d\n",    hmdl->mp[patternid].start_0,
     hmdl->mp[patternid].start_1);*/

    curr = MAX_ID_DIGITS + 5*MAX_TIME_DIGITS;

    hmdl->mp[patternid].A_old = hmdl->mp[patternid].A_new;
    hmdl->mp[patternid].A_new = hex2c_float(&in[curr]);

    hmdl->mp[patternid].active = 1;

}
;

void eval_set_servo_coeff_message(const char in[])
{
    int patternid, servoid, curr;
    patternid = hex2data(MAX_ID_DIGITS, in);
    servoid = hex2data(MAX_ID_DIGITS, &in[MAX_ID_DIGITS]);
    curr = 2*MAX_ID_DIGITS;
    int i = 0;
    while ((in[curr] != 'X') && (in[curr] != 0) && (i < MAX_COEFFS))
    {
        hmdl->mp[patternid].pp->sv[servoid].s_position[i] = hex2c_float(
            &in[curr]);
        write_cfloat(&hmdl->mp[patternid].pp->sv[servoid].s_position[i]);
        sendMesg("\r\n");
        write_int(i);
        sendMesg("\r\n");

        i++;
        curr += C_FLOAT_LENGTH;
    };

}
;

void eval_set_servo_small_coeff_message(const char in[])
{
    int patternid, servoid, coeffid, curr;
    patternid = hex2data(MAX_ID_DIGITS, in);
    servoid = hex2data(MAX_ID_DIGITS, &in[MAX_ID_DIGITS]);
    coeffid = hex2data(MAX_ID_DIGITS, &in[2*MAX_ID_DIGITS]);
    hmdl->mp[patternid].pp->sv[servoid].s_position[coeffid]
            = hex2c_float(&in[3*MAX_ID_DIGITS]);
}
;

//long long i;

void eval_new_pattern_message(const char in[])
{
    int id, wp_number, counter, i, j;

    /* get number */
    id = hex2data(MAX_ID_DIGITS, in);
    wp_number = hex2data(MAX_COE_DIGITS, &in[MAX_ID_DIGITS]);

    counter = MAX_ID_DIGITS + MAX_COE_DIGITS;
    for (i = 0; i < MAX_COEFFS; ++i)
        for (j = 0; j < MAX_SERVOS; ++j)
        {
            hmdl->mp[id].pp->sv[j].s_position[i].mantisse = 0;
            hmdl->mp[id].pp->sv[j].s_position[i].exponent = 0;
        };

    for (i = 0; i < (MAX_COEFFS - 1) / 2; ++i)
    {
        hmdl->mp[id].pp->wlqs_top[i] = 0;
        hmdl->mp[id].pp->wlqs_bot[i] = 1;

    };

    for (i = 0; i < (wp_number - 1) / 2; ++i)
    {
        hmdl->mp[id].pp->wlqs_top[i] = hex2data(MAX_WLQ_DIGITS, &in[counter]);
        counter += MAX_WLQ_DIGITS;
        hmdl->mp[id].pp->wlqs_bot[i] = hex2data(MAX_WLQ_DIGITS, &in[counter]);
        counter += MAX_WLQ_DIGITS;

        /* DEBUG */
        // printf ("MC: coeffs top %d bot  %d \n", hmdl->mp[id].pp->wlqs_top[i],
        //    hmdl->mp[id].pp->wlqs_bot[i]);

    };

    /* DEBUG */
    //intf("MC: Setting new pattern for number %d\n", id);
}
;

void init_hmdl()
{
    int i, j, k, l, p;

    /*
     * inititialize HDML structure
     */

    /* motion patterns */

    for (i = 0; i < MAX_MPS_IN_RAM; ++i)
    {
        hmdl->mp[i].pp = &(hmdl->mpp_in_ram[i]);
        hmdl->mp[i].pp->ID_prop = 8888 + i;

        for (j = 0; j < (MAX_COEFFS - 1) / 2; ++j)
        {
            hmdl->mp[i].pp->wlqs_top[j] = 0;
            hmdl->mp[i].pp->wlqs_bot[j] = 1;
        };

        for (j = 0; j < MAX_SERVOS; j++)
        {
            for (k = 0; k < MAX_COEFFS; k++)
            {
                hmdl->mp[i].pp->sv[j].s_position[k].mantisse = 0;
                hmdl->mp[i].pp->sv[j].s_position[k].exponent = 0;
            };

        };

    };

    /*for (i=MAX_MPS_IN_RAM;i<MAX_MPS;i++)                           // meant for motion patterns in flash
     hmdl->mp[i].pp = 0x00058000 + 0x00000800*(i-MAX_MPS_IN_RAM);

     for (i=0;i<MAX_MPS;++i)
     {

     hmdl->mp[i].A_old.mantisse=0;
     hmdl->mp[i].A_old.exponent=0;
     hmdl->mp[i].A_new.mantisse=0;
     hmdl->mp[i].A_new.exponent=0;

     hmdl->mp[i].start_0 = 0;
     hmdl->mp[i].start_1 = 0;
     hmdl->mp[i].updated = 0;
     hmdl->mp[i].active  = 0;
     };*/// until here


    hmdl->time = 0;
    hmdl->time_max = 0;
    hmdl->time_inc = 1;

    /* positions to set  */
    for (p = 0; p < MAX_SERVOS; ++p)
    {
        hmdl->positions_to_set[p].exponent = 0;
        hmdl->positions_to_set[p].mantisse = 0;
    }

}

void eval_get_current_sine_val(const char in[])
{
    int patternid, freqid;
    unsigned int retval;
    patternid = hex2data(MAX_ID_DIGITS, in);
    freqid = hex2data(MAX_ID_DIGITS, in);

    retval = sin_fixed((hmdl->time) * hmdl->mp[patternid].pp->wlqs_top[freqid]
            / hmdl->mp[patternid].pp->wlqs_bot[freqid]);
    retval /= 100000;

    sendMesg("current value:\r\n");
    write_int(retval);
    sendMesg("\r\n");
    sendMesg("wlqs_top:\r\n");
    write_int(hmdl->mp[patternid].pp->wlqs_top[freqid]);
    sendMesg("\r\n");

}
;

void eval_seq(const char in[])
{

    switch (in[0])
    {
        case 'P':
            switch (in[1])
            {
                case 'N':
                    eval_new_pattern_message(&in[2]); // PN command
                    break;
                case 'S':
                    eval_set_servo_coeff_message(&in[2]); // PS command
                    break;
                case 'I':
                    eval_set_servo_small_coeff_message(&in[2]);
                    // PS command
                    break;
                case 'U':
                    eval_use_pattern_message(&in[2]); // PU command
                    break;
                case 'P':
                    eval_use_short_pattern_message(&in[2]); // PU command
                    break;
                case 'Y':
                    eval_use_pattern_message_with_end(&in[2]); // PY command

                    break;
            }
            break;
        case 'T':
            switch (in[1])
            {
                case 'S':
                    eval_set_time_message(&in[2]); // TS command
                    break;
            }

            break;
        case '?':
            switch (in[1])
            {
                case 'S':
                    eval_get_current_sine_val(&in[2]);
                    break;

            }
            break;

    }
}
;

struct c_float interpolate_rational_c_float( /*Real amplitude interpolation*/
int size_c, struct c_float amp, int time, struct c_float coeffs[],
                                            unsigned int frequencies_up[],
                                            unsigned int frequencies_bottom[])
{
    int l;
    struct c_float y;
    y = mult_cc(amp, coeffs[0]);

    for (l = 0; l < (size_c - 1) / 2; ++l)
    {
        y = c_f_add(y, mult_cc_sinus(amp, coeffs[2* l + 1], sin_fixed((time
                * frequencies_up[l]) / frequencies_bottom[l])));

        y = c_f_add(y, mult_cc_sinus(amp, coeffs[2* l + 2], cos_fixed((time
                * frequencies_up[l]) / frequencies_bottom[l])));
    };

    return y;

}
;

struct c_float interpolate_c_float_phase(
/*Amplitude is rotational matrix (complex) */
int size_c, struct c_float amp[], int time, struct c_float coeffs[],
                                         unsigned int frequencies[])
{
    int i;
    struct c_float y, sine_amp, cos_amp;
    y = mult_cc(amp[0], coeffs[0]);
    for (i = 0; i < size_c / 2; i++)
    {
        sine_amp = c_f_add(mult_cc(amp[2], coeffs[2* i + 2]), mult_cc(amp[3],
            coeffs[2* i + 1]));
        cos_amp = c_f_add(mult_cc(amp[0], coeffs[2* i + 2]), mult_cc(amp[1],
            coeffs[2* i + 1]));

        y
                = c_f_add(y, mult_c_sinus(sine_amp, sin_fixed(time
                        * frequencies[i])));
        y = c_f_add(y, mult_c_sinus(cos_amp, cos_fixed(time * frequencies[i])));
    };

    return y;
}
;

