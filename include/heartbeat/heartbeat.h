#pragma once
#include <atomic>
#include <functional>
#include "translib/timerManager.h"
#include "translib/timer.h"
#include "logger/logger.hpp"
//template <typename instype>
class heartbeat
{
  public:
    heartbeat()
    {
        timer = translib::TimerManager::instance()->getTimer();
        interval = 5000;
    }
    ~heartbeat() {}
    typedef std::function<void(std::atomic<bool> &)> ping_f;
    typedef std::function<void(void)> hb_success_cb;
    typedef std::function<void(int)> hb_lost_cb;

    void onHeartbeatLost()
    {
        __LOG(debug, "onHeartbeatLost");
        if (l_cb)
        {
            l_cb(400);
        }
    }
    void onHeartbeatSuccess()
    {
        __LOG(debug, "onHeartbeatSuccess");
        if (s_cb)
        {
            s_cb();
        }
    }
    void start(ping_f fun, std::atomic<bool> &success)
    {

        timer->startForever(interval, [&]() {

            if (success)
            {
                onHeartbeatSuccess();
            }
            else
            {
                onHeartbeatLost();
            }
            success = false;
            fun(success);

        });
    }

    void set_interval(uint32_t iv) { interval = iv; }
    uint32_t get_interval() { return interval; }
    void set_hb_success_cb(hb_success_cb cb) { s_cb = cb; }
    void set_hb_lost_cb(hb_lost_cb cb) { l_cb = cb; }

  private:
    uint32_t interval;
    translib::Timer::ptr_p timer;
    hb_success_cb s_cb;
    hb_lost_cb l_cb;
};