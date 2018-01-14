CXXFLAGS:= -std=c++1y

release: CXXFLAGS+= -O3
release: LDFLAGS+= -s
release: fakemouse
debug: CXXFLAGS+= -ggdb3
debug: fakemouse
sanitize: CXXFLAGS+= -fsanitize=address,undefined
sanitize: fakemouse

.DEFAULT_GOAL := release

clean:
	rm -f fakemouse

.PHONY: clean release debug sanitize
