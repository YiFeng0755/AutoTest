#include "cryptutil.h"
#include <QCryptographicHash>
#include <QDateTime>

QByteArray CryptAdaptor::hashData(QByteArray text, QCryptographicHash::Algorithm alg,
                                  bool lower)
{
    QByteArray data = QCryptographicHash::hash(text, alg);

    if(lower) {
        return data.toHex();
    }
    return data.toHex().toUpper();
}

QByteArray CryptAdaptor::cryptMd5(QByteArray text, bool lower)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(text);
    QByteArray result = hash.result();
    if(lower) {
        return result.toHex();
    }
    return result.toHex().toUpper();
}

QByteArray CryptAdaptor::cryptSha1(QByteArray text, bool lower)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(text);
    QByteArray result = hash.result();
    if(lower) {
        return result.toHex();
    }
    return result.toHex().toUpper();
}

QByteArray CryptAdaptor::cryptSha256(QByteArray text, bool lower)
{
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(text);
    QByteArray result = hash.result();
    if(lower) {
        return result.toHex();
    }
    return result.toHex().toUpper();
}

QByteArray CryptAdaptor::cryptMd4(QByteArray text, bool lower)
{
    QCryptographicHash hash(QCryptographicHash::Md4);
    hash.addData(text);
    QByteArray result = hash.result();
    if(lower) {
        return result.toHex();
    }
    return result.toHex().toUpper();
}

QByteArray CryptAdaptor::toBase64(QByteArray text)
{
    return text.toBase64();
}
