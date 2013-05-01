CC=g++
LIBS=-lGLEW -lsfml-window -lsfml-system -lsfml-graphics -lm
BIN=DIP
SRC=./Application/src

#Application
all: Renderer.o
	$(CC) -o $(BIN) $(SRC)/main.cpp Renderer.o Camera.o $(LIBS)

Renderer.o: Camera.o
	$(CC) -c $(SRC)/Renderer.cpp

Camera.o:
	$(CC) -c $(SRC)/Camera.cpp

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
