#ifndef WSNODE_H
#define WSNODE_H

#include <time.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <jansson.h>
#include <sys/types.h>
#include <curl/curl.h>
//#include <arpa/inet.h>
#include <netinet/in.h>
#include <avahi-client/client.h>
//------------------------------------------------------------------
// macros
//------------------------------------------------------------------
#define INT_TRUE_FALSE(x) ((x) > 0 ? 1 : 0)
#define INT_TRUE_FALSE_STR(x) ((x) > 0 ? "true" : "false")
#define INT_GT_ZERO_AND_LE(x, y) (((x) > 0) && ((x) <= (y)) ? 1 : 0)
#define SAFE_STRCMP(a, b) (((a) && (b)) ? strcmp((a), (b)) : ((a) == (b) ? 0 : -1))
#define SAFE_FREE(p) do { if ((p) != NULL) { free(p); (p) = NULL; } } while (0)
#define SAFE_STRCPY(dest, src)                              \
    do {                                                    \
        if ((const void *)(dest) != (const void *)(src)) {  \
            strncpy((dest), (src), sizeof(dest) - 1);       \
            (dest)[sizeof(dest) - 1] = '\0';                \
        }                                                   \
    } while(0)
#define STRDUP_REPLACE_NULLABLE(dst, src) \
    do { free(dst); dst = (src) ? strdup(src) : NULL; } while (0)
#define STRDUP_REPLACE(dst, src) \
    do { free(dst); dst = strdup(src); } while (0)


//------------------------------------------------------------------
// Defines
//------------------------------------------------------------------
#define GRAVITY 9.80665f // m/s²
#define MAP_SIZE 128     // 96 (entries) / .75 (Load Factor) = 128
#define DATA_FILE_NAME "node_values.txt"
#define MPU_DATA_FILE_NAME "mpu_ilp_values.txt"

//------------------------------------------------------------------
// Sensor: MPU6050 struct(s)
//------------------------------------------------------------------
typedef struct {
    float ax, ay, az;
    float gx, gy, gz;
} AllAxes;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} IntAxes;

typedef struct {
    float x;
    float y;
    float z;
} FloatAxes;

typedef struct {
    float     roll;
    float     pitch;
    float     yaw;
    FloatAxes accel;
} Euler;

typedef struct{
    IntAxes   raw;
    FloatAxes g;
    FloatAxes ms2;
} Accel;

typedef struct{
    IntAxes   raw;
    FloatAxes dps;
} Gyro;

typedef struct {
    float c, f;
} Temp;

typedef struct {
    uint8_t  i2cAddress;
    uint8_t  accel_range;
    uint8_t  gyro_range;
    float    accel_scale;
    float    gyro_scale;
    uint16_t frequency_ms;
} MPUconfig;

typedef struct {
    uint8_t id;
    Accel   accel;
    Gyro    gyro;
    Temp    temp;
    Euler   euler;
    uint64_t timestamp;
    struct timespec last_dt;
} MPU;

//------------------------------------------------------------------
// Structs (Base)
//------------------------------------------------------------------
typedef struct {
    void *(*func)(void *);
    const char *name;
} ThreadFunctionEntry;

typedef struct {
    int one_shot;   
    int timeout_secs;
} MdnsScanParams;

typedef enum {
    TYPE_INT,
    TYPE_IP4,
    TYPE_IP6,
    TYPE_BOOL,
    TYPE_FLOAT,
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_STRING,
    TYPE_DOUBLE,
    TYPE_STRUCT,
    TYPE_SERVICE_LIST,
    TYPE_UNKNOWN
} ValueType;

typedef struct MapEntry {
    char *key;
    void *ptr;
    bool  canUpdate;
    ValueType type;
    struct MapEntry *next;
} MapEntry;

typedef struct {
    MapEntry *buckets[MAP_SIZE];
} PointerMap;

typedef struct {
    CURL *curl;
    bool  ready;
    pid_t pid;
    int   dbVersion;
    int   cfgVersion; 
    char *full_url;
    char *token;
    struct curl_slist *headers;
} InfluxCurl;

typedef struct ServiceInfo{
    char                *name;
    char                *host;
    char                *type;
    char                *interface;
    uint16_t            port;
    uint32_t            ip4address;
    struct in6_addr     ip6address;
    struct ServiceInfo  *next;
} ServiceInfo;

typedef struct {
    bool      use;
    bool      created;
    uint16_t  sizeMB;
} Tmpfs;

typedef struct {
    bool          https;
    bool          useServiceScan;
    bool          freshConnect;
    bool          forbidReuse;
    ServiceInfo   serviceInfo;
    uint16_t      postIntervalSeconds;
    uint8_t       dbVersion;
    uint16_t      cfgVersion;
    uint16_t      connectTimeoutMs;
    uint16_t      totalTimeoutMs;
    char         *org;
    char         *token;
    char         *username;
    char         *password;
    char         *precision;
    char         *dbOrBucket;
    char         *mDNSserviceType;
} InfluxDB;

typedef struct {
    bool        mDNS;
    uint16_t    httpPort;
    bool        mqPublish;
    ServiceInfo *mDNSserviceList;
    char        *httpmDNSserviceType;
    char        *mqBrokermDNSserviceType;
} Network;

typedef struct {
    char      *id;
    char      *clusterId;
    char      *configFile;
    char      *servePath;
    char      *i2cBusPath;
    char      *dataFolderName;
    uint16_t  logMilliseconds;
    bool      verbose;
    bool      logValues;
    bool      logValuesToFile;
    bool      logValuesToInflux;
    uint16_t  logValuesFileClearSeconds;
    Tmpfs     tmpfs;
    MPUconfig mpu;
    Network   network;
    InfluxDB  influxDB;
} Config;

typedef struct {
    pid_t main;
    pid_t http;
    pid_t mDNS;
    pid_t dbs;
    pid_t mpu;
    pid_t bme;
} Pid;

//------------------------------------------------------------------
// Primary Struct
//------------------------------------------------------------------

typedef struct{
    Pid     pid;
    MPU     mpu;
    Config  config;
    pthread_mutex_t lock;
} Node;

extern Node node;
extern PointerMap *node_map;


#endif