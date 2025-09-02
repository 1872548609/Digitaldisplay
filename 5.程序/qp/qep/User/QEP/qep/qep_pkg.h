#ifndef qep_pkg_h
#define qep_pkg_h


#include "qep_port.h"                                           /* QEP port */

extern QEvt const QEP_reservedEvt_[4];    //* 预分配的保留事件数组 */

/** internal QEP constants */
enum QEPConst {
    QEP_EMPTY_SIG_ = 0,   /**< 空信号，仅供内部使用 */

    /** maximum depth of state nesting (including the top level),
     * must be >= 3
     */
    QEP_MAX_NEST_DEPTH_ = 6        /* 状态嵌套的最大深度 */ 
};

 /* 触发状态处理函数 */
#define QEP_TRIG_(state_, sig_) \
    ((*(state_))(me, &QEP_reservedEvt_[(sig_)]))

/** helper macro to trigger exit action in an HSM */
#define QEP_EXIT_(state_) do { \
    if (QEP_TRIG_((state_), Q_EXIT_SIG) == Q_RET_HANDLED) { \
        QS_BEGIN_(QS_QEP_STATE_EXIT, QS_smObj_, me) \
            QS_OBJ_(me); \
            QS_FUN_(state_); \
        QS_END_() \
    } \
} while (0)   /* 触发状态退出动作并记录日志 */

/** 如果状态实现了 Q_ENTRY_SIG 的处理逻辑并返回 Q_RET_HANDLED，表示进入动作已执行。 */
#define QEP_ENTER_(state_) do { \
    if (QEP_TRIG_((state_), Q_ENTRY_SIG) == Q_RET_HANDLED) { \
        QS_BEGIN_(QS_QEP_STATE_ENTRY, QS_smObj_, me) \
            QS_OBJ_(me); \
            QS_FUN_(state_); \
        QS_END_() \
    } \
} while (0)

#endif                                                         /* qep_pkg_h */

/****************************************************************************/
#ifdef Q_SPY                                /* QS software tracing enabled? */
    #include "qs_port.h"                                 /* include QS port */
#else
    #include "qs_dummy.h"                /* disable the QS software tracing */
#endif                                                             /* Q_SPY */

