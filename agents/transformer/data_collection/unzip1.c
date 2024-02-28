// C program to read and print 
// all files in a zip file 
// uses library libzip 
#include <stdio.h>
#include <stdlib.h> 
#include <zip.h> 
  
// this is run from the command line with the zip file 
// passed in example usage: ./program zipfile.zip 
int main(int argc, char* argv[]) 
{ 
 	char loc[] = "test.zip";

	FILE *f = fopen(loc, "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

	char *zipbuffer = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);

	string[fsize] = 0;	


    int errorp = 0; 
    zip_t* arch = NULL; 
    arch = zip_open(loc, 0, &errorp); 
    struct zip_stat* finfo = NULL; 
  
    finfo = calloc(256, sizeof(int)); 
  
    zip_stat_init(finfo); 
  
    zip_file_t* fd = NULL; 
    char* txt = NULL; 
   
	int count = 0; 
  
    while ((zip_stat_index(arch, count, 0, finfo)) == 0) { 
        txt = calloc(finfo->size + 1, sizeof(char)); 
        fd = zip_fopen_index(arch, count, 0);
        zip_fread(fd, txt, finfo->size); 
  
        printf("file #%i: %s\n\n", count + 1, finfo->name);
        //printf("%s\n\n", txt);
  
        free(txt); 
        count++; 
    } 
    return 0; 
}

