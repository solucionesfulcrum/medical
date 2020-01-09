#include "clinicinput.h"
#include <QDebug>


clinicInput::clinicInput(QJsonObject object, QWidget *parent) : QWidget(parent)
{
    type << "text" << "date" << "checkbox" << "number" << "select" << "radiobutton";
    obj = object;

    QString textName;

    QVBoxLayout * l = new QVBoxLayout(this);
    QLabel * label = new QLabel(obj.value("label").toString());
    textName = obj.value("label").toString();

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

    label->setFixedWidth(250);
    foreach(QJsonValue s, obj.value("items").toArray())
        item.append(s.toString());
    foreach(QJsonValue s, obj.value("default").toArray())
        defaults.append(s.toString());
    if(defaults.size() == 0)
        defaults.append("");

    l->addWidget(label,0,Qt::AlignTop);

    if(textName == "Fecha de ultrasonido previo")
    {
        qDebug() << "Found the calendar that needs checkbox";

        QCheckBox * uecb = new QCheckBox("Primer Ultrasonido", this);
        uecb->setObjectName("uecb");
//        connect(uecb, SIGNAL(stateChange()), this, SLOT(firstUltrasoundCheck()));
        connect(uecb, SIGNAL(clicked()), this, SLOT(firstUltrasoundCheck()));
        l->addWidget(uecb, 0, Qt::AlignLeft);
    }


    if(obj.value("type").toString() == "text"){
        QVkLineEdit *input = new QVkLineEdit;
        input->setText(defaults.at(0));

        std_input = input;
    }


    //AVISO IMPORTANTE (jobenas)
    //Aqui es donde se estan insertando los campos de fecha, esto es lo
    //se requiere probar para ver si acepta widgets diferentes.
    //ACTUALIZACION (jobenas)
    //El cambio si genera el nuevo objeto en vez del anterior.
    //Queda pendiente como distinguirlos, voy a incluir un contador para eso
    //Queda ver en que parte se puede inicializar.
    //ACTUALIZACION (jobenas)
    //No se puede usar un contador para determinar el calendario
    //individual, sin embargo cada llamada genera una etiqueta (label)
    //diferente, esta etiqueta permite distinguir a que calendario se
    //esta haciendo referencia, estoy usando esto para desactivar el
    //calendario de FPP debido a que este es un valor que se va a calcular.
    if(obj.value("type").toString() == "date"){
        QCalendarWidget * input = new QCalendarWidget;
        input->setMinimumDate(QDate(1900, 1, 1));
        input->setMaximumDate(QDate(3000, 1, 1));

        qDebug() << "current  object name is: " + input->objectName();

        qDebug() << "Label for current calendar is " + textName;
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
        else
        {
            if(textName == "FUR" || textName == "LMP")
            {
                input->setObjectName("FUR");
            }
            else if(textName == "Fecha de ultrasonido previo")
            {
                input->setObjectName("FUU");
            }
            connect(input,SIGNAL(selectionChanged()),this,SLOT(calendarSelectedTestNoStr()));
        }
        std_input =  input;
    }

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

    if(!type.contains(obj.value("type").toString())){
        QVkLineEdit *input = new QVkLineEdit;
        input->setText(defaults.at(0));
        std_input = input;

    }
    l->addWidget(std_input);

}

//AVISO(jobenas): Esta es una funcion de prueba para evaluar la
//capacidad de interconexion de los calendarios. Cuando los
//calendarios son creados, se genera un connect que llama a esta
//funcion. La idea es identificar individualmente a cada calendario
//para el calculo del FPP.
//ACTUALIZACION(jobenas): La funcion logra interconectar los calendarios, permitiendo el
//calculo del FPP (uno de los requerimientos para esta parte). Lo unico que queda pendiente es
//poder extraer la informacion de estos valores para poder enviarlas al servicio cloud.
void clinicInput::calendarSelectedTestNoStr()
{
    qDebug() << "Detected calendar change, Running function without name";
    QCalendarWidget* cal = this->findChild<QCalendarWidget*>("FUR");
    if(cal)
    {
        QString dateFormat = "yyyy-MM-dd";
        //calculo de la fecha probable de parto es equivalente a 277 dias
        //desde la fur, segun la formula
        //ACTUALIZACION (jobenas): Segun lo conversado se usaran los valores
        //especificos de la formula >  1 año, 7 días y 3 meses
//        QDate fppDate = cal->selectedDate().addDays(277);
//        QDate fppDate = calcFPP(cal->selectedDate());
        QDate fppDate = cal->selectedDate();
        fppDate = fppDate.addDays(7);
        fppDate = fppDate.addYears(1);
        fppDate = fppDate.addMonths(-3);
        QString fppDateString = fppDate.toString(dateFormat);
        qDebug() << "Accessed FUR calendar object. Object Name is: " + cal->objectName();
        qDebug() << "FPP Value is: " + fppDateString;
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

//AVISO (jobenas): Estoy creando una funcion que se encargue de generar si el
//checkbox de pimer ultrasonido ha sido creado o no.
void clinicInput::firstUltrasoundCheck()
{
    qDebug() << "First Ultrasound checkbox checked";
    QObject* p = this->parent();
    QCheckBox* uecb = p->findChild<QCheckBox*>("uecb");
    if(!uecb)
    {
        qDebug() << "No checkbox found";
        return;
    }
    if(p)
    {
        QCalendarWidget* fuu = p->findChild<QCalendarWidget*>("FUU");
        if(fuu)
        {
            if(uecb->isChecked())
            {
                QDate blockDate = QDate(1900, 1, 1);
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
        checkboxes * input = (checkboxes*)std_input;
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
