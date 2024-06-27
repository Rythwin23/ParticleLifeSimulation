#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <cmath>
#include <thread>
#include <future>
#include <algorithm>
#include "thread-pool-master/include/BS_thread_pool.hpp"
#include "thread-pool-master/include/BS_thread_pool_utils.hpp"
#include <mutex> // Pour la gestion des mutex si nécessaire


// taille totale de la grille en pixels
constexpr int WIN_WIDTH = 1280;
constexpr int WIN_HIGHT = 720 ;

constexpr int GRILLE_WIDTH = 1280;
constexpr int GRILLE_HIGHT = 720 ;


constexpr int nb_species = 6;
constexpr int nb_particules = 3000;
constexpr int nb_threads = 10;

// Calculer la taille de chaque section de particules
inline  int particlesPerThread = nb_particules / nb_threads;

inline float frame = 60; // FPS
inline bool running = true;
inline bool isDragging = false; // pour le glissement de la vue

// Create a NxN matrix
inline std::vector<std::vector<float> > interactions_matrix(nb_species, std::vector<float>(nb_species));

//namespaces
using namespace std;
using namespace sf;