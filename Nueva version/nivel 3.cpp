#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <iostream>

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

public:
    Nivel3(sf::RenderWindow& ventana, b2World& mundo) : window(ventana), world(mundo), nivelCompletado(false) {
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

        // Configura texto objetivo
        textoObjetivo.setFont(font);
        textoObjetivo.setString("Toca\npara ganar");
        textoObjetivo.setCharacterSize(16); 
        textoObjetivo.setFillColor(sf::Color::White);
        textoObjetivo.setStyle(sf::Text::Bold);
        
        // Centra el texto dentro del cuadrado naranja
        sf::FloatRect textoBounds = textoObjetivo.getLocalBounds();
        textoObjetivo.setOrigin(textoBounds.width / 2, textoBounds.height / 2);
        textoObjetivo.setPosition(cajaObjetivo.getPosition());

        // Crea cuerpo físico para la caja objetivo
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

        // Obstáculos 
        crearObstaculo(300.f, 350.f, 20.f, 100.f, sf::Color::Yellow);     
        crearObstaculo(500.f, 250.f, 20.f, 100.f, sf::Color::Yellow);     
        crearObstaculo(350.f, 150.f, 100.f, 20.f, sf::Color(150, 75, 0)); 

        // Obstáculos dinámicos (mismo tamaño que nivel 2)
        crearObstaculoDinamico(300.f, 150.f, 60.f, 60.f, sf::Color::Transparent);    
        crearObstaculoDinamico(500.f, 150.f, 60.f, 60.f, sf::Color::Transparent);   
        crearObstaculoDinamico(400.f, 100.f, 60.f, 60.f, sf::Color::Transparent);   
        crearObstaculoDinamico(350.f, 200.f, 60.f, 60.f, sf::Color::Transparent);   
        crearObstaculoDinamico(450.f, 200.f, 60.f, 60.f, sf::Color::Transparent);   
    }

    void crearObstaculo(float x, float y, float ancho, float alto, sf::Color color) {
        
        sf::RectangleShape obstaculo(sf::Vector2f(ancho, alto));
        obstaculo.setOrigin(ancho / 2, alto / 2);
        obstaculo.setPosition(x, y);
        
        // Configura la textura de ladrillo
        obstaculo.setTexture(&texturaLadrillo);
        obstaculo.setTextureRect(sf::IntRect(0, 0, ancho, alto));
        obstaculo.setFillColor(sf::Color(255, 255, 255, 255)); 
        
        obstaculos.push_back(obstaculo);

        // Crea cuerpo físico
        b2BodyDef obstaculoDef;
        obstaculoDef.position.Set(x / 30.f, y / 30.f);
        b2Body* obstaculoBody = world.CreateBody(&obstaculoDef);

        b2PolygonShape obstaculoShape;
        obstaculoShape.SetAsBox(ancho / 2 / 30.f, alto / 2 / 30.f);
        obstaculoBody->CreateFixture(&obstaculoShape, 0.f);
        obstaculosBodies.push_back(obstaculoBody);
    }

    void crearObstaculoDinamico(float x, float y, float ancho, float alto, sf::Color color) {
        // Crea forma visual
        sf::RectangleShape obstaculo(sf::Vector2f(ancho, alto));
        obstaculo.setOrigin(ancho / 2, alto / 2);
        obstaculo.setPosition(x, y);

        // Configura la textura
        obstaculo.setTexture(&texturaCajaMadera);
        obstaculo.setTextureRect(sf::IntRect(0, 0, texturaCajaMadera.getSize().x, texturaCajaMadera.getSize().y));
        obstaculo.setFillColor(sf::Color(255, 255, 255, 255)); 

        // Asegura que la textura se aplique correctamente
        obstaculo.setOutlineThickness(0);
        obstaculos.push_back(obstaculo);

        // Crea cuerpo físico dinámico
        b2BodyDef obstaculoDef;
        obstaculoDef.type = b2_dynamicBody;
        obstaculoDef.position.Set(x / 30.f, y / 30.f);
        b2Body* obstaculoBody = world.CreateBody(&obstaculoDef);

        b2PolygonShape obstaculoShape;
        obstaculoShape.SetAsBox(ancho / 2 / 30.f, alto / 2 / 30.f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &obstaculoShape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.3f;
        obstaculoBody->CreateFixture(&fixtureDef);
        obstaculosBodies.push_back(obstaculoBody);
    }

    void actualizar() {
        // Actualiza la posición de los obstáculos dinámicos
        for (size_t i = 0; i < obstaculos.size(); ++i) {
            b2Vec2 pos = obstaculosBodies[i]->GetPosition();
            float angle = obstaculosBodies[i]->GetAngle() * 180.f / 3.14159f;
            obstaculos[i].setPosition(pos.x * 30.f, pos.y * 30.f);
            obstaculos[i].setRotation(angle);
        }
    }

    void dibujar() {
        // Dibuja caja objetivo
        window.draw(cajaObjetivo);
        window.draw(textoObjetivo);

        // Dibuja obstáculos
        for (const auto& obstaculo : obstaculos) {
            window.draw(obstaculo);
        }
    }

    bool verificarColision(b2Body* ragdollBody) {
        // Verificar si algún ragdoll está dentro de la caja objetivo
        b2Vec2 posRagdoll = ragdollBody->GetPosition();
        b2Vec2 posObjetivo = cajaObjetivoBody->GetPosition();

        float distanciaX = std::abs(posRagdoll.x - posObjetivo.x);
        float distanciaY = std::abs(posRagdoll.y - posObjetivo.y);

        // Imprimir información de depuración más detallada
        std::cout << "\n=== Información de Colisión ===" << std::endl;
        std::cout << "Posición Ragdoll: (" << posRagdoll.x * 30.f << ", " << posRagdoll.y * 30.f << ")" << std::endl;
        std::cout << "Posición Objetivo: (" << posObjetivo.x * 30.f << ", " << posObjetivo.y * 30.f << ")" << std::endl;
        std::cout << "Distancia X: " << distanciaX * 30.f << " unidades" << std::endl;
        std::cout << "Distancia Y: " << distanciaY * 30.f << " unidades" << std::endl;

        // Aumenta el margen de detección a 80 unidades
        if (distanciaX < 80.f / 30.f && distanciaY < 80.f / 30.f) {
            std::cout << "COLISIÓN DETECTADA!" << std::endl;
            std::cout << "El ragdoll está dentro del rango del objetivo" << std::endl;
            nivelCompletado = true;
            return true;
        }

        return false;
    }

    bool estaCompletado() const {
        return nivelCompletado;
    }
};