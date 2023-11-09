# Proyecto parcial 2

## Introducción

En este proyecto se nos pidió hacer una simulación combinando las funciones de box2d para la lógica y la librería SFML para tener una interfaz gráfica donde se pueda observar la simulación con el objetivo de crear una máquina de Rude Goldberg.

Una máquina de Rude Goldberg es cualquier aparato o máquina innecesariamente sofisticada que realiza una tarea muy simple haciendo uso de una reacción en cadena, es decir, una reacción secuencial en la que un objeto afecta al siguiente para que éste afecte al siguiente y así sucesivamente. [1]

A través del uso de cuerpos dinámicos y joints de la librería de box2d, podemos replicar este comportamiento complejo, ya que podemos recrear objetos rotatorios, poleas, movimiento en una dimensión y, por supuesto, interacciones simples de cuerpos dinámicos como efectos domino, rampas, etc. [2] 

## Explicación de código

```cpp
#include <iostream>  
#include <box2d/box2d.h>  
#include <SFML/Graphics.hpp>  
```

Primeramente, como es de costumbre, incluimos las librerías `iostream`, `box2d.h` y `SFML/Graphics.hpp`. Esto nos permitirá acceder a las funciones, variables y clases de box2d y SFML para realizar el proyecto

```cpp
//Constants for Pixels to meters and meters to pixels conversion  
const float M2P = 10.0f;  
const float P2M = 1.0f / M2P; 
```

Antes de la función principal, creamos estas constantes `M2P` y `P2M`. Box2d y SFML utilizan medidas distintas que no se traducen exactamente la una con la otra; estas constantes nos van a permitir convertir de metros a pixeles y de pixeles a metros cuando necesitemos insertar datos en box2d o SFML.

Ahora, para crear cuerpos en box2d y gráficos en SFML requerimos de ciertas líneas de código que en ocasiones resultan algo repetitivas. Por esto mismo, podemos ahorrarnos el espacio y crear clases para generar los cuerpos dinámicos y estáticos que requerimos de manera modular.

```cpp
//Classes for creating static and dynamic boxes  
class DBox {  
public:  
    DBox(b2World &world, float x, float y, float _shapeX, float _shapeY, float _angle, float _density, sf::Color _color){  
        b2BodyDef bodydef;  
        bodydef.position.Set(x * P2M, y * P2M);  
        bodydef.angle = _angle * b2_pi/180.0f;  
        bodydef.type = b2_dynamicBody;  
        body = world.CreateBody(&bodydef);  
  
        b2PolygonShape shape;  
        shape.SetAsBox(_shapeX/2, _shapeY/2);  
  
        b2FixtureDef fixturedef;  
        fixturedef.shape = &shape;  
        fixturedef.density = _density;  
        body->CreateFixture(&fixturedef);  
  
        box.setSize(sf::Vector2f(_shapeX * M2P, _shapeY * M2P));  
        box.setFillColor(_color);  
        box.setOrigin(_shapeX * M2P/2, _shapeY * M2P/2);  
    }  
  
    void update() {  
        box.setPosition(M2P * body->GetPosition().x, (900 - body->GetPosition().y * M2P));  
        box.setRotation(body->GetAngle()* -180/b2_pi);  
    }  
  
    void draw(sf::RenderWindow &win) const {  
        win.draw(box);  
    }  
  
    b2Body *getBody() {  
        return body;  
    }  
  
private:  
    sf::RectangleShape box;  
    b2Body *body;  
};  
```

`DBox` será nuestra clase de cuerpos dinámicos. El constructor de la clase es una forma de inicializar un objeto, y en este caso podemos darle parámetros que servirán como variables para la creación de los gráficos. Primero requiere de un `world` para generar el cuerpo, después variables como su posición en `x`, en `y`, su tamaño en `x` y en `y`, su ángulo en caso de que queramos rotarlo, su densidad y finalmente un color, el cual usaremos para diferenciar ciertos tipos de cuerpos y sus funciones en nuestra simulación.

