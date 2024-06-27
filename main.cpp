#include "main.h"
#include <bits/unique_ptr.h>


//namespaces
using namespace std;
using namespace sf;


// Définir la taille d'une cellule
constexpr int cellSize = 10;

// taille totale de la grille en pixels
constexpr int WIN_WIDTH = 1080;
constexpr int WIN_HIGHT = 720 ;

// nombre de cellules en largeur et en hauteur
constexpr int GRID_WIDTH = 300 ;
constexpr int GRID_HEIGHT = 300 ;

bool running = false;
bool isDraggingRight = false; // pour le glissement de la vue
bool isDraggingLeft = false; // pour dessiner en glissement de souris
float frame = 60; // FPS
Color gris(48, 48, 48);


struct Cell {
    int x;
    int y;
    mutable bool alive;
    mutable bool nextstat;

    // Constructeur par défaut
    Cell() : x(0), y(0), alive(false), nextstat(false) {}

    Cell(int x_, int y_, bool alive_) : x(x_), y(y_), alive(alive_), nextstat(alive_) {}

    // Méthode pour afficher les informations de la cellule
    void displayInfo() const {
        std::cout << "x: " << x << ", y: " << y << " etat:" << alive << std::endl;
    }

    void update() const {
        alive = nextstat;
    }

    void countLivingNeighbors(const std::vector<std::vector<Cell>>& grid) const {
        int count = 0;
        //std::cout << x << " cellule " << y << std::endl;

        for (int dx = -1; dx <= 1; ++dx){
             for (int dy = -1; dy <= 1; ++dy) {
                 //std::cout << dx << " decalage " << dy << std::endl;

                 // Ne pas compter la cellule elle-même
                 if (dx != 0 || dy != 0) {
                    // Calculer les coordonnées du voisin potentiel
                    int nx = x + dx;
                    int ny = y + dy;

                    // Vérifier si le voisin est à l'intérieur des limites de la grille
                    if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT) {
                        //std::cout << nx << " voisine " << ny << std::endl;
                        if (grid[nx][ny].alive) {
                            ++count;
                        }
                    }
                }
            }
        }

        if (alive) {
            if (count < 2 || count > 3) {
                nextstat = false;
                //std::cout << x << " " << y << " " << alive << " " << nextstat<< " " << count << std::endl;
            }
        }
        else {
            if (count==3) {
                nextstat = true;
                //std::cout << x << " " << y << " " << alive << " " << nextstat<< " " << count << std::endl;
            }
        }

    }
};

//prototypes de fonctions
void update(RenderWindow& window, std::vector<std::vector<Cell>>& cells, const View& view);
void InputHandler(const Event &event, RenderWindow& window, View& view, const std::vector<std::vector<Cell>>& cells);
void random_cases(const std::vector<std::vector<Cell>>& cells);

