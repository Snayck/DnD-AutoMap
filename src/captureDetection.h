#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <opencv2/opencv.hpp>

class captureDetection
{
public:
	static std::string currentMapPath;

public:
	static std::string takeScreenshotAndFindMap();
};

