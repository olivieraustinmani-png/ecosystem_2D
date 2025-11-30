#include "Core/Entity.h" 
#include <cmath> 
#include <iostream> 
#include <algorithm> 
#include <SDL3/SDL.h>

namespace Ecosystem { 
namespace Core { 

// 🏗 CONSTRUCTEUR PRINCIPAL 
Entity::Entity( EntityType type, Vector2D pos, std::string entityName) 
    : mType( type), position( pos), name( entityName),  
      mRandomGenerator( std::random_device{}())  // Initialisation du générateur alé
 { 

    // INITIALISATION SELON LE TYPE 
    switch( mType) { 
        case EntityType::HERBIVORE: 
            mEnergy = 80.0f; 
            mMaxEnergy = 150.0f; 
            mMaxAge = 200; 
            color = Color::Blue(); 
            size = 8.0f; 
            break; 

        case EntityType::CARNIVORE: 
            mEnergy = 100.0f; 
            mMaxEnergy = 200.0f; 
            mMaxAge = 150; 
            color = Color::Red(); 
            size = 12.0f; 
            break; 

        case EntityType::PLANT: 
            mEnergy = 50.0f; 
            mMaxEnergy = 100.0f; 
            mMaxAge = 300; 
            color = Color::Green(); 
            size = 6.0f; 
            break; 
    }

    mAge = 0; 
    mIsAlive = true; 
    mVelocity = GenerateRandomDirection(); 

    std::cout << "🌱Entité créée: " << name << " à (" << position.x << ", " << position.y<<")"<< std::endl;
 } 

// 🏗 CONSTRUCTEUR DE COPIE 
Entity::Entity(const Entity& other) 
    : mType(other.mType), position(other.position), name(other.name + "_copy"), 
      mEnergy(other.mEnergy * 0.7f),  // Enfant a moins d'énergie 
      mMaxEnergy(other.mMaxEnergy), 
      mAge(0),  // Nouvelle entité, âge remis à 0 
      mMaxAge(other.mMaxAge), 
      mIsAlive(true), 
      mVelocity(other.mVelocity), 
      color(other.color), 
      size(other.size * 0.8f),  // Enfant plus petit 
      mRandomGenerator(std::random_device{}()) 
{ 
    std::cout << "👶Copie d'entité créée: " << name << std::endl; 
} 
    
// 🗑 DESTRUCTEUR 
Entity::~Entity() { 
    std::cout << "💀Entité détruite: " << name << " (Âge: " << mAge << ")" << std::endl; 
 } 

//⚙MISE À JOUR PRINCIPALE 
void Entity::Update(float deltaTime) { 
    if ( !mIsAlive) return; 

    // PROCESSUS DE VIE 
    ConsumeEnergy(deltaTime); 
    Age( deltaTime); 
    Move( deltaTime); 
    CheckVitality(); 
} 

// MOUVEMENT 
void Entity::Move(float deltaTime) { 
    if ( mType == EntityType::PLANT) return;  // Les plantes ne bougent pas 
    // Comportement aléatoire occasionnel 
    std::uniform_real_distribution<float> chance( 0.0f, 1.0f); 
    if ( chance(mRandomGenerator) < 0.02f) { 
        mVelocity = GenerateRandomDirection(); 
    }
    // Application du mouvement 
    position = position + mVelocity * deltaTime * 20.0f; 
    // Consommation d'énergie due au mouvement 
    mEnergy -= mVelocity.Distance( Vector2D( 0, 0)) * deltaTime * 0.1f; 
} 

// 🍽 MANGER
 void Entity::Eat( float energy) 
 { 
    mEnergy += energy; 
    if ( mEnergy > mMaxEnergy) { 
         mEnergy = mMaxEnergy; 
    } 
       std::cout << "🍽 " << name << " mange et gagne " << energy << " énergie" << std::endl;
    }
    
