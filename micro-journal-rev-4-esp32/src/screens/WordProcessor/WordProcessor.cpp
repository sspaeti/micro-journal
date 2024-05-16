#include "WordProcessor.h"
#include "app/app.h"
#include "app/SD/sd.h"
#include "service/display/display.h"
#include "service/keyboard/keyboard.h"
#include "service/keyboard/ascii/ascii.h"

#include <FS.h>
#include <SD.h>
#include <algorithm>

// Initialize static instance
WordProcessor *WordProcessor::instance = nullptr;

// Get instance of WordProcessor
WordProcessor &WordProcessor::getInstance(TFT_eSPI *ptft, U8g2_for_TFT_eSPI *pu8f)
{
    if (!instance)
    {
        instance = new WordProcessor(ptft, pu8f);
    }
    return *instance;
}

// Setup function
void WordProcessor::setup()
{
    //
    // Check SD card status
    JsonDocument &app = app_status();

    // Check if the file exists, create if not
    if (!SD.exists(FILENAME))
    {
        File file = SD.open(FILENAME, FILE_WRITE);
        if (!file)
        {
            //
            Serial.println("Failed to create file");

            return;
        }
        file.close();
        Serial.println("File created");
    }

    // Load text from file
    loadText();

    // Clear background
    clear = true;

    // sleep timer reset
    last_sleep = millis();
}

// Render text on screen
void WordProcessor::render()
{
    clearBackground();
    clearTrails();
    checkSaved();
    checkSleep();

    // Calculate the screen text lines
    // try to display from the second line when possible
    // then when cursor reaches the end refresh the screen
    int total_line = 0;
    int row_character_count = 0;
    char last_char;

    line_position[0] = &text_buffer[0];
    line_length[0] = MAX_ROW_CHARACTERS;

    for (int i = 0; i < TEXT_BUFFER_SIZE; i++)
    {
        // when reaching end of text
        // break
        if (text_buffer[i] == '\0')
        {
            //
            line_length[total_line] = row_character_count;

            //
            line_position[total_line + 1] = nullptr;
            line_length[total_line + 1] = 0;

            // debug message
            // app_log("%d %d %d\n", total_line, line_length[total_line]);

            //
            break;
        }

        // count total characters in a line
        row_character_count++;

        // when receiving a \n or max character reached
        // start a new line
        if (text_buffer[i] == '\n' || row_character_count == MAX_ROW_CHARACTERS)
        {
            //
            total_line += 1;

            //
            line_position[total_line] = &text_buffer[i + 1];
            line_length[total_line - 1] = row_character_count;

            // if last character is new line don't count in
            if (text_buffer[i] == '\n')
            {
                line_length[total_line - 1] = row_character_count - 1;
            }

            //
            row_character_count = 0;
        }

        //
        last_char = text_buffer[i];
    }

    // determine where the current editing line will be
    int start_line = start_line_prev;

    // when reached the max line
    // refresh the screen to the 2nd line
    if (total_line - start_line_prev >= MAX_LINES)
    {
        // very start of the writing, there is nothing to show
        // so line starts from the first row
        start_line = total_line - 2;
        if (start_line < 0)
            start_line = 0;

        // refresh the screen
        clear = true;
        start_line_prev = start_line;
    }

    // with backspace, reaches the first character of the screen
    // means there will be no more character displayed on the screen
    if (total_line_prev > total_line && total_line == start_line - 1)
    {
        app_log("%d %d %d\n", total_line_prev, total_line, start_line);

        // clear screen
        clear = true;

        start_line -= MAX_LINES - 1;
        if (start_line < 0)
            start_line = 0;

        start_line_prev = start_line;
    }

    //
    if (total_line_prev != total_line)
    {
        total_line_prev = total_line;
    }

    // STATUS BAR
    ptft->setCursor(0, STATUSBAR_Y, 2);
    ptft->setTextColor(TFT_WHITE, STATUSBAR_COLOR);
    ptft->setTextSize(1);
    ptft->printf("%s bytes", formatNumberWithCommas(fileSize + text_pos - text_last_save_pos));
    /*
    ptft->printf("%s, %d, %d, %d, %d, %d",
                 formatNumberWithCommas(fileSize + text_pos - text_last_save_pos),
                 text_pos,
                 text_last_save_pos,
                 total_line,
                 start_line,
                 line_length[total_line - 1]);
    */
    if (text_pos == text_last_save_pos)
    {
        ptft->fillCircle(310, STATUSBAR_Y + 8, 5, TFT_GREEN);
    }
    else
    {
        ptft->fillCircle(310, STATUSBAR_Y + 8, 5, TFT_RED);
    }
    ptft->drawCircle(310, STATUSBAR_Y + 8, 5, TFT_BLACK);

    /////
    // Render the user text
    pu8f->setFont(u8g2_font_profont22_mf); // extended font
    pu8f->setForegroundColor(TFT_WHITE);   // apply color
    pu8f->setFontMode(1);                  // use u8g2 transparent mode (this is default)
    pu8f->setCursor(0, 24);                // start writing at this position

    for (int i = start_line; i <= total_line; i++)
    {
        // print new line
        if (i != start_line)
            pu8f->println("");

        // partial refresh
        if (total_line - i > 2 && total_line > LINE_MAX)
        {
            continue;
        }

        //
        if (line_position[i] != nullptr)
        {
            //
            if (line_position[i] == nullptr)
                break;

            //
            String line;
            int length = line_length[i];

            // render
            for (int j = 0; j < length; j++)
            {
                // convert extended ascii into a streamlined string
                uint8_t value = *(line_position[i] + j);
                if (value < 128)
                {
                    line += (char)value;
                }
                else
                {
                    line += ascii_convert_unicode(value);
                }
            }

            pu8f->print(line);
        }
    }

    /////
    blinkCursor();
}

