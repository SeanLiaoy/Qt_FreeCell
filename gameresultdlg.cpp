#include "gameresultdlg.h"
#include "ui_gameresultdlg.h"
#include <QMessageBox>
#include <QDesktopServices>

GameResultDlg::GameResultDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameResultDlg)
{
    ui->setupUi(this);
    connect(ui->pushScore,SIGNAL(clicked()),this,SLOT(uploadScore()));

    //连接服务器端的数据库
    qDebug() << QSqlDatabase::drivers();
    qDebug() << QCoreApplication::libraryPaths();

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName("freecelldb");
    db.setHostName("206.189.172.111");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("q9a6z3");
    bool ok = db.open();
    if(ok)
    {
        qDebug() << "成功连接数据库";
    }
    else
    {
        QMessageBox::warning(NULL,"警告","无法连接数据库!");
    }
}

GameResultDlg::~GameResultDlg()
{
    delete ui;
}

void GameResultDlg::setInfo(int times, int cardnums, int score)
{
    ui->label_cards->setText(QString::number(cardnums));
    ui->label_times->setText(QString::number(times));
    ui->label_score->setText(QString::number(score));

    this->times = times;
    this->score = score;
    this->cardnums = cardnums;

}

bool GameResultDlg::uploadScore()
{
    QString name = ui->lineEdit->text();
    if(name.isEmpty())
    {
        QMessageBox::warning(NULL,"警告","昵称不能为空");
        return false;
    }

    QSqlQuery query;
    query = (QSqlQuery)db;
    query.prepare("INSERT INTO gamerank(playername,score,submission_date) VALUES(?,?,now())");
    query.bindValue(0,name);
    query.bindValue(1,score);
    if(query.exec())
    {
        qDebug() << "Your score:" << score;

        query.exec("select last_insert_id()");
        if(query.next())
        {
            id = query.value(0).toInt();
        }

        query.exec("select u.rowNo from ( select id,(@rowNum:=@rowNum+1) as rowNo from gamerank,"
                   "(select (@rowNum :=0)) b"
                   " order by score desc ) u where u.id = " + QString::number(id));
        int rank = -1;
        while(query.next())
        {
            rank = query.value(0).toInt();
        }
        int reply = QMessageBox::information(NULL,"提示","上传成绩成功！\n您的世界排名为：" + QString::number(rank)+ "\n点击OK打开浏览器查看完整排名");
        if(reply == QMessageBox::Ok)
        {
            QDesktopServices::openUrl(QUrl(QLatin1String("http://206.189.172.111:8080/freecellScore/")));
            this->close();
        }

        return true;
    }
    else
    {
        QMessageBox::warning(NULL,"错误","上传成绩失败!");
        qDebug() << query.lastError();
    }
    return false;

}
