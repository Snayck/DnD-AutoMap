#include "captureDetection.h"
#include "Main.h"
#include <shlwapi.h>
#include <direct.h>
#include <filesystem>
namespace fs = std::filesystem;

using namespace cv;

BITMAPINFOHEADER createBitmapHeader(int width, int height)
{
    BITMAPINFOHEADER  bi;

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}

Mat captureScreenMat(HWND hwnd)
{
    Mat src;

    // get handles to a device context (DC)
    HDC hwindowDC = GetDC(GetDesktopWindow());
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT rect;
    GetWindowRect(hwnd, &rect);

    // define scale, height and width
    int screenx = rect.left;
    int screeny = rect.top;
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // create mat object
    src.create(height, width, CV_8UC4);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    BITMAPINFOHEADER bi = createBitmapHeader(width, height);

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);            //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(GetDesktopWindow(), hwindowDC);

    cvtColor(src, src, COLOR_RGBA2GRAY);

    return src;
}

std::vector<std::string> returnMaps() {
    std::vector<std::string> imagePaths;
    const std::string folderName = "maps"; 

    // Get the current working directory
    fs::path currentDir = fs::current_path();

    // Construct the path to the "map" folder
    fs::path mapFolderPath = currentDir / folderName;

    // Check if the folder exists
    if (fs::exists(mapFolderPath) && fs::is_directory(mapFolderPath)) {
        // Iterate over files in the folder
        for (const auto& entry : fs::directory_iterator(mapFolderPath)) {
            if (entry.is_regular_file()) {
                // Check if the file has an image extension (e.g., .jpg, .png, .bmp)
                const std::string extension = entry.path().extension().string();
                if (extension == ".jpg" || extension == ".png" || extension == ".bmp") {
                    imagePaths.push_back(entry.path().string());
                }
            }
        }
    }
    else {
        std::cerr << "Error: Folder '" << folderName << "' does not exist or is not a directory. Now creating a folder -> ./maps" << std::endl;
        fs::create_directory(mapFolderPath);
    }
    if (imagePaths.size() == 0) {
        std::cerr << "Could not locate any maps within the 'maps' folder." << std::endl;
    }

    return imagePaths;
}

bool compareDistances(const DMatch& a, const DMatch& b) {
    return a.distance < b.distance; // Compare string lengths in descending order
}

Rect findCropRanges(Mat inputMat) {
    int height = inputMat.rows;
    int width = inputMat.cols;

    // 0.87 x modifier
    // 0.77 y modifier
    int x = (int)(width * 0.87);  // Top-left x-coordinate
    int y = (int)(height * 0.77);   // Top-left y-coordinate
    int w = (width - x);  // Width of the cropped area
    int h = (height - y);  // Height of the cropped area

    return Rect(x, y, w, h);
}

Mat screenShotAndCrop() {
    Mat rawSC = captureScreenMat(Main::DnD);
    return rawSC(findCropRanges(rawSC));
}

std::string captureDetection::takeScreenshotAndFindMap()
{
    // Initialize ORB detector
    Ptr<ORB> orb = ORB::create();

    // Paths to map images (replace with actual paths)
    std::vector<std::string> map_paths = returnMaps();

    // Read the cropped screenshot image (replace with actual path)
    Mat cropped_screenshot = screenShotAndCrop();

    std::string best_map_path = "";
    int best_score = 0;
    std::vector<DMatch> best_matches;

    for (const std::string& map_path : map_paths) {
        // Load the template image
        Mat map = imread(map_path, IMREAD_GRAYSCALE);
        if (map.empty()) {
           std::cerr << "Error loading map image: " << map_path << std::endl;
            continue;
        }

        // Find keypoints and descriptors using ORB
        std::vector<KeyPoint> kp1, kp2;
        Mat des1, des2;
        orb->detectAndCompute(map, noArray(), kp1, des1);
        orb->detectAndCompute(cropped_screenshot, noArray(), kp2, des2);

        if (!des1.empty() && !des2.empty()) {
            // Create BFMatcher object
            BFMatcher bf(NORM_HAMMING, true);
            
            // Match descriptors
            std::vector<DMatch> matches;
            bf.match(des1, des2, matches);

            std::sort(matches.begin(), matches.end(), compareDistances);

            // Only keep really good matches
            std::vector<DMatch> good_matches;
            for (const DMatch& match : matches) {
                if (match.distance < 30) {
                    good_matches.push_back(match);
                }
            }

            if (good_matches.size() > best_score) {
                best_score = static_cast<int>(good_matches.size());
                best_map_path = map_path;
                best_matches = good_matches;
            }
        }
        else {
        }
    }

    if (!best_map_path.empty()) {

    }
    else {

    }

    return best_map_path;
}

