## Introducción del proyecto

Este proyecto es una experimentación con la librería "box2d", una librería escrita en C++ bastante popular que maneja los cálculos y lógica para la simulación de físicas, normalmente utilizada en videojuegos. Aquí se demuestran las bases de lo necesario para iniciar un proyecto básico en el que creamos un cubo y lo dejamos caer por 1 segundo a una altura de 30 metros hacia un piso.

## Explicación de código

```cpp
#include <iostream> //Incluir la librería iostream
#include <box2d/box2d.h> //Incluir la librería box2d
```
Primeramente importamos nuestras librerías, iostream, la cual generalmente incluimos de cajón, y box2d, la librería de físicas con la que estaremos trabajando.

```cpp
int main(){
    //Setear la gravedad para el mundo
    b2Vec2 gravity(0.0f, -9.81f);

    //Crear el mundo con el parametro de la gravedad
    b2World world(gravity);
```
Creamos la función de ```int main()```, la cual se ejecutará al iniciar el programa. Lo primero que se debe hacer en box2d es crear un mundo en el cual nuestra simulación ocurrirá, creando una variable de tipo ```b2World```. Esta variable requiere de un parámetro para crearse, siendo este la fuerza de gravedad; la fuerza de atracción que un cuerpo ejerce sobre otros. En la realidad, esta fuerza es ejercida por parte del planeta, pero en box2d debemos simularla a través de un vector que apunta hacia abajo. Los vectores en box2d se crean con el tipo de variable ```b2Vec2``` y, como nuestra simulación toma lugar en la tierra, le damos una dirección de `0.0f` en x y `-9.81f` en y; la `f` refiriéndose a que estos son valores de tipo flotante (contienen decimales).

El mundo es simplemente el espacio donde ocurren las simulaciones, más son los cuerpos los que son la simulación misma, por lo que ahora veremos cómo crear tanto el piso en el que caerá nuestra caja y la caja misma.

```cpp
//Definir caracteristicas para el cuerpo (piso)
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

//Creamos el cuerpo (piso)
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
```
Antes de crear un cuerpo que podamos manipular, debemos definir ciertos parámetros y características para éste; esto lo podemos hacer a través de la variable `b2BodyDef`. Primeramente queremos crear un piso sobre el cual tiraremos nuestro objeto. Le damos una posición inicial utilizando la función `position.Set()`, ingresando un valor en x y otro en y; queremos que el piso se encuentre a 10 metros debajo del origen, por lo que establecemos la posición y como `-10.0f`. Una vez tengamos las características listas,  podemos crear el cuerpo con la variable `b2Body*`, el `*` para acceder a los valores en lugar de al objeto. Con la función `world.CreateBody()` podemos agregar nuestro cuerpo al mundo y darle como parámetro las características que introdujimos anteriormente.

```cpp
//Crear la forma del cuerpo (piso)
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 1.0f);

//Dar carcterísticas al cuerpo (piso)
    groundBody->CreateFixture(&groundBox, 0.0f);
```
Una vez tengamos el cuerpo, ahora hace falta darle una forma. En este caso, queremos un cuadrado, el cual se categoriza como un polígono y por ende debemos crearlo con la variable `b2PolygonShape` y accediendo a su función `SetAsBox()`, la cual tiene como parámetros un tamaño en x y otro en y. Los volvemos `50.0f` en x y `1.0f` en y, haciendo que sea largo. Cuando tengamos la forma establecida debemos juntarla con el cuerpo que creamos anteriormente con la función `CreateFixture()`.  Aquí le damos como parámetros la información de la forma y adicionalmente nos pide una densidad, la cual por ahora será `0.0f`.

Con esto hemos completado el piso: definimos su posición inicial así como otros parámetros, lo agregamos al mundo como un cuerpo y creamos una forma para éste. Ahora crearemos la caja.

