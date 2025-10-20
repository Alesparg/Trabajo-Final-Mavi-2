#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "menu.cpp"
#include "contador.cpp"
#include "nivel 1.cpp"
#include "nivel 2.cpp"
#include "nivel 3.cpp"
#include "menu finales.cpp"

#define PI 3.14159265f
#define SCALE 30.f

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ragdolls por los Aires");
    window.setFramerateLimit(60);

    // Mostrar el menú principal
    Menu menu(window);
    if (!menu.mostrar()) {
        return 0; // Si el usuario presiona salir, cerrar el juego
    }

    int nivelActual = 1;
    bool jugando = true;

    while (jugando && window.isOpen()) {
        // Iniciar el juego
        b2Vec2 gravity(0.f, 9.8f);
        b2World world(gravity);

        // Crear contador
        Contador contador(window);

        // Crear nivel actual
        Nivel1* nivel1 = nullptr;
        Nivel2* nivel2 = nullptr;
        Nivel3* nivel3 = nullptr;

        if (nivelActual == 1) {
            nivel1 = new Nivel1(window, world);
        }
        else if (nivelActual == 2) {
            nivel2 = new Nivel2(window, world);
        }
        else {
            nivel3 = new Nivel3(window, world);
        }

        sf::Vector2f cannonBase(50.f, 550.f);
        sf::RectangleShape cannonBarrel(sf::Vector2f(80.f, 20.f));
        cannonBarrel.setOrigin(0.f, cannonBarrel.getSize().y / 2);
        cannonBarrel.setPosition(cannonBase);
        cannonBarrel.setFillColor(sf::Color::Black);

        sf::CircleShape cannonBaseCircle(25.f);
        cannonBaseCircle.setOrigin(25.f, 25.f);
        cannonBaseCircle.setPosition(cannonBase);
        cannonBaseCircle.setFillColor(sf::Color(50, 50, 50));

        // Crear bordes del mundo
        {
            b2BodyDef edgeDef;
            edgeDef.position.Set(0.f, 0.f);
            b2Body* edgeBody = world.CreateBody(&edgeDef);

            b2EdgeShape edge;
            edge.Set(b2Vec2(0.f, 600.f / SCALE), b2Vec2(800.f / SCALE, 600.f / SCALE));
            edgeBody->CreateFixture(&edge, 0.f);
            edge.Set(b2Vec2(0.f, 0.f), b2Vec2(800.f / SCALE, 0.f));
            edgeBody->CreateFixture(&edge, 0.f);
            edge.Set(b2Vec2(0.f, 0.f), b2Vec2(0.f, 600.f / SCALE));
            edgeBody->CreateFixture(&edge, 0.f);
            edge.Set(b2Vec2(800.f / SCALE, 0.f), b2Vec2(800.f / SCALE, 600.f / SCALE));
            edgeBody->CreateFixture(&edge, 0.f);
        }

        struct Ragdoll {
            sf::CircleShape head;
            sf::RectangleShape torso, leftArm, rightArm, leftLeg, rightLeg;
            b2Body* headBody, * torsoBody, * leftArmBody, * rightArmBody, * leftLegBody, * rightLegBody;
        };

        std::vector<Ragdoll> ragdolls;
        sf::Clock shotClock;
        bool nivelCompletado = false;

        while (window.isOpen() && !nivelCompletado) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    jugando = false;
                    break;
                }
            }

            if (!jugando) break;

            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f direction = sf::Vector2f(mousePos) - cannonBase;
            float angle = std::atan2(direction.y, direction.x) * 180.f / PI;
            cannonBarrel.setRotation(angle);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shotClock.getElapsedTime().asMilliseconds() > 500) {
                float dx = direction.x;
                float dy = direction.y;
                float distance = std::sqrt(dx * dx + dy * dy);

                // Incrementar contador cuando se dispara un ragdoll
                contador.incrementar();

                
                float minPower = 0.50f;    
                float maxPower = 10.0f;  
                float minDistance = 70.0f; 
                float maxDistance = 900.0f;

                // Calcular potencia basada en la distancia
                float power;
                if (distance < minDistance) {
                    power = minPower;
                }
                else if (distance > maxDistance) {
                    power = maxPower;
                }
                else {
                    // Interpolación lineal entre minPower y maxPower
                    float t = (distance - minDistance) / (maxDistance - minDistance);
                    power = minPower + t * (maxPower - minPower);
                }

                // Normalizar y aplicar la potencia
                b2Vec2 impulseDir(dx, dy);
                impulseDir.Normalize();
                impulseDir *= power;

                std::cout << "Distancia: " << distance << ", Potencia aplicada: " << power << std::endl;

                Ragdoll rag;

                rag.head.setRadius(6.f);
                rag.head.setOrigin(6.f, 6.f);
                rag.head.setFillColor(sf::Color::Red);
                rag.head.setPosition(cannonBase);

                b2BodyDef headDef;
                headDef.type = b2_dynamicBody;
                headDef.position.Set(cannonBase.x / SCALE, cannonBase.y / SCALE);
                rag.headBody = world.CreateBody(&headDef);

                b2CircleShape headShape;
                headShape.m_radius = 6.f / SCALE;

                b2FixtureDef headFixture;
                headFixture.shape = &headShape;
                headFixture.density = 1.f;
                rag.headBody->CreateFixture(&headFixture);
                rag.headBody->ApplyLinearImpulseToCenter(impulseDir, true);

                rag.torso.setSize(sf::Vector2f(10.f, 25.f));
                rag.torso.setOrigin(5.f, 12.5f);
                rag.torso.setFillColor(sf::Color(200, 0, 0));
                rag.torso.setPosition(cannonBase + sf::Vector2f(0.f, 15.f));

                b2BodyDef torsoDef;
                torsoDef.type = b2_dynamicBody;
                torsoDef.position.Set(cannonBase.x / SCALE, (cannonBase.y + 15.f) / SCALE);
                rag.torsoBody = world.CreateBody(&torsoDef);

                b2PolygonShape torsoShape;
                torsoShape.SetAsBox(5.f / SCALE, 12.5f / SCALE);

                b2FixtureDef torsoFixture;
                torsoFixture.shape = &torsoShape;
                torsoFixture.density = 1.f;
                rag.torsoBody->CreateFixture(&torsoFixture);
                rag.torsoBody->ApplyLinearImpulseToCenter(impulseDir, true);

                sf::Vector2f armSize(15.f, 4.f);
                b2Vec2 armHalf(armSize.x / 2 / SCALE, armSize.y / 2 / SCALE);

                auto createLimb = [&](sf::RectangleShape& shape, b2Body*& body, sf::Vector2f offset, sf::Color color) {
                    shape.setSize(armSize);
                    shape.setOrigin(armSize.x / 2, armSize.y / 2);
                    shape.setFillColor(color);
                    shape.setPosition(rag.torso.getPosition() + offset);

                    b2BodyDef def;
                    def.type = b2_dynamicBody;
                    def.position.Set((rag.torso.getPosition().x + offset.x) / SCALE, (rag.torso.getPosition().y + offset.y) / SCALE);
                    body = world.CreateBody(&def);

                    b2PolygonShape box;
                    box.SetAsBox(armHalf.x, armHalf.y);

                    b2FixtureDef fix;
                    fix.shape = &box;
                    fix.density = 1.f;
                    body->CreateFixture(&fix);
                    body->ApplyLinearImpulseToCenter(impulseDir, true);
                    };

                createLimb(rag.leftArm, rag.leftArmBody, sf::Vector2f(-10.f, -5.f), sf::Color::Yellow);
                createLimb(rag.rightArm, rag.rightArmBody, sf::Vector2f(10.f, -5.f), sf::Color::Yellow);

                sf::Vector2f legSize(6.f, 15.f);
                b2Vec2 legHalf(legSize.x / 2 / SCALE, legSize.y / 2 / SCALE);

                auto createLeg = [&](sf::RectangleShape& shape, b2Body*& body, sf::Vector2f offset, sf::Color color) {
                    shape.setSize(legSize);
                    shape.setOrigin(legSize.x / 2, legSize.y / 2);
                    shape.setFillColor(color);
                    shape.setPosition(rag.torso.getPosition() + offset);

                    b2BodyDef def;
                    def.type = b2_dynamicBody;
                    def.position.Set((rag.torso.getPosition().x + offset.x) / SCALE, (rag.torso.getPosition().y + offset.y) / SCALE);
                    body = world.CreateBody(&def);

                    b2PolygonShape box;
                    box.SetAsBox(legHalf.x, legHalf.y);

                    b2FixtureDef fix;
                    fix.shape = &box;
                    fix.density = 1.f;
                    body->CreateFixture(&fix);
                    body->ApplyLinearImpulseToCenter(impulseDir, true);
                    };

                createLeg(rag.leftLeg, rag.leftLegBody, sf::Vector2f(-5.f, 20.f), sf::Color::Green);
                createLeg(rag.rightLeg, rag.rightLegBody, sf::Vector2f(5.f, 20.f), sf::Color::Green);

                auto createJoint = [&](b2Body* a, b2Body* b, b2Vec2 anchorA, b2Vec2 anchorB) {
                    b2RevoluteJointDef jointDef;
                    jointDef.bodyA = a;
                    jointDef.bodyB = b;
                    jointDef.localAnchorA = anchorA;
                    jointDef.localAnchorB = anchorB;
                    jointDef.collideConnected = false;
                    world.CreateJoint(&jointDef);
                    };

                createJoint(rag.headBody, rag.torsoBody, b2Vec2(0, 6.f / SCALE), b2Vec2(0, -12.5f / SCALE));
                createJoint(rag.torsoBody, rag.leftArmBody, b2Vec2(-5.f / SCALE, -6.f / SCALE), b2Vec2(7.5f / SCALE, 0));
                createJoint(rag.torsoBody, rag.rightArmBody, b2Vec2(5.f / SCALE, -6.f / SCALE), b2Vec2(-7.5f / SCALE, 0));
                createJoint(rag.torsoBody, rag.leftLegBody, b2Vec2(-2.5f / SCALE, 12.5f / SCALE), b2Vec2(0, -7.5f / SCALE));
                createJoint(rag.torsoBody, rag.rightLegBody, b2Vec2(2.5f / SCALE, 12.5f / SCALE), b2Vec2(0, -7.5f / SCALE));
                // Joint adicional de distancia entre cabeza y torso
                b2DistanceJointDef distJointDef;
                distJointDef.bodyA = rag.headBody;
                distJointDef.bodyB = rag.torsoBody;
                distJointDef.localAnchorA = b2Vec2(0, 0);
                distJointDef.localAnchorB = b2Vec2(0, -12.5f / SCALE);
                distJointDef.length = 12.5f / SCALE;
                distJointDef.frequencyHz = 2.0f;
                distJointDef.dampingRatio = 0.5f;
                world.CreateJoint(&distJointDef);

                // Joint prismático en la pierna izquierda
                b2PrismaticJointDef prismJointDef;
                prismJointDef.bodyA = rag.torsoBody;
                prismJointDef.bodyB = rag.leftLegBody;
                prismJointDef.localAnchorA = b2Vec2(-2.5f / SCALE, 12.5f / SCALE);
                prismJointDef.localAnchorB = b2Vec2(0, -7.5f / SCALE);
                prismJointDef.localAxisA = b2Vec2(0.0f, 1.0f);
                prismJointDef.enableLimit = true;
                prismJointDef.lowerTranslation = -0.1f;
                prismJointDef.upperTranslation = 0.1f;
                world.CreateJoint(&prismJointDef);


                ragdolls.push_back(rag);
                shotClock.restart();
            }

            world.Step(1.f / 60.f, 8, 3);

            // Actualizar nivel actual
            if (nivelActual == 1) {
                nivel1->actualizar();
            }
            else if (nivelActual == 2) {
                nivel2->actualizar();
            }
            else {
                nivel3->actualizar();
            }

            // Verificar colisiones con la caja objetivo
            for (auto& rag : ragdolls) {
                if ((nivelActual == 1 && nivel1->verificarColision(rag.headBody)) ||
                    (nivelActual == 2 && nivel2->verificarColision(rag.headBody)) ||
                    (nivelActual == 3 && nivel3->verificarColision(rag.headBody))) {
                    nivelCompletado = true;
                    break;
                }
            }

            // Actualizar posición de los ragdolls
            for (auto& rag : ragdolls) {
                auto update = [&](sf::Transformable& shape, b2Body* body) {
                    b2Vec2 pos = body->GetPosition();
                    float angle = body->GetAngle() * 180 / PI;
                    shape.setPosition(pos.x * SCALE, pos.y * SCALE);
                    shape.setRotation(angle);
                    };

                update(rag.head, rag.headBody);
                update(rag.torso, rag.torsoBody);
                update(rag.leftArm, rag.leftArmBody);
                update(rag.rightArm, rag.rightArmBody);
                update(rag.leftLeg, rag.leftLegBody);
                update(rag.rightLeg, rag.rightLegBody);
            }

            window.clear(sf::Color(220, 220, 255));
            window.draw(cannonBaseCircle);
            window.draw(cannonBarrel);

            // Dibujar nivel actual
            if (nivelActual == 1) {
                nivel1->dibujar();
            }
            else if (nivelActual == 2) {
                nivel2->dibujar();
            }
            else {
                nivel3->dibujar();
            }

            // Dibujar contador
            contador.dibujar(window);

            for (const auto& rag : ragdolls) {
                window.draw(rag.head);
                window.draw(rag.torso);
                window.draw(rag.leftArm);
                window.draw(rag.rightArm);
                window.draw(rag.leftLeg);
                window.draw(rag.rightLeg);
            }

            window.display();
        }

        // Limpiar memoria
        if (nivel1) delete nivel1;
        if (nivel2) delete nivel2;
        if (nivel3) delete nivel3;

        // Si el nivel está completado, mostrar menú
        if (nivelCompletado) {
            MenuFinales menuFinales(window, nivelActual);
            if (menuFinales.mostrar()) {
                // Pasar al siguiente nivel
                nivelActual++;
                if (nivelActual > 3) {
                    // Si ya completamos todos los niveles, volver al menú principal
                    Menu menuPrincipal(window);
                    if (!menuPrincipal.mostrar()) {
                        jugando = false;
                    }
                    nivelActual = 1;
                }
            }
            else {
                jugando = false;
            }
        }
    }

    return 0;
}