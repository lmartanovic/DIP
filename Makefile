BIN=DIP


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
