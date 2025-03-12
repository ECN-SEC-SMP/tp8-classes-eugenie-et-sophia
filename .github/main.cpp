#include <iostream>
#include <cstdlib>
#include <ctime>

// a rajouter apres? srand(time(NULL)) pour initialiser rand
using namespace std;

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