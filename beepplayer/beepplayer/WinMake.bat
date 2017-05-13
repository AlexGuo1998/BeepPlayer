set args=-std=c99 -O2

gcc -c %args% beepplayer.c
gcc -c %args% decode.c
gcc -c %args% play.c
gcc -o beepplayer -lm %args% beepplayer.o decode.o play.o

pause
