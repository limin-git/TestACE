#include <iostream>
#include <string>
#include <sstream>
#include "ace/ACE.h"
#include "ace/Thread_Manager.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Queue.h"
#include "ace/Timer_Queue_Adapters.h"
#include "ace/OS.h"
#include <cmath>
#include <ctime>
#include <map>
#include <fstream>


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
        do
        {
            m_activeQueue.deactivate();
        }
        while ( 0 < m_activeQueue.thr_count() );
    }

private:

    struct TimerItem
    {
        void * user;
        bool singleShot;
        long timerId;
        void* userData;
    };

    typedef std::multimap<void*, long>                      TimerMap;
    typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>  ActiveQueue;
    ActiveQueue                                             m_activeQueue;
    //TA_Base_Core::ReEntrantThreadLockable                 m_mapLock;
    MyNonReEntrantThreadLockable                               m_mapLock;
    TimerMap                                                m_timerMap;
};


int main( int argc, char* argv[] )
{
    try
    {   	
        ACE::init();

        if ( 1 < argc )
        {
            if ( argv[1] == std::string("cmd-log") )
            {
                std::cout << "string from command line" << std::endl;
                for ( int i = 2; i < argc; ++i )
                {
                    DebugUtil_logGeneric( "%s", argv[i] );
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
            else if ( argv[2] == std::string( "ace" ) )
            {
                std::cout << "ACE_Thread_Timer_Queue_Adapter" << std::endl;
                if ( argv[3] == std::string( "new" ) )
                {
                    std::cout << "new" << std::endl;
                    typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>  ActiveQueue;
                    ActiveQueue                                             m_activeQueue;
                    ActiveQueue* queue = new ActiveQueue;
                    queue->activate();
                    queue->deactivate();
                    queue->thr_count();
                    delete queue;
                }
                else
                {
                    std::cout << "stack" << std::endl;
                    typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap>  ActiveQueue;
                    ActiveQueue                                             m_activeQueue;
                    m_activeQueue.activate();
                    m_activeQueue.deactivate();
                    m_activeQueue.thr_count();
                }
            }
        }

        std::cout << "END" << std::endl;
        std::cout << "hello, world" << std::endl;

        ACE::fini();
    }
    catch ( std::exception& e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
    catch ( ... )
    {
        std::cout << "unknown exception" << std::endl;
    }

    return 0;
}
