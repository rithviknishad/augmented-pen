/*
 * Standalone Positioning Project's HW firmware.
 *  
 * Responsible for initializing the WiFi module, initializing
 * & interfacing w/ MPU6050 device, and implementing the
 * Standalone Positioning System (SPS).
 */

#include "component3d.cpp"

Component3D linearAcceleration, linearVelocity, position, positionOffset;
Component3D angularAcceleration, angularVelocity, orientation, orientationOffset;


void setup() {
    // initialize MPU6050

    // establish wifi connection

    // establish mqtt client
}

void loop() {
    // wifi connectivity check
}