#pragma once

#include <cstdio>
#include <cstdlib>
#include <deque>
#include <string>
#include <switch.h>

// debug the stuff
#include <fstream>

class AsyncLogger {
  public:
    enum log_lvl : u8 {
        lvl_trace,
        lvl_info,
        lvl_warn,
        lvl_error,
        lvl_fatal,
    };

    AsyncLogger(FILE *fp, log_lvl lvl = lvl_trace)
        : fp(fp), start_tick(armGetSystemTick()), lvl(lvl)
    {
        setvbuf(fp, NULL, _IOLBF, 0x50);
    }
    AsyncLogger &operator=(const AsyncLogger &) = delete;
    AsyncLogger(const AsyncLogger &) = delete;

    Result start();
    void stop();

    bool is_running() const { return this->running; }

    void set_log_level(log_lvl lvl) { this->lvl = lvl; }
    log_lvl get_log_lvl() const { return this->lvl; }

    template <typename... Args>
    void enqueue(log_lvl lvl, const char *fmt, Args &&... args)
    {
#ifdef DEBUG
        if (lvl < this->lvl || !is_running())
            return;
        std::string fmted(
            snprintf(nullptr, 0, fmt, std::forward<Args>(args)...) + 1, 0);
        snprintf((char *)fmted.data(), fmted.size(), fmt,
                 std::forward<Args>(args)...);
        mutexLock(&this->dequeue_mutex);
        this->messages.push_back({armGetSystemTick(), lvl, fmted});
        mutexUnlock(&this->dequeue_mutex);
#endif
    }

    template <typename... Args>
    void enqueue(const std::string &fmt, log_lvl lvl, Args &&... args)
    {
        enqueue(lvl, fmt.c_str(), std::forward<Args>(args)...);
    }

#define DECL_LVL_HELPER(lvl)                                                   \
    template <typename Fmt, typename... Args>                                  \
    void lvl(Fmt &&fmt, Args &&... args)                                       \
    {                                                                          \
        enqueue(lvl_##lvl, std::forward<Fmt>(fmt),                             \
                std::forward<Args>(args)...);                                  \
    }

    DECL_LVL_HELPER(trace)
    DECL_LVL_HELPER(info)
    DECL_LVL_HELPER(warn)
    DECL_LVL_HELPER(error)
    DECL_LVL_HELPER(fatal)

#undef DECL_LVL_HELPER

    void data(const void *data, size_t size, unsigned int indent = 0,
              const char *prefix = NULL, log_lvl lvl = lvl_trace)
#ifdef DEBUG // We rely on the compiler stripping the function to not data abort
        ;
#else
    {
    }
#endif

  private:
    typedef struct {
        u64 tick;
        log_lvl lvl;
        std::string string;
    } message_t;

    FILE *fp;
    u64 start_tick;
    Thread thread;
    bool running = false;
    log_lvl lvl;
    Mutex dequeue_mutex = {0};
    std::deque<message_t> messages;

    static void writer_thread(void *args);
};