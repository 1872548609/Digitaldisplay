#include "qep_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qfsm_dis")

void QFsm_dispatch(QFsm * const me, QEvt const * const e) {
    QState r;
    QS_CRIT_STAT_

    Q_REQUIRE(me->state == me->temp); /* state configuration must be stable */

    QS_BEGIN_(QS_QEP_DISPATCH, QS_smObj_, me)
        QS_TIME_();                                           /* time stamp */
        QS_SIG_(e->sig);                         /* the signal of the event */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(me->state);                            /* the current state */
    QS_END_()

    r = (*me->state)(me, e);                      /* call the event handler */
    if (r == Q_RET_TRAN) {                             /* transition taken? */

        QS_BEGIN_(QS_QEP_TRAN, QS_smObj_, me)
            QS_TIME_();                                       /* time stamp */
            QS_SIG_(e->sig);                     /* the signal of the event */
            QS_OBJ_(me);                       /* this state machine object */
            QS_FUN_(me->state);             /* the source of the transition */
            QS_FUN_(me->temp);              /* the target of the transition */
        QS_END_()

        QEP_EXIT_(me->state);                            /* exit the source */
        QEP_ENTER_(me->temp);                           /* enter the target */
        me->state = me->temp;                /* record the new active state */
    }
    else {                                          /* transition not taken */
#ifdef Q_SPY

        if (r == Q_RET_UNHANDLED) {
            QS_BEGIN_(QS_QEP_UNHANDLED, QS_smObj_, me)
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(me->state);                    /* the current state */
            QS_END_()
        }

        if (r == Q_RET_HANDLED) {

            QS_BEGIN_(QS_QEP_INTERN_TRAN, QS_smObj_, me)
                QS_TIME_();                                   /* time stamp */
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(me->state);                    /* the current state */
            QS_END_()

        }
        else {

            QS_BEGIN_(QS_QEP_IGNORED, QS_smObj_, me)
                QS_TIME_();                                   /* time stamp */
                QS_SIG_(e->sig);                 /* the signal of the event */
                QS_OBJ_(me);                   /* this state machine object */
                QS_FUN_(me->state);                    /* the current state */
            QS_END_()

        }
#endif
    }
}
