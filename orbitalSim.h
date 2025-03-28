﻿

/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 

 * @copyright Copyright (c) 2022-2023
 */

#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "raylib.h"
#include "raymath.h"

/**
 * @brief Orbital body definition
 */
struct OrbitalBody
{
    char name[20]; // Name
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
    float timeStep, timeTotal;
    int bodyNumber;
    OrbitalBody *bodies;  // Puntero a estructura, se asigna como arreglo de manera dinámica usando new
    float centerMass;
};

OrbitalSim *constructOrbitalSim(float timeStep);
void destroyOrbitalSim(OrbitalSim *sim);

void updateOrbitalSim(OrbitalSim *sim);

#endif
