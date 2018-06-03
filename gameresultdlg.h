#ifndef GAMERESULTDLG_H
#define GAMERESULTDLG_H

#include <QDialog>
#include <QString>
#include <QtSql>
#include <QSqlDatabase>
namespace Ui {
class GameResultDlg;
}

class GameResultDlg : public QDialog
{
    Q_OBJECT

public:
    explicit GameResultDlg(QWidget *parent = 0);
    ~GameResultDlg();

    int score;
    int times;
    int cardnums;
    int id;
    QString name;
    void setInfo(int times,int cardnums,int score);

private slots:
    bool uploadScore();

private:
    Ui::GameResultDlg *ui;
    QSqlDatabase db;
};

#endif // GAMERESULTDLG_H
