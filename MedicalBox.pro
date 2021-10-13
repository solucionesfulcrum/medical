#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T16:39:53
#
#-------------------------------------------------

QT += core gui sql network printsupport multimedia multimediawidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MedicalBox4
TEMPLATE = app


SOURCES += main.cpp\
    form/calform.cpp \
    form/checkboxline.cpp \
        mainwindow.cpp \
    sqlite.cpp \
    queuewidget.cpp \
    study.cpp \
    studyfinished.cpp \
    visor.cpp \
    configuration.cpp \
    historical.cpp \
    checkbandwith.cpp \
    qhistwidget.cpp \
    entitites/series.cpp \
    form/touchcombobox.cpp \
    form/qvirtualkeyboard.cpp \
    form/qvklineedit.cpp \
    dialog/dialogpatient.cpp \
    entitites/patient.cpp \
    entitites/studies.cpp \
    qheaderwidget.cpp \
    entitites/config.cpp \
    entitites/studydesc.cpp \
    qmedicalboxwidget.cpp \
    entitites/entities.cpp \
    serieswidget.cpp \
    entitites/operators.cpp \
    widgets/wlogin.cpp \
    studycheck.cpp \
    dialog/dialogoperator.cpp \
    accesor.cpp \
    studyWidget/studyprotocols.cpp \
    studyWidget/studyinfowidget.cpp \
    studyWidget/sweepsline.cpp \
    studyWidget/capturebutton.cpp \
    studyWidget/studyprotocolsform.cpp \
    widgets/slidingstackedwidget.cpp \
    dialog/myffplay.cpp \
    form/datebox.cpp \
    studyWidget/clinicdatawidget.cpp \
    form/checkboxes.cpp \
    studyWidget/clinicinput.cpp \
    widgets/battery.cpp \
    widgets/titlelabel.cpp \
    studyWidget/sendbutton.cpp \
    capture/captureprocess.cpp \
    widgets/captureprocesswidget.cpp \
    widgets/diskspace.cpp \
    ejecuta.cpp

HEADERS  += mainwindow.h \
    form/calform.h \
    form/checkboxline.h \
    sqlite.h \
    queuewidget.h \
    study.h \
    studyfinished.h \
    visor.h \
    configuration.h \
    historical.h \
    checkbandwith.h \
    qhistwidget.h \
    entitites/series.h \
    form/touchcombobox.h \
    form/qvirtualkeyboard.h \
    form/qvklineedit.h \
    dialog/dialogpatient.h \
    entitites/patient.h \
    entitites/studies.h \
    qheaderwidget.h \
    entitites/config.h \
    entitites/studydesc.h \
    qmedicalboxwidget.h \
    entitites/entities.h \
    serieswidget.h \
    entitites/operators.h \
    widgets/wlogin.h \
    include.h \
    studycheck.h \
    dialog/dialogoperator.h \
    accesor.h \
    studyWidget/studyprotocols.h \
    studyWidget/studyinfowidget.h \
    studyWidget/sweepsline.h \
    studyWidget/capturebutton.h \
    studyWidget/studyprotocolsform.h \
    widgets/slidingstackedwidget.h \
    dialog/myffplay.h \
    form/datebox.h \
    studyWidget/clinicdatawidget.h \
    form/checkboxes.h \
    studyWidget/clinicinput.h \
    widgets/battery.h \
    widgets/titlelabel.h \
    studyWidget/sendbutton.h \
    capture/captureprocess.h \
    widgets/captureprocesswidget.h \
    widgets/diskspace.h \
    ejecuta.h

RESOURCES += \
    res.qrc \
    translations.qrc

DISTFILES += \
    style.qss \
    style_old.qss

TRANSLATIONS += MedicalBox_en.ts

#INCLUDEPATH += D:/library/medisecure/include
#DEPENDPATH += D:/library/medisecure/include
#LIBS += -LD:/library/medisecure/lib/ -llibmedisecure


INCLUDEPATH += D:/library/medisecure/include
DEPENDPATH += D:/library/medisecure/include
#LIBS += -LD:/library/medisecure/lib/ -libmedisecure
LIBS += -LD:/library/medisecure/lib/ -llibmedisecure
