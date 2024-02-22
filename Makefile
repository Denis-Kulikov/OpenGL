APP_NAME = OpenGL
LIB_NAME = Lib
#TEST_NAME = Test_${APP_NAME}

CC = g++
CFLAGS = -c -Wall -Wextra -Werror
CPPFLAGS = -I src -MP -MMD
LDLIBS = -lglut -lGL  -lGLU -lGLEW
#CFLAGS_TEST = -I test -MMD

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src
#TEST_DIR = test

APP_PATH = $(BIN_DIR)/$(APP_NAME)
#TEST_PATH = $(BIN_DIR)/$(TEST_NAME)
#TEST_OBJ_PATH = $(OBJ_DIR)/$(TEST_DIR)

SRC_EXT = cpp
APP_RUN = $(BIN_DIR)/./$(APP_NAME)
#TEST_CHECK = $(BIN_DIR)/./$(TEST_NAME)

APP_SOURCES = $(shell find $(SRC_DIR)/$(APP_NAME) -name '*.$(SRC_EXT)')
APP_OBJECTS = $(APP_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/%.o)

LIB_SOURCES = $(shell find $(SRC_DIR)/$(LIB_NAME) -name '*.$(SRC_EXT)')
LIB_OBJECTS = $(LIB_SOURCES:$(SRC_DIR)/%.$(SRC_EXT)=$(OBJ_DIR)/%.o)

DEPS = $(APP_OBJECTS:.o=.d) $(LIB_OBJECTS:.o=.d)

.PHONY: all
all: $(APP_PATH)

$(APP_PATH): src/OpenGL/main.cpp src/OpenGL/math_3d.cpp src/OpenGL/pipeline.cpp src/OpenGL/distance.cpp src/OpenGL/n_body.cpp
	$(CC) -o $@ -Wall $^ $(LDLIBS) -lm

#-include $(DEPS)

#$(APP_PATH): $(APP_OBJECTS) $(LIB_PATH)
#	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

#$(LIB_PATH): $(LIB_OBJECTS)
#	ar rcs $@ $^

#$(OBJ_DIR)/%.o: %.${SRC_EXT}
#	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -f $(APP_PATH) $(TEST_PATH) $(LIB_PATH)
	rm -rf $(DEPS) $(APP_OBJECTS) $(LIB_OBJECTS)
	
.PHONY: run
run: $(APP_RUN)
	$(APP_RUN)

#.PHONY: test
#test: $(TEST_PATH)
	
#.PHONY: rtest
#rtest: $(TEST_CHECK)
#	$(TEST_CHECK)
