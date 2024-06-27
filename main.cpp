#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include "Particule.cpp"  // Assurez-vous d'inclure correctement le fichier de définition de la classe Particle
#include "main.h"


// Prototypes de fonctions
void InputHandler(const Event &event, RenderWindow &window, View &view, std::vector<Particle> &particles, std::vector<Particle*> &target);
void update(RenderWindow &window, View &view, std::vector<Particle> &particles, const std::vector<Particle*> &target, const std::vector<std::vector<float> > &matrix);
float forces(float d, float f);
void processParticles(std::vector<Particle> &particles, const std::vector<std::vector<float> > &matrix, int start, int end);

// Initialisation de la variable statique
int Particle::nextID = 0;

int main() {
    // Initialisation de la fenêtre SFML et de la vue
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HIGHT), "ParticleSimulationLife");
    View view(FloatRect(0.f, 0.f, WIN_WIDTH, WIN_HIGHT));
    view.zoom(1);
    view.setCenter(GRILLE_WIDTH / 2, GRILLE_HIGHT / 2);

    // Initialisation de la matrice d'interactions
    interactions_matrix = {
        {0.0, 0.3, -0.9, 0.4, -0.1, 0.5},
        {-0.1, 0.1, -0.6, 0.5, -0.2, 0.9},
        {-0.6, -0.9, 0.2, -0.3, -0.5, -0.8},
        {0.8, -0.4, 0.5, -0.7, 0.6, -0.6},
        {0.6, -0.1, 0.3, 0.1, 0.9, 0.6},
        {0.4, 0.1, -0.2, -1.0, -0.2, 0.1}
    };

    // Affichage de la matrice d'interactions
    std::cout << "Teal | Red Green Yellow Purlpe Orange" << std::endl;
    for (int i = 0; i < nb_species; ++i) {
        for (int j = 0; j < nb_species; ++j) {
            std::cout << interactions_matrix[i][j] << " | ";
        }
        std::cout << std::endl;
    }

    std::cout << "Commandes pour changer les valeurs : " << std::endl;
    std::cout << "AttractionDistance : D" << std::endl;
    std::cout << "AttractionForce : A" << std::endl;
    std::cout << "ParticuleRadius : R" << std::endl;
    std::cout << "DeltaTime: T" << std::endl;
    std::cout << "DeltaInteractionDistance: B" << std::endl;
    std::cout << "HalfLifeVelocity: H" << std::endl;
    std::cout << "Friction: F" << std::endl;

    // Création des particules avec des espèces et couleurs aléatoires
    std::vector<Particle> particles;
    std::vector<Particle*> target;
    std::vector<Color> colors = {
        Color(64, 154, 183), Color(180, 0, 0), Color(0, 165, 70), Color(230, 230, 0), Color(153, 0, 153),
        Color(255, 165, 0), Color(0, 128, 128), Color(128, 0, 128), Color(255, 192, 203), Color(0, 100, 0)
    };

    // Initialisation du générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_species(0, nb_species - 1);

    // Création des particules avec des positions aléatoires
    for (int i = 0; i < nb_particules; ++i) {
        int index = dist_species(gen);
        particles.emplace_back(index, colors[index]);
    }

    // Boucle principale de rendu et de mise à jour
    Clock frameClock;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            InputHandler(event, window, view, particles, target);
        }

        Vector2i lastMousePos = Mouse::getPosition(window);
        if (isDragging) {
            if (lastMousePos != Mouse::getPosition(window)) {
                // Ajustement du point central de la vue en fonction du déplacement de la souris
                Vector2i delta = Mouse::getPosition(window) - lastMousePos;
                view.setCenter(view.getCenter().x - 3 * delta.x, view.getCenter().y - 3 * delta.y);
            }
        }

        // Mise à jour du jeu
        if (Time elapsedTime = frameClock.getElapsedTime(); elapsedTime.asSeconds() >= 1.f / frame) {
            update(window, view, particles, target, interactions_matrix);
            frameClock.restart();
        }
    }

    return 0;
}

