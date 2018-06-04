#include "gamecontroller.h"
#include "card.h"
#include "cardtemp.h"
#include "cardlist.h"
#include "constant.h"
#include "carddest.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QList>
#include <QInputDialog>
#include <algorithm>
#include <QTimer>
#include <QThread>
#include "gameresultdlg.h"
#include <QMessageBox>
GameController::GameController(QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    scene(scene)
{
    usedTime = 0;
    delCardsNum = 0;
    gameStart = false;
    //用于随机洗牌的随机数种子
    std::srand(seed);
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
}

GameController::~GameController()
{

}

void GameController::initCardList()  //初始化手牌区
{
    QString path = QDir::currentPath();

    QDir dir(":/card/images/");
    foreach (QFileInfo img, dir.entryInfoList())
    {
        Card *c = new Card(img.filePath(),*this);
        allcards.append(c);
    }

    //random_shuffle 洗牌
    std::random_shuffle(allcards.begin(),allcards.end());


    int cnt = 0;
    int list_card_num[CARD_LIST_NUM] = {7,7,7,7,6,6,6,6};
    for(int i = 0; i < CARD_LIST_NUM; i++)
    {
        cardList[i] = new CardList();
        for(int j = 0; j < list_card_num[i]; j++)
        {
            cardList[i]->appendCard(allcards[cnt++]);
        }
    }

    scene.addItem(cardList[0]);
    cardList[0]->setPos(CARD_LIST_X,CARD_LIST_Y);


    //放置CardList
    for(int i = 1; i < CARD_LIST_NUM; i++)
    {
        scene.addItem(cardList[i]);
        cardList[i]->setPos(cardList[i-1]->x()+CARD_WIDTH+CARD_LIST_WIDTH_LEAP,CARD_LIST_Y);
        cardList[i]->setZValue(0);
    }
}

void GameController::initCards()
{
    //在CardList上放置初始的卡牌
    for(int i = 0; i < CARD_LIST_NUM; i++)
    {
        Card *card = cardList[i]->getCard(0);
        scene.addItem(card);
        card->setPos(cardList[i]->pos());
        card->cardlist = cardList[i];
        int cardNums = cardList[i]->getCardNums();
        for(int j = 1; j < cardNums; j++)
        {
            card = cardList[i]->getCard(j);
            card->cardlist = cardList[i];
            scene.addItem(card);
            card->setPos(cardList[i]->getCard(j-1)->x(),cardList[i]->getCard(j-1)->y()+CARD_LIST_LEAP);

            cardList[i]->getCard(j-1)->setFlag(QGraphicsItem::ItemIsMovable,false);
        }

        cardList[i]->lastCard = cardList[i]->getCard(cardNums-1);

        cardList[i]->initCardsZValue();
    }

}

void GameController::initCardTemps()
{
    cardTemp[0] = new CardTemp();
    scene.addItem(cardTemp[0]);
    cardTemp[0]->setPos(CARD_TEMP_X,CARD_TEMP_Y);
    for(int i = 1; i < CARD_TEMP_NUM;i++)
    {
        cardTemp[i] = new CardTemp();
        scene.addItem(cardTemp[i]);
        cardTemp[i]->setPos(cardTemp[i-1]->x() + CARD_LIST_WIDTH_LEAP + CARD_WIDTH,CARD_TEMP_Y);
    }
}

void GameController::initCardDests()
{
    cardDest[0] = new CardDest();
    scene.addItem(cardDest[0]);
    cardDest[0]->setPos(CARD_DEST_X,CARD_DEST_Y);
    for(int i = 1; i < CARD_DEST_NUM; i++)
    {
        cardDest[i] = new CardDest();
        scene.addItem(cardDest[i]);
        cardDest[i]->setPos(cardDest[i-1]->x() + CARD_LIST_WIDTH_LEAP + CARD_WIDTH,CARD_DEST_Y);
    }
}


bool GameController::CardToTemp(Card *card, CardTemp *temp)
{
    if(temp->isEmpty() == false || temp == card->cardtemp)
        return false;
    else
    {
        //如果Card来自其他CardTemp，先清除原来的CardTemp信息
        if(card->cardtemp != NULL)
        {
            card->cardtemp->setEmpty();
        }

        //移动到新的CardTemp
        card->cardtemp = temp;
        temp->pushCard(card);
        card->setPos(temp->pos());
        //qDebug() << "card->setPos()";

        //card来自cardlist
        if(card->cardlist != NULL)
        {
            card->cardlist->removeCard();
            card->cardlist = NULL;
        }
        card->setZValue(2);
        return true;
    }
}

bool GameController::CardToList(Card *card, CardList *cardList)
{
    if(card->cardlist == cardList)
    {
        qDebug() << "同一个CardList";
        return false;
    }

    if(cardList->enableCard(card))
    {
        if(card->cardlist != NULL)  //卡牌来自其他牌区
        {
            card->clearCardList();
        }
        if(card->cardtemp != NULL)  //卡牌来自temp区
        {
            card->clearCardTemp();
        }

        //更新位置
        if(cardList->lastCard != NULL)
        {
            QPointF posi= cardList->getLastCard()->pos();
            qDebug() << posi;
            qreal y_new = posi.y() + CARD_LIST_LEAP;
            posi.setY(y_new);
            qDebug() << posi;
            card->setPos(posi);
            card->stackBefore(cardList->lastCard);

            //实现同时拖动多张纸牌
            cardList->getLastCard()->nextCard = card;
        }
        else
        {
            card->setPos(cardList->pos());
            card->setZValue(1);
        }

        //插入cardList
        if(card->nextCard == NULL)
        {
            cardList->insertCard(card);
            card->cardlist = cardList;
        }
        else
        {
            Card *p = card;
            while(p != NULL)
            {
                p->cardlist = cardList;
                cardList->insertCard(p);
                p = p->nextCard;
            }
        }
        return true;
    }
    return false;
}

bool GameController::CardToDest(Card *card, CardDest *carddest)
{
    if(card->cardtemp != NULL)  //CardTemp不能直接移动到CardDest
        return false;

    if(carddest->enableCard(card))
    {
        qDebug() << "CardDest enable this card!";
        if(card->cardlist != NULL)
        {
            card->clearCardList();
        }
        carddest->insertCard(card); //这里setZvalue
        delCardsNum += 1;
        if(delCardsNum == TOTAL_CARDS)
        {
            GameOver();
        }
        return true;
    }

    return false;

}

void GameController::GameOver()
{
    if(!timer->isActive())
    {
        QMessageBox::warning((QWidget*)scene.parent(),"错误","请先开始一场游戏！");
        return;
    }
    qDebug() << "In Game Over";
    timer->stop();

    for(int i = 0; i < CARD_LIST_NUM; i++)
    {
        if(cardList[i] != NULL)
        {
            cardList[i]->setVisible(false);
            delete cardList[i];
        }
        cardList[i] = NULL;
    }
    for(int i = 0; i < CARD_TEMP_NUM; i++)
    {
        if(cardTemp[i] != NULL)
        {
            cardTemp[i]->setVisible(false);
            delete cardTemp[i];
        }
        cardTemp[i] = NULL;
    }
    for(int i = 0; i < CARD_DEST_NUM; i++)
    {
        if(cardDest[i] != NULL)
        {
            cardDest[i]->setVisible(false);
            delete cardDest[i];
            cardDest[i] = NULL;
        }
    }

    for(int i = 0; i < allcards.size(); i++)
    {
        if(allcards[i] != NULL)
        {
            allcards[i]->setVisible(false);
            delete allcards[i];
            allcards[i] = NULL;
        }
    }
    if(delCardsNum == 0)
    {
        score = 0;
    }
    else
    {
        score = SCORE_PER_CARD*delCardsNum - TIME_DEC*usedTime;
    }
    qDebug() << "您的得分：" << score;
    GameResultDlg *dlg = new GameResultDlg((QWidget*)scene.parent());
    dlg->setInfo(usedTime,delCardsNum,score);
    dlg->show();

    allcards.clear();

    usedTime = 0;
    delCardsNum = 0;
}

void GameController::startNewGame()
{
    if(!allcards.empty())
    {
        GameOver();
    }

    int shuffleSeed = QInputDialog::getInt((QWidget*)scene.parent(),tr("请输入一个洗牌随机数种子"),
                                           tr("随机数:"),QLineEdit::Normal,
                                           6);
    qDebug() <<"shuffleSeed:" <<shuffleSeed;
    seed = shuffleSeed;
    initCardList();
    initCards();
    initCardTemps();
    initCardDests();

    timer->start(1000); // 按秒开始计时
}

void GameController::updateTime()
{
    usedTime += 1;
    qDebug() << "当前时间：" << usedTime;
}
