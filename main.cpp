#include <iostream>
#include <string>
#include <sstream>
#include "ace/ACE.h"
#include "ace/Thread_Manager.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue.h"
#include "ace/Timer_Queue_Adapters.h"
#include "core/timers/src/SingletonTimerUtil.h"
#include "ace/OS.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/threads/src/Thread.h"
#include <cmath>
#include <ctime>

using namespace TA_Base_Core;


std::string Logger_constructMessage
(
 DebugUtil::EDebugType type, 
 const std::string& file,
 int line, 
 va_list& args, 
 bool simpleLog
 )
{
    {
        // limin deadlock
        int level = va_arg ( args, int );
        char* format = va_arg ( args, char* );
        //char buff[100] = { 0 };
        char* buff = new char[2000+1];
        int ret = vsnprintf ( buff, 2000, format, args ); //limin deadlock
        va_end( args );
        std::cout << buff << std::endl;
        return buff;
    }
}

int Logger_log
(
 DebugUtil::EDebugType type, 
 const std::string& file, 
 int line, 
 va_list& args, 
 std::ostream& outfile, 
 bool simpleLog
 )
{
    std::string logMsg = Logger_constructMessage ( type, file, line, args, simpleLog );
    return logMsg.size();
}


void DebugUtil_logMessage
(
 const std::string file, 
 int line, 
 DebugUtil::EDebugType  type, 
 va_list& args, 
 bool simpleLog, 
 bool printCache
 )
{
    Logger_log( type, file, line, args, std::cout, simpleLog );
}

void DebugUtil_logGeneric(const std::string& file, int line, ...)
{
    va_list args;
    va_start ( args, line );
    int level = va_arg ( args, int );
    va_start ( args, line );
    DebugUtil_logMessage( file, line, DebugUtil::GenericLog, args, false, false );
    va_end ( args );
}



int main( int argc, char* argv[] )
{
    //ACE::init();
#if 1
    DebugUtil_logGeneric( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "%s",
        "\n"
        "\t**********************************************************************************************************************\n"
        "\t*                                                   START OF FILE                                                    *\n" 
        "\t**********************************************************************************************************************" );
#endif


#if 0
    LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "\n"
        "\t**********************************************************************************************************************\n"
        "\t*                                                   START OF FILE                                                    *\n" 
        "\t**********************************************************************************************************************" );
#endif

    {
        //TA_Base_Core::DebugUtil::getInstance().setLevel( TA_Base_Core::DebugUtil::DebugTrace );
        //TA_Base_Core::DebugUtil::getInstance().setFile( "c:\\temp\\test.log" );


        //typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>	ActiveQueue;
        //ActiveQueue												m_activeQueue;
        //m_activeQueue.activate();
        //m_activeQueue.deactivate();
        //m_activeQueue.thr_count();

        TA_Base_Core::SingletonTimerUtil::getInstance();
        TA_Base_Core::SingletonTimerUtil::removeInstance();
        std::cout << "hello, world" << std::endl;
    }

    //std::srand( std::time(NULL) );
    //std::cout << "hello, world again" << std::endl;

    //{
    //    DebugThread t1;
    //    TimerThread t2;
    //    t1.start();
    //    t2.start();
    //    _getch();
    //}

    //ACE::fini();
    return 1;
}
