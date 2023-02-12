#ifndef WLOGIN_H
#define WLOGIN_H

#include <QtWidgets>
#include <form/qvklineedit.h>
#include <entitites/operators.h>
#include "widgets/slidingstackedwidget.h"

class WLogin : public QWidget
{
    Q_OBJECT
public:
    explicit WLogin(QWidget *parent = 0);
    ~WLogin();
    void init();
    QVkLineEdit * _pass;

signals:
    void logged();
    void canceled();

private slots:
    void login();
    void cancel();
    void showList(bool);
    void chooseOpe(int);
    void cancelOpe();


private:
    void createUserLoginBox();
    void refreshList();
    void createUserList();
    void createUserBox();
    void createUserLogin();
    void createCloseButton();

    SlidingStackedWidget * loginSlideWidget;
    int idLogin;
    int _width;
    int _y;
    int _areaHeight;

    QString testPass;

    QPropertyAnimation *animationList;
    QPropertyAnimation *animationBox;

    QLabel * error;
    QWidget *logBox;

    QWidget * _userBox;
    QLabel * _user;
    QPushButton *_userlistboton;

    QWidget * _loginBox;


    operators ope;

    QWidget * _listBox;
    QWidget * _listWidg;
    QScrollArea * _areaBox;
    QVBoxLayout * listlayout;
    QButtonGroup * usersButtons;



};

#endif // WLOGIN_H
