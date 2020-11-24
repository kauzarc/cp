CC = gcc

SRC_PATH = src

SRC_FOLDER = src
HEADER_FOLDER = header

BUILD_PATH = build

MAIN = main

SRC_EXT = c
BUILD_EXT = o

CXXFLAGS = -Wall
LDFLAGS =

EXEC = cp

DEBUG_FOLDER = debug
DBGCFLAGS = -g -O0 -DDEBUG

RELEASE_FOLDER = release
RELCFLAGS = -O2

SRC:= $(wildcard $(SRC_PATH)/$(SRC_FOLDER)/*.$(SRC_EXT))

DBEXEC = $(BUILD_PATH)/$(DEBUG_FOLDER)/$(EXEC)
DBOBJ:= $(SRC:$(SRC_PATH)/$(SRC_FOLDER)/%.$(SRC_EXT)=$(BUILD_PATH)/$(DEBUG_FOLDER)/%.$(BUILD_EXT))
DBOBJ:= $(DBOBJ) $(BUILD_PATH)/$(DEBUG_FOLDER)/$(MAIN).$(BUILD_EXT)

REEXEC = $(BUILD_PATH)/$(RELEASE_FOLDER)/$(EXEC)
REOBJ:= $(SRC:$(SRC_PATH)/$(SRC_FOLDER)/%.$(SRC_EXT)=$(BUILD_PATH)/$(RELEASE_FOLDER)/%.$(BUILD_EXT))
REOBJ:= $(REOBJ) $(BUILD_PATH)/$(RELEASE_FOLDER)/$(MAIN).$(BUILD_EXT)

all: debug

debug: prep $(DBEXEC) dbgcp

$(DBEXEC): $(DBOBJ)
	$(CC) -o $@ $(LDFLAGS) $(DBGCFLAGS) $^

$(BUILD_PATH)/$(DEBUG_FOLDER)/%.$(BUILD_EXT): $(SRC_PATH)/$(SRC_FOLDER)/%.$(SRC_EXT)
	$(CC) -c $^ $(CXXFLAGS) $(DBGCFLAGS) -o $@

$(BUILD_PATH)/$(DEBUG_FOLDER)/%.$(BUILD_EXT): $(SRC_PATH)/%.$(SRC_EXT)
	$(CC) -c $^ $(CXXFLAGS) $(DBGCFLAGS) -o $@

dbgcp:
	cp $(DBEXEC) $(EXEC)

release:prep $(REEXEC) relcp

$(REEXEC): $(REOBJ)
	$(CC) -o $@ $(LDFLAGS) $(RELCFLAGS) $^

$(BUILD_PATH)/$(RELEASE_FOLDER)/%.$(BUILD_EXT): $(SRC_PATH)/$(SRC_FOLDER)/%.$(SRC_EXT)
	$(CC) -c $^ $(CXXFLAGS) $(RELCFLAGS) -o $@

$(BUILD_PATH)/$(RELEASE_FOLDER)/%.$(BUILD_EXT): $(SRC_PATH)/%.$(SRC_EXT)
	$(CC) -c $^ $(CXXFLAGS) $(RELCFLAGS) -o $@

relcp:
	cp $(REEXEC) $(EXEC)

clean:
	rm -f $(BUILD_PATH)/$(DEBUG_FOLDER)/*.$(BUILD_EXT) $(BUILD_PATH)/$(RELEASE_FOLDER)/*.$(BUILD_EXT) $(EXEC)

prep:
	mkdir -p $(BUILD_PATH) $(BUILD_PATH)/$(RELEASE_FOLDER) $(BUILD_PATH)/$(DEBUG_FOLDER)

remake: clean all

folder:
	mkdir $(BUILD_PATH) $(BUILD_PATH)/$(RELEASE_FOLDER) $(BUILD_PATH)/$(DEBUG_FOLDER)
	mkdir $(SRC_PATH) $(SRC_PATH)/$(HEADER_FOLDER) $(SRC_PATH)/$(SRC_FOLDER)
	touch $(SRC_PATH)/$(MAIN).$(SRC_EXT)
	echo $$'#include <stdio.h>\n\nint main()\n{\n    printf("Hello world !\\n");\n    return 0;\n}' > $(SRC_PATH)/$(MAIN).$(SRC_EXT)