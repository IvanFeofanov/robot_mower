#ifndef DEBUG_H
#define DEBUG_H

#include <string.h>
#include <stdlib.h>

#define BIN 2
#define OCT 8
#define DEC 10
#define HEX 16

class NoDebug
{
public:
    static void print(const char* str)
    {
    }

    static void print(int value, int redix = DEC)
    {
    }

    static void print(unsigned int value, int redix = DEC)
    {
    }

    static void print(unsigned long value, int redix = DEC)
    {
    }

    static void print(long value, int redix = DEC)
    {
    }

    static void println()
    {
    }
};

template<typename Serial>
class SerialDebug
{
public:
    static void print(const char* str)
    {
        uint16_t len = strlen(str);

        while(Serial::txAvailable() < len + 1);

        Serial::write(str, len);
    }

    static void print(int value, int redix = DEC)
    {
        char buffer[sizeof(int) * 8 + 1];
        itoa(value, buffer, redix);
        print(buffer);
    }

    static void print(unsigned int value, int redix = DEC)
    {
        char buffer[sizeof(unsigned int) * 8 + 1];
        utoa(value, buffer, redix);
        print(buffer);
    }

    static void print(unsigned long value, int redix = DEC)
    {
        char buffer[sizeof(unsigned long) * 8 + 1];
        ultoa(value, buffer, redix);
        print(buffer);
    }

    static void print(long value, int redix = DEC)
    {
        char buffer[sizeof(long) * 8 + 1];
        ltoa(value, buffer, redix);
        print(buffer);
    }

    static void println()
    {
        print("\n\r");
    }
};

#endif
