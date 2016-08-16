/* 
 * Copyright (c) 2005-2012 by KoanLogic s.r.l. - All rights reserved.  
 */

#ifndef _U_LOGPRV_H_
#define _U_LOGPRV_H_

#include <u/libu_conf.h>
#include <stdarg.h>
#include <u/missing/syslog.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
    LOG_WRITE_FLAG_NONE,
    LOG_WRITE_FLAG_CTX      /* include file:line in the output message */
};
#ifdef OS_ANDROID

#ifndef LOG_NDEBUG
#ifdef NDEBUG
#define LOG_NDEBUG 1
#else
#define LOG_NDEBUG 0
#endif
#endif

#ifndef LOG_TAG
#define LOG_TAG "NULL"
#endif

#ifndef LOG_TAG_PRE
#define LOG_TAG_PRE ""
#endif

#ifndef PRINTF_LOG
#define PRINTF_LOG 0
#endif

#undef LOGV
#undef LOGI
#undef LOGD
#undef LOGW
#undef LOGE

#undef ALOGV
#undef ALOGI
#undef ALOGD
#undef ALOGW
#undef ALOGE

#define u_log_write(fac, lev, flags, err, ...)                               \
    u_log_write_ex_android(LOG_TAG_PRE, LOG_TAG, LOG_NDEBUG, fac, lev, flags, err, __FILE__, __LINE__, __FUNCTION__, \
            __VA_ARGS__)

#if LOG_NDEBUG
#define LOGV(...)    u_log_write(LOG_LOCAL0,LOG_NOTICE,0,0,__VA_ARGS__)
#else
#define LOGV(...)
#endif
#define LOGI(...)    u_log_write(LOG_LOCAL0,LOG_INFO,0,0,__VA_ARGS__)
#define LOGD(...)    u_log_write(LOG_LOCAL0,LOG_DEBUG,0,0,__VA_ARGS__)
#define LOGW(...)    u_log_write(LOG_LOCAL0,LOG_WARNING,0,0,__VA_ARGS__)
#define LOGE(...)    u_log_write(LOG_LOCAL0,LOG_ERR,0,0,__VA_ARGS__)

#if LOG_NDEBUG
#define ALOGV(...)    u_log_write(LOG_LOCAL0,LOG_NOTICE,0,0,__VA_ARGS__)
#else
#define ALOGV(...)
#endif
#define ALOGI(...)    u_log_write(LOG_LOCAL0,LOG_INFO,0,0,__VA_ARGS__)
#define ALOGD(...)    u_log_write(LOG_LOCAL0,LOG_DEBUG,0,0,__VA_ARGS__)
#define ALOGW(...)    u_log_write(LOG_LOCAL0,LOG_WARNING,0,0,__VA_ARGS__)
#define ALOGE(...)    u_log_write(LOG_LOCAL0,LOG_ERR,0,0,__VA_ARGS__)

#define u_console_write(err, ...) \
    u_console_write_ex_android(LOG_TAG_PRE, LOG_TAG, LOG_NDEBUG, PRINTF_LOG ,err, __FILE__, __LINE__, __FUNCTION__,  __VA_ARGS__)

#define printf(...)  u_console_write(0,__VA_ARGS__)

#else
#define u_log_write(fac, lev, flags, err, ...)                               \
    u_log_write_ex(fac, lev, flags, err, __FILE__, __LINE__, __FUNCTION__, \
            __VA_ARGS__)

#define u_console_write(err, ...) \
    u_console_write_ex(err, __FILE__, __LINE__, __FUNCTION__,  __VA_ARGS__)
#endif

int u_log_write_ex(int fac, int lev, int flags, int err, const char* file, 
    int line, const char *func, const char* fmt, ...);

int u_console_write_ex(int err, const char* file, int line, 
    const char *func, const char* fmt, ...);

#ifdef OS_ANDROID
int u_log_write_ex_android(const char *tag_pre,const char *tag, int debug, int fac, int lev, int flags, int err, char* file,
    int line, const char *func, const char* fmt, ...);

int u_console_write_ex_android(const char *tag_pre,const char *tag, int debug, int printf_log, int err, char* file, int line,
    const char *func, const char* fmt, ...);
#endif

#ifdef __cplusplus
}
#endif

#endif /* !_U_LOGPRV_H_ */
