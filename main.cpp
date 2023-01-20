#include <iostream>
#include <sstream>
#include <Input_Lite.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>

#include "./screenshot.h"
#include "./controller.h"

using namespace std;

int main() {
  auto noteTpl = cv::imread("../notes_images/note_greyscale_small.png", cv::ImreadModes::IMREAD_GRAYSCALE);

  cv::namedWindow("window", cv::WINDOW_NORMAL);
  
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

    cv::threshold(result, threshold, 0.78, 1.0, cv::THRESH_BINARY);

    auto pts = vector<cv::Point>();

    // Filter out non-zeros
    cv::findNonZero(threshold, pts);

    auto noteTplSize = noteTpl.size();

    if (pts.size() > 0) {
      auto lastPt = pts.at(0);

      for (cv::Point p : pts) {
        // If the last found point is close enough to this one, just skip it
        if (abs(p.x - lastPt.x) < 30 && abs(p.y - lastPt.y) < 30) continue;

        lastPt = p;
        
        auto rec = cv::Rect();
        rec.x = p.x;
        rec.y = p.y;
        rec.width = noteTpl.cols;
        rec.height = noteTpl.rows;

        auto col = cv::Scalar(255, 255, 255);

        cv::rectangle(img, rec, col);

        // Check thresholds and see if we can press a button
        // Order is ABYXL
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
      }
    }

    cv::resize(img, img, cv::Size {
      640, 480
    });

    cv::imshow("window", img);

    int k = cv::waitKey(1);
    if (k == 113) {
      // Close that mf
      break;
    }
  }
}