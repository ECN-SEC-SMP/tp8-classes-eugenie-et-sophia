#include <iostream>
#include <cstdlib>
#include <ctime>

// a rajouter apres? srand(time(NULL)) pour initialiser rand
using namespace std;

//---------------------------------------------------------------PLateau---------------------------------------------------------------

//Taille du tableau
const int Taille = 10;

//Initialisation du tableau

void initPlateau(vector<vector<Animal *>> &plateau) {
    int TotalAnimal = (Taille * Taille) / 4; //25% du plateau est rempli

    for (int i = 0; i < TotalAnimal / 2; i++){
        Animal *Loup = new Loup(Taille, Taille);
        Animal *Pierre = new Pierre(Taille, Taille);
        plateau[Loup -> get(X)][Loup -> get(Y)] = Loup;
        plateau[Pierre -> get(X)][Pierre -> get(Y)] = Pierre;
    }
}

void afficherPlateau(vector<vector<Animal *>> &plateau) {
    //On parcourt  l'ensemble des cases
    for (int i = 0; i < Taille; i++){
        for (int j = 0; j < Taille; j++){
            //Cas où la case est vide
            if (plateau[i][j] == nullptr){
                cout << ".  ";
            }
            //Comparaison du type de la case du plateau avec Loup : si le pointeur est un Loup alors la conversion est réussit (=true)
            else if (dynamic_cast<Loup *> (plateau[i][j])){
                cout << "L";
            }
            else{
                cout << "P";
            }
        }
    }
}

int main() {

    vector<vector<Animal *>> plateau(Taille, vector<Animal *>(Taille,nullptr));

    initPlateau(plateau);
    afficherPlateau(plateau);

    return 0;
}

//Classe Animal
class Animal {
protected:
    std::string nom;
    int x, y;
    bool vivant;
    Attaque typeAttaque;

public:
    Animal(int maxX, int maxY) {
        x = rand() % maxX;
        y = rand() % maxY;
        vivant = true;
    }

    std::string getNom() const { return nom; }
    int getX() const { return x; }
    int getY() const { return y; }
    bool getVivant() const { return vivant; }
    Attaque getAttaque() const { return typeAttaque; }

    void setVivant(bool v) { vivant = v; }

    virtual void setAttaque() = 0;
    virtual void deplace(int maxX, int maxY) = 0;

    bool attaque(Animal& a) {
        if (typeAttaque.resoudreAttaque(a.getAttaque())) {
            a.setVivant(false);
            return true;
        }
        return false;
    }
};


//Classe Attaque
class Attaque {
    protected:
    int type // 0 :pierre, 1: Feuille, 2:Ciseaux

    public:
    Attaque() {
        type = rand() % 3; // Attaque aléatoire pierre feuille ou ciseaux
    }

    Attaque(int a) : type(a) {}//crée une attaque spécifique
    
    int getTypeAttaque() const { return type; } // retourne l'attaque
    
    bool resoudreAttaque(Attaque &a) const { // envoie vrai si l’attaque sur laquelle est appliquée la fonction est "plus forte" que l’attaque passée en paramètre
        if ( type == 0 && a.type == 2 || type == 1 && a.type == 0 || type == 2 && a.type == 1){
            return True;
        }
        return false;
    }

    string getNomAttaque() const { // traduit le type numéro en string pour décrire les attaques
        if (type == 0){
            return pierre;
        }
        else if (type == 1){
            return feuille;
        }
        else if (type == 2){
            return ciseaux;
        }
    }
}
