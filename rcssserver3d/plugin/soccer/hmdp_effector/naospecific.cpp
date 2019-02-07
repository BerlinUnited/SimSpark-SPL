#include "naospecific.h"
#include <vector>

double NaoSpecificPluginFunctions::zeroPosition(int id) //! Nao specific
{
    if ((id == 2) || (id == 6))
        return -90.0; // NAO specific zero positions (put the arms down and hold them on the sides diagonally)
    if (id == 3)
        return -25.0;
    if (id == 7)
        return 25.0;
    return 0.0;
}

std::string NaoSpecificPluginFunctions::getJointName(int ID)
{
    const char jointnames[22][8] =
        { "HE11-P", "HE12-Y", // Head Joint IDs
          "RA11+P", "RA12-R", "RA13-Y", // Right Arm: Shoulder Joints
          "RA21-R", // Right Elbow
          "LA11+P", "LA12-R", "LA13-Y", // Left Arm: Shoulder Joints
          "LA21-R", // Left Elbow
          "RL11+PY", "RL12-R", "RL13+P", // Right Leg: Hip Joints
          "RL21+P", // Right Knee
          "RL31+P", "RL32-R", // Right Leg: Foot joints
          "LL11+PY", "LL12-R", "LL13+P", // Right Leg: Hip Joints
          "LL21+P", // Right Knee
          "LL31+P", "LL32-R" }; // Right Leg: Foot joints
    // (in the simultated NAO some joints have parity + otherwise have parity  ) In real robots all are -
    std::string rval;
    rval.resize(6);
    for (int j = 0; j < 6; j++)
        rval[j] = jointnames[ID][j];
    return rval;
}

/*
 int NaoSpecificPluginFunctions::read_back_id( int ID)
 // on the Microcontroller this gives the map between the continuous internal servo ids versus the actual servo ids
 {
 if ((ID>=0) && (ID <22)) return -1;  // in this simulation both are the same
 return 0;
 };
 */
