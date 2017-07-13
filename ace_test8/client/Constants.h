#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_

#include "ace/Time_Value.h"

namespace ACE_Client {
    static const size_t BUFFER_SIZE = 4096;
    static const size_t SERVER_PORT_NUM = 10101;
    static const ACE_Time_Value TIME_INTERVAL(0, 1000000); // 1 second
}

#endif

