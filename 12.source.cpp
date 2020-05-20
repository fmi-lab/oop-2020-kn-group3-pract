#include <iostream>
#include <string>
#include <vector>

const unsigned MAX_LEN = 80;

class DisplayBehaviour 
{
public:
    virtual void displayText(std::string const& s) = 0;
};

class RightBehaviour : public DisplayBehaviour
{
public:
    void displayText(std::string const& content) override
    {
        unsigned numSpaces = MAX_LEN - content.size();
        for (unsigned i = 0; i < numSpaces; i++) std::cout << " ";
        std::cout << content;
    }
};

class LeftBehaviour : public DisplayBehaviour
{
public:
    void displayText(std::string const& content)
    {
        std::cout << content;
    }
};

class CenterBehaviour : public DisplayBehaviour
{
public:
    void displayiText(std::string const& content)
    {
        unsigned numSpaces = (MAX_LEN - content.size()) / 2;
        for (unsigned i = 0; i < numSpaces; i++) std::cout << " ";
        std::cout << content;
    }
};

struct Entity {
    virtual void display() = 0;

    virtual void setDisplayBehaviour(DisplayBehaviour* db) = 0;

    virtual ~Entity() { }
};

struct Text : Entity
{
    std::string content;
    // displayBehaviour is our strategy, remember Strategy pattern
    DisplayBehaviour *displBehaviour{new LeftBehaviour()};

    Text(std::string const& s) : content{s} { }

    Text() {} // needed for the factory

    void setDisplayBehaviour(DisplayBehaviour* db) override
    {
        if (displBehaviour) 
        {
            // delete the old one so to 
            // avoid a memory leak
            delete displBehaviour;
        }
        displBehaviour = db;
    }

    void display() override
    {
        displBehaviour->displayText(content);
        std::cout << std::endl;
    }

    virtual ~Text()
    {
        delete displBehaviour;
    }
};

struct Paragraph : Entity
{
    std::vector<Entity*> lines;

    Paragraph(std::vector<Entity*> const& ls) : lines{ls} {}

    Paragraph() {} // needed for the factory

    void display() override
    {
        for(Entity* line : lines)
        {
            line->display();   
        }
    }

    void setDisplayBehaviour(DisplayBehaviour* db) override
    {
        for(Entity* line : lines)
        {
            line->setDisplayBehaviour(db);
        }   
    }
    // TODO
};

enum EntityType {
    text,
    paragraph
};

class Factory {
public:
    static Entity* create(EntityType type)
    {
        switch (type)
        {
        case text:
            return new Text();
            break;
        case paragraph:
            return new Paragraph();
            break;
        default:
            return nullptr;
            break;
        }
    }
};

int main() 
{
    std::string s = "some text";
    Paragraph *p = dynamic_cast<Paragraph*>(Factory::create(paragraph));
    p->lines = std::vector<Entity*>{new Text{"line 1"}, 
        new Text{"line 2"}};
    p->display();
    p->setDisplayBehaviour(new RightBehaviour);
    p->display();
}