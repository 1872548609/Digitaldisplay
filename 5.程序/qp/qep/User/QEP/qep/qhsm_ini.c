#include "qep_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qhsm_ini")

/**
 * @brief 初始化层次状态机（HSM）并执行初始转换
 * @param[in,out] me 指向状态机对象的指针（const指针保证对象本身不被修改）
 * @param[in] e     指向初始事件的指针（通常为空或特定初始化事件）
 * 
 * 该函数完成以下工作：
 * 1. 验证状态机初始条件
 * 2. 执行顶层初始转换（必须返回Q_RET_TRAN）
 * 3. 处理嵌套状态的进入路径
 * 4. 处理可能的嵌套初始转换（Q_INIT_SIG触发）
 * 5. 记录状态机初始化完成
 * 6. 更新状态机当前状态
 */
	
	
/*
state：
表示 当前有效状态（current active state），即状态机当前所处的状态。
在 HSM（层次状态机）中，state 可能指向一个 复合状态（包含父状态和子状态）。
初始时，state 通常被设置为 顶层初始状态（如 s_initial 或 s_top）。
temp：
表示 目标状态（target state），即在状态转换过程中计算出的下一个状态。
在状态转换逻辑中，temp 会被赋值为新的状态（如 s_off、s_on），最终可能成为新的 state。
如果转换被拒绝（如守护条件不满足），temp 可能被忽略，state 保持不变。
*/

void QHsm_init(QHsm * const me, QEvt const * const e) {
    QStateHandler t = me->state; //保存当前状态（初始应为 QHsm_top）
	
    QS_CRIT_STAT_      // 临界区声明（用于QS调试工具）

    Q_REQUIRE((me->temp != Q_STATE_CAST(0))        /* 必须非零（表示构造函数已执行） */
              && (t == Q_STATE_CAST(&QHsm_top))); /*当前状态必须是 QHsm_top（初始转换未执行过）。 */
				
                           
    Q_ALLEGE((*me->temp)(me, e) == Q_RET_TRAN);  /*执行顶层初始转换（必须返回Q_RET_TRAN） */

    do {        
					
        QStateHandler path[QEP_MAX_NEST_DEPTH_];      // 用于存储进入路径的缓冲区（深度受QEP_MAX_NEST_DEPTH_限制）
					
        int8_t ip = (int8_t)0;               // 进入路径索引初始为0

        QS_BEGIN_(QS_QEP_STATE_INIT, QS_smObj_, me)        // 记录初始转换过程（QS调试工具） 默认是不启动的
            QS_OBJ_(me);                      // 状态机对象
            QS_FUN_(t);                                 // 源状态（应为QHsm_top）
            QS_FUN_(me->temp);      // 初始转换的目标状态
        QS_END_()

        path[0] = me->temp;       // 构建进入路径：从目标状态回溯到当前状态
					
        (void)QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);        // 触发空信号以获取父状态（构建进入路径）
					
        while (me->temp != t) {  // 直到回溯到当前状态（QHsm_top）
            ++ip;
            path[ip] = me->temp;                        // 记录路径中的每个父状态
            (void)QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);     // 继续向上查询
        }
        me->temp = path[0];
                                            
        Q_ASSERT(ip < (int8_t)QEP_MAX_NEST_DEPTH_);      /* 断言路径深度未超过最大嵌套深度（防止栈溢出）。 */

        do {        /*反向遍历 path：从根状态（path[ip]）到叶状态，依次调用 QEP_ENTER_。 */
            QEP_ENTER_(path[ip]);                         /* enter path[ip] */
            --ip;
        } while (ip >= (int8_t)0);

        t = path[0];                /* 当前状态为叶状态。 */
    } while (QEP_TRIG_(t, Q_INIT_SIG) == Q_RET_TRAN);     //触发 INIT 信号

    QS_BEGIN_(QS_QEP_INIT_TRAN, QS_smObj_, me)     //记录初始化完成后的最终状态。
        QS_TIME_();                                           /* time stamp */
        QS_OBJ_(me);                           /* this state machine object */
        QS_FUN_(t);                                 /* the new active state */
    QS_END_()

    me->state = t;                       /* c设置为当前活动状态 t */
    me->temp  = t;                      /* 标记配置稳定（防止重复初始化） */
}
