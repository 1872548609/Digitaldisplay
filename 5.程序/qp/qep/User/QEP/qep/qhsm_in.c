#include "qep_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qhsm_in")

/**
* \file
* \ingroup qep
* \brief QHsm_isIn() implementation.
*/

/*..........................................................................*/
uint8_t QHsm_isIn(QHsm * const me, QStateHandler const state) {
    uint8_t inState = (uint8_t)0; /* assume that this HSM is not in 'state' */
    QState r;

    Q_REQUIRE(me->temp == me->state); /* state configuration must be stable */

    do {
        if (me->temp == state) {                    /* do the states match? */
            inState = (uint8_t)1;               /* match found, return TRUE */
            r = Q_RET_IGNORED;                     /* break out of the loop */
        }
        else {
            r = QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);
        }
    } while (r != Q_RET_IGNORED);             /* QHsm_top state not reached */
    me->temp = me->state;         /* restore the stable state configuration */

    return inState;                                    /* return the status */
}