void InputHandler(const Event &event, RenderWindow &window, View &view, std::vector<Particle> &particles, std::vector<Particle*> &target) {
    switch (event.type) {
        case Event::Closed:
            window.close();
            break;

        case Event::KeyPressed:
            switch (event.key.code) {
                case Keyboard::Up:
                    if (frame < 60) {
                        frame++;
                        std::cout << "FPS : " << frame << std::endl;
                    }
                    break;

                case Keyboard::Down:
                    if (frame > 1) {
                        frame--;
                        std::cout << "FPS : " << frame << std::endl;
                    }
                    break;

                case Keyboard::Space:
                    running = !running;
                    break;

                case Keyboard::Enter:
                    std::srand(static_cast<unsigned>(std::time(nullptr)));
                    for (int i = 0; i < nb_species; ++i) {
                        for (int j = 0; j < nb_species; ++j) {
                            interactions_matrix[i][j] = -1 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 2));
                            std::cout << interactions_matrix[i][j] << " | ";
                        }
                        std::cout << std::endl;
                    }
                    std::cout << "Commandes pour changer les valeurs : " << std::endl;
                    std::cout << "AttractionDistance : D" << std::endl;
                    std::cout << "AttractionForce : A" << std::endl;
                    std::cout << "ParticuleRadius : R" << std::endl;
                    std::cout << "DeltaTime: T" << std::endl;
                    std::cout << "DeltaInteractionDistance: B" << std::endl;
                    std::cout << "HalfLifeVelocity: H" << std::endl;
                    std::cout << "Friction: F" << std::endl;
                    break;

                case Keyboard::D:
                    std::cout << "Nouvelle valeur pour attractionDistance : ";
                    std::cin >> attractionDistance;
                    break;

                case Keyboard::A:
                    std::cout << "Nouvelle valeur pour attractionForce : ";
                    std::cin >> attractionForce;
                    std::cout << "Nouvelle valeur pour attractionForce : ";
                    std::cin >> attractionForce;
                    break;

                case Keyboard::R:
                    std::cout << "Nouvelle valeur pour particuleRadius : ";
                    std::cin >> particuleRadius;
                    break;

                case Keyboard::H:
                    std::cout << "Nouvelle valeur pour HalfLifeVelocity : ";
                    std::cin >> halflife;
                    break;

                case Keyboard::F:
                    std::cout << "Nouvelle valeur pour Friction : ";
                    std::cin >> excitation;
                    break;

                case Keyboard::B:
                    std::cout << "Nouvelle valeur pour DeltaInteractionDistance : ";
                    std::cin >> delta;
                    break;

                case Keyboard::T:
                    std::cout << "Nouvelle valeur pour DeltaTime : ";
                    std::cin >> dt;
                    break;

                default:
                    break;
            }
            break;

        case Event::MouseButtonPressed:
            if (event.mouseButton.button == Mouse::Left) {

                const Vector2i mousePosition = Mouse::getPosition(window);
                Vector2f worldPosition = window.mapPixelToCoords(mousePosition);

                std::cout << "Position de la souris : " << worldPosition.x << ", " << worldPosition.y << std::endl;

                if (target.empty()) {
                    for (auto &particle : particles) {
                        float dx = worldPosition.x - particle.getX();
                        float dy = worldPosition.y - particle.getY();
                        if (std::sqrt(dx * dx + dy * dy) < 30) {
                            target.emplace_back(&particle);
                        }
                    }
                    if (target.size() > 12) {
                        view.zoom(0.5);
                    }
                } else {
                    view.setSize(WIN_WIDTH, WIN_HIGHT);
                    view.zoom(1);
                    view.setCenter(GRILLE_WIDTH / 2, GRILLE_HIGHT / 2);
                    target.clear();
                }
            } else if (event.mouseButton.button == Mouse::Right) {
                isDragging = true;
            }
            break;

        case Event::MouseButtonReleased:
            if (event.mouseButton.button == Mouse::Right) {
                isDragging = false;
            }
            break;

        case Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.delta > 0) {
                view.zoom(0.98f);
            } else if (event.mouseWheelScroll.delta < 0) {
                view.zoom(1.02f);
            }
            break;

        default:
            break;
    }
}