En esta clase podemos añadir funciones para poder ejecutar cierta funcionalidad de nuestro cuerpo o acceder a información específica del mismo. Debido a que nuestra simulación va a ser un loop infinito hasta cerrarse, box2d se encarga de la simulación, mas SFML debe actualizarse continuamente, que es lo que hacen las funciones `void update()` y `void draw()`; la primera toma como referencia el cuerpo creado y actualiza la posición y rotación correspondientemente, la segunda toma la ventana generada en SFML y dibuja el gráfico que actualizamos previamente. `b2Body *getBody()` es una función para obtener la información del cuerpo creado en caso de requerirse para crear un joint o por alguna otra razón.

Tanto las funciones como el constructor fueron hechos de tipo público, de modo que se pueden acceder externamente. En las variables de tipo privado colocamos el `b2Body` y `sf::RectangleShape` ya que no es beneficioso editar o acceder a estos datos de manera externa.

```cpp
class SBox {  
public:  
    SBox(b2World &world, float x, float y, float _shapeX, float _shapeY, float _angle, float _density){  
        //Definir caracteristicas para el cuerpo (piso)  
        b2BodyDef groundBodyDef;  
        groundBodyDef.position.Set(x * P2M, y * P2M);  
        groundBodyDef.angle = _angle * b2_pi/180.0f;  
  
        //Creamos el cuerpo (piso)  
        groundBody = world.CreateBody(&groundBodyDef);  
  
        //Crear la forma del cuerpo (piso)  
        b2PolygonShape groundBox;  
        groundBox.SetAsBox(_shapeX/2, _shapeY/2);  
  
        //Dar carcterísticas al cuerpo (piso)  
        groundBody->CreateFixture(&groundBox, _density);  
  
        box.setSize(sf::Vector2f(_shapeX * M2P, _shapeY * M2P));  
        box.setFillColor(sf::Color::Red);  
        box.setOrigin(_shapeX * M2P/2, _shapeY * M2P/2);  
    }  
  
    void update() {  
        box.setPosition(M2P * groundBody->GetPosition().x, (900 - groundBody->GetPosition().y * M2P));  
        box.setRotation(groundBody->GetAngle() * -180/b2_pi);  
    }  
  
    void draw(sf::RenderWindow &win) const {  
        win.draw(box);  
    }  
  
    b2Body *getBody() {  
        return groundBody;  
    }  
  
private:  
    sf::RectangleShape box;  
    b2Body *groundBody;  
}; 
```

`SBox` será nuestra clase de cuerpos estáticos. Como vemos es muy similar a la clase de cuerpos dinámicos, la diferencia siendo que los cuerpos creados serán estáticos y por ende el proceso de creación con box2d es un poco más corto.

Una vez tenemos nuestras clases, podemos comenzar con la creación de la simulación en la función principal.

```cpp
int main() {  
  
    //--------------Initial-world-and-window-setup--------------  
  
    sf::RenderWindow window(sf::VideoMode(900, 900), "Proyecto Parcial 2 IvanL");  
    b2Vec2 gravity(0.0f, -0.1f);  
    b2World world(gravity);  
```

Este es el setup inicial tanto para box2d como para SFML. `sf::RenderWindow` es la ventana en la que veremos la simulación, en este caso le damos una dimensión de 900px X 900px; esto debemos tomarlo en cuenta cuando introduzcamos cosas como posición o tamaño de nuestros cuerpos. `b2Vec2 gravity`, como es de costumbre, es el vector que indicará la fuerza de gravedad en el mundo, la cual introduciremos a `b2World world()` como parámetro para crear el mundo que será simulado.

```cpp
//--------------Static-and-Dynamic-Bodies--------------  
  
    SBox sBody1(world, 20.0f, 850.0f, 10.0f, 1.0f, -30.0f, 1.0f);  
    SBox sBody2(world, 165.0f, 700.0f, 8.0f, 1.0f, 0.0f, 1.0f);  
    SBox sBody3(world, 473.0f, 800.0f, 10.0f, 1.0f, 0.0f, 1.0f);  
    SBox sBody4(world, 600.0f, 730.0f, 15.0f, 1.0f, -20.0f, 1.0f);  
    SBox sBody5(world, 664.0f, 440.0f, 5.0f, 1.0f, 0.0f, 1.0f);  
    SBox sBody6(world, 700.0f, 380.0f, 11.0f, 1.0f, 30.0f, 1.0f);  
    DBox dBody1(world, 20.0f, 900.0f, 1.0f, 1.0f, 0.0f, 2.0f, sf::Color::Blue);  
    DBox dBody2(world, 135.0f, 745.0f, 1.0f, 8.0f, 0.0f, 1.0f, sf::Color::Blue);  
    DBox dBody3(world, 180.0f, 745.0f, 1.0f, 8.0f, 0.0f, 1.0f, sf::Color::Blue);  
    DBox dBody4(world, 424.0f, 830.0f, 1.0f, 6.0f, 0.0f, 0.5f, sf::Color::Blue);  
    DBox dBody5(world, 470.0f, 830.0f, 1.0f, 6.0f, 0.0f, 0.5f, sf::Color::Blue);  
    DBox dBody6(world, 674.0f, 469.0f, 1.0f, 4.0f, 0.0f, 1.0f, sf::Color::Blue);  
```

