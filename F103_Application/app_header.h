/*
 * app_header.h
 *
 */

#ifndef INC_APP_HEADER_H_
#define INC_APP_HEADER_H_

#include "flash_layout.h"

typedef struct
{
    uint32_t ota_flag;  // OTA flag
    uint32_t magic;     // Magic number identifier
    uint32_t app_size;  // Size of the application in bytes 
    uint32_t app_crc;   // CRC32 of the application
    uint32_t version;   // Application version
} app_header_t;
 
#endif /* INC_APP_HEADER_H_ */