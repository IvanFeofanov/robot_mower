#ifndef PIO_H
#define PIO_H

#include <avr/io.h>
#include <stdint.h>

#define MAKE_PIO(ddr, port, pin, class_name)                                \
    typedef struct class_name                                               \
    {                                                                       \
        typedef uint8_t DataType;                                           \
                                                                            \
        static inline void      write(DataType data)    { port = data; }    \
        static inline DataType  read(DataType data)     { return port; }    \
        static inline void      dirWrite(DataType data) { ddr = data; }     \
        static inline DataType  dirRead(DataType data)  { return ddr; }     \
        static inline void      set(DataType data)      { port |= data; }   \
        static inline void      clear(DataType data)    { port &= ~data; }  \
        static inline void      toggle(DataType data)   { port ^= data; }   \
        static inline void      dirSet(DataType data)   { ddr |= data; }    \
        static inline void      dirClear(DataType data) { ddr &= ~data; }   \
        static inline void      dirToggle(DataType data){ ddr ^= data; }    \
        static inline DataType  pinRead()               { return pin; }     \
    }class_name;

enum PortBit
{
    BIT_0,
    BIT_1,
    BIT_2,
    BIT_3,
    BIT_4,
    BIT_5,
    BIT_6,
    BIT_7
};

template
<
    typename UNIT,
    PortBit  BIT
>
struct Pin
{
    static const PortBit pin_number = BIT;

    typedef UNIT Port;

    static inline void setHigh()
    {
        UNIT::set(1<<BIT);
    }
    static inline void setLow()
    {
        UNIT::clear(1<<BIT);
    }
    static inline void toggle()
    {
        UNIT::toggle(1<<BIT);
    }
    static inline void setInput()
    {
        UNIT::dirClear(1<<BIT);
    }
    static inline void setOutput()
    {
        UNIT::dirSet(1<<BIT);
    }
    static inline void dirToggle()
    {
        UNIT::dirToggle(1<<BIT);
    }
    static inline bool isHigh()
    {
        return UNIT::pinRead() & (1<<BIT);
    }
};

#ifdef PORTA
    MAKE_PIO(DDRA, PORTA, PINA, UnitA)

#ifdef DDA0
    typedef Pin<UnitA, BIT_0> PioA0;
#endif
#ifdef DDA1
    typedef Pin<UnitA, BIT_1> PioA1;
#endif
#ifdef DDA2
    typedef Pin<UnitA, BIT_2> PioA2;
#endif
#ifdef DDA3
    typedef Pin<UnitA, BIT_3> PioA3;
#endif
#ifdef DDA4
    typedef Pin<UnitA, BIT_4> PioA4;
#endif
#ifdef DDA5
    typedef Pin<UnitA, BIT_5> PioA5;
#endif
#ifdef DDA6
    typedef Pin<UnitA, BIT_6> PioA6;
#endif
#ifdef DDA7
    typedef Pin<UnitA, BIT_7> PioA7;
#endif

#endif

#ifdef PORTB
    MAKE_PIO(DDRB, PORTB, PINB, UnitB)

#ifdef DDB0
    typedef Pin<UnitB, BIT_0> PioB0;
#endif
#ifdef DDB1
    typedef Pin<UnitB, BIT_1> PioB1;
#endif
#ifdef DDB2
    typedef Pin<UnitB, BIT_2> PioB2;
#endif
#ifdef DDB3
    typedef Pin<UnitB, BIT_3> PioB3;
#endif
#ifdef DDB4
    typedef Pin<UnitB, BIT_4> PioB4;
#endif
#ifdef DDB5
    typedef Pin<UnitB, BIT_5> PioB5;
#endif
#ifdef DDB6
    typedef Pin<UnitB, BIT_6> PioB6;
#endif
#ifdef DDB7
    typedef Pin<UnitB, BIT_7> PioB7;
#endif

#endif

#ifdef PORTC
    MAKE_PIO(DDRC, PORTC, PINC, UnitC)

#ifdef DDC0
    typedef Pin<UnitC, BIT_0> PioC0;
#endif
#ifdef DDC1
    typedef Pin<UnitC, BIT_1> PioC1;
#endif
#ifdef DDC2
    typedef Pin<UnitC, BIT_2> PioC2;
#endif
#ifdef DDC3
    typedef Pin<UnitC, BIT_3> PioC3;
#endif
#ifdef DDC4
    typedef Pin<UnitC, BIT_4> PioC4;
#endif
#ifdef DDC5
    typedef Pin<UnitC, BIT_5> PioC5;
#endif
#ifdef DDC6
    typedef Pin<UnitC, BIT_6> PioC6;
#endif
#ifdef DDC7
    typedef Pin<UnitC, BIT_7> PioC7;
#endif

#endif

#ifdef PORTD
    MAKE_PIO(DDRD, PORTD, PIND, UnitD)

#ifdef DDD0
    typedef Pin<UnitD, BIT_0> PioD0;
#endif
#ifdef DDD1
    typedef Pin<UnitD, BIT_1> PioD1;
#endif
#ifdef DDD2
    typedef Pin<UnitD, BIT_2> PioD2;
#endif
#ifdef DDD3
    typedef Pin<UnitD, BIT_3> PioD3;
#endif
#ifdef DDD4
    typedef Pin<UnitD, BIT_4> PioD4;
#endif
#ifdef DDD5
    typedef Pin<UnitD, BIT_5> PioD5;
#endif
#ifdef DDD6
    typedef Pin<UnitD, BIT_6> PioD6;
#endif
#ifdef DDD7
    typedef Pin<UnitD, BIT_7> PioD7;
#endif

#endif


#endif
