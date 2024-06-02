
/// Ejemplo de una subasta simple.
/** La Subasta puede poseer Lotes de articulos a subastar, cada uno de ellos
 *  recibe ofertas hechas por personas de las cuales solo se registra la mayor.
 */

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Person
{

private:
    string name;

public:
    Person(string);
    ~Person();

    string getName();

    friend ostream &operator<<(ostream &, const Person &);
};

class Bid
{

private:
    long value;
    Person *bidder;

public:
    Bid(Person *, long);
    ~Bid();

    long getValue();
    Person *getBidder();

    friend ostream &operator<<(ostream &, const Bid &);
};

class Lot
{

private:
    string descripcion;
    int numero;
    Bid *highestBid;

public:
    Lot(int, string);

    ~Lot();

    bool bidFor(Bid *);
    string getDescripcion();
    int getNumero();
    Bid *getHighestBid();

    friend ostream &operator<<(ostream &, const Lot &);
};

class Auction
{

private:
    vector<Lot *> lots;
    int nextLot;

public:
    /**
     * Constructor de la clase
     * Se inicializa el vector y el loteSiguiente se setea en 1
     */
    Auction();
    /**
     * Destructor de la clase
     */
    ~Auction();

    void insertLot(string);
    void showLots();
    void bidFor(int, Person *, long);
    Lot *getLot(int);
};
