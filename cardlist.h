#ifndef CARDLIST_H
#define CARDLIST_H

#include "card.h"
#include "cardtemp.h"
#include "constant.h"
#include <QList>
//八个卡牌放置区 用链表实现
class CardList : public QGraphicsItem
{

private:
    QList<Card*> cardlist;


public:
    CardList();
    Card *lastCard;

    //以下是用来被GameController调用的
    bool enableCard(Card *card);
    void insertCard(Card *card);   //在尾端插入一张纸牌
    void removeCard();   // 移走CardList后进行的更新

    Card* getLastCard();
    void appendCard(Card* newCard);

    Card* getCard(int i);
    int getCardNums() const;
    void initCardsZValue();

    //移除Card链
    void removeCardLink(Card *begincard);

protected:

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

};

#endif // CARDLIST_H
