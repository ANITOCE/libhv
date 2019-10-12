#ifndef HW_LOG_H_
#define HW_LOG_H_

/*
 * hlog is thread-safe
 */

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CL_CLR      "\033[0m"       /* 恢复颜色 */
#define CL_BLACK    "\033[30m"      /* 黑色字 */
#define CL_RED      "\e[1;31m"      /* 红色字 */
#define CL_GREEN    "\e[1;32m"      /* 绿色字 */
#define CL_YELLOW   "\e[1;33m"      /* 黄色字 */
#define CL_BLUE     "\033[34m"      /* 蓝色字 */
#define CL_PURPLE   "\e[1;35m"      /* 紫色字 */
#define CL_SKYBLUE  "\e[1;36m"      /* 天蓝字 */
#define CL_WHITE    "\033[37m"      /* 白色字 */

#define CL_BLK_WHT  "\033[40;37m"   /* 黑底白字 */
#define CL_RED_WHT  "\033[41;37m"   /* 红底白字 */
#define CL_GRE_WHT  "\033[42;37m"   /* 绿底白字 */
#define CL_YEW_WHT  "\033[43;37m"   /* 黄底白字 */
#define CL_BLUE_WHT "\033[44;37m"   /* 蓝底白字 */
#define CL_PPL_WHT  "\033[45;37m"   /* 紫底白字 */
#define CL_SKYB_WHT "\033[46;37m"   /* 天蓝底白字 */
#define CL_WHT_BLK  "\033[47;30m"   /* 白底黑字 */

// XXX(id, str, clr)
#define LOG_LEVEL_MAP(XXX) \
    XXX(LOG_LEVEL_DEBUG, "DEBUG", CL_WHITE)     \
    XXX(LOG_LEVEL_INFO,  "INFO ", CL_GREEN)     \
    XXX(LOG_LEVEL_WARN,  "WARN ", CL_YELLOW)    \
    XXX(LOG_LEVEL_ERROR, "ERROR", CL_RED)       \
    XXX(LOG_LEVEL_FATAL, "FATAL", CL_RED_WHT)

typedef enum {
    LOG_LEVEL_VERBOSE = 0,
#define XXX(id, str, clr) id,
    LOG_LEVEL_MAP(XXX)
#undef  XXX
    LOG_LEVEL_SILENT
} log_level_e;

#define DEFAULT_LOG_FILE            "default"
#define DEFAULT_LOG_LEVEL           LOG_LEVEL_VERBOSE
#define DEFAULT_LOG_REMAIN_DAYS     1
#define DEFAULT_LOG_MAX_BUFSIZE     (1<<14)  // 16k
#define DEFAULT_LOG_MAX_FILESIZE    (1<<24)  // 16M

// logger: default file_logger
// network_logger() see event/nlog.h
typedef void (*logger_handler)(int loglevel, const char* buf, int len);

void stdout_logger(int loglevel, const char* buf, int len);
void stderr_logger(int loglevel, const char* buf, int len);
void file_logger(int loglevel, const char* buf, int len);

typedef struct logger_s logger_t;
logger_t* logger_create();
void logger_destroy(logger_t* logger);

void logger_set_handler(logger_t* logger, logger_handler fn);
void logger_set_level(logger_t* logger, int level);
void logger_set_max_bufsize(logger_t* logger, unsigned int bufsize);
void logger_enable_color(logger_t* logger, int on);
int  logger_print(logger_t* logger, int level, const char* fmt, ...);

// below for file logger
void logger_set_file(logger_t* logger, const char* filepath);
void logger_set_max_filesize(logger_t* logger, unsigned long long filesize);
void logger_set_remain_days(logger_t* logger, int days);
void logger_enable_fsync(logger_t* logger, int on);
void logger_fsync(logger_t* logger);

// hlog: default logger instance
logger_t* default_logger();

// macro hlog*
#define hlog default_logger()
#define hlog_set_file(filepath)         logger_set_file(hlog, filepath)
#define hlog_set_level(level)           logger_set_level(hlog, level)
#define hlog_set_max_filesize(filesize) logger_set_max_filesize(hlog, filesize)
#define hlog_set_remain_days(days)      logger_set_remain_days(hlog, days)

#define hlogd(fmt, ...) logger_print(hlog, LOG_LEVEL_DEBUG, fmt " [%s:%d:%s]\n", ## __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define hlogi(fmt, ...) logger_print(hlog, LOG_LEVEL_INFO,  fmt " [%s:%d:%s]\n", ## __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define hlogw(fmt, ...) logger_print(hlog, LOG_LEVEL_WARN,  fmt " [%s:%d:%s]\n", ## __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define hloge(fmt, ...) logger_print(hlog, LOG_LEVEL_ERROR, fmt " [%s:%d:%s]\n", ## __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define hlogf(fmt, ...) logger_print(hlog, LOG_LEVEL_FATAL, fmt " [%s:%d:%s]\n", ## __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)

// below for android
#if defined(ANDROID) || defined(__ANDROID__)
#include <android/log.h>
#define LOG_TAG "JNI"
#undef  hlogd
#undef  hlogi
#undef  hlogw
#undef  hloge
#undef  hlogf
#define hlogd(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define hlogi(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define hlogw(...) __android_log_print(ANDROID_LOG_WARN,  LOG_TAG, __VA_ARGS__)
#define hloge(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define hlogf(...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__)
#endif

// macro alias
#if !defined(LOGD) && !defined(LOGI) && !defined(LOGW) && !defined(LOGE) && !defined(LOGF)
#define LOGD    hlogd
#define LOGI    hlogi
#define LOGW    hlogw
#define LOGE    hloge
#define LOGF    hlogf
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif  // HW_LOG_H_
