### Comment utiliser le Makefile

## Règles

# all: Compile tous les fichiers (règle par défaut)
# indent: Indente les fichiers sources avec indent
# senderUDP, recvUDP, ...: Compile un fichier spécifique

## Variables

# CC: Le compilateur à utiliser pour compiler (gcc par défaut)
# CCFLAGS: Drapeaux supplémentaires à utiliser pour compiler
# DEBUG: Variable booléenne, si 1, conserve les informations de débogage (0 par défaut)

### Variables

CC = gcc

override CCFLAGS += -Wall -Wextra -Wshadow -Werror

DEBUG ?= 0
ifeq ($(DEBUG),1)
	override CCFLAGS += -g
endif

override TARGETS = senderUDP recvUDP serveurTCP clientTCP fileReader\
	fileWriter sendeTransferUDP recvTransferUDP
	
override SOURCES := $(wildcard *.c)

### Règles

all: $(TARGETS)

# Toujours créer si demandé / n'est jamais à jour
.PHONY: all indent clean

indent: $(SOURCES)
	indent $^
	rm *~

clean:
	rm -f *.o $(TARGETS)

# Activer les expansions secondaires
.SECONDEXPANSION:

# L'expansion secondaire permet de rendre le Makefile plus concis.
# $$@ devient le nom de l'un des exécutables, celui choisi
$(TARGETS): $$@.c
	$(CC) $(CCFLAGS) -o $@ $^
