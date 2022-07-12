#include <iostream>
#include <thread>
#include <Windows.h>

// Left arrow key
constexpr char PAUSE_KEY = 0x25;
constexpr int SLEEP_TIME = 1000;

constexpr int VK_Q = 0x51;
constexpr int VK_W = 0x57;
constexpr int VK_E = 0x45;
constexpr int VK_R = 0x52;
constexpr int VK_A = 0x41;
constexpr int VK_S = 0x53;
constexpr int VK_D = 0x44;
constexpr int VK_F = 0x46;
const char* PAUSE_KEY_NAME = "Left Arrow";
void Update(bool*);
int main()
{
    std::cout << "Program started as paused, press \"Left arrow key\" to start, pause or resume the program\n";
    bool isRunning = false;
    std::thread InputReaderThread([](bool* state) {
        while (1) {
            if (GetAsyncKeyState(PAUSE_KEY)) {
                *state = !*state;
                if (!*state) {
                    std::cout << "Bot paused.\n";
                    std::cout << "Press "<< PAUSE_KEY_NAME<<" to continue." << std::endl;
                }
                else {
                    system("cls");
                    std::cout << "Press " << PAUSE_KEY_NAME << " to pause." << std::endl;
                }
            }
            Sleep(100);
        }}, &isRunning);
    static int count = 0;
    std::thread BotThread([](bool* state) {
        static int internal_count;
        while (1) {
            if (*state)
            {
                Update(state);
                if ((++internal_count) > 5) {
                    internal_count = 0;
                    system("cls");
                    printf("Bot still running (%i);\n", ++count);
                }
            }
            Sleep(SLEEP_TIME);                        
            
            
        }}, &isRunning);
    InputReaderThread.join();
    BotThread.join();
}


void KeyPress(int code) {
    INPUT inputs[2] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = code;
    
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = code;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(2, inputs, sizeof(INPUT));
}

struct Color {
    int Red, Green, Blue;
    Color() :Red(0), Green(0), Blue(0) {}
    Color(COLORREF color) :Red(GetRValue(color)), Green(GetGValue(color)), Blue(GetBValue(color)) {}
};



void Update(bool* state) {
    const POINT StartPoint = { 742, 727 };
    const POINT EndPoint = { 1162 , 727 };
    const float STEP = 2.8f;
    float x = StartPoint.x;
    float y = StartPoint.y;

    std::thread crescent([&]() {
        for (x = StartPoint.x; x < EndPoint.x; x += STEP) {            
            Color upper_color = GetPixel(GetDC(0), x, y);
            Color down_color = GetPixel(GetDC(0), x, y + 20);
            if (upper_color.Blue > 150 && (down_color.Red > 100 && down_color.Green > 100)) {
                KeyPress(0x20);
            }
        }
    });
    std::thread decrescent([&]() {
        for (x = EndPoint.x; x > StartPoint.x; x += (STEP*-1)) {            
            Color upper_color = GetPixel(GetDC(0), x, y);
            Color down_color = GetPixel(GetDC(0), x, y + 20);
            if (upper_color.Blue > 150 && (down_color.Red > 100 && down_color.Green > 100)) {
                KeyPress(0x20);
            }
        }
        });
    crescent.join();
    decrescent.join();
    
  
}
