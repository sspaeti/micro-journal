
#include "latin.h"
#include "../locale.h"

uint8_t hid_keycode_to_ascii_latin[][3] = {

    {0, 0, 0},           /* 0x00 */
    {0, 0, 0},           /* 0x01 */
    {0, 0, 0},           /* 0x02 */
    {0, 0, 0},           /* 0x03 */
    {'a', 'A', 0},       /* 0x04 */
    {'b', 'B', 0},       /* 0x05 */
    {'c', 'C', 0},       /* 0x06 */
    {'d', 'D', 0},       /* 0x07 */
    {'e', 'E', 128},     /* 0x08 */
    {'f', 'F', 0},       /* 0x09 */
    {'g', 'G', 0},       /* 0x0a */
    {'h', 'H', 0},       /* 0x0b */
    {'i', 'I', 0},       /* 0x0c */
    {'j', 'J', 0},       /* 0x0d */
    {'k', 'K', 0},       /* 0x0e */
    {'l', 'L', 0},       /* 0x0f */
    {'m', 'M', 0},       /* 0x10 */
    {'n', 'N', 0},       /* 0x11 */
    {'o', 'O', 0},       /* 0x12 */
    {'p', 'P', 0},       /* 0x13 */
    {'q', 'Q', '@'},       /* 0x14 */
    {'r', 'R', 0},       /* 0x15 */
    {'s', 'S', 0},       /* 0x16 */
    {'t', 'T', 0},       /* 0x17 */
    {'u', 'U', 0},       /* 0x18 */
    {'v', 'V', 0},       /* 0x19 */
    {'w', 'W', 0},       /* 0x1a */
    {'x', 'X', 0},       /* 0x1b */
    {'y', 'Y', 0},       /* 0x1c */
    {'z', 'Z', 0},       /* 0x1d */
    {'1', '!', 0},       /* 0x1e */
    {'2', '\"', 0},      /* 0x1f */
    {'3', '#', 0},       /* 0x20 */
    {'4', '$', 0},       /* 0x21 */
    {'5', '%', 0},       /* 0x22 */
    {'6', '&', 0},       /* 0x23 */
    {'7', '/', 0},       /* 0x24 */
    {'8', '(', 0},       /* 0x25 */
    {'9', ')', 0},       /* 0x26 */
    {'0', '=', 0},       /* 0x27 */
    {'\n', '\n', 0},     /* 0x28 */
    {27, 27, 27},        /* 0x29 ESC */
    {'\b', '\b', 0},     /* 0x2a */
    {'\t', '\t', 0},     /* 0x2b */
    {' ', ' ', 0},       /* 0x2c */
    {'\'', '?', '\\'},   /* 0x2d */
    {191, 161, 0},       /* 0x2e */
    {'\'', '"', 0},     /* 0x2f */
    {'+', '*', '~'},     /* 0x30 */
    {'\\', '|', 0},      /* 0x31 */
    {'}', ']', '`'},       /* 0x32 */
    {241, 209, 0},     /* 0x33 */
    {'{', '[', '^'},     /* 0x34 */
    {'`', '~', 0},       /* 0x35 */
    {',', ';', 0},       /* 0x36 */
    {'.', ':', 0},       /* 0x37 */
    {'-', '_', 0},       /* 0x38 */

    {0, 0, 0},       /* 0x39 */
    {0, 0, 0},       /* 0x3a */
    {0, 0, 0},       /* 0x3b */
    {0, 0, 0},       /* 0x3c */
    {0, 0, 0},       /* 0x3d */
    {5, 5, 5},       /* 0x3e F5 */
    {0, 0, 0},       /* 0x3f */
    {0, 0, 0},       /* 0x40 */
    {0, 0, 0},       /* 0x41 */
    {0, 0, 0},       /* 0x42 */
    {0, 0, 0},       /* 0x43 */
    {0, 0, 0},       /* 0x44 */
    {0, 0, 0},       /* 0x45 */
    {0, 0, 0},       /* 0x46 */
    {0, 0, 0},       /* 0x47 */
    {24, 24, 24},       /* 0x48 PAUSE */
    {0, 0, 0},       /* 0x49 */
    {2, 0, 0},       /* 0x4a */
    {22, 0, 0},      /* 0x4b */
    {127, 127, 127}, /* 0x4c */
    {3, 0, 0},       /* 0x4d */
    {23, 0, 0},      /* 0x4e */
    {19, 0, 0},      /* 0x4f */
    {18, 0, 0},      /* 0x50 */
    {21, 0, 0},      /* 0x51 */
    {20, 0, 0},      /* 0x52 */
    {0, 0, 0},       /* 0x53 */

    {'/', '/', 0},   /* 0x54 */
    {'*', '*', 0},   /* 0x55 */
    {'-', '-', 0},   /* 0x56 */
    {'+', '+', 0},   /* 0x57 */
    {'\n', '\n', 0}, /* 0x58 */
    {'1', 0, 0},     /* 0x59 */
    {'2', 0, 0},     /* 0x5a */
    {'3', 0, 0},     /* 0x5b */
    {'4', 0, 0},     /* 0x5c */
    {'5', '5', 0},   /* 0x5d */
    {'6', 0, 0},     /* 0x5e */
    {'7', 0, 0},     /* 0x5f */
    {'8', 0, 0},     /* 0x60 */
    {'9', 0, 0},     /* 0x61 */
    {'0', 0, 0},     /* 0x62 */
    {'.', 0, 0},     /* 0x63 */
    {0, 0, 0},       /* 0x64 */
    {0, 0, 0},       /* 0x65 */
    {0, 0, 0},       /* 0x66 */
    {'=', '=', 0},   /* 0x67 */

};

uint8_t keyboard_keycode_ascii_latin(int keycode, bool shift, bool alt)
{
    if (keycode >= sizeof(hid_keycode_to_ascii_latin) / sizeof(hid_keycode_to_ascii_latin[0]))
    {
        return 0; // Invalid keycode
    }

    int index = shift ? 1 : 0;
    if (alt)
        index += 1;

    uint8_t ascii = hid_keycode_to_ascii_latin[keycode][index];

    // go through capslock filter
    ascii = keyboard_caplock_filter(ascii);
    
    return ascii;
}