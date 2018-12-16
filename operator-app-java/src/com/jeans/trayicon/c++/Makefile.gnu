# Usage:
# This makefile is for compiling TrayIcon.dll with GCC/Cygwin
# $ make -f Makefile.gnu
# ... making
# $ make -f Makefile.gnu clean
# ... removing objs and precompiled headers

# Use this for Sun's JDK 1.1.x
# JAVA=/cygdrive/d/Java/jdk1.1.8
# JLIB=$(JAVA)/lib/javai.lib
# JVER=11
# JDEF="HAS_OLD_JVM_LIB"

# Use this for Sun's Java 1.2.x and above
JAVA="/cygdrive/c/Program Files/Lang/Java2"
# JAVA="/cygdrive/g/j2sdk1.4.1_01"
#JLIB=$(JAVA)/lib/jvm.lib
JVER=12
#JDEF="HAS_JVM_LIB"
JDEF="NOTHING"

JAWT=$(JAVA)/lib/jawt.lib

CFLAGS= -I$(JAVA)/include -I$(JAVA)/include/win32 -D$(JDEF) -DCYGWIN -mno-cygwin

all:	TrayIcon$(JVER).dll HookDLL.dll

clean:
	rm *.obj; rm *.dll
	
install:	TrayIcon$(JVER).dll
	strip *.dll; mv *.dll ../../../../; rm *.obj *.lib *.a *.exp 2>/dev/null; true
	

OBJS=WindowsTrayIcon.obj

TrayIcon$(JVER).dll:	$(OBJS)
			g++ $(CFLAGS) -shared -Wl,-add-stdcall-alias $(OBJS) -lgdi32 -o TrayIcon$(JVER).dll

WindowsTrayIcon.obj:	WindowsTrayIcon.cpp
			g++ $(CFLAGS) -c WindowsTrayIcon.cpp -o WindowsTrayIcon.obj

# HookDLL.dll:	HookDLL.cpp
#		g++ $(CFLAGS) -shared -Wl,-add-stdcall-alias HookDLL.cpp -o HookDLL.dll
