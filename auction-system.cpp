#include "subasta.h"

Person::Person(string name)
{

    this->name = name;
}

Person::~Person(){};

string Person::getName()
{

    return this->name;
}

ostream &operator<<(ostream &output, const Person &person)
{

    output << "Person's name: " << person.name;
    return output;
}

Bid::Bid(Person *person, long amount)
{

    this->bidder = person;
    this->amount = amount;
}

Bid::~Bid()
{
    delete (bidder);
}

long Bid::getAmount()
{

    return this->amount;
}

Person *Bid::getBidder()
{

    return this->bidder;
}

ostream &operator<<(ostream &output, const Bid &bid)
{

    output << "Oferta: " << *(bid.bidder) << " - Valor de oferta: " << bid.amount
           << endl;
    return output;
}

Lot::Lot(int lotId, string description)
{

    this->highestBid = NULL;
    this->lotId = lotId;
    this->description = description;
}

Lot::~Lot()
{
    delete (this->highestBid);
}

bool Lot::bidFor(Bid *bid)
{

    if ((highestBid == NULL) || (bid->getAmount() > highestBid->getAmount()))
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

string Lot::getDescription()
{

    return this->description;
}

int Lot::getLotId()
{

    return this->lotId;
}

Bid *Lot::getHighestBid()
{

    return this->highestBid;
}

ostream &operator<<(ostream &output, const Lot &lot)
{

    output << "Lote " << lot.description << ": " << "\n \t Numero: " << lot.lotId
           << "\n \t Oferta: " << lot.highestBid << endl;
    return output;
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
        if (selectedLot->getLotId() != lotNumber)
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

void Auction::doOffer(int lotId, Person *person, long amount)
{

    Lot *selectedLot = getLot(lotId);
    if (selectedLot != NULL)
    {
        if (selectedLot->bidFor(new Bid(person, amount)))
        {
            cout << "La oferta al lote numero " << lotId << " fue exitosa.\n";
            cout << *(selectedLot->getHighestBid());
        }
        else
        {
            cout << "El lote numero: " << lotId << " ya tiene una oferta mayor: "
                 << selectedLot->getHighestBid()->getAmount() << endl;
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

    auction->doOffer(1, pablo, 8000);
    auction->doOffer(3, maria, 3000);
    auction->doOffer(1, jorge, 5000);

    auction->showLots();

    auction->doOffer(2, maria, 3000);

    auction->showLots();

    delete (auction);

    return 0;
}
