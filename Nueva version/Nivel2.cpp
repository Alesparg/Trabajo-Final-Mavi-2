#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <vector>
#include <cmath>

class Nivel2 {
private:
    sf::RenderWindow& window;
    b2World& world;
    sf::RectangleShape cajaObjetivo;
    sf::Text textoObjetivo;
    b2Body* cajaObjetivoBody;
    bool nivelCompletado;
    sf::Font font;

    // --- Texturas ---
    sf::Texture texturaCajaMadera;
    sf::Texture texturaSoga;
    sf::Texture texturaLadrillo;   

    // --- Soga ---
    std::vector<b2Body*> sogaCuerpos;
    std::vector<sf::RectangleShape> sogaVisual;

    // --- Caja dinámica ---
    sf::RectangleShape cajaMovilShape;
    b2Body* cajaMovilBody;

    // --- Muro estático ---
    sf::RectangleShape muroShape;
    b2Body* muroBody;

public:
    Nivel2(sf::RenderWindow& ventana, b2World& mundo)
        : window(ventana), world(mundo), nivelCompletado(false) {

        // Fuente
        if (!font.loadFromFile("C:\\Users\\Equipo\\Desktop\\Tipografia\\Manrope-VariableFont_wght.ttf")) {
            std::cout << "Error al cargar la fuente Manrope" << std::endl;
            return;
        }


        // Textura caja madera
        texturaCajaMadera.loadFromFile("C:\\Users\\Equipo\\Desktop\\imagenes j\\caja de madera.jpg");
        texturaCajaMadera.setSmooth(true);

        

        // --- Cargar textura ladrillo  ---
        if (!texturaLadrillo.loadFromFile("C:\\Users\\Equipo\\Desktop\\imagenes j\\ladrillo.jpg")) {
            std::cout << "Error al cargar la textura de ladrillo" << std::endl;
            return;
        }
        texturaLadrillo.setSmooth(true);
        texturaLadrillo.setRepeated(true);


        // --- Caja objetivo ---
        cajaObjetivo.setSize(sf::Vector2f(100.f, 100.f));
        cajaObjetivo.setOrigin(50.f, 50.f);
        cajaObjetivo.setPosition(700.f, 300.f);
        cajaObjetivo.setFillColor(sf::Color(255, 165, 0));

        textoObjetivo.setFont(font);
        textoObjetivo.setString("Toca\npara ganar");
        textoObjetivo.setCharacterSize(16);
        textoObjetivo.setFillColor(sf::Color::White);
        textoObjetivo.setStyle(sf::Text::Bold);
        sf::FloatRect textoBounds = textoObjetivo.getLocalBounds();
        textoObjetivo.setOrigin(textoBounds.width / 2, textoBounds.height / 2);
        textoObjetivo.setPosition(cajaObjetivo.getPosition());

        // --- Cuerpo físico de la caja objetivo ---
        b2BodyDef objetivoDef;
        objetivoDef.position.Set(700.f / 30.f, 300.f / 30.f);
        cajaObjetivoBody = world.CreateBody(&objetivoDef);

        b2PolygonShape objetivoShape;
        objetivoShape.SetAsBox(50.f / 30.f, 50.f / 30.f);
        cajaObjetivoBody->CreateFixture(&objetivoShape, 0.f);

        // --- Crear la soga colgante ---
        crearSoga(sf::Vector2f(400.f, 200.f), 20, 25.f, 8.f);

        // --- Crear caja móvil (dinámica) ---
        cajaMovilShape.setSize(sf::Vector2f(50.f, 50.f));
        cajaMovilShape.setOrigin(25.f, 25.f);
        cajaMovilShape.setPosition(300.f, 100.f);
        cajaMovilShape.setTexture(&texturaCajaMadera);

        b2BodyDef cajaDef;
        cajaDef.type = b2_dynamicBody;
        cajaDef.position.Set(300.f / 30.f, 100.f / 30.f);
        cajaMovilBody = world.CreateBody(&cajaDef);

        b2PolygonShape cajaBox;
        cajaBox.SetAsBox(25.f / 30.f, 25.f / 30.f);

        b2FixtureDef cajaFixture;
        cajaFixture.shape = &cajaBox;
        cajaFixture.density = 1.0f;
        cajaFixture.friction = 0.5f;
        cajaMovilBody->CreateFixture(&cajaFixture);

        // --- Crear muro estático con textura ---  
        muroShape.setSize(sf::Vector2f(20.f, 150.f));
        muroShape.setOrigin(10.f, 75.f);
        muroShape.setPosition(500.f, 400.f);

        // Aplicar textura
        muroShape.setTexture(&texturaLadrillo);

        // Ajustar repetición para que se vea bien
        muroShape.setTextureRect(sf::IntRect(
            0, 0,
            (int)muroShape.getSize().x,
            (int)muroShape.getSize().y
        ));

        b2BodyDef muroDef;
        muroDef.position.Set(500.f / 30.f, 400.f / 30.f);
        muroBody = world.CreateBody(&muroDef);

        b2PolygonShape muroBox;
        muroBox.SetAsBox(10.f / 30.f, 75.f / 30.f);
        muroBody->CreateFixture(&muroBox, 0.f); // 
    }

