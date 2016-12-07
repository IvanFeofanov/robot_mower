#include "uart.h"

ISR(USART_RX_vect)
{
    Serial::receptionComplate();
}

ISR(USART_UDRE_vect)
{
    Serial::dataRegisterEmpty();
}

ISR(USART_TX_vect)
{
    Serial::transferComplate();
}
