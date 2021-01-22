g++ -o ./bin/pas-marquee.o ./src/pas-marquee.c `sdl2-config --libs --cflags` -lSDL2_ttf
sudo cp ./bin/pas-marquee.o /usr/local/bin/pas-marquee
echo "built."
pas-marquee -i lulllllllllllllllllllllllllllllllllllllllllllll -t 20000 -s 1