    // --- Crea la soga ---
    void crearSoga(sf::Vector2f posicionInicial, int cantidadSegmentos, float largo, float grosor) {
        b2Body* cuerpoAnterior = nullptr;

        for (int i = 0; i < cantidadSegmentos; ++i) {
            b2BodyDef cuerpoDef;
            cuerpoDef.type = b2_dynamicBody;
            cuerpoDef.position.Set(posicionInicial.x / 30.f, (posicionInicial.y + i * largo) / 30.f);
            b2Body* segmento = world.CreateBody(&cuerpoDef);

            b2PolygonShape shape;
            shape.SetAsBox(grosor / 2 / 30.f, largo / 2 / 30.f);

            b2FixtureDef fixtureDef;
            fixtureDef.shape = &shape;
            fixtureDef.density = 1.0f;
            fixtureDef.friction = 0.5f;
            fixtureDef.restitution = 0.1f;
            segmento->CreateFixture(&fixtureDef);

            sf::RectangleShape rect(sf::Vector2f(grosor, largo));
            rect.setOrigin(grosor / 2, largo / 2);
            rect.setTexture(&texturaSoga);
            rect.setTextureRect(sf::IntRect(0, 0, grosor, largo));
            sogaVisual.push_back(rect);
            sogaCuerpos.push_back(segmento);

            // Joints
            if (i == 0) {
                b2BodyDef techoDef;
                techoDef.position.Set(posicionInicial.x / 30.f, posicionInicial.y / 30.f);
                b2Body* techo = world.CreateBody(&techoDef);

                b2RevoluteJointDef jointDef;
                jointDef.Initialize(techo, segmento, b2Vec2(posicionInicial.x / 30.f, posicionInicial.y / 30.f));
                jointDef.collideConnected = false;
                world.CreateJoint(&jointDef);
                cuerpoAnterior = segmento;
            }
            else {
                b2RevoluteJointDef jointDef;
                jointDef.Initialize(cuerpoAnterior, segmento,
                    b2Vec2(posicionInicial.x / 30.f, (posicionInicial.y + (i - 0.5f) * largo) / 30.f));
                jointDef.collideConnected = false;
                world.CreateJoint(&jointDef);
                cuerpoAnterior = segmento;
            }
        }
    }

    // --- Actualiza físicas y posiciones ---
    void actualizar() {
        for (size_t i = 0; i < sogaCuerpos.size(); ++i) {
            b2Vec2 pos = sogaCuerpos[i]->GetPosition();
            float angle = sogaCuerpos[i]->GetAngle() * 180.f / 3.14159f;
            sogaVisual[i].setPosition(pos.x * 30.f, pos.y * 30.f);
            sogaVisual[i].setRotation(angle);
        }

        b2Vec2 posCaja = cajaMovilBody->GetPosition();
        float angleCaja = cajaMovilBody->GetAngle() * 180.f / 3.14159f;
        cajaMovilShape.setPosition(posCaja.x * 30.f, posCaja.y * 30.f);
        cajaMovilShape.setRotation(angleCaja);
    }

    // --- Dibujar todo ---
    void dibujar() {
        for (const auto& segmento : sogaVisual)
            window.draw(segmento);

        window.draw(cajaObjetivo);
        window.draw(textoObjetivo);
        window.draw(cajaMovilShape);
        window.draw(muroShape);
    }

    bool verificarColision(b2Body* ragdollBody) {
        b2Vec2 posRagdoll = ragdollBody->GetPosition();
        b2Vec2 posObjetivo = cajaObjetivoBody->GetPosition();

        float distanciaX = std::abs(posRagdoll.x - posObjetivo.x);
        float distanciaY = std::abs(posRagdoll.y - posObjetivo.y);

        if (distanciaX < 80.f / 30.f && distanciaY < 80.f / 30.f) {
            nivelCompletado = true;
            return true;
        }
        return false;
    }

    bool estaCompletado() const {
        return nivelCompletado;
    }
};







