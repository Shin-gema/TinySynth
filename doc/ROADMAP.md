# Feuille de Route de Développement du TinySynth

Cette feuille de route décrit les étapes clés pour le développement du synthétiseur monophonique, structurées en phases incrémentales.

## Phase 1: Fondations du Moteur Audio (MVP - Minimum Viable Product)

*   **Objectif :** Obtenir un son basique via ALSA, contrôlé par des paramètres fixes.
*   **Tâches :**
    *   **Initialisation du Projet :**
        *   Création de la structure de répertoires.
        *   Mise en place d'un système de build (Makefile, CMake, etc.).
    *   **Module de Base (Node/Module Interface) :**
        *   Définition d'une interface commune pour tous les modules (ex: `process(buffer_size)`).
        *   Gestion des entrées/sorties de signaux (buffers).
    *   **Module Oscillateur Simple (OscillatorNode) :**
        *   Implémentation d'une onde sinusoïdale.
        *   Génération d'échantillons à une fréquence fixe.
    *   **Module Sortie Audio (AudioOutputNode) :**
        *   Initialisation de la connexion ALSA (ouverture du périphérique, configuration des paramètres).
        *   Envoi des buffers audio à ALSA.
    *   **Gestionnaire de Graphe Simplifié (GraphManager) :**
        *   Permettre la connexion statique d'un Oscillateur à la Sortie Audio.
        *   Mise en place de la boucle de traitement audio principale.
    *   **Test Initial :** Générer un son sinusoïdal continu.

## Phase 2: Contrôle et Formes d'Ondes

*   **Objectif :** Introduire le contrôle MIDI et des formes d'ondes variées.
*   **Tâches :**
    *   **Implémentation MIDI :**
        *   Lecture des événements MIDI (Note On/Off, Pitch).
        *   Conversion des notes MIDI en fréquences (Hz).
    *   **Amélioration de l'Oscillateur :**
        *   Ajout des formes d'ondes carré, dent de scie, triangle.
        *   Permettre le changement de forme d'onde via un paramètre.
    *   **Module Enveloppe ADSR (ADSRNode) :**
        *   Implémentation complète de l'enveloppe ADSR.
        *   Connexion de l'ADSR à un VCA (voir Phase 3).
    *   **Intégration :** Connecter l'entrée MIDI à l'oscillateur (pour la fréquence) et à l'ADSR (pour le déclenchement).

## Phase 3: Mise en Forme du Son

*   **Objectif :** Ajouter des capacités de sculpture sonore essentielles.
*   **Tâches :**
    *   **Module Amplificateur (VCANode) :**
        *   Implémentation d'un VCA contrôlé par un signal d'entrée (ex: ADSR).
    *   **Module Filtre (FilterNode) :**
        *   Implémentation d'un filtre passe-bas résonant (LPF).
        *   Contrôles pour la fréquence de coupure et la résonance.
    *   **Module Mixeur (MixerNode) :**
        *   Permettre le mixage de deux signaux audio (pour préparer les oscillateurs multiples).
    *   **Refonte du Graphe :** Permettre la connexion dynamique des modules dans l'ordre : Oscillateur -> Mixeur (si plusieurs) -> Filtre -> VCA -> Sortie.

## Phase 4: Enrichissement Sonore et Contrôle Avancé

*   **Objectif :** Ajouter des fonctionnalités pour un son plus riche et un contrôle plus fin.
*   **Tâches :**
    *   **Oscillateurs Multiples :**
        *   Permettre l'instanciation de plusieurs `OscillatorNode`.
        *   Connecter les sorties de plusieurs oscillateurs au `MixerNode`.
        *   Ajout du "detune" entre oscillateurs.
    *   **LFO (LFONode) :**
        *   Implémentation du LFO.
        *   Permettre la connexion du LFO pour moduler la fréquence de l'oscillateur (vibrato) ou la fréquence de coupure du filtre.
    *   **Contrôle MIDI Avancé :**
        *   Gestion du Pitch Bend.
        *   Mapping des Control Change (CC) MIDI à des paramètres de modules (ex: CC pour le cutoff du filtre, CC pour les temps ADSR).

## Phase 5: Optimisation et Robustesse

*   **Objectif :** Améliorer la performance, la stabilité et la gestion des erreurs.
*   **Tâches :**
    *   **Optimisation du Code :** Profilage et optimisation des sections critiques du code audio.
    *   **Gestion des Erreurs ALSA :** Amélioration de la gestion des erreurs et des "xruns".
    *   **Gestion des Ressources :** Assurer une bonne gestion de la mémoire et des ressources système.
    *   **Documentation :** Documentation technique du code et des modules.

## Phases Futures (Idées)

*   **Effets :** Délai, Réverbération, Chorus, Distorsion.
*   **Polyphonie :** Extension pour jouer plusieurs notes simultanément.
*   **Séquenceur / Arpégiateur :** Intégration d'un séquenceur simple.
*   **Interface Graphique (GUI) :** Création d'une interface utilisateur visuelle pour contrôler le synthétiseur.
*   **Chargement/Sauvegarde de Presets :** Possibilité de sauvegarder et charger des configurations de synthétiseur.
