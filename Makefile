# Makefile

# compiler options
CXX			= g++
CXXFLAGS	= -pipe -Wall -std=c++11 -O2 -mtune=native # -g -ggdb
LDFLAGS		= -s
SRC_NAME	= main my_alg qmm
LIBS		=
EXECUTABLE	= mask_calc
SRC_EXT		= cpp


# targets
all: $(EXECUTABLE)

$(EXECUTABLE): $(patsubst %,%.o,$(SRC_NAME))
	@echo
	$(CXX) $(LDFLAGS) $(patsubst %,%.o,$(SRC_NAME)) -o $(EXECUTABLE)
	@echo

$(patsubst %,%.o,$(SRC_NAME)): %.o : %.$(SRC_EXT)
	@echo
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo
	rm -f $(patsubst %,./%.o,$(SRC_NAME)) ./$(EXECUTABLE)
	@echo
