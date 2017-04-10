#ifndef _NAO_SPECIFIC_HMDP__
#define _NAO_SPECIFIC_HMDP__

#include <string>

class NaoSpecificPluginFunctions // ! This class contains NAO Specific functions
{
public:
    double zeroPosition(int ID); //! reads back zero positions for each ID in order to generate a standard posture
    std::string getJointName(int ID); //! according to HMDP convention -- some information is not provided from the server (see doc)
    //int read_back_id( int ID);  //! returns -1 if ID exists else 0
};

#endif
