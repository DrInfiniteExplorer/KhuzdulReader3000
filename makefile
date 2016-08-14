ThisPath := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

sources := $(wildcard $(ThisPath)/src/*.cpp)
objects := $(addprefix ./tmp/,$(subst $(ThisPath)/,,$(sources:.cpp=.o)))

libs:=\
  -static -lstdc++ \

flags:=\
  -std=c++1y \
  -gdwarf-2 \
  -MMD \
  -I$(ThisPath)/include \

all:

$(objects):$(ThisPath)/makefile
-include $(objects:.o=.d)

./tmp/%.o : $(ThisPath)/%.cpp
	mkdir -p $(dir $@)
	gcc $(flags) -c -o $@ $<

./out:
	mkdir ./out
   
./out/program.exe: $(objects) | ./out
	gcc -g -o $@ $^ $(libs)

./out/split-program.exe: ./out/program.exe
	cp $< $@
	objcopy --only-keep-debug $@ $@.debug
	strip -g $@
	objcopy --add-gnu-debuglink=$@.debug $@
   
all: ./out/program.exe ./out/split-program.exe 

test: all
	./out\program.exe
   
