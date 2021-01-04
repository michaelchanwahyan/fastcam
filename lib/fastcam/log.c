/* fswebcam - FireStorm.cx's webcam generator                 */
/*============================================================*/
/* Copyright (C)2005-2011 Philip Heron <phil@sanslogic.co.uk> */
/*                                                            */
/* This program is distributed under the terms of the GNU     */
/* General Public License, version 2. You may use, modify,    */
/* and redistribute it under the terms of this license. A     */
/* copy should be included with this source.                  */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include "log.h"

#define RESET     (0)
#define BOLD      (1)
#define FG_BLACK  (30)
#define FG_RED    (31)
#define FG_GREEN  (32)
#define FG_BROWN  (33)
#define FG_BLUE   (34)
#define FG_PURPLE (35)
#define FG_CYAN   (36)
#define FG_GREY   (37)

char use_syslog = 0;
int fd_log = STDERR_FILENO;

char quiet = 0;
char verbose = 0;

/* Taken from the GCC manual and cleaned up a bit. */
char *vmake_message(const char *fmt, va_list ap)
{
    /* Start with 100 bytes. */
    int n, size = 100;
    char *p;
    
    p = (char *) malloc(size);
    if(!p) return(NULL);
    
    while(1)
    {
        char *np;
        va_list apc;
        
        /* Try to print in the allocated space. */
        va_copy(apc, ap);
        n = vsnprintf(p, size, fmt, apc);
        
        /* If that worked, return the string. */
        if(n > -1 && n < size) return(p);
        
        /* Else try again with more space. */
        if(n > -1) size = n + 1; /* gibc 2.1: exactly what is needed */
        else size *= 2; /* glibc 2.0: twice the old size */
        
        np = (char *) realloc(p, size);
        if(!np)
        {
            free(p);
            return(NULL);
        }
        
        p = np;
    }
    
    free(p);
        return(NULL);
}

char *make_message(const char *fmt, ... )
{
    va_list ap;
    char *msg;
    
    va_start(ap, fmt);
    msg = vmake_message(fmt, ap);
    va_end(ap);
    
    return(msg);
}

