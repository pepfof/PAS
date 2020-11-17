g++ -rdynamic `pkg-config --cflags gtk+-3.0 glib-2.0` -o pas-control.o pas-control.c `pkg-config --libs gtk+-3.0 glib-2.0` 
echo "built."
sleep 2
./pas-control.o
