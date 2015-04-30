CXX=g++
C=gcc
CFLAGS=-Wall -g
CXXFLAGS=$(CFLAGS)
LFLAGS_LLAPI=-lqpidclient -lqpidtypes
LFLAGS_HLAPI=-lqpidmessaging -lqpidtypes -lboost_regex
OPT=2

# -r ~ --no-buildin-rules -- disable built in Makefile rules that breaks our own
MAKEFLAGS=-r

# those become binaries
targetsrc=qc2_connector.cpp qc2_spout.cpp qc2_drain.cpp qmf2_list_objects.cpp qc2_map_sender.cpp qc2_map_receiver.cpp qc2_server_example.cpp qc2_client_example.cpp qmf2_qpid_ctrl.cpp
# those become shared objects
objsrc=OptionParser.cpp BrokerAgent.cpp Formatter.cpp Utils.cpp

# internal name generation
targets=$(targetsrc:.cpp=)
targetsbins=$(targetsrc:.cpp=.out)

objhdr=$(objsrc.cpp=.h)
objs=$(objsrc:.cpp=.o)
objsall=$(objs) $(targetsrc:.cpp=.o)

# ---------------------------------------------------------------- #
### RULES

all: $(targets) Makefile

# compilint all .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# implicit rule for compiling binaries to .out form
$(targets): $(objsall)
	$(CXX) $(CXXFLAGS) -O$(OPT) $(LFLAGS_HLAPI) $@.o $(objs) -o $@

# making standard named binaries from .out
#$(targets): $(targetsbins)
#	cp $(@:=.out) $(@)



clean:
	$(RM) $(objsall) $(targetsbins) $(targets)



# ingnoring existence of files 'all' 'clean' taking those as pure rules
.PHONY: all clean
# preventing make to delete objects, speeds up recompilation
.PRECIOUS: %.o
# disabling default rule
.DEFAULT:

