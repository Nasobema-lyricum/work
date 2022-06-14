#include <stdio.h>

int main(void)
{
	FILE *file;
	file = fopen("test.bin","wb");

#if 0
	unsigned char buf[] = {
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff,
		0xff,0xff,0xff,0xff
	};

	for (int i = 0; i < 0x00800000 ; i++) {
		fwrite(buf,sizeof(buf),1,file);
	}
#endif

	unsigned short buf[0x10000] = {};

	for (int i = 0; i < 0x10000; i++) {
		buf[i] = (unsigned short)i;
	}

	for (int i = 0; i < 0x400; i++) {
		fwrite(buf,sizeof(buf),1,file);
	}

	fclose(file);
	return 0;
}
