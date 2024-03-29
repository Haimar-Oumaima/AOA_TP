CC=gcc
CFLAGS=-O2 -g -Wall
OPTFLAGS=-O3 -g -Wall
OBJS_COMMON=kernel.o rdtsc.o

all:	check calibrate measure

check:	$(OBJS_COMMON) driver_check.o
	$(CC) -o $@ $^
calibrate: $(OBJS_COMMON) driver3.o
	$(CC) -o $@ $^
measure: $(OBJS_COMMON) driver.o
	$(CC) -o $@ $^

driver_check.o: driver_check.c
	$(CC) $(CFLAGS) -D CHECK -c $< -o $@
driver_calib.o: driver3.c
	$(CC) $(CFLAGS) -D CALIB -c $< -o $@
driver.o: driver.c
	$(CC) $(CFLAGS) -c $<

kernel.o: baselinekernel.c
	$(CC) $(OPTFLAGS) -D $(OPT) -c $< -o $@

clean:
	rm -rf $(OBJS_COMMON) driver_check.o driver_calib.o driver.o check calibrate measure


#exécutant "make OPT=OPT1" (pour la version OPT1 du noyau,  etc. pour OPT2). Sans définir une variable d’environnement OPT, ça ne compilait pas.