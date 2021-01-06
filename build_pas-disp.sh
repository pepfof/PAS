g++ -o pas-disp.o pas-disp.c `pkg-config --libs --cflags mpv`
echo "built."
./pas-disp.o
