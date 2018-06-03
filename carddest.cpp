#include "carddest.h"
#include <QDebug>
#include "constant.h"
CardDest::CardDest()
{
    setData(GD_Type,CDEST);
}

bool CardDest::enableCard(Card *c)
{
    if(this->card == NULL)
    {
        return c->getCardNum() == ACE;
    }
    else
    {
        if(card->cardType == c->cardType && card->cardNum == CardType(c->cardNum-1))
            return true;
    }
    return false;
}

void CardDest::insertCard(Card *c)
{
    qDebug() << "插入到CardDest";
    if(card != NULL)
    {
        card->setVisible(false);
        //delete card;
    }
    card = c;
    card->setPos(this->pos());
    card->setZValue(1);
    card->setFlag(ItemIsMovable,false);
}
