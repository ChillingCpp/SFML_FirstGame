#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>

// sf::Vertex is trivial class

class VertexContiguousDeque : public sf::Drawable
{
public:
    VertexContiguousDeque(sf::PrimitiveType type, size_t vertexCount = 1);
    VertexContiguousDeque(){mPtr = new sf::Vertex[1];}
    ~VertexContiguousDeque();

    size_t size() const;
    sf::Vertex& operator[](size_t index);

    const sf::Vertex& operator[](size_t index) const;
    void resize(size_t vertexCount);
    void initall(sf::PrimitiveType type, size_t vertexCount = 1){
        mPtr = vertexCount ? new sf::Vertex[vertexCount] : new sf::Vertex[1];
        mPrimitiveType = type;
        mSize = vertexCount;
    }
    void append(const sf::Vertex& vertex);
    void prepend(const sf::Vertex& vertex);
    void setPrimitiveType(sf::PrimitiveType type);
    sf::PrimitiveType getPrimitiveType() const;
    void pop_back();
    void pop_front();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void expand(size_t newCapacity, bool centerCopy = true);
    void centerize(size_t deltaSize = 0);

private:
    sf::Vertex* mPtr = nullptr;
    size_t mSize = 0;
    size_t mSpaceFront = 0;
    size_t mSpaceBack = 0;
    sf::PrimitiveType mPrimitiveType = sf::Points;
};
