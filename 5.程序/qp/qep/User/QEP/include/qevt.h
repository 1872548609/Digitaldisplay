#ifndef qevt_h
#define qevt_h


#define QP_VERSION      0x4504U

#ifndef Q_ROM
    
    #define Q_ROM
#endif
#ifndef Q_ROM_VAR        

    #define Q_ROM_VAR
#endif
#ifndef Q_ROM_BYTE
   
    #define Q_ROM_BYTE(rom_var_)   (rom_var_)
#endif

#ifndef Q_SIGNAL_SIZE

   
    #define Q_SIGNAL_SIZE 2
#endif
#if (Q_SIGNAL_SIZE == 1)
    typedef uint8_t QSignal;
#elif (Q_SIGNAL_SIZE == 2)
   
    typedef uint16_t QSignal;
#elif (Q_SIGNAL_SIZE == 4)
    typedef uint32_t QSignal;
#else
    #error "Q_SIGNAL_SIZE defined incorrectly, expected 1, 2, or 4"
#endif

typedef char char_t;

typedef float float32_t;

typedef double float64_t;

typedef int enum_t;


typedef struct QEvtTag {
    QSignal sig;                          /**< signal of the event instance */
    uint8_t poolId_;                      /**< pool ID (0 for static event) */
    uint8_t refCtr_;                                 /**< reference counter */
} QEvt;

#ifdef Q_EVT_CTOR            /* Provide the constructor for the QEvt class? */

QEvt *QEvt_ctor(QEvt * const me, enum_t const sig);

#endif


#define Q_EVT_CAST(class_) ((class_ const *)e)


#define Q_DIM(array_) (sizeof(array_) / sizeof(array_[0]))


#define Q_UINT2PTR_CAST(type_, uint_)  ((type_ *)(uint_))


#ifndef Q_NQEVENT
    typedef QEvt QEvent;          /**< typedef for backwards compatibility */
#endif

#endif                                                            /* qevt_h */
