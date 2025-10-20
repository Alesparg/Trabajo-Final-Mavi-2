#include <SFML/Graphics.hpp>
#include <iostream>

class Contador {
private:
    sf::Font font;
    sf::Text texto;
    int contador;

public:
    Contador(sf::RenderWindow& window) : contador(0) {
        // Cargar fuente
        if (!font.loadFromFile("C:\\Users\\Equipo\\Desktop\\Tipografia\\Manrope-VariableFont_wght.ttf")) {
            std::cout << "Error al cargar la fuente" << std::endl;
            return;
        }

        // Configurar texto del contador
        texto.setFont(font);
        texto.setCharacterSize(30);
        texto.setFillColor(sf::Color::Red);
        actualizarTexto();
    }

    void incrementar() {
        contador++;
        actualizarTexto();
    }

    void actualizarTexto() {
        texto.setString("Ragdolls: " + std::to_string(contador));
    }

    void dibujar(sf::RenderWindow& window) {
        // Posicionar en la esquina superior derecha
        texto.setPosition(
            window.getSize().x - texto.getLocalBounds().width - 20,
            20
        );
        window.draw(texto);
    }

    int obtenerContador() const {
        return contador;
    }
};