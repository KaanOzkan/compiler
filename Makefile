$(shell clang-format -i src/*.cpp)
CC = g++

EXEC ?= run.out
SRC_DIRS ?= ./src

SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJECTS := $(addsuffix .o,$(basename $(SRCS)))
DEPS := $(OBJECTS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS = $(CUSTOM) $(INC_FLAGS) -MMD -MP -g -Wall -Wextra -Werror -std=c++11

$(EXEC): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LOADLIBES) $(LDLIBS)

.PHONY: clean test run
clean:
	$(RM) $(EXEC) $(OBJECTS) $(DEPS)

test: $(EXEC)
	./test_runner.sh $(filter-out $@,$(MAKECMDGOALS))

run: $(EXEC)
	./$(EXEC) $(filter-out $@,$(MAKECMDGOALS))

%:
	@:

-include $(DEPS)