int main(){
    std::cout << "Largeur de la grille de cellule : " << GRID_WIDTH << std::endl;
    std::cout << "Longueur de la grille de cellule : " << GRID_HEIGHT << std::endl;
    std::cout << "Nombre de cellule total : " << GRID_WIDTH*GRID_HEIGHT << std::endl;
    std::cout << "-------------Manuel d'utilisation-----------\n"
                 "    ESPACE : ON/OFF \n"
                 "    Clique gauche : activer/desactiver une cellule \n"
                 "    Clique droit(maintenir) : glisser le champ de vision \n"
                 "    Touche 'R' : reset la grille ( efface tout ) \n"
                 "    Touche 'D' : cree une grille aleatoire \n"
                 "    Molette : zoom/dezoom sur le champ de vision\n"
                 "    Fleche haut/bas : augmenter/baisser la vitesse d'execution (FPS)"  << GRID_WIDTH*GRID_HEIGHT << std::endl;
    std::cout << "FPS actuels : " << frame << std::endl;

    float centerx = static_cast<float>(GRID_WIDTH)*cellSize/2;  // centre de la grille
    float centery = static_cast<float>(GRID_HEIGHT)*cellSize/2; // centre de la grille

    ContextSettings settings;
    settings.antialiasingLevel = 2;

    // boucle de jeu
    Clock frameClock;

    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HIGHT), "LifeGame", Style::Default,settings);
    View view(FloatRect(0.f, 0.f, WIN_WIDTH, WIN_HIGHT));
    view.zoom(1);
    view.setCenter(centerx, centery); // Modifie la position de la vue pour qu'elle soit centrée sur le point (200, 200) dans la grille


    //activer la Vsynch
    window.setVerticalSyncEnabled(false);


    // Création de la grille de cellules en utilisant un tableau 2D
    std::vector<std::vector<Cell>> cells(GRID_WIDTH, std::vector<Cell>(GRID_HEIGHT));
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            // Ajouter une nouvelle cellule à la grille avec les coordonnées et l'état initial
            auto cell = Cell(x, y, false); // Par défaut, toutes les cellules sont mortes

            cells[x][y] = cell;
            //cell.displayInfo();
        }
    }


    while (window.isOpen()){
        Event event{};
        while (window.pollEvent(event))
        {
            //gestion des inputs/events
            InputHandler(event, window, view, cells);
        }

        Vector2i lastMousePos = Mouse::getPosition(window);
        if (isDraggingRight) {
            if (lastMousePos != Mouse::getPosition(window)) {
                // Ajustez le point central de la vue pour maintenir la position de la souris
                Vector2i delta = Mouse::getPosition(window) - lastMousePos;
                view.setCenter(view.getCenter().x - delta.x*3, view.getCenter().y - delta.y*3);
            }
        }

        if (isDraggingLeft) {
            // Obtenir les coordonnées du clic de souris
            Vector2i mousePosition = Mouse::getPosition(window);
            // Convertir les coordonnées de la souris de la fenêtre au monde de jeu
            Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
            if (cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].alive) {
                cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].alive = false;
                cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].nextstat = false;
            }
            else {
                cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].alive = true;
                cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].nextstat = true;
            }

        }
        // Mesurer le temps écoulé depuis la dernière frame
        if (Time elapsedTime = frameClock.getElapsedTime(); elapsedTime.asSeconds() >= 1.f/frame) {
            // Mettre à jour le jeu uniquement si le temps écoulé est supérieur ou égal à la durée cible d'une frame

            update(window, cells, view);

            window.display();

            // Réinitialiser la minuterie
            frameClock.restart();
        }
    }

    return 0;
}

void random_cases(const std::vector<std::vector<Cell>>& cells) {
    for (auto& row : cells) {
        for (auto& cell : row) {
            // Générer un entier aléatoire entre 0 et 1
            const int randomInt = rand() % 2;

            // Convertir l'entier en booléen
            cell.alive = static_cast<bool>(randomInt);
            cell.nextstat = static_cast<bool>(randomInt);
        }
    }
}