// Empty the file
void WordProcessor::emptyFile()
{
    // Rename the existing file and create a new one
    char newName[30];
    snprintf(newName, sizeof(newName), "/%lu.txt", millis());

    if (SD.rename(FILENAME, newName))
    {
        app_log("File renamed successfully: %s.\n", newName);
    }
    else
    {
        app_log("Error renaming file. %s\n", newName);
        return;
    }

    File file = SD.open(FILENAME, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to create file");
        JsonDocument &app = app_status();
        app["screen"] = ERRORSCREEN;
        return;
    }
    file.close();
    app_log("File created\n");

    // after write operation make sure to give some delay for safe operation
    delay(250);
}

// Load text from file
void WordProcessor::loadText()
{
    app_log("Reading file: %s ... ", FILENAME);
    File file = SD.open(FILENAME);
    if (!file)
    {
        app_log("Failed to open file for reading\n");
        JsonDocument &app = app_status();
        app["screen"] = ERRORSCREEN;
        return;
    }

    // Determine file size and set buffer accordingly
    fileSize = file.size();
    if (fileSize > TEXT_BUFFER_SIZE / 2)
    {
        if (!file.seek(-TEXT_BUFFER_SIZE / 2, SeekEnd))
        {
            app_log("Failed to seek file pointer\n");
            JsonDocument &app = app_status();
            app["screen"] = ERRORSCREEN;
            file.close();
            return;
        }
    }

    // Read file content into text buffer
    int pos = 0;
    text_buffer[pos] = '\0';
    while (file.available())
    {
        text_buffer[pos++] = file.read();
        text_buffer[pos] = '\0';
    }
    text_pos = pos;
    text_pos_prev = pos;
    text_last_save_pos = pos;
    start_line_prev = 0;

    app_log("File loading completed: text_pos: %d, text_last_save: %d, fileSize: %d\n", text_pos, text_last_save_pos, fileSize);

    file.close();
    delay(100);
}

// Save text to file
void WordProcessor::saveText()
{
    if (text_last_save_pos == text_pos)
    {
        // No changes, no need to save
        app_log("Nothing is changed\n");
        return;
    }

    static unsigned int last = 0;
    if (millis() - last > 1000)
    {
        last = millis();

        app_log("Writing file: %s ...", FILENAME);
        File file = SD.open(FILENAME, FILE_WRITE);
        if (!file)
        {
            app_log("Failed to open file for writing\n");
            JsonDocument &app = app_status();
            app["screen"] = ERRORSCREEN;
            return;
        }

        // Seek to the specified offset
        size_t offset = fileSize - (TEXT_BUFFER_SIZE / 2);
        if (fileSize < (TEXT_BUFFER_SIZE / 2))
            offset = 0;
        app_log("file size: %d file offset: %d text_last_save: %d text_pos: %d\n",
                fileSize, offset, text_last_save_pos, text_pos);
        if (!file.seek(offset))
        {
            app_log("Failed to seek file pointer\n");
            file.close();
            return;
        }

        size_t length = file.print(text_buffer);
        if (length >= 0)
        {
            app_log("File written from %d at length %d\n", offset, length);
            //
            text_last_save_pos = text_pos;
        }
        else
        {
            app_log("Write failed\n");
            JsonDocument &app = app_status();
            app["screen"] = ERRORSCREEN;
        }

        //
        file.close();

        // save operation takes time before loading is available
        delay(100);

        // recalculate the file size
        // calculate the file size
        file = SD.open(FILENAME, FILE_READ);
        if (!file)
        {
            app_log("Failed to open file for writing\n");
            JsonDocument &app = app_status();
            app["screen"] = ERRORSCREEN;
            return;
        }
        fileSize = file.size();
        file.close();

        // save operation takes time before loading is available
        delay(100);

        app_log("after save file size: %d file offset: %d text_last_save: %d text_pos: %d\n",
                fileSize, offset, text_last_save_pos, text_pos);
    }
}

