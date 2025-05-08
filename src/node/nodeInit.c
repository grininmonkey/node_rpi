
#include <unistd.h>
#include <uuid/uuid.h>
#include "../../include/nodeInit.h"
#include "../../include/nodeMap.h"
#include "../../include/utilsHasArgument.h"
#include "../../include/utilsGenerateUuid.h"
#include "../../include/configProcessFile.h"

Node node;
PointerMap *node_map;

void node_init(int argc, char *argv[]) {

    //------------------------------------------------
    // Initialize node_map - Hash Map
    //------------------------------------------------
    node_map = map_create();
    if (node_map == NULL) {
        fprintf(stderr, "Error: Failed to create node map\n");
        exit(EXIT_FAILURE);
    }

    //------------------------------------------------
    // Initialize node, set non char defaults
    //------------------------------------------------
    node = (Node){
        .config = {
            .logValues = true,
            .logValuesToFile = false,
            .logValuesToInflux = true,
            .logValuesFileClearSeconds = 5,
            .logMilliseconds = 10,
            .tmpfs = {
                .use = true,
                .sizeMB = 5
            },
            .influxDB = {
                .https = false,
                .dbVersion = 1,
                .cfgVersion = 1,
                .serviceInfo = {
                    .port = 8086,
                },
                .useServiceScan = true,
                .postIntervalSeconds = 2
            },
            .network = {
                .mDNS = true,
                .httpPort = 80,
                .mqPublish = 0,
                .mDNSserviceList = NULL
            },
        },
        .lock = PTHREAD_MUTEX_INITIALIZER
    };

    //-----------------------------------------------
    // Set node char *defaults
    //-----------------------------------------------
    node.pid.main                               = getpid();
    node.config.verbose                         = has_arg(argc, argv, "-v");
    node.config.id                              = generate_uuid_string();
    node.config.clusterId                       = strdup("7B28F91B307D4AD5BF586057CEAF010A");
    node.config.servePath                       = strdup("/srv/wsNode");
    node.config.configFile                      = strdup("/etc/wsNode/config.json");
    node.config.i2cBusPath                      = strdup("/dev/i2c-1");
    node.config.dataFolderName                  = strdup("data");
    node.config.network.httpmDNSserviceType     = strdup("_wsNodeHttp._tcp");
    node.config.network.mqBrokermDNSserviceType = strdup("_wsNodeMQbroker._tcp");
    node.config.influxDB.org                    = NULL;
    node.config.influxDB.token                  = NULL;
    node.config.influxDB.username               = NULL;
    node.config.influxDB.password               = NULL;
    node.config.influxDB.precision              = strdup("ns");
    node.config.influxDB.dbOrBucket             = strdup("test");
    node.config.influxDB.mDNSserviceType        = strdup("_wsInfluxDB._tcp");

    //-----------------------------------------------
    // Set config file from command line
    //-----------------------------------------------
    const char *configFile = get_arg_value(argc, argv, "--config");
    if (configFile) {
        STRDUP_REPLACE_NULLABLE(node.config.configFile, configFile);
    }

    //-----------------------------------------------
    // Set config properties from file
    //-----------------------------------------------
    config_process_file(argc, argv);

    //-----------------------------------------------
    // Populate node_map - Hash Map
    //-----------------------------------------------
    map_add(node_map, "node.config.id", &node.config.id, TYPE_STRING);
    map_add(node_map, "node.config.clusterId", &node.config.clusterId, TYPE_STRING);
    map_add(node_map, "node.config.servePath", &node.config.servePath, TYPE_STRING);
    map_add(node_map, "node.config.configFile", &node.config.configFile, TYPE_STRING);
    map_add(node_map, "node.config.i2cBusPath", &node.config.i2cBusPath, TYPE_STRING);
    map_add(node_map, "node.config.dataFolderName", &node.config.dataFolderName, TYPE_STRING);
    map_add(node_map, "node.config.verbose", &node.config.verbose, TYPE_BOOL);
    map_add(node_map, "node.config.logValues", &node.config.logValues, TYPE_BOOL);
    map_add(node_map, "node.config.logValuesToFile", &node.config.logValuesToFile, TYPE_BOOL);
    map_add(node_map, "node.config.logValuesToInflux", &node.config.logValuesToInflux, TYPE_BOOL);
    map_add(node_map, "node.config.logValuesFileClearSeconds", &node.config.logValuesFileClearSeconds, TYPE_INT);
    map_add(node_map, "node.config.logMilliseconds", &node.config.logMilliseconds, TYPE_INT);
    map_add(node_map, "node.config.tmpfs.use", &node.config.tmpfs.use, TYPE_BOOL);
    map_add(node_map, "node.config.tmpfs.sizeMB", &node.config.tmpfs.sizeMB, TYPE_INT);
    map_add(node_map, "node.config.influxDB.https", &node.config.influxDB.https, TYPE_BOOL);
    map_add(node_map, "node.config.influxDB.dbVersion", &node.config.influxDB.dbVersion, TYPE_INT);
    map_add(node_map, "node.config.influxDB.cfgVersion", &node.config.influxDB.cfgVersion, TYPE_INT);
    map_add(node_map, "node.config.influxDB.serviceInfo.port", &node.config.influxDB.serviceInfo.port, TYPE_INT);
    map_add(node_map, "node.config.influxDB.useServiceScan", &node.config.influxDB.useServiceScan, TYPE_BOOL);
    map_add(node_map, "node.config.influxDB.postIntervalSeconds", &node.config.influxDB.postIntervalSeconds, TYPE_INT);
    map_add(node_map, "node.config.influxDB.org", &node.config.influxDB.org, TYPE_STRING);
    map_add(node_map, "node.config.influxDB.token", &node.config.influxDB.token, TYPE_STRING);
    map_add(node_map, "node.config.influxDB.username", &node.config.influxDB.username, TYPE_STRING);
    map_add(node_map, "node.config.influxDB.password", &node.config.influxDB.password, TYPE_STRING);
    map_add(node_map, "node.config.influxDB.precision", &node.config.influxDB.precision, TYPE_STRING);
    map_add(node_map, "node.config.influxDB.dbOrBucket", &node.config.influxDB.dbOrBucket, TYPE_STRING);
    map_add(node_map, "node.config.influxDB.mDNSserviceType", &node.config.influxDB.mDNSserviceType, TYPE_STRING);
    map_add(node_map, "node.config.network.mqPublish", &node.config.network.mqPublish, TYPE_INT);
    map_add(node_map, "node.config.network.mqBrokermDNSserviceType", &node.config.network.mqBrokermDNSserviceType, TYPE_STRING);
    map_add(node_map, "node.config.network.httpPort", &node.config.network.httpPort, TYPE_INT);
    map_add(node_map, "node.config.network.mDNS", &node.config.network.mDNS, TYPE_BOOL);
    map_add(node_map, "node.config.network.mDNSserviceType", &node.config.network.httpmDNSserviceType, TYPE_STRING);
    map_add(node_map, "node.config.network.httpmDNSserviceType", &node.config.network.httpmDNSserviceType, TYPE_STRING);
    map_add(node_map, "node.config.network.mqBrokermDNSserviceType", &node.config.network.mqBrokermDNSserviceType, TYPE_STRING);
    map_add(node_map, "node.config.network.mDNSserviceList", &node.config.network.mDNSserviceList, TYPE_SERVICE_LIST);

}