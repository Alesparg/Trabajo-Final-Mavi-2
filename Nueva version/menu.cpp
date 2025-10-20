#include <SFML/Graphics.hpp>
#include <iostream>

class Menu {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text titulo;
    sf::Text instrucciones;
    sf::Text objetivo;
    sf::RectangleShape botonContinuar;
    sf::RectangleShape botonSalir;
    sf::Text textoContinuar;
    sf::Text textoSalir;

public:
    Menu(sf::RenderWindow& ventana) : window(ventana) {
        // Cargar fuente
        if (!font.loadFromFile("C:\\Users\\Equipo\\Desktop\\Tipografia\\Manrope-VariableFont_wght.ttf")) {
            std::cout << "Error al cargar la fuente Manrope" << std::endl;
            return;
        }

        // Configurar título
        titulo.setFont(font);
        titulo.setString("Ragdolls por los Aires");
        titulo.setCharacterSize(50);
        titulo.setFillColor(sf::Color::White);
        titulo.setPosition(
            (window.getSize().x - titulo.getLocalBounds().width) / 2,
            50
        );

        // Configurar instrucciones
        instrucciones.setFont(font);
        instrucciones.setString("Como se juega:\nSe dispara con click izquierdo");
        instrucciones.setCharacterSize(30);
        instrucciones.setFillColor(sf::Color::White);
        instrucciones.setPosition(
            (window.getSize().x - instrucciones.getLocalBounds().width) / 2,
            150
        );

        // Configurar objetivo
        objetivo.setFont(font);
        objetivo.setString("Para ganar:\nMeter el ragdoll en la caja");
        objetivo.setCharacterSize(30);
        objetivo.setFillColor(sf::Color::White);
        objetivo.setPosition(
            (window.getSize().x - objetivo.getLocalBounds().width) / 2,
            250
        );

        // Configurar botón continuar
        botonContinuar.setSize(sf::Vector2f(200, 50));
        botonContinuar.setPosition(
            (window.getSize().x - 200) / 2,
            350
        );
        botonContinuar.setFillColor(sf::Color::Green);

        textoContinuar.setFont(font);
        textoContinuar.setString("Continuar");
        textoContinuar.setCharacterSize(30);
        textoContinuar.setFillColor(sf::Color::Black);
        textoContinuar.setPosition(
            botonContinuar.getPosition().x + (200 - textoContinuar.getLocalBounds().width) / 2,
            botonContinuar.getPosition().y + 10
        );

        // Configurar botón salir
        botonSalir.setSize(sf::Vector2f(200, 50));
        botonSalir.setPosition(
            (window.getSize().x - 200) / 2,
            420
        );
        botonSalir.setFillColor(sf::Color::Red);

        textoSalir.setFont(font);
        textoSalir.setString("Salir");
        textoSalir.setCharacterSize(30);
        textoSalir.setFillColor(sf::Color::Black);
        textoSalir.setPosition(
            botonSalir.getPosition().x + (200 - textoSalir.getLocalBounds().width) / 2,
            botonSalir.getPosition().y + 10
        );
    }

    bool mostrar() {
        while (window.isOpen()) {
            sf::Event evento;
            while (window.pollEvent(evento)) {
                if (evento.type == sf::Event::Closed) {
                    window.close();
                    return false;
                }

                if (evento.type == sf::Event::MouseButtonPressed) {
                    if (evento.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i posicionMouse = sf::Mouse::getPosition(window);

                        // Verificar clic en botón continuar
                        if (botonContinuar.getGlobalBounds().contains(posicionMouse.x, posicionMouse.y)) {
                            return true;
                        }

                        // Verificar clic en botón salir
                        if (botonSalir.getGlobalBounds().contains(posicionMouse.x, posicionMouse.y)) {
                            window.close();
                            return false;
                        }
                    }
                }
            }

            window.clear(sf::Color(50, 50, 50));

            // Dibujar todos los elementos
            window.draw(titulo);
            window.draw(instrucciones);
            window.draw(objetivo);
            window.draw(botonContinuar);
            window.draw(textoContinuar);
            window.draw(botonSalir);
            window.draw(textoSalir);

            window.display();
        }
        return false;
    }
};