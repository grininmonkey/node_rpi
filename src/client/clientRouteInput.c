#include <stdio.h>
#include "../../include/clientScan.h"
#include "../../include/utilsHasArgument.h"

void client_route_input_usage(int argc, char *argv[]) {

    fprintf(stderr, "\nUsage: %s --service | --get-tag node-id=<nodeID> tag-id=<tagID> ....\n\n", argv[0]);

}

void client_route_input(int argc, char *argv[]) {

    if (has_arg(argc, argv, "--scan") == 1)
        return client_scan(argc, argv);
        

    //soon™
    printf("\nClient commands comming soon\u2122\n\n");

    return;
}