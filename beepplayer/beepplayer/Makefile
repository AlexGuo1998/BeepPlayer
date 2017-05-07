args=-std=c99 -O2

beepplayer: beepplayer.o decode.o play.o
	cc -o beepplayer -lm beepplayer.o decode.o play.o

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
	-rm *.o
	-rm beepplayer