#ifndef CONSTANT_H
#define CONSTANT_H


#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900

#define CARD_LIST_Y 400
#define CARD_LIST_X 40

#define TOTAL_CARDS 52

#define CARD_WIDTH 105
#define CARD_HEIGHT 150



#define CARD_LIST_LEAP 25  //牌区两张牌之间的间隔
#define CARD_LIST_WIDTH_LEAP 30  //不同牌区的间距
#define CARD_LIST_NUM 8  //多少个CARD_LIST

#define CARD_TEMP_NUM 4
#define CARD_TEMP_X 40
#define CARD_TEMP_Y 10

#define CARD_DEST_NUM 4
#define CARD_DEST_X 600
#define CARD_DEST_Y 10

#define SCORE_PER_CARD 1000  //每张卡的得分
#define TIME_DEC 10  //时间得分惩罚
enum GameObjectsData {
    GD_Type
};

enum CardTempTypes {
    EMPTY,   //可以被放入纸牌
    FULL,     //不可以放入纸牌
    CLIST,   //Card List区
    CDEST  //终点区
};

enum CardType{
    HEART,   //红桃
    SPADE,   //黑桃
    CLUB,    //梅花
    DIAMOND  //方块

};

enum CardNum{   //纸牌牌号
    ACE,  // A
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NIGHT,
    TEN,
    JACK,  //J
    QUEEN,  //Q
    KING    //KING
};



#endif // CONSTANT_H
