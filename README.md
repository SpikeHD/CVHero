# CVHero
Cheating in Clone Hero using OpenCV

# Building

1. Install Input_Lite (https://github.com/smasherprog/input_lite)
2. Install OpenCV (https://opencv.org/releases/)
3. Run `make`

# Running

You should not run this program in it's current state. It uses all sorts of hardcoded values, resolutions, offsets, and more.
It also only works on Linux with an X11 desktop environment. Just don't bother right now, not that you should anyways, cheating is bad and wrong.

# Todo

* [ ]  Better star note detection
  * Since input image is now cropped, false detections shouldn't happen when the confidence threshold is lowered. Guess we'll see.
* [ ] Fix hold bars being detected as notes, causing combo-breaks or hold-breaks
* [x] Hold notes
  * [x] Starting AND finishing
* [ ] MAYBE Properly handle combo notes (AKA don't strum)
  * Kinda hard, how to know if combo has been broken or not? Not worth the effort probs

# Current Progress
Not indicitive of true performance. This is running off of my less-powerful laptop while OBS runs it's software video encoding. Performance is even better when recording is not being done!

https://user-images.githubusercontent.com/25207995/213955567-a3090e00-4270-4d35-b089-76aa9671e707.mp4

