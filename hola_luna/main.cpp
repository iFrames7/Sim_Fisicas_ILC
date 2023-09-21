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
