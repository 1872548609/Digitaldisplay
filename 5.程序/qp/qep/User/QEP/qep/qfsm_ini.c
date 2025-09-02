
#include "qep_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qfsm_ini")

/**
* \file
* \ingroup qep
* \brief QFsm_init() implementation.
*/

/*..........................................................................*/
void QFsm_init(QFsm * const me, QEvt const * const e) {
    QS_CRIT_STAT_

    Q_REQUIRE((me->temp != Q_STATE_CAST(0))        /* ctor must be executed */
              && (me->state == Q_STATE_CAST(0))); /*initial tran. NOT taken */

    QS_BEGIN_(QS_QEP_STATE_INIT, QS_smObj_, me)
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(Q_STATE_CAST(0));   /* source state (not defined for a FSM) */
        QS_FUN_(me->temp);                  /* the target of the transition */
    QS_END_()

                                 /* execute the top-most initial transition */
    Q_ALLEGE((*me->temp)(me, e) == Q_RET_TRAN); /* transition must be taken */

    (void)QEP_TRIG_(me->temp, Q_ENTRY_SIG);             /* enter the target */
    me->state = me->temp;                    /* record the new active state */

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_smObj_, me)
        QS_TIME_();                                           /* time stamp */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(me->state);                         /* the new active state */
    QS_END_()
}
