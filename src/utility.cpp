#include "sl.h"
#include <malloc.h>
#include <string.h>

//
// Tracking Memory Block Allocator
//
static uint64 global_alloc_total_bytes{0};
static uint64 global_num_allocations{0};

#include <list>
#include <algorithm>

struct memory_block
{
	void *block_pointer;
	uint32 size;
	char *description;
};

std::list<memory_block> global_allocs;

bool is_globally_alloced(void *block)
{
	for (auto mb : global_allocs)
	{
		if (mb.block_pointer == block)
			return true;
	}
	return false;
}
bool get_global_block(void *pointer, memory_block &block)
{
	for (auto mb : global_allocs)
	{
		if (mb.block_pointer == pointer)
		{
			block = mb;
			return true;
		}
	}
	return false;
}
void add_to_alloced_list(memory_block block)
{
	global_allocs.push_front(block);
}

void remove_from_alloced_list(memory_block block)
{
	std::list<memory_block> kept_blocks;
	for (auto b : global_allocs)
	{
		if (b.block_pointer != block.block_pointer)
		{
			kept_blocks.emplace_back(b);
		}
	}
	global_allocs.swap(kept_blocks);
}

void dump_global_allocs()
{
	uint64 blocksize;
	for (auto block : global_allocs)
	{
#ifdef _WIN32
		blocksize = _msize(block.block_pointer);
#else
		blocksize = malloc_usable_size(block.block_pointer);
#endif
		if (block.description)
		{
			fprintf(stderr, "global block %p : size=%u : %s\n", block.block_pointer, block.size, block.description);
		}
		else
		{
			fprintf(stderr, "global block %p : size=%u \n", block.block_pointer, block.size);
		}
	}
}
void global_free(void *block)
{
	assert(block);
	uint64 block_size{0};
#ifdef _WIN32
	block_size = _msize(block);
	assert((int64)block_size != -1);
#else
	block_size = malloc_usable_size(block);
	assert(is_globally_alloced(block));
#endif
	global_alloc_total_bytes -= block_size;
	global_num_allocations--;

	memory_block global_block;
	if (get_global_block(block, global_block))
	{
		remove_from_alloced_list(global_block);
	}
	free(block);
}

void *global_alloc(const uint32 requested_numbytes, const char *description)
{
	void *block = malloc(requested_numbytes);
	assert(block);
	memset(block, 0, requested_numbytes);
	uint64 num_usable_bytes{0};
#ifdef _WIN32
	num_usable_bytes = _msize(block);
#else
	num_usable_bytes = malloc_usable_size(block);
#endif
	global_alloc_total_bytes += num_usable_bytes;
	global_num_allocations++;

	memory_block memblock;
	memblock.block_pointer = block;
	memblock.size = num_usable_bytes;
	if (description != nullptr)
	{
		memblock.description = strdup(description);
	}
	else
	{
		memblock.description = nullptr;
	}
	add_to_alloced_list(memblock);
	return block;
}

//
// Binary Utils
// note: this is some of my legacy code.
// handles up to 4GB files only.
//
#include <fstream>

ubyte *load_binary_file(const std::string &filename)
{
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	ubyte *buffer = (ubyte *)global_alloc(size);
	if (file.read((char *)buffer, size))
	{
		return buffer;
	}
	else
	{
		global_free(buffer);
		return nullptr;
	}
}
//
// Text Utils
// note: this is some of my legacy code.
// handles up to 4GB files only.
//
std::string load_text_file(const std::string &filename)
{
	std::ifstream file(filename);
	std::string str;
	std::string file_contents;
	while (std::getline(file, str))
	{
		file_contents += str;
		file_contents.push_back('\n');
	}
	return file_contents;
}
std::vector<std::string> load_text_file_lines(const std::string &filename)
{
	std::ifstream file(filename);
	std::string str;
	std::vector<std::string> file_contents;
	while (std::getline(file, str))
	{
		file_contents.emplace_back(str);
	}
	return std::move(file_contents);
}

std::vector<std::string> split_string(const std::string &str, const std::string &delims,
									  std::vector<std::string> &elems, bool skip_empty)
{
	std::string::size_type pos, prev = 0;
	while ((pos = str.find_first_of(delims, prev)) != std::string::npos)
	{
		if (pos > prev)
		{
			if (skip_empty && 1 == pos - prev)
				break;
			elems.emplace_back(str, prev, pos - prev);
		}
		prev = pos + 1;
	}
	if (prev < str.size())
	{
		elems.emplace_back(str, prev, str.size() - prev);
	}
	return std::move(elems);
}


//
// High Res Sleep
//
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
	if (!inited)
	{
		timer = CreateWaitableTimer(nullptr, TRUE, nullptr);
		inited = true;
	}
	SetWaitableTimer(timer, &ft, 0, nullptr, nullptr, 0);
	WaitForSingleObject(timer, INFINITE);
}

#else
#include <time.h>
#include <errno.h>
void sleep_in_ms(uint32 ms)
{
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = ms % 1000 * 1000000;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}
void sleep_in_us(uint32 us)
{
	struct timespec ts;
	ts.tv_sec = us / 1000000;
	ts.tv_nsec = us % 1000000 * 1000;
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
		;
}
#endif

//
// RNG
//
#include <random>
static std::random_device rdevice;
static std::default_random_engine rengine(rdevice());
int32 random_int(int32 start, int32 end)
{
	std::uniform_int_distribution<int32> udist(start, end);
	return udist(rengine);
}

float32 random_float(float32 start, float32 end)
{
	std::uniform_real_distribution<float32> fdist(start, end);
	return fdist(rengine);
}