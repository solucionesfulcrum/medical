#ifndef AESCRIPT_H
#define AESCRIPT_H

#include <QByteArray>
#include <QString>
#include <QVector>
#include <QDebug>
#define byte uint8_t

class aescript
{
public:
    aescript();
    QByteArray meditEncrypt(QByteArray message,QByteArray key, QByteArray messageKey, int n, int m);
QByteArray decodif(QByteArray message);
QByteArray codif(QByteArray message);
private:
    byte Xor(byte a, byte b);
    QByteArray AddRoundKey(QByteArray state, QByteArray w);
    QByteArray SubBytes(QByteArray state);
    QByteArray ShiftRows(QByteArray state);
    QByteArray MixColumns( QByteArray s);
    QByteArray encryptRound(QByteArray state,QByteArray key);
    byte FFMul(byte a, byte b);
    int nb;
    QVector<int> sbox;
};

#endif // AESCRIPT_H
