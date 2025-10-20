#include <SFML/Graphics.hpp>
#include <iostream>

class MenuFinales {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text titulo;
    sf::RectangleShape botonSiguienteNivel;
    sf::RectangleShape botonSalir;
    sf::Text textoSiguienteNivel;
    sf::Text textoSalir;
    int nivelActual;

public:
    MenuFinales(sf::RenderWindow& ventana, int nivel) : window(ventana), nivelActual(nivel) {
        // Cargar fuente
        if (!font.loadFromFile("C:\\Users\\Equipo\\Desktop\\Tipografia\\Manrope-VariableFont_wght.ttf")) {
            std::cout << "Error al cargar la fuente Manrope" << std::endl;
            return;
        }

        // Configurar título
        titulo.setFont(font);
        if (nivelActual == 3) {
            titulo.setString("Felicidades completaste el juego");
        }
        else {
            titulo.setString("Nivel " + std::to_string(nivelActual) + " completado");
        }
        titulo.setCharacterSize(50);
        titulo.setFillColor(sf::Color::White);
        titulo.setPosition(
            (window.getSize().x - titulo.getLocalBounds().width) / 2,
            150
        );

        // Configurar botón siguiente nivel 
        if (nivelActual < 3) {
            botonSiguienteNivel.setSize(sf::Vector2f(300, 50));
            botonSiguienteNivel.setPosition(
                (window.getSize().x - 300) / 2,
                300
            );
            botonSiguienteNivel.setFillColor(sf::Color::Green);

            textoSiguienteNivel.setFont(font);
            textoSiguienteNivel.setString("Siguiente Nivel");
            textoSiguienteNivel.setCharacterSize(25);
            textoSiguienteNivel.setFillColor(sf::Color::Black);
            textoSiguienteNivel.setPosition(
                botonSiguienteNivel.getPosition().x + (300 - textoSiguienteNivel.getLocalBounds().width) / 2,
                botonSiguienteNivel.getPosition().y + 10
            );
        }

        // Configurar botón salir
        botonSalir.setSize(sf::Vector2f(200, 50));
        botonSalir.setPosition(
            (window.getSize().x - 200) / 2,
            nivelActual == 3 ? 300 : 380 
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

                        // Verificar clic en botón siguiente nivel 
                        if (nivelActual < 3 && botonSiguienteNivel.getGlobalBounds().contains(posicionMouse.x, posicionMouse.y)) {
                            return true; // Retornar true para indicar que se debe pasar al siguiente nivel
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
            if (nivelActual < 3) {
                window.draw(botonSiguienteNivel);
                window.draw(textoSiguienteNivel);
            }
            window.draw(botonSalir);
            window.draw(textoSalir);

            window.display();
        }
        return false;
    }
};
