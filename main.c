#include "header.h"
#include "settings.h"
#include "main.h"
#include "daemon.h"
#include "modconf.h"
#include "biccp.h"
#include <iostream>

int main(int argc, char* argv[])
{
	conf = new ConfManager();
	if (conf->ReadConf(CONFPATH))
	{
		int iReturnCode = ProcessArguments(argc, argv);
		if (iReturnCode == -1)
		{
			PrintVersion();
			PrintHelp(argv[0]);
			return EXIT_SUCCESS;
		}
		else if (iReturnCode == 0)
			return 0;
		else
			return EXIT_SUCCESS;
	}
	else
	{
		printf("Error detected, exiting...\n");
		return 0;
	}
}

int ProcessArguments(int argc, char* argv[])
{
	int iReturn = -1; // -1=Nothing done; 0=Failed; 1=Success

	bool bVersion = false;
	bool bHelp = false;
	bool bScan = false;
	bool bDumpConfig = false;
	bool bDaemon = false;
	uint8_t iSoftReset = 0;
	uint8_t iHardReset = 0;
	uint8_t iGetIdent = 0;
	uint8_t iSetAddr = 0;
	uint8_t iSetType = 0;
	uint8_t iSetDesc = 0;
	char*   sSetDesc = 0;

	if (argc > 0)
	{
		int i = 1;
		int l = 0;
		while(i < argc)
		{
			if (strcmp(argv[i], "--version") == 0)
			{
				bVersion = true;
			}
			else if (strcmp(argv[i], "--help") == 0)
			{
				bVersion = true;
				bHelp = true;
			}
			else if (strcmp(argv[i], "--scan") == 0)
			{
				bScan = true;
			}
			else if (strcmp(argv[i], "--dumpconfig") == 0)
			{
				bDumpConfig = true;
			}
			else if (strcmp(argv[i], "--daemon") == 0)
			{
				bDaemon = true;
			}
			else if (l = CheckArgumentGetByte(argc, argv, i, (char*)"--ident", 1, &iGetIdent))
			{
				if (l > 0) i++;
				else return 0;
			}
			else if (l = CheckArgumentGetByte(argc, argv, i, (char*)"--setaddr", 1, &iSetAddr))
			{
				if (l > 0) i++;
				else return 0;
			}
			else if (l = CheckArgumentGetByte(argc, argv, i, (char*)"--settype", 1, &iSetType))
			{
				if (l > 0) i++;
				else return 0;
			}
			else if (l = CheckArgumentGetByte(argc, argv, i, (char*)"--setdesc", 2, &iSetDesc))
			{
				if (l > 0)
				{
					sSetDesc = argv[i + 2];
					if (strlen(sSetDesc) > DESCSIZE)
					{
						printf("Description too long in argument passed to %s %s %s\n", argv[i], argv[i + 1], argv[i + 2]);
						return 0;
					}
					i += 2;
				}
				else return 0;
			}
			else if (l = CheckArgumentGetByte(argc, argv, i, (char*)"--softreset", 1, &iSoftReset))
			{
				if (l > 0) i++;
				else return 0;
			}
			else if (l = CheckArgumentGetByte(argc, argv, i, (char*)"--hardreset", 1, &iHardReset))
			{
				if (l > 0) i++;
				else return 0;
			}
			else
			{
				printf("Unknown argument %s\n", argv[i]);
				return 0;
			}
			i++;
		}
	}

	if (bVersion && argc > 2)
	{
		printf("Argument --version should be used alone.\n");
		return 0;
	}
	if (bHelp && argc > 2)
	{
		printf("Argument --help should be used alone.\n");
		return 0;
	}
	if (bScan && argc > 2)
	{
		printf("Argument --scan should be used alone.\n");
		return 0;
	}
	if (bDumpConfig && argc > 2)
	{
		printf("Argument --dumpconfig should be used alone.\n");
		return 0;
	}
	if (bDaemon && argc > 2)
	{
		printf("Argument --daemon should be used alone.\n");
		return 0;
	}

	if (bVersion)
	{
		PrintVersion();
		iReturn = 1;
	}

	if (bHelp)
	{
		PrintHelp(argv[0]);
		iReturn = 1;
	}

	if (bScan)
	{
		struct ModuleIdent* list;
		int iNbModules = ScanBus(&list);
		PrintModuleList(list, iNbModules);
		free(list);
		iReturn = 1;
	}

	if (bDumpConfig)
	{
		conf->Display(std::cout);
		iReturn = 1;
	}

	if (bDaemon)
	{
		return start_daemon();
	}

	if (iGetIdent)
	{
		struct ModuleIdent mi = ModConfGetIdent(iGetIdent);
		if (mi.InfoPresent)
		{
			printf("\n");
			printf("    Version     : %d.%d.%d\n", mi.Major, mi.Minor, mi.Build);
			printf("    Address     : %#02x\n", mi.Address);
			printf("    Type        : %#02x\n", mi.Type);
			printf("    Description : %s\n", mi.Description);
			printf("\n");
		}
		iReturn = 1;
	}
	if (iSetAddr)
	{
		ModConfSetAddress(0x77, iSetAddr);
		iReturn = 1;
	}
	if (iSetType)
	{
		ModConfSetType(0x77, iSetType);
		iReturn = 1;
	}
	if (iSetDesc)
	{
		ModConfSetDescription(iSetDesc, sSetDesc);
		iReturn = 1;
	}
	if (iSoftReset)
	{
		ModConfSoftReset(iSoftReset);
		iReturn = 1;
	}
	if (iHardReset)
	{
		ModConfHardReset(iHardReset);
		iReturn = 1;
	}

	return iReturn;
}

