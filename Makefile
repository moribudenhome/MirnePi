CXXFLAGS = -MD -lpthread -lrt -rdynamic -std=c++11
CXXFLAGS += $(shell sdl2-config --cflags --libs)
CXXFLAGS += -lSDL2_ttf -lSDL2_image -g

OBJS = \
    main.o \
	OmxPlayer.o \
	SDLContext.o \
	Explorer.o \
	JoyStick.o \
	WGWidgetBase.o \
	WGSprite.o \

LIBS =

TARGET =	MirnePi

$(TARGET):	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)
	
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o $(LIBS)
OmxPlayer.o: Player/OmxPlayer.cpp
	$(CXX) $(CXXFLAGS) -c Player/OmxPlayer.cpp -o OmxPlayer.o $(LIBS)
Explorer.o: Player/Explorer.cpp
	$(CXX) $(CXXFLAGS) -c Player/Explorer.cpp -o Explorer.o $(LIBS)

SDLContext.o: SDL/SDLContext.cpp
	$(CXX) $(CXXFLAGS) -c SDL/SDLContext.cpp -o SDLContext.o $(LIBS)	
JoyStick.o: SDL/JoyStick.cpp
	$(CXX) $(CXXFLAGS) -c SDL/JoyStick.cpp -o JoyStick.o $(LIBS)
	
WGWidgetBase.o: Widget/WGWidgetBase.cpp
	$(CXX) $(CXXFLAGS) -c Widget/WGWidgetBase.cpp -o WGWidgetBase.o $(LIBS)
WGSprite.o: Widget/Parts/WGSprite.cpp
	$(CXX) $(CXXFLAGS) -c Widget/Parts/WGSprite.cpp -o WGSprite.o $(LIBS)
all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	
-include *.d
