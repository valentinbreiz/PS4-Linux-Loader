LIBPS4	:=	$(PS4SDK)/libPS4

TEXT	:=	0x926200000
DATA	:=	0x926300000

CC		:=	gcc
AS		:=	gcc
OBJCOPY	:=	objcopy
ODIR	:=	build
SDIR	:=	source
IDIRS	:=	-I$(LIBPS4)/include -I. -Iinclude
LDIRS	:=	-L$(LIBPS4) -L. -Llib 
CFLAGS	:=	$(IDIRS) -O3 -std=gnu11 -fno-builtin -nostartfiles -nostdlib -Wall -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=large -DTEXT_ADDRESS=$(TEXT) -DDATA_ADDRESS=$(DATA)
SFLAGS	:=	-nostartfiles -nostdlib -march=btver2 -mtune=btver2
LFLAGS	:=	$(LDIRS) -Xlinker -T $(LIBPS4)/linker.x -Wl,--build-id=none -Ttext=$(TEXT) -Tdata=$(DATA)
CFILES	:=	$(wildcard $(SDIR)/*.c)
SFILES	:=	$(wildcard $(SDIR)/*.s)
OBJS	:=	$(patsubst $(SDIR)/%.c, $(ODIR)/%_normal.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%_normal.o, $(SFILES))
OBJS_DEBUG	:=	$(patsubst $(SDIR)/%.c, $(ODIR)/%_debug.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%_debug.o, $(SFILES))


DIP_1 = 192
DIP_2 = 168
DIP_3 = 1
DIP_4 = 12

LIBS	:=	-lPS4

TARGET = everything

everything: $(shell basename $(CURDIR))_normal.bin $(shell basename $(CURDIR))_debug.bin


$(shell basename $(CURDIR))_normal.bin: $(ODIR) $(OBJS)
	$(CC) $(LIBPS4)/crt0.s $(ODIR)/*_normal.o -o temp.t $(CFLAGS) $(LFLAGS) $(LIBS)
	$(OBJCOPY) -O binary temp.t $(shell basename $(CURDIR))_normal.bin
	rm -f temp.t

$(shell basename $(CURDIR))_debug.bin: $(ODIR) $(OBJS_DEBUG)
	$(CC) $(LIBPS4)/crt0.s $(ODIR)/*_debug.o -o temp.t $(CFLAGS) $(LFLAGS) $(LIBS)
	$(OBJCOPY) -O binary temp.t $(shell basename $(CURDIR))_debug.bin
	rm -f temp.t



$(ODIR)/%_normal.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%_normal.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR)/%_debug.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS) -D DEBUG_SOCKET=1 -D DIP_1=$(DIP_1) -D DIP_2=$(DIP_2) -D DIP_3=$(DIP_3) -D DIP_4=$(DIP_4)

$(ODIR)/%_debug.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(ODIR)/*.o
