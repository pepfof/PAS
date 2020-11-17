g++ -o pas-mpv.o pas-mpv.c `pkg-config --libs --cflags mpv`
echo "built."
./pas-mpv.o 9.mp4
