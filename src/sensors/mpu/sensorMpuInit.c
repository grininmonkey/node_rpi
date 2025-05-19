
/*
void mpu6050_init(mpuData *mpu) {
    //-------------------------------------------------------
    // Initialize MPU6050
    //-------------------------------------------------------
    mpuFD = open(I2C_DEV, O_RDWR);
    if (mpuFD < 0 || ioctl(mpuFD, I2C_SLAVE, mpu->address) < 0) {
        perror("[MPU6050] I2C setup failed");
        return;
    }
    //-------------------------------------------------------
    // Write 0x00 to register 0x6B to wake up MPU6050
    //-------------------------------------------------------
    char config[2] = {0x6B, 0x00};  
    if (write(mpuFD, config, 2) != 2) {
        printf("[MPU6050] Error waking up sensor\n");
    } else {
        printf("[MPU6050] Sensor woke up\n");
    }
    //-------------------------------------------------------
    // Configure Accel & Gyro scale
    //-------------------------------------------------------
    char accel_config[2] = {0x1C, mpu->ac_config};  
    char gyro_config[2] = {0x1B, mpu->gy_config};
    
    if (write(mpuFD, accel_config, 2) != 2) {
        printf("[MPU6050] Error configuring accelerometer\n");
    }
    
    if (write(mpuFD, gyro_config, 2) != 2) {
        printf("[MPU6050] Error configuring gyroscope\n");
    }    
    //-------------------------------------------------------
    // Read back Accel & Gyro scale
    //-------------------------------------------------------
    mpu->id         = i2c_read_register(mpuFD, 0x75);
    mpu->ac_config  = i2c_read_register(mpuFD, 0x1C);
    mpu->gy_config  = i2c_read_register(mpuFD, 0x1B);

    switch (mpu->ac_config & 0x18) {
        case 0x00: mpu->ac_scale = 16384.0f; break; // ±2g
        case 0x08: mpu->ac_scale = 8192.0f;  break; // ±4g
        case 0x10: mpu->ac_scale = 4096.0f;  break; // ±8g
        case 0x18: mpu->ac_scale = 2048.0f;  break; // ±16g
    }

    switch (mpu->gy_config & 0x18) {
        case 0x00: mpu->gy_scale = 131.0f;  break; // ±250 °/s
        case 0x08: mpu->gy_scale = 65.5f;   break; // ±500 °/s
        case 0x10: mpu->gy_scale = 32.8f;   break; // ±1000 °/s
        case 0x18: mpu->gy_scale = 16.4f;   break; // ±2000 °/s
    }
    //-------------------------------------------------------
    // Set Defaults if missing
    //-------------------------------------------------------
    mpu->last_time.tv_sec = 0;
    mpu->last_time.tv_nsec = 0;
    mpu->filtered_alpha = (mpu->filtered_alpha == 0.0f) ? 0.1f : mpu->filtered_alpha;
    //-------------------------------------------------------
    // Init Madgwick(s)
    //-------------------------------------------------------
    madgwick_init(&mpu->madgwick1, mpu->madgwick1.sampleFreq, mpu->madgwick1.beta);
    madgwick_init(&mpu->madgwick2, mpu->madgwick2.sampleFreq, mpu->madgwick2.beta);
    //-------------------------------------------------------
    // Output
    //-------------------------------------------------------
    printf("[MPU6050] id: 0x%02X\n", mpu->id);
    printf("[MPU6050] accel_config: 0x%02X\n", mpu->ac_config);
    printf("[MPU6050] accel_scale: %.2f\n", mpu->ac_scale);
    printf("[MPU6050] gyro_config:  0x%02X\n", mpu->gy_config);
    printf("[MPU6050] gyro_scale:  %.2f\n", mpu->gy_scale);

}
*/