#include <random_num.h>
#include <stdint.h>
#include <printk.h>

/**
 * @brief Function for getting randomly initialized value
 * @return initialized random value
**/
uint8_t start_random() {
    uint32_t random_base_address = 0x4000D000;

    uint32_t* start_address = (uint32_t*)random_base_address;

    // trigger random generator
    *start_address = 1;

    // get value from event register
    uint32_t* event_address = (uint32_t*)(random_base_address + 0x100);

    while (1) {
        uint8_t bit_value = *event_address & 1;
        if (bit_value) {
            break;
        }
    }

    uint32_t* value_address = (uint32_t*)(random_base_address + 0x508);
    uint8_t result = *value_address & 0xff;

    return result;
}






