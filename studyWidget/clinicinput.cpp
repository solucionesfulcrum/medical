#include "clinicinput.h"
#include <QDebug>
#include <QButtonGroup>

clinicInput::clinicInput(QJsonObject object,QButtonGroup *g, QRadioButton *w, QWidget *parent) : QWidget(parent)
{
    type << "text" << "date" << "checkbox" << "number" << "select" << "radiobutton"<<"mix";
    obj = object;

    QString textName = obj.value("label").toString();
    QString inputId = obj.value("name").toString(); // deberiamos usar el name y no el label
    QVBoxLayout * l = new QVBoxLayout(this);
    QLabel * label = new QLabel(textName);

    l->addWidget(label,0,Qt::AlignTop);
    setItemDefaults();

//  DATE Input Logic
    if(obj.value("type").toString() == "date")
    {
       QFont f = label->font();
       f.setPointSize(10);

       QString strFechaUltrasonidoPrevioEs="Fecha de ultrasonido previo";       //JB18022020
       QString strFechaUltrasonidoPrevioEn="Previous ultrasound date";          //JB18022020

       QCalendarWidget * input = new QCalendarWidget;
       input->setMinimumDate(QDate(1900, 1, 1));
       input->setMaximumDate(QDate(3000, 1, 1));

       if(textName == "FPP" || textName == "EDD")
       {
          if (textName == "FPP") l->setObjectName("fppParent");

          QObject* p = this->parent();
          QCalendarWidget* fur = p->findChild<QCalendarWidget*>("FUR");
          if(fur)
          {
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
       else if(textName == strFechaUltrasonidoPrevioEs || textName == strFechaUltrasonidoPrevioEn) //JB18022020
       {
           QCheckBox * uecb = new QCheckBox(tr("Primer Ultrasonido"), this);
           uecb->setObjectName("uecb");
           connect(uecb, SIGNAL(clicked()), this, SLOT(firstUltrasoundCheck()));
           l->addWidget(uecb, 0, Qt::AlignLeft);

           input->setMaximumDate(QDate::currentDate());// JB20200114 Validacion  FUU debe ser menor o igual a la fecha actual.
           input->setObjectName("FUU");
           connect(input,SIGNAL(selectionChanged()),this,SLOT(calendarSelectedTestNoStr()));
       }
       else if(textName == "FUR" || textName == "LMP")
       {
               input->setMaximumDate(QDate::currentDate());// JB20200120 Validacion  FUR debe ser menor o igual a la fecha actual.
               input->setObjectName("FUR");
               connect(input,SIGNAL(selectionChanged()),this,SLOT(calendarSelectedTestNoStr()));
       }
       else if(inputId == "dateFechaToma" || inputId == "dateSymtomStart" || inputId == "dateCovidStart" || inputId == "dateHospitalization" || inputId == "dateLastDose")
       {
           // Italo 31/05/2021
           // Identifica el objeto de fecha de toma y lo deshabilita para habilitarlo
           // cuando la repuesta sea afirmativa en el campo "¿Se realizó la prueba COVID?"
           input->setMaximumDate(QDate::currentDate().addDays(1));// Validacion fecha toma debe ser menor o igual a la fecha actual.
           input->setSelectedDate(QDate(1900, 1, 1));
           input->setEnabled(false);
           input->setObjectName(inputId);
       }
       else {
           input->setMaximumDate(QDate::currentDate());
       }

       std_input =  input;
    }

//-------------------------------------------------------------------
//  TEXT Input Logic
    else if(obj.value("type").toString() == "text")
    {
        QVkLineEdit *input = new QVkLineEdit;
        input->setText(defaults.at(0));

        std_input = input;
    }

//-------------------------------------------------------------------
//  CHECKBOX Input Logic
    else if(obj.value("type").toString() == "checkbox")
    {
        checkboxes * input = new checkboxes(item,defaults);
        input->setObjectName(inputId);

        // Italo 26/05/2021
        // SIGNAL: Valida si se marca el checkbox de "Cirugías abdominales anteriores"
        // para habilitar el text line de especificación.
        if (textName == "Historia Médica")
        {
            QCheckBox* cbLast = input->getItem(item.size()-1);
            QString cbText = cbLast->text();
            if (cbText == "Otras cirugías abdominales anteriores")
            {
                connect(cbLast,SIGNAL(stateChanged(int)),this,SLOT(specificChecked(int)));
            }
        }

        // Italo 16/08/2021
        // SIGNAL: Valida si se marca el checkbox de "Otros"
        // para habilitar el text line de otros.
        else if (textName == "Indicaciones del examen")
        {
            QCheckBox* cbLast = input->getItem(item.size()-1);
            QString cbText = cbLast->text();
            if (cbText == "Otros")
            {
                connect(cbLast,SIGNAL(stateChanged(int)),this,SLOT(otherChecked(int)));
            }
        }
        else if (inputId == "cbSymptomKind")
        {
            input->setEnabled(false);

            int i = item.indexOf("Otro síntoma");
            if (i > -1)
            {
                QCheckBox *cbOther = input->getItem(i);
                connect(cbOther,SIGNAL(stateChanged(int)),this,SLOT(otherSymtomChecked(int)));
            }
        }
        else if (inputId == "cbComorbidity")
        {
            int i = item.indexOf("Otra cond. de comorbilidad");
            if (i > -1)
            {
                QCheckBox *cbOther = input->getItem(i);
                connect(cbOther,SIGNAL(stateChanged(int)),this,SLOT(otherComorbidityChecked(int)));
            }

            i = item.indexOf("Ninguna");
            if (i > -1)
            {
                QCheckBox *cbNon = input->getItem(i);
                connect(cbNon,SIGNAL(stateChanged(int)),this,SLOT(nonComorbidityChecked(int)));
            }
        }

        std_input = input;
    }
//-------------------------------------------------------------------
//  NUMBER Input Logic
    else if(obj.value("type").toString() == "number")
    {
        QVkLineEdit *input = new QVkLineEdit;
        input->setText(defaults.at(0));
        input->setValidator(new QIntValidator(0, 9999, this));
        std_input = input;
    }

//-------------------------------------------------------------------
//  SELECT Input Logic
    else if(obj.value("type").toString() == "select")
    {
        TouchComboBox * input = new TouchComboBox();
        input->setObjectName(inputId);
    //------------------------------------------------------
    //  CR: 11/11/23
        if(inputId=="puestoSalud")
        {
            sites s;
            input->setItems(s.getSites());
        }
        else
        {
            input->setItems(item);
        }
    //------------------------------------------------------
        if (defaults.at(0) != "")
            input->setText(defaults.at(0));

        /* Italo 21/09/2021(updated)
         * 21/09/2021: Agrega campo de calificación del caso
         * 25/05/2021: Valida si hubieron cambios en el campo "¿Se realizó la prueba COVID?"
         * para luego habilitar el campo "Tipo de Prueba" */
        if (inputId == "selCaseStatus")
            connect(input,SIGNAL(textChanged(QString)),this,SLOT(caseStatusSelected(QString)));
        else if (inputId == "selCovidTest")
        {
            input->setEnabled(false);
            connect(input,SIGNAL(textChanged(QString)),this,SLOT(covidTestSelected(QString)));
        }
        else if (inputId == "selCovidTestType")
        {          
            input->setEnabled(false);
            connect(input,SIGNAL(textChanged(QString)),this,SLOT(otherTypeSelected(QString)));
        }
        else if (inputId == "selSymptom")
            connect(input,SIGNAL(textChanged(QString)),this,SLOT(haveSymtomSelected(QString)));
        else if (inputId == "selHospitalized")
            connect(input,SIGNAL(textChanged(QString)),this,SLOT(hospitalizedSelected(QString)));
        else if (inputId == "selMechanicVentilation")
            input->setEnabled(false);
        else if (inputId == "selVaccinated")
            connect(input,SIGNAL(textChanged(QString)),this,SLOT(vaccinatedSelected(QString)));
        else if (inputId == "selVaccineType")
        {
            input->setEnabled(false);
            connect(input,SIGNAL(textChanged(QString)),this,SLOT(otherVaccineTypeSelected(QString)));
        }
        else if (inputId == "selVaccineDose")
            input->setEnabled(false);
        else if (inputId == "selHadCovid")
            connect(input,SIGNAL(textChanged(QString)),this,SLOT(hadCovidSelected(QString)));



        std_input = input;
    }
    /* RADIOBUTTON Input Logic */
    else if(obj.value("type").toString() == "radiobutton")
    {
        radiobuttons * input = new radiobuttons(item,defaults);
        std_input = input;
    }
    /* MIX Input Logic - Christiam*/
    else if(obj.value("type").toString() == "mix")
    {
        checkboxLine *input = new checkboxLine(item);
        std_input = input;
    }
    /* DEFAULT Input Logic */
    else
    {
        // Same as TEXT evaluate using only one
        QVkLineEdit *input = new QVkLineEdit;
        input->setText(defaults.at(0));
        input->setObjectName(inputId);

        if (inputId == "txtCovidTestOther" || inputId == "txtSymtomOther" || inputId == "txtVaccineOther" || inputId == "txtComorbidityOther")
            input->setEnabled(false);
        else if (textName == "Otras cirugías abdominales anteriores(Especificar)")
        {
            input->setEnabled(false);
            input->setObjectName("Specific");
        }
        else if (textName == "Otros")
        {
            input->setEnabled(false);
            input->setObjectName("Other");
        }
        else if(inputId=="txtLugarCampania")
        {
            input->setText("");
            input->setEnabled(false);
            connect(g,SIGNAL(buttonClicked(int)),this,SLOT(buttonChange(int)));
            connect(w,SIGNAL(clicked(bool)),this,SLOT(selectButtonCampaign(bool)));
        }

        std_input = input;
    }

    l->addWidget(std_input);
}

void clinicInput::calendarSelectedTestNoStr()
{
    QCalendarWidget* cal = this->findChild<QCalendarWidget*>("FUR");
    if(cal)
    {
        //QString dateFormat = "yyyy-MM-dd";
        QDate fppDate = cal->selectedDate();
        fppDate = fppDate.addDays(7);
        fppDate = fppDate.addYears(1);
        fppDate = fppDate.addMonths(-3);
        //QString fppDateString = fppDate.toString(dateFormat);

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

// Italo 21/09/2021
// Permite activar y desactivar la opción de prueba covid
// según seleccion (si o no) en el campo = "selCaseStatus"
void clinicInput::caseStatusSelected(const QString &text)
{
    QObject* p = this->parent();

    if (p)
    {
        TouchComboBox* cbTest = p->findChild<TouchComboBox*>("selCovidTest");
        if (cbTest)
        {
            cbTest->clear();
            qDebug() << text;
            if (text == "Confirmado")
                cbTest->setEnabled(true);
            else
                cbTest->setEnabled(false);
        }
    }
}

// Italo 21/09/2021
// Permite activar y desactivar la opción para marcar síntomas
// según seleccion (si o no) en el campo = "selSymtom"
void clinicInput::haveSymtomSelected(const QString &text)
{
    toggleCheckBox(this->parent(),"Si",text,"cbSymptomKind");
    toggleCalendar(this->parent(),"Si",text,"dateSymtomStart");
}

void clinicInput::toggleCheckBox(QObject *parent, const QString validationVal, const QString currentVal, const QString inputToggle)
{
    if (parent)
    {
        checkboxes *cb = parent->findChild<checkboxes*>(inputToggle);
        if (cb)
        {
            cb->clear();
            if (currentVal == validationVal) cb->setEnabled(true);
            else cb->setEnabled(false);
        }
    }
}

void clinicInput::toggleInput(QObject *parent, const QString validationVal, const QString currentVal, const QString inputToggle)
{
    if (parent)
    {
        QVkLineEdit *txt = parent->findChild<QVkLineEdit*>(inputToggle);
        if (txt)
        {
            txt->clear();
            if (currentVal == validationVal) txt->setEnabled(true);
            else txt->setEnabled(false);
        }
    }
}

void clinicInput::toggleCalendar(QObject *parent, const QString validationVal, const QString currentVal, const QString inputToggle)
{
    if (parent)
    {
        QCalendarWidget* cal = parent->findChild<QCalendarWidget*>(inputToggle);

        if (cal)
        {
            if (currentVal == validationVal)
            {
                cal->setSelectedDate(QDate::currentDate().addDays(1));
                cal->setEnabled(true);
            }
            else
            {
                QDate blockDate = QDate(1900, 1, 1);
                cal->setMinimumDate(blockDate);
                cal->setSelectedDate(blockDate);
                cal->setEnabled(false);
            }
        }
    }
}

void clinicInput::toggleSelect(QObject *parent, const QString validationVal, const QString currentVal, const QString inputToggle)
{
    if (parent)
    {
        TouchComboBox *sel = parent->findChild<TouchComboBox*>(inputToggle);
        if (sel)
        {
            sel->clear();
            if (currentVal == validationVal) sel->setEnabled(true);
            else sel->setEnabled(false);
        }
    }
}
// Italo 25/05/2021 -> updated 31/05/2021
// SLOT: función que permite activar y desactivar la opción de tipo de prueba covid
// según seleccion (si o no) en el campo = "¿Se realizó la prueba COVID?"
void clinicInput::covidTestSelected(const QString &text)
{
    QObject* p = this->parent();
    if(p)
    {
        TouchComboBox* cbType = p->findChild<TouchComboBox*>("selCovidTestType");
        QCalendarWidget* calType = p->findChild<QCalendarWidget*>("dateFechaToma");
        if(cbType)
        {
            cbType->clear();

            if( text == "Si" )  {
                cbType->setEnabled(true);

                QDate initialDate = QDate();
                initialDate = initialDate.currentDate().addDays(1);
                calType->setEnabled(true);
                calType->setSelectedDate(initialDate);
            }
            else
            {
                cbType->setEnabled(false);

                QDate blockDate = QDate(1900, 1, 1);
                calType->setMinimumDate(blockDate);
                calType->setSelectedDate(blockDate);
                calType->setEnabled(false);
            }         
        }
    }
}

// Italo 25/05/2021
// SLOT: función que permite activar y desactivar la opción de tipo de prueba covid
// según seleccion (si o no) en el campo = "¿Se realizó la prueba COVID?"
void clinicInput::otherTypeSelected(const QString &text)
{
    QObject* p = this->parent();
    if(p)
    {
        QVkLineEdit*  ldOther = p->findChild<QVkLineEdit*>("txtCovidTestOther");
        if(ldOther)
        {
            ldOther->clear();

            if( text == "Otros" ) ldOther->setEnabled(true);
            else ldOther->setEnabled(false);

        }
    }
}

// Italo 26/05/2021
// SLOT: función que permite activar y desactivar la opción de tipo de prueba covid
// según seleccion (si o no) en el campo = "¿Se realizó la prueba COVID?"
void clinicInput::specificChecked(int state)
{
    QObject* p = this->parent();
    if(p)
    {
        QVkLineEdit*  txtSpecific = p->findChild<QVkLineEdit*>("Specific");
        if(txtSpecific)
        {
            txtSpecific->clear();

            if( state == 2 ) txtSpecific->setEnabled(true);
            else txtSpecific->setEnabled(false);

        }
    }
}

// Italo 16/08/2021
// SLOT: función que permite activar y desactivar la opción de otros
// cuando se selecciona la opción otros
void clinicInput::otherChecked(int state)
{
    QObject* p = this->parent();
    if(p)
    {
        QVkLineEdit*  txtOther = p->findChild<QVkLineEdit*>("Other");
        if(txtOther)
        {
            txtOther->clear();

            if( state == 2 ) txtOther->setEnabled(true);
            else txtOther->setEnabled(false);

        }
    }
}

// Italo 21/09/2021
// Permite activar y desactivar la opción espesificar otros síntomas
// si se selecciona el checkbox de otros síntomas
void clinicInput::otherSymtomChecked(int state)
{
    toggleInput(this->parent(),"2",QString::number(state),"txtSymtomOther");
}

void clinicInput::otherComorbidityChecked(int state)
{
    toggleInput(this->parent(),"2",QString::number(state),"txtComorbidityOther");
}

void clinicInput::nonComorbidityChecked(int state)
{
    checkboxes *cbComorbidity = this->parent()->findChild<checkboxes*>("cbComorbidity");
    if (cbComorbidity)
    {
        for (int i = 0; i < cbComorbidity->size(); i++)
        {
            QCheckBox *cb = cbComorbidity->getItem(i);
            if (cb->text() != "Ninguna" && state == 2)
            {
                cb->setChecked(false);
                cb->setEnabled(false);
            }
            else if (cb->text() != "Ninguna") cb->setEnabled(true);
        }
    }
}
//-----------------------------------------------------------------
// CR: 11/11/23
void clinicInput::selectButtonCampaign(bool state)
{
    QVkLineEdit*  txtSpecific =  this->parent()->findChild<QVkLineEdit*>("txtLugarCampania");

    if(state==true)
    {
        txtSpecific->setEnabled(true);
    }
    else
    {
        txtSpecific->setText("");
        txtSpecific->setEnabled(false);
    }
}
//-----------------------------------------------------------------
// CR:  12/11/23
void clinicInput::buttonChange(int buttonSelected)
{
    QVkLineEdit*  txtSpecific =  this->parent()->findChild<QVkLineEdit*>("txtLugarCampania");

    if(buttonSelected==3)
    {
        txtSpecific->setText("");
        txtSpecific->setEnabled(false);
    }

    else if(buttonSelected==4)
    {
        txtSpecific->setText("");
        txtSpecific->setEnabled(false);
    }
}



void clinicInput::hospitalizedSelected(const QString &text)
{
    toggleCalendar(this->parent(),"Si",text,"dateHospitalization");
    toggleSelect(this->parent(),"Si",text,"selMechanicVentilation");
}
void clinicInput::vaccinatedSelected(const QString &text)
{
    toggleSelect(this->parent(),"Si",text,"selVaccineType");
    toggleSelect(this->parent(),"Si",text,"selVaccineDose");
    toggleCalendar(this->parent(),"Si",text,"dateLastDose");
}

void clinicInput::otherVaccineTypeSelected(const QString &text)
{
    toggleInput(this->parent(),"Otro",text,"txtVaccineOther");
}

void clinicInput::hadCovidSelected(const QString &text)
{
    toggleCalendar(this->parent(),"Si",text,"dateCovidStart");
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
            //-------------------------------------------------------
            //  CR: 19/05/23
                fuu->hide();
            //-------------------------------------------------------
            }
            else
            {
                QDate now = QDate();
                now = now.currentDate();
                fuu->setEnabled(true);
                fuu->setSelectedDate(now);
            //-------------------------------------------------------
            // CR: 19/05/23
                fuu->show();
            //-------------------------------------------------------
            }
        }
        else
        {
            qDebug() << "Calendar not found in this object";
        }
    }
}

void clinicInput::setItemDefaults()
{
    foreach(QJsonValue s, obj.value("items").toArray())
        item.append(s.toString());
    foreach(QJsonValue s, obj.value("default").toArray())
        defaults.append(s.toString());
    if(defaults.size() == 0)
        defaults.append("");
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

    if(obj.value("type").toString() == "date"){
        QString dateFormat("dd/MM/yyyy");
        QCalendarWidget * input = (QCalendarWidget*)std_input;
        values.append(input->selectedDate().toString(dateFormat));
    }

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
//  CR: 04/01/23
    if(obj.value("type").toString() == "mix"){
        checkboxLine *input = (checkboxLine*)std_input;
        values.append(input->myLine->text());
        /*if(input->myChk->checkState()==Qt::Checked)
            values.append(input->myLine->text());
        else
            values.append("-32768");*/
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
