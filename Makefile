#############################################################################
# Makefile for building: bin/nobleNote
# Generated by qmake (2.01a) (Qt 4.8.1) on: Tue May 8 23:01:01 2012
# Project:  nobelNote.pro
# Template: app
# Command: /usr/bin/qmake -o Makefile nobelNote.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_WEBKIT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++-64 -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4 -I. -Isrc -Ibuild -Ibuild
LINK          = g++
LFLAGS        = -m64 -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu -lQtGui -lQtCore -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = build/

####### Files

SOURCES       = src/main.cpp \
		src/mainwindow.cpp \
		src/note.cpp \
		src/filesystemmodel.cpp build/moc_mainwindow.cpp \
		build/moc_note.cpp \
		build/moc_filesystemmodel.cpp
OBJECTS       = build/main.o \
		build/mainwindow.o \
		build/note.o \
		build/filesystemmodel.o \
		build/moc_mainwindow.o \
		build/moc_note.o \
		build/moc_filesystemmodel.o
DIST          = /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		nobelNote.pro
QMAKE_TARGET  = nobleNote
DESTDIR       = bin/
TARGET        = bin/nobleNote

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET): build/ui_note.h build/ui_mainwindow.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) bin/ || $(MKDIR) bin/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: nobelNote.pro  /usr/share/qt4/mkspecs/linux-g++-64/qmake.conf /usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/common/gcc-base.conf \
		/usr/share/qt4/mkspecs/common/gcc-base-unix.conf \
		/usr/share/qt4/mkspecs/common/g++-base.conf \
		/usr/share/qt4/mkspecs/common/g++-unix.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/lib/x86_64-linux-gnu/libQtGui.prl \
		/usr/lib/x86_64-linux-gnu/libQtCore.prl
	$(QMAKE) -o Makefile nobelNote.pro
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/common/gcc-base.conf:
/usr/share/qt4/mkspecs/common/gcc-base-unix.conf:
/usr/share/qt4/mkspecs/common/g++-base.conf:
/usr/share/qt4/mkspecs/common/g++-unix.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/modules/qt_webkit_version.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/unix/gdb_dwarf_index.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/lib/x86_64-linux-gnu/libQtGui.prl:
/usr/lib/x86_64-linux-gnu/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile nobelNote.pro

dist: 
	@$(CHK_DIR_EXISTS) build/nobleNote1.0.0 || $(MKDIR) build/nobleNote1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/nobleNote1.0.0/ && $(COPY_FILE) --parents src/mainwindow.h src/note.h src/filesystemmodel.h build/nobleNote1.0.0/ && $(COPY_FILE) --parents src/main.cpp src/mainwindow.cpp src/note.cpp src/filesystemmodel.cpp build/nobleNote1.0.0/ && $(COPY_FILE) --parents src/ui/note.ui src/ui/mainwindow.ui build/nobleNote1.0.0/ && (cd `dirname build/nobleNote1.0.0` && $(TAR) nobleNote1.0.0.tar nobleNote1.0.0 && $(COMPRESS) nobleNote1.0.0.tar) && $(MOVE) `dirname build/nobleNote1.0.0`/nobleNote1.0.0.tar.gz . && $(DEL_FILE) -r build/nobleNote1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: build/moc_mainwindow.cpp build/moc_note.cpp build/moc_filesystemmodel.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) build/moc_mainwindow.cpp build/moc_note.cpp build/moc_filesystemmodel.cpp
build/moc_mainwindow.cpp: build/ui_mainwindow.h \
		src/mainwindow.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/mainwindow.h -o build/moc_mainwindow.cpp

build/moc_note.cpp: build/ui_note.h \
		src/note.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/note.h -o build/moc_note.cpp

build/moc_filesystemmodel.cpp: src/filesystemmodel.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) src/filesystemmodel.h -o build/moc_filesystemmodel.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: build/qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) build/qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all: build/ui_note.h build/ui_mainwindow.h
compiler_uic_clean:
	-$(DEL_FILE) build/ui_note.h build/ui_mainwindow.h
build/ui_note.h: src/ui/note.ui
	/usr/bin/uic-qt4 src/ui/note.ui -o build/ui_note.h

build/ui_mainwindow.h: src/ui/mainwindow.ui
	/usr/bin/uic-qt4 src/ui/mainwindow.ui -o build/ui_mainwindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

build/main.o: src/main.cpp src/mainwindow.h \
		build/ui_mainwindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/main.o src/main.cpp

build/mainwindow.o: src/mainwindow.cpp src/mainwindow.h \
		build/ui_mainwindow.h \
		src/note.h \
		build/ui_note.h \
		src/filesystemmodel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/mainwindow.o src/mainwindow.cpp

build/note.o: src/note.cpp src/note.h \
		build/ui_note.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/note.o src/note.cpp

build/filesystemmodel.o: src/filesystemmodel.cpp src/filesystemmodel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/filesystemmodel.o src/filesystemmodel.cpp

build/moc_mainwindow.o: build/moc_mainwindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_mainwindow.o build/moc_mainwindow.cpp

build/moc_note.o: build/moc_note.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_note.o build/moc_note.cpp

build/moc_filesystemmodel.o: build/moc_filesystemmodel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_filesystemmodel.o build/moc_filesystemmodel.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

