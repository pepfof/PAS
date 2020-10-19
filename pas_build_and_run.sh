g++ -rdynamic `pkg-config --cflags gtk+-3.0 glib-2.0` -o pas.o pas.c `pkg-config --libs gtk+-3.0 glib-2.0` 
./pas.o
