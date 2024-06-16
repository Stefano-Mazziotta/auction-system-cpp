#include "auction-system.h"

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

    output << person.name;
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

        //  store in file about this fact
        // search if the lot exists in file
        // yes - replace highest bid
        //  no - add the lot to file

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

    int highestBidAmount = 0;
    if (lot.highestBid)
    {
        highestBidAmount = lot.highestBid->getAmount();
    }

    output << "Lote " << lot.description << ": " << "\n \t Numero: " << lot.lotId
           << "\n \t Oferta: " << highestBidAmount << endl;
    return output;
}

Auction::Auction()
{
    nextLot = 1;

    // create file .txt to save auction data
    // auction-timestamp.txt
    // lot.id - lot.description - highestBid.name - highestBid.amount
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
        Lot *lot = lots[lotNumber - 1];

        // Include a confidence check to be sure we have the right lot.
        if (lot->getLotId() != lotNumber)
        {
            cout << "Error interno: el lote retornado no corresponde al" << " numero " << lotNumber << endl;
        }
        return lot;
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

void Auction::loadLotsFromFile(const string &filename)
{
    ifstream infile(filename);
    if (!infile)
    {
        cerr << "Could not open file " << filename << " for reading.\n";
        return;
    }

    string line;
    while (getline(infile, line))
    {
        int id;
        // istringstream iss(line);
        if (line.length() > 0)
        {
            string lot = line.substr(2, line.length());
            insertLot(lot);
        }
    }
    infile.close();
}

void Auction::saveLotsToFile(const string &filename)
{
    ofstream outfile(filename);
    if (!outfile)
    {
        cerr << "Could not open file " << filename << " for writing.\n";
        return;
    }

    for (const auto &lot : lots)
    {
        if (lot->getHighestBid() == nullptr)
        {
            outfile << lot->getLotId() << " " << lot->getDescription() << endl;
        }
    }
    outfile.close();
}

void Auction::saveBidsToFile(const string &filename)
{
    ofstream outfile(filename);
    if (!outfile)
    {
        cerr << "Could not open file " << filename << " for writing.\n";
        return;
    }

    for (const auto &lot : lots)
    {
        if (lot->getHighestBid() != nullptr)
        {
            outfile << lot->getLotId() << " " << lot->getDescription() << " "
                    << lot->getHighestBid()->getBidder()->getName() << " "
                    << lot->getHighestBid()->getAmount() << endl;
        }
    }
    outfile.close();
}

int main()
{
    Person *pablo = new Person("Pablo");
    Person *jorge = new Person("Jorge");
    Person *maria = new Person("Maria");

    Auction *auction = new Auction();

    auction->loadLotsFromFile("lots.txt");
    auction->showLots();

    auction->doOffer(1, pablo, 8000);
    auction->doOffer(1, jorge, 5000);
    auction->doOffer(2, maria, 5000);

    auction->saveLotsToFile("lots.txt");
    auction->saveBidsToFile("auctions.txt");

    delete (pablo);
    delete (jorge);
    delete (maria);

    return 0;
}