 // CONSOMMATION D'ÉNERGIE 
void Entity::ConsumeEnergy(float deltaTime) { 
    float baseConsumption = 0.0f; 
    switch( mType) { 
        case EntityType::HERBIVORE: 
            baseConsumption = 1.5f; 
            break; 
        case EntityType::CARNIVORE: 
            baseConsumption = 2.0f; 
            break; 
        case EntityType::PLANT: 
            baseConsumption = -0.5f;  // Les plantes génèrent de l'énergie ! 
            break; 
    }
    mEnergy -= baseConsumption * deltaTime; 

} 
// VIEILLISSEMENT 
void Entity::Age( float deltaTime) { 
    mAge += static_cast<int>( deltaTime * 10.0f);  // Accéléré pour la simulation 
} 

// ❤VÉRIFICATION DE LA SANTÉ 
void Entity::CheckVitality() { 
    if ( mEnergy <= 0.0f || mAge >= mMaxAge) { 
        mIsAlive = false; 
        std::cout << "💀" << name << " meurt - "; 
        if ( mEnergy <= 0) std::cout << "Faim"; 
        else std::cout << "Vieillesse"; 
        std::cout << std::endl; 
    }
 } 

// REPRODUCTION 
bool Entity::CanReproduce() const { 
    return mIsAlive && mEnergy > mMaxEnergy * 0.8f && mAge > 20; 
} 
std::unique_ptr<Entity> Entity::Reproduce() { 
    if ( !CanReproduce()) return nullptr; 
    // Chance de reproduction 
    std::uniform_real_distribution<float> chance(0.0f, 1.0f); 
    if ( chance(mRandomGenerator) < 0.3f) { 
    }
        mEnergy *= 0.6f;  // Coût énergétique de la reproduction 
        return std::make_unique<Entity>( *this);  // Utilise le constructeur de copi
    return nullptr; 
} 

// GÉNÉRATION DE DIRECTION ALÉATOIRE 
Vector2D Entity::GenerateRandomDirection() { 
    std::uniform_real_distribution<float> dist( -1.0f, 1.0f); 
    return Vector2D( dist( mRandomGenerator), dist(mRandomGenerator)); 
} 

// CALCUL DE LA COULEUR BASÉE SUR L'ÉTAT 
Color Entity::CalculateColorBasedOnState() const { 
    float energyRatio = GetEnergyPercentage(); 
    Color baseColor = color; 
    // Rouge si faible énergie 
    if ( energyRatio < 0.3f) { 
        baseColor.r = 255; 
        baseColor.g = static_cast<uint8_t>( baseColor.g * energyRatio); 
        baseColor.b = static_cast<uint8_t>( baseColor.b * energyRatio); 
    }
    return baseColor; 
} 
//RENDU GRAPHIQUE 
void Entity::Render( SDL_Renderer* renderer) const { 
    if ( !mIsAlive) return; 
    Color renderColor = CalculateColorBasedOnState(); 

    SDL_FRect rect = { 
        position.x - size / 2.0f, 
        position.y - size / 2.0f, 
        size, 
        size 
    }; 

    SDL_SetRenderDrawColor( renderer, renderColor.r, renderColor.g, renderColor.b, renderColor.a);
    SDL_RenderFillRect( renderer, &rect); 
    
    // Indicateur d'énergie (barre de vie) 
    if ( mType != EntityType::PLANT) { 
        float energyBarWidth = size * GetEnergyPercentage(); 
        SDL_FRect energyBar = { 
            position.x - size / 2.0f, 
            position.y - size / 2.0f - 3.0f, 
            energyBarWidth, 
            2.0f 
        };
        SDL_SetRenderDrawColor( renderer, 0, 255, 0, 255); 
        SDL_RenderFillRect( renderer, &energyBar); 
    }
 } 

// implémentation de la fonction pour la gestion de la fonction de silulation
// Cette fonction permet de modifier la vélocvité inetrne de l'entité

//APllication une force physique à l'entite (simulation de movement)
void Entity::ApplyForce(vector2D ApplyForce) { 
    //addition vevctoriel de la force à la velocité actuelle
    mVelocity = mVelocity + force;  
    float maxpSped = ( mType == Entitype::CARNIVORE) ? 120.0f : 80.0f;
    float currebtSpeed = mVelocity.Length();

    // Condition d'evaluation de la vitesse de déplacement: si maximum → ramener au minimum
    if ( currentSpeed > maxSpeed) {
        mVelocity = mVelocity * ( maxSpeed / currentSpeed);
    } 
}

// Cette fonction permet de maintenir l'entite dans les limites du monde simule
// Evite la disparition des entitites de l'ecran
Vector2D Entity::StayInBounds ( float worldWidth, float worldHeight) const {
    Vector steering ( 0.0f, 0.0f);
    float margin = 30.0f; //zone tampon avant les bord

    // condition detection des bords gauche et droite:
     // Si trop près du bord gauche, force positive vers la droite
    // Si trop près du bord droit, force négative vers la gauche
    if ( position.x < margin) steering.x = margin;
    else if ( position.x > worldWidth - margin) steering.x = -margin;
    
    // condition de detection des bords hauts et bas!
    // Pincipee  est le  meme  que  pour les bords gauches et droites
    if ( position.y < margin) steering.y = margin;  
    else if ( position.y > worldHeight - margin) steering.y = -margin;
    
    // AMPLIFICATION : La force est multipliée pour être plus efficace
    return steering * 3.0f;
}

// Fonction permettant aux entite ( animaux) d'echaper aux predateur: fonction de survi
Vector2D Entity::AvoidPredators(const std::vector<Entity>& predators) const {

    // Selection ou flitrage : Seuls les herbivores ont besoin de fuir
    // Les carnivores et plantes ne sont pas concerné!
    if ( mType != EntityType::HERBIVORE) {
        return Vector2D(0.0f, 0.0f);
    }
    
    Vector2D avoidance(0.0f, 0.0f);
    
    // ANALYSE DES PRÉDATEURS : Parcours de tous les prédateurs potentiels
    for ( const auto& predator : predators) {
        // VÉRIFICATION : Le prédateur doit être vivant et de type carnivore
        if ( predator.IsAlive() && predator.mType == EntityType::CARNIVORE) {
            Vector2D toPredator = predator.position - position;
            float distance = toPredator.Length();
            
            // ZONE DE DANGER : Si le prédateur est dans un rayon de 80 unités
            if ( distance < 80.0f && distance > 0) {
                // CALCUL DE FUITE : Direction opposée au prédateur
                // L'intensité augmente avec la proximité
                Vector2D fleeDirection = -toPredator.Normalized();
                avoidance = avoidance + fleeDirection * (80.0f - distance);
            }
        }
    }
    
    return avoidance;
}


// Fonction SeekFood prend en charge la recherche de nutriment pour la survie et le maintien d'energie

Vector2D Entity::SeekFood(const std::vector<Food>& foodSources) const
{
    // EXCLUSION : Les plantes ne cherchent pas de nourriture (photosynthèse)
    if ( mType == EntityType::PLANT) {
        return Vector2D(0.0f, 0.0f);
    }
    
    Vector2D seeking( 0.0f, 0.0f);
    float closestDist = 1000.0f; // Distance initiale très grande
    Vector2D bestDirection( 0.0f, 0.0f);
    
    // RECHERCHE DE NOURRITURE : Analyse de toutes les sources disponibles
    for ( const auto& food : foodSources) {
        Vector2D toFood = food.position - position;
        float dist = toFood.Length();
        
        // CHAÎNE ALIMENTAIRE :
        // - Herbivores mangent des plantes
        // - Carnivores mangent des herbivores
        bool isGoodFood = ( mType == EntityType::HERBIVORE && food.IsPlant()) ||
                         ( mType == EntityType::CARNIVORE && food.IsHerbivore());
        
        // SÉLECTION : On garde la nourriture valide la plus proche
        if (isGoodFood && dist < closestDist) {
            closestDist = dist;
            bestDirection = toFood.Normalized();
        }
    }
    
    // ACTION : Se déplacer vers la nourriture si elle est assez proche
    if ( closestDist < 150.0f) {
        seeking = bestDirection * 2.0f;
    }
    
    return seeking;
}


 
} // namespace Core 
} // namespace Ecosystem