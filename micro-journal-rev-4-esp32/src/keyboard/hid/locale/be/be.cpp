#include "be.h"
#include "../locale.h"
#include "keyboard/keyboard.h"

uint8_t hid_keycode_to_ascii_be[][4] = {
    {0, 0, 0, 0},          /* 0x00 */
    {0, 0, 0, 0},          /* 0x01 */
    {0, 0, 0, 0},          /* 0x02 */
    {0, 0, 0, 0},          /* 0x03 */
    {'q', 'Q', 0, 0},      /* 0x04 */
    {'b', 'B', 0, 0},      /* 0x05 */
    {'c', 'C', 169, 162},  /* 0x06 */
    {'d', 'D', 0, 0},      /* 0x07 */
    {'e', 'E', 234, 202},  /* 0x08 */
    {'f', 'F', 0, 183},      /* 0x09 */
    {'g', 'G', 0, 0},      /* 0x0a */
    {'h', 'H', 204, 206},  /* 0x0b */
    {'i', 'I', 238, 239},  /* 0x0c */
    {'j', 'J', 207, 205},  /* 0x0d */
    {'k', 'K', 200, 203},  /* 0x0e */
    {'l', 'L', 172, '|'},  /* 0x0f */
    {',', '?', 0, 191},    /* 0x10 */
    {'n', 'N', '~', '|'},  /* 0x11 */
    {'o', 'O', 156, 140},  /* 0x12 */
    {'p', 'P', 0, 0},      /* 0x13 */
    {'a', 'A', 230, 198},  /* 0x14 */
    {'r', 'R', 174, ','},  /* 0x15 */
    {'s', 'S', 210, 0},    /* 0x16 */
    {'t', 'T', 134, 153},  /* 0x17 */
    {'u', 'U', 176, 170},  /* 0x18 */
    {'v', 'V', 0, 0},      /* 0x19 */
    {'z', 'Z', 194, 197},  /* 0x1a */
    {'x', 'X', 0, '/'},    /* 0x1b */
    {'y', 'Y', 218, 159},  /* 0x1c */
    {'w', 'W', 139, 155},  /* 0x1d */
    {'&', '1', 0, '\''},   /* 0x1e */
    {233, '2', 235, '"'},  /* 0x1f */
    {'"', '3', 147, 148},  /* 0x20 */
    {'\'', '4', 145, 146}, /* 0x21 */
    {'(', '5', '{', '['},  /* 0x22 */
    {167, '6', 182, 229},  /* 0x23 */
    {232, '7', 171, 187},  /* 0x24 */
    {'!', '8', 'i', 251},  /* 0x25 */
    {231, '9', 231, 225},  /* 0x26 */
    {224, '0', 248, 216},  /* 0x27 */
    {'\n', '\n', 0, 0},    /* 0x28 */
    {27, 27, 27, 27},        /* 0x29 ESC */
    {'\b', '\b', 0, 0},    /* 0x2a */
    {'\t', '\t', 0, 0},    /* 0x2b */
    {' ', ' ', 160, 160},      /* 0x2c */
    {')', 176, '}', ']'},  /* 0x2d */
    {'-', '_', 151, 150},  /* 0x2e */
    {'^', 168, 244, 212},  /* 0x2f */
    {'$', '*', 128, 165},  /* 0x30 */
    {'`', 163, '@', '#'},  /* 0x31 */
    {181, 167, 0, 0},      /* 0x32 */
    {'m', 'M', 181, 211},  /* 0x33 */
    {249, '%', 217, 137},  /* 0x34 */
    {'<', '>', 0, 0},      /* 0x35 */
    {';', '.', 133, 149},  /* 0x36 */
    {':', '/', '%', '\\'}, /* 0x37 */
    {'=', '+', 0, 0},      /* 0x38 */

    {0, 0, 0, 0},       /* 0x39 */
    {0, 0, 0, 0},       /* 0x3a */
    {0, 0, 0, 0},       /* 0x3b */
    {0, 0, 0, 0},       /* 0x3c */
    {0, 0, 0, 0},       /* 0x3d */
    {5, 5, 5, 5},       /* 0x3e F5 */
    {0, 0, 0, 0},       /* 0x3f */
    {0, 0, 0, 0},       /* 0x40 */
    {0, 0, 0, 0},       /* 0x41 */
    {0, 0, 0, 0},       /* 0x42 */
    {0, 0, 0, 0},       /* 0x43 */
    {0, 0, 0, 0},       /* 0x44 */
    {0, 0, 0, 0},       /* 0x45 */
    {0, 0, 0, 0},       /* 0x46 */
    {0, 0, 0, 0},       /* 0x47 */
    {24, 24, 24, 24},       /* 0x48 PAUSE */
    {0, 0, 0, 0},       /* 0x49 */
    {2, 0, 0, 0},       /* 0x4a */
    {22, 0, 0, 0},      /* 0x4b */
    {127, 127, 127, 0}, /* 0x4c */
    {3, 0, 0, 0},       /* 0x4d */
    {23, 0, 0, 0},      /* 0x4e */
    {19, 0, 0, 0},      /* 0x4f */
    {18, 0, 0, 0},      /* 0x50 */
    {21, 0, 0, 0},      /* 0x51 */
    {20, 0, 0, 0},      /* 0x52 */
    {0, 0, 0, 0},       /* 0x53 */

    {'/', '/', 0, 0},     /* 0x54 */
    {'*', '*', 0, 0},     /* 0x55 */
    {'-', '-', 0, 0},     /* 0x56 */
    {'+', '+', 0, 0},     /* 0x57 */
    {'\n', '\n', 0, 0},   /* 0x58 */
    {'1', '1', 0, '\''},  /* 0x59 */
    {'2', '2', 235, '"'}, /* 0x5a */
    {'3', '3', 147, 148}, /* 0x5b */
    {'4', '4', 145, 146}, /* 0x5c */
    {'5', '5', '{', '['}, /* 0x5d */
    {'6', '6', 182, 229}, /* 0x5e */
    {'7', '7', 171, 187}, /* 0x5f */
    {'8', '8', 'i', 251}, /* 0x60 */
    {'9', '9', 231, 225}, /* 0x61 */
    {'0', '0', 248, 216}, /* 0x62 */
    {'.', ',', 133, 0},   /* 0x63 */
    {'@', '#', 149, 255}, /* 0x64 */
    {0, 0, 0, 0},         /* 0x65 */
    {0, 0, 0, 0},         /* 0x66 */
    {'=', '=', 0, 0},     /* 0x67 */
};

uint8_t keyboard_keycode_ascii_be(int keycode, bool shift, bool alt)
{
    if (keycode >= sizeof(hid_keycode_to_ascii_be) / sizeof(hid_keycode_to_ascii_be[0]))
    {
        return 0; // Invalid keycode
    }

    // Shift key increases the index by 1
    int index = shift ? 1 : 0;

    // if alt key is on adds 2
    if (alt)
        index += 2;

    // detect the alt key
    uint8_t ascii = hid_keycode_to_ascii_be[keycode][index];

    // if ' " then do not make precursor
    if (ascii == '\'' || ascii == '"')
        return ascii;

    // go through precursor
    ascii = keyboard_precursor_filter(ascii);

    // go through capslock filter
    ascii = keyboard_caplock_filter(ascii);

    return ascii;
} 