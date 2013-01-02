# variables del make

CXX = g++
CPPFLAGS = -c #-Wall -g -c

#--------------Objetivos-----------------#

all: cmeshfoil

install: 
	@echo '**Installing...'
	cp cmeshfoil /usr/bin
	@echo '**...Installation completed!'

clean:
	#si colocamos un - delante de la orden, esta se silencia
	@echo '**Removing obsolete files...'
	-rm *.h.gch
	-rm *.o
	-rm cmeshfoil
	@echo '**Done...'

#------------- Compilacion de modulos----#


meshGen.o : meshGen.h naca4.o
	$(CXX) $(CPPFLAGS) $^ 

splineShape.o : splineShape.h naca4.o
	$(CXX) $(CPPFLAGS) $^

naca4.o : naca4.h common.o data.o parameterStructs.o
	$(CXX) $(CPPFLAGS) $^

data.o : data.h common.o
	$(CXX) $(CPPFLAGS) $<

parameterStructs.o : parameterStructs.h
	$(CXX) $(CPPFLAGS) $<

common.o : common.h
	$(CXX) $(CPPFLAGS) $<


#------------
cmeshfoil : main.cpp common.o data.o meshGen.o splineShape.o naca4.o
	$(CXX) main.cpp common.o data.o meshGen.o splineShape.o naca4.o -o cmeshfoil


