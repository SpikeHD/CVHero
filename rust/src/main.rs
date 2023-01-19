
use opencv::{
    prelude::*, highgui, imgcodecs, imgproc, core::ToInputArray
};

fn main() {
    // // Create training data folder
    // if !Path::new("./training_images").is_dir() {
    //     println!("Training data folder does not exist, creating...");
    //     fs::create_dir("./training_images");
    // }

    let note_tpl = imgcodecs::imread("./notes_images/note_greyscale.png", imgcodecs::IMREAD_GRAYSCALE).unwrap();
    let combo_tpl = imgcodecs::imread("./notes_images/combo_greyscale.png", imgcodecs::IMREAD_GRAYSCALE).unwrap();
    let star_tpl = imgcodecs::imread("./notes_images/star_greyscale.png", imgcodecs::IMREAD_GRAYSCALE).unwrap();
    let star_combo_tpl = imgcodecs::imread("./notes_images/star_combo_greyscale.png", imgcodecs::IMREAD_GRAYSCALE).unwrap();

    // train::capture_data();
    highgui::named_window("Window", highgui::WINDOW_NORMAL).unwrap();

    // Get one of the testing images
    let mut img = imgcodecs::imread("./testing_images/clonehero3_greyscale.png", imgcodecs::IMREAD_GRAYSCALE).unwrap();

    let zero = Mat::zeros(
        img.rows() - note_tpl.rows() + 1,
        img.cols() - note_tpl.cols() + 1,
        opencv::core::CV_32SC1,
    )
    .unwrap();
    let mut r = zero.to_mat().unwrap();
    let img_mask = Mat::default();

    // Detect notes in image
    imgproc::match_template(&img, &note_tpl, &mut r, imgproc::TM_CCORR_NORMED, &img_mask).unwrap();

    let mut min_val: f64 = 0.0;
    let mut max_val: f64 = 0.0;
    let mut min_loc: opencv::core::Point = opencv::core::Point::default();
    let mut max_loc: opencv::core::Point = opencv::core::Point::default();

    opencv::core::min_max_loc(&r, Some(&mut min_val), Some(&mut max_val), Some(&mut min_loc), Some(&mut max_loc), &img_mask).unwrap();

    println!("{:?}: {:?}", max_loc, max_val);

    let mut threshold = Mat::default();

    imgproc::threshold(&r, &mut threshold, 0.7, 1.0, imgproc::THRESH_BINARY).unwrap();

    // Filter out non-zeros
    opencv::core::find_non_zero(&threshold, &mut r).unwrap();

    let note_tpl_h = note_tpl.rows();
    let note_tpl_w = note_tpl.cols();

    for n in 0..r.rows()-1 {
        let xy: &opencv::core::Point = r.at_2d::<opencv::core::Point>(n, 0).unwrap();
        println!("{:?}", xy);
        let x = xy.x;
        let y = xy.y;
        let rect = opencv::core::Rect::new(
            x,
            y,
            note_tpl_w,
            note_tpl_h
        );

        // Draw rect to input image
        opencv::imgproc::rectangle(&mut img, rect, 255.into(), 2, opencv::imgproc::LINE_4, 0).unwrap();
    }


    loop {
        std::thread::sleep(std::time::Duration::from_millis(50));

        highgui::imshow("Window", &img).unwrap();
        let key = highgui::wait_key(1).unwrap();
        if key == 113 { // quit with q
            break;
        }
    }
}

















