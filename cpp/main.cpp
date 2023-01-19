#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

int main() {
  auto noteTpl = cv::imread("../notes_images/note_greyscale.png", cv::ImreadModes::IMREAD_GRAYSCALE);

  // DEBUG this is for detection testing
  auto testImg = cv::imread("../testing_images/clonehero3.png", cv::ImreadModes::IMREAD_GRAYSCALE);
  cv::Size s;

  s.height = testImg.rows - noteTpl.rows + 1;
  s.width = testImg.cols - noteTpl.cols + 1;
  auto zero = cv::Mat::zeros(
    s,
    CV_32SC1
  );

  auto result = cv::Mat();

  cv::matchTemplate(testImg, noteTpl, result, cv::TM_CCORR_NORMED);

  auto threshold = cv::Mat();

  cerr << "calced" << endl;

  cv::threshold(result, threshold, 0.70, 1.0, cv::THRESH_BINARY);

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

    cv::rectangle(testImg, rec, col);
  }

    // for n in 0..r.rows()-1 {
    //     let xy: &opencv::core::Point = r.at_2d::<opencv::core::Point>(n, 0).unwrap();
    //     println!("{:?}", xy);
    //     let x = xy.x;
    //     let y = xy.y;
    //     let rect = opencv::core::Rect::new(
    //         x,
    //         y,
    //         note_tpl_w,
    //         note_tpl_h
    //     );

    //     // Draw rect to input image
    //     opencv::imgproc::rectangle(&mut img, rect, 255.into(), 2, opencv::imgproc::LINE_4, 0).unwrap();
    // }

  cv::namedWindow("window", cv::WINDOW_NORMAL);
  
  while(1) {
    cv::imshow("window", testImg);

    int k = cv::waitKey(1);
    if (k == 113) {
      // Close that mf
      break;
    }
  }
}