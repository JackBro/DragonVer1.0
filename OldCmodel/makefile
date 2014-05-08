
SRC = 	main.cpp chip568.cpp chipctrl.cpp dbugPrint.cpp ge.cpp ipp.cpp isp.cpp	\
	jbuf.cpp jpeg.cpp lbuf.cpp lcdif.cpp sensorIF.cpp busif.cpp global.cpp chipbase.cpp
	
SRCHEADER = main.h chip568.h chipctrl.h dbugPrint.h ge.h ipp.h isp.h jbuf.h jpeg.h \
		lbuf.h lcdif.h sensorIF.h global.h busif.h chipbase.h
		
TARGET	=	../bin/test

CC	=	g++

$(TARGET):	$(SRC) $(SRCHEADER)
	$(CC) -o $@ $(SRC)

.PHONY	:	clean
clean:
	rm -f $(TARGET)
	
	
