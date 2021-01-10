g++ -o ./bin/pas-mpv.o ./src/pas-mpv.c `pkg-config --libs --cflags mpv`
sudo cp ./bin/pas-mpv.o /usr/local/bin/pas-mpv
echo "built."
