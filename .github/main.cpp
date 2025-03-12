#include <iostream>
#include <cstdlib>
#include <ctime>

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
