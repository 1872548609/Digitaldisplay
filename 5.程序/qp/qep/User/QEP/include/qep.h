
#ifndef qep_h
#define qep_h


#include "qevt.h"             /* QEP事件处理器需要QEvt事件设施 */


char_t const Q_ROM * Q_ROM_VAR QEP_getVersion(void);

typedef uint8_t QState;     /* 状态返回值类型（8位无符号整数） */

         
/**
* \brief 状态处理函数指针类型（State Handler Function Pointer）。
*
* 这是一个函数指针类型，用于定义状态机（HSM/FSM）中状态的处理函数。  
* 每个状态处理函数必须符合此签名，以便 QEP 事件处理器能够统一调用它们。
*
* \param me  指向状态机对象（当前状态机实例）的指针（通常为 `void*` 以实现泛型）。
* \param e   指向当前事件的指针（`QEvt const*`），表示触发状态处理的事件。
*
* \return 返回 `QState` 类型的状态返回值，通常包括：
*         - `Q_RET_HANDLED`：事件已处理。
*         - `Q_RET_IGNORED`：事件被忽略。
*         - `Q_RET_TRANSITION`：发生状态转移。
*         - `Q_RET_SUPER`：委托给父状态处理。
*         - 其他框架定义的返回值（如 `Q_RET_UNHANDLED`）。
*
* \note 状态处理函数是 Hierarchical State Machine (HSM) 的核心组件，
*       负责处理进入/退出动作、事件分发和状态转移逻辑。
*/									
typedef QState (*QStateHandler)(void * const me, QEvt const * const e);       //状态处理函数结构声明
/*典型用例
// 定义状态处理函数 
QState MyState_handler(void * const me, QEvt const * const e) {
    switch (e->sig) {
        case Q_ENTRY_SIG:
            // 处理进入动作 
            return Q_RET_HANDLED;
        case Q_EXIT_SIG:
            // 处理退出动作 
            return Q_RET_HANDLED;
        case BUTTON_PRESS_SIG:
            // 处理按钮事件，可能触发状态转移 
            return Q_TRAN(&MyOtherState);
        default:
            // 事件未处理，委托给父状态 
            return Q_RET_SUPER;
    }
}
 
//在状态机中注册状态处理函数 
QHsm_ctor(&myHsm, (QStateHandler)&MyState_handler);
*/

#define Q_STATE_CAST(handler_)  ((QStateHandler)(handler_))       //定义一个用来比较

typedef struct QFsmTag {           ///有限状态机
    QStateHandler state;          /**< 当前活跃状态（状态变量） */
    QStateHandler temp; 										/**< 临时状态：目标状态或父状态 */
} QFsm;


#define QFsm_ctor(me_, initial_) do { \
    (me_)->state = Q_STATE_CAST(0); \
    (me_)->temp  = (initial_); \
} while (0)


void QFsm_init(QFsm * const me, QEvt const * const e);


void QFsm_dispatch(QFsm * const me, QEvt const * const e);



typedef struct QFsmTag QHsm;


#define QHsm_ctor(me_, initial_) do { \
    (me_)->state = Q_STATE_CAST(&QHsm_top); \
    (me_)->temp  = (initial_); \
} while (0)


#define QHsm_state(me_) (Q_STATE_CAST((me_)->state))

//初始化层次状态机
void QHsm_init(QHsm * const me, QEvt const * const e);


void QHsm_dispatch(QHsm * const me, QEvt const * const e);

//顶层状态函数
QState QHsm_top(void const * const me, QEvt const * const e);

//状态函数执行完后返回的状态值定义 ======================

#define Q_RET_HANDLED       ((QState)0)


#define Q_HANDLED()         (Q_RET_HANDLED)

#define Q_RET_IGNORED       ((QState)1)


#define Q_IGNORED()         (Q_RET_IGNORED)

#define Q_RET_TRAN          ((QState)2)


#define Q_TRAN(target_)  \
    (((QFsm *)me)->temp = Q_STATE_CAST(target_), Q_RET_TRAN)


#define Q_RET_SUPER         ((QState)3)


#define Q_SUPER(super_)  \
    (((QHsm *)me)->temp = Q_STATE_CAST(super_),  Q_RET_SUPER)


#define Q_RET_UNHANDLED    ((QState)4)


#define Q_UNHANDLED()      (Q_RET_UNHANDLED)

//执行信号常量枚举 ======================

enum QReservedSignals {
    Q_ENTRY_SIG = 1,                   /**< signal for coding entry actions */
    Q_EXIT_SIG,                         /**< signal for coding exit actions */
    Q_INIT_SIG,                  /**< signal for coding initial transitions */
    Q_USER_SIG      /**< first signal that can be used in user applications */
};

#endif                                                             /* qep_h */
