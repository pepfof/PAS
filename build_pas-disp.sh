g++ -o ./bin/pas-disp.o ./src/pas-disp.c `pkg-config --libs --cflags mpv`
sudo cp ./bin/pas-disp.o /usr/local/bin/pas-disp
echo "built."