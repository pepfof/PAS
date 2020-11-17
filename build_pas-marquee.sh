g++ -o pas-marquee.o pas-marquee.c `sdl2-config --libs --cflags` -lSDL2_ttf
echo "built."
sleep 1
./pas-marquee.o -i lul