Estos cuerpos estáticos y dinámicos son aquellos que usaremos fuera de joints, es decir, servirán como suelos, rampas, o piezas dominó que se tumbarán unas a las otras con la intención de continuar con la simulación. A estos cuerpos dinámicos les daremos un color azul. Como vemos, estamos utilizando las clases que creamos anteriormente: `DBox` para cuerpos dinámicos y `SBox` para cuerpos estáticos. Aquí pasamos los atributos que serán introducidos en el constructor de cada clase: el mundo, la posición en `x` y en `y`, las dimensiones, su ángulo y su densidad. Es importante aquí notar que algunos cuerpos dinámicos tienen distinta densidad, esto es porque en algunas instancias requerimos que un cuerpo sea más pesado que otro para poder moverlo.

```cpp
    //----------------Pulley-Joint----------------------------  
  
    DBox pulleyBody1(world, 265.0f, 500.0f, 8.0f, 1.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    DBox pulleyBody2(world, 380.0f, 400.0f, 8.0f, 1.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    SBox pulleyAnchor1(world, 265.0f, 900.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
    SBox pulleyAnchor2(world, 380.0f, 900.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
  
    b2PulleyJointDef pulleyJointDef;  
    pulleyJointDef.Initialize(pulleyBody1.getBody(), pulleyBody2.getBody(), pulleyAnchor1.getBody()->GetPosition(), pulleyAnchor2.getBody()->GetPosition(), pulleyBody1.getBody()->GetWorldCenter(), pulleyBody2.getBody()->GetWorldCenter(), 1.0f);  
  
    b2PulleyJoint *pulleyJoint;  
    pulleyJoint = (b2PulleyJoint*)world.CreateJoint(&pulleyJointDef); 
```

Aquí utilizaremos un pulley joint, en el cual caerá un objeto, haciendo que un lado baje y otro suba, activando la siguiente secuencia de la máquina. Es importante remarcar aquí que too cuerpo dinámico que forma parte de un joint le daremos un color amarillo. 

Para crear el pulley joint necesitamos de 2 cuerpos dinámicos y 2 anclas, las cuales pueden ser cuerpos estáticos. En la función `pulleyJointDef.Initialize()` requerimos como parámetro los 2 cuerpos dinámicos que se afectarán el uno al otro, las anclas en las cuales "colgarán" los cuerpos, un punto de anclaje cada cuerpo para el cual utilizaremos el centro del cuerpo y finalmente un ratio para el pulley que representa el multiplicador de fuerza que será transmitida del primer lado al segundo.

Una vez inicializado, lo podemos establecer con `world.CreateJoint()`.

```cpp
//---------------------Distance-Joint-----------------------  
  
    DBox distanceBody1(world, 690.0f, 700.0f, 1.0f, 5.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    SBox distanceBody2(world, 690.0f, 640.0f, 1.0f, 1.0f, 0.0f, 1.0f);  
  
    b2DistanceJointDef distanceJointDef;  
    distanceJointDef.Initialize(distanceBody1.getBody(), distanceBody2.getBody(), distanceBody1.getBody()->GetWorldCenter(), distanceBody2.getBody()->GetWorldCenter());  
  
    b2DistanceJoint *distanceJoint;  
    distanceJoint = (b2DistanceJoint*)world.CreateJoint(&distanceJointDef);
```

