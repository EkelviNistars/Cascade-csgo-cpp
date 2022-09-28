#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <Windows.h>
#include <iostream>     
//#include <chrono>

using namespace std;
using namespace cv;
CascadeClassifier Detect;

Mat hwnd2mat(HWND hwnd, int fov) {
    HDC hwindowDC, hwindowCompatibleDC;
    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    Mat src;
    BITMAPINFOHEADER  bi;
    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);
    RECT windowsize;
    GetClientRect(hwnd, &windowsize);
    float valueOfChangeTheSizeOfTextAppsAndOtherItemsInWindowsDisplaySettings = 1.5;
    srcheight = fov;
    srcwidth = fov;
    width = srcwidth;
    height = srcheight;
    src.create(height, width, CV_8UC4);
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;
    SelectObject(hwindowCompatibleDC, hbwindow);
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, (windowsize.right - fov) / 2, (windowsize.bottom - fov) / 2, srcwidth, srcheight, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}

HWND Gethwnd() {
    LPCWSTR window_title = L"Counter-Strike: Global Offensive";
    HWND hWnd = FindWindow(NULL, window_title);
    return hWnd;
}


int main() {
    int fov = 150;
    //int num = 0;
    Detect.load("C:\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_default.xml");
    namedWindow("output");
    HWND hWND = Gethwnd();
    while (waitKey(1) != 27) {
        //auto start = chrono::system_clock::now();
        Mat src = hwnd2mat(hWND, fov);
        vector<Rect> features;
        Detect.detectMultiScale(src, features, 1.1, 3, 0, Size(20, 32));
        for (auto&& feature : features) {
            rectangle(src, feature, Scalar(255, 255, 255), 1);
        }
        imshow("output", src);
        //if (num % 50 == 0) {
        //    auto last = chrono::system_clock::now();
        //    chrono::duration<double> elapsed_seconds = last - start;
        //    cout << "FPS: " << 1 / elapsed_seconds.count() << "\n";
        //}
        //num = num + 1;
    }
}
