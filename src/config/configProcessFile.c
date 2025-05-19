#include <stdio.h>
#include <string.h>
#include <jansson.h>
#include "../../include/node.h"
#include "../../include/configHelpers.h"
#include "../../include/configProcessFile.h"
#include "../../include/configFileCompleted.h"

void config_process_file(int argc, char *argv[]) {

    json_t *root;
    json_error_t error;
    //-----------------------------------------------------------
    // Load the JSON config file
    //-----------------------------------------------------------
    root = json_load_file(node.config.configFile, 0, &error);
    if (!root) {
        fprintf(stderr, "[main][%d]: Error loading config file: %s (line %d)\n",
            node.pid.main, error.text, error.line);
        return;
    }

    //-----------------------------------------------------------
    // Access data and set rpiNode.config and external commands
    //-----------------------------------------------------------
    set_string_nullable(&node.config.id, json_object_get(root, "id"));
    set_boolean(&node.config.logValues, json_object_get(root, "logValues"));
    set_string_nullable(&node.config.clusterId, json_object_get(root, "clusterId"));
    set_string_nullable(&node.config.servePath, json_object_get(root, "servePath"));
    set_string_nullable(&node.config.i2cBusPath, json_object_get(root, "i2cBusPath"));
    set_uint16(&node.config.logMilliseconds, json_object_get(root, "logMilliseconds"));
    set_boolean(&node.config.logValuesToFile, json_object_get(root, "logValuesToFile"));
    set_boolean(&node.config.logValuesToInflux, json_object_get(root, "logValuesToInflux"));
    set_string_nullable(&node.config.dataFolderName, json_object_get(root, "dataFolderName"));
    set_uint16(&node.config.logValuesFileClearSeconds, json_object_get(root, "logValuesFileClearSeconds"));

    json_t *tmpfs = json_object_get(root, "tmpfs");
    if (json_is_object(tmpfs)) {
        set_boolean(&node.config.tmpfs.use, json_object_get(tmpfs, "use"));
        set_uint16(&node.config.tmpfs.sizeMB, json_object_get(tmpfs, "sizeMB"));
    }

    json_t *influx = json_object_get(root, "influxDB");
    if (json_is_object(influx)) {
        set_boolean(&node.config.influxDB.https, json_object_get(influx, "https"));
        set_uint8(&node.config.influxDB.dbVersion, json_object_get(influx, "dbVersion"));
        set_string_nullable(&node.config.influxDB.org, json_object_get(influx, "org"));
        set_string_nullable(&node.config.influxDB.token, json_object_get(influx, "token"));
        set_boolean(&node.config.influxDB.forbidReuse, json_object_get(influx, "forbidReuse"));
        set_boolean(&node.config.influxDB.freshConnect, json_object_get(influx, "freshConnect"));
        set_string_nullable(&node.config.influxDB.username, json_object_get(influx, "username"));
        set_string_nullable(&node.config.influxDB.password, json_object_get(influx, "password"));
        set_string_nullable(&node.config.influxDB.precision, json_object_get(influx, "precision"));
        set_uint16(&node.config.influxDB.totalTimeoutMs, json_object_get(influx, "totalTimeoutMs"));
        set_boolean(&node.config.influxDB.useServiceScan, json_object_get(influx, "useServiceScan"));
        set_string_nullable(&node.config.influxDB.dbOrBucket, json_object_get(influx, "dbOrBucket"));
        set_uint16(&node.config.influxDB.connectTimeoutMs, json_object_get(influx, "connectTimeoutMs"));
        set_uint16(&node.config.influxDB.postIntervalSeconds, json_object_get(influx, "postIntervalSeconds"));
        set_string_nullable(&node.config.influxDB.mDNSserviceType, json_object_get(influx, "mDNSserviceType"));
        set_ip4(&node.config.influxDB.serviceInfo.ip4address ,json_object_get(influx, "ipAddress"));
    }

    json_t *network = json_object_get(root, "network");
    if (json_is_object(network)) {
        set_boolean(&node.config.network.mDNS, json_object_get(network, "mDNS"));
        set_uint16(&node.config.network.httpPort, json_object_get(network, "httpPort"));
        set_boolean(&node.config.network.mqPublish, json_object_get(network, "mqPublish"));
        set_string_nullable(&node.config.network.httpmDNSserviceType, json_object_get(network, "httpmDNSserviceType"));
        set_string_nullable(&node.config.network.mqBrokermDNSserviceType, json_object_get(network, "mqBrokermDNSserviceType"));
    }

    //-----------------------------------------------------------
    // Free memory & complete
    //-----------------------------------------------------------
    json_decref(root);

    config_file_completed();

}