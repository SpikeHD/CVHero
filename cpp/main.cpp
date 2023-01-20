#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>

#include "./screenshot.h"
#include "./controller.h"

using namespace std;

// Only keycodes we need
enum KC {
  a = 65,
  s = 83,
  j = 74,
  k = 75,
  l = 76
};

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

    for (cv::Point p : pts) {
      auto rec = cv::Rect();
      rec.x = p.x;
      rec.y = p.y;
      rec.width = noteTpl.cols;
      rec.height = noteTpl.rows;

      auto col = cv::Scalar(255, 255, 255);

      cv::rectangle(img, rec, col);
    }

    cv::resize(img, img, cv::Size {
      640, 480
    });

    cv::imshow("window", img);

    // Press the a key for testing
    pressKey(KC::a);

    int k = cv::waitKey(1);
    if (k == 113) {
      // Close that mf
      break;
    }
  }
}