// Handle keyboard input
void WordProcessor::keyboard(char key)
{
    if (file_ongoing)
    {
        app_log("File operation on going. Skipping key press");

        return;
    }
    // Check if menu key is pressed
    if (key == MENU)
    {
        // Save before transitioning to the menu
        file_ongoing = true;
        saveText();
        file_ongoing = false;

        //
        JsonDocument &app = app_status();
        app["screen"] = MENUSCREEN;
        return;
    }

    ///
    // handling BACKSPACE
    if (key == '\b')
    {
        if (text_pos > 0)
        {
            text_buffer[--text_pos] = '\0';
        }
        else
        {
            // Load previous contents from the file if at the beginning of the buffer
            app_log("Backspace reached the beginning of the buffer\n");
            clear = true;
            text_pos = 0;
            text_buffer[0] = '\0';

            file_ongoing = true;
            saveText();
            loadText();
            file_ongoing = false;
        }
    }
    ///

    else
    {
        text_buffer[text_pos++] = key;
        text_buffer[text_pos] = '\0';
    }

    //
    if (text_pos >= TEXT_BUFFER_SIZE)
    {
        app_log("Text buffer full\n");

        file_ongoing = true;
        saveText();
        loadText();
        file_ongoing = false;

        // when the buffer is about to finish try to save
        clear = true;
    }
}

// Clear background
void WordProcessor::clearBackground()
{
    if (clear)
    {
        clear = false;
        ptft->fillScreen(TFT_BLACK);
        ptft->fillRect(0, STATUSBAR_Y, 320, 240, STATUSBAR_COLOR);

        // write keyboard layout
        JsonDocument &app = app_status();
        String layout = app["config"]["keyboard_layout"].as<String>();
        if (layout == "null" || layout.isEmpty())
            layout = "US"; // defaults to US layout

        ptft->setCursor(280, STATUSBAR_Y, 2);
        ptft->setTextColor(TFT_WHITE, STATUSBAR_COLOR);
        ptft->setTextSize(1);
        ptft->print(layout);
    }
}

// Clear trails
void WordProcessor::clearTrails()
{
    if (text_pos_prev != text_pos)
    {
        // get current cursor position
        int cursorX = pu8f->getCursorX();
        int cursorY = pu8f->getCursorY();

        // remove previous cursor
        for (int i = 0; i <= text_pos - text_pos_prev; i++)
        {
            ptft->drawLine(cursorX + 2 - (10 * (i + 1)), cursorY + 2, cursorX + 24 - (10 * (i + 1)), cursorY + 2, TFT_BLACK);
        }

        // in case when back space is pressed
        if (text_pos_prev > text_pos)
        {
            // delete the character
            ptft->fillRect(cursorX - 12, cursorY - 16, 320, 40, TFT_BLACK);
        }

        // always show the cursor when typing
        blink = true;

        // update the text_pos
        text_pos_prev = text_pos;
    }
}

// Blink Cursor
void WordProcessor::blinkCursor()
{
    static unsigned int last = millis();
    if (millis() - last > 500)
    {
        last = millis();
        blink = !blink;
    }

    int cursorX = pu8f->getCursorX();
    int cursorY = pu8f->getCursorY();

    //
    if (blink)
    {
        ptft->drawLine(cursorX + 2, cursorY + 2, cursorX + 12, cursorY + 2, TFT_WHITE);
    }
    else
    {
        ptft->drawLine(cursorX + 2, cursorY + 2, cursorX + 12, cursorY + 2, TFT_BLACK);
    }
}

// Check if text is saved
void WordProcessor::checkSaved()
{
    static unsigned int last = millis();
    static unsigned int text_pos_last = -1;
    if (text_pos_last != text_pos)
    {
        text_pos_last = text_pos;
        last = millis();
    }

    if (millis() - last > 3000)
    {
        last = millis();
        if (text_pos != text_last_save_pos)
        {
            file_ongoing = true;
            saveText();
            file_ongoing = false;
            // clear = true;
        }
    }
}

//
void WordProcessor::checkSleep()
{
    static unsigned int text_pos_last = -1;
    if (text_pos_last != text_pos)
    {
        // when typed then reset sleep timer
        text_pos_last = text_pos;
        last_sleep = millis();
    }

    if (millis() - last_sleep > 60000)
    {
        // if no action for 1 minute go to sleep
        last_sleep = -1;

        //
        JsonDocument &app = app_status();
        app["screen"] = SLEEPSCREEN;
    }
}

// Format number with commas
String WordProcessor::formatNumberWithCommas(long num)
{
    String formattedNumber = "";
    int digitCount = 0;

    if (num < 0)
    {
        formattedNumber += "-";
        num = -num;
    }

    do
    {
        if (digitCount > 0 && digitCount % 3 == 0)
        {
            formattedNumber = "," + formattedNumber;
        }
        formattedNumber = String(num % 10) + formattedNumber;
        num /= 10;
        digitCount++;
    } while (num > 0);

    return formattedNumber;
}
