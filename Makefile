SRCS := src/main.cpp src/buffer.cpp src/background.cpp src/xmlParser.cpp
TINYXML_SRCS := tinyxml/tinyxml.cpp tinyxml/tinyxmlparser.cpp tinyxml/tinyxmlerror.cpp tinyxml/tinystr.cpp

all:
	mkdir -p bin
	g++ -std=c++17 -I include -I tinyxml ${SRCS} ${TINYXML_SRCS} -Wall -o bin/exe
