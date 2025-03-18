/**
 * @brief Orbital simulation
 * @author Marc S. Ressl
 *
 * @copyright Copyright (c) 2022-2023
 */

// Enables M_PI #define in Windows
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include "orbitalSim.h"
#include "ephemerides.h"

#define GRAVITATIONAL_CONSTANT 6.6743E-11F
#define ASTEROIDS_MEAN_RADIUS 4E11F

/**
 * PROTOTIPES
 */

 /**
  * @brief Calculates the acceleration of a planet
  * @param index is the index that point to the planet which is beeing calculated
  * @param sim pointer that contains all the planets data
  */
 Vector3 calcAcceleration (OrbitalSim *sim, int index);


/**
 * @brief Gets a uniform random value in a range
 *
 * @param min Minimum value
 * @param max Maximum value
 * @return The random value
 */
float getRandomFloat(float min, float max)
{
    return min + (max - min) * rand() / (float)RAND_MAX;
}

/**
 * @brief Configures an asteroid
 *
 * @param body An orbital body
 * @param centerMass The mass of the most massive object in the star system
 */
void configureAsteroid(OrbitalBody *body, float centerMass)
{
    // Logit distribution
    float x = getRandomFloat(0, 1);
    float l = logf(x) - logf(1 - x) + 1;

    // https://mathworld.wolfram.com/DiskPointPicking.html
    float r = ASTEROIDS_MEAN_RADIUS * sqrtf(fabsf(l));
    float phi = getRandomFloat(0, 2.0F * (float)M_PI);

    // Surprise!
    // phi = 0;

    // https://en.wikipedia.org/wiki/Circular_orbit#Velocity
    float v = sqrtf(GRAVITATIONAL_CONSTANT * centerMass / r) * getRandomFloat(0.6F, 1.2F);
    float vy = getRandomFloat(-1E2F, 1E2F);

    // Fill in with your own fields:
    body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    body->radius = 2E3F; // Typical asteroid radius: 2km
    body->color = GRAY;
    body->position = {r * cosf(phi), 0, r * sinf(phi)};
    body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
}

/**
 * @brief Constructs an orbital simulation
 *
 * @param float The time step
 * @return The orbital simulation
 */
OrbitalSim *constructOrbitalSim(float timeStep)
{

    OrbitalSim *p = new OrbitalSim();
    p->bodyNumber = SOLARSYSTEM_BODYNUM+ASTEROID_BODYNUM;
    p->bodies = new OrbitalBody[p->bodyNumber];
    p->timeStep = timeStep;     //User defined.
    p->timeTotal = 0;

    int i;
    for(i=0 ; i < SOLARSYSTEM_BODYNUM ; i++) {
        p->bodies[i].mass = solarSystem[i].mass;
        p->bodies[i].radius = solarSystem[i].radius;
        p->bodies[i].color = solarSystem[i].color;
        p->bodies[i].position = solarSystem[i].position;
        p->bodies[i].velocity = solarSystem[i].velocity;
    }
    for(i = SOLARSYSTEM_BODYNUM ; i < ASTEROID_BODYNUM ; i++) {
        configureAsteroid(& p->bodies[i], solarSystem->mass);    //Masa del sol
    }

    return p; // Returns pointer to your new orbital sim.
}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim)
{

    delete[] sim->bodies;
    delete sim;

}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim *sim) 
{
    int index;

    for (index=0 ; index < sim->bodyNumber ; index++){    //Nota: Usar bodyNumber como cota.
    
    float mass = sim->bodies[index].mass;
    Vector3 force, aceleration;

    aceleration = calcAcceleration(sim,index); //Calculates aceleration vector

    sim->bodies[index].velocity = Vector3Add(sim->bodies[index].velocity, Vector3Scale(aceleration,sim->timeStep) ); //Calculates and saves the new velocity

    sim->bodies[index].position = Vector3Add(sim->bodies[index].position, Vector3Scale(sim->bodies[index].velocity,sim->timeStep)); //Calculates and saves the new position

    }

}

Vector3 calcAcceleration (OrbitalSim *sim, int index){
    int i;
    Vector3 totalAcceleration = Vector3Zero();

    for(i=0 ; i < sim->bodyNumber ; i++){

        if(i!=index){   //skip (no comparar consigo mismo)
            float acceleration;
            float m2 = sim->bodies[i].mass;

            float vectorModule = Vector3DistanceSqr(sim->bodies[index].position,sim->bodies[i].position);   //norma al cuadrado.

            if (vectorModule<0.1){ //it makes sure it's not dividing by cero
                vectorModule = 0.1;
            }


            acceleration = -((GRAVITATIONAL_CONSTANT*m2)/vectorModule);
            
            Vector3 normal = Vector3Zero();
            normal = Vector3Subtract(sim->bodies[index].position, sim->bodies[i].position); //Create a normal vector and multiply it by the force to obtain force vector
            normal = Vector3Normalize(normal);
            normal = Vector3Scale(normal,acceleration);

            totalAcceleration = Vector3Add(totalAcceleration, normal);
        }
    }
    return totalAcceleration;
}