Aquí crearemos el distance joint, el cual provoca que 2 cuerpos mantengan una misma distancia entre sí, en este ejemplo tumbaremos el cuerpo dinámico de balance, causando que rote alrededor del cuerpo de ancla que se encuentra debajo del mismo, actuando como una especie de martillo.

Para crear el distance joint solo hace falta un 2 cuerpos, en este caso utilizaremos un cuerpo dinámico y otro estático, los colocaremos uno encima del otro para poder crear este efecto de martillo que mencionamos. En la función `distanceJointDef.Initialize()` le damos como parámetro los 2 cuerpos que se unirán y un punto en el cuerpo del cual se unirán, en este caso usamos el centro de ambos cuerpos.

Nuevamente, `world.CreateJoint()` establece el joint ante el mundo.

```cpp
//----------------------Prismatic-and-Gear-Joint----------------------  
  
    DBox prisBody1(world, 690.0f, 585.0f, 4.0f, 1.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    SBox prisAnchor1(world, 500.0f, 585.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
    DBox prisBody2(world, 500.0f, 485.0f, 4.0f, 1.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    SBox prisAnchor2(world, 690.0f, 485.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
  
    b2PrismaticJointDef prismaticJointDef;  
    prismaticJointDef.Initialize(prisAnchor1.getBody(), prisBody1.getBody(), prisBody1.getBody()->GetWorldCenter(), b2Vec2(1.0f, 0.0f));  
    prismaticJointDef.lowerTranslation = -19.0f;  
    prismaticJointDef.enableLimit = true;  
  
    b2PrismaticJoint *prismaticJoint;  
    prismaticJoint = (b2PrismaticJoint*)world.CreateJoint(&prismaticJointDef);  
  
    b2PrismaticJointDef prismaticJointDef2;  
    prismaticJointDef2.Initialize(prisAnchor2.getBody(), prisBody2.getBody(), prisBody2.getBody()->GetWorldCenter(), b2Vec2(1.0f, 0.0f));  
    prismaticJointDef2.upperTranslation = 19.0f;  
    prismaticJointDef2.enableLimit = true;  
  
    b2PrismaticJoint *prismaticJoint2;  
    prismaticJoint2 = (b2PrismaticJoint*)world.CreateJoint(&prismaticJointDef2);
```

Aquí vamos a crear un prismatic joint y unir 2 de ellos con un gear joint, pero primero hay que crear los prismatics. Un prismatic joint es una forma en la que le podemos dar movimiento unidimensional a un cuerpo. Para nuestro ejemplo el martillo que creamos anteriormente va a golpear este prismatic y a moverlo de modo que empuja a otro componente posteriormente.

Para crear un prismatic solo se requiere de un cuerpo dinámico y un punto de anclaje, el cual puede ser un cuerpo estático, un punto de anclaje en el cuerpo dinámico y un axis, el cual podemos establecer con un `b2Vec2`. Una vez creada la definición del joint, podemos editar un par de propiedades, como es el que tenga un límite con `enableLimit` y establecer estos límites con `upperTranslation` y `lowerTranslation`. Aquí estamos creando dos que se encuentran en fines opuestos, ya que el movimiento de uno va a afectar el movimiento del otro a través de un gear joint, el cual crearemos posteriormente.

```cpp
    b2GearJointDef gearJointDef;  
    gearJointDef.bodyA = prisBody1.getBody();  
    gearJointDef.bodyB = prisBody2.getBody();  
    gearJointDef.joint1 = prismaticJoint;  
    gearJointDef.joint2 = prismaticJoint2;  
    gearJointDef.ratio = 1.0f;  
  
    b2GearJoint *gearJoint;  
    gearJoint = (b2GearJoint*)world.CreateJoint(&gearJointDef);
```

Aquí creamos el gear joint para unir los 2 prismatics que acabamos de crear. Un gear joint puede unir distintos joints y transferir fuerza como si hubiera un engranaje entre los dos cuerpos. En nuestro ejemplo, queremos que los 2 prismatics vayan en direcciones opuestas de modo que la fuerza de uno afecte al otro.

Para crear el gear joint requerimos de 2 cuerpos que formen parte de un joint distinto, especificar estos 2 joints, y darles un ratio para la fuerza similar al pulley joint.

