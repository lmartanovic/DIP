BIN=DIP


#Application source code documentation
doc:
	doxygen Doxyfile
#cleanup
clean:
	rm -rf *~ *.o $(BIN) ./Application/doc/* *.zip

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
