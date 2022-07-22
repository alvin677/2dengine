// Made by Cedric#0591
// My work in progress 2D engine (with some explanations)

// Includes the important libraries, such as Windows.h for the Sleep function and SetPixel function.
#include <Windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <thread>

// Arrays of object properties.
int objectPosX[128];
int objectPosY[128];
int objectWidth[128];
int objectHeight[128];
int objectR[128];
int objectG[128];
int objectB[128];
bool objectInvisible[128];

// The important stuff for console graphics.
HWND console = GetConsoleWindow();
HDC mdc = GetDC(console);
HANDLE console2 = GetStdHandle(STD_OUTPUT_HANDLE);
COORD screen;

//* A function to create a new object (sets standard properties values)
void newObject(int id) {
    objectPosX[id] = 1;
    objectPosY[id] = 1;
    objectWidth[id] = 1;
    objectHeight[id] = 1;
    objectR[id] = 1;
    objectG[id] = 1;
    objectB[id] = 1;
    objectInvisible[id] = false;
}

// One of the most useful functions. Allows you to edit the objects.
void editObject(int id, int posX, int posY, int width = 1, int height = 1, bool invisible = false, int r = 0, int g = 0, int b = 0, int rr = 0, int rg = 0, int rb = 0) {
    // The part below replaces the background from where the object moved.
    COLORREF COLOR = RGB(rr, rg, rb);
    for (int y = objectPosY[id]; y < objectHeight[id] + objectPosY[id]; y++) {
        for (int x = objectPosX[id]; x < objectWidth[id] + objectPosX[id]; x++) {
            SetPixelV(mdc, x, y, COLOR);
        }
    }

    // Applies the changes to the object.
    objectPosX[id] = posX;
    objectPosY[id] = posY;
    objectWidth[id] = width;
    objectHeight[id] = height;
    objectR[id] = r;
    objectG[id] = g;
    objectB[id] = b;
    objectInvisible[id] = invisible;

}

// Deletes an object by changing all the values to NULL (nothing).
void delObject(int id) {
    objectPosX[id] = NULL;
    objectPosY[id] = NULL;
    objectWidth[id] = NULL;
    objectHeight[id] = NULL;
    objectR[id] = NULL;
    objectG[id] = NULL;
    objectB[id] = NULL;
    objectInvisible[id] = NULL;
}

// Returns a list of all created objects.
std::string objects() {
    std::string objectList;
    for (int i = 1; i < 128 + 1; i++) {
        if (objectPosX[i]) {
            objectList = objectList + std::to_string(i) + " ";
        }
    }
    return objectList;
}

// Draws all the objects to the console window.
void update() { 
    for (int i = 1; i < 128 + 1; i++) {
        if (objectPosX[i]) {
            if (objectInvisible[i] == false) {
                COLORREF COLOR = RGB(objectR[i], objectG[i], objectB[i]);
                for (int y = objectPosY[i]; y < objectHeight[i] + objectPosY[i]; y++) {
                    for (int x = objectPosX[i]; x < objectWidth[i] + objectPosX[i]; x++) {
                        SetPixelV(mdc, x, y, COLOR);
                    }
                }
            }
        }
    }
}

// Draws a specific object to the console window. Can speed up the game you've made if you only redraw what's necessary.
void draw(int id) {
    if (objectInvisible[id] == false) {
        COLORREF COLOR = RGB(objectR[id], objectG[id], objectB[id]);
        for (int y = objectPosY[id]; y < objectHeight[id] + objectPosY[id]; y++) {
            for (int x = objectPosX[id]; x < objectWidth[id] + objectPosX[id]; x++) {
                SetPixelV(mdc, x, y, COLOR);
            }
        }
    }
}

// Changes console window size (resolution)
void resolution(int resolutionX = 210, int resolutionY = 55) {
    std::string res = "mode " + std::to_string(resolutionX) + ", " + std::to_string(resolutionY);
    system(res.c_str());
}

