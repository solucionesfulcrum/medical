#include "touchcombobox.h"

TouchButton::TouchButton(const QString &text, QWidget *parent, Qt::WindowFlags f)
    :QLabel(text, parent, f){
    setObjectName("TouchButton");
    setAlignment(Qt::AlignCenter);
    id = 0;
}

void TouchButton::mousePressEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton ){
        t.restart();
    }
}

void TouchButton::mouseReleaseEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton ){
        qDebug() << t.elapsed();
        if (t.elapsed() < 500){
            sendValue(text());
            sendId(id);
        }
    }
}

void TouchButton::setId(int i){
    id = i;
}
int TouchButton::getId(){
    return id;
}

void TouchButton::setChecked(bool b){
    setProperty("checked",b);
    style()->unpolish(this);
    style()->polish(this);
    update();
}



TouchComboBox::TouchComboBox(QWidget *parent) : QLineEdit(parent)
{
    setReadOnly(true);
    setObjectName("TouchComboBox");
    pop = NULL;
}

TouchComboBox::~TouchComboBox()
{

}

void TouchComboBox::mousePressEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton ){
        //accesor::startEffect();
        refresh();
        emit clicked();

        pop->show();
        pop->setModal(true);
        animation->setDuration(200);
        int mx = accesor::mw->x()+(accesor::mw->width()-pop->width())/2;
        int my = accesor::mw->y()+(accesor::mw->height()-pop->height())/2;

        animation->setStartValue(QRect(mx,accesor::mw->y()+5,pop->width(),pop->height()));
        animation->setEndValue(QRect(mx,my,pop->width(),pop->height()));
        animation->start();
    }

}

void TouchComboBox::setItems(QStringList l){
    items = l;
    createPopUpWidget();
}

void TouchComboBox::refresh(){
    int x =0;
    foreach(TouchButton *t,touchButtons){
        if(text() == t->text()){
            t->setChecked(true);
            area->verticalScrollBar()->setValue((x-2)*81);
        }
        else t->setChecked(false);
        x++;
    }
    area->update();
}

void TouchComboBox::createPopUpWidget(){
    if(pop != NULL)
        delete pop;


    touchButtons.clear();
    pop = new QDialog;
    pop->setWindowFlags(Qt::WindowDoesNotAcceptFocus | Qt::Tool |
                        Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    pop->setObjectName("pop");
    pop->setModal(true);
    animation = new QPropertyAnimation(pop,"geometry");

    QVBoxLayout * lay = new QVBoxLayout(pop);
    lay->setMargin(3);

    QWidget * tb = new QWidget();
    tb->setObjectName("areaTransBg");
    area = new QScrollArea();
    area->setFixedWidth(500);
    area->setWidget(tb);
    area->setWidgetResizable(true);
    area->setObjectName("areaTransBg");
    area->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(area->viewport(),QScroller::LeftMouseButtonGesture);


    lay->addWidget(area);

    QPushButton * cancel = new QPushButton(QIcon(":/icon/res/img/form/cancel.png"),tr("Regresar"));
    connect(cancel,SIGNAL(clicked()),this,SLOT(cancel()));
    cancel->setIconSize(QSize(32,32));
    cancel->setObjectName("redButton");
    cancel->setFixedSize(160,60);
    lay->addWidget(cancel,0,Qt::AlignCenter);
    lay->addSpacing(10);


    QVBoxLayout *l = new QVBoxLayout(tb);
    l->setAlignment(Qt::AlignTop);
    l->setMargin(0);
    l->setSpacing(1);
    int x = 0;
    foreach(QString s, items){
        TouchButton *tb = new TouchButton(s);
        connect(tb,SIGNAL(sendValue(QString)),this,SLOT(save(QString)));
        tb->setText(s);
        tb->setFixedHeight(80);
        touchButtons.append(tb);
        l->addWidget(tb);
        x++;
    }
    area->verticalScrollBar()->setMaximum(touchButtons.size()*81);
    int height = x*81;
    if(height > 550)
        height = 550;
    area->setFixedHeight(height);

}

void TouchComboBox::cancel(){
    //accesor::stopEffect();
    pop->hide();
}

void TouchComboBox::save(QString s){
    accesor::stopEffect();
    setText(s);
    pop->hide();
}
