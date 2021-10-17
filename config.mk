# $1 : List
# Returns all uniq elements of '$1' in the order they first appear
uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))
# $1, $2 : lists
# Returns '$(join $1,$2)' for as many elements are in $1 and $2
# Returns empty if either are empty
2minjoin = $(filter-out $2,$(filter-out $1,$(join $1,$2)))
# $1 : Index
# $2 : List
# Returns all words before index '$1' in '$2'
before = $(wordlist 1,$1,$2)
# $1 : Index
# $2 : List
# Returns all words at and after index '$1' in '$2'
after = $(wordlist $1,$(words $2),$2)
# $1 : Map function
# $2, $3 : Map lists
# $4 : Argument to pass down to all iterations
# Returns '$(call $1,$i,$j)' for each i,j pair in '$2' and '$3'
2map = $(if $(call 2minjoin,$2,$3),$(call $1,$(firstword $2),$(firstword $3),$4) \
             $(call 2map,$1,$(call after,2,$2),$(call after,2,$3),$4))

CURDIR:=$(PWD)
PROJECT:=$(notdir $(CURDIR))

$(PROJECT)_SRCDIR:=src

$(PROJECT)_SRC:=\
	main.c\
	levenshtein.c\
	list.c\
	matrix.c\

$(PROJECT)_HDR:=\
	levenshtein.h\
	list.h\
	matrix.h\
	types.h\

aio_names = OBJ ASS INT
aio_extensions = o s e
gen_aios = $(eval $1=$(3:.c=$2))
$(call 2map,gen_aios,$(addprefix $(PROJECT)_,$(aio_names)),\
	$(addprefix .,$(aio_extensions)),\
	$(addprefix $($(PROJECT)_SRCDIR)/,$($(PROJECT)_SRC)))

ifndef prefix
 prefix:=/usr/local
endif

CC:=gcc
ifndef DEBUG
 OPT:=-O3 -Ofast
else
 OPT:=-O0 -g
 ifndef MEMCHECK
  PRF:=-pg -no-pie
 endif
endif

$(PROJECT)_CFLAGS = -std=c11 $(OPT) $(PRF) $(CFLAGS)
$(PROJECT)_CPPFLAGS = $(STCPPFLAGS) $(CPPFLAGS)
$(PROJECT)_LDFLAGS = $(PRF) $(LDFLAGS)
