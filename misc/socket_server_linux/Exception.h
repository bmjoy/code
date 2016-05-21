/*
auth:  Ishaq
email: msl_cn@126.com
*/
#include <execinfo.h>
 #include <signal.h>
 #include <exception>
 #include <iostream>
 using namespace std;
 /////////////////////////////////////////////
 class ExceptionTracer
 {
 public:
     ExceptionTracer()
     {
         void * array[25];
         int nSize = backtrace(array, 25);
         char ** symbols = backtrace_symbols(array, nSize);
         for (int i = 0; i < nSize; i++)
         {
             cout << symbols[i] << endl;
         }
         free(symbols);
     }
 };

template <class SignalExceptionClass> class SignalTranslator
 {
 private:
     class SingleTonTranslator
     {
     public:
         SingleTonTranslator()
         {
             signal(SignalExceptionClass::GetSignalNumber(), SignalHandler);
         }
         static void SignalHandler(int)
         {
             throw SignalExceptionClass();
         }
     };
 public:
     SignalTranslator()
     {
         static SingleTonTranslator s_objTranslator;
     }
 };
 // An example for SIGSEGV
 class SegmentationFault : public ExceptionTracer, public exception
 {
 public:
     static int GetSignalNumber() {return SIGSEGV;}
 };
 SignalTranslator<SegmentationFault> g_objSegmentationFaultTranslator;
 // An example for SIGFPE
 class FloatingPointException : public ExceptionTracer, public exception
 {
 public:
     static int GetSignalNumber() {return SIGFPE;}
 };

  class ExceptionHandler
 {
 private:
     class SingleTonHandler
     {
     public:
         SingleTonHandler()
         {
             set_terminate(Handler);
         }
         static void Handler()
         {
             // Exception from construction/destruction of global variables
             try
             {
                 // re-throw
                 throw;
             }
             catch (SegmentationFault &)
             {
                 cout << "SegmentationFault" << endl;
             }
             catch (FloatingPointException &)
             {
                 cout << "FloatingPointException" << endl;
             }
             catch (...)
             {
                 cout << "Unknown Exception" << endl;
             }
             //if this is a thread performing some core activity
             abort();
             // else if this is a thread used to service requests
             // pthread_exit();
         }
     };
 public:
     ExceptionHandler()
     {
         static SingleTonHandler s_objHandler;
     }
 };
