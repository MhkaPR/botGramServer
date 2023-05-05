#include "authentication.h"

Authentication::Authentication()
{
   int ans = QRandomGenerator::global()->bounded(9000)+1000;


    safeVerify.setLink(QString::number(ans));
     safeVerify.setAnswer(QString::number(ans));

}

CheckVerifySafePacket Authentication::getSafeVerify()
{
    return safeVerify;
}