// Fills the screen with a color. (2 methods available)
void fill(int method = 1, int r = 0, int g = 0, int b = 0, int sizeX = 9, int sizeY = 20) {
    COLORREF COLOR = RGB(r, g, b);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int cX, cY;
    GetConsoleScreenBufferInfo(console2, &csbi);
    cX = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    cY = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if (method == 1) {
        SetConsoleTextAttribute(console2, r * 17);
        for (int y = 1; y < cY + 1; y++) {
            for (int x = 1; x < cX + 1; x++) {
                std::cout << " ";
            }
        }
    }

    else if (method == 2) {
        for (int y = 0; y < cY*sizeY; y++) {
            for (int x = 0; x < cX*sizeX; x++) {
                SetPixelV(mdc, x, y, COLOR);
            }
        }
    }
}

// Print out a text at a specific position on the console with color and time between each character.
void text(std::string text, int posX, int posY, int color = 15, int time = 0) {
    screen.X = posX;
    screen.Y = posY;
    SetConsoleTextAttribute(console2, color);
    SetConsoleCursorPosition(console2, screen);
    for (int i = 0; i < text.length(); i++) {
        std::cout << text[i];
        Sleep(time);
    }

}

// Collision check between 2 objects.
bool collision(int id, int id2) {
    int objectPosXb = objectPosX[id] + objectWidth[id];
    int objectPosYb = objectPosY[id] + objectHeight[id];
    if ( objectPosX[id] < objectPosX[id2] + objectWidth[id2] &&
        objectPosX[id] + objectWidth[id] > objectPosX[id2] &&
        objectPosY[id] < objectPosY[id2] + objectHeight[id2] &&
        objectPosY[id] + objectHeight[id] > objectPosY[id2]
        ) {
        return true;
    }

    else {
        return false;
    }
    
}

// End of graphics functions
















bool pause = false; // A pause function with the bool datatype.

// A collision check function between the player and the red rectangles running asynchronous using the thread library.
void check() {
    for (int i = 3; i < 48; i++) {
        if (collision(1, i)) {
            pause = true;
        }
    }
    Sleep(100);
    check();
}


// Inside the main() function is just a test game that I wrote
int main()
{
    
    
    resolution(50, 20); // Changes window size
    system("title Cool Game"); // Sets window title
    Sleep(100); // Waits for 1/10 second
    fill(2, 0, 100, 100); // Fills the screen with the following rgb values: 0, 100, 100
    newObject(1); // Player
    srand(time(0));

    // This part creates all the red rectangles.
    for (int i = 2; i < 48; i++) {
        int randomX = rand() % 420;
        int randomY = rand() % 350 + 30;
        int randomSizeX = rand() % 55 + 5;
        int randomSizeY = rand() % 55 + 5;

        newObject(i); // Create obstacles
        editObject(i, randomX, randomY, randomSizeX, randomSizeY, false, 255); // Edit obstacle
    }

    newObject(2); // Finish line
    int linePos = rand() % 395; // Random x position.
    int lineLen = rand() % 100 + 50; // Random x size.
    editObject(2, linePos, 395, lineLen, 5, false, 0, 255); // Moves the finish line and changes its color.
    Sleep(100);
    update(); // Draws all objects on the console

    pause = false;
    int playerX = 200;
    float playerY = 1;
    int size = 20;

    editObject(1, playerX, playerY, size, size, false, 255, 255, 255, 0, 100, 100); // Edit player

    Sleep(1000); // Waits a second

    std::thread colcheck(check); // Starts the thread to check collision between the player and the red rectangles.

    // Key press movement and collision check between finish line.
    while (true) {
        if (GetAsyncKeyState('D') & 0x8000) {
            playerX += 3;
            Sleep(1);
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            playerX -= 3;
            Sleep(1);
        }

        // If the player touches the finish line.
        if (collision(1, 2) == true) {
            text("You won!", 0, 0, 2, 100);
            Sleep(10);
        }

        // While the game is not paused
        if (pause == false) {
            playerY += 0.5;
            editObject(1, playerX, playerY, size, size, false, 255, 255, 255, 0, 100, 100);
            draw(1);
            Sleep(10);
        }

        // When the game pauses.
        else {
            text("Game over!", 0, 0, 4, 10);
            Sleep(3000);
            colcheck.detach();
            main();
        }
    }
}