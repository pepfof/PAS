g++ -rdynamic `pkg-config --cflags gtk+-3.0 glib-2.0` -o ./bin/pas-control.o ./src/pas-control.c `pkg-config --libs gtk+-3.0 glib-2.0` 
sudo cp ./bin/pas-control.o /usr/local/bin/pas-control
sudo cp ./bin/pas-control.glade ~/PAS/paconfig/pas-control.glade
echo "built."
pas-control
