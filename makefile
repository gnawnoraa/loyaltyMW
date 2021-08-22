# ========================================================
# makefile -- makefile of loyaltyMW_boost
#       Written by: Ggg
#       Date      : 2021-08-08
# ========================================================

# path of lib
BOOST_INCLUDE_PATH = /usr/local/boost/include

# path of source file(for makefile)
VPATH = $(BOOST_INCLUDE_PATH)

# variables
CC            = g++
C11_FLAGS     = --std=c++11
BOOST_FLAGS   = -lboost_system
LD_FLAGS      = -pthread
CXX_FLAGS     = -Wall -g -I$(BOOST_INCLUDE_PATH)
#-----------------------------------------------------------
	# -Wall: asks for all warnings
	# -g: asks for debugging information
	# -I$(PATH): adds an include directory to the include path
	# --std=c++11: for c++11
#-----------------------------------------------------------
INC           = include
OBJ           = obj/async_server.o obj/server.o obj/client.o
TARGET        = bin/async_server bin/server bin/client

# execute file
all: $(TARGET)

bin/async_server: obj/async_server.o
	$(CC) $(CXX_FLAGS) $(LD_FLAGS) -o $@ $^

bin/server: obj/server.o
	$(CC) $(CXX_FLAGS) $(LD_FLAGS) -o $@ $^

bin/client: obj/client.o
	$(CC) $(CXX_FLAGS) $(LD_FLAGS) -o $@ $^

#.o有特定資料夾，-o $@就需要，若和makefile相同資料夾則可省略
obj/%.o: src/%.cpp $(INC)
	$(CC) $(CXX_FLAGS) -c $< -o $@

# clean
.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
