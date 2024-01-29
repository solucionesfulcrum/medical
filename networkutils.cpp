#include "networkutils.h"

QString NetworkUtils::obtenerDireccionMAC() {
    QString macAddress;

    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    foreach (QNetworkInterface interface, interfaces) {
        //if (interface.isValid() && interface.flags().testFlag(QNetworkInterface::IsUp) && !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
        //Detecta la primera interfaz y obteniene su dirección MAC
        if (interface.isValid()) {
            macAddress = interface.hardwareAddress();
            break;
        }
    }

    return macAddress;
}
