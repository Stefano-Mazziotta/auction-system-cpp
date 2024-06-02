#include "subasta.h"

Person::Person(string n)
{

    name = n;
}

Person::~Person(){};

string Person::getName()
{

    return name;
}

ostream &operator<<(ostream &o, const Person &p)
{

    o << "Person's name: " << p.name;
    return o;
}

Bid::Bid(Person *p, long v)
{

    bidder = p;
    value = v;
}

Bid::~Bid()
{
    delete (bidder);
}

long Bid::getValue()
{

    return value;
}

Person *Bid::getBidder()
{

    return bidder;
}

ostream &operator<<(ostream &o, const Bid &bid)
{

    o << "Oferta: " << *(bid.bidder) << " - Valor de oferta: " << bid.value
      << endl;
    return o;
}

Lot::Lot(int n, string d)
{

    highestBid = NULL;
    numero = n;
    descripcion = d;
}

Lot::~Lot()
{
    delete (highestBid);
}

bool Lot::bidFor(Bid *bid)
{

    if ((highestBid == NULL) || (bid->getValue() > highestBid->getValue()))
    {
        // This bid is the best so far.
        highestBid = bid;
        // cout << *(mayorOferta);
        return true;
    }
    else
    {
        return false;
    }
}

string Lot::getDescripcion()
{

    return descripcion;
}

int Lot::getNumero()
{

    return numero;
}

Bid *Lot::getHighestBid()
{

    return highestBid;
}

ostream &operator<<(ostream &o, const Lot &lot)
{

    o << "Lote " << lot.descripcion << ": " << "\n \t Numero: " << lot.numero
      << "\n \t Oferta: " << lot.highestBid << endl;
    return o;
}

Auction::Auction()
{
    nextLot = 1;
}

Auction::~Auction()
{
    vector<Lot *>::const_iterator it;
    for (it = lots.begin(); it != lots.end(); ++it)
    {
        delete (*it);
    }
}

void Auction::insertLot(string descripcion)
{

    lots.push_back(new Lot(nextLot, descripcion));
    nextLot++;
}

void Auction::showLots()
{

    vector<Lot *>::const_iterator it;
    for (it = lots.begin(); it != lots.end(); ++it)
    {
        cout << **it << endl;
    }
}

Lot *Auction::getLot(int lotNumber)
{
    if ((lotNumber >= 1) && (lotNumber < nextLot))
    {
        // The number seems to be reasonable.
        Lot *selectedLot = lots[lotNumber - 1];
        // Include a confidence check to be sure we have the
        // right lot.
        if (selectedLot->getNumero() != lotNumber)
        {
            cout << "Error interno: el lote retornado no corresponde al"
                 << " numero " << lotNumber << endl;
        }
        return selectedLot;
    }
    else
    {
        cout << "El lote numero: " << lotNumber << " no existe.\n";
        return NULL;
    }
}

void Auction::bidFor(int n, Person *p, long v)
{

    Lot *selectedLot = getLot(n);
    if (selectedLot != NULL)
    {
        if (selectedLot->bidFor(new Bid(p, v)))
        {
            cout << "La oferta al lote numero " << n << " fue exitosa.\n";
            cout << *(selectedLot->getHighestBid());
        }
        else
        {
            cout << "El lote numero: " << n << " ya tiene una oferta mayor: "
                 << selectedLot->getHighestBid()->getValue() << endl;
        }
    }
}

int main()
{

    Person *pablo = new Person("Pablo");
    Person *jorge = new Person("Jorge");
    Person *maria = new Person("Maria");

    Auction *auction = new Auction();
    auction->insertLot("computadora");
    auction->insertLot("impresora");
    auction->showLots();

    auction->bidFor(1, pablo, 8000);
    auction->bidFor(3, maria, 3000);
    auction->bidFor(1, jorge, 5000);

    auction->showLots();

    auction->bidFor(2, maria, 3000);

    auction->showLots();

    delete (auction);

    return 0;
}
