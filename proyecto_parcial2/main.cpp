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
