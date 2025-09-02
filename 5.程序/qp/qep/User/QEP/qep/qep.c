#include "qep_pkg.h"

/**
* \file
* \ingroup qep
* \brief QEP_reservedEvt_ definition and QEP_getVersion() implementation.
*/

/* Package-scope objects ---------------------------------------------------*/
QEvt const QEP_reservedEvt_[] = {
    { (QSignal)QEP_EMPTY_SIG_, (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_ENTRY_SIG,    (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_EXIT_SIG,     (uint8_t)0, (uint8_t)0 },
    { (QSignal)Q_INIT_SIG,     (uint8_t)0, (uint8_t)0 }
};

/*..........................................................................*/
char_t const Q_ROM * Q_ROM_VAR QEP_getVersion(void) {
    static char_t const Q_ROM Q_ROM_VAR version[] = {
        (char_t)((uint8_t)((QP_VERSION >> 12) & 0xFU) + (uint8_t)'0'),
        (char_t)'.',
        (char_t)((uint8_t)((QP_VERSION >>  8) & 0xFU) + (uint8_t)'0'),
        (char_t)'.',
        (char_t)((uint8_t)((QP_VERSION >>  4) & 0xFU) + (uint8_t)'0'),
        (char_t)((uint8_t)(QP_VERSION         & 0xFU) + (uint8_t)'0'),
        (char_t)'\0'
    };
    return version;
}
