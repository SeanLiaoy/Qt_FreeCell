#include "cardtemp.h"
#include <QPainter>
#include "constant.h"
#include <QColor>
#include <QGraphicsSceneEvent>
#include <QWidget>
#include <QMimeData>

CardTemp::CardTemp()
{
    card = NULL;
    setData(GD_Type,EMPTY);
}

CardTemp::~CardTemp()
{
    if(this->card != NULL)
    {
        card->setVisible(false);
    }
    this->card = NULL;
}

QRectF CardTemp::boundingRect() const
{
    QRectF bound = QRectF(-3,-3,CARD_WIDTH+3,CARD_HEIGHT+3);
    return bound;
}

void CardTemp::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->setPen(QPen(QColor(211,211,211),6));
    painter->drawRect(0,0,CARD_WIDTH,CARD_HEIGHT);
    painter->restore();
}

QPainterPath CardTemp::shape() const
{
    QPainterPath path;
    path.addRect(-3,-3,CARD_WIDTH+3,CARD_HEIGHT+3);
    return path;
}

Card *CardTemp::getCard()
{
    return card;
}

void CardTemp::setFull()
{
    this->setData(GD_Type,FULL);
}

void CardTemp::setEmpty()
{
    this->setData(GD_Type,EMPTY);
    card = NULL;
}

bool CardTemp::isEmpty()
{
    return this->data(GD_Type) == EMPTY;
}

void CardTemp::pushCard(Card *card)
{
    this->card = card;
    setFull();
}


