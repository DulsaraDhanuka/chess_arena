#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include "zlib.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

void download_file(const char *url, struct MemoryStruct *chunk) {
	CURL *curl_handle;
	CURLcode res;

	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, url);
	curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
	curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYSTATUS, 0);
	curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)chunk);
	res = curl_easy_perform(curl_handle);

	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	} else {
		printf("%lu bytes retrieved\n", (unsigned long)chunk->size);
	}

	curl_easy_cleanup(curl_handle);
}

int main(int argc, char *argv[]) {
	curl_global_init(CURL_GLOBAL_ALL);

	struct MemoryStruct chunk;
	chunk.memory = malloc(1);
	chunk.size = 0;
	download_file("https://www.pgnmentor.com/players/Morphy.zip", &chunk);

	FILE *f = fopen("test.zip", "wb");
	fwrite(chunk.memory, chunk.size, 1, f);
	fclose(f);

	free(chunk.memory);

	curl_global_cleanup();
	return 0;
}


