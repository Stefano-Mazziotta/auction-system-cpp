#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>

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
    long amount;
    Person *bidder;

public:
    Bid(Person *, long);
    ~Bid();

    long getAmount();
    Person *getBidder();

    friend ostream &operator<<(ostream &, const Bid &);
};

class Lot
{

private:
    string description;
    int lotId;
    Bid *highestBid;

public:
    Lot(int, string);
    ~Lot();

    bool bidFor(Bid *);
    string getDescription();
    int getLotId();
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
    void doOffer(int lotId, Person *person, long amount);
    Lot *getLot(int);
    void loadLotsFromFile(const string &filename);
    void saveLotsToFile(const string &filename);
    void saveBidsToFile(const string &filename);
};
