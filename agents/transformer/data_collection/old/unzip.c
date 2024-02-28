#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include "zlib.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};

int main(int argc, char *argv) {
	FILE *f;
	f = fopen("test.zip", "r");

	char header[30];
	fread(header, 30, 1, f);

	if (header[0] != 0x50 && header[1] != 0x4B) {
		printf("Invalid file format");
		return -1;
	}

	unsigned char a[4];
	a[0] = header[18];
	a[1] = header[19];
	a[2] = header[20];
	a[3] = header[21];
	unsigned int compressedSize = *(int *)a;
	printf("Compressed size: %d\n", compressedSize);
	
	a[0] = header[22];
	a[1] = header[23];
	a[2] = header[24];
	a[3] = header[25];
	unsigned int uncompressedSize = *(int *)a;
	printf("Uncompressed size: %d\n", uncompressedSize);

	a[0] = header[26];
	a[1] = header[27];
	a[2] = 0x00;
	a[3] = 0x00;
	unsigned int filenameLength = *(int *)a;
	printf("Filename length: %d\n", filenameLength);

	char *filename;
	filename = (char*)malloc((filenameLength+1));
	//fseek(f, 30, SEEK_SET);
	size_t len = fread(filename, 1, filenameLength, f);
	filename[len] = '\0';
	printf("Filename: %s\n", filename);


	struct MemoryStruct compressed;
	compressed.memory = (char*)malloc((compressedSize+1));
	compressed.size = compressedSize;
	len = fread(compressed.memory, 1, compressedSize, f);
	compressed.memory[len] = '\0';

	struct MemoryStruct uncompressed;
	uncompressed.memory = (char*)malloc((uncompressedSize+1));
	uncompressed.size = uncompressedSize;
	//len = inflate123(compressed.memory, compressed.size, uncompressed.memory, uncompressed.size);	
	//uncompress((Bytef *)uncompressed.memory, (uLongf*)&uncompressed.size, (Bytef *)compressed.memory, (uLong)compressed.size);
	//uncompressed.memory[len] = '\0';
	//int inflate123(const void *src, int srcLen, void *dst, int dstLen) {

	printf("%d\n", len);
	//printf("%s\n", uncompressed.memory);
	printf("%02x\n", compressed.memory[compressed.size-2]);


	free(compressed.memory);
	free(uncompressed.memory);
	free(filename);
	fclose(f);

	return 0;
}

