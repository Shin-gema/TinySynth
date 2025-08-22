# Architecture du Synthétiseur Modulaire Monophonique

Ce document décrit l'architecture proposée pour un synthétiseur musical monophonique, basé sur un modèle de graphe de traitement audio. L'objectif est de créer un système flexible et extensible où les composants audio peuvent être connectés dynamiquement.

## 1. Principes Fondamentaux

*   **Modularité :** Le synthétiseur est composé de modules indépendants, chacun ayant une fonction spécifique (génération d'onde, filtrage, mixage, etc.).
*   **Graphe de Traitement :** Les modules sont des "nœuds" dans un graphe orienté. Les "arêtes" représentent le flux des signaux (audio ou de contrôle) entre ces nœuds.
*   **Traitement en Temps Réel :** Le système est conçu pour un traitement audio en temps réel, avec une attention particulière à la performance et à la minimisation de la latence.
*   **Monophonique :** Initialement, le synthétiseur ne produira qu'une seule note à la fois.

## 2. Composants Clés (Types de Nœuds)

Chaque module (nœud) implémentera une interface commune, permettant de le connecter à d'autres modules et de traiter des données.

### 2.1. Nœuds de Génération de Signal

*   **Oscillateur (OscillatorNode) :**
    *   **Rôle :** Génère des formes d'ondes audio (sinus, carré, dent de scie, triangle, etc.).
    *   **Entrées :** Fréquence (Hz), Type de forme d'onde, Amplitude.
    *   **Sorties :** Signal audio.
*   **LFO (Low-Frequency Oscillator) (LFONode) :**
    *   **Rôle :** Génère des ondes à très basse fréquence pour la modulation de paramètres.
    *   **Entrées :** Fréquence (Hz), Type de forme d'onde.
    *   **Sorties :** Signal de contrôle (valeurs flottantes).
*   **Enveloppe ADSR (ADSRNode) :**
    *   **Rôle :** Génère une courbe de contrôle temporelle (Attack, Decay, Sustain, Release) en réponse à un événement de déclenchement.
    *   **Entrées :** Signal de déclenchement (gate, ex: Note On/Off), Paramètres ADSR (temps d'attaque, de déclin, niveau de maintien, temps de relâchement).
    *   **Sorties :** Signal de contrôle (valeurs flottantes entre 0.0 et 1.0).

### 2.2. Nœuds de Traitement de Signal Audio

*   **Mixeur (MixerNode) :**
    *   **Rôle :** Combine plusieurs signaux audio en un seul.
    *   **Entrées :** Multiples signaux audio.
    *   **Sorties :** Signal audio combiné.
*   **Filtre (FilterNode) :**
    *   **Rôle :** Modifie le contenu fréquentiel d'un signal audio (ex: passe-bas, passe-haut, passe-bande).
    *   **Entrées :** Signal audio, Fréquence de coupure, Résonance (Q), Type de filtre.
    *   **Sorties :** Signal audio filtré.
*   **Amplificateur / VCA (Voltage Controlled Amplifier) (VCANode) :**
    *   **Rôle :** Contrôle l'amplitude d'un signal audio en fonction d'un signal de contrôle.
    *   **Entrées :** Signal audio, Signal de contrôle d'amplitude (généralement une enveloppe).
    *   **Sorties :** Signal audio dont l'amplitude est modulée.

### 2.3. Nœuds d'Entrée/Sortie

*   **Entrée MIDI (MIDINode) :**
    *   **Rôle :** Reçoit et interprète les messages MIDI (Note On/Off, Pitch Bend, Control Change, etc.).
    *   **Sorties :** Événements de contrôle (Note On/Off, Pitch, CC values) pour d'autres modules.
*   **Sortie Audio (AudioOutputNode) :**
    *   **Rôle :** Envoie le signal audio final au serveur ALSA.
    *   **Entrées :** Signal audio final.
    *   **Sorties :** Aucune (terminal du graphe).

## 3. Structure du Graphe

Le cœur du système sera un `GraphManager` qui gère l'ensemble des modules et leurs connexions.

*   **Connexions :** Chaque connexion spécifie un module source, un port de sortie de ce module, un module de destination et un port d'entrée de ce module.
*   **Types de Ports :** Les ports peuvent être typés (audio, contrôle) pour assurer la cohérence des connexions.
*   **Ordre de Traitement :** Le `GraphManager` sera responsable de déterminer l'ordre correct dans lequel les modules doivent être traités pour assurer que les entrées d'un module sont disponibles avant son exécution.

## 4. Flux de Traitement Audio

1.  **Initialisation :** Le `GraphManager` est configuré avec les modules et leurs connexions. La connexion ALSA est établie.
2.  **Boucle Principale (Thread Audio) :**
    *   **Lecture MIDI :** Les événements MIDI sont lus et convertis en signaux de contrôle pour les modules appropriés (ex: Note On déclenche l'ADSR et définit la fréquence de l'oscillateur).
    *   **Traitement par Bloc :** Le traitement audio s'effectue par petits blocs (buffers) d'échantillons pour minimiser la latence.
    *   **Propagation des Signaux :** Pour chaque bloc, le `GraphManager` itère sur les modules dans l'ordre de traitement :
        *   Chaque module `process()` ses entrées et génère ses sorties pour le bloc courant.
        *   Les sorties sont mises à disposition des modules connectés en aval.
    *   **Sortie ALSA :** Le signal audio final du `AudioOutputNode` est envoyé au buffer ALSA.

## 5. Considérations Techniques

*   **Langage :** C/C++ est recommandé pour les performances en temps réel. Python pourrait être utilisé pour la logique de haut niveau et l'interface utilisateur, mais le cœur du traitement audio devrait être en C/C++.
*   **Gestion de la Mémoire :** Utilisation de buffers pré-alloués pour éviter les allocations dynamiques coûteuses pendant le traitement audio.
*   **Gestion des Erreurs ALSA :** Implémentation robuste pour gérer les erreurs et les "xruns".
*   **Multithreading :** Un thread dédié pour le traitement audio pour garantir la stabilité et la réactivité.
