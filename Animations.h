/*
 * Aurora: https://github.com/pixelmatix/aurora
 * Copyright (c) 2014 Jason Coon
 *
 * Portions of this code are adapted from Craig Lindley's LightAppliance: https://github.com/CraigLindley/LightAppliance
 * Copyright (c) 2014 Craig A. Lindley
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

#ifndef Animations_H
#define Animations_H

class Animations : public Playlist {
public:

    char* Drawable::name = "Animations";

    void move(int step) {
        currentIndex += step;

        if (currentIndex >= imageCount) currentIndex = 0;
        else if (currentIndex < 0) currentIndex = imageCount - 1;

        openImageFile();
    }

    void moveRandom() {
        random16_add_entropy(analogRead(0));
        currentIndex = random(0, imageCount);
        
        if (currentIndex >= imageCount) currentIndex = 0;
        else if (currentIndex < 0) currentIndex = imageCount - 1;

        openImageFile();
    }

    unsigned int drawFrame() {
        if (!sdAvailable) {
            matrix.fillScreen({ 0, 0, 0 });
            matrix.setFont(font5x7);
            matrix.drawString(4, 24, { 255, 255, 255 }, "No SD");
            return 0;
        }

        if (!imageFile.available())
            return 0;

        unsigned long result = gifPlayer.drawFrame();
        if (result == ERROR_FINISHED) {
            openImageFile();
        }
        else if (result < 0) {
            return 0;
        }

        return result;
    }

    void start() {
        openImageFile();
    }

    void stop() {
        if (imageFile.available())
            imageFile.close();
    }


    void setup(const char* directoryName) {
        path = directoryName;

        imageCount = countFiles(directoryName);
        currentIndex = 0;
    }

    bool isAnimationFile(const char filename []) {
        if (filename[0] == '_')
            return false;

        if (filename[0] == '~')
            return false;

        if (filename[0] == '.')
            return false;

        String filenameString = String(filename).toUpperCase();
        if (filenameString.endsWith(".GIF") != 1)
            return false;

        return true;
    }

private:

    const char* path;

    File imageFile;

    int currentIndex = 0;
    int imageCount = 1;

    int animationDurationSeconds = 3;

    bool paused = true;

    // count the number of files
    int countFiles(const char* directoryName) {
        int count = 0;

        File directory = SD.open(directoryName);

        File file = directory.openNextFile();
        while (file) {
            if (!file.isDirectory()) {
                if (isAnimationFile(file.name())) {
                    count++;
                }
            }

            file.close();
            file = directory.openNextFile();
        }

        directory.close();

        return count;
    }

    void openImageFile() {
        if (!sdAvailable)
            return;

        if (imageFile.available())
            imageFile.close();

        char name[13];
        getNameByIndex(path, currentIndex, name, imageCount);

        char filepath[255];
        strcpy(filepath, path);
        strcat(filepath, name);

        imageFile = SD.open(filepath);
        if (!imageFile)
            return;

        if (imageFile.isDirectory()){
            imageFile.close();
            return;
        }

        gifPlayer.setFile(imageFile);

        if (!gifPlayer.parseGifHeader()) {
            imageFile.close();
            return;
        }

        gifPlayer.parseLogicalScreenDescriptor();
        gifPlayer.parseGlobalColorTable();
    }

    // Get the name of the GIF file with specified index
    void getNameByIndex(const char *directoryName, int index, char *nameBuffer, int numberOfFiles) {
        // Make sure index is in range
        if ((index < 0) || (index >= numberOfFiles))
            return;

        File directory = SD.open(directoryName);
        if(!directory)
            return;

            // Make sure file is closed before starting
        File file = directory.openNextFile();

        while (file && (index >= 0)) {
            if (file.isDirectory()) {
                file.close();
                continue;
            }

            if (isAnimationFile(file.name())) {
                index--;

                // Copy the filename name into the buffer
                strcpy(nameBuffer, file.name());
                }

                file.close();
            file = directory.openNextFile();
            }

        directory.close();
    }
};

#endif
