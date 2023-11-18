# Álvaro Bernabeu Gomariz - 77042743B
#OPTS = -mthumb

# desencadenar otros objetivos
#all: programa 

#programa: programa.cc												
#	g++ ${OPTS} -o programa programa.cc	 

#clean: 
#	rm -f programa 


# Makefile
CXX = g++
TARGET = programa_thumb
SOURCE = programa.cc

all: $(TARGET)

$(TARGET): $(SOURCE)
    $(CXX) -mthumb -o $@ $<

clean:
    rm -f $(TARGET)
