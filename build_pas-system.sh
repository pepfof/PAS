g++ -rdynamic `pkg-config --cflags gtk+-3.0 glib-2.0` -o ./bin/pas-overlord.o ./src/pas-overlord.c `pkg-config --libs gtk+-3.0 glib-2.0` 
sudo cp ./bin/pas-overlord.o /usr/local/bin/pas-overlord
cp -R ./paconfig ~/PAS
echo "built."
sleep 2
pas-overlord
