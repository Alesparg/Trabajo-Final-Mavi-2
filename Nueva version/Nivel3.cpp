#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>
#include <vector>
#include <cmath>

class Nivel3 {
private:
    sf::RenderWindow& window;
    b2World& world;
    sf::RectangleShape cajaObjetivo;
    sf::Text textoObjetivo;
    b2Body* cajaObjetivoBody;
    std::vector<sf::RectangleShape> obstaculos;
    std::vector<b2Body*> obstaculosBodies;
    bool nivelCompletado;
    sf::Font font;
    sf::Texture texturaCajaMadera;
    sf::Texture texturaLadrillo;

    // --- Plataforma móvil ---
    sf::RectangleShape plataforma;
    b2Body* plataformaBody;
    b2PrismaticJoint* plataformaJoint;
    float velocidadPlataforma;
    bool moviendoDerecha;

public:
    Nivel3(sf::RenderWindow& ventana, b2World& mundo)
        : window(ventana), world(mundo), nivelCompletado(false),
        plataformaBody(nullptr), plataformaJoint(nullptr),
        velocidadPlataforma(1.0f), moviendoDerecha(true)
    {
        // Carga textura de la caja de madera
        if (!texturaCajaMadera.loadFromFile("C:\\Users\\Equipo\\Desktop\\imagenes j\\caja de madera.jpg")) {
            std::cout << "Error al cargar la textura de la caja de madera" << std::endl;
            return;
        }
        texturaCajaMadera.setRepeated(true);
        texturaCajaMadera.setSmooth(true);

        // Carga textura de ladrillo
        if (!texturaLadrillo.loadFromFile("C:\\Users\\Equipo\\Desktop\\imagenes j\\ladrillo.jpg")) {
            std::cout << "Error al cargar la textura de ladrillo" << std::endl;
            return;
        }
        texturaLadrillo.setRepeated(true);
        texturaLadrillo.setSmooth(true);

        // Carga fuente
        if (!font.loadFromFile("C:\\Users\\Equipo\\Desktop\\Tipografia\\Manrope-VariableFont_wght.ttf")) {
            std::cout << "Error al cargar la fuente Manrope" << std::endl;
            return;
        }

        // Crea caja objetivo 
        cajaObjetivo.setSize(sf::Vector2f(100.f, 100.f));
        cajaObjetivo.setOrigin(50.f, 50.f);
        cajaObjetivo.setPosition(700.f, 350.f);
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
        objetivoDef.position.Set(700.f / 30.f, 350.f / 30.f);
        cajaObjetivoBody = world.CreateBody(&objetivoDef);
        b2PolygonShape objetivoShape;
        objetivoShape.SetAsBox(50.f / 30.f, 50.f / 30.f);
        cajaObjetivoBody->CreateFixture(&objetivoShape, 0.f);

        // Crea obstáculos estáticos
        crearObstaculo(400.f, 500.f, 200.f, 20.f, sf::Color::Green);
        crearObstaculo(200.f, 400.f, 20.f, 200.f, sf::Color::Yellow);
        crearObstaculo(600.f, 300.f, 20.f, 200.f, sf::Color::Yellow);
        crearObstaculo(400.f, 200.f, 150.f, 20.f, sf::Color(150, 75, 0));

        crearObstaculo(300.f, 350.f, 20.f, 100.f, sf::Color::Yellow);
        crearObstaculo(500.f, 250.f, 20.f, 100.f, sf::Color::Yellow);
        crearObstaculo(350.f, 150.f, 100.f, 20.f, sf::Color(150, 75, 0));

        // Obstáculos dinámicos
        crearObstaculoDinamico(300.f, 150.f, 50.f, 50.f, sf::Color::Transparent);
        crearObstaculoDinamico(500.f, 150.f, 60.f, 60.f, sf::Color::Transparent);
        crearObstaculoDinamico(400.f, 100.f, 60.f, 60.f, sf::Color::Transparent);
        crearObstaculoDinamico(350.f, 200.f, 60.f, 60.f, sf::Color::Transparent);
        crearObstaculoDinamico(450.f, 200.f, 60.f, 60.f, sf::Color::Transparent);

        // Crea la plataforma móvil
        crearPlataformaMovil(400.f, 50.f, 120.f, 20.f);
    }
    


