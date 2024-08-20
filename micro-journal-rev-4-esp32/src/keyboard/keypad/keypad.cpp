#include "keypad.h"
#include "app/app.h"
#include "display/display.h"
#include "keyboard/keyboard.h"
#include "keyboard/ascii/ascii.h"
#include "editor/editor.h"
//
#include "display/WordProcessor/WordProcessor.h"
#include "display/ErrorScreen/ErrorScreen.h"
#include "display/Menu/Menu.h"

//
#include <SD.h>

//
#define LAYERS 4 // layers

// GPIO PINS
#define ROWS 4 // rows
byte rowPins[ROWS] = {15, 16, 17, 18};

#define COLS 12 // columns
byte colPins[COLS] = {1, 2, 42, 41, 40, 39, 45, 48, 47, 21, 20, 19};

//
// KEYBOARD LAYOUT
//

// https://www.ascii-code.com/
// ESC - 27
// BACKSPACE - 8
// SHIFT - 14
// ALT - 17

// arrow keys
// 18 - Left, 19 - Right, 20 - Up, 21 - Down
// 2 - Home 3 - End
// 22 - Page Up 
// 23 - Page Down

// layers
// prettier-ignore
int layers[LAYERS][ROWS * COLS] = {

    {// normal layers
     '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', ',',
     27, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 8,
     17, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\'', '\n',
     14, 'z', 'x', 'c', 'v', ' ', 'b', 'n', 'm', '.', '/', 14},

    {// when shift is pressed
     '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '<',
     27, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 127,
     17, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '\"', '\n',
     14, 'Z', 'X', 'C', 'V', ' ', 'B', 'N', 'M', '>', '?', 14},

    {// alt layer
     '`', '1', '2', '3', '4', '5', '6', '7', '8', '[', ']', '\\',
     27, 'q', 'w', 'e', 'r', 't', 22 , 'u', 20 , '-', '=', 127, // 2, 20, 3, 22,
     17, 'a', 's', 'd', 'f', 'g', 23 , 18 , 21 , 19 , ';', '\n', //  18, 21, 19, 23
     14, 'z', 'x', 'c', 'v', ' ', 'b', 2  , 'm', 3  , '/', 14},

    {// alt layer shift
     '~', '!', '@', '#', '$', '%', '^', '&', '*', '{', '}', '|',
     27, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', '_', '+', 127,
     17, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\n',
     14, 'Z', 'X', 'C', 'V', ' ', 'B', 'N', 'M', '<', '?', 14},

};

