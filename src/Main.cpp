#include "Main.h"
#include "GUI.h"
#include "captureDetection.h"

int main()
{
    GUI::initGUI();
    
    // retrieve dark and darker window handle
    DnD = FindWindowA(NULL, "Dark and Darker  ");

    // capture image
    while (true) {
        if (DnD) {
            captureDetection::findBestMap();
        }
        else {
            std::cout << "dark and darker not found, searching again. (5s)\n";
            DnD = FindWindowA(NULL, "Dark and Darker  ");
        }
        Sleep(5000);
    }
    
    
}