/////////////////////////////////////////////////
// Checks a CLI argument
// argc    : number of CLI arguments
// argv    : argument list
// argi    : index to check
// argname : name of argument to check
// nparam  : number of parameters required
/////////////////////////////////////////////////
int CheckArgument(int argc, char* argv[], int argi, char* argname, int nparam)
{
	if (strcmp(argv[argi], argname) == 0)
	{
		if (argc < argi + nparam + 1)
		{
			printf("Argument %s needs %d parameter(s), please use %s --help\n", argname, nparam, argv[0]);
			return -1;
		}
		for (int i = 1; i <= nparam; i++)
		{
			if (strlen(argv[argi + i]) > 2)
				if (argv[argi + i][0] == '-' && argv[argi + i][1] == '-')
				{
					printf("Argument %s needs %d parameter(s), please use %s --help\n", argname, nparam, argv[0]);
					return -1;
				}
		}
		return 1;
	}
	else
		return 0;
}

int CheckArgumentGetByte(int argc, char* argv[], int argi, char* argname, int nparam, uint8_t* data)
{
	int iReturn = CheckArgument(argc, argv, argi, argname, nparam);
	if (iReturn > 0)
	{
		*data = (uint8_t)strtol(argv[argi + 1], NULL, 0);
		if (*data == 0)
		{
			printf("Invalid argument passed to %s %s\n", argv[argi], argv[argi + 1]);
			return -1;
		}
	}
	return iReturn;
}

void PrintModuleList(struct ModuleIdent* list, int iNbModules)
{
	printf("\nBus scan complete: %d modules found.\n\n", iNbModules);
	printf("+---------+----------+------+----------------+\n");
	printf("| Address | Version  | Type | Description    |\n");
	printf("+---------+----------+------+----------------+\n");
	for (int i = 0; i < iNbModules; i++)
	{
		char buf[9] = "        ";
		if (list[i].InfoPresent)
		{
			char line[] = "| 0x%02x    |          | 0x%02x |                |\n";
			sprintf(buf, "%d.%d.%d", list[i].Major, list[i].Minor, list[i].Build);
			memcpy(line + 14, buf, strlen(buf));
			if (list[i].Address == 0x77)
				memcpy(line + 34, "(new module)", 12);
			else
				memcpy(line + 34, list[i].Description, strlen(list[i].Description));
			printf(line, list[i].Address, list[i].Type);
		}
		else
		{
			printf("| 0x%02x    |    --    |  --  | (unidentified) |\n", list[i].Address);
		}
	}
	printf("+---------+----------+------+----------------+\n");
}

int PrintVersion()
{
	printf("iChooChoo version %d.%d.%d\n", PROG_VERSION_MAJ, PROG_VERSION_MIN, PROG_VERSION_BLD);
	return -1;
}

int PrintHelp(char* executableName)
{
	printf("\n");
	printf("    Usage: %s [options]\n", executableName);
	printf("\n");
	printf("  --version             Print version info\n");
	printf("  --help                Print this help text\n");
	printf("  --scan                Scans the i2c bus and reports the list of modules detected\n");
	printf("  --daemon              Starts the daemon mode\n");
	printf("  --dumpconfig          Dumps the configuration file to the screen (throws an error if the configuration file is wrong\n");
	printf("  --ident 0xHH          Print module identification on 0xHH address\n");
	printf("  --setaddr 0xHH        Set new module* address to 0xHH\n");
	printf("  --settype 0xHH        Set new module* type to 0xHH\n");
	printf("  --setdesc 0xHH desc   Set description desc to 0xHH address\n");
	printf("  --softreset 0xHH      Sends a soft reset signal to 0xHH address\n");
	printf("  --hardreset 0xHH      Sends a hard reset** signal to 0xHH address\n");
	printf("\n");
	printf("*  new module is only on address 0x77.\n");
	printf("** be sure that no other new module is connected on the bus before using hard reset.\n");
	return -1;
}
