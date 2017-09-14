
#include "logger/logger.hpp"
#include "translib/timerManager.h"
#include "heartbeat/heartbeat.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
int main()
{
    set_log_level(logger_iface::log_level::debug);
    __LOG(error, "hello logger!"
                     << "this is error log");
    __LOG(warn, "hello logger!"
                    << "this is warn log");
    __LOG(info, "hello logger!"
                    << "this is info log");
    __LOG(debug, "hello logger!"
                     << "this is debug log");

    std::atomic<bool> flag;

    heartbeat hb;
    hb.start([&](std::atomic<bool> &flagg) {

        srand(time(NULL));
        int randnum = rand() % 10;
        flagg = (randnum > 5) ? 1 : 0;
        __LOG(debug, "flag is : " << flagg);
    },
             flag);

    hb.set_hb_success_cb([]() {
        __LOG(debug, "in the success callback function");
    });
    hb.set_hb_lost_cb([](int a) {
        __LOG(debug, "in the heartbeat lost callback function, error code is " << a);
    });
#if 0
    {
        heartbeat hb;
        hb.start([&](std::atomic<bool> &flagg) {

            test = !test;
            flagg = test;
            __LOG(debug, "flag is : " << flagg);
        },
                 flag);
    }
#endif

    sleep(100);
    __LOG(error, "exit example!");
}
