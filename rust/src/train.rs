use std::{fs, path::PathBuf, time::Instant};
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
  let mut btn_map: Vec<(String, u128)> = Vec::new();
  let begin_ts = std::time::Instant::now();

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
    let mut btn_string = String::new();

    // DEBUG: print buttons
    print!("[{}] Buttons pressed: ", itr);
    for b in btn_state.as_slice() {
      btn_string += format!("{:?}_", b).as_str();
      print!("{:?}, ", format!("{:?}_", b).as_str());
    }
    println!("");

    // Get screen
    let screens = Screen::all().unwrap();
    let screen = screens.first().unwrap();

    if btn_state.len() > 0 {
      // Write buf to filesystem
      let pair = (btn_string, begin_ts.elapsed().as_millis());
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