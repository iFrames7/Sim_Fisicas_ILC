## Introducción al proyecto

La gravedad es una fuerza de atracción que se representa en forma de una aceleración hacia abajo y algo importante que remarcar es que este valor varía dependiendo del tamaño o la masa del cuerpo que la ejerce. Cuerpos como los humanos no ejercen mucha gravedad, más los planetas ejercen una gravedad considerablemente grande. En este ejemplo con box2d, veremos qué ocurre si creamos una caja en la luna y la dejamos caer.

## Explicación de código

```cpp
#include <iostream>
#include <box2d/box2d.h>
```
Como siempre, incluimos las librerías `iostream` y `box2d`, la última nos permitirá acceder a las funciones para la simulación.

```cpp
int main(){
    //Setear la gravedad para el mundo
    b2Vec2 gravity(0.0f, -1.62f);

    //Crear el mundo con el parametro de la gravedad
    b2World world(gravity);
```
Aquí es donde ocurrirá el mayor cambio a nuestro proyecto. Al crear el vector de gravedad, lo debemos igualar a la gravedad que ejerce la luna sobre los cuerpos cercanos, en este caso es `-1.62f`, menor a la de la tierra debido a su menor tamaño y masa. Cuando creamos el mundo con este cambio de vector de gravedad, box2d hará todos los cálculos gravitacionales con respecto a este nuevo valor.

```cpp
//Definir caracteristicas para el cuerpo (piso)
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    //Creamos el cuerpo (piso)
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    //Crear la forma del cuerpo (piso)
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 1.0f);

    //Dar carcterísticas al cuerpo (piso)
    groundBody->CreateFixture(&groundBox, 0.0f);
```
Similar a otras veces, primero creamos un cuerpo estático que servirá como nuestro suelo. Lo colocamos a 10 metros por debajo del origen y lo hacemos de 50 metros de largo y 1 de alto.

```cpp
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 20.0f);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    body->CreateFixture(&fixtureDef);
```
De igual manera, creamos nuestro cuerpo dinámico con sus características correspondientes: dinámico, 20 metros encima del origen, caja de 1m x 1m, con una densidad de 1 y sin fricción ya que estará en el aire.

```cpp
    float timeStep = 1.0f/60.0f;

    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    for (int32 i=0; i<60; i++){
        world.Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }
}
```
Finalmente simulamos el mundo con los mismos valores previamente establecidos: `timeStep` de `1/60`, 6 iteraciones del solucionador de velocidad y 2 del de posición, 60 iteraciones de `world.Step()` e imprimimos la posición en x y en y de nuestra caja (el ángulo no se usará en este ejercicio).

Al correr la simulación, los valores deberían de salir de esta manera:
```
0.00 20.00 0.00
0.00 20.00 0.00
0.00 20.00 0.00
0.00 20.00 0.00
0.00 19.99 0.00
0.00 19.99 0.00
0.00 19.99 0.00
0.00 19.98 0.00
0.00 19.98 0.00
0.00 19.98 0.00
0.00 19.97 0.00
0.00 19.96 0.00
0.00 19.96 0.00
0.00 19.95 0.00
0.00 19.95 0.00
0.00 19.94 0.00
0.00 19.93 0.00
0.00 19.92 0.00
0.00 19.91 0.00
0.00 19.91 0.00
0.00 19.90 0.00
0.00 19.89 0.00
0.00 19.88 0.00
0.00 19.86 0.00
0.00 19.85 0.00
0.00 19.84 0.00
0.00 19.83 0.00
0.00 19.82 0.00
0.00 19.80 0.00
0.00 19.79 0.00
0.00 19.78 0.00
0.00 19.76 0.00
0.00 19.75 0.00
0.00 19.73 0.00
0.00 19.72 0.00
0.00 19.70 0.00
0.00 19.68 0.00
0.00 19.67 0.00
0.00 19.65 0.00
0.00 19.63 0.00
0.00 19.61 0.00
0.00 19.59 0.00
0.00 19.57 0.00
0.00 19.55 0.00
0.00 19.53 0.00
0.00 19.51 0.00
0.00 19.49 0.00
0.00 19.47 0.00
0.00 19.45 0.00
0.00 19.43 0.00
0.00 19.40 0.00
0.00 19.38 0.00
0.00 19.36 0.00
0.00 19.33 0.00
0.00 19.31 0.00
0.00 19.28 0.00
0.00 19.26 0.00
0.00 19.23 0.00
0.00 19.20 0.00
0.00 19.18 0.00
```
Si los observamos, será obvio que el decremento en la posición y de la caja es bastante lento, moviéndose menos de 1 metro en el segundo simulado. Del mismo modo, el inicio de cambio de posición tomó más iteraciones que solo 1. Ambos son efectos de la baja gravedad que ejerce la luna en los cuerpos que atrae.

## Código completo

```cpp
#include <iostream>
//Libreria box2d
#include <box2d/box2d.h>

int main(){
    //Setear la gravedad para el mundo
    b2Vec2 gravity(0.0f, -1.62f);

    //Crear el mundo con el parametro de la gravedad
    b2World world(gravity);

    //-------------------------------------------

    //Definir caracteristicas para el cuerpo (piso)
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    //Creamos el cuerpo (piso)
    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    //Crear la forma del cuerpo (piso)
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 1.0f);

    //Dar carcterísticas al cuerpo (piso)
    groundBody->CreateFixture(&groundBox, 0.0f);

    //-------------------------------------------

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 20.0f);
    b2Body* body = world.CreateBody(&bodyDef);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    body->CreateFixture(&fixtureDef);

    float timeStep = 1.0f/60.0f;

    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    for (int32 i=0; i<60; i++){
        world.Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();
        float angle = body->GetAngle();
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
    }
}
```

## Box2d

- Documentación: https://box2d.org/documentation/index.html