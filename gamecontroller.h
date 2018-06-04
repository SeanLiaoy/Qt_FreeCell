#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "constant.h"
#include <QObject>
#include <QTime>
#include <QTimer>
#include <QThread>
class Card;
class CardTemp;
class CardList;
class QGraphicsScene;
class CardTemp;
class CardDest;
class CardDest;

class GameController : public QObject
{
    Q_OBJECT


public:
    GameController(QGraphicsScene &scene, QObject *parent = 0);
    ~GameController();

    void initCardList();
    void initCards();
    void initCardTemps();
    void initCardDests();
    bool CardToTemp(Card *card,CardTemp *temp);
    bool TempToCard(CardTemp *temp,Card *card);
    bool CardToList(Card *card,CardList *cardlist);
    bool CardToDest(Card *card,CardDest *carddest);


    void GameOver();
    void startNewGame();
private:
    QGraphicsScene &scene;
    CardList* cardList[CARD_LIST_NUM];
    CardTemp* cardTemp[CARD_TEMP_NUM];
    CardDest* cardDest[CARD_DEST_NUM];
    QList<Card*> allcards;
    qreal cards_num_leave = 0;
    int seed = 6;  //用于洗牌的随机数种子

    bool gameStart;
    int delCardsNum;  //消灭的卡牌数
    int score;    //得分
    int usedTime;  //游戏用时(秒)
    QTimer *timer;
    QTime *time;

private slots:
    void updateTime();
};

#endif // GAMECONTROLLER_H