```cpp
    //--------------------Revolute-Joint------------------------  
  
    DBox revolBody1(world, 550.0f, 300.0f, 7.0f, 1.0f, 0.0f, 1.0f, sf::Color::Green);  
    SBox revolAnchor1(world, 550.0f, 300.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
  
    b2RevoluteJointDef revoluteJointDef;  
    revoluteJointDef.Initialize(revolAnchor1.getBody(), revolBody1.getBody(), revolAnchor1.getBody()->GetPosition());  
    revoluteJointDef.enableLimit = false;  
  
    b2RevoluteJoint *revoluteJoint;  
    revoluteJoint = (b2RevoluteJoint*)world.CreateJoint(&revoluteJointDef);  
```

Aquí creamos el revolute joint, el cual permite que un cuerpo rote alrededor de cierto punto. En nuestro ejemplo, este es el resultado final de la máquina; el prismatic empuja un cuerpo que cae en una rampa y golpea este joint, haciéndolo girar.

Para crear el joint requerimos de 2 cuerpos, uno será el que rote y otro funcionará como ancla, en nuestro caso utilizamos un cuerpo estático como ancla y uno dinámico como el que rotará. Una vez establecido el joint podemos cambiar ciertas propiedades, por ejemplo podemos limitar el ángulo de rotación del joint, agregar un motor, etc.; en este caso solo nos interesa remover el límite ya que queremos que el cuerpo rote indefinidamente.

Con este joint hecho, tenemos todo en su lugar para comenzar el loop de dibujo de SFML y poder ejecutar la simulación.

```cpp
    //------------------SFML-loop--------------------------  
  
    while (window.isOpen()) {  
        sf::Event event{};  
        while (window.pollEvent(event)) {  
            if (event.type == sf::Event::Closed)  
                window.close();  
        }  
```

`while(window.isOpen())` es el loop principal, el cual establece que la función se ejecutará mientras la ventana esté abierta y si detecta que hay un evento de cierre, se cerrará.

```cpp
//Update positions + rotation  
        sBody1.update();  
        sBody2.update();  
        sBody3.update();  
        sBody4.update();  
        sBody5.update();  
        sBody6.update();  
        dBody1.update();  
        dBody2.update();  
        dBody3.update();  
        dBody4.update();  
        dBody5.update();  
        dBody6.update();  
        pulleyBody1.update();  
        pulleyBody2.update();  
        distanceBody1.update();  
        distanceBody2.update();  
        prisBody1.update();  
        prisBody2.update();  
        revolBody1.update();
```

Si recordamos, en nuestras clases tenemos la función de `update()` para nuestros cuerpos, la cual actualiza la posición e los gráficos de SFML para que sean igual que los cuerpos de la simulación. Es importante realizar esta función antes de la de `draw()` ya que primero queremos saber dónde está el objeto antes de dibujarlo.

```cpp
        // Step the Box2D simulation  
        world.Step(1.0f / 60.0f, 6, 2);  
  
        // Clear the window  
        window.clear(sf::Color::Black);  
```

`world.Step()` es la función que avanza el tiempo en nuestra simulación, tomando como parámetro una medida de tiempo para avanzar cada loop, las iteraciones en el módulo de velocidad y las iteraciones en el módulo de posición. `window.clear()` es la función que estará limpiando la pantalla cada loop, si pensamos en cada iteración de tiempo que avanza como una pintura, cada que haya una iteración de loop se deja una pintura en pantalla y si no la limpiamos manchará la siguiente pintura. Es una forma de que la simulación se vea fluida.

```cpp
        // Draw  
        sBody1.draw(window);  
        sBody2.draw(window);  
        sBody3.draw(window);  
        sBody4.draw(window);  
        sBody5.draw(window);  
        sBody6.draw(window);  
        dBody1.draw(window);  
        dBody2.draw(window);  
        dBody3.draw(window);  
        dBody4.draw(window);  
        dBody5.draw(window);  
        dBody6.draw(window);  
        pulleyBody1.draw(window);  
        pulleyBody2.draw(window);  
        distanceBody1.draw(window);  
        distanceBody2.draw(window);  
        prisBody1.draw(window);  
        prisBody2.draw(window);  
        revolBody1.draw(window);  
  
        // Display the frame  
        window.display();  
    }
    return 0;  
}
```

