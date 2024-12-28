#include <libopencm3/stm32/memorymap.h>

#include "common-defines.h"

#define BOOTLOADER_SIZE         (0x8000U)
#define MAIN_APP_START_ADDRESS  (FLASH_BASE + BOOTLOADER_SIZE)

/* Jump to program entry point */
static void bootloader_jump_to_main(void)
{
        typedef void (*void_fn)(void);
        /*
         * uint32_t* is a pointer to reset_vector_entry
         * Value is the MAIN_APP_START_ADDRESS + 4U to get the second entry.
         * First entry is supposed to be the stack pointer.
         * reset_vector_entry is a pointer to a location in memory that contains
         * the address of the reset handler (entry point).
         */
        uint32_t* reset_vector_entry = (uint32_t*)(MAIN_APP_START_ADDRESS) + 4U;
        /*
         * Read value at address of reset_vector_entry
         * Interpret value as pointer.
         * Call it reset_vector
         */
        uint32_t* reset_vector = (uint32_t*)(*reset_vector_entry);
        /*
         * Value of jump_fn is being interpreted as a function
         */
        void_fn jump_fn = (void_fn)reset_vector;
        jump_fn();
}


int main(void)
{
        bootloader_jump_to_main();
        return 0;
}