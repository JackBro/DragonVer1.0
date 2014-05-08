

#include "internal.h"

#define printf tmPrintf

#ifndef WIN32
//extern void * malloc(unsigned nbyte);
//extern void free(void *pt);
#endif
//int Help_Command(int, char**);
//extern char argvs[10][100];

#define START_BLOCK 128 /* 2MB */
#define END_BLOCK 1023 /* 16MB */

//void APP_Serial_Update(int handle);

void APP_Disk_Tools(void) 
{//Format only
	int i;
	/* Erase Flash Content */ 
	printf("Now, Formatting....\n");

	DEV_FD_Reset();
	for(i = START_BLOCK; i< END_BLOCK ; i++) 
	{
		if((DEV_FD_Erase_Block(i)) == 1) 
		{
			printf(">%d:OK\n",i);
		}
		else 
		{ 
			printf(">%d:FAIL\n",i);
		}
	}
	printf("Flash has been formatted!\n");
}

void APP_Serial_Update(int handle) 
{
	unsigned int i, size, len;
	unsigned short int checkSum=0, dnCS;	//Check Sum 
	char header[4];
	char *buf;
	char *pbuf;
	
	/* Get Image size from serial port */ 
	printf("Ready for download file!\n@");
	size = 4;
	i = 0;
	while(size)
	{
		len = tmReadBuf(header+i, size);
		size -= len;
		i += len;
	}
	size = header[0] + (header[1]<<8) + (header[2]<<16) + (header[3]<<24) - 4;// - 4 
	printf("Loading Image[%7d bytes]\n",size);
	buf = (char *)vmalloc(size);
	/* Download Image data from serial port to memory */ 
	pbuf = buf;	
	printf("Downloading data from serial port...\n@");
	i = 0;
	while (i < size) 
	{
		len = tmReadBuf(pbuf, size - i);
		i += len;
		pbuf += len;
	}
	printf("\nImage data loaded in memory!\n");
	pbuf = buf;
   	for(i = 0; i < size-2; i++, pbuf++)
		checkSum += (unsigned short)(*pbuf) & 0xff;
   	dnCS=*((char *)(buf+size-2))+(*( (char *)(buf+size-1) )<<8);
	
   	if(checkSum!=dnCS)
    {
		printf("Checksum: @Error!!! MEM:%x DN:%x\n",checkSum,dnCS);
		return;
	}
    printf("Checksum: @OK\n");

	i=0;
	size -= 2;
	printf("Writing file...\n");
	yaffs_write(handle, buf, size);
	vfree(buf);
	printf("File upload:@OK\n");
}

void dumpDir(const char *dname)
{
	yaffs_DIR *d;
	yaffs_dirent *de;
	struct yaffs_stat s;
	char str[100];
			
	d = yaffs_opendir(dname);
	
	if(!d)
	{
		printf("opendir failed\n");
	}
	else
	{
		while((de = yaffs_readdir(d)) != NULL)
		{
			sprintf(str,"%s/%s",dname,de->d_name);
			
			yaffs_lstat(str,&s);
			
			printf("%s length %d mode %X ",de->d_name,s.st_size,s.st_mode);
			switch(s.st_mode & S_IFMT)
			{
				case S_IFREG: printf("data file"); break;
				case S_IFDIR: printf("directory"); break;
				case S_IFLNK: printf("symlink -->");
							  if(yaffs_readlink(str,str,100) < 0)
							  	printf("no alias");
							  else
								printf("\"%s\"",str);	 
							  break;
				default: printf("unknown"); break;
			}
			
			printf("\n");		
		}
		
		yaffs_closedir(d);
	}
	printf("\n");
	
	printf("Free space in %s is %d\n\n",dname,yaffs_freespace(dname));

}

void format_command(int argc, char **argv) 
{
	argc = (int)argv[0][0];
	APP_Disk_Tools();
}

void ls_command(int argc, char **argv) 
{
	if (argc < 2) 
	{
		printf("Please give a name\n");
		return;
	}
	dumpDir(argv[1]);
}

void rm_command(int argc, char **argv) 
{
	struct yaffs_stat s;
	if (argc < 2) 
	{
		printf("Please give a name\n");
		return;
	}
	yaffs_lstat(argv[1],&s);
	switch(s.st_mode & S_IFMT) 
	{
		case S_IFREG:	/* Data File */
			yaffs_unlink(argv[1]);
			break;
		case S_IFDIR:	/* Directory */
			yaffs_rmdir(argv[1]);
			break;
	}
}

void mv_command(int argc, char **argv) 
{
	if (argc < 3) 
	{
		printf("Please give a name\n");
		return;
	}
	yaffs_rename(argv[1], argv[2]);
	return;
}

void cp_command(int argc, char **argv)
{
	char buffer[256];
	int ho, hn, r, j;

	if (argc < 3) 
	{
		printf("Please give a name\n");
		return;
	}

	ho = yaffs_open(argv[1] , O_RDWR, 0);
	hn = yaffs_open(argv[2], O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
	for(j = 0;; j++) 
	{
		r = yaffs_read(ho, buffer, 256);
		if (r == 0)
			break;
		yaffs_write(hn, buffer, r);
	}
	yaffs_close(ho);
	yaffs_close(hn);
	printf("one file copied.\n");
}

void mkdir_command(int argc, char **argv) 
{
	if (argc < 2) 
	{
		printf("Please give a name\n");
		return;
	}
	yaffs_mkdir(argv[1], 0);
}

void upload_command(int argc, char **argv) 
{
	int h;
	
	if (argc < 2) 
	{
		printf("Please give a name\n");
		return;
	}
	h = yaffs_open(argv[1], O_CREAT | O_RDWR | O_TRUNC, S_IREAD | S_IWRITE);
	APP_Serial_Update(h);
	yaffs_close(h);
}