// define the symbols on the buttons of the keypads
// prettier-ignore
char keys[ROWS][COLS] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
    {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
    {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
    {36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
};

Adafruit_Keypad customKeypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//
int layer = 0;
bool _shift_pressed = false;
bool _fn_pressed = false;

// back space
int _backspace_last = 0;
bool _backspace_pressed = false;

// initialize keymap
void keyboard_keypad_setup()
{
    keyboard_keypad_load_config();
    customKeypad.begin();
    app_log("Keypad initialized\n");
}

///
void keyboard_keypad_loop()
{
    static unsigned int last = 0;
    if (millis() - last > 10)
    {
        //
        last = millis();

        // put your main code here, to run repeatedly:
        customKeypad.tick();

        while (customKeypad.available())
        {
            keypadEvent e = customKeypad.read();
            int ascii = keyboard_get_key(e);
            if (ascii != 0)
            {
                // depending on the screen
                // send the keystrokes
                JsonDocument &app = app_status();
                int screen = app["screen"].as<int>();

                if (ascii == 27)
                {
                    keyboard_key(MENU);
                }

                else if (ascii != 0)
                {
                    keyboard_key(ascii);
                }
            }
        }
    }

    // Handle Backspace
    if (millis() > 60 + _backspace_last && _backspace_pressed)
    {
        _backspace_last = millis();

        // send backspace key
        keyboard_key('\b');
    }
}

int keyboard_get_key(keypadEvent e)
{
    // release back space when any other keys are pressed
    if (_backspace_pressed)
    {
        _backspace_pressed = false;
    }

    //
    // step 1. layer or speical key processing
    //
    int key = layers[layer][e.bit.KEY];
    if (key == 17)
    {
        if (e.bit.EVENT == KEY_JUST_PRESSED)
        {
            _fn_pressed = true;
            return 0;
        }
        else
        {
            _fn_pressed = false;
            return 0;
        }
    }
    else if (key == 14)
    {
        if (e.bit.EVENT == KEY_JUST_PRESSED)
        {
            _shift_pressed = true;
            return 0;
        }

        else if (e.bit.EVENT == KEY_JUST_RELEASED)
        {
            _shift_pressed = false;
            return 0;
        }
    }
    // mark back space press
    else if (key == '\b')
    {
        if (e.bit.EVENT == KEY_JUST_PRESSED)
        {
            _backspace_last = millis() + 500;
            _backspace_pressed = true;
        }

        else if (e.bit.EVENT == KEY_JUST_RELEASED)
        {
            // register the timer
            _backspace_last = millis();
            _backspace_pressed = false;
        }
    }

    // step 2. process the key
    if (e.bit.EVENT == KEY_JUST_PRESSED)
    {
        // define the layer
        layer = 0;

        // check if the layer key is pressed
        if (_fn_pressed)
            layer = 2;
        // check if the shift key is pressed
        if (_shift_pressed)
            layer += 1;

        // return the corresponding key
        return layers[layer][e.bit.KEY];
    }

    // Serial.printf("KEY: %d EVENT: %d\n", e.bit.KEY, e.bit.EVENT);
    return 0;
}

// look for keyboard.json in SD card
// if found load the configuration to the global variable
#define KEYBOARD_FILE "/keyboard.json"

bool load_keymap(int layer, JsonArray keyArray)
{
    if (keyArray.size() == 48)
    {
        // only when 48 elements are presented
        int pos = 0;
        for (JsonVariant obj : keyArray)
        {
            // check if it has extended ascii
            if (obj.is<int>())
            {
                layers[layer][pos++] = obj.as<int>();
            }
            else
            {
                //
                String key = obj.as<String>();
                layers[layer][pos++] = unicode_convert_ascii(key);
            }
        }
        return true;
    }

    return false;
}

void keyboard_keypad_load_config()
{
    //
    JsonDocument &app = app_status();

    // check if file exists in SD card
    if (SD.exists(KEYBOARD_FILE))
    {
        app_log("Loading keyboard.json from SD\n");
        // load image
        File file = SD.open(KEYBOARD_FILE, "r");
        if (!file)
        {
            app_log("Failed to open file for reading\n");
            return;
        }

        String fileString = file.readString();
        file.close();

        //
        // Prepare a JsonDocument for the keyboard configuration
        // The size should be adjusted according to your configuration's needs
        JsonDocument keyboardConfig;
        // convert to JsonObject
        DeserializationError error = deserializeJson(keyboardConfig, fileString);
        if (error)
        {
            //
            app["error"] = format("keyboard.json not in a correct json form: %s\n", error.c_str());
            app_log(app["error"].as<const char *>());
            app["screen"] = ERRORSCREEN;

            return;
        }

        // overwrite to layers
        // int layers[LAYERS][ROWS * COLS]
        const char *keys[] = {"main", "main-shift", "alt", "alt-shift"};
        int keymapIndices[] = {0, 1, 2, 3};

        for (int i = 0; i < 4; i++)
        {
            const char *key = keys[i];
            int index = keymapIndices[i];

            if (keyboardConfig.containsKey(key) && keyboardConfig[key].is<JsonArray>())
            {
                if (load_keymap(index, keyboardConfig[key].as<JsonArray>()))
                {
                    app_log("%s loaded\n", key);
                }
                else
                {
                    //
                    app["error"] = format("%s keyboard layout load failed\n", key);
                    app_log(app["error"].as<const char *>());
                    app["screen"] = ERRORSCREEN;

                    return;
                }
            }
        }

        // Close the file
        file.close();

        return;
    }
}
