$(shell clang-format -i src/*.cpp)
MAKEFLAGS += --silent
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

.PHONY: clean test run exec
clean:
	$(RM) $(EXEC) $(OBJECTS) $(DEPS)

test: $(EXEC)
	./test_runner.sh $(filter-out $@,$(MAKECMDGOALS))

run:
	@$(MAKE) -s $(EXEC)
	@./$(EXEC) $(filter-out $@,$(MAKECMDGOALS))

exec: $(EXEC)
	@./$(EXEC) $(filter-out $@,$(MAKECMDGOALS)) > output.s
	@as -o output.o output.s
	@ld -o output output.o
	@./output
	@rm -f output output.o output.s

%:
	@:

-include $(DEPS)
