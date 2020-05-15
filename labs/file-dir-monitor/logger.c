#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"

#define RESETCOLOR "\033[0m"
#define RESET       0
#define BRIGHT      1
#define DIM         2
#define UNDERLINE   3
#define BLINK       4
#define REVERSE     7
#define HIDDEN      8

#define BLACK       0
#define RED         1
#define GREEN       2
#define YELLOW      3
#define BLUE        4
#define MAGENTA     5
#define CYAN        6
#define WHITE       7

int currentlog = 0;

int initLogger(char *logType) {
    if(strcmp(logType,"stdout") == 0 || logType == ""){
        currentlog = 0;
        printf("\nInitializing Logger on: %s\n", logType);
    }
    else if(strcmp(logType, "syslog") == 0){
        currentlog = 1;
        printf("\nInitializing Logger on: %s\n", logType);
    }
    else{
        printf("Invalid Logger type \n");
        return -1;
    }
    return 0;
}

void textcolor(int attr, int fg, int bg)
{   char command[13];

    /* Command is the control command to the terminal */
    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    printf("%s", command);
}

int infof(const char *format, ...){
    va_list values;
    va_start(values, format);
    if(currentlog == 0){
        textcolor(BRIGHT, BLUE, HIDDEN);
        printf("Information:%s",RESETCOLOR);
        vprintf(format, values);
    }
    else{
        openlog("syslog", LOG_PID | LOG_CONS | LOG_NOWAIT , LOG_SYSLOG);
        syslog(LOG_INFO, format, values);
        closelog();
    }
    va_end(values);
    return 0;
}

int namef(const char *format, ...){
    va_list values;
    va_start(values, format);
    if(currentlog == 0){
        textcolor(BRIGHT, RED, HIDDEN);
        printf("Name:%s",RESETCOLOR);
        vprintf(format, values);
    }
    else{
        openlog("syslog", LOG_PID | LOG_CONS | LOG_NOWAIT , LOG_SYSLOG);
        syslog(LOG_INFO, format, values);
        closelog();
    }
    va_end(values);
    return 0;
}

int warnf(const char *format, ...){
    va_list values;
    va_start(values, format);
    if(currentlog == 0){
        textcolor(REVERSE, YELLOW, HIDDEN);
        printf("Warning:%s",RESETCOLOR);
        vprintf(format, values);
    }
    else{
        openlog("syslog", LOG_PID | LOG_CONS | LOG_NOWAIT , LOG_SYSLOG);
        syslog(LOG_INFO, format, values);
        closelog();
    }
    va_end(values);
    return 0;
}

int errorf(const char *format, ...){
    va_list values;
    va_start(values, format);
    if(currentlog == 0){
        textcolor(BLINK, RED, HIDDEN);
        printf("Error:%s",RESETCOLOR);
        vprintf(format, values);
    }
    else{
        openlog("syslog", LOG_PID | LOG_CONS | LOG_NOWAIT , LOG_SYSLOG);
        syslog(LOG_INFO, format, values);
        closelog();
    }
    va_end(values);
    return 0;
}

int panicf(const char *format, ...){
    va_list values;
    va_start(values, format);
    if(currentlog == 0){
        textcolor(BRIGHT, RED, HIDDEN);
        printf("Panic:%s",RESETCOLOR);
        vprintf(format, values);
    }
    else{
        openlog("syslog", LOG_PID | LOG_CONS | LOG_NOWAIT , LOG_SYSLOG);
        syslog(LOG_INFO, format, values);
        closelog();
    }
    va_end(values);
    return 0;
}

