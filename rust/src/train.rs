use std::{fs, path::PathBuf, time::Instant};
use scrap;
use image;
use quad_gamepad::*;

//These go by Guitar Hero guitar button color, nto button name. Easier to keep track of
#[derive(PartialEq)]
#[derive(Debug)]
#[derive(Clone)]
enum Buttons {
  G,
  R,
  B,
  Y,
  O = 9
}

pub fn capture_data() {
  let mut itr = 0;
  let mut controller = ControllerContext::new().unwrap();
  let mut btn_map: Vec<(String, u128)> = Vec::new();
  let begin_ts = std::time::Instant::now();

  // For getting screenshots
  let mut capturer = get_capturer();
  let w = capturer.width();
  let h = capturer.height();

  // When a button is pressed, take a screenshot and record which buttons are being pressed
  // Keep track of previous state to see if we have been holding a button
  loop {
    std::thread::sleep(std::time::Duration::from_millis(46));

    controller.update();
    itr += 1;

    // State of button presses
    let mut btn_state: Vec<Buttons> = Vec::new();

    // Controller status
    let status = controller.state(0).status;
    if status == ControllerStatus::Connected {
      let state = controller.state(0);

      // Possible buttons to be pressed and recorded into state
      let possible_btns = [ Buttons::G, Buttons::R, Buttons::B, Buttons::Y, Buttons::O ]; 

      for b in possible_btns {
        let ub = b.clone() as usize;
        if state.digital_state[ub] {
          btn_state.push(b);
        }
      }
    }

    // Use state to name the image
    let mut btn_string = String::new();

    // DEBUG: print buttons
    print!("[{}] Buttons pressed: ", itr);
    for b in btn_state.as_slice() {
      btn_string += format!("{:?}_", b).as_str();
      print!("{:?}, ", format!("{:?}_", b).as_str());
    }
    println!("");

    let ts = begin_ts.elapsed().as_millis();

    // Capture frame and write
    match capturer.frame() {
      Ok(frame) => {
        let buffer = &*frame;
        let mut bitflipped = Vec::with_capacity(w * h * 4);
        let stride = buffer.len() / h;

        for y in 0..h {
          for x in 0..w {
            let i = stride * y + 4 * x;
            bitflipped.extend_from_slice(&[
              buffer[i],
              buffer[i + 1],
              buffer[i + 2],
          ]);
        }

        // Write bitflipped image
        image::save_buffer_with_format(
          PathBuf::from(format!("./training_images/{}.png", ts)),
          &bitflipped,
          w as u32,
          h as u32,
          image::ColorType::Rgba8,
          image::ImageFormat::Png
        );
      } 
      }
      Err(ref e) if e.kind() == std::io::ErrorKind::WouldBlock => {
        // Wait.
      },
      Err(e) => {
        println!("{}", e);
      }
    };

    if btn_state.len() > 0 {
      // Store timestamp/button combo
      let pair = (btn_string, ts);
      btn_map.push(pair);
    }

    // DEBUG stop after 500 iterations
    if itr == 200 {
      println!("Captured {} timestamps of button input", btn_map.len());
      println!("First buttons looked like this: ({}, {})", btn_map.first().unwrap().0, btn_map.first().unwrap().1);
      break;
    }
  }
}

pub fn get_capturer() -> scrap::Capturer {
  let display = scrap::Display::primary().unwrap();
  scrap::Capturer::new(display).unwrap()
}