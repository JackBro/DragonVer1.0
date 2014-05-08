
BATCHPARSE	=	./bin/batchparse.o
BUFFERBASE	=	./bin/bufferbase.o
CHARTOOLS	=	./bin/chartools.o
DBUGTOOLS	=	./bin/dbugtools.o
FIFOBUF		=	./bin/fifobuf.o
FILEBASE	=	./bin/filebase.o
FILEBIN		=	./bin/filebin.o
FILOBUF		=	./bin/filobuf.o
GENERALTOOLS	=	./bin/generaltools.o
SPANBUF		=	./bin/spanbuf.o
STRINGTOOLS	=	./bin/stringtools.o

MAIN		=	./bin/main.o

AUDIF		=	./bin/audif.o
CHIPBASE	=	./bin/chipbase.o
CPM		=	./bin/cpm.o
DBUGSET		=	./bin/dbugset.o
GE		=	./bin/ge.o
GLOBAL		=	./bin/GLOBAL.o
HOSTIF		=	./bin/hostif.o
IPP		=	./bin/ipp.o
ISP		=	./bin/isp.o
JPEG		=	./bin/jpeg.o
LBUF		=	./bin/lbuf.o
LCDC		=	./bin/lcdc.o
LCDIF		=	./bin/lcdif.o
MARB		=	./bin/marb.o
PANEL		=	./bin/panel.o
PLLCTRL		=	./bin/pllctrl.o
SDCARD		=	./bin/sdcard.o
SENSOR		=	./bin/sensor.o
SIF		=	./bin/sif.o
STO		=	./bin/sto.o
USBC		=	./bin/usbc.o
V568		=	./bin/v568.o


OBJS 		=	$(MAIN) \
			$(BATCHPARSE) $(BUFFERBASE) $(CHARTOOLS) $(DBUGTOOLS) $(FIFOBUF) $(FILEBASE)	\
			$(FILEBIN) $(FILOBUF) $(GENERALTOOLS) $(SPANBUF) $(STRINGTOOLS)	\
			$(AUDIF) $(CHIPBASE) $(CPM) $(DBUGSET) $(GE) $(GLOBAL) $(HOSTIF) $(IPP) $(ISP) \
			$(JPEG) $(LBUF) $(LCDC) $(LCDIF) $(MARB) $(PANEL) $(PLLCTRL) $(SDCARD) $(SENSOR) \
			$(SIF) $(STO) $(USBC) $(V568)	\
			
			
CC		=	g++

TARGET		=	./bin/test

SRCHEADER	=	./tools/batchparse.h ./tools/bufferbase.h ./tools/chartools.h 	\
			./tools/dbugtools.h ./tools/fifobuf.h ./tools/filebase.h	\
			./tools/filebin.h ./tools/filobuf.h ./tools/generaltools.h	\
			./tools/spanbuf.h ./tools/stringtools.h 	\
			./cmodelenv/main.h	\
			./cmodel/audif.h ./cmodel/chipbase.h ./cmodel/cpm.h 	\
			./cmodel/dbugset.h ./cmodel/ge.h ./cmodel/global.h 	\
			./cmodel/hostif.h ./cmodel/ipp.h ./cmodel/isp.h ./cmodel/jpeg.h	\
			./cmodel/lbuf.h ./cmodel/lcdc.h ./cmodel/lcdif.h ./cmodel/marb.h\
			./cmodel/panel.h ./cmodel/pllctrl.h ./cmodel/sdcard.h	\
			./cmodel/sensor.h ./cmodel/sif.h ./cmodel/sto.h ./cmodel/usbc.h	\
			./cmodel/v568.h
			
SRC		=	./tools/batchparse.cpp ./tools/bufferbase.cpp ./tools/chartools.cpp 	\
			./tools/dbugtools.cpp ./tools/fifobuf.cpp ./tools/filebase.cpp	\
			./tools/filebin.cpp ./tools/filobuf.cpp ./tools/generaltools.cpp	\
			./tools/spanbuf.cpp ./tools/stringtools.cpp 	\
			./cmodelenv/main.cpp	\
			./cmodel/audif.cpp ./cmodel/chipbase.cpp ./cmodel/cpm.cpp 	\
			./cmodel/dbugset.cpp ./cmodel/ge.cpp ./cmodel/global.cpp 	\
			./cmodel/hostif.cpp ./cmodel/ipp.cpp ./cmodel/isp.cpp ./cmodel/jpeg.cpp	\
			./cmodel/lbuf.cpp ./cmodel/lcdc.cpp ./cmodel/lcdif.cpp ./cmodel/marb.cpp\
			./cmodel/panel.cpp ./cmodel/pllctrl.cpp ./cmodel/sdcard.cpp	\
			./cmodel/sensor.cpp ./cmodel/sif.cpp ./cmodel/sto.cpp ./cmodel/usbc.cpp	\
			./cmodel/v568.cpp

CC	=	g++

$(TARGET):	$(SRC) $(SRCHEADER)
	$(CC) -o $@ $(SRC)

.PHONY	:	clean
clean:
	rm -f $(TARGET)
