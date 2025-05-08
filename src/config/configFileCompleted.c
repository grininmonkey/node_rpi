#include <stdio.h>
#include "../../include/node.h"

void config_file_completed() {

    if (node.config.verbose) {
        printf("[main][%d]: Config loaded successfully\n", node.pid.main);
        printf("[main][%d]:\t\t# id: %s\n", node.pid.main, node.config.id);
        printf("[main][%d]:\t\t# clusterId: %s\n", node.pid.main, node.config.clusterId);
        printf("[main][%d]:\t\t# servePath: %s\n", node.pid.main, node.config.servePath);
        printf("[main][%d]:\t\t# i2cBusPath: %s\n", node.pid.main, node.config.i2cBusPath);
        printf("[main][%d]:\t\t# dataFolderName: %s\n", node.pid.main, node.config.dataFolderName);
        printf("[main][%d]:\t\t# logValues: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.logValues));
        printf("[main][%d]:\t\t# logValuesToFile: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.logValuesToFile));
        printf("[main][%d]:\t\t# logValuesToInflux: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.logValuesToInflux));
        printf("[main][%d]:\t\t# logValuesFileClearSeconds: %i\n", node.pid.main, node.config.logValuesFileClearSeconds);
        printf("[main][%d]:\t\t# logMilliseconds: %i\n", node.pid.main, node.config.logMilliseconds);
        printf("[main][%d]:\t\t# tmpfs->use: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.tmpfs.use));
        printf("[main][%d]:\t\t# tmpfs->sizeMB: %i\n", node.pid.main, node.config.tmpfs.sizeMB);
        printf("[main][%d]:\t\t# Influx->https: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.influxDB.https));
        printf("[main][%d]:\t\t# Influx->org: %s\n", node.pid.main, node.config.influxDB.org);
        printf("[main][%d]:\t\t# Influx->token: %s\n", node.pid.main, node.config.influxDB.token);
        printf("[main][%d]:\t\t# Influx->username: %s\n", node.pid.main, node.config.influxDB.username);
        printf("[main][%d]:\t\t# Influx->password: %s\n", node.pid.main, node.config.influxDB.password);
        printf("[main][%d]:\t\t# Influx->dbVersion: %i\n", node.pid.main, node.config.influxDB.dbVersion);
        printf("[main][%d]:\t\t# Influx->precision: %s\n", node.pid.main, node.config.influxDB.precision);
        printf("[main][%d]:\t\t# Influx->dbOrBucket: %s\n", node.pid.main, node.config.influxDB.dbOrBucket);
        printf("[main][%d]:\t\t# Influx->port: %i\n", node.pid.main, node.config.influxDB.serviceInfo.port);
        printf("[main][%d]:\t\t# Influx->freshConnect: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.influxDB.freshConnect));
        printf("[main][%d]:\t\t# Influx->forbidReuse: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.influxDB.forbidReuse));
        //printf("[main][%d]:\t\t# Influx->host: %s\n", node.pid.main, node.config.influxDB.serviceInfo.);
        printf("[main][%d]:\t\t# Influx->mDNSserviceType: %s\n", node.pid.main, node.config.influxDB.mDNSserviceType);
        printf("[main][%d]:\t\t# Influx->useServiceScan: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.influxDB.useServiceScan));
        printf("[main][%d]:\t\t# Influx->connectTimeoutMs: %i\n", node.pid.main, node.config.influxDB.connectTimeoutMs);
        printf("[main][%d]:\t\t# Influx->totalTimeoutMs: %i\n", node.pid.main, node.config.influxDB.totalTimeoutMs);
        printf("[main][%d]:\t\t# Influx->postIntervalSeconds: %i\n", node.pid.main, node.config.influxDB.postIntervalSeconds);
        printf("[main][%d]:\t\t# network->mDNS: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.network.mDNS));
        printf("[main][%d]:\t\t# network->httpPort: %i\n", node.pid.main, node.config.network.httpPort);
        printf("[main][%d]:\t\t# network->mqPublish: %s\n", node.pid.main, INT_TRUE_FALSE_STR(node.config.network.mqPublish));
        printf("[main][%d]:\t\t# network->httpmDNSserviceType: %s\n", node.pid.main, node.config.network.httpmDNSserviceType);
        printf("[main][%d]:\t\t# network->mqBrokermDNSserviceType: %s\n", node.pid.main, node.config.network.mqBrokermDNSserviceType);
    }

}