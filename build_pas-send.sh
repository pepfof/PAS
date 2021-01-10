g++ -rdynamic `pkg-config --cflags gtk+-3.0 glib-2.0` -o ./bin/pas-send.o ./src/pas-send.c `pkg-config --libs gtk+-3.0 glib-2.0` 
sudo cp ./bin/pas-send.o /usr/local/bin/pas-send
cp -R ./paconfig ~/PAS
echo "built."