    void crearObstaculo(float x, float y, float ancho, float alto, sf::Color color) {
        sf::RectangleShape obstaculo(sf::Vector2f(ancho, alto));
        obstaculo.setOrigin(ancho / 2, alto / 2);
        obstaculo.setPosition(x, y);
        obstaculo.setTexture(&texturaLadrillo);
        obstaculo.setTextureRect(sf::IntRect(0, 0, ancho, alto));
        obstaculos.push_back(obstaculo);

        b2BodyDef obstaculoDef;
        obstaculoDef.position.Set(x / 30.f, y / 30.f);
        b2Body* obstaculoBody = world.CreateBody(&obstaculoDef);
        b2PolygonShape obstaculoShape;
        obstaculoShape.SetAsBox(ancho / 2 / 30.f, alto / 2 / 30.f);
        obstaculoBody->CreateFixture(&obstaculoShape, 0.f);
        obstaculosBodies.push_back(obstaculoBody);
    }

    void crearObstaculoDinamico(float x, float y, float ancho, float alto, sf::Color color)
    {
        
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(ancho, alto));
        shape.setOrigin(ancho / 2.f, alto / 2.f);
        shape.setPosition(x, y);
        shape.setTexture(&texturaCajaMadera);  
        shape.setFillColor(sf::Color::White);  

        obstaculos.push_back(shape);

        
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(x / 30.f, y / 30.f);

        b2Body* body = world.CreateBody(&bodyDef);

        b2PolygonShape box;
        box.SetAsBox((ancho / 2.f) / 30.f, (alto / 2.f) / 30.f);

        body->CreateFixture(&box, 1.f);

        obstaculosBodies.push_back(body);
    }


    // ---  Plataforma móvil ---
    void crearPlataformaMovil(float x, float y, float ancho, float alto) {
        plataforma.setSize(sf::Vector2f(ancho, alto));
        plataforma.setOrigin(ancho / 2, alto / 2);
        plataforma.setPosition(x, y);
        plataforma.setFillColor(sf::Color::Blue);

        // Cuerpo dinámico
        b2BodyDef plataformaDef;
        plataformaDef.type = b2_dynamicBody;
        plataformaDef.position.Set(x / 30.f, y / 30.f);
        plataformaBody = world.CreateBody(&plataformaDef);

        b2PolygonShape plataformaShape;
        plataformaShape.SetAsBox(ancho / 2 / 30.f, alto / 2 / 30.f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &plataformaShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.5f;
        plataformaBody->CreateFixture(&fixtureDef);

        // Cuerpo estático guía
        b2BodyDef guiaDef;
        guiaDef.position.Set(x / 30.f, y / 30.f);
        b2Body* cuerpoGuia = world.CreateBody(&guiaDef);

        // Joint prismatic
        b2PrismaticJointDef jointDef;
        jointDef.Initialize(cuerpoGuia, plataformaBody, plataformaBody->GetWorldCenter(), b2Vec2(1, 0)); // movimiento horizontal
        jointDef.enableLimit = true;
        jointDef.lowerTranslation = -3.0f; 
        jointDef.upperTranslation = 3.0f; 
        jointDef.enableMotor = true;
        jointDef.maxMotorForce = 50.0f;
        jointDef.motorSpeed = velocidadPlataforma;
        plataformaJoint = (b2PrismaticJoint*)world.CreateJoint(&jointDef);
    }

    void actualizar() {
        // Actualiza obstáculos dinámicos
        for (size_t i = 0; i < obstaculos.size(); ++i) {
            b2Vec2 pos = obstaculosBodies[i]->GetPosition();
            float angle = obstaculosBodies[i]->GetAngle() * 180.f / 3.14159f;
            obstaculos[i].setPosition(pos.x * 30.f, pos.y * 30.f);
            obstaculos[i].setRotation(angle);
        }

        // Movimiento automático de la plataforma
        if (plataformaJoint) {
            float pos = plataformaJoint->GetJointTranslation();
            if (pos >= 3.0f) moviendoDerecha = false;
            else if (pos <= -3.0f) moviendoDerecha = true;

            float velocidad = moviendoDerecha ? velocidadPlataforma : -velocidadPlataforma;
            plataformaJoint->SetMotorSpeed(velocidad);

            b2Vec2 posPlat = plataformaBody->GetPosition();
            plataforma.setPosition(posPlat.x * 30.f, posPlat.y * 30.f);
            plataforma.setRotation(plataformaBody->GetAngle() * 180.f / 3.14159f);
        }
    }

    void dibujar() {
        window.draw(cajaObjetivo);
        window.draw(textoObjetivo);

        for (const auto& obstaculo : obstaculos) window.draw(obstaculo);

        window.draw(plataforma);
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

    bool estaCompletado() const { return nivelCompletado; }
};


