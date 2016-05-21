#ifndef __DDZ_SERVER_FRAME_H__
#define __DDZ_SERVER_FRAME_H__
#include "singleton.h"

class server_frame : public singleton<server_frame>
{
public:
    server_frame(void);
    ~server_frame(void);
    void run();

private:
    bool init();
    bool fini();
    bool start();
    bool stop();
    void prompt();
    void wait_running();
    void dump();
};

#endif//__DDZ_SERVER_FRAME_H__