/*******************************************************************************************

 ! HMDP wrapper connects Effector class with c-coded base.c and hmdp_c.c functions (analogue to the compatibile.c)

 ******************************************************************************************/

#include "hmdpeffector.h"
//! Handler to the corresponding instantiation of the effector (each agent has one instantiation)
extern HMDPEffector *hmdpEffectorHandle;

//! All naked C-functions of the initial motor controller are covered in a separate namespace
namespace HMDP
{
extern "C"
{
using namespace HMDP;

//! message buffer for messages FROM the motor controller
std::string messageToSend = "";
//! sends a message byte from HMDP motorcontroller (server) to the HMDP client
//! one of the functions required in the HMDP compatibility layer
// \param data byte to send
int sendByte(int data)
{

    char outChar = data;
    if ((outChar != ' ') && (outChar != 13) && (outChar != 10)
            && (outChar != 0))
        messageToSend = messageToSend + outChar;
    else if ((outChar != 0) && (messageToSend.size() > 0))
    {
        hmdpEffectorHandle->sendMessage(messageToSend); //! after a carriage return or line feed is emmitted from the  motor controller
        messageToSend = "";
    }
    return 0;
}

//! reads the current servo positions (needed by the motorcontroller)
// \param ID of the servo
int read_back_pos(int ID)
{
    return (hmdpEffectorHandle->servo_angle[ID] / 180.0 * 2048.0 + 2048.0);
}

//! reads the target servo position
// \param ID of the servo
int read_back_pos_set(int ID)
{
    return (hmdpEffectorHandle->servo_target_pos[ID] / 180.0 * 2048.0 + 2048.0);

}

//! allows to turn off the servo -- not used now
// \param ID of the servo
void send_servo_off(int ID)
{

}

//! allows to turn on the servo -- not used now
// \param ID of the servo
void send_servo_on(int ID)
{
}

//!  set servo's target position
// \param ID of the servo
void send_servo_to_pos(int ID, int pos)
{
    hmdpEffectorHandle->servo_target_pos[ID] = (pos - 2048.) * 180. / 2048.; // transfer from degrees to range from 0-4096
}

void send_servo_to_gain(int ID, int gain) // used to set gains in the servo
{
    hmdpEffectorHandle->servo_gain[ID] = 0.015 * gain; // !  0.015 -> right now an arbitrary number

}

//!  is called when the motorcontroller sets the IRQ timer off
void disableIRQ() // mimicks IRQ functionality
{
    hmdpEffectorHandle->ifIRQ = false;
}

//!  is called when the motorcontroller sets the IRQ timer on
void enableIRQ()
{
    hmdpEffectorHandle->ifIRQ = true;
}

//Parser funcs
//! this variable is used to feed in the next character into the microcontroller parser
//! Necessary because
int readChar = 0;

int readByte() //! is called by the HMDP parser loop to get the next character
{
    int retVal = 13;

    if (hmdpEffectorHandle->inMessage.size() > 0)
        readChar = (int) hmdpEffectorHandle->inMessage[0];
    else
        readChar = 13;
    if (hmdpEffectorHandle->inMessage.size() > 1)
        hmdpEffectorHandle->inMessage = hmdpEffectorHandle->inMessage.substr(1,
            hmdpEffectorHandle->inMessage.size() - 1);
    if (hmdpEffectorHandle->inMessage.size() == 1)
        hmdpEffectorHandle->inMessage = "";
    retVal = readChar;
    //

    return retVal;
}

void clearBuffer()
{

}

int sendBytetoMo(int data)
{
    return 0;
}

//!currently max 7 character (+\0) per servo for the name
char jointnames[MAX_ALLOWED_SERVOS][8];

int read_back_id(int ID) //! on the Microcontroller this gives the map between the continuous internal servo ids versus the actual servo ids
{
    if (hmdpEffectorHandle->checkIfServoIDExists(ID))
        return -1; // in this simulation both are the same
    else
        return 0;
}

#include "hmdp_c/base.c"
#include "hmdp_c/hmdp_c.c"
#include "hmdp_c/sine_fixed.c"

} // end of c-code area
} // end of namespace HMDP
