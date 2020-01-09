#include "datebox.h"

datebox::datebox(QWidget * parent) : QWidget(parent)
{
    monthname << tr("Enero")
              << tr("Febrero")
              << tr("Marzo")
              << tr("Abril")
              << tr("Mayo")
              << tr("Junio")
              << tr("Julio")
              << tr("Agosto")
              << tr("Septiembre")
              << tr("Octubre")
              << tr("Noviembre")
              << tr("Diciembre");



    QWidget *dbox = new QWidget;
    dbox->setObjectName("datebox");
    QHBoxLayout * l = new QHBoxLayout(this);
    l->setSpacing(0);
    l->setMargin(0);
    l->addWidget(dbox);

    QHBoxLayout * layout = new QHBoxLayout(dbox);
    layout->setSpacing(5);
    layout->setMargin(0);




    QStringList monthList;
    for(int x = 1 ; x < 13; x++)
        monthList <<  monthname.at(x-1);

    QStringList yearList;
    for(int x = QDate::currentDate().year()+1 ; x > 1899 ; x--)
        yearList << QString::number(x);

    day = new TouchComboBox;
    day->setPlaceholderText(tr("Día"));



    month = new TouchComboBox;
    month->setItems(monthList);
    month->setPlaceholderText(tr("Mes"));
    connect(month,SIGNAL(textChanged(QString)),this,SLOT(updateDay(QString)));

    year = new TouchComboBox;
    year->setItems(yearList);
    year->setPlaceholderText(tr("Año"));
    connect(year,SIGNAL(textChanged(QString)),this,SLOT(updateDay(QString)));

    layout->addWidget(day);
    layout->addWidget(month);
    layout->addWidget(year);
    updateDay("");


}

datebox::~datebox()
{

}

void datebox::updateDay(QString s){
    int m = monthname.indexOf(month->text())+1;
    int numday = 31;

    // jchang 18/10/2019. Se anadio validaion del mes
    // en caso de no tener valor, se selecciona el primer valor de la lista
    if (m==0) return;

    switch (m){
    case 1: numday = 31; break;
    case 3: numday = 31; break;
    case 4: numday = 30; break;
    case 5: numday = 31; break;
    case 6: numday = 30; break;
    case 7: numday = 31; break;
    case 8: numday = 31; break;
    case 9: numday = 30; break;
    case 10: numday = 31; break;
    case 11: numday = 30; break;
    case 12: numday = 31; break;
    }

    if(m == 2){
        if( QDate::isLeapYear((year->text().toInt())) )
            numday = 29;
        else numday = 28;
    }

    dayList.clear();
    for(int x = 1 ; x <= numday; x++)
        dayList << QString("%1").arg(x, 2, 10, QChar('0'));
    day->setItems(dayList);

    if(day->text().toInt() > numday ){
        day->setText(QString::number(numday));
    }
}

void datebox::setDate(QDate d){
    day->setText(d.toString("dd"));
    month->setText(monthname.at(d.month()-1));
    year->setText(d.toString("yyyy"));
}

QDate datebox::date(){
    //Check Date
    QDate d;
    int m = monthname.indexOf(month->text())+1;
    d.setDate(year->text().toInt(),m,day->text().toInt());
    if(d.isValid())
        return d;
    else return QDate::currentDate();
}

