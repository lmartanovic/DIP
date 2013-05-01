CC=g++
CFLAGS=-Wall -O2
LIBS=-lassimp -lGLEW -lsfml-window -lsfml-system -lsfml-graphics -lm
BIN=DIP
SRC=./Application/src

#Application
all: Renderer.o Camera.o Geometry.o Shader.o
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)/main.cpp Renderer.o Camera.o Geometry.o Shader.o $(LIBS)

Renderer.o:
	$(CC) $(CFLAGS) -c $(SRC)/Renderer.cpp

Camera.o:
	$(CC) $(CFLAGS) -c $(SRC)/Camera.cpp

Geometry.o:
	$(CC) $(CFLAGS) -c $(SRC)/Geometry.cpp

Shader.o:
	$(CC) $(CFLAGS) -c $(SRC)/Shader.cpp

#Application source code documentation
doc:
	doxygen Doxyfile

report:
	cd ./Report; make pdf
#cleanup
clean:
	rm -rf *~ *.o $(BIN) ./Application/doc/* *.zip

clean-report:
	cd ./Report; make clean

#Git repository update
push:
	git push -u origin master

#Packing
pack:
	zip -r xmarta00.zip ./Report/ ./Application/
	
pack-report:
	zip -r xmarta00-report.zip ./Report/

pack-app:
	zip -r xmarta00-app.zip ./Application/
