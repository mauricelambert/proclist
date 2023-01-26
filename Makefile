COMPILER := gcc
FILE_SRC := proclist
EXE_FILE := $(FILE_SRC)_tests
LIB_FILE := $(FILE_SRC).o
SO_FLAGS := -c --shared -o $(LIB_FILE)
EXE_FLAGS := -Wl,$(LIB_FILE) -O5
OUT_FILES := $(EXE_FILE) $(LIB_FILE)

default: all

all: sharedobject tests

sharedobject:
	$(COMPILER) $(SO_FLAGS) $(FILE_SRC).c
    
tests:
	$(COMPILER) $(EXE_FLAGS) tests.c -o $(EXE_FILE)
	./$(EXE_FILE)
    
clean:
	rm $(OUT_FILES)
