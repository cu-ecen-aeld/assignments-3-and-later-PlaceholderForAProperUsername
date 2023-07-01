#include <stdio.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
	openlog(NULL, 0, LOG_USER);
	FILE *fp;
	
	if (argc != 3)
	{
		syslog(LOG_ERR, "Invalid Number of arguments: %d", argc);
		return 1;
	}
	
	if ((fp = fopen(argv[1], "w")) == NULL)
	{
		syslog(LOG_ERR, "Can't open file for writin");
		return 1;
	}
	
	syslog(LOG_DEBUG, "Writing %s to %s", argv[2], argv[1]);
	fprintf(fp, "%s", argv[2]);
	
	fclose(fp);
	
	return 0;
}
