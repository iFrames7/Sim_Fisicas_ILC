## Introducción del proyecto

En este proyecto nuevamente cambiaremos la gravedad que usará box2d para los cálculos, mas ahora comenzaremos a tomar en cuenta las propiedades del material que vamos a tirar. En este caso, queremos tirar una caja hecha de carburo de tungsteno, la cual tiene una densidad relativamente alta, que se encuentra en la atmósfera de Júpiter, el cuál tiene una fuerza gravitacional mucho mayor a la de la tierra, desde 15 metros de altura. De este modo, los cálculos y resultados cambiarán.

## Explicación de código

```cpp
#include <iostream>  
#include <box2d/box2d.h>  
```
Como es normal en estos proyectos, importamos la librería iostream y box2d.

```cpp
int main(){  
    b2Vec2 gravity(0.0f, -24.79f);  
  
    b2World world(gravity);  
```
Creamos la función `main()` que se ejecutará al inicio y comenzamos a darle nuestros parámetros para la creación del mundo. Ahora, como queremos la fuerza gravitacional de Júpiter, igualamos el vector de gravedad a `-24.79f`.

```cpp
    b2BodyDef groundBodyDef;  
    groundBodyDef.position.Set(0.0f, -0.5f);  
  
    b2Body* groundBody = world.CreateBody(&groundBodyDef);  
  
    b2PolygonShape groundBox;  
    groundBox.SetAsBox(50.0f, 1.0f);  
  
    groundBody->CreateFixture(&groundBox, 0.0f);
```
Creamos el suelo sobre el cual nuestra caja va a caer. Debe ser un cuerpo estático y se encuentra a -0.5m, esto debido a que como de altura es de 1m, una mitad se encuentra por encima y la otra por debajo, por lo que colocarlo a esta altura garantiza que el suelo se encuentre en el origen.

```cpp
    b2BodyDef bodyDef;  
    bodyDef.type = b2_dynamicBody;  
    bodyDef.position.Set(0.0f, 15.0f);  
    b2Body* body = world.CreateBody(&bodyDef);  
  
    b2PolygonShape dynamicBox;  
    dynamicBox.SetAsBox(1.0f, 1.0f);  
```
Ahora creamos nuestra caja que tiraremos. La colocamos a 15m sobre el origen, la hacemos de 1m x 1m y la hacemos dinámica para que la gravedad la pueda afectar.

```cpp
    b2FixtureDef fixtureDef;  
    fixtureDef.shape = &dynamicBox;  
    fixtureDef.density = 15600.0f;  
    fixtureDef.friction = 1.0f;  
  
    body->CreateFixture(&fixtureDef);
```
En las propiedades del cuerpo, le vamos a dar una densidad de `15600.0f`, ya que el carburo de tungsteno posee esta densidad. Posteriormente, una fricción de `1.0f`, ya que ésta no afectará la simulación en esta ocasión.

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
Finalmente simulamos el mundo como lo hemos hecho previamente.

Con todos los cálculos en su lugar, deberían de salir valores similares a estos:
```
0.00 14.99 0.00
0.00 14.98 0.00
0.00 14.96 0.00
0.00 14.93 0.00
0.00 14.90 0.00
0.00 14.86 0.00
0.00 14.81 0.00
0.00 14.75 0.00
0.00 14.69 0.00
0.00 14.62 0.00
0.00 14.55 0.00
0.00 14.46 0.00
0.00 14.37 0.00
0.00 14.28 0.00
0.00 14.17 0.00
0.00 14.06 0.00
0.00 13.95 0.00
0.00 13.82 0.00
0.00 13.69 0.00
0.00 13.55 0.00
0.00 13.41 0.00
0.00 13.26 0.00
0.00 13.10 0.00
0.00 12.93 0.00
0.00 12.76 0.00
0.00 12.58 0.00
0.00 12.40 0.00
0.00 12.20 0.00
0.00 12.00 0.00
0.00 11.80 0.00
0.00 11.58 0.00
0.00 11.36 0.00
0.00 11.14 0.00
0.00 10.90 0.00
0.00 10.66 0.00
0.00 10.41 0.00
0.00 10.16 0.00
0.00 9.90 0.00
0.00 9.63 0.00
0.00 9.35 0.00
0.00 9.07 0.00
0.00 8.78 0.00
0.00 8.49 0.00
0.00 8.18 0.00
0.00 7.87 0.00
0.00 7.56 0.00
0.00 7.23 0.00
0.00 6.90 0.00
0.00 6.56 0.00
0.00 6.22 0.00
0.00 5.87 0.00
0.00 5.51 0.00
0.00 5.15 0.00
0.00 4.77 0.00
0.00 4.40 0.00
0.00 4.01 0.00
0.00 3.62 0.00
0.00 3.22 0.00
0.00 2.81 0.00
0.00 2.40 0.00
```
Aquí la rapidez con la que la posición en `y` de la caja decrementa es mucho mayor a los ejemplos anteriores, ya que la caja es más densa y la gravedad del planeta es mucho mayor.

## Código completo

```cpp
#include <iostream>  
//Libreria box2d  
#include <box2d/box2d.h>  
  
int main(){  
    b2Vec2 gravity(0.0f, -24.79f);  
  
    b2World world(gravity);  
  
    //-------------------------------------------  
  
    b2BodyDef groundBodyDef;  
    groundBodyDef.position.Set(0.0f, -0.5f);  
  
    b2Body* groundBody = world.CreateBody(&groundBodyDef);  
  
    b2PolygonShape groundBox;  
    groundBox.SetAsBox(50.0f, 1.0f);  
  
    groundBody->CreateFixture(&groundBox, 0.0f);  
  
    //-------------------------------------------  
  
    b2BodyDef bodyDef;  
    bodyDef.type = b2_dynamicBody;  
    bodyDef.position.Set(0.0f, 15.0f);  
    b2Body* body = world.CreateBody(&bodyDef);  
  
    b2PolygonShape dynamicBox;  
    dynamicBox.SetAsBox(1.0f, 1.0f);  
  
    b2FixtureDef fixtureDef;  
    fixtureDef.shape = &dynamicBox;  
    fixtureDef.density = 15600.0f;  
    fixtureDef.friction = 1.0f;  
  
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