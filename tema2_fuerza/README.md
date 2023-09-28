## Introducción al proyecto

Box2d también es capaz de una simulación de fuerzas aplicadas hacia los cuerpos que creamos. Mientras los cuerpos sean dinámicos, seremos capaces de simular distintas magnitudes fuerzas en distintas direcciones. En este proyecto, realizaremos un lanzamiento simple de una caja, la dispararemos a un ángulo de 45 grados con una velocidad de 400 m/s.

## Explicación de código

```
#include <Box2D/Box2D.h>
#include <iostream>
```
Como es de costumbre, importamos las librerías de box2d y iostream a nuestro proyecto.

```cpp
int main() {
	b2Vec2 gravity(0.0f, -9.81f);
	b2World world(gravity);
```
Dentro de la función principal, define primeramente el vector de gravedad y se crea el mundo con `b2World` dándole el vector de gravedad como parámetro.

```cpp
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.0f);
	
	b2Body* body = world.CreateBody(&bodyDef);
```
Creamos las definiciones para el cuerpo antes de crearlo. Es dinámico, lo que quiere decir que será afectado por fuerzas, y ubicado en el origen (`0, 0`). Crear el cuerpo mismo con `world.CreateBody()` y como parámetro se le introduce la información de las definiciones.

```cpp
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	
	body->CreateFixture(&fixtureDef);
```
Le damos forma al cuerpo con `b2PolygonShape` con la función `SetAsBox()`, parámetros son `1.0f` en x, `1.0f` en y, de modo que nuestra caja queda de 1m x 1m. Agregamos propiedades físicas con `b2FixtureDef` como es la forma (tomada directamente del `b2PolygonShape`), densidad y fricción. Se asignan dichas características al cuerpo con `body->CreateFixture()`, como parámetro la información de las propiedades.

```cpp
//Setear variables para lanzamiento
	float angle = 45.0f;
	float speed = 400.0f;

//Componentes de fuerza descompuesto en x y y
	float vx = speed * std::cos(angle*3.14/180);  
	float vy = speed * std::sin(angle*3.14/180);
```
Para realizar el lanzamiento vamos a requerir parámetros, en este caso queremos un ángulo hacia el cual lanzar nuestra caja y una velocidad. `angle` determinará el ángulo en el que el lanzamiento ocurrirá y `speed` la velocidad hacia la que viajará. Como box2d no recibe input de ángulo, tendremos que dividir el vector en sus componentes x y y. `vx` y `vy` determinan estos componentes del vector de lanzamiento, utilizando la velocidad y las identidades trigonométricas para determinarse. Hacemos la conversión de grados a radianes ya que box2d recibe los ángulos en radianes.

```cpp
	body->SetLinearVelocity(b2Vec2(vx, vy));
```
Aplicar la información de los componentes del vector al cuerpo con `body->SetLinearVelocity()`, esto hará que nuestro cuerpo tenga la velocidad aplicada con los parámetros que determinamos.

```cpp
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
```
Establecer las variables para el intervalo de tiempo que avanza el mundo en cada iteración (`1/60` en este caso), para las veces que itera el solucionador de velocidad con cada iteración (`6` en este cado) y las veces que itera el solucionador de posición con cada iteración (`2` en este caso).

```cpp
	for (int i = 0; i < 60; ++i)
	{
		world.Step(timeStep, velocityIterations, positionIterations);
		std::cout << body->GetPosition().x << " " << body->GetPosition().y << std::endl;
	}
```
Finalmente simulamos el mundo como siempre: iterar el mundo 60 veces (de modo que como avanza `1/60` cada vez se simula 1 segundo en total) con `world.Step()`, poniendo como parámetro las variables que se establecieron previamente. Imprimimos los valores de posición en x y en y del cuerpo en cada iteración.

Con todo realizado, los valores saldrán algo parecidos a esto:
```
1.42 1.41
2.83 2.82
4.25 4.23
5.66 5.64
7.08 7.04
8.49 8.44
9.91 9.84
11.32 11.23
12.74 12.62
14.15 14.01
15.57 15.40
16.98 16.78
18.40 18.16
19.81 19.54
21.23 20.91
22.64 22.28
24.06 23.65
25.47 25.02
26.89 26.39
28.30 27.75
29.72 29.11
31.13 30.46
32.55 31.82
33.96 33.17
35.38 34.51
36.79 35.86
38.21 37.20
39.63 38.54
41.04 39.88
42.46 41.21
43.87 42.54
45.29 43.87
46.70 45.20
48.12 46.52
49.53 47.84
50.95 49.16
52.36 50.48
53.78 51.79
55.19 53.10
56.61 54.40
58.02 55.71
59.44 57.01
60.85 58.31
62.27 59.60
63.68 60.90
65.10 62.19
66.51 63.48
67.93 64.76
69.34 66.04
70.76 67.32
72.17 68.60
73.59 69.88
75.00 71.15
76.42 72.42
77.84 73.68
79.25 74.95
80.67 76.21
82.08 77.46
83.50 78.72
84.91 79.97
```
Podemos observar que el valor de x y de y aumentan a un ritmo similar, lo cual se debe a que disparamos la caja a un ángulo de 45 grados, lo que tiene el efecto de que ambos componentes x y y del vector de velocidad son del mismo valor. Como solo simulamos 1 segundo, no pudimos ver la caja bajar por efecto de la gravedad, pero dejarla correr más iteraciones demostraría valores parabólicos.

## Código completo

```cpp
#include <iostream>  
#include <box2d/box2d.h>  
  
int main() {  
    b2Vec2 gravity(0.0f, -9.81f);  
  
    b2World world(gravity);  
  
    b2BodyDef bodyDef;  
    bodyDef.type = b2_dynamicBody;  
    bodyDef.position.Set(0.0f, 0.0f);  
  
    b2Body* body = world.CreateBody(&bodyDef);  
  
    b2PolygonShape dynamicBox;  
    dynamicBox.SetAsBox(0.2f, 0.05f);  
  
    b2FixtureDef fixtureDef;  
    fixtureDef.shape = &dynamicBox;  
    fixtureDef.density = 1.0f;  
    fixtureDef.friction = 0.3f;  
  
    body->CreateFixture(&fixtureDef);  
  
    //Setear variables para lanzamiento  
    float angle = 45.0f;  
    float speed = 400.0f;  
  
    //Componentes de fuerza descompuesto en x y y  
    float vx = speed * std::cos(angle*3.14/180);  
    float vy = speed * std::sin(angle*3.14/180);  
  
    body->SetLinearVelocity(b2Vec2(vx, vy));  
  
    float timeStep = 1.0f/60.0f;  
  
    int32 velocityIterations = 6;  
    int32 positionIterations = 2;  
  
    for (int32 i=0; i<60; i++){  
        world.Step(timeStep, velocityIterations, positionIterations);  
        b2Vec2 position = body->GetPosition();  
        printf("%4.2f %4.2f\n", position.x, position.y);  
    }  
}
```

## Box2d

- Documentación: https://box2d.org/documentation/index.html