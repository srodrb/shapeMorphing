CC = g++
CCFLAGS = -c -g

LIBSDIR = -I inc -I obj
OBJDIR = obj

vpath %.h inc
vpath %.cpp src
vpath %.o obj 

APPNAME = cmeshfoil

#.SILENT: //para no ver aviso //para no ver aviss

all: cmeshfoil

clean:
	@echo '**Removing files...'
	rm *.o
	@echo '**All clean!'

install:
	@echo '**Removing previous installation...'
	rm /usr/bin/$(APPNAME)
	rm -r /opt/$(APPNAME)
	@echo '**Installing...'
	cp $(APPNAME) /usr/bin
	@echo '**Creating /opt/$(APPNAME) directory...'
	mkdir /opt/$(APPNAME)
	cd ..
	cp -r * /opt/$(APPNAME)
	@echo '**Done!'

$(APPNAME): main.cpp common.o data.o parameterStructs.o meshGen.o splineShape.o naca4.o
	$(CC) $(LIBSDIR) $^ -o $(APPNAME)
	chmod +x $(APPNAME)
	rm *.o                              #clean objects on actual folder

meshGen.o: meshGen.cpp meshGen.h naca4.o
	$(CC) $(LIBSDIR) $(CCFLAGS) $<

splineShape.o: splineShape.cpp splineShape.h naca4.o
	$(CC) $(LIBSDIR) $(CCFLAGS) $<

naca4.o: naca4.cpp naca4.h common.o data.o parameterStructs.o
	$(CC) $(LIBSDIR) $(CCFLAGS) $<

data.o: data.cpp data.h common.o
	$(CC) $(LIBSDIR) $(CCFLAGS) $<

parameterStructs.o: parameterStructs.cpp parameterStructs.h
	$(CC) $(LIBSDIR) $(CCFLAGS) $<

common.o: common.cpp common.h
	$(CC) $(LIBSDIR) $(CCFLAGS) $<
