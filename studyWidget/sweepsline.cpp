#include "sweepsline.h"

sweepItem::sweepItem(int id, QWidget * parent) : QWidget(parent){
    id_serie = id;
    _series.loadData(id_serie);
    actual = false;
    done = false;
    first = false;
    last = false;
    setFixedHeight(30);
    update();
}

sweepItem::~sweepItem()
{
}

void sweepItem::isfirst(){
    first = true;
}

void sweepItem::islast(){
    last = true;
}

int sweepItem::idSweep(){
    return _series.getValue("id_sweeps").toInt();
}

QString sweepItem::name(){
    studydesc sd;
    return sd.getSweepName(_series.getValue("id_sweeps").toInt());
}

int sweepItem::idSerie(){
    return id_serie;
}

void sweepItem::setActual(bool b)
{
    actual = b;
    update();
}

void sweepItem::setDone(bool b)
{
    done = b;
}

void sweepItem::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing );

    int w = width()*0.8;
    if( w > 20)
        w = 20;

    int h = w;

    int centerH = (height()-h)/2;
    int centerW = (width()-w)/2;


    QRect r(centerW,centerH,w,h);

    QColor defaultColor(QColor("#00D4D8"));
    QPen pen(defaultColor, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);


    painter.setPen(pen);

    //Draw First Line
    if(!first)
        painter.drawLine(0,height()/2,((width()-w)/2)-1,height()/2);

    //Draw line2
    if(!last)
        painter.drawLine(((width()-w)/2)+w+2,height()/2,width(),height()/2);

    //Draw Circle
    if(done && !actual)
        painter.setBrush(QBrush(defaultColor));

    painter.setPen(pen);
    painter.drawEllipse(r);



}

sweepsLine::sweepsLine(QWidget * parent) : QWidget(parent)
{
    QWidget * sweepsWidget = new QWidget;
    sweepsWidget->setObjectName("sweepsWidget");
    QHBoxLayout * hl = new QHBoxLayout(this);
    hl->addWidget(sweepsWidget);
    hl->setMargin(0);
    hl->setSpacing(0);

    title = new titlelabel;
    title->setLine(false);
    title->setFixedWidth(650);
    setFixedHeight(90);

    QWidget *sline = new QWidget;
    sline->setFixedWidth(350);
    slinelayout = new QHBoxLayout(sline);
    slinelayout->setSpacing(0);
    slinelayout->setMargin(0);

    QVBoxLayout *vl = new QVBoxLayout(sweepsWidget);

    vl->addWidget(title,0,Qt::AlignCenter | Qt::AlignBottom);
    vl->addWidget(sline,0,Qt::AlignCenter | Qt::AlignTop);
    vl->setSpacing(10);
    vl->setMargin(10);

    numberOfSeries = 0;
}



sweepsLine::~sweepsLine()
{
}

void sweepsLine::clear(){
    foreach(sweepItem * s, sweeps){
        slinelayout->removeWidget(s);
        delete s;
    }
    sweeps.clear();
}


void sweepsLine::setStudy(int id){
    studyID = id;
    clear();
    QList<int> listSweeps = _series.listeIDFromStudy(id);
    numberOfSeries = listSweeps.size();
    int x = 0;
    int actual = 0;
    foreach(int i,listSweeps){
        sweepItem * si = new sweepItem(i);
        if(x == 0)
            si->isfirst();
        if(x == listSweeps.size()-1)
            si->islast();
        series s;
        s.loadData(i);
        if(s.getValue("toqueue").toInt() == 1){
            actual++;
            si->setDone(true);
        }
        slinelayout->addWidget(si);
        sweeps.append(si);
        x++;
    }
    if (numberOfSeries > actual)
        setActual(actual);
}

int sweepsLine::actualId()
{
    return _actualId;
}

int sweepsLine::actual()
{
    return _actual;
}

void sweepsLine::setActual(int i)
{
    _actual = i;
    if (i < sweeps.size()){
        foreach(sweepItem* s,  sweeps)
            s->setActual(false);
        sweepItem* actSweep = sweeps.at(i);
        _actualId = actSweep->idSerie();
        actSweep->setActual(true);
        actSweep->setDone(true);
        title->setText(actSweep->name());

    }
}

int sweepsLine::sweepsSize()
{
    return numberOfSeries;
}

bool sweepsLine::isLast(){
    if (_actual+1 <  numberOfSeries )
        return false;
    else return true;
}

void sweepsLine::next(){
    if(!isLast())
        setActual(_actual+1);
}

