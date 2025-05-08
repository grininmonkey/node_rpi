#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/file.h>

#define PID_FILE "/tmp/wsNode_service.pid"

int check_service_running(pid_t pid) {
    int fd = open(PID_FILE, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open");
        return -1;
    }

    if (flock(fd, LOCK_EX | LOCK_NB) < 0) {
        if (errno == EWOULDBLOCK) {
            // Another instance is running
            close(fd);
            return 1;
        } else {
            perror("flock");
            close(fd);
            return -1;
        }
    }

    //-----------------------------------------------------
    // Write PID to the file (for debugging)
    //-----------------------------------------------------
    if (ftruncate(fd, 0) == -1) {
        perror("PID File ftruncate failed");
        close(fd);
        return -1;
    }    
    
    dprintf(fd, "%d\n", pid);

    //-----------------------------------------------------
    // Don't close fd — keep it open to maintain the lock
    //-----------------------------------------------------
    return 0;

}