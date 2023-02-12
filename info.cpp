#include "info.h"

info::info(QMedicalBoxWidget *parent) : QMedicalBoxWidget(parent)
{
    InfoLayout = new QGridLayout;
    InfoLayout->setHorizontalSpacing(50);
    InfoLayout->setVerticalSpacing(50);

    setProtocolObstetric();
    setProtocolPulmonar();
    setProtocolQuadrant();

    infoWidget = new QWidget;
    infoWidget->setContentsMargins(10,10,10,10);

    mainLayout->addWidget(infoWidget,0,Qt::AlignCenter);
    mainLayout->setMargin(30);

}

info::~info()
{
}

void info::setProtocolObstetric(void)
{
    _protocolObstetric = new QWidget;    

    QLabel *Label_Obs_Title = new QLabel(tr("Protocolo obstétrico"));

    QLabel *Label_Obs_Text =
            new QLabel(tr("1. Dirigido a pacientes femeninas\n"
                          "   en estado de gestación desde\n"
                          "   el II y III trimestre de embarazo,\n"
                          "   a partir de los 09 años de edad,\n"
                          "   específicamente para controlar\n"
                          "   el embarazo y detectar\n"
                          "   anomalías oportunamente."));

    QLabel *Label_Obs_Steps =
            new QLabel(tr("8 pasos por Protocolo"));

    Label_Obs_Title->setStyleSheet("QLabel{font-size: 14pt;font-weight: bold;}");
    Label_Obs_Text->setStyleSheet("QLabel{font-size: 12pt;}");
    Label_Obs_Steps->setStyleSheet("QLabel{font-size: 12pt;font-weight: bold;}");

    InfoLayout->addWidget(Label_Obs_Title ,0,0,1,2,Qt::AlignTop);
    InfoLayout->addWidget(Label_Obs_Text  ,2,0,4,2,Qt::AlignTop);
    InfoLayout->addWidget(Label_Obs_Steps ,6,0,1,2,Qt::AlignTop);
}

void info::setProtocolPulmonar(void)
{
    _protocolPulmonar = new QWidget;

    QLabel *Label_Pul_Title = new QLabel(tr("Protocolo obstétrico"));

    QLabel *Label_Pul_Text =
            new QLabel(tr("1. Dirigido a pacientes desde el\n"
                          "   primer año de edad con:\n"
                          "   - Fiebre asociada a dificultad\n"
                          "     para respirar.\n"
                          "   - Dolor de espalda y/o pecho\n"
                          "     que se incrementa al respirar.\n"
                          "   - Secuelas de COVID-19.\n"
                          "   - Neumonía.\n"
                          "   - Fibrosis pulmonar.\n"
                          "   - Derrame pleural.\n"));

    QLabel *Label_Pul_Steps =
            new QLabel(tr("12 pasos por Protocolo\n"
                          "(06 pasos para cada hemitórax)"));

    Label_Pul_Title->setStyleSheet("QLabel{font-size: 14pt;font-weight: bold;}");
    Label_Pul_Text->setStyleSheet("QLabel{font-size: 12pt;}");
    Label_Pul_Steps->setStyleSheet("QLabel{font-size: 12pt;font-weight: bold;}");

    InfoLayout->addWidget(Label_Pul_Title ,0,2,1,2,Qt::AlignTop);
    InfoLayout->addWidget(Label_Pul_Text  ,2,2,4,2,Qt::AlignTop);
    InfoLayout->addWidget(Label_Pul_Steps ,6,2,1,2,Qt::AlignTop);

}

void info::setProtocolQuadrant(void)
{
    _protocolQuadrant = new QWidget;

    QLabel *Label_Qua_Title = new QLabel(tr("Protocolo de Cuadrante\nSuperior Derecho"));

    QLabel *Label_Qua_Text =
            new QLabel(tr("1. Dirigido a pacientes desde los\n"
                          "   18 años de edad con:\n"
                          "   - Dolor abdominal en la zona.\n"
                          "   - Antecedentes o control  de\n"
                          "     hígado graso.\n"
                          "   - Cálculos en la vesícula.\n"
                          "   - Inflamaciones, tumores o\n"
                          "     nódulos en hígado y vesícula.\n"
                          "2. Se debe realizar con el\n"
                          "   paciente en estado de ayuno\n"
                          "   por un periodo de 8 horas."));

    QLabel *Label_Qua_Steps =
            new QLabel(tr("6 pasos por Protocolo\n"));

    Label_Qua_Title->setStyleSheet("QLabel{font-size: 14pt;font-weight: bold;}");
    Label_Qua_Text->setStyleSheet("QLabel{font-size: 12pt;}");
    Label_Qua_Steps->setStyleSheet("QLabel{font-size: 12pt;font-weight: bold;}");

    InfoLayout->addWidget(Label_Qua_Title ,0,4,1,2);
    InfoLayout->addWidget(Label_Qua_Text  ,2,4,4,2);
    InfoLayout->addWidget(Label_Qua_Steps ,6,4,1,2);
}


void info::refresh()
{
    infoWidget->setLayout(InfoLayout);
}


