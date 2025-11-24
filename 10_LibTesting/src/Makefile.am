CFLAGS = -Wall -Wextra -std=c99

if COND_GCOV
CFLAGS += -O0 -g -fprofile-arcs -ftest-coverage
endif

lib_LTLIBRARIES = libgrow.la
libgrow_la_SOURCES = libgrow.c