```cpp
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody; //Especificar el tipo de cuerpo que es
    bodyDef.position.Set(0.0f, 20.0f);
    b2Body* body = world.CreateBody(&bodyDef);
```
El proceso es similar al del piso, la única diferencia siendo la posición inicial, la cual es 20 metros sobre el origen, y que ahora debemos establecer un tipo de cuerpo. Box2d por default hace los cuerpos como estáticos, es decir, que no se mueven por fuerzas o interacciones, por ejemplo el piso que creamos no caerá ni se verá atraído por la gravedad. Como queremos que nuestra caja sí sea afectada por la gravedad, debemos hacer que sea un cuerpo dinámico, esto al acceder a la propiedad `type` de la definición de cuerpo e igualándola a `b2_dynamicBody`.

```cpp
    b2PolygonShape dynamicBox; //Forma que el cuerpo tomará
    dynamicBox.SetAsBox(1.0f, 1.0f); //Tamaño 

    b2FixtureDef fixtureDef; //Definicion de propiedades
    fixtureDef.shape = &dynamicBox; //Forma (tomada del b2PolygonShape)
    fixtureDef.density = 1.0f; //Densidad
    fixtureDef.friction = 0.3f; //Fricción

    body->CreateFixture(&fixtureDef);
```
Nuevamente, lo creamos como una caja, esta vez de `1.0f` en x y `1.0f` en y. Esta vez queremos añadir más propiedades a nuestra caja antes de mezclarla con el cuerpo, todo a través de la variable `b2FixtureDef`: `shape` es la forma, la cual tomamos directamente de la forma que creamos anteriormente; `density` es la densidad, lo que indirectamente afecta el peso de nuestro objeto; `friction` es la fricción, lo que afecta cómo nuestro objeto se desliza a través de las superficies.. Una vez establecidas, vamos a combinar la forma y sus propiedades al cuerpo, nuevamente con la función `CreateFixture()`.

Con ambos cuerpos creados y listos, debemos simular el tiempo, ya que es un componente importante en los cálculos de física. Para esto, debemos establecer un par de variables primero.

```cpp
    float timeStep = 1.0f/60.0f; //Tiempo que el mundo avanzará con cada iteración

    int32 velocityIterations = 6; //Iteraciones del solucionador de velocidad
    int32 positionIterations = 2; //Iteaciones del solucionador de posición
```
`timeStep` representa la cantidad de tiempo que el mundo avanzará con cada iteración, este lo volvemos `1/60` de modo que el mundo avanza una sesentava de segundo cada iteración, de modo que si hacemos 60 iteraciones, en el mundo se calculará 1 segundo de movimiento. Box2d contiene un solucionador de restricciones que se ejecuta múltiples cada iteración con el fin de evitar problemas, estos siendo uno de posición (el cual ajusta la posición de los cuerpos y evita que ocupen el mismo espacio) y uno de velocidad (el cual calcula los impulsos necesarios para que los cuerpos se muevan correctamente). En este caso, las iteraciones de posición las haremos `2` (se ejecutan 2 veces por iteración) y las de velocidad `6` (6 veces por iteración).

```cpp
    for (int32 i=0; i<60; i++){ //Ciclo for, 60 iteraciones
        world.Step(timeStep, velocityIterations, positionIterations); //Avanzar mundo
        b2Vec2 position = body->GetPosition(); //Posicion del cuerpo (caja)
        float angle = body->GetAngle(); //Angulo del cuerpo (caja)
        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle); //Imprimir info
    }
}
```

Una vez tenemos las variables, podemos utilizarlas en la función `world.Step()`. Como establecimos antes, queremos que la simulación dure 1 segundo, por lo que debemos iterar el `world.Step()` 60 veces a través de un ciclo `for`. Dentro de este ciclo, podemos obtener la información de nuestro cuerpo e imprimirla en pantalla; en este caso queremos saber la posición, obtenida con la función de nuestro cuerpo `body->GetPosition()`, y podemos guardar esto en un vector el cual podemos imprimir posteriormente (el ángulo no será usado en este ejemplo).

Si todo fue hecho correctamente, al correr la simulación nos deberían aparecer los valores de cambio de la caja en cada iteración:
![[Pasted image 20230927180254.png]]
## Código completo

```cpp
#include <iostream>
//Libreria box2d
#include <box2d/box2d.h>

int main(){
    //Setear la gravedad para el mundo
    b2Vec2 gravity(0.0f, -9.81f);

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
    fixtureDef.friction = 0.3f;

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
