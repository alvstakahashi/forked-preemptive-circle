#
# Makefile
#

CIRCLEHOME = ../..

#ssp include
INCSSP = -I./ssp -I./include -I./arch/RPI
CFLAGS   += $(INCSSP)

OBJS	= main.o kernel.o mykernel.o

#ssp liblary
OBJS+= ssp/kernel_cfg.o ssp/startup.o ssp/task.o ssp/task_manage.o

LIBS	= $(CIRCLEHOME)/lib/libcircle.a

include ../Rules.mk

-include $(DEPS)
