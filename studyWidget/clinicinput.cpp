 #include "clinicinput.h"
#include <QDebug>

clinicInput::clinicInput(QJsonObject object, QWidget *parent) : QWidget(parent)
{
    type << "text" << "date" << "checkbox" << "number" << "select" << "radiobutton"<<"mix";
    obj = object;

    QString textName;
    textName = obj.value("label").toString();
    QVBoxLayout * l = new QVBoxLayout(this);
    QLabel * label = new QLabel(textName);


    if(obj.value("type").toString() == "date")
    {
       QFont f = label->font();
       f.setPointSize(10);
    }

    QString labelObjName = "";

    if(obj.value("type").toString() == "date" && textName == "FPP")
    {
        l->setObjectName("fppParent");
    }
    else if(obj.value("type").toString() == "select")
    {
        qDebug() << "Select found, name is: " + textName;
    }

    //label->setFixedWidth(250);        // Christiam
    foreach(QJsonValue s, obj.value("items").toArray())
        item.append(s.toString());
    foreach(QJsonValue s, obj.value("default").toArray())
        defaults.append(s.toString());
    if(defaults.size() == 0)
        defaults.append("");

    l->addWidget(label,0,Qt::AlignTop);

    QString strFechaUltrasonidoPrevioEs="Fecha de ultrasonido previo"; //JB18022020
    QString strFechaUltrasonidoPrevioEn="Previous ultrasound date"; //JB18022020
    if(textName == strFechaUltrasonidoPrevioEs || textName == strFechaUltrasonidoPrevioEn) //JB18022020
    {
        QCheckBox * uecb = new QCheckBox(tr("Primer Ultrasonido"), this);
        uecb->setObjectName("uecb");
        connect(uecb, SIGNAL(clicked()), this, SLOT(firstUltrasoundCheck()));
        l->addWidget(uecb, 0, Qt::AlignLeft);
    }

    if(obj.value("type").toString() == "text"){
        QVkLineEdit *input = new QVkLineEdit;
        input->setText(defaults.at(0));

        std_input = input;
    }

    if(obj.value("type").toString() == "date"){
        QCalendarWidget * input = new QCalendarWidget;
        input->setMinimumDate(QDate(1900, 1, 1));
        input->setMaximumDate(QDate(3000, 1, 1));

        if(textName == "FPP" || textName == "EDD")
        {
            QObject* p = this->parent();
            QCalendarWidget* fur = p->findChild<QCalendarWidget*>("FUR");
            if(fur)
            {
//                QDate fppDate = calcFPP(fur->selectedDate());
                QDate fppDate = fur->selectedDate();
                fppDate = fppDate.addDays(7);
                fppDate = fppDate.addYears(1);
                fppDate = fppDate.addMonths(-3);
                input->setSelectedDate(fppDate);
            }
            else {
                qDebug() << "no fur calendar previously created";
            }

            input->setObjectName("FPP");
            input->setEnabled(false);
        }
        /*else
        {
            if(textName == "FUR" || textName == "LMP")
            {
                input->setMaximumDate(QDate::currentDate());// JB20200120 Validacion  FUR debe ser menor o igual a la fecha actual.
                input->setObjectName("FUR");
            }
            else if(textName == strFechaUltrasonidoPrevioEs || textName == strFechaUltrasonidoPrevioEn)
            {
                input->setMaximumDate(QDate::currentDate());// JB20200114 Validacion  FUU debe ser menor o igual a la fecha actual.
                input->setObjectName("FUU");
            }
            connect(input,SIGNAL(selectionChanged()),this,SLOT(calendarSelectedTestNoStr()));
        }*/

        else if(textName == "FUR" || textName == "LMP"){
                input->setMaximumDate(QDate::currentDate());// JB20200120 Validacion  FUR debe ser menor o igual a la fecha actual.
                input->setObjectName("FUR");
                connect(input,SIGNAL(selectionChanged()),this,SLOT(calendarSelectedTestNoStr()));
        }

        else if(textName == strFechaUltrasonidoPrevioEs || textName == strFechaUltrasonidoPrevioEn){
                input->setMaximumDate(QDate::currentDate());// JB20200114 Validacion  FUU debe ser menor o igual a la fecha actual.
                input->setObjectName("FUU");
                connect(input,SIGNAL(selectionChanged()),this,SLOT(calendarSelectedTestNoStr()));
            }
        else {
            input->setMaximumDate(QDate::currentDate());
        }
        std_input =  input;
    }
//---------------------------------------------------------------------------------
//  Christiam: Falto terminar
//    if( (obj.value("type").toString() == "date") && (textName == "FUR") )
//    {


//    }
//---------------------------------------------------------------------------------
//    if(obj.value("type").toString() == "date"){
//        datebox * input = new datebox;
//        if (defaults.at(0) != "")
//            input->setDate(QDate::fromString(defaults.at(0),"dd/MM/yyyy"));
//        std_input = input;
//    }

    if(obj.value("type").toString() == "checkbox"){
        checkboxes * input = new checkboxes(item,defaults);
        std_input = input;
    }

    if(obj.value("type").toString() == "number"){
        QVkLineEdit *input = new QVkLineEdit;
        input->setText(defaults.at(0));
        input->setValidator(new QIntValidator(0, 9999, this));
        std_input = input;
    }

    if(obj.value("type").toString() == "select"){
        TouchComboBox * input = new TouchComboBox();
        input->setObjectName(textName);
        input->setItems(item);
        if (defaults.at(0) != "")
            input->setText(defaults.at(0));
        std_input = input;
    }

    if(obj.value("type").toString() == "radiobutton"){
        radiobuttons * input = new radiobuttons(item,defaults);
        std_input = input;
    }
//----------------------------------------------------------------------
//  Christiam
    if(obj.value("type").toString() == "mix"){
        checkboxLine *input = new checkboxLine(item);
        std_input = input;
    }
//----------------------------------------------------------------------

    if(!type.contains(obj.value("type").toString())){
        QVkLineEdit *input = new QVkLineEdit;
        input->setText(defaults.at(0));        
        std_input = input;

    }
    l->addWidget(std_input);

}

