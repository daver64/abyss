#include "sl.h"
#include <malloc.h>
#include <string.h>
static uint64 global_alloc_total_bytes{0};
static uint64 global_num_allocations{0};

void global_free(void *pointer)
{
    uint64 pointer_size{0};
    #ifdef _WIN32
    pointer_size = _msize(pointer);
    #else
    pointer_size = malloc_usable_size(pointer);
    #endif
    global_alloc_total_bytes-=pointer_size;
    global_num_allocations--;
    fprintf(stderr,"global free #%zu::%p::%zu bytes\n",
        global_num_allocations,pointer,pointer_size);
    free(pointer);
}

void *global_alloc(const uint32 requested_numbytes)
{
    void *pointer = malloc(requested_numbytes);
    memset(pointer,0,requested_numbytes);
    uint64 num_usable_bytes{0};
    #ifdef _WIN32
    num_usable_bytes=_msize(pointer);
    #else
    num_usable_bytes=malloc_usable_size(pointer);
    #endif
    global_alloc_total_bytes+=num_usable_bytes;
    global_num_allocations++;
    fprintf(stderr,"global alloc #%zu::%p::%zu bytes\n",
        global_num_allocations,pointer,num_usable_bytes);
    return pointer;
}

char* load_text_file(const char* filename, int32& num_bytes_read)
{
	FILE* fp = fopen(filename, "r");
	if (!fp)
	{
		num_bytes_read = 0;
		return NULL;
	}
	fseek(fp, 0, SEEK_END);
	int32 len = ftell(fp);
	if (len < 1)
	{
		num_bytes_read = 0;
		fclose(fp);
		return NULL;
	}
	fseek(fp, 0, SEEK_SET);
	char* buffer = (char*)global_alloc(len + 1);

	memset(buffer, 0, len + 1);
	num_bytes_read = (int32)fread(buffer, 1, len, fp);
	fclose(fp);
	return buffer;
}

int32 string_split_c(const char* txt, char delim, char*** tokens)
{
	int32* tklen, * t, count = 1;
	char** arr, * p = (char*)txt;

	while (*p != '\0')
	{
		if (*p++ == delim)
		{
			count += 1;
		}
	}
    int32 *ptr=(int32*)global_alloc(sizeof(int32) * count);
	t = tklen = ptr;//(int32_t*)global_alloc(count, sizeof(int32_t));
	for (p = (char*)txt; *p != '\0'; p++)
	{
		*p == delim ? *t++ : (*t)++;

	}
	*tokens = arr = (char**)global_alloc(count * sizeof(char*));
	t = tklen;
    char *ptr2=(char*)global_alloc((*(t++) + 1)*sizeof(char*));
    
	p = *arr++ = ptr2;//(char*)sl_calloc(*(t++) + 1, sizeof(char*));
	while (*txt != '\0')
	{
		if (*txt == delim)
		{
			*p++ = delim;
            char *ptr3=(char*)global_alloc((*(t++)+1)*sizeof(char*));
			p = *arr++ = ptr3;// (char*)sl_calloc(*(t++) + 1, sizeof(char*));
			txt++;
		}
		else
		{
			*p++ = *txt++;
		}
	}
	//sl_free(tklen);
    global_free(tklen);
	return count;
}

// Timing
#ifdef _WIN32
#include <windows.h>
void sleep_in_ms(uint32 ms)
{
	sleep_in_us(ms * 1000);
}
static HANDLE timer;
static bool inited{false};
void sleep_in_us(uint32 us)
{
	LARGE_INTEGER ft;
	ft.QuadPart = -(int64)(us * 10);
	if(!inited)
	{
		timer=CreateWaitableTimer(nullptr,TRUE,nullptr);
		inited=true;
	}
	SetWaitableTimer(timer,&ft,0,nullptr,nullptr,0);
	WaitForSingleObject(timer,INFINITE);
}

#else
#include <time.h>
#include <errno.h>
void sleep_in_ms(uint32 ms)
{
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = ms % 1000 * 1000000;
	while(nanosleep(&ts,&ts)==-1 && errno==EINTR);
}
void sleep_in_us(uint32 us)
{
	struct timespec ts;
	ts.tv_sec = us /1000000;
	ts.tv_nsec = us % 1000000 * 1000;
	while(nanosleep(&ts,&ts)==-1 && errno==EINTR);
}
#endif
