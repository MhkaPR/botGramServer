#include "authentication.h"

Authentication::Authentication()
{
   int ans = QRandomGenerator::global()->bounded(9000)+1000;
    safeVerify.Link = QString::number(ans);
     safeVerify.Answer = QString::number(ans);

}

CheckVerifySafePacket Authentication::getSafeVerify()
{
    return safeVerify;
}
