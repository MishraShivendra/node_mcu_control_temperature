SHELL := /bin/bash
CC=g++
CFLAGS=-Wall -std=c++11 -c
BIN_CFLAGS=-Wall -std=c++11
INCLUDES=-I/usr/include/cppconn
LFLAGS=-L/usr/lib -L/usr/local/bin
LIBS=-lboost_system -lboost_thread -lboost_filesystem -pthread -lmysqlcppconn -lcurl
DFLAGS=-DDEBUG -g -v


SRCS=push_pull_app.cpp 
OBJS=$(SRCS:.cpp=.o)
TARGET_DIR=_app_bin
TARGET=push_pull_app
GEN_BIN=bin
GEN_BIN_DEBUG=bin_debug
