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
static int MIN_Y = 75;

// Input delay (higher is better for slower computers that process at a slower framerate)
static int INPUT_DELAY_MS = 15;

// Button pressing thresholds. These basically make up the columns that each note belongs to.
static int A_Min = 50;
static int A_Max = 90;
static int B_Min = 140;
static int B_Max = 180;
static int Y_Min = 220;
static int Y_Max = 260;
static int X_Min = 300;
static int X_Max = 340;
static int L_Min = 380;
static int L_Max = 430;

// DEBUG
static bool controlDisabled = false;

// Store keys pressed for visualization
auto keysPressed = vector<char>();

/**
 * Get current time in milliseconds.
 */
int curTimeMs() {
  auto duration = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

/**
 * Handler for regular note presses.
 */
bool doNotePresses(int &delayEnd, cv::Point p) {
  bool btnsHeld = false;

  // Reset the pressed keys tracker
  keysPressed = vector<char>();

  // Check thresholds and see if we can press a button
  // Order is ABYXL
  bool delayed = delayEnd > curTimeMs();

  if (p.y > MIN_Y && !controlDisabled && !delayed) {
    if (p.x > A_Min && p.x < A_Max) {
      pressKey(SL::Input_Lite::KEY_A);
      btnsHeld = true;
      keysPressed.push_back('A');
    }

    if (p.x > B_Min && p.x < B_Max) {
      pressKey(SL::Input_Lite::KEY_S);
      btnsHeld = true;
      keysPressed.push_back('S');
    }

    if (p.x > Y_Min && p.x < Y_Max) {
      pressKey(SL::Input_Lite::KEY_J);
      btnsHeld = true;
      keysPressed.push_back('J');
    }

    if (p.x > X_Min && p.x < X_Max) {
      pressKey(SL::Input_Lite::KEY_K);
      btnsHeld = true;
      keysPressed.push_back('K');
    }

    if (p.x > L_Min && p.x < L_Max) {
      pressKey(SL::Input_Lite::KEY_L);
      btnsHeld = true;
      keysPressed.push_back('L');
    }
  }

  return btnsHeld;
}

/**
 * Handler for open note presses.
 */
bool doOpenPress(int &delayEnd, cv::Point p) {
  bool pressed = false;
  bool delayed = delayEnd > curTimeMs();

  if (p.y > MIN_Y && !controlDisabled && !delayed) {
    // Hitting open notes requires not having anything pressed
    unpressAll();
    keysPressed.push_back('O');

    pressed = true;
  }

  return pressed;
}

void handleTplMatch(cv::Mat &img, cv::Mat &tpl, double thresholdMin, int &delayEnd, string tplType) {
  // Template image size.
  cv::Size s;

  // Note size
  s.height = img.rows - tpl.rows + 1;
  s.width = img.cols - tpl.cols + 1;

  auto zero = cv::Mat::zeros(
    s,
    CV_32SC1
  );

  // Match result
  auto result = cv::Mat();

  cv::matchTemplate(img, tpl, result, cv::TM_CCORR_NORMED);

  auto threshold = cv::Mat();

  // Get only points that match the passed threshold
  cv::threshold(result, threshold, thresholdMin, 1.0, cv::THRESH_BINARY);

  auto pts = vector<cv::Point>();

  // Filter out non-zeros
  cv::findNonZero(threshold, pts);

  // Keep trck of whether any amount of buttons is being held
  bool btnsHeld = false;
  auto ptsSeen = vector<cv::Point>();

  if (pts.size() > 0) {
    auto lastPt = pts.at(0);
    unpressAll();

    for (cv::Point p : pts) {
      // If the last found point is close enough to this one, just skip it
      // This is currently pretty inefficient but I think the performance cost shouldn't be enough to care
      bool tooClose = false;
      for (cv::Point sp : ptsSeen) {
        if (abs(p.x - sp.x) < 20 && abs(p.y - sp.y) < 10) {
          tooClose = true;
          break;
        }
      }

      // Point too close? Ignore it
      if (tooClose) continue;

      // More strictness on open notes
      if (tplType == "open") {
        if (ptsSeen.size() > 0 && abs(p.x - lastPt.x) < 100 && abs(p.y - lastPt.y) < 30) continue;
      }

      lastPt = p;
      ptsSeen.push_back(p);
      
      // Rectangle drawn overtop of found image, just for visualization purposes
      auto rec = cv::Rect();
      rec.x = p.x;
      rec.y = p.y;
      rec.width = tpl.cols;
      rec.height = tpl.rows;

      // White
      auto col = cv::Scalar(255, 255, 255);
      cv::rectangle(img, rec, col);

      // DEBUG show coordinates of match
      stringstream s;
      s << "X: " << p.x << "Y: " << p.y;

      // Pop the X/Y coordinates right above the detected note
      cv::putText(
        img,
        s.str(),
        p,
        cv::FONT_HERSHEY_COMPLEX_SMALL,
        1.0,
        col
      );

      if (tplType == "note") btnsHeld = doNotePresses(delayEnd, p);
      if (tplType == "open") btnsHeld = doOpenPress(delayEnd, p);
    }

    if (btnsHeld) {
      delayEnd = curTimeMs() + chrono::milliseconds(INPUT_DELAY_MS).count();

      strum();
    }
  }
}

int main() {
  auto noteTpl = cv::imread("./notes_images/note_greyscale_small.png", cv::ImreadModes::IMREAD_GRAYSCALE);
  auto barTpl = cv::imread("./notes_images/note_open_greyscale_new_extrasmaller.png", cv::ImreadModes::IMREAD_GRAYSCALE);

  cv::namedWindow("window", cv::WINDOW_NORMAL);

  auto delayEnd = curTimeMs();
  
  while(1) {
    // My (hardcoded) screen width and height. Sorry not sorry.
    int width = 1920;
    int height = 1080;
    int bpp = 0;
    vector<uint8_t> pixels;

    // Get what is basically a screenshot of the whole display
    ImageFromDisplay(pixels, width, height, bpp);

    auto img = cv::Mat(height, width, bpp > 24 ? CV_8UC4 : CV_8UC3, &pixels[0]);
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // Crop to make processing a bit better AND faster
    img = img(cv::Range(550, 700), cv::Range(1000, 1550));

    // Template match notes and open notes
    handleTplMatch(img, noteTpl, 0.745, delayEnd, "note");
    handleTplMatch(img, barTpl, 0.91, delayEnd, "open");

    // Resize the now-processed image, just for preview
    cv::resize(img, img, cv::Size {
      img.cols * 1.2, img.rows * 1.2
    });

    // Draw pressed buttons on screen
    cv::putText(
      img,
      "Keys: " + string(keysPressed.begin(), keysPressed.end()),
      cv::Point(20, img.rows - 20),
      cv::FONT_HERSHEY_COMPLEX_SMALL,
      1.0,
      cv::Scalar(255, 255, 255)
    );

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