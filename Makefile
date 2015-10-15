TARGET   = diretiger
OBJECTS  = main.o Bezier.o Camera.o Point.o Vector.o
OBJECTS += Rocketship.o Familiar.o BezPatch.o Light.o
OBJECTS += Color.o FPSCounter.o SceneElement.o PatchHero.o
OBJECTS += Car.o Orientation.o Material.o BezierHero.o
OBJECTS += Car.o Orientation.o Material.o Wagon.o BezierHero.o

CXX    = g++
CFLAGS = -Wall -g

#############################
## SETUP GLUI
##      We must link against
##  GLUI before we link against
##  GLUT.
##
##     If you want to build
##  on your own machine, you
##  need to change the
##  appropriate paths.
#############################

# Windows Lab builds 
ifeq ($(OS), Windows_NT)
	INCPATH += -IC:/Strawberry/c/CSCI441-Code/DireTigerGraphics/include/
	LIBPATH += -IC:/Strawberry/c/CSCI441-Code/DireTigerGraphics/lib/
	INCPATH += -IZ:/CSCI441GFx/include/
	LIBPATH += -LZ:/CSCI441GFx/lib/

# Non-Windows build
else
	#Feel free to add any directories you have on your system
	#And if you choose to comment out any, probably best to not commit
	#it, as it might break other people's builds
	INCPATH += -I/home/rey/csm/graphics/include/
	LIBPATH += -L/home/rey/csm/graphics/lib/
	INCPATH += -I/Users/jpaone/Desktop/include/
	LIBPATH += -L/Users/jpaone/Desktop/lib
endif

##LIBS += -lglui

#############################
## SETUP OpenGL & GLUT 
#############################

# Windows builds 
ifeq ($(OS), Windows_NT)
	INCPATH += -IC:/Strawberry/c/include/
	LIBS += -lglut -lopengl32 -lglu32 -LC:/Strawberry/c/lib/

# Mac builds
else ifeq ($(shell uname), Darwin)
	LIBS += -framework GLUT -framework OpenGL

# Linux and all other builds
else
	LIBS += -lglut -lGL -lGLU
endif

#############################
## COMPILATION INSTRUCTIONS 
#############################

all: $(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

depend:
	rm -f Makefile.bak
	mv Makefile Makefile.bak
	sed '/^# DEPENDENCIES/,$$d' Makefile.bak > Makefile
	echo '# DEPENDENCIES' >> Makefile
	$(CXX) $(INCPATH) -MM *.cpp >> Makefile

.c.o: 	
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

.cc.o: 	
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

.cpp.o: 	
	$(CXX) $(CFLAGS) $(INCPATH) -c -o $@ $<

$(TARGET): $(OBJECTS) 
	$(CXX) $(CFLAGS) $(INCPATH) -o $@ $^ $(LIBPATH) $(LIBS)

# DEPENDENCIES
BezPatch.o: BezPatch.cpp BezPatch.h Bezier.h Point.h Vector.h \
 SceneElement.h Orientation.h Material.h
Bezier.o: Bezier.cpp Bezier.h Point.h Vector.h
BezierHero.o: BezierHero.cpp BezierHero.h Bezier.h Point.h Vector.h \
 SceneElement.h Orientation.h Material.h
Camera.o: Camera.cpp Camera.h Point.h Vector.h
Car.o: Car.cpp Car.h Point.h Vector.h
Color.o: Color.cpp Color.h
FPSCounter.o: FPSCounter.cpp FPSCounter.h
Familiar.o: Familiar.cpp Familiar.h Bezier.h Point.h Vector.h
Light.o: Light.cpp Light.h Point.h Vector.h Color.h
Material.o: Material.cpp Material.h
Orientation.o: Orientation.cpp Orientation.h Vector.h
PatchHero.o: PatchHero.cpp PatchHero.h SceneElement.h Point.h Vector.h \
 Orientation.h Material.h BezPatch.h Bezier.h
Point.o: Point.cpp Point.h Vector.h
Rocketship.o: Rocketship.cpp Rocketship.h BezierHero.h Bezier.h Point.h \
 Vector.h SceneElement.h Orientation.h Material.h
SceneElement.o: SceneElement.cpp SceneElement.h Point.h Vector.h \
 Orientation.h Material.h
Vector.o: Vector.cpp Vector.h
Wagon.o: Wagon.cpp Wagon.h Point.h Vector.h
main.o: main.cpp Point.h Vector.h Camera.h BezPatch.h Bezier.h \
 SceneElement.h Orientation.h Material.h Familiar.h Rocketship.h \
 BezierHero.h Car.h Wagon.h Light.h Color.h FPSCounter.h PatchHero.h
