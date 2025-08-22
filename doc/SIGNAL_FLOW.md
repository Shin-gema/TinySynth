# Exemples de Flux de Signal (Signal Flow)

Ce document décrit différentes architectures de synthétiseurs, du plus simple au plus complet, pour illustrer comment les modules peuvent être connectés.

## 1. Synthétiseur Monophonique de Base

C'est l'architecture que nous avons mise en place. Un oscillateur génère le son, et un VCA contrôle son volume en fonction d'une enveloppe ADSR.

```mermaid
graph TD
    subgraph "Source Sonore"
        OSC[Oscillateur]
    end

    subgraph "Contrôleur d'Amplitude"
        ADSR["ADSR (pour VCA)"]
        VCA[VCA]
    end

    subgraph "Sortie"
        OUT[Audio Output]
    end

    OSC -- Signal Audio --> VCA
    ADSR -- Signal de Contrôle --> VCA
    VCA -- Son final --> OUT
```

- **OSC (VCO)** : Génère l'onde brute (la hauteur de la note).
- **ADSR** : Génère une enveloppe (Attack, Decay, Sustain, Release) quand une note est jouée.
- **VCA** : Agit comme un portail. Il laisse passer le son de l'oscillateur en suivant la forme de l'enveloppe ADSR.

---

## 2. Synthétiseur à Plusieurs Oscillateurs

Pour créer des sons plus riches, on utilise plusieurs oscillateurs. Un module **Mixer** est nécessaire pour les additionner avant de traiter le volume.

```mermaid
graph TD
    subgraph "Sources Sonores"
        OSC1[Oscillateur 1]
        OSC2[Oscillateur 2]
        MIX[Mixer]
    end

    subgraph "Contrôleur d'Amplitude"
        ADSR_VCA["ADSR (pour VCA)"]
        VCA[VCA]
    end

    subgraph "Sortie"
        OUT[Audio Output]
    end

    OSC1 -- Audio --> MIX
    OSC2 -- Audio --> MIX
    MIX -- Audio Mixé --> VCA
    ADSR_VCA -- Contrôle --> VCA
    VCA -- Son final --> OUT
```

- **Mixer** : Prend plusieurs signaux audio et les additionne pour n'en former qu'un seul.

---

## 3. Chemin de Synthèse Soustractive Complet

Un synthétiseur soustractif classique ajoute un **filtre (VCF)** pour sculpter le timbre du son. Le filtre lui-même est souvent contrôlé par sa propre enveloppe ADSR.

```mermaid
graph TD
    subgraph "Sources"
        OSC1[Oscillateur 1]
        OSC2[Oscillateur 2]
        MIX[Mixer]
    end

    subgraph "Timbre (Filtre)"
        ADSR_VCF["ADSR (pour Filtre)"]
        VCF["Filtre (VCF)"]
    end

    subgraph "Amplitude (Volume)"
        ADSR_VCA["ADSR (pour Volume)"]
        VCA[VCA]
    end
    
    subgraph "Sortie"
        OUT[Audio Output]
    end

    OSC1 --> MIX
    OSC2 --> MIX
    MIX -- Son brut --> VCF
    ADSR_VCF -- Contrôle du Filtre --> VCF
    VCF -- Son filtré --> VCA
    ADSR_VCA -- Contrôle du Volume --> VCA
    VCA -- Son final --> OUT
```

- **VCF (Voltage-Controlled Filter)** : Agit comme un égaliseur dynamique. Il retire des fréquences (par exemple, rend le son plus sourd ou plus brillant) pour changer le caractère du son.
- **ADSR pour le Filtre** : Permet de créer des effets de balayage, où le timbre du son évolue dans le temps (par exemple, un son qui devient plus brillant à l'attaque).