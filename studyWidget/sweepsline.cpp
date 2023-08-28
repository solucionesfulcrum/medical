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


    /*if(done && !actual)
        painter.setBrush(QColor("#00D4D8"));        */

    QPen pen1(defaultColor, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    pen1.setColor(defaultColor);                            //Circle

    if(done)
        painter.setBrush(QColor("#00D4D8"));

    if(actual)        
        painter.setBrush(QColor("#FF5454"));

    painter.setPen(pen1);
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

//---------------------------------------
//  CR: 02/07/23
    title = new titlelabel("",QFont("Arial",16),0);
//---------------------------------------
    title->setLine(false);
//---------------------------------------
//  CR: 25/01/23
    title->setFixedWidth(800);
    title->setFixedHeight(70);

    setFixedHeight(110);

    QWidget *sline = new QWidget;    
    sline->setFixedWidth(450);
    slinelayout = new QHBoxLayout(sline);
    slinelayout->setSpacing(0);
    slinelayout->setMargin(0);

    QVBoxLayout *vl = new QVBoxLayout(sweepsWidget);
    vl->setSpacing(0);

    /*vl->addWidget(title,0,Qt::AlignCenter | Qt::AlignBottom);
    vl->setSpacing(10);
    vl->setMargin(10);
    vl->addWidget(sline,0,Qt::AlignCenter | Qt::AlignBottom);*/


    vl->addWidget(title,0,Qt::AlignCenter | Qt::AlignTop);
    vl->addWidget(sline,0,Qt::AlignCenter|Qt::AlignBottom);

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

//----------------------------------------------------------
// CR: 01/02/23
bool sweepsLine::setStudy(int id){
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
        //if(s.getValue("toqueue").toInt() == 1){
        if(s.getValue("capture").toInt() == 1){
            actual++;
            si->setDone(true);
        }
        slinelayout->addWidget(si);
        sweeps.append(si);
        x++;
    }
    if (numberOfSeries > actual)
        setActual(actual);

    if(numberOfSeries==actual){
        setActual(actual-1);
        return true;
    }

    return false;
}
//----------------------------------------------------------
bool sweepsLine::IsCompleted(void)
{
    series s;
    QList<int> listSweeps = _series.listeIDFromStudy(studyID);

    int m=0;

    foreach(int i,listSweeps){
        s.loadData(i);
        if(s.getValue("capture").toInt() == 1)
            m++;
    }

    if(m==(listSweeps.size()))
        return true;
    else
        return false;
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
    int n = sweeps.size();
    if (i < n){
        foreach(sweepItem* s,  sweeps)
            s->setActual(false);
        sweepItem* actSweep = sweeps.at(i);
        _actualId = actSweep->idSerie();
        actSweep->setActual(true);
        actSweep->setDone(true);
        QString ss= actSweep->name();
//------------------------------------------------------------------------------
//      CR: 19/05/23        
        title->setText("PASO "+QString::number(i+1)+"\n"+actSweep->name());
//------------------------------------------------------------------------------
    }
}

int  sweepsLine::getActual(int i)
{
    sweepItem* actSweep = sweeps.at(i);
    return actSweep->idSerie();
}

int sweepsLine::sweepsSize()
{
    return numberOfSeries;
}

bool sweepsLine::isLast(){
    if ( (_actual+1) <  numberOfSeries )
        return false;
    else return true;
}

bool sweepsLine::islastBefore(){
    if ( (_actual+1) <  (numberOfSeries-1) )
        return false;
    else return true;
}


//-----------------------------------------
//CR: 18/01/23
void sweepsLine::next(){
    if(!isLast())
        setActual(_actual+1);
}

void sweepsLine::prev(){
    if(_actual!=0)
        setActual(_actual-1);
}

