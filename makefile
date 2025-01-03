SYSCONF_LINK = g++
CPPFLAGS     = -O3
LDFLAGS      = -O3
LIBS         = -lm

DESTDIR = ./bin/
TARGET  = main

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

all: $(TARGET)

$(DESTDIR):
	mkdir -p $(DESTDIR)

$(TARGET): $(OBJECTS)
	$(SYSCONF_LINK) -Wall $(LDFLAGS) -o $(TARGET) $(addprefix $(DESTDIR), $(OBJECTS)) $(LIBS)

$(OBJECTS): %.o: %.cpp | $(DESTDIR)
	$(SYSCONF_LINK) -Wall $(CPPFLAGS) -c $(CFLAGS) $< -o $(DESTDIR)$@

run: $(TARGET)
	./$(TARGET)

clean:
	-rm -f $(DESTDIR)*
	-rm -f $(TARGET)
	-rm -f *.tga
