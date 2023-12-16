#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Animal
{
public:
    virtual ~Animal();
    virtual void speak() const = 0;

protected:
    Animal(string name);
    string getName() const;

private:
    string name;
};

Animal::~Animal() {}

Animal::Animal(string name)
{
    this->name = name;
}

string Animal::getName() const
{
    return this->name;
}

class Dog : public Animal
{
public:
    virtual ~Dog();
    virtual void speak() const;

    Dog(string name);
};

Dog::~Dog() {}

Dog::Dog(string name) : Animal(name) {}

void Dog::speak() const
{
    cout << "    Dog " << this->getName() << " says \"woof\"." << endl;
}

class Sheep : public Animal
{
public:
    virtual ~Sheep();
    virtual void speak() const;

    Sheep(string name);
};

Sheep::~Sheep() {}

Sheep::Sheep(string name) : Animal(name) {}

void Sheep::speak() const
{
    cout << "    Sheep " << this->getName() << " says \"baaa\"." << endl;
}

class Flock
{
public:
    Flock(string dogName);
    virtual ~Flock();
    void addSheep(string name);
    void soundOff() const;

private:
    Dog *dog;
    vector<Sheep *> sheepList;
};

Flock::Flock(string dogName)
{
    this->dog = new Dog{dogName};
}

Flock::~Flock()
{
    delete this->dog;

    for (auto sheep : this->sheepList)
    {
        delete sheep;
    }
}

void Flock::addSheep(string name)
{
    Sheep *new_sheep = new Sheep{name};
    this->sheepList.push_back(new_sheep);
}

void Flock::soundOff() const
{
    cout << "The flock of " << this->sheepList.size() << " sheep speaks!" << endl;
    this->dog->speak();
    for (auto sheep : this->sheepList)
    {
        sheep->speak();
    }

    cout << endl;
}

int main(int argc, char *argv[])
{
    Animal *a = new Dog("doggy");

    Flock *myFlock = new Flock{"Spot"};

    myFlock->soundOff();
    myFlock->addSheep("Daisy");
    myFlock->addSheep("Clover");
    myFlock->addSheep("Estelle");
    myFlock->soundOff();

    delete myFlock;

    myFlock = new Flock{"Rover"};
    myFlock->addSheep("Butch");
    myFlock->addSheep("Jonno");
    myFlock->soundOff();

    delete myFlock;

    return 0;
}