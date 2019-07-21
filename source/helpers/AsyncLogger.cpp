#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <switch.h>

#include "helpers/AsyncLogger.hpp"

Result AsyncLogger::start()
{
    if (is_running())
        return -1;
    Result rc = threadCreate(&this->thread, writer_thread, (void *)this, 0x1000,
                             0x3b, -2);
    if (R_SUCCEEDED(rc)) {
        this->running = true;
        rc = threadStart(&this->thread);
        if (R_FAILED(rc))
            threadClose(&this->thread);
    }
    this->running = R_SUCCEEDED(rc);
    return rc;
}

void AsyncLogger::stop()
{
    if (!is_running())
        return;
    this->running = false;
    threadWaitForExit(&this->thread);
    threadClose(&this->thread);
}

void AsyncLogger::writer_thread(void *args)
{
    AsyncLogger *s_this = (AsyncLogger *)args;
    float freq = (float)armGetSystemTickFreq();
    static const char *const lvl_strings[] = {
        "[TRACE]:", "[INFO]: ", "[WARN]: ", "[ERROR]:", "[FATAL]:"};
    while (s_this->is_running() || !s_this->messages.empty()) {
        svcSleepThread(1e+6);
        for (message_t &message : s_this->messages) {
            float time_ms = (float)(message.tick - s_this->start_tick) / freq;
            fprintf(s_this->fp, "[%#.2fs] %s %s", time_ms,
                    lvl_strings[message.lvl], message.string.c_str());
            mutexLock(&s_this->dequeue_mutex);
            s_this->messages.pop_front();
            mutexUnlock(&s_this->dequeue_mutex);
        }
    }
}

#ifdef DEBUG

void AsyncLogger::data(const void *data, size_t size, unsigned int indent,
                       const char *prefix, log_lvl lvl)
{
    if (lvl < this->lvl || !is_running())
        return;
    size_t prefix_len = (prefix) ? strlen(prefix) : 0;
    std::string str(prefix_len + ceil(size / 16.0) * (indent + 70) + 1, ' ');
    char *str_data = (char *)str.data();
    if (prefix)
        strncpy(str_data, prefix, prefix_len);
    size_t data_idx = prefix_len + indent, ascii_idx = data_idx + 53;
    for (size_t i = 0; i < size; ++i) {
        str_data[data_idx + snprintf(&str_data[data_idx], 3, "%02x",
                                     ((u8 *)data)[i])] = ' ';
        str_data[ascii_idx] = (' ' <= ((u8 *)data)[i] && ((u8 *)data)[i] <= '~')
                                  ? ((u8 *)data)[i]
                                  : '.';
        data_idx += 3;
        ++ascii_idx;
        if ((i + 1) % 16 == 0) {
            str_data[data_idx + 1] = '|';
            str_data[ascii_idx] = '\n';
            data_idx += indent + 21;
            ascii_idx = data_idx + 53;
        }
        else if ((i + 1) % 8 == 0) {
            ++data_idx;
        }
        else if (i + 1 == size) {
            str_data[data_idx + (16 - i % 16) * 3 - 1] = '|';
        }
    }
    *(u16 *)&str_data[str.size() - 2] = '\n';
    mutexLock(&this->dequeue_mutex);
    this->messages.push_back({armGetSystemTick(), lvl, str});
    mutexUnlock(&this->dequeue_mutex);
}

#endif // DEBUG