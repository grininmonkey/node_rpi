
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
    //-------------------------------------------------------
    // Initialize PointerMap(s)
    // No mutex as this should be the only time
    // its modified. Considered read-only by other threads
    //-------------------------------------------------------
    node_map = map_create();
    if (node_map == NULL) {
        fprintf(stderr, "Error: Failed to create node map\n");
        exit(EXIT_FAILURE);
    }

    //-------------------------------------------------------
    // Initialize node, set non char defaults
    //-------------------------------------------------------
    node = (Node){
        .config = {
            .logValues = true,
            .logMilliseconds = 10,
            .logValuesToFile = false,
            .logValuesToInflux = true,
            .logValuesFileClearSeconds = 5,
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
                .forbidReuse = false,
                .freshConnect = false,
                .totalTimeoutMs = 5000,
                .useServiceScan = true,
                .connectTimeoutMs = 2000,
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

    //-------------------------------------------------------
    // Set node char* defaults
    //-------------------------------------------------------
    node.pid.main                               = getpid();
    node.config.verbose                         = has_arg(argc, argv, "-v");
    node.config.id                              = generate_uuid_string();
    node.config.clusterId                       = strdup("7B28F91B307D4AD5BF586057CEAF010A");
    node.config.servePath                       = strdup("/srv/node_rpi");
    node.config.configFile                      = strdup("/etc/node_rpi/config.json");
    node.config.i2cBusPath                      = strdup("/dev/i2c-1");
    node.config.dataFolderName                  = strdup("data");
    node.config.network.httpmDNSserviceType     = strdup("_nodeRpiHttp._tcp");
    node.config.network.mqBrokermDNSserviceType = strdup("_nodeRpiMQbroker._tcp");
    node.config.influxDB.org                    = NULL;
    node.config.influxDB.token                  = NULL;
    node.config.influxDB.username               = NULL;
    node.config.influxDB.password               = NULL;
    node.config.influxDB.precision              = strdup("ns");
    node.config.influxDB.dbOrBucket             = strdup("test");
    node.config.influxDB.mDNSserviceType        = strdup("_wsInfluxDB._tcp");

    //-------------------------------------------------------
    // Set config file from command line
    //-------------------------------------------------------
    const char *configFile = get_arg_value(argc, argv, "--config");
    if (configFile) {
        STRDUP_REPLACE_NULLABLE(node.config.configFile, configFile);
    }

    //-------------------------------------------------------
    // Set config properties from file
    //-------------------------------------------------------
    config_process_file(argc, argv);

    //-------------------------------------------------------
    // Populate node_map (config properties)
    //-------------------------------------------------------
    map_add(node_map, "node.config.id", &node.config.id, TYPE_STRING, false);
    map_add(node_map, "node.config.verbose", &node.config.verbose, TYPE_BOOL, false);
    map_add(node_map, "node.config.clusterId", &node.config.clusterId, TYPE_STRING, false);
    map_add(node_map, "node.config.servePath", &node.config.servePath, TYPE_STRING, false);
    map_add(node_map, "node.config.logValues", &node.config.logValues, TYPE_BOOL, false);
    map_add(node_map, "node.config.configFile", &node.config.configFile, TYPE_STRING, false);
    map_add(node_map, "node.config.i2cBusPath", &node.config.i2cBusPath, TYPE_STRING, false);
    map_add(node_map, "node.config.dataFolderName", &node.config.dataFolderName, TYPE_STRING, false);
    map_add(node_map, "node.config.logValuesToFile", &node.config.logValuesToFile, TYPE_BOOL, false);
    map_add(node_map, "node.config.logMilliseconds", &node.config.logMilliseconds, TYPE_UINT16, false);
    map_add(node_map, "node.config.logValuesToInflux", &node.config.logValuesToInflux, TYPE_BOOL, true);
    map_add(node_map, "node.config.logValuesFileClearSeconds", &node.config.logValuesFileClearSeconds, TYPE_UINT16, false);
    map_add(node_map, "node.config.tmpfs.use", &node.config.tmpfs.use, TYPE_BOOL, false);
    map_add(node_map, "node.config.tmpfs.sizeMB", &node.config.tmpfs.sizeMB, TYPE_UINT16, false);
    map_add(node_map, "node.config.influxDB.org", &node.config.influxDB.org, TYPE_STRING, true);
    map_add(node_map, "node.config.influxDB.host", &node.config.influxDB.serviceInfo.host, TYPE_STRING, true);
    map_add(node_map, "node.config.influxDB.port", &node.config.influxDB.serviceInfo.port, TYPE_UINT16, true);
    map_add(node_map, "node.config.influxDB.https", &node.config.influxDB.https, TYPE_BOOL, true);
    map_add(node_map, "node.config.influxDB.token", &node.config.influxDB.token, TYPE_STRING, true);
    map_add(node_map, "node.config.influxDB.username", &node.config.influxDB.username, TYPE_STRING, true);
    map_add(node_map, "node.config.influxDB.password", &node.config.influxDB.password, TYPE_STRING, true);
    map_add(node_map, "node.config.influxDB.dbVersion", &node.config.influxDB.dbVersion, TYPE_UINT8, true);
    map_add(node_map, "node.config.influxDB.precision", &node.config.influxDB.precision, TYPE_STRING, true);
    map_add(node_map, "node.config.influxDB.dbOrBucket", &node.config.influxDB.dbOrBucket, TYPE_STRING, true);
    map_add(node_map, "node.config.influxDB.cfgVersion", &node.config.influxDB.cfgVersion, TYPE_UINT16, false);
    map_add(node_map, "node.config.influxDB.ip4address", &node.config.influxDB.serviceInfo.ip4address, TYPE_IP4, true);
    map_add(node_map, "node.config.influxDB.ip6address", &node.config.influxDB.serviceInfo.ip6address, TYPE_IP6, true);
    map_add(node_map, "node.config.influxDB.forbidReuse", &node.config.influxDB.forbidReuse, TYPE_BOOL, true);
    map_add(node_map, "node.config.influxDB.freshConnect", &node.config.influxDB.freshConnect, TYPE_BOOL, true);
    map_add(node_map, "node.config.influxDB.totalTimeoutMs", &node.config.influxDB.totalTimeoutMs, TYPE_UINT16, true);
    map_add(node_map, "node.config.influxDB.mDNSserviceType", &node.config.influxDB.mDNSserviceType, TYPE_STRING, false);
    map_add(node_map, "node.config.influxDB.useServiceScan", &node.config.influxDB.useServiceScan, TYPE_BOOL, true);
    map_add(node_map, "node.config.influxDB.connectTimeoutMs", &node.config.influxDB.connectTimeoutMs, TYPE_UINT16, true);
    map_add(node_map, "node.config.influxDB.postIntervalSeconds", &node.config.influxDB.postIntervalSeconds, TYPE_UINT16, false);
    map_add(node_map, "node.config.network.mDNS", &node.config.network.mDNS, TYPE_BOOL, false);
    map_add(node_map, "node.config.network.mqPublish", &node.config.network.mqPublish, TYPE_BOOL, false);
    map_add(node_map, "node.config.network.httpPort", &node.config.network.httpPort, TYPE_UINT16, false);
    map_add(node_map, "node.config.network.mDNSserviceList", &node.config.network.mDNSserviceList, TYPE_SERVICE_LIST, false);
    map_add(node_map, "node.config.network.httpmDNSserviceType", &node.config.network.httpmDNSserviceType, TYPE_STRING, false);
    map_add(node_map, "node.config.network.mqBrokermDNSserviceType", &node.config.network.mqBrokermDNSserviceType, TYPE_STRING, false);
    map_add(node_map, "node.config.network.mqBrokermDNSserviceType", &node.config.network.mqBrokermDNSserviceType, TYPE_STRING, false);

    //-------------------------------------------------------
    // Populate node_map (sensor properties)
    //-------------------------------------------------------
    // soon™
    map_add(node_map, "sensor.mpu.accel.raw.x", &node.mpu.accel.raw.x, TYPE_INT, false);
    map_add(node_map, "sensor.mpu.accel.raw.y", &node.mpu.accel.raw.y, TYPE_INT, false);
    map_add(node_map, "sensor.mpu.accel.raw.z", &node.mpu.accel.raw.z, TYPE_INT, false);
    map_add(node_map, "sensor.mpu.accel.ms2.x", &node.mpu.accel.ms2.x, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.accel.ms2.y", &node.mpu.accel.ms2.y, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.accel.ms2.z", &node.mpu.accel.ms2.z, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.accel.g.x", &node.mpu.accel.g.x, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.accel.g.y", &node.mpu.accel.g.y, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.accel.g.z", &node.mpu.accel.g.z, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.gyro.raw.x", &node.mpu.gyro.raw.x, TYPE_INT, false);
    map_add(node_map, "sensor.mpu.gyro.raw.y", &node.mpu.gyro.raw.y, TYPE_INT, false);
    map_add(node_map, "sensor.mpu.gyro.raw.z", &node.mpu.gyro.raw.z, TYPE_INT, false);
    map_add(node_map, "sensor.mpu.gyro.dps.x", &node.mpu.gyro.dps.x, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.gyro.dps.y", &node.mpu.gyro.dps.y, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.gyro.dps.z", &node.mpu.gyro.dps.z, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.euler.roll", &node.mpu.euler.roll, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.euler.pitch", &node.mpu.euler.pitch, TYPE_FLOAT, false);
    map_add(node_map, "sensor.mpu.euler.yaw", &node.mpu.euler.yaw, TYPE_FLOAT, false);

}