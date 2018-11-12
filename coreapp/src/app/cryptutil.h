#ifndef MD5TOOL
#define MD5TOOL
#include <QByteArray>
#include <QCryptographicHash>
#include <QDateTime>

/**adptor for mdr crypt
 * by chendonghua
 */

class CryptAdaptor
{
public:
  static QByteArray hashData(QByteArray text,
                             QCryptographicHash::Algorithm alg,bool lower = true);

  static QByteArray cryptMd5(QByteArray text, bool lower);
  static QByteArray cryptMd4(QByteArray text, bool lower);
  static QByteArray cryptSha1(QByteArray text, bool lower);
  static QByteArray cryptSha256(QByteArray text, bool lower);
  static QByteArray toBase64(QByteArray text);
private:

};




#endif // MD5TOOL

