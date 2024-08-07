NOBENCH_PATH = ../..
WORKDIR = $(shell pwd)
FREERTOS_SRC_DIR =$(WORKDIR)/FreeRTOS/Source

MARCH = rv32imah_zicsr
BASE_ADDRESS ?= 0x80000000
STACK_SIZE=300

OBJDIR = bld

ifeq ($(NOBENCH_TRACING), 2)
CFLAGS += -finstrument-functions \
		  -finstrument-functions-exclude-file-list=portASM,tests,tracing \
		  -finstrument-functions-exclude-file-list=printf,rpi_aux,rpi_base,rpi_gpio,rpi_i2c,rpi_systimer,startup \
		  -finstrument-functions-exclude-function-list=trace_switch_in,trace_switch_out,no_cycle_get_count,no_serial_write,rpi_aux_mu_string,sprintf \
		  -I $(NOBENCH_PATH)/tracing \
		  -DTRACING
VPATH += $(NOBENCH_PATH)/tracing:
TRACING_OBJS = $(OBJDIR)/cygprofile.o
else
ifeq ($(NOBENCH_TRACING), 1)
CFLAGS += -I $(NOBENCH_PATH)/tracing \
		  -DTRACING
VPATH += $(NOBENCH_PATH)/tracing:
TRACING_OBJS = $(OBJDIR)/trace.o
else
TRACING_OBJS =
endif
endif


CFLAGS += -nostdlib -nostartfiles -ffreestanding \
		  -I $(WORKDIR)/include \
		  -I $(FREERTOS_SRC_DIR)/include \
		  -I $(FREERTOS_SRC_DIR)/portable/GCC/RISC-V \
		  -I $(FREERTOS_SRC_DIR)/portable/GCC/RISC-V/chip_specific_extensions/RV32I_CLINT_no_extensions \
		  -I $(FREERTOS_SRC_DIR)/../Demo/RISC-V-spike-htif-GCC \
		  -I $(NOBENCH_PATH)/include \
		  -D RPI2 \
		  -D NB_ITER=$(NO_NB_ITER) \
		  -MD \
		  -O2 -g \
		  -march=$(MARCH) \
		  -mabi=ilp32

ALFLAGS += -I $(FREERTOS_SRC_DIR)/portable/GCC/RISC-V/chip_specific_extensions/RV32I_CLINT_no_extensions \
	-I $(WORKDIR)/include \
	-march=$(MARCH)
LDFLAGS = -nostartfiles \
		-Xlinker --defsym=__stack_size=$(STACK_SIZE)

VPATH +=$(FREERTOS_SRC_DIR):
VPATH +=$(FREERTOS_SRC_DIR)/portable/GCC/RISC-V:
VPATH +=$(FREERTOS_SRC_DIR)/portable/MemMang:
VPATH +=$(FREERTOS_SRC_DIR)/../Demo/RISC_V:
VPATH +=$(FREERTOS_SRC_DIR)/../Demo/CORTEX_A7_BCM2836/Drivers:

NOBENCH_OBJS = $(NO_TEST_SRC:.c=.o)
NOBENCH_TARGET = $(NO_TEST_SRC:.c=.elf)

OBJECTS = $(OBJDIR)/list.o \
	  $(OBJDIR)/queue.o \
	  $(OBJDIR)/tasks.o \
	  $(OBJDIR)/heap_4.o \
	  $(OBJDIR)/event_groups.o \
	  $(OBJDIR)/port.o  \
	  $(OBJDIR)/portASM.o  \
	  $(OBJDIR)/freertos_porting_layer.o \
	  $(OBJDIR)/$(NOBENCH_OBJS) \
	  $(TRACING_OBJS)
	  #$(OBJDIR)/portISR.o  
	  #$(OBJDIR)/mmu.o \
#	  $(OBJDIR)/htif.o 
#	  $(OBJDIR)/riscv-virt.o 
#	  $(OBJDIR)/start.o 

#RPI_DRIVER_OBJS = $(OBJDIR)/rpi_aux.o \
#				  $(OBJDIR)/rpi_gpio.o \
#				  $(OBJDIR)/rpi_i2c.o \
#				  $(OBJDIR)/rpi_irq.o \
#				  $(OBJDIR)/rpi_systimer.o \
#				  $(OBJDIR)/printf.o \

CC = riscv32-unknown-elf-gcc
AS = riscv32-unknown-elf-as
AR = riscv32-unknown-elf-ar
OBJCOPY = riscv32-unknown-elf-objcopy
LD = riscv32-unknown-elf-ld
CPP = riscv32-unknown-elf-cpp

LIBGCC = $(abspath $(shell $(CC) -print-libgcc-file-name))
LIBC = $(abspath $(shell $(CC) -print-file-name=libc.a))

TARGETS = $(NO_TEST_SRC:.c=)

all: test $(OBJDIR) $(TARGETS)

test:
	echo "$(OBJECTS)"

$(TARGETS): % : $(OBJECTS) $(OBJDIR)/fake_rom$(BASE_ADDRESS).lds
	$(CC) $(LDFLAGS) -T$(OBJDIR)/fake_rom$(BASE_ADDRESS).lds -o $(OBJDIR)/$@ $(OBJECTS) $(LIBS)

#bld/htif.o: include/htif.c | $(OBJDIR)
#	$(CC) -c Dgcc $< $(CFLAGS) -o $@
#bld/riscv-virt.o: include/riscv-virt.c | $(OBJDIR)
#	$(CC) -c Dgcc $< $(CFLAGS) -o $@

# Build fake_rom
# Run lds through the C preprocessor, to replace BASE_ADDRESS with the actual
# value. It might be simpler to use sed instead.
# removed $(CPPFLAGS)
$(OBJDIR)/%$(BASE_ADDRESS).lds: fake_rom.lds Makefile
	$(CPP) -DBASE_ADDRESS=$(BASE_ADDRESS) $< | grep -v '^#' > $@

include $(NOBENCH_PATH)/mk/common.mk