void clinicInput::calendarSelectedTestNoStr()
{
    QCalendarWidget* cal = this->findChild<QCalendarWidget*>("FUR");
    if(cal)
    {
        QString dateFormat = "yyyy-MM-dd";
        QDate fppDate = cal->selectedDate();
        fppDate = fppDate.addDays(7);
        fppDate = fppDate.addYears(1);
        fppDate = fppDate.addMonths(-3);
        QString fppDateString = fppDate.toString(dateFormat);

        QObject* p = this->parent();
        if(p)
        {
            QCalendarWidget* fpp = p->findChild<QCalendarWidget*>("FPP");
            if(fpp)
            {
                fpp->setSelectedDate(fppDate);
            }

            QCalendarWidget* fuu = p->findChild<QCalendarWidget*>("FUU");
            if(fuu)
            {
                fuu->setMinimumDate(cal->selectedDate());
            }
        }
    }
    else
    {
        QCalendarWidget* fup = this->findChild<QCalendarWidget*>("Fecha de ultrasonido previo");
        if(fup)
        {
            qDebug() << "Acessed FUP calendar object. Object Name is: " + fup->objectName();
        }
        else
        {
            qDebug() << "Accessed a different calendar object";
        }
    }
}

void clinicInput::firstUltrasoundCheck()
{
    QObject* p = this->parent();
    QCheckBox* uecb = p->findChild<QCheckBox*>("uecb");

    if(p && uecb)
    {
        QCalendarWidget* fuu = p->findChild<QCalendarWidget*>("FUU");

        if(fuu)
        {
            if(uecb->isChecked())
            {
                QDate blockDate = QDate(1900, 1, 1);

                fuu->setMinimumDate(blockDate); //Christiam
                fuu->setSelectedDate(blockDate);
                fuu->setEnabled(false);
            }
            else
            {
                QDate now = QDate();
                now = now.currentDate();
                fuu->setEnabled(true);
                fuu->setSelectedDate(now);
            }
        }
        else
        {
            qDebug() << "Calendar not found in this object";
        }
    }
}

QJsonObject clinicInput::getJsonObject(){

    QJsonObject object;
    object.insert("label",obj.value("label"));
    object.insert("name",obj.value("name"));
    object.insert("type",obj.value("type"));
    object.insert("items",obj.value("items"));
    QJsonArray values;

    if(obj.value("type").toString() == "text"){
        QVkLineEdit * input = (QVkLineEdit*)std_input;
        values.append(input->text());
    }

    //AVISO (jobenas):
    //Sospecho que aqui esta el problema de porque se
    //cuelga el programa. Aqui aun se utiliza el widget
    //del datebox. Voy a modificar este if condition por
    //uno mio para tratar de resolver el problema.
    if(obj.value("type").toString() == "date"){
        QString dateFormat("dd/MM/yyyy");
        QCalendarWidget * input = (QCalendarWidget*)std_input;
        values.append(input->selectedDate().toString(dateFormat));
    }

//    if(obj.value("type").toString() == "date"){
//        datebox * input = (datebox*)std_input;
//        values.append(input->date().toString("dd/MM/yyyy"));
//    }

    if(obj.value("type").toString() == "checkbox"){
        checkboxes * input = (checkboxes*)(std_input);
        foreach(QString s, input->getChecked())
        values.append(s);
    }

    if(obj.value("type").toString() == "number"){
        QVkLineEdit * input = (QVkLineEdit*)std_input;
        values.append(input->text());
    }

    if(obj.value("type").toString() == "select"){
        TouchComboBox * input = (TouchComboBox*)std_input;
        values.append(input->text());
    }

    if(obj.value("type").toString() == "radiobutton"){
        radiobuttons * input = (radiobuttons*)std_input;
        values.append(input->text());
    }
//----------------------------------------------------------------------------------
//  Christiam
    if(obj.value("type").toString() == "mix"){
        checkboxLine *input = (checkboxLine*)std_input;
        if(input->myChk->checkState()==Qt::Checked)
            values.append(input->myLine->text());
        else
            values.append("-32768");
    }
//----------------------------------------------------------------------------------

    if(!type.contains(obj.value("type").toString())){
        QVkLineEdit * input = (QVkLineEdit*)std_input;
        values.append(input->text());
    }
    object.insert("values",values);
    return object;
}

QDate calcFPP(QDate fur)
{
    QDate fppDate = fur;
    fppDate = fppDate.addDays(7);
    fppDate = fppDate.addYears(1);
    fppDate = fppDate.addMonths(-3);

    return fppDate;
}
