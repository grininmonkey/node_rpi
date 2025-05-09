#include <string.h>
#include <unistd.h>
#include "../include/node.h"
#include "../include/nodeInit.h"
#include "../include/nodeMap.h"
#include "../include/utilsNodeFree.h"
#include "../include/utilsHasArgument.h"
#include "../include/clientRouteInput.h"
#include "../include/utilsSignalHandler.h"
#include "../include/serviceLaunchThreads.h"

//+++++++++++++++++++++++++++++++++++++++++++++++++++
// Main
//+++++++++++++++++++++++++++++++++++++++++++++++++++
int main(int argc, char *argv[]) {
    //-----------------------------------------------
    // Command line check
    //-----------------------------------------------
    if (argc < 2) {
        client_route_input_usage(argc, argv);
        return 1;
    }

    //-----------------------------------------------
    // Register signal handlers
    //-----------------------------------------------
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    //-----------------------------------------------
    // Initialise global node struct 
    //-----------------------------------------------
    node_init(argc, argv);

    //-----------------------------------------------
    // Route to Service or Client funcs
    //-----------------------------------------------
    if (has_arg(argc, argv, "--service") == 1) {
        service_launch_threads(argc, argv);
    } else {
        client_route_input(argc, argv);
    }
    
    //-----------------------------------------------
    // Completed
    //----------------------------------------------- 
    free_config();
    free_service_list(true);
    map_destroy(node_map);
    
    return 0;

}

