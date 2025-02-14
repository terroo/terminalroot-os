TARGET=terminal-root-os.bin
GNU=as
CXX=g++
DEBUG=-g
OPT=-O1
WARN=-Wall -Werror
LD=ld
CXXFLAGS = $(DEBUG) $(OPT) $(WARN)
CXXFLAGS += -w -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASFLAGS = --32
LDFLAGS = -melf_i386

objs = loader.o kernel.o main.o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

%.o: %.s
	$(GNU) $(ASFLAGS) -o $@ $<

$(TARGET): linker.ld $(objs)
	$(LD) $(LDFLAGS) -T $< -o $@ $(objs)

install: $(TARGET)
	sudo cp $< /boot/$(TARGET)

clean:
	@rm -rf *.o $(TARGET)
