#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>

// https://stackoverflow.com/a/24988220
void ImageFromDisplay(std::vector<uint8_t>& pixels, int& w, int& h, int& bitsPerPix) {
  Display* display = XOpenDisplay(nullptr);
  Window root = DefaultRootWindow(display);

  XWindowAttributes attributes = {0};
  XGetWindowAttributes(display, root, &attributes);

  w = attributes.width;
  h = attributes.height;

  XImage* img = XGetImage(display, root, 0, 0 , w, h, AllPlanes, ZPixmap);
  bitsPerPix = img->bits_per_pixel;
  pixels.resize(w * h * 4);

  memcpy(&pixels[0], img->data, pixels.size());

  XDestroyImage(img);
  XCloseDisplay(display);
}