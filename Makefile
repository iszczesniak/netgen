TARGETS = netgen
TARGET_OBJS = $(addsuffix .o, $(TARGETS))

OBJS = cli_args.o gabriel.o mypoint.o teventqueue.o utils.o

CXXFLAGS := $(CXXFLAGS) -g
CXXFLAGS := $(CXXFLAGS) -std=c++14
CXXFLAGS := $(CXXFLAGS) -I include

# Use the C++ linker
LINK.o = $(LINK.cc)

ifdef BOOST_ROOT
	CXXFLAGS := $(CXXFLAGS) -I $(BOOST_ROOT)/include
	LDFLAGS := $(LDFLAGS) -L $(BOOST_ROOT)/lib
endif

LDFLAGS := $(LDFLAGS) -l boost_program_options
LDFLAGS := $(LDFLAGS) -l boost_graph

all: $(TARGETS)

netgen: $(OBJS)

info: $(OBJS)

.PHONY: clean count depend test

clean:
	rm -f *~
	rm -f $(OBJS)
	rm -f $(TARGET_OBJS)
	rm -f $(TARGETS)

count:
	wc -l *.hpp *.cc

depend:
	g++ $(CXXFLAGS) -MM *.cc > dependencies

include dependencies
