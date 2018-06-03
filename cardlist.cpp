#include "cardlist.h"
#include "constant.h"
#include <QPainter>
#include <QDebug>
CardList::CardList()
{
    lastCard = NULL;
    setData(GD_Type,CLIST); //为了方便，就跟CardTemp一样的数据好了  
}

bool CardList::enableCard(Card *card)
{
    if(lastCard == NULL)
    {
        return true;
    }
    else
    {
        Card *t = lastCard;
        if(int(t->getCardNum()) == int(card->getCardNum() + 1))
        {
            return (t->isBlackCard() != card->isBlackCard()); //花色不同
        }
        return false;
    }
}

void CardList::insertCard(Card *card)
{
    qDebug() << "insertCard()";
    cardlist.append(card);
//    for(int i = 0; i < cardlist.size()-1; i ++)
//    {
//        //把前面的卡片都设置成不可拖动
//        cardlist[i]->setFlag(ItemIsMovable,false);
//        cardlist[i]->setTop(false);
//    }
    if(lastCard != NULL)
        card->setZValue(lastCard->zValue() + 1);
    lastCard = card;
    update();
}


void CardList::removeCard() //清理被移除的最后一张纸牌
{
    qDebug() << "removeCard()";
    cardlist.removeLast();
    if(cardlist.size() == 0)
        lastCard = NULL;
    else
    {
        lastCard = cardlist.last();
        qDebug() << "after remove, old cardlist's last Card:" << lastCard->cardNum;
        lastCard->setFlag(ItemIsMovable,true);
        lastCard->setTop(true);
        lastCard->nextCard = NULL;
    }
    update();
}


void CardList::removeCardLink(Card *begincard)  //清理被移除的整个纸牌链
{
    qDebug() << "removeCardLink";
    int beginIndex = 0;
    for(int i = 0; i < cardlist.size(); i++)
    {
        if(cardlist.at(i) == begincard)
        {
            beginIndex = i;
            break;
        }
    }
    if(beginIndex == 0)
    {
        cardlist.clear();
        lastCard = NULL;
    }
    else
    {
        int nums = cardlist.size() - beginIndex;
        while(nums--)
        {
            cardlist.removeLast();
        }
        lastCard = cardlist.last();
        lastCard->setFlag(ItemIsMovable,true);
        lastCard->setTop(true);
        lastCard->nextCard = NULL;
    }

    update();
}

Card *CardList::getLastCard()
{
    return lastCard;
}

void CardList::appendCard(Card *newCard)
{
    cardlist.append(newCard);
}

Card *CardList::getCard(int i)
{
    if(i > cardlist.size())
        return NULL;
    return cardlist[i];
}

int CardList::getCardNums() const
{
    return cardlist.size();
}

void CardList::initCardsZValue()
{
    for(int i = 0; i < cardlist.size(); i++)
    {
        cardlist[i]->setZValue(i+1);
       // qDebug() <<"z:" <<cardlist[i]->zValue();
    }
}

QRectF CardList::boundingRect() const
{
    qreal ht = CARD_HEIGHT + cardlist.size() * CARD_LIST_LEAP;
    qreal hw = CARD_WIDTH;

    return QRectF(0,0,hw,ht);
}

void CardList::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->setPen(QPen(QColor(255,180,100),6));
    painter->drawRect(0,0,CARD_WIDTH,CARD_HEIGHT);
    painter->restore();
}

//***等待更改：返回最后一张纸牌的shape()，用于碰撞检测
QPainterPath CardList::shape() const
{
    QPainterPath path;

    //path.addRect(this->x(),this->y() + cardlist.size()*CARD_LIST_LEAP,CARD_WIDTH,CARD_HEIGHT);
    path.addRect(0,cardlist.size()*CARD_LIST_LEAP,CARD_WIDTH,CARD_HEIGHT);
    return path;

}


