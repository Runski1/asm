#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

__attribute__((naked)) int prt(const char *a) {
    // the pointer to a character array is passed to the
    // subroutine in R0
    // remember to follow AAPCS:
    // push any register above r3 at the beginning and pop the same registers at
    // the end note: lr needs to be pushed because we are calling another
    // subroutine
    // cortex-M0 requires popping to PC if LR was pushed. See the code below
    asm volatile(
        "push { r4, lr } \n" // we need to save return address because we call
                             // another subroutine
        "mov r4, r0 \n" // Copy the ch pointer to r4
        "loop: \n"
        "ldrb r0, [r4] \n" // load character to r0
        "cmp r0, #0\n" 
        "beq end \n" // if ch == 0 goto end
        "bl putchar \n" // else putchar
        "add r4, r4, #1 \n" // increment ch pointer
        "b loop \n" // loop while ch != 0
        "end: \n"
        "pop { r4, pc } \n" // cortex-M0 requires popping to PC if LR was pushed
        // popping to PC will cause return from subroutine (~same as "bx lr")
    );
}
void fail() {
    printf("Failed\n"); // set a break point here
    while (1) {
        tight_loop_contents();
    }
}

void ok() {
    printf("\nAll ok\n"); // set a break point here
}

int main(void) {

    const uint led_pin = 22;

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();

    // TODO: insert code here
    printf("\nExercise5\n");

    char test1[] = "Computer Architecture\n";
    char test2[] = "Computer Architecture\n";
    printf("test 1 begin\n");
    prt(test1);
    printf("test 1 done\n");
    if (strcmp(test1, test2)) {
        fail(); // error - string modified
    }
    char test3[] = "Johnny Ca$h:Live @Folsom\n";
    char test4[] = "Johnny Ca$h:Live @Folsom\n";
    printf("test 2 begin\n");
    prt(test3);
    printf("test 2 done\n");
    if (strcmp(test3, test4)) {
        fail(); // error - string modified
    }

    char test5[] = "If you like to gamble, I tell you I'm your man\n";
    char test6[] = "If you like to gamble, I tell you I'm your man\n";
    printf("test 3 begin\n");
    prt(test5);
    printf("test 3 done\n");
    if (strcmp(test5, test6)) {
        fail(); // error - string modified
    }

    ok();

    // Loop forever
    while (true) {
        // Blink LED
        gpio_put(led_pin, true);
        sleep_ms(1000);
        gpio_put(led_pin, false);
        sleep_ms(1000);
    }
    return 0;
}