void InputHandler(const Event &event, RenderWindow& window, View& view, const std::vector<std::vector<Cell>>& cells) {

    //fermer la fenetre avec la croix
    if (event.type == Event::Closed) {
        window.close();
    }


    //boutton clavier
    else if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Up) {
            if (frame < 60) {
                frame++;
                std::cout << "FPS : " << frame << std::endl;
            }
        }
        else if (event.key.code == Keyboard::Down) {
            if (frame > 1) {
                frame--;
                std::cout << "FPS : " << frame << std::endl;
            }
        }
        else if (event.key.code == Keyboard::Space) {
            if (running) {
                running = false;
            }
            else {
                running = true;
            }
        }
        else if (event.key.code == Keyboard::R) {
            for (auto& row : cells) {
                for (const auto& cell : row) {
                    cell.alive = false;
                    cell.nextstat = false;
                }
            }
        }
        else if (event.key.code == Keyboard::D) {
            random_cases(cells);
        }
    }

    // Vérifier si un clic de souris a eu lieu
    else if (event.type == Event::MouseButtonPressed) {

        if (event.mouseButton.button == Mouse::Left) {
            Vector2i mousePosition = Mouse::getPosition(window);
            // Convertir les coordonnées de la souris de la fenêtre au monde de jeu
            Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
            if (cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].alive) {
                cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].alive = false;
                cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].nextstat = false;
            }
            else {
                cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].alive = true;
                cells[static_cast<int>(worldPosition.x/cellSize)][static_cast<int>(worldPosition.y/cellSize)].nextstat = true;
            }
            //isDraggingLeft = true;
        }
        else if (event.mouseButton.button == Mouse::Right) {
            isDraggingRight = true;
        }
    }

    // verifier relachement souris
    else if (event.type == Event::MouseButtonReleased) {
            if (event.mouseButton.button == Mouse::Right) {
                isDraggingRight = false;
            }
            if (event.mouseButton.button == Mouse::Left) {
                isDraggingLeft = false;
            }
    }

    // molette de la souris
    else if (event.type == Event::MouseWheelScrolled) {
        // Mettez à jour la vue actuelle

        if (event.mouseWheelScroll.delta > 0) {
            // La molette a été tournée vers le haut
            view.zoom(0.98f);
        } else if (event.mouseWheelScroll.delta < 0) {
            // La molette a été tournée vers le bas
            view.zoom(1.02f);
        }
    }
}



void update(RenderWindow& window, std::vector<std::vector<Cell>>& cells, const View& view) {
    window.clear(gris);

    // Obtenir le rectangle de vue (viewport)
    window.setView(view);
    const FloatRect viewport = view.getViewport();

    // Dessiner les lignes verticales de la grille
    RectangleShape verticalLine(Vector2f(1.f, static_cast<float>(GRID_HEIGHT*cellSize)));
    verticalLine.setFillColor(Color::Black);
    for (int x = -0; x <= GRID_WIDTH; ++x) {
        verticalLine.setPosition(x * cellSize, 0);
        window.draw(verticalLine);
    }

    // Dessiner les lignes horizontales de la grille
    RectangleShape horizontalLine(Vector2f(static_cast<float>(GRID_WIDTH*cellSize), 1.f));
    horizontalLine.setFillColor(Color::Black);
    for (int y = -0; y <= GRID_HEIGHT; ++y) {
        horizontalLine.setPosition(0, y * cellSize);
        window.draw(horizontalLine);
    }

    for (auto& row : cells) {
        for (const auto& cellData : row) {
            // Obtenez les coordonnées normalisées de la cellule par rapport à la taille de la vue
            const float normalizedX = (static_cast<float>(cellData.x * cellSize) - view.getCenter().x + view.getSize().x / 2) / view.getSize().x;
            const float normalizedY = (static_cast<float>(cellData.y * cellSize) - view.getCenter().y + view.getSize().y / 2) / view.getSize().y;

            // Vérifiez si les coordonnées normalisées sont à l'intérieur du viewport
            if (normalizedX >= viewport.left && normalizedX <= viewport.left + viewport.width &&
                normalizedY >= viewport.top && normalizedY <= viewport.top + viewport.height) {
                if (cellData.alive) {
                    RectangleShape cell;
                    const float dx = static_cast<float>(cellData.x) * cellSize;
                    const float dy = static_cast<float>(cellData.y) * cellSize;
                    // Définir la position de la cellule vivante
                    cell.setPosition(dx+1, dy+1);
                    // Définir la couleur de la cellule vivante
                    cell.setFillColor(Color::White);
                    // Définir la taille de la cellule
                    cell.setSize(Vector2f(cellSize-1, cellSize-1));
                    // Dessiner la cellule vivante
                    window.draw(cell);
                }
            }
        }
    }

    if(running) {
        for (auto& row : cells) {
            for (auto& cell : row) {
                cell.countLivingNeighbors(cells);
            }
        }

        for (auto& row : cells) {
            for (auto& cell : row) {
                cell.update();
            }
        }
    }
}

