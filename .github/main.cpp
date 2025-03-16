#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

enum TypeAttaque
{
    PIERRE,
    FEUILLE,
    CISEAUX
}; // Pour simplifier le codage

//---------------------------------------------------------------Classe Attaque---------------------------------------------------------------
class Attaque
{
protected:
    int type; // 0 : Pierre, 1 : Feuille, 2 : Ciseaux

public:
    Attaque()
    {
        type = rand() % 3; // Génère une attaque aléatoire, l'attribut type stocke le type d'attaque de l'objet
    }
    Attaque(int a) : type(a) {} // Crée une attaque spécifique

    int getTypeAttaque() const { return type; } // Retourne le type de l'attaque

    bool resoudreAttaque(const Attaque &a) const
    { // détermine qui gagne entre deux attaques
        if (type == a.type)
            return rand() % 2; // Cas d'égalité, le gagnant est choisi au hasard
        return (type == PIERRE && a.type == CISEAUX) ||
               (type == FEUILLE && a.type == PIERRE) ||
               (type == CISEAUX && a.type == FEUILLE);
    }

    string getNomAttaque() const
    {
        if (type == 0)
            return "Pierre";
        else if (type == 1)
            return "Feuille";
        else
            return "Ciseaux";
    }
};

//---------------------------------------------------------------Classe Animal---------------------------------------------------------------
class Animal
{
protected:
    string nom;
    int x, y;
    bool vivant;
    Attaque typeAttaque;

public:
    Animal(int maxX, int maxY)
    {
        x = rand() % maxX;
        y = rand() % maxY;
        vivant = true;
    }

    // On met toutes les caractéritiques de la classe Animal (utilisé pour chaque animal par la suite)
    string getNom() const { return nom; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool getVivant() const
    {
        return this->vivant; // This sert à désigner l'objet (utilisé seulement dans la classe)
    }

    void setVivant(bool v) { vivant = v; }
    virtual void setAttaque() = 0;
    virtual void deplace(int maxX, int maxY) = 0;

    bool attaque(Animal &a) // On fait l'attaque sur le pointeur de l'animal
    {
        if (typeAttaque.resoudreAttaque(a.typeAttaque)) // Si l'attaque est gagnante
        {
            a.setVivant(false); // a meurt
            return true;
        }
        return false;
    }
};

//---------------------------------------------------------------Classes spécifiques---------------------------------------------------------------
class Pierre : public Animal
{
public:
    Pierre(int maxX, int maxY) : Animal(maxX, maxY)
    {
        nom = "Pierre";
        typeAttaque = Attaque(0); // Toujours "Pierre"
    }

    void setAttaque() override { typeAttaque = Attaque(PIERRE); } // L'attaque est toujours P
    void deplace(int maxX, int maxY) override
    {

    } // Ne se déplace pas
};

class Loup : public Animal
{
public:
    Loup(int maxX, int maxY) : Animal(maxX, maxY)
    {
        nom = "Loup";
        setAttaque();
    }

    void setAttaque() override
    {
        typeAttaque = Attaque(rand() % 3); // Aléatoire
    }

    // Se déplacent au hasard sur n’importe quelle cellule du plateau
    void deplace(int maxX, int maxY) override
    {
        x = rand() % maxX;
        y = rand() % maxY;
    }
};

class Lion : public Animal
{
public:
    Lion(int maxX, int maxY) : Animal(maxX, maxY)
    {
        nom = "Lion";
        setAttaque();
    }

    // Définition de l'attaque (aléatoirement feuille ou ciseaux)
    void setAttaque() override
    {
        int choix = rand() % 2;                         // Choix aléatoire entre 0 ou 1
        typeAttaque = (choix == 0) ? FEUILLE : CISEAUX; // Soit Feuille, soit Ciseaux
    }

