#ifndef CARDDEST_H
#define CARDDEST_H
#include <QGraphicsItem>
#include "card.h"
#include "cardtemp.h"
class CardDest : public CardTemp
{
//父类是有个Card的

public:
    CardDest();

    bool enableCard(Card *c);
    void insertCard(Card *c);


};

#endif // CARDDEST_H
