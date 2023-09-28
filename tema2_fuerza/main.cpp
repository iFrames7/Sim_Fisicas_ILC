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
