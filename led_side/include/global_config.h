#ifndef GLOBAL_CONFIG_H /* include guards */
#define GLOBAL_CONFIG_H

#define DBG_PRT 0
#define DBGPRINT(...) if(DBG_PRT == 1) Serial.printf(__VA_ARGS__);
#define PRINT(...) Serial.printf(__VA_ARGS__);

#endif /* GLOBAL_CONFIG_H */