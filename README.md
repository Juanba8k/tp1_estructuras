# EDA #level1: Orbital simulation

## Integrantes del grupo y contribución al trabajo de cada integrante

* [Nombre]: [contribución]

    Juan Bautista Correa Uranga: desarrollo de la función update orbital sim, mejoras en la eficiencia y portabilidad para usarse con otros sistemas solares.

    Augusto Milani: Creación de estructuras de datos y inicializacion de los mismos, display, mejoras de eficiencia en el display.


## Verificación del timestep

El timestep funciona correctamente con valores bajos, pero al simular varios años por segundo los planetas cercanos abandonan su órbita. Al elegir un timestep de 1 día por segundo, no se ve reflejado en el paso del tiempo.
Ocurre un overflow al timestep cuando seleccionamos un paso de 100 años por segundo.

## Verificación del tipo de datos float

Al trabajar con floats y considerar un time step muy alto, hay overflow, lo que hacer que el time step sea negativo. Al mismo tiempo, si bien en el calculo de la aceleración uno puede evitar la multiplicacion y division de la masa, si uno lo deja, la multiplicacion entre la masa de dos planetas trae overflow.

## Complejidad computacional con asteroides

La complejidad computacional del programa es O(N²).

## Mejora de la complejidad computacional

Despreciamos el efecto de órbita que tienen los asteroides, siendo que su masa es de 10 órdenes menor. Al acotar las comparaciones, se incrementa la eficiencia de 4fps a 32fps en nuestra computadora menos potente y no se observan cambios significativos en la simulación. En la computadora con un procesador mejor funciona a 58 fps.
Además, al alejar la cámara de manera significativa, los objetos se renderizarán como líneas y no como esferas, a fin de mejorar la velocidad de ejecución. Al hacer esto último, la simulacion corre a 60 fps en ambas compturadoras.

## Bonus points
Al multiplicar la masa de Jupiter por 1000, el sol empieza a orbitar alrededor del mismo, y al mismo tiempo todos los otros planetas salen despedidos de la orbita. Al mismo tiempo, como no hay colisiones, los planetas al juntarse, se atraviezan y salen disparados. Si un agujero negro apareciera dentro del sistema solar, todos los planetas (y el sol) abandonarían su órbita para colisionar con él. En ambos casos, la vida en la Tierra es imposible.

El easter egg encontrado se encuentra al cargar los asteroides, permitiendo simularlos sobre un mismo eje (sin desfase).

Se crearon dos constantes en orbitalSim.cpp llamadas SISTEM_BODYNUM y SISTEM_EPHEMERIDES, las cuales deben ser cambiadas y permite que el código pueda usarse para simular otros sistemas solares, con planetas diferentes. En este caso se probó también con Alfa Centauri, mostrando dos estrellas que orbitaban en una circunferencia.