    // Déplacement en diagonale
    void deplace(int maxX, int maxY) override
    {
        // Ensemble des possibilités pour un lion
        int dx[4] = {1, 1, -1, -1};
        int dy[4] = {1, -1, -1, 1};

        int direction = rand() % 4;
        x = (x + dx[direction] + maxX) % maxX; 
        y = (y + dy[direction] + maxY) % maxY;
    }
};

class Ours : public Animal
{
public:
    Ours(int maxX, int maxY) : Animal(maxX, maxY)
    {
        nom = "Ours";
        setAttaque();
    }

    void setAttaque() override
    {
        int choix = rand() % 4; // Choix aléatoire entre 0 ou 3
        typeAttaque = FEUILLE;  // Attaque Feuille
    }

    void deplace(int maxX, int maxY) override
    {
        // Ensemble des possibilités pour un ours
        int dx[8] = {1, 1, -1, -1, 2, 2, -2, -2};
        int dy[8] = {1, -1, -1, 1, 2, 2, -2, -2};

        int direction = rand() % 8;
        x = (x + dx[direction] + maxX) % maxX;
        y = (y + dy[direction] + maxY) % maxY;
    }
};

//---------------------------------------------------------------PLateau---------------------------------------------------------------
const int Taille = 10; // On crée un tableau de taille 10

// Initialisation du plateau avec 25% des cases remplies
void initPlateau(vector<vector<Animal *>> &plateau, int maxX, int maxY)
{
    int TotalAnimal = (Taille * Taille) / 4; // 25% du plateau
    int animauxPlaces = 0;

    srand(time(NULL)); // Initialisation du générateur aléatoire

    while (animauxPlaces != TotalAnimal)
    {
        int x = rand() % Taille;
        int y = rand() % Taille;

        if (plateau[x][y] == nullptr)
        {                                 // Vérifie que la case est vide
            int type = animauxPlaces % 4; // Réparti les types de façon équilibrée : 0 → L ; 1 → P ; 2 → L ; 3 → O

            if (type == PIERRE)
                plateau[x][y] = new Loup(Taille, Taille);
            else if (type == FEUILLE)
                plateau[x][y] = new Pierre(Taille, Taille);
            else if (type == CISEAUX)
                plateau[x][y] = new Lion(Taille, Taille);
            else
                plateau[x][y] = new Ours(Taille, Taille);

            animauxPlaces++;
        }
    }
}

// Affichage du plateau
void afficherPlateau(const vector<vector<Animal *>> &plateau)
{
    for (int i = 0; i < Taille; i++) // Parcourt toutes les lignes et colonnes
    {
        for (int j = 0; j < Taille; j++)
        {
            if (plateau[i][j] != nullptr) // Si la case contient un animal
            {
                string nameAnimal = plateau[i][j]->getNom(); // associe la case au nom de l'animal

                // Affiche la première lettre du nom de l'animal, pour les lions -> G
                cout << (nameAnimal == "Lion" ? 'G' : nameAnimal[0]) << " ";
            }
            else
            {
                cout << ". "; // Sinon affiche un point
            }
        }
        cout << endl;
    }
}

// Libération de la mémoire allouée dynamiquement
void libererPlateau(vector<vector<Animal *>> &plateau)
{
    for (int i = 0; i < Taille; i++)
    {
        for (int j = 0; j < Taille; j++)
        {
            if (plateau[i][j] != nullptr)
            {
                delete plateau[i][j];
                plateau[i][j] = nullptr;
            }
        }
    }
}

void supprimerAnimal(vector<vector<Animal *>> &plateau, int i, int j)
{
    if (plateau[i][j] != nullptr) // Si case n'est pas vide
    {
        delete plateau[i][j];    // Libère la mémoire
        plateau[i][j] = nullptr; // Évite les accès à un pointeur invalide

    }
}

/*
vector<vector<Animal *>>

    C'est un vecteur de vecteurs contenant des pointeurs vers des objets de type Animal.
    Il représente une grille (matrice) en 2D où chaque case peut contenir un Animal * (un pointeur vers un objet Animal).

&plateau

    Le & signifie que le paramètre est passé par référence, ce qui évite la copie et permet de modifier directement la grille passée en argument.*/

void deplacer(vector<vector<Animal *>> &plateau)
{
    // Ne pas jouer si l'animal est mort
    for (int i = 0; i < Taille; i++)
    {
        for (int j = 0; j < Taille; j++)
        {

            if (plateau[i][j] != nullptr && plateau[i][j]->getVivant())
            {
                int oldX = plateau[i][j]->getX();
                int oldY = plateau[i][j]->getY();

                plateau.at(i).at(j)->deplace(Taille, Taille);

                // Les anciennes coordonnées deviennent les nouvelles
                int newX = plateau[i][j]->getX();
                int newY = plateau[i][j]->getY();

                if (plateau[newX][newY] != nullptr && plateau[newX][newY] != plateau[i][j])
                {                                        // Case non nulle et se combat pas lui même
                    Animal *proie = plateau[newX][newY]; // On stocke l'animal qui occupe déja la case dans une variable proie

                    if (proie->getVivant())
                    {
                        if (plateau[i][j]->attaque(*proie))
                        { // Si a gagne alors la fonction retourne true et la proie est éliminée
                            cout << proie->getNom() << "est mort \n";
                            plateau[newX][newY] = nullptr; // Marquer la case avant de supprimer
                            cout << "test2";
                            delete proie; // On efface la mémoire de l'animal perdant
                            cout << "test3";

                            /* Effacer l'animal mort de la liste des animaux
                            auto it = find(animaux.begin(), animaux.end(), proie);
                            if (it != animaux.end()) {
                                animaux.erase(it); // Retirer l'animal mort de la liste
                                }

                            plateau[newX][newY] = a; //On place l'animal vainqueur sur la case
                            cout<<"test4";*/
                        }
                        else
                        {
                            plateau[i][j]->setVivant(false); // Si a perd le combat il est marqué comme mort et reste sur sa case précédente
                            cout << "test5";
                        }
                    }
                }
                else
                {
                    // Si la case est vide, a se déplace sur la nouvelle case
                    plateau[newX][newY] = plateau[i][j];
                    plateau[oldX][oldY] = nullptr;
                }
            }
        }
    }
}

void resoudreConflits(vector<Animal *> &animaux)
{ // vector<Animal*>& animaux : Une référence à un vecteur contenant des pointeurs vers des objets Animal. Cela représente la liste des animaux sur le plateau.
    for (size_t i = 0; i < animaux.size(); ++i)
    {
        if (!animaux[i]->getVivant())
        {
            continue;
        } // Ignore les animaux déjà morts
        for (size_t j = i + 1; j < animaux.size(); ++j)
        {
            // Pour chaque case on regarde si deux animaux vivants sont présents
            if (animaux[i]->getVivant() && animaux[j]->getVivant() &&
                animaux[i]->getX() == animaux[j]->getX() &&
                animaux[i]->getY() == animaux[j]->getY())
            {
                if (animaux[i]->attaque(*animaux[j]))
                {
                    animaux[j]->setVivant(false); // Si animaux[i] gagne → animaux[j] est marqué comme mort (setVivant(false);).
                    delete animaux[j];
                }
                else
                {
                    animaux[i]->setVivant(false); // Sinon animaux[i] meurt
                    delete animaux[i];
                }
            }
        }
    }
}

//---------------------------------------------------------------Main---------------------------------------------------------------

int main()
{
    srand(time(NULL)); // Initialisation du générateur aléatoire

    // Création du plateau vide
    vector<vector<Animal *>> plateau(Taille, vector<Animal *>(Taille, nullptr));

    // Initialisation du plateau
    initPlateau(plateau, Taille, Taille);
    cout << "Plateau initial : " << endl;
    afficherPlateau(plateau);

    // Déplacement
    deplacer(plateau);
    cout << "Plateau après déplacement : " << endl;
    afficherPlateau(plateau);

    return 0;
}
