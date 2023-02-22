#include "Renderer.h"
#include <iostream>
namespace ParticleLife {
    constexpr char WINDOW_TITLE[] = "Zapoctovy projekt - Vilem Gutvald";

    constexpr unsigned int WINDOW_WIDTH = 1280;
    constexpr unsigned int WINDOW_HEIGHT = 720;

    constexpr char FONT_FILENAME[] = "font.ttf";
    constexpr float TEXT_LINE_HEIGHT = 30;
    constexpr float TEXT_PADDING = 6;

    constexpr size_t CIRCLE_POINTS = 8;

    Renderer::Renderer(Options& options) :
        options_(options), window_(sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE)), font_(), text_() {}

    void Renderer::init() {
        font_.loadFromFile(FONT_FILENAME);
        text_.setFont(font_);
    }
    void Renderer::handleEvents() {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window_.close();
                exit(0);
            }
            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect view(0, 0, (float)event.size.width, (float)event.size.height);
                window_.setView(sf::View(view));
            }
        }
    }
    void Renderer::clear() {
        window_.clear();
    }
    void Renderer::renderParticles(const std::vector<std::vector<Particle>>& particles) {
        float minSize = std::min(window_.getSize().x, window_.getSize().y);
        float pixelsPerUnit = minSize / options_.getWorldSize();
        sf::Vector2f windowCenterOffset = sf::Vector2f((window_.getSize().x - minSize) / 2, (window_.getSize().y - minSize) / 2);
        if (options_.getParticleRadius() == 0)
            renderParticlesAsPoints(particles, pixelsPerUnit, windowCenterOffset);
        else
            renderParticlesAsCircles(particles, pixelsPerUnit, windowCenterOffset);
    }

    void Renderer::renderParticlesAsPoints(const std::vector<std::vector<Particle>>& particles, float pixelsPerUnit, sf::Vector2f windowCenterOffset)
    {
        sf::VertexArray verts = sf::VertexArray();
        for (size_t s = 0; s < particles.size(); s++)
        {
            for (auto&& p : particles[s]) {
                verts.append(sf::Vertex(p.getPosition() * pixelsPerUnit + windowCenterOffset, options_.getSpecies(s).color));
            }
        }
        window_.draw(&verts[0], verts.getVertexCount(), sf::Points);
    }

    void Renderer::renderParticlesAsCircles(const std::vector<std::vector<Particle>>& particles, float pixelsPerUnit, sf::Vector2f windowCenterOffset)
    {
        float circleRadiusPx = options_.getParticleRadius() * pixelsPerUnit;
        sf::Vector2f circleCenterOffset(-circleRadiusPx, -circleRadiusPx);
        sf::CircleShape circle(circleRadiusPx, CIRCLE_POINTS);

        for (size_t s = 0; s < particles.size(); s++)
        {
            circle.setFillColor(options_.getSpecies(s).color);
            for (auto&& p : particles[s]) {
                circle.setPosition(p.getPosition() * pixelsPerUnit + windowCenterOffset + circleCenterOffset);
                window_.draw(circle);
            }
        }
    }

    void Renderer::renderText(size_t line, std::string text, bool alignRight) {
        text_.setString(text);
        float yPos = TEXT_LINE_HEIGHT * line + TEXT_PADDING;
        if (alignRight) {
            unsigned int windowWidth = window_.getSize().x;
            auto textWidth = text_.getLocalBounds().width;
            text_.setPosition(windowWidth - textWidth - TEXT_PADDING, yPos);
        }
        else {
            text_.setPosition(TEXT_PADDING, yPos);
        }
        window_.draw(text_);
    }
    void Renderer::display() {
        window_.display();
    }
}