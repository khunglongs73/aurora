/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef SettingsChooseClock_H
#define SettingsChooseClock_H

class SettingsChooseClock : public Runnable {
private:

public:
    void run() {
        while (true) {
            drawFrame();
            clockDisplay.drawFrame();

            matrix.swapBuffers();

            InputCommand command = readCommand(defaultHoldDelay);

            switch (command) {
                case InputCommand::Up:
                    clockDisplay.move(-1);
                    clockDisplay.saveSettings();
                    break;

                case InputCommand::Down:
                    clockDisplay.move(1);
                    clockDisplay.saveSettings();
                    break;

                case InputCommand::Select:
                case InputCommand::Back:
                    return;
            }
        }
    }

    unsigned int drawFrame() {
        matrix.fillScreen(CRGB(CRGB::Black));
        return 0;
    }
};

#endif
