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
#define SET_BINARY_MODE(file) \
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
  
int decompress(
Bytef* dest,
uLongf* destLen,
const Bytef* source,
uLong sourceLen) 
{ 
    z_stream stream; 
    int err; 
    const uInt max = (uInt)-1; 
    uLong len, left; 
  
    // for detection of incomplete stream when 
    // *destLen == 0 
    Byte buf[1]; 
  
    len = sourceLen; 
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
  
    sourceLen -= len + stream.avail_in; 
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

struct ZippedFile {
	uLong comp_size;
	uLong uncomp_size;
	size_t filename_length;
	char *filename;
	char *compressed_data;
	char *uncompressed_data;
};

struct ZippedFile* read_zipped_file(const char *zipbuffer) {
	struct ZippedFile* f = (struct ZippedFile*)malloc(sizeof(struct ZippedFile));

	if (zipbuffer[0] != 0x50 || zipbuffer[1] != 0x4B) {
		return f;
	}

	memcpy(&f->comp_size, &zipbuffer[18], 4);
	memcpy(&f->uncomp_size, &zipbuffer[22], 4);
	memcpy(&f->filename_length, &zipbuffer[26], 2);

	f->filename = (char*)malloc(f->filename_length+1);
	memcpy(f->filename, &zipbuffer[30], f->filename_length);
	f->filename[f->filename_length] = '\0';

	f->compressed_data = (char*)malloc(f->comp_size);
	memcpy(f->compressed_data, &zipbuffer[30+f->filename_length], f->comp_size);

	return f;
}

void unzip(struct ZippedFile *zipped_file) {
	zipped_file->uncompressed_data = (char*)malloc(zipped_file->uncomp_size);
	decompress(zipped_file->uncompressed_data, (uLongf *)&zipped_file->uncomp_size, zipped_file->compressed_data, zipped_file->comp_size);
}

void free_zipped_file(struct ZippedFile *file) {
	free(file->filename);
	free(file->compressed_data);
	free(file->uncompressed_data);
	free(file);
}

int main(int argc, char *argv) {
	FILE *f;
	f = fopen("test.zip", "rb");

	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *zipbuffer = (char*)malloc(fsize+1);
	fread(zipbuffer, fsize, 1, f);
	zipbuffer[fsize] = 0;
	fclose(f);

	struct ZippedFile* cf = read_zipped_file(zipbuffer);
	printf("Compressed size: %lu\n", cf->comp_size);
	printf("Uncompressed size: %lu\n", cf->uncomp_size);
	printf("Filename length: %d\n", cf->filename_length);
	printf("Filename: %s\n", cf->filename);

	unzip(cf);
	printf("%s\n", cf->uncompressed_data);

	free_zipped_file(cf);
	free(zipbuffer);

	return 0;
}