Así como con cada cuerpo lo actualizamos, cada cuerpo lo dibujamos en la ventana que creamos. Finalmente, con `window.display()` mostramos a la ventana lo que hemos dibujado. Esto se loopeará hasta cerrarse la ventana, en cuyo caso terminamos el programa con `return 0;`, indicando el final de la función principal.

## Código completo

```cpp
#include <iostream>  
#include <box2d/box2d.h>  
#include <SFML/Graphics.hpp>  
  
//Constants for Pixels to meters and meters to pixels conversion  
const float M2P = 10.0f;  
const float P2M = 1.0f / M2P;  
  
//Classes for creating static and dynamic boxes  
class DBox {  
public:  
    DBox(b2World &world, float x, float y, float _shapeX, float _shapeY, float _angle, float _density, sf::Color _color){  
        b2BodyDef bodydef;  
        bodydef.position.Set(x * P2M, y * P2M);  
        bodydef.angle = _angle * b2_pi/180.0f;  
        bodydef.type = b2_dynamicBody;  
        body = world.CreateBody(&bodydef);  
  
        b2PolygonShape shape;  
        shape.SetAsBox(_shapeX/2, _shapeY/2);  
  
        b2FixtureDef fixturedef;  
        fixturedef.shape = &shape;  
        fixturedef.density = _density;  
        body->CreateFixture(&fixturedef);  
  
        box.setSize(sf::Vector2f(_shapeX * M2P, _shapeY * M2P));  
        box.setFillColor(_color);  
        box.setOrigin(_shapeX * M2P/2, _shapeY * M2P/2);  
    }  
  
    void update() {  
        box.setPosition(M2P * body->GetPosition().x, (900 - body->GetPosition().y * M2P));  
        box.setRotation(body->GetAngle()* -180/b2_pi);  
    }  
  
    void draw(sf::RenderWindow &win) const {  
        win.draw(box);  
    }  
  
    b2Body *getBody() {  
        return body;  
    }  
  
private:  
    sf::RectangleShape box;  
    b2Body *body;  
};  
  
class SBox {  
public:  
    SBox(b2World &world, float x, float y, float _shapeX, float _shapeY, float _angle, float _density){  
        //Definir caracteristicas para el cuerpo (piso)  
        b2BodyDef groundBodyDef;  
        groundBodyDef.position.Set(x * P2M, y * P2M);  
        groundBodyDef.angle = _angle * b2_pi/180.0f;  
  
        //Creamos el cuerpo (piso)  
        groundBody = world.CreateBody(&groundBodyDef);  
  
        //Crear la forma del cuerpo (piso)  
        b2PolygonShape groundBox;  
        groundBox.SetAsBox(_shapeX/2, _shapeY/2);  
  
        //Dar carcterísticas al cuerpo (piso)  
        groundBody->CreateFixture(&groundBox, _density);  
  
        box.setSize(sf::Vector2f(_shapeX * M2P, _shapeY * M2P));  
        box.setFillColor(sf::Color::Red);  
        box.setOrigin(_shapeX * M2P/2, _shapeY * M2P/2);  
    }  
  
    void update() {  
        box.setPosition(M2P * groundBody->GetPosition().x, (900 - groundBody->GetPosition().y * M2P));  
        box.setRotation(groundBody->GetAngle() * -180/b2_pi);  
    }  
  
    void draw(sf::RenderWindow &win) const {  
        win.draw(box);  
    }  
  
    b2Body *getBody() {  
        return groundBody;  
    }  
  
private:  
    sf::RectangleShape box;  
    b2Body *groundBody;  
};  
  
int main() {  
  
    //--------------Initial-world-and-window-setup--------------  
  
    sf::RenderWindow window(sf::VideoMode(900, 900), "Proyecto Parcial 2 IvanL");  
    b2Vec2 gravity(0.0f, -0.1f);  
    b2World world(gravity);  
  
    //--------------Static-and-Dynamic-Bodies--------------  
  
    SBox sBody1(world, 20.0f, 850.0f, 10.0f, 1.0f, -30.0f, 1.0f);  
    SBox sBody2(world, 165.0f, 700.0f, 8.0f, 1.0f, 0.0f, 1.0f);  
    SBox sBody3(world, 473.0f, 800.0f, 10.0f, 1.0f, 0.0f, 1.0f);  
    SBox sBody4(world, 600.0f, 730.0f, 15.0f, 1.0f, -20.0f, 1.0f);  
    SBox sBody5(world, 664.0f, 440.0f, 5.0f, 1.0f, 0.0f, 1.0f);  
    SBox sBody6(world, 700.0f, 380.0f, 11.0f, 1.0f, 30.0f, 1.0f);  
    DBox dBody1(world, 20.0f, 900.0f, 1.0f, 1.0f, 0.0f, 2.0f, sf::Color::Blue);  
    DBox dBody2(world, 135.0f, 745.0f, 1.0f, 8.0f, 0.0f, 1.0f, sf::Color::Blue);  
    DBox dBody3(world, 180.0f, 745.0f, 1.0f, 8.0f, 0.0f, 1.0f, sf::Color::Blue);  
    DBox dBody4(world, 424.0f, 830.0f, 1.0f, 6.0f, 0.0f, 0.5f, sf::Color::Blue);  
    DBox dBody5(world, 470.0f, 830.0f, 1.0f, 6.0f, 0.0f, 0.5f, sf::Color::Blue);  
    DBox dBody6(world, 674.0f, 469.0f, 1.0f, 4.0f, 0.0f, 1.0f, sf::Color::Blue);  
  
    //----------------Pulley-Joint----------------------------  
  
    DBox pulleyBody1(world, 265.0f, 500.0f, 8.0f, 1.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    DBox pulleyBody2(world, 380.0f, 400.0f, 8.0f, 1.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    SBox pulleyAnchor1(world, 265.0f, 900.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
    SBox pulleyAnchor2(world, 380.0f, 900.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
  
    b2PulleyJointDef pulleyJointDef;  
    pulleyJointDef.Initialize(pulleyBody1.getBody(), pulleyBody2.getBody(), pulleyAnchor1.getBody()->GetPosition(), pulleyAnchor2.getBody()->GetPosition(), pulleyBody1.getBody()->GetWorldCenter(), pulleyBody2.getBody()->GetWorldCenter(), 1.0f);  
  
    b2PulleyJoint *pulleyJoint;  
    pulleyJoint = (b2PulleyJoint*)world.CreateJoint(&pulleyJointDef);  
  
    //---------------------Distance-Joint-----------------------  
  
    DBox distanceBody1(world, 690.0f, 700.0f, 1.0f, 5.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    SBox distanceBody2(world, 690.0f, 640.0f, 1.0f, 1.0f, 0.0f, 1.0f);  
  
    b2DistanceJointDef distanceJointDef;  
    distanceJointDef.Initialize(distanceBody1.getBody(), distanceBody2.getBody(), distanceBody1.getBody()->GetWorldCenter(), distanceBody2.getBody()->GetWorldCenter());  
  
    b2DistanceJoint *distanceJoint;  
    distanceJoint = (b2DistanceJoint*)world.CreateJoint(&distanceJointDef);  
  
    //----------------------Prismatic-and-Gear-Joint----------------------  
  
    DBox prisBody1(world, 690.0f, 585.0f, 4.0f, 1.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    SBox prisAnchor1(world, 500.0f, 585.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
    DBox prisBody2(world, 500.0f, 485.0f, 4.0f, 1.0f, 0.0f, 1.0f, sf::Color::Yellow);  
    SBox prisAnchor2(world, 690.0f, 485.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
  
    b2PrismaticJointDef prismaticJointDef;  
    prismaticJointDef.Initialize(prisAnchor1.getBody(), prisBody1.getBody(), prisBody1.getBody()->GetWorldCenter(), b2Vec2(1.0f, 0.0f));  
    prismaticJointDef.lowerTranslation = -19.0f;  
    prismaticJointDef.enableLimit = true;  
  
    b2PrismaticJoint *prismaticJoint;  
    prismaticJoint = (b2PrismaticJoint*)world.CreateJoint(&prismaticJointDef);  
  
    b2PrismaticJointDef prismaticJointDef2;  
    prismaticJointDef2.Initialize(prisAnchor2.getBody(), prisBody2.getBody(), prisBody2.getBody()->GetWorldCenter(), b2Vec2(1.0f, 0.0f));  
    prismaticJointDef2.upperTranslation = 19.0f;  
    prismaticJointDef2.enableLimit = true;  
  
    b2PrismaticJoint *prismaticJoint2;  
    prismaticJoint2 = (b2PrismaticJoint*)world.CreateJoint(&prismaticJointDef2);  
  
    b2GearJointDef gearJointDef;  
    gearJointDef.bodyA = prisBody1.getBody();  
    gearJointDef.bodyB = prisBody2.getBody();  
    gearJointDef.joint1 = prismaticJoint;  
    gearJointDef.joint2 = prismaticJoint2;  
    gearJointDef.ratio = 1.0f;  
  
    b2GearJoint *gearJoint;  
    gearJoint = (b2GearJoint*)world.CreateJoint(&gearJointDef);  
  
    //--------------------Revolute-Joint------------------------  
  
    DBox revolBody1(world, 550.0f, 300.0f, 7.0f, 1.0f, 0.0f, 1.0f, sf::Color::Green);  
    SBox revolAnchor1(world, 550.0f, 300.0f, 1.0f, 1.0f, 0.0f, 0.0f);  
  
    b2RevoluteJointDef revoluteJointDef;  
    revoluteJointDef.Initialize(revolAnchor1.getBody(), revolBody1.getBody(), revolAnchor1.getBody()->GetPosition());  
    revoluteJointDef.enableLimit = false;  
  
    b2RevoluteJoint *revoluteJoint;  
    revoluteJoint = (b2RevoluteJoint*)world.CreateJoint(&revoluteJointDef);  
  
    //------------------SFML-loop--------------------------  
  
    while (window.isOpen()) {  
        sf::Event event{};  
        while (window.pollEvent(event)) {  
            if (event.type == sf::Event::Closed)  
                window.close();  
        }  
  
        //Update positions + rotation  
        sBody1.update();  
        sBody2.update();  
        sBody3.update();  
        sBody4.update();  
        sBody5.update();  
        sBody6.update();  
        dBody1.update();  
        dBody2.update();  
        dBody3.update();  
        dBody4.update();  
        dBody5.update();  
        dBody6.update();  
        pulleyBody1.update();  
        pulleyBody2.update();  
        distanceBody1.update();  
        distanceBody2.update();  
        prisBody1.update();  
        prisBody2.update();  
        revolBody1.update();  
  
        // Step the Box2D simulation  
        world.Step(1.0f / 60.0f, 6, 2);  
  
        // Clear the window  
        window.clear(sf::Color::Black);  
  
        // Draw  
        sBody1.draw(window);  
        sBody2.draw(window);  
        sBody3.draw(window);  
        sBody4.draw(window);  
        sBody5.draw(window);  
        sBody6.draw(window);  
        dBody1.draw(window);  
        dBody2.draw(window);  
        dBody3.draw(window);  
        dBody4.draw(window);  
        dBody5.draw(window);  
        dBody6.draw(window);  
        pulleyBody1.draw(window);  
        pulleyBody2.draw(window);  
        distanceBody1.draw(window);  
        distanceBody2.draw(window);  
        prisBody1.draw(window);  
        prisBody2.draw(window);  
        revolBody1.draw(window);  
  
        // Display the frame  
        window.display();  
    }  
  
    return 0;  
}
```

## Bibliografía

1. Colaboradores de los proyectos Wikimedia. “Máquina de Rube Goldberg - Wikipedia, la enciclopedia libre”. Wikipedia, la enciclopedia libre. Accedido el 8 de noviembre de 2023. [En línea]. Disponible: [Máquina de Rube Goldberg](https://es.wikipedia.org/wiki/M%C3%A1quina_de_Rube_Goldberg)
2. “Box2D: Dynamics Module”. Box2D. Accedido el 8 de noviembre de 2023. [En línea]. Disponible: [Dynamics Module](https://box2d.org/documentation/md__d_1__git_hub_box2d_docs_dynamics.html)
3. “GitHub - BiomedicLJZ/Joints”. GitHub. Accedido el 8 de noviembre de 2023. [En línea]. Disponible: [Joints](https://github.com/BiomedicLJZ/Joints)