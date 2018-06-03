#ifndef CARDTEMP_H
#define CARDTEMP_H
#include <QRectF>
#include "constant.h"
#include "card.h"
#include "gamecontroller.h"

//4个暂时存放单张牌的类
class CardTemp : public QGraphicsItem
{
protected:
    Card *card;  //存放的卡牌

public:
    friend class Card;
    friend class GameController;
    CardTemp();
    ~CardTemp();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QPainterPath shape() const;
    Card* getCard();

    void setFull();
    void setEmpty();
    bool isEmpty();

    void pushCard(Card *card);
};

#endif // CARDTEMP_H