void update(RenderWindow &window, View &view, std::vector<Particle> &particles, const std::vector<Particle*> &target, const std::vector<std::vector<float> > &matrix) {
    window.clear(Color(0, 0, 15));


    if(running) {
        // Pool de threads pour le traitement parallèle des particules
        BS::thread_pool pool(nb_threads);
        std::vector<std::future<void>> futures;
        const int particlesPerThread = particles.size() / nb_threads;

        // Traitement parallèle des particules
        for (int i = 0; i < nb_threads; ++i) {
            int start = i * particlesPerThread;
            int end = (i == nb_threads - 1) ? particles.size() : (start + particlesPerThread);
            futures.emplace_back(pool.submit_task(
                [&particles, matrix, start, end]
                {
                    processParticles(particles, matrix, start, end);
                }));
        }

        // Attente de la fin du traitement parallèle
        for (auto &future : futures) {
            future.get();
        }

        // Mise à jour de la position des particules
        for (auto &particle : particles) {
            particle.updatePosition();
        }
    }


    // Centre la vue sur le groupe de particules cibles si le nombre est suffisant
    if (target.size() > 12) {
        float x = 0;
        float y = 0;
        for (const auto particle : target) {
            x += particle->getX();
            y += particle->getY();
        }
        view.setCenter(x / target.size(), y / target.size());
    }

    window.setView(view);

    // Affichage des particules visibles dans le viewport
    const FloatRect viewport = view.getViewport();
    for (const auto &particle : particles) {
        float normalizedX = (static_cast<float>(particle.getX()) - view.getCenter().x + view.getSize().x / 2.f) / view.getSize().x;
        float normalizedY = (static_cast<float>(particle.getY()) - view.getCenter().y + view.getSize().y / 2.f) / view.getSize().y;

        if (normalizedX >= viewport.left && normalizedX <= viewport.left + viewport.width &&
            normalizedY >= viewport.top && normalizedY <= viewport.top + viewport.height) {
            window.draw(particle.draw());
        }
    }

    window.display();
}

float forces(float d, float f) {
    float result;
    if (d < delta) {
        result = d / delta - 1;
    } else if (delta < d && d < 1) {
        result = f * (1 - std::abs(2 * d - 1 - delta) / (1 - delta));
    } else {
        result = 0;
    }
    return result;
}

void processParticles(std::vector<Particle> &particles, const std::vector<std::vector<float> > &matrix, int start, int end) {
    for (int i = start; i < end; ++i) {
        float total_force_x = 0;
        float total_force_y = 0;

        for (size_t j = 0; j < particles.size(); ++j) {
            if (j != i) {
                float dx = particles[j].getX() - particles[i].getX();
                float dy = particles[j].getY() - particles[i].getY();

                // Effet de rebobinage autour de la grille
                if (dx > GRILLE_WIDTH / 2.0) dx -= GRILLE_WIDTH;
                if (dx < -GRILLE_WIDTH / 2.0) dx += GRILLE_WIDTH;
                if (dy > GRILLE_HIGHT / 2.0) dy -= GRILLE_HIGHT;
                if (dy < -GRILLE_HIGHT / 2.0) dy += GRILLE_HIGHT;

                float distance = std::sqrt(dx * dx + dy * dy);

                if (distance > particuleRadius && distance < attractionDistance) {
                    float f = forces(distance / attractionDistance,
                                     matrix[particles[i].getSpecie()][particles[j].getSpecie()]);
                    total_force_x += dx / distance * f;
                    total_force_y += dy / distance * f;
                }
            }
        }

        total_force_x *= attractionForce * attractionDistance;
        total_force_y *= attractionForce * attractionDistance;

        particles[i].applyFriction();
        particles[i].applyForce(total_force_x, total_force_y);
    }
}


