// C program to read and print 
// all files in a zip file 
// uses libraries libzip and zlib 
#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <zip.h> 
#include <zlib.h> 
  
// Compatibility with Windows 
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__) 
#include <fcntl.h> 
#include <io.h> 
#define SET_BINARY_MODE(file)                              \
    setmode(fileno(file), O_BINARY)
#else 
#define SET_BINARY_MODE(file) 
#endif 
  
#define CHUNK 1000 
  
// We need to change one line of the zlib library function 
// uncompress2 from err = inflateInit(&stream); to err = 
// inflateInit2(&stream, -MAX_WBITS); 
  
// This tells function that there is no extra zlib, gzip, z 
// header information It's just a pure stream of compressed 
// data to decompress 
  
int ZEXPORT uncompress2(dest, destLen, source, sourceLen) 
Bytef* dest; 
uLongf* destLen; 
const Bytef* source; 
uLong* sourceLen; 
{ 
    z_stream stream; 
    int err; 
    const uInt max = (uInt)-1; 
    uLong len, left; 
  
    // for detection of incomplete stream when 
    // *destLen == 0 
    Byte buf[1]; 
  
    len = *sourceLen; 
    if (*destLen) { 
        left = *destLen; 
        *destLen = 0; 
    } 
    else { 
        left = 1; 
        dest = buf; 
    } 

    stream.next_in = (z_const Bytef*)source; 
    stream.avail_in = 0; 
    stream.zalloc = (alloc_func)0; 
    stream.zfree = (free_func)0; 
    stream.opaque = (voidpf)0; 
  
    err = inflateInit2(&stream, 
                       -MAX_WBITS); // THIS LINE IS CHANGED 
    if (err != Z_OK) 
        return err; 
 
    stream.next_out = dest; 
    stream.avail_out = 0; 
  
    do { 
        if (stream.avail_out == 0) { 
            stream.avail_out 
                = left > (uLong)max ? max : (uInt)left; 
            left -= stream.avail_out; 
        } 
        if (stream.avail_in == 0) { 
            stream.avail_in 
                = len > (uLong)max ? max : (uInt)len; 
            len -= stream.avail_in; 
        } 
        err = inflate(&stream, Z_NO_FLUSH); 
    } while (err == Z_OK); 
  
    *sourceLen -= len + stream.avail_in; 
    if (dest != buf) 
        *destLen = stream.total_out; 
    else if (stream.total_out && err == Z_BUF_ERROR) 
        left = 1; 
  
    inflateEnd(&stream); 
    return err == Z_STREAM_END 
               ? Z_OK 
               : err == Z_NEED_DICT 
                     ? Z_DATA_ERROR 
                     : err == Z_BUF_ERROR 
                               && left + stream.avail_out 
                           ? Z_DATA_ERROR 
                           : err; 
} 
  
int main(int argc, char* argv[]) 
{ 
	char loc[] = "test.zip";

    int errorp = 0; // error code variable 
    zip_t* arch = NULL; // Zip archive pointer 
    arch = zip_open(loc, 0, &errorp); 
  
    // allocates space for file information 
    struct zip_stat* finfo = NULL; 
    finfo = calloc(256, sizeof(int)); // must be allocated 
    zip_stat_init(finfo); 
  
    // Loop variables 
    int index = 0; 
    char* txt = NULL; 
    zip_file_t* fd = NULL; 
    char* outp = NULL; 
  
    while (zip_stat_index(arch, index, 0, finfo) == 0) { 
  
        /*txt = calloc(finfo->comp_size + 1, sizeof(char)); 
        // Read compressed data to buffer txt 
        // ZIP_FL_COMPRESSED flag is passed in to read the 
        // compressed data 
        fd = zip_fopen_index(arch, 0, ZIP_FL_COMPRESSED); 
        zip_fread(fd, txt, finfo->comp_size); 
  
        outp = calloc(finfo->size + 1, sizeof(char)); 
        // uncompresses from txt buffer to outp buffer 
        // uncompress function calls our uncompress2 
        // function defined at top 
  
        printf("FILE #%i: %s\n", index + 1, finfo->name); 
        printf("\n%s\n", outp); 
  
        // free memory every iteration 
        free(txt); 
        free(outp); 
        index++; */

		// allocate room for the entire file contents 
        txt = calloc(finfo->size + 1, sizeof(char)); 
        fd = zip_fopen_index( 
            arch, index, ZIP_FL_COMPRESSED); // opens file at count index 
                             // reads from fd finfo->size 
                             // bytes into txt buffer 
        zip_fread(fd, txt, finfo->size); 
  
        outp = calloc(finfo->size + 1, sizeof(char)); 
        int ret = uncompress2(outp, (uLongf *)&finfo->size, txt, (uLong*)&finfo->comp_size); 
		printf("RET: %d\n", ret);

        printf("file #%i: %s\n\n", index + 1, 
               finfo->name); // prints filename 
        printf("%s\n\n", 
               outp); // prints entire file contents 
  
        // frees allocated buffer, will 
        // reallocate on next iteration of loop 
        free(txt); 
  
        // increase index by 1 and the loop will 
        // stop when files are not found 
        index++; 
    } 
}

