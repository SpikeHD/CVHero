build:
	PKG_CONFIG_PATH=/usr/local/lib/pkgconfig/ g++ -I/usr/local/include/opencv4/ -I/usr/include/X11/ -o cvhero main.cpp screenshot.cpp controller.cpp -O3 -L/usr/local/lib/ -l:libinput_lite.a -lXext -lXtst -lX11  `pkg-config opencv4 --cflags --libs`
