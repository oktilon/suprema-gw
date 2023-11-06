#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

const char * server_error_message (int error);
const char * server_card_type (int type);
const char * server_connection_mode (int type);
const char * server_device_type (int type);
const char * device_support_info (int flags);
const char * event_code (uint16_t code);
const char * rs485_mode (int mode);
const char * door_flag (int flags);
const char * format_date (time_t time);
const char * join_uint32_array (uint32_t *arr, const char *del, uint32_t count);
const char * card_data (uint8_t *data);


#endif // COMMON_H