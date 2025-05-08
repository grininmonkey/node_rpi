#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../include/node.h"
#include "../../include/utilsVerbosePrint.h"
#include "../../include/utilsMakePath.h"

int setTmpfs(pid_t pid) {

    char *mount_point = NULL;  
    char size_m[15]; 

    pthread_mutex_lock(&node.lock);
    mount_point = make_path_n(2, node.config.servePath, node.config.dataFolderName);
    umount(mount_point);
    if (!node.config.tmpfs.use) {
        pthread_mutex_unlock(&node.lock);
        verbose_print("[main][%d]: use tmpfs=false, not creating tmpfs @ %s\n", pid, mount_point);
        free(mount_point);
        return 1; 
    }
    pthread_mutex_unlock(&node.lock);
    //-----------------------------------------------------------
    // Creating the mount point directory if it does not exist
    //-----------------------------------------------------------
    if (access(mount_point, F_OK) == -1) {
        if(mkdir(mount_point, 0755) == -1) {
            perror("mkdir failure");
            return 0;
        }
    }  
    
    //-----------------------------------------------------------
    // Mount tmpfs with a size limit
    //-----------------------------------------------------------
    snprintf(size_m, sizeof(size_m), "size=%iM", node.config.tmpfs.sizeMB);
    if (mount("tmpfs", mount_point, "tmpfs", MS_NOATIME, size_m) == -1) {
        perror("mount");
        return 0;
    }
    
    verbose_mutex_print("[main][%d]: tmpfs mounted successfully @ %s\n", pid, mount_point);

    //-----------------------------------------------------------
    // Clean up
    //-----------------------------------------------------------
    if (mount_point) free(mount_point);
    
    return 1;
}

