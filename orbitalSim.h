﻿#include "raymath.h"

/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 

 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

/**
 * @brief Orbital body definition
 */
struct OrbitalBody
{
    // Fill in your code here...
    float mass;		  // [kg]
    float radius;	  // [m]
    Color color;	  // Raylib color
    Vector3 position; // [m]
    Vector3 velocity; // [m/s]

};

/**
 * @brief Orbital simulation definition
 */
struct OrbitalSim
{
    // Fill in your code here...
    double timeStep, timeTotal;
    int bodyNumber;
    OrbitalBody *bodys;

};

OrbitalSim *constructOrbitalSim(float timeStep);
void destroyOrbitalSim(OrbitalSim *sim);

void updateOrbitalSim(OrbitalSim *sim);

#endif
