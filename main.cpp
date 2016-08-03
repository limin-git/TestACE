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
#include <fstream>

using namespace TA_Base_Core;


std::string Logger_constructMessage( const char* fmt, va_list& args )
{
    char buff[2001] = { 0 };
    int ret = vsprintf( buff, fmt, args );
    std::cout << buff << std::endl;
    return buff;
}

int Logger_log( const char* fmt, va_list& args )
{
    if ( 0 )
    {
        char buff[2001] = { 0 };
        int ret = vsnprintf ( buff, 2000, fmt, args );
        va_end( args );
        std::cout << buff << std::endl;
        return 0;
    }

    std::string logMsg = Logger_constructMessage ( fmt, args );
    return logMsg.size();
}

void DebugUtil_logMessage( const char* fmt, va_list& args )
{
    Logger_log( fmt, args );
}

void DebugUtil_logGeneric( const char* fmt, ...)
{
    va_list args;
    va_start ( args, fmt );
    DebugUtil_logMessage( fmt, args );
    va_end ( args );
}

class My_ACE_Event_Handler
{
public:
    virtual ~My_ACE_Event_Handler (void) {}
    char x[60];
};


struct MyNonReEntrantThreadLockable
{
    virtual ~MyNonReEntrantThreadLockable() {};
    char x[12];
};


class MyTimerUtil 
    : public My_ACE_Event_Handler
{

public:

    MyTimerUtil()
    {
        m_activeQueue.activate();
    }

    virtual ~MyTimerUtil()
    {
        unsigned long attemptCount = 0;

        // Deactivate the active queue and wait for its thread to terminate.
        // It is done manually to overcome an observed problem where using the
        // queues wait method would block indefinitely as the condition did not
        // seem to be signalled to allow the thread to terminate.
        do
        {
            m_activeQueue.deactivate();
            TA_Base_Core::Thread::sleep( 100 );
        } 
        while ( 0 < m_activeQueue.thr_count() );
    }

private:

    struct TimerItem
    {
        ITimeoutCallback * user;
        bool singleShot;
        long timerId;
        void* userData;
    };

    typedef std::multimap<ITimeoutCallback*, long>          TimerMap;
    typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>  ActiveQueue;
    ActiveQueue                                             m_activeQueue;
    //TA_Base_Core::ReEntrantThreadLockable                 m_mapLock;
    MyNonReEntrantThreadLockable                               m_mapLock;
    TimerMap                                                m_timerMap;
};


int main( int argc, char* argv[] )
{
    ACE::init();

    std::cout << "size of ACE_Event_Handler = " << sizeof(ACE_Event_Handler) << std::endl;
    std::cout << "size of TimerUtil = " << sizeof(TA_Base_Core::TimerUtil) << std::endl;
    std::cout << "size of My_ACE_Event_Handler = " << sizeof(My_ACE_Event_Handler) << std::endl;
    std::cout << "size of MyTimerUtil = " << sizeof(MyTimerUtil) << std::endl;
    std::cout << "size of NonReEntrantThreadLockable = " << sizeof(TA_Base_Core::NonReEntrantThreadLockable) << std::endl;
    std::cout << "size of MyNonReEntrantThreadLockable = " << sizeof(MyNonReEntrantThreadLockable) << std::endl;


    if ( 1 < argc )
    {
        if ( argv[1] == std::string("cmd-log") )
        {
            std::cout << "string from command line" << std::endl;
            for ( int i = 2; i < argc; ++i )
            {
                LOG1( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "%s", argv[i]);
            }
        }
        else if ( argv[1] == std::string( "mylog" ) )
        {
            std::cout << "DebugUtil_logGeneric" << std::endl;
            DebugUtil_logGeneric( "%s",
                "\n"
                "\t**********************************************************************************************************************\n"
                "\t*                                                   START OF FILE                                                    *\n" 
                "\t**********************************************************************************************************************" );
        }
        else if ( argv[1] == std::string( "log" ) )
        {
            LOG0( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "\n"
                "\t**********************************************************************************************************************\n"
                "\t*                                                   START OF FILE                                                    *\n" 
                "\t**********************************************************************************************************************" );
        }
        else if ( argv[1] == std::string( "nolog" ) )
        {
        }
        else
        {
        }
    }

    std::cout << "BEGIN" << std::endl;

    if ( 3 < argc )
    {
        if ( argv[2] == std::string( "myace" ) )
        {
            std::cout << "MyTimerUtil" << std::endl;
            if ( argv[3] == std::string( "new" ) )
            {
                std::cout << "new" << std::endl;
                MyTimerUtil* myTimer = new MyTimerUtil;
                delete myTimer;
            }
            else
            {
                std::cout << "stack" << std::endl;
                MyTimerUtil test;
            }
        }
        else
        {
            std::cout << "TimerUtil" << std::endl;
            if ( argv[3] == std::string( "new" ) )
            {
                std::cout << "new" << std::endl;
                TA_Base_Core::TimerUtil* test = new TA_Base_Core::TimerUtil;
                delete test;
            }
            else
            {
                std::cout << "stack" << std::endl;
                TA_Base_Core::TimerUtil test;
            }
        }
    }
    else
    {
        std::cout << "SingletonTimerUtil" << std::endl;
        TA_Base_Core::SingletonTimerUtil::getInstance();
        TA_Base_Core::SingletonTimerUtil::removeInstance();
    }
    std::cout << "END" << std::endl;
    std::cout << "hello, world" << std::endl;

    ACE::fini();
    return 1;
}
