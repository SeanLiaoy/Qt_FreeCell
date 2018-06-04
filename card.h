#ifndef CARD_H
#define CARD_H
#include <QGraphicsItem>
#include <QPixmap>
#include <QString>
#include <QGraphicsSceneEvent>
#include "constant.h"
#include "gamecontroller.h"
#include "cardtemp.h"
#include "cardlist.h"
class Card : public QGraphicsItem
{
private:

    QPointF oldPos;
    qreal oldZValue;

    void setInfo(const QString &path);  //根据文件名设置花色等信息
public:
    QPixmap *pic;
    CardTemp *cardtemp;
    CardList *cardlist;
    CardNum cardNum;   //牌号
    CardType cardType;  //牌的花色
    Card *nextCard;  //在CardList中形成的Card链，为了实现同时拖动多张卡片
    GameController &controller;


    Card(const QString &path,GameController &controller);

    CardNum getCardNum() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    bool advance(); //called from GraphWidget::timerEvent()

    bool isBlackCard();  //花色是黑色的

    bool handleCollisions();  //拖拽鼠标松开时进行碰撞检测

    void clearCardTemp();   //从CardTemp上移出
    void clearCardList();   //从CardList上移出

    void updateCardLinkPosi(); //**移到GameController
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

};

#endif // CARD_H
