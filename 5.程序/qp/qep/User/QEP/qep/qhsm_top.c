#include "qep_pkg.h"

//¶¥²ã×´Ì¬´¦Àí
QState QHsm_top(void const * const me, QEvt const * const e) {
    (void)me;            /* suppress the "unused argument" compiler warning */
    (void)e;             /* suppress the "unused argument" compiler warning */
    return Q_IGNORED();                 /* the top state ignores all events */
}
