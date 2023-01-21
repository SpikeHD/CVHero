#include <iostream>
#include <sstream>
#include <Input_Lite.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <chrono>

#include "./screenshot.h"
#include "./controller.h"

using namespace std;

// Y value to hit buttons at
static int MIN_Y = 600;

// Button pressing thresholds
static int A_Min = 1000;
static int A_Max = 1080;
static int B_Min = 1100;
static int B_Max = 1180;
static int Y_Min = 1200;
static int Y_Max = 1280;
static int X_Min = 1300;
static int X_Max = 1380;
static int L_Min = 1400;
static int L_Max = 1480;

int curTimeMs() {
  auto duration = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

int main() {
  auto noteTpl = cv::imread("./notes_images/note_greyscale_small.png", cv::ImreadModes::IMREAD_GRAYSCALE);

  cv::namedWindow("window", cv::WINDOW_NORMAL);

  auto delayEnd = curTimeMs();
  
  while(1) {
    int width = 1920;
    int height = 1080;
    int bpp = 0;
    vector<uint8_t> pixels;

    ImageFromDisplay(pixels, width, height, bpp);

    auto img = cv::Mat(height, width, bpp > 24 ? CV_8UC4 : CV_8UC3, &pixels[0]);
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    cv::Size s;

    s.height = img.rows - noteTpl.rows + 1;
    s.width = img.cols - noteTpl.cols + 1;
    auto zero = cv::Mat::zeros(
      s,
      CV_32SC1
    );

    auto result = cv::Mat();

    cv::matchTemplate(img, noteTpl, result, cv::TM_CCORR_NORMED);

    auto threshold = cv::Mat();

    cv::threshold(result, threshold, 0.77, 1.0, cv::THRESH_BINARY);

    auto pts = vector<cv::Point>();

    // Filter out non-zeros
    cv::findNonZero(threshold, pts);

    auto noteTplSize = noteTpl.size();

    // eep trck of whether any amount of buttons is being held
    bool btnsHeld = false;

    if (pts.size() > 0) {
      bool firstPt = true;
      auto lastPt = pts.at(0);

      for (cv::Point p : pts) {
        // If the last found point is close enough to this one, just skip it
        if (!firstPt && abs(p.x - lastPt.x) < 30 && abs(p.y - lastPt.y) < 30) continue;

        firstPt = false;
        lastPt = p;
        
        auto rec = cv::Rect();
        rec.x = p.x;
        rec.y = p.y;
        rec.width = noteTpl.cols;
        rec.height = noteTpl.rows;

        auto col = cv::Scalar(255, 255, 255);

        cv::rectangle(img, rec, col);

        // DEBUG show coordinates of match
        stringstream s;
        s << "X: " << p.x << "Y: " << p.y;

        cv::putText(
          img,
          s.str(),
          p,
          cv::FONT_HERSHEY_COMPLEX_SMALL,
          1.0,
          col
        );

        // Check thresholds and see if we can press a button
        // Order is ABYXL
        bool controlDisabled = false;
        bool delayed = delayEnd > curTimeMs();

        if (p.y > MIN_Y && !controlDisabled && !delayed) {
          delayEnd = curTimeMs() + chrono::milliseconds(100).count();

          if (p.x > A_Min && p.x < A_Max) {
            pressKey(SL::Input_Lite::KEY_A);
            btnsHeld = true;
          }

          if (p.x > B_Min && p.x < B_Max) {
            pressKey(SL::Input_Lite::KEY_S);
            btnsHeld = true;
          }

          if (p.x > Y_Min && p.x < Y_Max) {
            pressKey(SL::Input_Lite::KEY_J);
            btnsHeld = true;
          }

          if (p.x > X_Min && p.x < X_Max) {
            pressKey(SL::Input_Lite::KEY_K);
            btnsHeld = true;
          }

          if (p.x > L_Min && p.x < L_Max) {
            pressKey(SL::Input_Lite::KEY_L);
            btnsHeld = true;
          }
        }
      }
    }

    if (btnsHeld) {
      strum();
      unpressAll();
    }

    cv::resize(img, img, cv::Size {
      852, 480
    });

    // cv::resize(result, result, cv::Size {
    //   852, 480
    // });

    cv::imshow("window", img);

    int k = cv::waitKey(1);
    if (k == 113) {
      // Close that mf
      break;
    }
  }
}