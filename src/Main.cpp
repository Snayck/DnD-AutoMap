#include "Main.h"
#include "captureDetection.h"
#include <opencv2/opencv.hpp>
using namespace cv;

std::string Main::bestMapMatchPath = "";
HWND Main::DnD = nullptr;

HANDLE InterfaceThread = NULL;

DWORD WINAPI MainThread()
{
    while (true) {
        if (Main::bestMapMatchPath != "") {
            std::cout << Main::bestMapMatchPath << "\n";
            Mat baseImage = imread(Main::bestMapMatchPath);
            if (!baseImage.empty()) {
                cv::namedWindow("Interactive Map", WINDOW_NORMAL);
                cv::resizeWindow("Interactive Map", 750, 750);
                imshow("Interactive Map", baseImage);
                waitKey(0);
            }
        }
        Sleep(1000);
    }
}


int main()
{
    InterfaceThread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(MainThread), nullptr, 0, nullptr);

    // retrieve dark and darker window handle
    Main::DnD = FindWindowA(NULL, "Dark and Darker  ");

    // capture image
    while (true) {
        if (Main::DnD) {
            Main::bestMapMatchPath = captureDetection::takeScreenshotAndFindMap();
        }
        else {
            std::cout << "dark and darker not found, searching again. (5s)\n";
            Main::DnD = FindWindowA(NULL, "Dark and Darker  ");
        }
        Sleep(5000);
    }
}

