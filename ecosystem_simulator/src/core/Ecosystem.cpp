#include "Core/Ecosystem.h" 
#include <algorithm> 
#include <iostream> 

namespace Ecosystem { 
namespace Core { 

// 🏗 CONSTRUCTEUR 
Ecosystem::Ecosystem(float width, float height, int maxEntities) 
    : mWorldWidth(width), mWorldHeight(height), mMaxEntities(maxEntities), 
      mDayCycle(0), mRandomGenerator(std::random_device{}()) 
{ 
    // Initialisation des statistiques 
    mStats = {0, 0, 0, 0, 0, 0}; 
    std::cout << "🌍Écosystème créé: " << width << "x" << height << std::endl; 
} 

// 🗑 DESTRUCTEUR 
Ecosystem::~Ecosystem() { 
    std::cout << "🌍Écosystème détruit (" << mEntities.size() << " entités nettoyé)"<< std::endl; 
 } 

// INITIALISATION 
void Ecosystem::Initialize(int initialHerbivores, int initialCarnivores, int initialPlants)
{
    mEntities.clear(); 
    mFoodSources.clear(); 

    // Création des entités initiales 
    for (int i = 0; i < initialHerbivores; ++i) { 
        SpawnRandomEntity(EntityType::HERBIVORE); 
    }
    for (int i = 0; i < initialCarnivores; ++i) { 
        SpawnRandomEntity(EntityType::CARNIVORE); 
    }
    for (int i = 0; i < initialPlants; ++i) { 
        SpawnRandomEntity(EntityType::PLANT); 
    }
    // Nourriture initiale 
    SpawnFood(20); 
    std::cout << "🌱Écosystème initialisé avec " << mEntities.size() << " entités"<< std::endl;
 } 

// MISE À JOUR 
void Ecosystem::Update(float deltaTime) { 
    // Mise à jour de toutes les entités 
    for (auto& entity : mEntities) { 
        entity->Update(deltaTime); 
    }
    // Gestion des comportements 
    HandleEating(); 
    HandleReproduction(); 
    RemoveDeadEntities(); 
    HandlePlantGrowth(deltaTime); 
    // Mise à jour des statistiques 
    UpdateStatistics(); 
    mDayCycle++; 
} 

// GÉNÉRATION DE NOURRITURE 
void Ecosystem::SpawnFood(int count) { 
    for (int i = 0; i < count; ++i) { 
        if (mFoodSources.size() < 100) {  // Limite maximale de nourriture 
            Vector2D position = GetRandomPosition(); 
            mFoodSources.emplace_back(position, 25.0f); 
        } 
    }
 } 

// SUPPRESSION DES ENTITÉS MORTES 
void Ecosystem::RemoveDeadEntities() { 
    int initialCount = mEntities.size(); 

    mEntities.erase( 
        std::remove_if(mEntities.begin(), mEntities.end(), 
            [](const std::unique_ptr<Entity>& entity) {  
                return !entity->IsAlive();  
            }), 
        mEntities.end() 
    ); 
    
    int removedCount = initialCount - mEntities.size(); 
    if (removedCount > 0) { 
        mStats.deathsToday += removedCount; 
    }
 } 

// GESTION DE LA REPRODUCTION 
void Ecosystem::HandleReproduction() {
    std::vector<std::unique_ptr<Entity>> newEntities;
    
    for (auto& entity : mEntities) {
        if (entity->CanReproduce() && mEntities.size() < mMaxEntities) {
            auto baby = entity->Reproduce();
            if (baby) {
                newEntities.push_back(std::move(baby));
                mStats.birthsToday++;
            }
        }
    }
    
    // Ajout des nouveaux entités
    for (auto& newEntity : newEntities) {
        mEntities.push_back(std::move(newEntity));
    }
}

// 🍽 GESTION DE L'ALIMENTATION 
void Ecosystem::HandleEating() { 
    // Ici on implémenterait la logique de recherche de nourriture 
    // Pour l'instant, gestion simplifiée 
    for (auto& entity : mEntities) { 
        if (entity->GetType() == EntityType::PLANT) { 
            // Les plantes génèrent de l'énergie 
            entity->Eat(0.1f); 
        } 
    }
 } 

// MISE À JOUR DES STATISTIQUES 
void Ecosystem::UpdateStatistics() { 
    mStats.totalHerbivores = 0; 
    mStats.totalCarnivores = 0; 
    mStats.totalPlants = 0; 
    mStats.totalFood = mFoodSources.size(); 
    for (const auto& entity : mEntities) { 
        switch (entity->GetType()) { 
            case EntityType::HERBIVORE: 
                mStats.totalHerbivores++; 
                break; 
            case EntityType::CARNIVORE: 
                mStats.totalCarnivores++; 
                break; 
            case EntityType::PLANT: 
                mStats.totalPlants++; 
                break; 
        } 
    }
 } 

// CRÉATION D'ENTITÉ ALÉATOIRE 
void Ecosystem::SpawnRandomEntity(EntityType type) { 
    if (mEntities.size() >= mMaxEntities) return; 
    Vector2D position = GetRandomPosition(); 
    std::string name; 
    switch (type) { 
        case EntityType::HERBIVORE: 
            name = "Herbivore_" + std::to_string(mStats.totalHerbivores); 
            break; 
        case EntityType::CARNIVORE: 
            name = "Carnivore_" + std::to_string(mStats.totalCarnivores); 
            break; 
        case EntityType::PLANT: 
            name = "Plant_" + std::to_string(mStats.totalPlants); 
            break; 
    }
    mEntities.push_back(std::make_unique<Entity>(type, position, name)); 
} 

// POSITION ALÉATOIRE 
Vector2D Ecosystem::GetRandomPosition() { 
    std::uniform_real_distribution<float> distX(0.0f, mWorldWidth); 
    std::uniform_real_distribution<float> distY(0.0f, mWorldHeight); 
    return Vector2D(distX(mRandomGenerator), distY(mRandomGenerator)); 
} 

// CROISSANCE DES PLANTES 
void Ecosystem::HandlePlantGrowth(float deltaTime) { 
    // Occasionnellement, faire pousser de nouvelles plantes 
    std::uniform_real_distribution<float> chance(0.0f, 1.0f); 
    if (chance(mRandomGenerator) < 0.01f && mEntities.size() < mMaxEntities) { 
        SpawnRandomEntity(EntityType::PLANT); 
    }
 } 

// RENDU 
void Ecosystem::Render(SDL_Renderer* renderer) const { 
    // Rendu de la nourriture 
    for (const auto& food : mFoodSources) { 
        SDL_FRect rect = { 
            food.position.x - 3.0f, 
            food.position.y - 3.0f, 
            6.0f, 
            6.0f 
        };
        SDL_SetRenderDrawColor(renderer, food.color.r, food.color.g, food.color.b, food.color.a);
        SDL_RenderFillRect(renderer, &rect); 
    }
    // Rendu des entités 
    for (const auto& entity : mEntities) { 
        entity->Render(renderer); 
    }
} 



// debut de mes implementations
// Cette Fonction permet de  retirer les entités qui sont mortes
void Ecosystem::RemoveDeadEntities()
{
    // Je crée une nouvelle liste vide pour stocker seulement les entités vivantes
    std::vector<std::unique_ptr<Entity>> stillAlive;
    
    // Je regarde chaque entité une par une dans la liste originale
    for (auto& entity : mEntities) {
        if (entity->IsAlive()) {
            stillAlive.push_back(std::move(entity));
        }
    }
    
    // Je remplace l'ancienne liste complète par la nouvelle liste qui contient seulement les vivantes
    // std::move est utilisé pour éviter de copier et optimiser les performances
    mEntities = std::move(stillAlive);
    
}

// Fonction pour gérer le respet de la chaine alimentaire dans l'écosystème
void Ecosystem::HandleEating()
{
    // Parcours  de toutes les entités pour les permettre de  manger
    for (auto& entity : mEntities) {
        // Si l'entité est morte, je passe à la suivante
        if (!entity->IsAlive()) continue;
        
        //  Si c'est un herbivore, il cherche de la nourriture
        if (entity->GetType() == EntityType::HERBIVORE) {
            // verification de disponibilité de nourriture
            for (auto it = mFoodSources.begin(); it != mFoodSources.end(); ) {
                // Calcul la distance entre l'herbivore et la nourriture
                float dist = (it->position - entity->position).Length();
                
                // Si la nourriture est assez proche (moins de 10 unités)
                if (dist < 10.0f) {
                    // L'herbivore mange et gagne de l'énergie
                    entity->Eat(15.0f);
                    // supression de la nourriture mangée 
                    it = mFoodSources.erase(it);
                    break;
                } else {
                    // Si pas assez proche, je passe à la nourriture suivante
                    it++;
                }
            }
        }
        
        // Si c'est un carnivore, il cherche des herbivores
        if (entity->GetType() == EntityType::CARNIVORE) {
            // Recherche des herbivores parmis les autres entités pour trouver des herbivores
            for (auto& other : mEntities) {
                // verification de l'entite detcté ( si herbivore ou pas!)
                if (other->IsAlive() && other->GetType() == EntityType::HERBIVORE) {
                    // Calcule de la distance entre le carnivore et l'herbivore
                    float dist = (other->position - entity->position).Length();
                    
                    // Si l'herbivore est assez proche (moins de 15 unités)
                    if (dist < 15.0f) {
                        // Le carnivore mange et gagne beaucoup d'énergie
                        entity->Eat(30.0f);
                        // Elimination de l'herbivore manger
                        other->mEnergy = 0;
                        break;
                    }
                }
            }
        }
    }
    
}

// Fonction pour créer une nouvelle entité à un endroit aléatoire
void Ecosystem::SpawnRandomEntity(EntityType type)
{
    // Verification du nombre d'entité presente dans l'arène 
    if (mEntities.size() >= mMaxEntities) {
        return; // ne rien faire si c'est plein
    }
    
    // Creation  d'une position aléatoire dans la simulation
    std::uniform_real_distribution<float> distX(0, mWorldWidth);
    std::uniform_real_distribution<float> distY(0, mWorldHeight);
    Vector2D pos(distX(mRandomGenerator), distY(mRandomGenerator));

    std::string name;
    if (type == EntityType::HERBIVORE) {
        name = "Herbivore";
    } else if (type == EntityType::CARNIVORE) {
        name = "Carnivore";
    } else {
        name = "Plante";
    }
    
}


} // namespace Core 
} // namespace Ecosystem