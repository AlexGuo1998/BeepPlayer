args=-O2

beepplayer: beepplayer.o decode.o play.o
	cc -o beepplayer beepplayer.o decode.o play.o -lm

beepplayer.o: beepplayer.c decode.h play.h
	cc -c $(args) beepplayer.c

decode.o: decode.c decode.h
	cc -c $(args) decode.c

play.o: play.c play.h
	cc -c $(args) play.c

chmod: beepplayer
	sudo chown root beepplayer
	sudo chmod 4774 beepplayer

clean:
	-rm -f beepplayer *.o
