.POSIX:
.SUFFIXES:

include config.mk

all: $(PROJECT)

%.o: %.c Makefile config.mk
	$(CC) $($(PROJECT)_CFLAGS) $($(PROJECT)_CPPFLAGS) -c $< -o $@

$(PROJECT): $($(PROJECT)_OBJ) Makefile config.mk
	$(CC) -o $(PROJECT) $($(PROJECT)_OBJ) $($(PROJECT)_LDFLAGS)

clean:
	@rm -v $(PROJECT) 2>/dev/null || :
	@rm -v $($(PROJECT)_OBJ) 2>/dev/null || :

again: clean all

install: all
	@cp -v $(PROJECT) $(prefix)/bin/$(PROJECT)
	@ln -svi $(prefix)/bin/$(PROJECT) $(prefix)/bin/strdst
uninstall:
	@rm -v $(prefix)/bin/$(PROJECT) || :
	@rm -v $(prefix)/bin/strdst || :
