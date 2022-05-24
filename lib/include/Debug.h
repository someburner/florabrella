#ifndef DEBUG__h
#define DEBUG__h

#if SERIAL_EN
    #define SERIAL_BAUD                 115200 //19200 //57600 //115200 // 500000
    #define DEBUGbegin(input)           Serial.begin(input)
    #define DEBUG(input)                Serial.print(input)
    #define DEBUGln(input)              Serial.println(input)
    #define DEBUGf(input)               Serial.print( F(input) )
    #define DEBUGfln(input)             Serial.println( F(input) )
    #define DEBUGval(input, format)     Serial.print(input, format)
    #define DEBUGvalln(input, format)   Serial.println(input, format)
    #define DEBUGflush(...)             Serial.flush()
    #define PRVAR(x) \
    do { \
        DEBUG( F( #x ": " ) ); \
        DEBUGln(x); \
    } while(0);
#else
    #define SERIAL_BAUD                 115200 //19200 //57600 //115200 // 500000
    #define DEBUGbegin(input)           do { } while(0)
    #define DEBUG(input)                do { } while(0)
    #define DEBUGln(input)              do { } while(0)
    #define DEBUGf(input)               do { } while(0)
    #define DEBUGfln(input)             do { } while(0)
    #define DEBUGval(input, format)     do { } while(0)
    #define DEBUGvalln(input, format)   do { } while(0)
    #define DEBUGflush(...)             do { } while(0)
#endif /* SERIAL_EN */

#if SERIAL_EN && RADIO_DEBUG_EN
    #define RADIO_DBG                       DEBUG
    #define RADIO_DBGln                     DEBUGln
    #define RADIO_DBGval                    DEBUGval
    #define RADIO_DBGvalln                  DEBUGvalln
#else
    #define RADIO_DBG(input)                do { } while(0)
    #define RADIO_DBGln(input)              do { } while(0)
    #define RADIO_DBGval(input, format)     do { } while(0)
    #define RADIO_DBGvalln(input, format)   do { } while(0)
#endif


#endif /* DEBUG__h */
