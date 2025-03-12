#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
// Reorganisé 

//---------------------------------------------------------------Classe Attaque---------------------------------------------------------------
class Attaque {
protected:
    int type; // 0 : Pierre, 1 : Feuille, 2 : Ciseaux

public:
    Attaque() {
        type = rand() % 3; // Génère une attaque aléatoire
    }

    Attaque(int a) : type(a) {} // Crée une attaque spécifique

    int getTypeAttaque() const { return type; }

    bool resoudreAttaque(const Attaque &a) const {
        // Vérifie si l'attaque actuelle gagne contre l'autre attaque
        if ((type == 0 && a.type == 2) || (type == 1 && a.type == 0) || (type == 2 && a.type == 1)) {
            return true;
        }
        return false;
    }

    string getNomAttaque() const {
        if (type == 0) return "Pierre";
        else if (type == 1) return "Feuille";
        else return "Ciseaux";
    }
};

//---------------------------------------------------------------Classe Animal---------------------------------------------------------------
class Animal {
protected:
    string nom;
    int x, y;
    bool vivant;
    Attaque typeAttaque;

public:
    Animal(int maxX, int maxY) {
        x = rand() % maxX;
        y = rand() % maxY;
        vivant = true;
    }

    virtual ~Animal() {} // Destructeur virtuel

    string getNom() const { return nom; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool getVivant() const { return vivant; }
    Attaque getAttaque() const { return typeAttaque; }

    void setVivant(bool v) { vivant = v; }

    virtual void setAttaque() = 0;
    virtual void deplace(int maxX, int maxY) = 0;

    bool attaque(Animal &a) {
        if (typeAttaque.resoudreAttaque(a.getAttaque())) {
            a.setVivant(false); //Quand l'attaque est true le joueur a n'est plus vivant
            return true;
        }
        return false;
    }

    virtual void afficher() const = 0;
};

//---------------------------------------------------------------Classes spécifiques---------------------------------------------------------------
class Pierre : public Animal {
public:
    Pierre(int maxX, int maxY) : Animal(maxX, maxY) {
        nom = "Pierre";
        typeAttaque = Attaque(0); // Toujours "Pierre"
    }

    void setAttaque() override {} // Ne change jamais

    void deplace(int maxX, int maxY) override {} // Ne se déplace pas

    void afficher() const override { cout << " P "; }
};


class Loup : public Animal {
public:
    Loup(int maxX, int maxY) : Animal(maxX, maxY) {
        nom = "Loup";
        setAttaque();
    }

    void setAttaque() override {
        typeAttaque = Attaque(rand() % 3); // Aléatoire
    }

    void deplace(int maxX, int maxY) override {
        x = rand() % maxX;
        y = rand() % maxY;
    }

    void afficher() const override { cout << " L "; }
};


class Lion : public Animal{
public :
    Lion(int maxX, int maxY) : Animal(maxX, maxY) {
        nom = "Lion";
        setAttaque();
    }

    // Définition de l'attaque (aléatoirement feuille ou ciseaux)
    void setAttaque() override {
        int choix = rand() % 2; //Choix aléatoire entre 0 ou 1
        typeAttaque = (choix == 0) ? Attaque(1) : Attaque(2); //Soit Feuille, soit Ciseaux
    }

    // Déplacement en diagonale
    void deplace(int maxX, int maxY) override {
        //Ensemble des possibilités pour un lion
        int dx[4] = {1, 1, -1, -1};
        int dy[4] = {1, -1, -1, 1};

        int direction = rand() % 4;
        x = (x + dx[direction] + maxX) % maxX;
        y = (y + dy[direction] + maxY) % maxY;
    }

    void afficher() const override { cout << " L "; }
};


class Ours : public Animal{
public:
    Ours(int maxX, int maxY) : Animal(maxX, maxY) {
        nom = "Ours";
        setAttaque();
    }

    void setAttaque() override {
        int choix = rand() % 4; //Choix aléatoire entre 0 ou 3
        typeAttaque = Attaque(1); //Attaque Feuille 
    }

    // Déplacement en diagonale
    void deplace(int maxX, int maxY) override {
        //Ensemble des possibilités pour un lion
        int dx[8] = {1, 1, -1, -1, 2, 2, -2, -2};
        int dy[8] = {1, -1, -1, 1, 2, 2, -2, -2};

        int direction = rand() % 8;
        x = (x + dx[direction] + maxX) % maxX;
        y = (y + dy[direction] + maxY) % maxY;
    }

    void afficher() const override { cout << " O "; }

};

//---------------------------------------------------------------PLateau---------------------------------------------------------------
const int Taille = 10;

// Initialisation du plateau avec 25% des cases remplies
void initPlateau(vector<vector<Animal *>> &plateau) {
    int TotalAnimal = (Taille * Taille) / 4; // 25% du plateau
    int animauxPlaces = 0;

    srand(time(NULL)); // Initialisation du générateur aléatoire

    while (animauxPlaces < TotalAnimal) {
        int x = rand() % Taille;
        int y = rand() % Taille;

        if (plateau[x][y] == nullptr) { // Vérifie que la case est vide
            if (animauxPlaces < TotalAnimal / 2)
                plateau[x][y] = new Loup(Taille, Taille);
            else
                plateau[x][y] = new Pierre(Taille, Taille);

            animauxPlaces++;
        }
    }
}

// Affichage du plateau
void afficherPlateau(vector<vector<Animal *>> &plateau) {
    for (int i = 0; i < Taille; i++) {
        for (int j = 0; j < Taille; j++) {
            if (plateau[i][j] == nullptr) {
                cout << " . ";
            } else {
                plateau[i][j]->afficher();
            }
        }
        cout << endl;
    }
}

// Libération de la mémoire
void libererPlateau(vector<vector<Animal *>> &plateau) {
    for (int i = 0; i < Taille; i++) {
        for (int j = 0; j < Taille; j++) {
            delete plateau[i][j]; // Libère la mémoire allouée
            plateau[i][j] = nullptr;
        }
    }
}

//---------------------------------------------------------------Programme principal---------------------------------------------------------------
int main() {
    vector<vector<Animal *>> plateau(Taille, vector<Animal *>(Taille, nullptr));

    initPlateau(plateau);
    afficherPlateau(plateau);
    libererPlateau(plateau);

    return 0;
}
