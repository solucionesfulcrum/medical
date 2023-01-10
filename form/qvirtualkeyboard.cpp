#include "qvirtualkeyboard.h"

QVirtualKeyboard* QVirtualKeyboard::vk = NULL;

QVirtualKeyboard::QVirtualKeyboard(QWidget * parent)
    :QWidget(parent)
{
    _visibleKeyboard = true;
    animation = new QPropertyAnimation(this, "geometry");
    setObjectName("keyboard");
    setWindowFlags(Qt::WindowDoesNotAcceptFocus | Qt::Tool |  Qt::Popup |
                   Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    edit = new QLineEdit;
//------------------------------------------------
//  CR: 08/01/23
    edit->setEchoMode(QLineEdit::Password);
//------------------------------------------------
    edit->setFixedHeight(100);
    edit->setAlignment(Qt::AlignCenter);
    connect(edit,SIGNAL(textChanged(QString)),this,SLOT(doSend()));
    edit->setFocus();
    keyboard = new QButtonGroup;
    connect(keyboard,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(writeletter(QAbstractButton*)));
    QWidget *kb = new QWidget;
    QGridLayout * l = new QGridLayout(kb);

    int row = 0;
    l->addWidget(createButton("1"),row,0);
    l->addWidget(createButton("2"),row,1);
    l->addWidget(createButton("3"),row,2);
    l->addWidget(createButton("4"),row,3);
    l->addWidget(createButton("5"),row,4);
    l->addWidget(createButton("6"),row,5);
    l->addWidget(createButton("7"),row,6);
    l->addWidget(createButton("8"),row,7);
    l->addWidget(createButton("9"),row,8);
    l->addWidget(createButton("0"),row,9);

    row++;
    l->addWidget(createButton("Q"),row,0);
    l->addWidget(createButton("W"),row,1);
    l->addWidget(createButton("E"),row,2);
    l->addWidget(createButton("R"),row,3);
    l->addWidget(createButton("T"),row,4);
    l->addWidget(createButton("Y"),row,5);
    l->addWidget(createButton("U"),row,6);
    l->addWidget(createButton("I"),row,7);
    l->addWidget(createButton("O"),row,8);
    l->addWidget(createButton("P"),row,9);

    row++;
    l->addWidget(createButton("A"),row,0);
    l->addWidget(createButton("S"),row,1);
    l->addWidget(createButton("D"),row,2);
    l->addWidget(createButton("F"),row,3);
    l->addWidget(createButton("G"),row,4);
    l->addWidget(createButton("H"),row,5);
    l->addWidget(createButton("J"),row,6);
    l->addWidget(createButton("K"),row,7);
    l->addWidget(createButton("L"),row,8);
    l->addWidget(createButton("Ñ"),row,9);

    row++;
    l->addWidget(createButton("Z"),row,0);
    l->addWidget(createButton("X"),row,1);
    l->addWidget(createButton("C"),row,2);
    l->addWidget(createButton("V"),row,3);
    l->addWidget(createButton("B"),row,4);
    l->addWidget(createButton("N"),row,5);
    l->addWidget(createButton("M"),row,6);
    l->addWidget(createButton(","),row,7);
    l->addWidget(createButton("."),row,8);
    l->addWidget(createButton("_"),row,9);

    row++;
    l->addWidget(createButton(";"),row,0);
    l->addWidget(createButton(":"),row,1);
    l->addWidget(createButton("-"),row,2);
    QPushButton * space = new QPushButton(" ");
    space->setFixedHeight(80);

    keyboard->addButton(space);
    l->addWidget(space,row,3,1,4);
    l->addWidget(createButton("+"),row,7);
    l->addWidget(createButton("*"),row,8);
    l->addWidget(createButton("@"),row,9);

    QPushButton *backspace = new QPushButton(QIcon(":/icon/res/img/keyboard/backspace.png"),"");
    connect(backspace,SIGNAL(clicked()),this,SLOT(doBackspace()));
    backspace->setIconSize(QSize(32,32));
    backspace->setFixedHeight(80);

    QPushButton *delAll = new QPushButton(QIcon(":/icon/res/img/keyboard/remove.png"),"");
    connect(delAll,SIGNAL(clicked()),this,SLOT(remove()));
    delAll->setFixedHeight(80);
    delAll->setIconSize(QSize(32,32));

    QPushButton *cancel = new QPushButton(tr("Cancelar"));
    connect(cancel,SIGNAL(clicked()),this,SLOT(cancel()));
    cancel->setFixedHeight(80);
    cancel->setObjectName("redButton");

    QPushButton *enter = new QPushButton(tr("Validar"));
    connect(enter,SIGNAL(clicked()),this,SLOT(validate()));
    enter->setFixedHeight(258);
    enter->setObjectName("greenButton");

    l->addWidget(backspace,0,10);
    l->addWidget(delAll,0,11);
    l->addWidget(enter,1,10,3,2);
    l->addWidget(cancel,4,10,1,2);
    l->setSpacing(8);
    l->setMargin(10);

    QWidget *kbw = new QWidget();
    kbw->setObjectName("keyboard");


    QVBoxLayout *layout = new QVBoxLayout(kbw);
    layout->addWidget(edit);
    layout->addWidget(kb);
    layout->setSpacing(1);
    layout->setMargin(0);

    QVBoxLayout *ms = new QVBoxLayout(this);
    ms->addWidget(kbw);
    ms->setSpacing(0);
    ms->setMargin(0);
    setFixedWidth(accesor::mw->width());
}

void QVirtualKeyboard::setVisibleKeyboard(bool b){
    _visibleKeyboard = b;
}

bool QVirtualKeyboard::visibleKeyBoard(){
    return _visibleKeyboard;
}

void QVirtualKeyboard::show(){
    //qDebug() << "Show Keyboard" <<  _visibleKeyboard;
    if(_visibleKeyboard){
        //qDebug() << "Show Keyboard" <<  _visibleKeyboard;
        QWidget::show();
    }
}

void QVirtualKeyboard::showEvent(QShowEvent * event){
    //accesor::startEffect();
    setFixedWidth(accesor::mw->width());
    animation->stop();
    disconnect(animation, SIGNAL(finished()),this,SLOT(close()));
    animation->setDuration(100);
    animation->setStartValue(QRect(accesor::mw->x(), accesor::mw->height()+height()+accesor::mw->y(), 0, 0));
    animation->setEndValue(QRect(accesor::mw->x(), accesor::mw->height()-height()+accesor::mw->y(), 0, 0));
    animation->start();
}

//void QVirtualKeyboard::focu


void QVirtualKeyboard::finish(){
    //accesor::stopEffect();
    animation->stop();
    animation->setDuration(100);
    animation->setEndValue(QRect(accesor::mw->x(), accesor::mw->height()+height()+accesor::mw->y(), 0, 0));
    animation->setStartValue(QRect(accesor::mw->x(), accesor::mw->height()-height()+accesor::mw->y(), 0, 0));
    animation->start();
    connect(animation, SIGNAL(finished()),this,SLOT(close()));
}

void QVirtualKeyboard::setText(QString s){
    edit->setText(s);
    entervalue = s;
}

void QVirtualKeyboard::setLineEdit(QLineEdit *e){
    toEdit = e;
}

QVirtualKeyboard::~QVirtualKeyboard(){
}

QPushButton * QVirtualKeyboard::createButton(QString s){
    QPushButton * pb = new QPushButton(s);
    pb->setFixedHeight(80);
    keyboard->addButton(pb);
    return pb;
}

void QVirtualKeyboard::remove(){
      edit->setText("");
}


void QVirtualKeyboard::doSend(){
    //emit send(edit->text());
      toEdit->setText(edit->text());
}

void QVirtualKeyboard::doBackspace(){
    edit->setText(edit->text().left(edit->text().size()-1));
}

void QVirtualKeyboard::validate(){
    //emit send(edit->text());
    toEdit->setText(edit->text());
    finish();
}

void QVirtualKeyboard::cancel(){
    edit->setText(entervalue);
    //emit send(edit->text());
    toEdit->setText(edit->text());
    finish();
}


void QVirtualKeyboard::writeletter(QAbstractButton *b){
    edit->setText(edit->text()+b->text());
}
