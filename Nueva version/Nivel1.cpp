#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

class Nivel1 {
private:
    sf::RenderWindow& window;
    b2World& world;

    sf::RectangleShape cajaObjetivo;
    sf::Text textoObjetivo;
    b2Body* cajaObjetivoBody;

    sf::RectangleShape penduloShape;
    b2Body* penduloBody;
    b2Body* puntoAnclajeBody;
    b2DistanceJoint* jointPendulo;

    // --- Nuevos elementos ---
    sf::RectangleShape cajaMovibleShape;
    b2Body* cajaMovibleBody;

    sf::RectangleShape muroShape;
    b2Body* muroBody;

    bool nivelCompletado;
    sf::Font font;

    // --- Texturas nuevas ---
    sf::Texture texturaMuro;
    sf::Texture texturaCaja;

public:
    Nivel1(sf::RenderWindow& ventana, b2World& mundo)
        : window(ventana), world(mundo), nivelCompletado(false),
        penduloBody(nullptr), puntoAnclajeBody(nullptr), jointPendulo(nullptr),
        cajaMovibleBody(nullptr), muroBody(nullptr)
    {
        // Fuente
        if (!font.loadFromFile("C:\\Users\\Equipo\\Desktop\\Tipografia\\Manrope-VariableFont_wght.ttf")) {
            std::cout << "Error al cargar la fuente" << std::endl;
        }

        
        //   carga textura
        
        if (!texturaCaja.loadFromFile("C:\\Users\\Equipo\\Desktop\\imagenes j\\caja de madera.jpg")) {
            std::cout << "Error al cargar textura de caja" << std::endl;
        }

        if (!texturaMuro.loadFromFile("C:\\Users\\Equipo\\Desktop\\imagenes j\\ladrillo.jpg")) {
            std::cout << "Error al cargar textura de ladrillo del muro" << std::endl;
        }
        texturaMuro.setRepeated(true);

        // --- Caja objetivo ---
        cajaObjetivo.setSize(sf::Vector2f(100.f, 100.f));
        cajaObjetivo.setOrigin(50.f, 50.f);
        cajaObjetivo.setPosition(700.f, 400.f);
        cajaObjetivo.setFillColor(sf::Color(255, 165, 0));

        textoObjetivo.setFont(font);
        textoObjetivo.setString("Toca\npara ganar");
        textoObjetivo.setCharacterSize(16);
        textoObjetivo.setFillColor(sf::Color::White);
        textoObjetivo.setStyle(sf::Text::Bold);

        sf::FloatRect textoBounds = textoObjetivo.getLocalBounds();
        textoObjetivo.setOrigin(textoBounds.width / 2, textoBounds.height / 2);
        textoObjetivo.setPosition(cajaObjetivo.getPosition());

        // Cuerpo físico de la caja objetivo
        b2BodyDef objetivoDef;
        objetivoDef.position.Set(700.f / 30.f, 400.f / 30.f);
        cajaObjetivoBody = world.CreateBody(&objetivoDef);

        b2PolygonShape objetivoShape;
        objetivoShape.SetAsBox(50.f / 30.f, 50.f / 30.f);
        cajaObjetivoBody->CreateFixture(&objetivoShape, 0.f);

        // --- Punto de anclaje (estático) ---
        b2BodyDef anclajeDef;
        anclajeDef.position.Set(400.f / 30.f, 100.f / 30.f);
        puntoAnclajeBody = world.CreateBody(&anclajeDef);

        // --- Cuerpo del péndulo (dinámico) ---
        float ancho = 20.f;
        float alto = 550.f;

        b2BodyDef penduloDef;
        penduloDef.type = b2_dynamicBody;
        penduloDef.position.Set(400.f / 30.f, (100.f + alto / 2) / 30.f);
        penduloBody = world.CreateBody(&penduloDef);

        b2PolygonShape penduloShapeBox;
        penduloShapeBox.SetAsBox(ancho / 2 / 30.f, alto / 2 / 30.f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &penduloShapeBox;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        penduloBody->CreateFixture(&fixtureDef);

        // Joint del péndulo
        b2DistanceJointDef jointDef;
        jointDef.bodyA = puntoAnclajeBody;
        jointDef.bodyB = penduloBody;
        jointDef.localAnchorA.Set(0, 0);
        jointDef.localAnchorB.Set(0, -alto / 2 / 30.f);
        jointDef.length = alto / 2 / 30.f;
        jointDef.frequencyHz = 0.5f;
        jointDef.dampingRatio = 0.1f;
        jointPendulo = (b2DistanceJoint*)world.CreateJoint(&jointDef);

        // Visual del péndulo
        penduloShape.setSize(sf::Vector2f(ancho, alto));
        penduloShape.setOrigin(ancho / 2, alto / 2);
        penduloShape.setFillColor(sf::Color(100, 100, 100));

        
        //  textura caja
        
        cajaMovibleShape.setSize(sf::Vector2f(60.f, 60.f));
        cajaMovibleShape.setOrigin(30.f, 30.f);
        cajaMovibleShape.setPosition(200.f, 600.f);
        cajaMovibleShape.setTexture(&texturaCaja);

        b2BodyDef cajaMovibleDef;
        cajaMovibleDef.type = b2_dynamicBody;
        cajaMovibleDef.position.Set(500.f / 30.f, 500.f / 30.f);
        cajaMovibleBody = world.CreateBody(&cajaMovibleDef);

        b2PolygonShape cajaMovibleBox;
        cajaMovibleBox.SetAsBox(30.f / 30.f, 30.f / 30.f);

        b2FixtureDef cajaFixture;
        cajaFixture.shape = &cajaMovibleBox;
        cajaFixture.density = 1.0f;
        cajaFixture.friction = 0.4f;
        cajaMovibleBody->CreateFixture(&cajaFixture);

        
        //  textura muro
        
        muroShape.setSize(sf::Vector2f(20.f, 150.f));
        muroShape.setOrigin(10.f, 75.f);
        muroShape.setPosition(100.f, 400.f);
        muroShape.setTexture(&texturaMuro);
        muroShape.setTextureRect(sf::IntRect(0, 0, 40, 300));

        b2BodyDef muroDef;
        muroDef.position.Set(100.f / 30.f, 400.f / 30.f);
        muroBody = world.CreateBody(&muroDef);

        b2PolygonShape muroBox;
        muroBox.SetAsBox(10.f / 30.f, 75.f / 30.f);
        muroBody->CreateFixture(&muroBox, 0.f);
    }

    void actualizar() {
        // Pendulo
        b2Vec2 posPend = penduloBody->GetPosition();
        float angPend = penduloBody->GetAngle() * 180.f / 3.14159f;
        penduloShape.setPosition(posPend.x * 30.f, posPend.y * 30.f);
        penduloShape.setRotation(angPend);

        // Caja movible
        b2Vec2 posCaja = cajaMovibleBody->GetPosition();
        float angCaja = cajaMovibleBody->GetAngle() * 180.f / 3.14159f;
        cajaMovibleShape.setPosition(posCaja.x * 30.f, posCaja.y * 30.f);
        cajaMovibleShape.setRotation(angCaja);
    }

    void dibujar() {
        window.draw(cajaObjetivo);
        window.draw(textoObjetivo);
        window.draw(penduloShape);
        window.draw(cajaMovibleShape);
        window.draw(muroShape);
    }

    bool verificarColision(b2Body* ragdollBody) {
        b2Vec2 p1 = ragdollBody->GetPosition();
        b2Vec2 p2 = cajaObjetivoBody->GetPosition();

        float dx = std::abs(p1.x - p2.x);
        float dy = std::abs(p1.y - p2.y);

        if (dx < 60.f / 30.f && dy < 60.f / 30.f) {
            nivelCompletado = true;
            return true;
        }
        return false;
    }

    bool estaCompletado() const {
        return nivelCompletado;
    }
};













