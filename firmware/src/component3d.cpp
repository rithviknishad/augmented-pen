/*
 * component3d.cpp
 * 
 * Implements the Component3D definition for use with 3 Dimensional variables.
 * 
 * Authors: RITHVIK NISHAD
 */

struct Component3D {
    static const int dimensions = 3;
    
    union {
        struct
        {
            double x, y, z;
        };
        struct
        {
            double alpha, beta, gamma;
        };
        struct
        {
            double roll, yaw, pitch;
        };
        double components[3];
    };

    Component3D(double _x = 0.0, double _y = 0.0, double _z = 0.0)
        : x(_x), y(_y), z(_z) { }

};