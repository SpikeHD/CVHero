use std::{fs, path::PathBuf};
use screenshots::Screen;

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

  // When a button is pressed, take a screenshot and record which buttons are being pressed
  // Keep track of previous state to see if we have been holding a button
  loop {
    std::thread::sleep(std::time::Duration::from_millis(32));

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
    let mut image_name = String::new();

    // DEBUG: print buttons
    print!("Buttons pressed: ");
    for b in btn_state.as_slice() {
      image_name += format!("{:?}_", b).as_str();
      print!("{:?}, ", format!("{:?}_", b).as_str());
    }
    println!("");

    image_name += format!("{}_", itr).as_str();

    // Get screen
    let screens = Screen::all().unwrap();
    let screen = screens.first().unwrap();

    // DEBUG: Only if the A button is pressed we create and save a screenshot
    if btn_state.contains(&Buttons::G) {
      let img = screen.capture().unwrap();
      let buf = img.buffer();

      // Write buf to filesystem
      fs::write(PathBuf::from(format!("./training_images/{}.jpg", image_name)), buf).unwrap();
    }
  }
}