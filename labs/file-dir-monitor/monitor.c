#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <limits.h>
#include "logger.h"

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int inotifyFd;
char* lastName;
char* path;

static int display_info(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf){
    int wd = inotify_add_watch(inotifyFd, fpath, IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO);
    if (wd == -1){
        errorf("Error in inotify_add_watch \n");
        exit(EXIT_FAILURE);
    }
    return 0;
}

static void displayInotifyEvent(struct inotify_event *i){
    if(i->mask & IN_CREATE){
        infof("IN_CREATE ");
    }
    else if(i->mask & IN_DELETE){
        infof("IN_DELETE ");
        if(i->mask && IN_ISDIR){
            inotify_rm_watch(inotifyFd, i->wd);    
        }
    }
    else if(i->mask & IN_MOVED_FROM){
        infof("IN_MOVED_FROM ");
    }
    else if(i->mask & IN_MOVED_TO){
        infof("IN_MOVED_TO ");
    }

    if(i->len > 1){
        if(i->name != NULL){
            namef("%s\n", i->name);
        }
    }
} 

int main(int argc, char* argv[]){
    // Place your magic here
    if (argc < 2 || argc > 2){
        warnf("Usage: ./monitor {Dir} \n");
        exit(EXIT_FAILURE);
    }
    
    int nftwTree;
    int flags = FTW_PHYS;
    char buf[BUF_LEN] __attribute__ ((aligned(8)));
    ssize_t numRead;
    char *p;
    struct inotify_event *event;

    inotifyFd = inotify_init();
    if (inotifyFd == -1){
        errorf("In inotify_init \n");
        exit(EXIT_FAILURE);
    }

    nftwTree = nftw(argv[1], display_info, 20, flags);
    if(nftwTree == -1){
        errorf("In nftw init \n");
        exit(EXIT_FAILURE);
    }

    for(;;){

        numRead = read(inotifyFd, buf, BUF_LEN);
        if(numRead == 0){
            panicf("read() from inotifyFd returned 0! \n");
            exit(EXIT_FAILURE);
        }

        if(numRead == -1){
            errorf("Error in read() \n");
            exit(EXIT_FAILURE);
        }

        for(p = buf; p < buf + numRead; ){
            event = (struct inotify_event *) p;
            displayInotifyEvent(event);
            p += sizeof(struct inotify_event *) + event->len;

        }

        nftwTree = nftw(argv[1], display_info, 20, flags);
        if(nftwTree == -1){
            errorf("Errror in nftw \n");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}
