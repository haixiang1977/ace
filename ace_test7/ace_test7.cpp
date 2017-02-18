/* test ace log */
#include <fstream>
#include "ace/Log_Msg.h"

using namespace std;

// http://www.informit.com/articles/article.aspx?p=169524
// output print to file

int ACE_TMAIN(int argc, ACE_TCHAR* argv[]) {
    ACE_OSTREAM_TYPE *output = new std::ofstream("ostream.output.test");
    ACE_LOG_MSG->msg_ostream (output, 1);
    ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
    ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR);

    ACE_TRACE(ACE_TEXT ("main"));

    ACE_DEBUG((LM_INFO, ACE_TEXT("Main Hello World\n")));

    return 0;
}

