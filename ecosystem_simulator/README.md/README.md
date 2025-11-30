# Simulateur d'Écosystème Intelligent

* Un simulateur d'écosystème en 2D développé en C++ avec SDL3 pour mon projet à l'ENSPY1.

## Description

* Ce projet simule un écosystème simple avec différentes entités qui interagissent entre elles :
- **Plantes** - Se reproduisent et servent de nourriture
- **Herbivores** - Mangent les plantes et se reproduisent
- **Carnivores** - Chassent les herbivores
- **Charognards** - Mangent les cadavres

* L'objectif est d'observer comment ces populations évoluent et s'équilibrent naturellement.
* De reproduire dans un monde virtuel le mecanisme et le fonctionnement de l'ecosystem plantes et animaux comme dans la réalité!


* Pour ce faire il nous a fallu préalablement l'installation des outils logiciel pour la réalisation de notre travail ou devoir.
## Installation

### Prérequis
- **SDL3** librairie graphique
- **Git** 

### Étapes d'installation

1. **Télécharger SDL3**
   - Allez sur [libsdl.org](https://libsdl.org/)
   - Téléchargez la version Windows (SDL3-devel-[version]-mingw.zip ou SDL3.3.2.26)
   - Extrayez dans `C:/SDL3/`

2. **Cloner le projet**
   bash
   git clone https://github.com/[NOM_Utilisateur]/Ecosystem_2D.git
   cd ecosystem_simulator


## Compilation

# Avec g++
g++ -std=c++17 -Iinclude -o ecosystem src/*.cpp src/Core/*.cpp src/Graphics/*.cpp -lSDL3

# Avec clang++
clang++ -std=c++17 -Iinclude -o ecosystem src/*.cpp src/Core/*.cpp src/Graphics/*.cpp -lSDL3

* Je n'ai  plus utilisé la commande du prof car avec elle je n'arrivais pas à modifier les erreurs de compilation
bash
g++ -std=c++17 src/core/.cpp src/graphics/.cpp -IC:/SDL3/include -LC:/SDL3/lib/x64 -lmingw32 -lSDL3main -lSDL3 -o ecosystem_simulator


## Exécution
```bash
./ecosystem
```

## Contrôles
- `ESPACE` : Pause/Reprise
- `R` : Reset de la simulation
- `F` : Ajouter de la nourriture
- `FLÈCHES` : Ajuster la vitesse
- `ÉCHAP` : Quitter

---



## Structure du Projet


ecosystem_simulator/
├── include/
│   ├── Core/
│   │   ├── Structs.hpp
│   │   ├── Entity.hpp
│   │   └── Ecosystem.hpp
│   └── Graphics/
│       ├── Window.hpp
│       └── Renderer.hpp
├── src/
│   ├── Core/
│   │   ├── Entity.cpp
│   │   └── Ecosystem.cpp
│   ├── Graphics/
│   │   ├── Window.cpp
│   │   └── Renderer.cpp
│   └── main.cpp
├── assets/
│   └── (futures textures)
└── README.md






## **GUIDE DE DÉVELOPPEMENT ÉTAPE PAR ÉTAPE**

###  **Étape 1 : Configuration de Base**
1. **Créer la structure de fichiers** comme indiqué
2. **Configurer SDL3** dans votre environnement
3. **Implémenter les structs** de base (Vector2D, Color)
4. **Tester la compilation** avec un programme minimal

###  **Étape 2 : Entités de Base**
1. **Implémenter la classe Entity** avec constructeurs/destructeurs
2. **Tester le cycle de vie** des entités
3. **Ajouter le mouvement** basique
4. **Vérifier la gestion mémoire** avec les destructeurs

###  **Étape 3 : Système d'Écosystème**
1. **Créer la classe Ecosystem**
2. **Gérer les collections** d'entités avec std::unique_ptr
3. **Implémenter la nourriture**
4. **Ajouter les statistiques**

###  **Étape 4 : Rendu Graphique**
1. **Créer la fenêtre SDL3**
2. **Afficher les entités** et la nourriture
3. **Tester l'affichage** de base

###  **Étape 5 : Comportements Avancés**
1. **Implémenter la reproduction**
2. **Ajouter la recherche de nourriture**
3. **Créer des interactions** prédateurs/proies
4. **Équilibrer la simulation**


## Fonctionnalités

· Simulation en temps réel d'un écosystème
· Reproduction et mort naturelle des entités
· Chaine alimentaire complète
· Interface graphique simple avec SDL3
· Possibilité d'interagir avec la simulation

## Difficultés Rencontrées

- Installation et configuration de SDL3

- Gestion des dépendances avec MinGW
- Compilation des fichiers multiples
- Gestion de la mémoire et des pointeurs
- Ajour des fichers 'json'dans l'arborescence pour pouvoir compiler le programme suite à des multiple echec survenus lors des multiples tentative.
- En suite, j'ai commencé à implementer depuis un moment déja les fonctions qu'il fallait dans les fichiers .cpp à l'instar de Entity.cp et Ecosytem.cpp. 
- Dans Entity.cpp ont été implémenté les fonctions RemoveDeadENtities(): qui traite du netoyage des entité mortes en creeant une nouvelle liste d'entité ne comporte que des vivantes; HandleEating(): prend en charge la gestion de la chaine alimentaire, elle gère la stabilité de la chaine alimentaire; SpawnRandomEntity(): permet la génération des entités et de l'écosystem. 
- Dans Entity.cpp/ ApplyForce(Vector2D force) prend en charge l'application des forces ^hysique sur les entités à fin de rendre les mouvement entre prédateur et proiees rélistes; StayInBounds(float wordlWidth, float worldHeight) a pour role ou fonction de maintenir les entites dans le cadre de simulation, verifie de gauche à droite les coins de l'eespace représentant le monde virtuel; AvouidPredators(const std::vector<Entity>&predators) simule le comportement defensif chez les herbivores qui representent des proies face aux carnivores; SeekFood(const std::vector<Food>& foodSources) simule le comportement alimentaire du monde réel dans notre monde virtuel selon le respect de la chaine alimentaire.

## Auteur

## **Olivier Austin Mani** 
- Étudiant en Art Numerique à l'ENSPY1 (Ecole National Superieur Polytechnique de Yaounde)
 C'est un projet universitaire de simulation d'écosystème!

## License

Ce projet est développé dans un cadre éducatif.

