.POSIX:
CC := clang

CFLAGS := -std=c11 -ggdb -Wall -Wextra -pedantic

X11CPPFLAGS := `pkg-config --cflags x11`
X11LDFLAGS  := `pkg-config --libs x11`
XFTCPPFLAGS := `pkg-config --cflags xft`
XFTLDFLAGS  := `pkg-config --libs xft`
FONTCONFIGLDFLAGS := `pkg-config --libs fontconfig`

CPPFLAGS := ${X11CPPFLAGS} ${XFTCPPFLAGS}
LDFLAGS  := ${X11LDFLAGS}  ${XFTLDFLAGS} ${FONTCONFIGLDFLAGS}

BIN := prog

SRCS := main.c
OBJS := ${SRCS:c=o}

${BIN}: ${OBJS}
	${CC} -o $@ ${LDFLAGS} ${OBJS}

${OBJS}: ${SRCS}
	${CC} ${CFLAGS} ${CPPFLAGS} -c $<

clean:
	rm -f ${BIN} *.o *.d *.core

.PHONY: clean
