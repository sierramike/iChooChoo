#pragma once

#define DEBUG

#ifndef _BSD_SOURCE
#define _BSD_SOURCE // Needed for usleep
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>  // Needed for O_RDWR
#include <unistd.h> // Needed for usleep
#include <string.h>

#define PROG_VERSION_MAJ 0
#define PROG_VERSION_MIN 30
#define PROG_VERSION_BLD 0

// Data size if Packet size - 4
#define PACKETSIZE 20
#define DATASIZE 16
#define DESCSIZE 14

// Global Structs
struct ModuleIdent
{
	int InfoPresent;
	int Major;
	int Minor;
	int Build;
	int Address;
	int Type;
	char Description[DESCSIZE + 1];
};

