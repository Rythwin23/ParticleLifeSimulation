//
// Created by rabah on 19/06/2024.
//
#pragma once

#include "main.h"

#ifndef PARTICULE_H
#define PARTICULE_H


inline int attractionForce = 16; // Réduire la force d'attraction pour des mouvements moins brusques
inline int attractionDistance = 100; // Augmenter légèrement la distance d'attraction pour des transitions plus douces
inline float particuleRadius = 3; // Garder la taille des particules inchangée
inline float dt = 0.013;        // Réduire la vitesse d'exécution pour des mouvements plus fluides
inline float delta = 0.15;      // Augmenter légèrement la distance de rapprochement pour éviter les réactions brusques
inline float halflife = 0.025;  // Augmenter la demi-vie de la vélocité pour une stabilisation plus rapide
inline float excitation = 0.4;  // Augmenter la friction pour des mouvements plus calmes
inline float friction = std::pow(excitation, dt / halflife);  // Recalculer la friction




#endif //PARTICULE_H
