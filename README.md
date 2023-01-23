# CVHero

<div align="center">
This project is a research project for myself involving <a href="https://opencv.org/">OpenCV</a> and <a href="https://clonehero.net/">Clone Hero</a>. It uses template matching to attempt to play the game.
<br />
It definitely performs better than me, and overall I believe it does fairly well! (See: <a href="#scores">Scores</a>)
</div>

# Table of Contents

* [CVHero](#cvhero)
* [Table of Contents](#table-of-contents)
* [Building](#building)
* [Running](#running)
* [TODO](#todo)
* [Current Progress](#current-progress)
  * [Scores](#scores)
  * [Example Video](#example-video)

# Building

Currently somewhat fine-tuned for my own computer. Modify the Makefile if you have issues. Or just don't cheat 😡.

## Requirements
* `g++`
* X11 Desktop Environment (yea, I know, it's a silly requirement, I have yet to capture the screen in a cross-platform way)

## Build
1. Install Input_Lite (https://github.com/smasherprog/input_lite)
2. Install OpenCV (https://opencv.org/releases/)
3. Run `make`

## Run
```sh
./cvhero
```

# Running

You should not run this program in it's current state. It uses all sorts of hardcoded values, resolutions, offsets, and more.
It also only works on Linux with an X11 desktop environment. Just don't bother right now, not that you should anyways, cheating is bad and wrong.

# TODO

* [ ] Do the 8x combo power thing.
  * Could get fancy with it and detect if there is juice in the bar. Or just do the keybind every X seconds. Latter is probably easier.
* [ ] Better star note detection
  * Since input image is now cropped, false detections shouldn't happen when the confidence threshold is lowered. Guess we'll see.
* [ ] Fix hold bars being detected as notes, causing combo-breaks or hold-breaks.
* [x] Fix fretboard lines being detected as open notes. I think I just need a better open-note template to match.
  * This issue and the one above seems to be caused by thick fretboard lines showing up. Ugh.
* [x] Hold notes
  * [x] Starting AND finishing
* [ ] MAYBE Properly handle combo notes (AKA don't strum)
  * Kinda hard, how to know if combo has been broken or not? Not worth the effort probs

# Current Progress

## Scores
**As of 2023-01-22** | Bot does not yet activate the 8x score thing lol

| Song | Difficulty | Accuracy | Stars | Score | Best Streak |
|-|-|-|-|-|-|
| "Soulless V" | Expert | 92% | 4☆ | 1,796,349 | 510
| "Flamewall" | Expert | 98% | 5☆ | 529,132 | 396 
| "Infinite Blue" | Expert | 97% | 4☆ | 221,248 | 172
| "Inori -warera shinso to tomo no ayumu mono nari-" | Expert | 96% | 5☆ | 232,712 | 312|

## Example Video

Not indicitive of true performance since I have OBS running, but is pretty close.

https://user-images.githubusercontent.com/25207995/213955567-a3090e00-4270-4d35-b089-76aa9671e707.mp4

