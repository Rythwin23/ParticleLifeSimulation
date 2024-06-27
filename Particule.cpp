//
// Created by rabah on 19/06/2024.
//

#include "Particule.h"

class Particle {
private:
    int id;
    float x;
    float y;
    Vector2f velocity;
    int specie;
    Color color;
    static int nextID;

public:
    Particle() : id(nextID++), x(generateRandomFloatX()), y(generateRandomFloatY()), velocity(0.f, 0.f), specie(1), color(Color::White) {}

    Particle(const int s, const Color c) : id(nextID++), x(generateRandomFloatX()), y(generateRandomFloatY()), velocity(0.f, 0.f), specie(s), color(c) {}

    Particle(const float x, const float y, const int s, const Color c) : id(nextID++), x(x), y(y), velocity(0.f, 0.f), specie(s), color(c) {}

    void display() const {
        std::cout << "ID: " << id << " Coor(" << x << "," << y << ") Specie: " << specie << std::endl;
    }

    [[nodiscard]] CircleShape draw() const {
        CircleShape circle(particuleRadius);
        circle.setFillColor(color);
        circle.setPosition(x, y);
        return circle;
    }

    void updatePosition() {
        x += velocity.x * dt;
        y += velocity.y * dt;

        // Wrap-around logic
        if (x < 0) x = GRILLE_WIDTH;
        else if (x > GRILLE_WIDTH) x = 1;

        if (y < 0) y = GRILLE_HIGHT;
        else if (y > GRILLE_HIGHT) y = 1;
    }

    [[nodiscard]] float getX() const {
        return x;
    }

    [[nodiscard]] float getY() const {
        return y;
    }

    void setX(const float xp) {
        x = xp;
    }

    void setY(const float yp) {
        y = yp;
    }

    [[nodiscard]] int getSpecie() const {
        return specie;
    }

    void setSpecie(const int s) {
        specie = s;
    }




    [[nodiscard]] Vector2f getVelocity() const {
        return velocity;
    }

    void setVelocity(const Vector2f& v) {
        velocity = v;
    }

    void applyForce(const float x, const float y) {
        velocity.x += x * dt;
        velocity.y += y * dt;
    }

    void applyFriction() {
        velocity.x *= friction;
        velocity.y *= friction;
    }

    static float generateRandomFloatY() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> distribution(0, static_cast<int>(GRILLE_HIGHT));
        return distribution(gen);
    }

    static float generateRandomFloatX() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<> distribution(0, static_cast<int>(GRILLE_WIDTH));
        return distribution(gen);
    }
};
