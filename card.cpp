#include "card.h"
#include <QPainter>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QApplication>
#include "cardtemp.h"
#include <QMimeData>
#include "constant.h"
#include "cardlist.h"
#include "carddest.h"
void Card::setInfo(const QString &path)
{
    //从图片文件的路径中提取出纸牌的花色和大小
    QStringList strlist = path.split('/');
    QString filename = strlist.last();
    QString num,color;
    //qDebug() << filename;
    int i;
    for(i = 0; i < filename.size(); i++)
    {
        if(filename[i] == '_')
        {
            break;
        }
        num = num + filename[i];
    }
    for(i = i+1; i < filename.size(); i++)
    {
        if(filename[i] == '.')
        {
            break;
        }
        color = color + filename[i];
    }

    int no = num.toInt();
    int co = color.toInt();

    cardType = CardType(co);
    cardNum = CardNum(no-1);

}

Card::Card(const QString &path,GameController &controller) : controller(controller)
{
    setInfo(path); //设置牌号花色信息
    if(cardType == CardType::HEART)
        qDebug() << "cardType is Heart";
    if(cardNum == CardNum::ACE)
        qDebug() << "cardNum is ACE";
    pic = new QPixmap;
    pic->load(path);
    isTop = true;  //默认置顶
    cardtemp = NULL;
    cardlist = NULL;
    nextCard = NULL;
    //frozen = false;
    setFlag(ItemIsMovable,true);  // 取消 : setFalg(ItemIsMovable,false);
    setFlag(ItemSendsGeometryChanges);
}


QRectF Card::boundingRect() const
{
    QRectF bound = QRectF(0,0,pic->width(),pic->height());
    return bound;
}

void Card::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->save();
    painter->drawPixmap(0,0,pic->width(),pic->height(),*pic);
    painter->restore();
}

QPainterPath Card::shape() const
{
    QPainterPath path;
    path.addRect(0,0,pic->width(),pic->height());
    return path;
}

bool Card::advance()
{
    if (newPos == pos())
       return false;
    qDebug() << "advance()";
    //setPos(newPos);
    //handleCollisions();
    return true;
}



void Card::setTop(bool t)
{
    isTop = t;
}

bool Card::isBlackCard()
{
    if(cardType == CardType::HEART || cardType == CardType::DIAMOND)
        return false;
    return true;
}


//***这个函数的功能部分要转移到GameControll中，只留下检测***
bool Card::handleCollisions() //碰撞检测
{
    QList<QGraphicsItem *> collisions = collidingItems();

    foreach (QGraphicsItem *collidingItem, collisions)
    {
        if(collidingItem->data(GD_Type) == EMPTY)
        {
            //控制，使得卡牌放入Temp
            qDebug() << "检测碰撞到CardTemp";
            if(this->nextCard != NULL)
                return false;
            return controller.CardToTemp(this,dynamic_cast<CardTemp*>(collidingItem));

        }
        else if(collidingItem->data(GD_Type) == CLIST)
        {
            //移动到CardList
            qDebug() << "碰撞到CardList";
            return controller.CardToList(this,dynamic_cast<CardList*>(collidingItem));
        }
        else if(collidingItem->data(GD_Type) == CDEST)
        {
            qDebug() << "碰撞到CardDest";
            if(this->nextCard != NULL)
                return false;
            return controller.CardToDest(this,dynamic_cast<CardDest*>(collidingItem));
        }
    }
    return false;
}

void Card::clearCardTemp()
{
    if(cardtemp != NULL)
    {
        cardtemp->setData(GD_Type,EMPTY);
        cardtemp->card = NULL;
        cardtemp = NULL;
    }
}

void Card::clearCardList()
{
    if(cardlist != NULL)
    {
        if(this->nextCard == NULL)
        {
            cardlist->removeCard();
            cardlist = NULL;
        }
        else
        {
            //清除CardLink
            cardlist->removeCardLink(this);
        }
    }
}

CardNum Card::getCardNum() const
{
    return this->cardNum;
}

void Card::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton || this->isTop == false)
    {
        event->ignore();
        return;
    }
    oldZValue = this->zValue();
    oldPos = pos();

    this->setZValue(2000);
    //qDebug() << "this->nextCard:" << nextCard;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Card::updateCardLinkPosi()
{
    if(this->nextCard != NULL)
    {
        Card *p = this,*pn = nextCard;
        QPointF posi = this->pos();
        while(pn != NULL)
        {
            qreal y_new = posi.y() + CARD_LIST_LEAP;
            posi.setY(y_new);
            pn->setPos(posi);
            pn->setZValue(p->zValue() + 1);
            p = pn;
            pn = pn->nextCard;
        }
    }
}

void Card::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    if(handleCollisions() == false)  //没把牌移到了其他地方
    {
        setPos(oldPos);
        this->setZValue(oldZValue);
        updateCardLinkPosi();
    }
    if(cardtemp != NULL)
        this->setZValue(oldZValue);
    QGraphicsItem::mouseReleaseEvent(event);
}

QVariant Card::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch(change)
    {
        case ItemPositionChange:
        {
            newPos = this->pos();
            if(this->x() < 0)
            {
                newPos.setX(0);
            }
            else if(this->x() > (this->scene()->width() - pic->width()))
            {
                newPos.setX(this->scene()->width() - pic->width());
            }

            if(this->y() < 0)
            {
                newPos.setY(0);
            }
            else if(this->y() > (this->scene()->height() - this->pic->height()))
            {
                newPos.setY(this->scene()->height() - pic->height());
            }
            updateCardLinkPosi();
        }
        break;
    default:
        break;

    }
    return QGraphicsItem::itemChange(change, value);
}

