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
  * @brief Calculates the total force that a planet is recieving from the others planets
  * @param index is the index that point to the planet which is beeing calculated
  * @param sim pointer that contains all the planets data
  */
 Vector3 calcForces (OrbitalSim *sim, int index);


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
    // body->mass = 1E12F;  // Typical asteroid weight: 1 billion tons
    // body->radius = 2E3F; // Typical asteroid radius: 2km
    // body->color = GRAY;
    // body->position = {r * cosf(phi), 0, r * sinf(phi)};
    // body->velocity = {-v * sinf(phi), vy, v * cosf(phi)};
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
    p->bodies = new OrbitalBody[SOLARSYSTEM_BODYNUM];
    p->bodyNumber = SOLARSYSTEM_BODYNUM;
    p->timeStep = timeStep;     //User defined.

    int i;
    for(i=0 ; i<SOLARSYSTEM_BODYNUM ; i++) {
        p->bodies[i].mass = solarSystem[i].mass;
        p->bodies[i].radius = solarSystem[i].radius;
        p->bodies[i].color = solarSystem[i].color;
        p->bodies[i].position = solarSystem[i].position;
        p->bodies[i].velocity = solarSystem[i].velocity;
    }

    return p; // Returns pointer to your new orbital sim.
}

/**
 * @brief Destroys an orbital simulation
 */
void destroyOrbitalSim(OrbitalSim *sim)
{
    // Your code goes here...
    delete[] sim->bodies; //check syntaxis

}

/**
 * @brief Simulates a timestep
 *
 * @param sim The orbital simulation
 */
void updateOrbitalSim(OrbitalSim *sim)
{
    // Your code goes here...
    int index;

    for (index=1; index<SOLARSYSTEM_BODYNUM; index++){
    
    float mass = sim->bodies[index].mass;
    float timestep =sim->timeStep;
    Vector3 force, aceleration;


    force = calcForces(sim,index);
    aceleration = Vector3Scale(force, (1/(sim->bodies[index].mass))); //Calculates aceleration vector
    //aceleration.x= (force.x/mass);
    //aceleration.y= (force.y/mass);
    //aceleration.z= (force.y/mass);

    //sim->bodies[index].velocity.x= (sim->bodies[index].velocity.x) + (aceleration.x * timestep);
   // sim->bodies[index].velocity.y= (sim->bodies[index].velocity.y) + (aceleration.y * timestep);
    //sim->bodies[index].velocity.z= (sim->bodies[index].velocity.z) + (aceleration.z * timestep);

    sim->bodies[index].velocity = Vector3Add(sim->bodies[index].velocity, Vector3Scale(aceleration,sim->timeStep) ); //Calculates and saves the new velocity

    //sim->bodies[index].position.x= (sim->bodies[index].position.x) + ((sim->bodies[index].velocity.x)*timestep);
    //sim->bodies[index].position.y= (sim->bodies[index].position.y) + ((sim->bodies[index].velocity.y)*timestep);
    //sim->bodies[index].position.z= (sim->bodies[index].position.z) + ((sim->bodies[index].velocity.z)*timestep);

    sim->bodies[index].position = Vector3Add(sim->bodies[index].position, Vector3Scale(sim->bodies[index].velocity,sim->timeStep)); //Calculates and saves the new position

    }
    // fórmulas de aceleración etc.

}

Vector3 calcForces (OrbitalSim *sim, int index){
    int i;
    float m1=sim->bodies[index].mass,m2;

    Vector3 totalForce = Vector3Zero();

    for(i=0;i<SOLARSYSTEM_BODYNUM;i++){

        if(i==index){ //skip
        }
        else{
            float force;
            float m2 =sim->bodies[i].mass;

            printf("%f %f %f index:%d\n",sim->bodies[index].position.x,sim->bodies[index].position.y, sim->bodies[index].position.z,index); //el error estaria por aca
            fflush(stdout);
            printf("%f %f %f i:%d\n",sim->bodies[i].position.x,sim->bodies[i].position.y, sim->bodies[i].position.z,index);
            fflush(stdout);
            sleep(2);
            float vectorModule = Vector3DistanceSqr(sim->bodies[index].position,sim->bodies[i].position);
            printf("vector modulo %f\n",vectorModule);
             fflush(stdout);
            if (vectorModule<0.1){ //it makes sure it's not dividing by cero
                vectorModule = 1;
            }

            force = (-(GRAVITATIONAL_CONSTANT*m1*m2)/vectorModule);

            Vector3 normal = Vector3Zero();
            normal = Vector3Subtract(sim->bodies[index].position,sim->bodies[i].position); //Create a normal vector and multiply it by the force to obtain force vector
            normal = Vector3Normalize(normal);
            normal = Vector3Scale(normal,force);

            totalForce = Vector3Add(totalForce, normal);

        }
    }
    return totalForce;
}