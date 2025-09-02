
#include "qep_pkg.h"
#include "qassert.h"

Q_DEFINE_THIS_MODULE("qhsm_dis")


/**
 * @brief 状态机事件分发函数
 * @param[in,out] me 指向状态机对象的指针（const 修饰防止意外修改）
 * @param[in] e     指向待分发事件的指针
 */
void QHsm_dispatch(QHsm * const me, QEvt const * const e) {
    QStateHandler t = me->state;  // 当前状态（临时保存）
    QStateHandler s;             // 用于遍历状态层次
    QState r;                    // 状态处理函数的返回值
    QS_CRIT_STAT_                // 临界区保护（如果启用 QS 调试）

    // 确保状态配置稳定（temp 必须与 state 一致） 防止在状态转换的过程去操作
    Q_REQUIRE(t == me->temp);

    // 调试信息：记录事件分发（QS 调试工具）
    QS_BEGIN_(QS_QEP_DISPATCH, QS_smObj_, me)
        QS_TIME_();                                     // 时间戳
        QS_SIG_(e->sig);                               // 事件信号
        QS_OBJ_(me);                                   // 状态机对象
        QS_FUN_(t);                                    // 当前状态
    QS_END_()

    // 事件处理循环：从当前状态开始，逐级向上处理事件
    do {
        s = me->temp;                                  // 保存当前状态到 s
        r = (*s)(me, e);                               // 调用状态处理函数

        // 如果事件未被处理（因 guard 条件不满足）
        if (r == Q_RET_UNHANDLED) {
            // 调试信息：记录未处理事件
            QS_BEGIN_(QS_QEP_UNHANDLED, QS_smObj_, me)
                QS_SIG_(e->sig);                       // 事件信号
                QS_OBJ_(me);                           // 状态机对象
                QS_FUN_(s);                            // 当前状态
            QS_END_()

            // 触发空信号，查找父状态（QEP_EMPTY_SIG_ 用于查询层次结构）
            r = QEP_TRIG_(s, QEP_EMPTY_SIG_);
        }
    } while (r == Q_RET_SUPER);                        // 继续处理父状态

    // 如果返回值是 Q_RET_TRAN，表示需要状态转换
    if (r == Q_RET_TRAN) {
        QStateHandler path[QEP_MAX_NEST_DEPTH_];       // 存储状态进入路径
        int8_t ip = (int8_t)(-1);                      // 路径索引（初始为 -1）
        int8_t iq;                                     // 辅助索引
#ifdef Q_SPY
        QStateHandler src = s;                         // 保存转换源状态（用于调试）
#endif

        // 保存转换目标和当前状态到路径数组
        path[0] = me->temp;                            // 目标状态
        path[1] = t;                                   // 当前状态（源）

        // 退出当前状态，直到找到转换源状态（s）回溯查找
        while (t != s) {
            // 触发退出信号（Q_EXIT_SIG）
            if (QEP_TRIG_(t, Q_EXIT_SIG) == Q_RET_HANDLED) {
                // 调试信息：记录状态退出
                QS_BEGIN_(QS_QEP_STATE_EXIT, QS_smObj_, me)
                    QS_OBJ_(me);                       // 状态机对象
                    QS_FUN_(t);                        // 退出的状态
                QS_END_()

                // 触发空信号，查找父状态
                (void)QEP_TRIG_(t, QEP_EMPTY_SIG_);
            }
            t = me->temp;                              // 更新为父状态
        }

        t = path[0];                                   // 目标状态

								//确认ip也就是进入深度
        // 情况 (a): 源状态 == 目标状态（自转换）    退出再进入
        if (s == t) {
            QEP_EXIT_(s);                              // 退出源状态
            ip = (int8_t)0;                            // 从目标状态开始进入
        }
        else {
            // 触发空信号，查找目标状态的父状态
            (void)QEP_TRIG_(t, QEP_EMPTY_SIG_);
            t = me->temp;

            // 情况 (b): 源状态的父状态 == 目标状态的父状态
            if (s == t) {
                ip = (int8_t)0;                        // 从目标状态开始进入
            }
            else {
                // 触发空信号，查找源状态的父状态
                (void)QEP_TRIG_(s, QEP_EMPTY_SIG_);

                // 情况 (c): 源状态的父状态 == 目标状态的父状态的父状态
                if (me->temp == t) {
                    QEP_EXIT_(s);                      // 退出源状态
                    ip = (int8_t)0;                    // 从目标状态开始进入
                }
                else {
                    // 情况 (d): 源状态的父状态 == 目标状态
                    if (me->temp == path[0]) {
                        QEP_EXIT_(s);                  // 退出源状态
                    }
                    else {
                        // 情况 (e): 其他复杂层次关系
                        iq = (int8_t)0;                // 标记未找到最低公共祖先（LCA）
                        ip = (int8_t)1;                // 从目标状态及其父状态开始进入
                        path[1] = t;                   // 保存目标状态的父状态
                        t = me->temp;                  // 保存源状态的父状态

                        // 查找目标状态的更高层父状态
                        r = QEP_TRIG_(path[1], QEP_EMPTY_SIG_);
                        while (r == Q_RET_SUPER) {
                            ++ip;
                            path[ip] = me->temp;       // 存储进入路径
                            if (me->temp == s) {       // 是否找到源状态？
                                iq = (int8_t)1;        // 标记找到 LCA
                                Q_ASSERT(ip < (int8_t)QEP_MAX_NEST_DEPTH_); // 检查路径深度
                                --ip;                  // 不进入源状态
                                r = Q_RET_HANDLED;     // 终止循环
                            }
                            else {
                                r = QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);
                            }
                        }

                        // 如果仍未找到 LCA
                        if (iq == (int8_t)0) {
                            Q_ASSERT(ip < (int8_t)QEP_MAX_NEST_DEPTH_);
                            QEP_EXIT_(s);              // 退出源状态

                            // 情况 (f): 继续查找更高层父状态
                            iq = ip;
                            r = Q_RET_IGNORED;
                            do {
                                if (t == path[iq]) {   // 找到 LCA？
                                    r = Q_RET_HANDLED;
                                    ip = (int8_t)(iq - (int8_t)1);
                                    iq = (int8_t)(-1); // 终止循环
                                }
                                else {
                                    --iq;              // 尝试更低层目标父状态
                                }
                            } while (iq >= (int8_t)0);

                            // 情况 (g): 遍历所有可能的父状态组合
                            if (r != Q_RET_HANDLED) {
                                r = Q_RET_IGNORED;
                                do {
                                    // 退出当前状态（未处理）
                                    if (QEP_TRIG_(t, Q_EXIT_SIG) == Q_RET_HANDLED) {
                                        QS_BEGIN_(QS_QEP_STATE_EXIT, QS_smObj_, me)
                                            QS_OBJ_(me);
                                            QS_FUN_(t);
                                        QS_END_()
                                        (void)QEP_TRIG_(t, QEP_EMPTY_SIG_);
                                    }
                                    t = me->temp;     // 更新为父状态
                                    iq = ip;
                                    do {
                                        if (t == path[iq]) { // 找到 LCA？
                                            ip = (int8_t)(iq - (int8_t)1);
                                            iq = (int8_t)(-1);
                                            r = Q_RET_HANDLED;
                                        }
                                        else {
                                            --iq;
                                        }
                                    } while (iq >= (int8_t)0);
                                } while (r != Q_RET_HANDLED);
                            }
                        }
                    }
                }
            }
        }

        // 反向遍历通过获取的ip进入路径，依次进入目标状态及其父状态
        for (; ip >= (int8_t)0; --ip) {
            QEP_ENTER_(path[ip]);                       // 进入路径中的状态
        }
        t = path[0];                                   // 最终目标状态
        me->temp = t;                                  // 更新临时状态

        // 处理目标状态的初始转换（Q_INIT_SIG）
        while (QEP_TRIG_(t, Q_INIT_SIG) == Q_RET_TRAN) {
            // 调试信息：记录状态初始化
            QS_BEGIN_(QS_QEP_STATE_INIT, QS_smObj_, me)
                QS_OBJ_(me);                           // 状态机对象
                QS_FUN_(t);                            // 源状态（伪状态）
                QS_FUN_(me->temp);                     // 目标状态
            QS_END_()

            ip = (int8_t)0;
            path[0] = me->temp;

            // 查找目标状态的父状态
            (void)QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);
            while (me->temp != t) {
                ++ip;
                path[ip] = me->temp;
                (void)QEP_TRIG_(me->temp, QEP_EMPTY_SIG_);
            }
            me->temp = path[0];

            // 检查路径深度是否溢出
            Q_ASSERT(ip < (int8_t)QEP_MAX_NEST_DEPTH_);

            // 反向进入初始状态层次
            do {
                QEP_ENTER_(path[ip]);                   // 进入状态
                --ip;
            } while (ip >= (int8_t)0);

            t = path[0];
        }

        // 调试信息：记录状态转换完成
        QS_BEGIN_(QS_QEP_TRAN, QS_smObj_, me)
            QS_TIME_();                                 // 时间戳
            QS_SIG_(e->sig);                           // 事件信号
            QS_OBJ_(me);                               // 状态机对象
            QS_FUN_(src);                              // 转换源状态
            QS_FUN_(t);                                // 转换目标状态
        QS_END_()
    }
    else {
        // 转换未发生（事件被处理或忽略）
#ifdef Q_SPY
        if (r == Q_RET_HANDLED) {
            // 调试信息：记录内部转换（同一状态内处理）
            QS_BEGIN_(QS_QEP_INTERN_TRAN, QS_smObj_, me)
                QS_TIME_();
                QS_SIG_(e->sig);
                QS_OBJ_(me);
                QS_FUN_(me->state);
            QS_END_()
        }
        else {
            // 调试信息：记录事件被忽略
            QS_BEGIN_(QS_QEP_IGNORED, QS_smObj_, me)
                QS_TIME_();
                QS_SIG_(e->sig);
                QS_OBJ_(me);
                QS_FUN_(me->state);
            QS_END_()
        }
#endif
    }

    // 更新状态机的当前状态和临时状态
    me->state = t;                                     // 设置新状态
    me->temp  = t;                                     // 标记配置稳定
}
