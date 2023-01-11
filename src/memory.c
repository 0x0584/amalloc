#include "memory.h"

size_t              size_offset(size_t size) {
    return size + sizeof(t_chunk);
}

size_t              size_no_offset(size_t size) {
    return size - sizeof(t_chunk);
}

bool        chunk_init(t_ptr_chunk chunk, t_generic_ptr mmap_ptr, size_t mmap_page_size) {
    assert(ptr != NULL);
    assert(chunk_size != 0);
    assert(chunk_size < MAX_CHUNK_SIZE);
    chunk->ptr = (unsigned char *)mmap_ptr + sizeof(t_chunk);
    chunk->size = size_no_offset(mmap_page_size);
    chunk->available = true;
    chunk->next = NULL;
    return true;
}

void        page_init(t_ptr_page arena, int page_size) {

/*

    page of size page_size

     +-------------+<--------------------------| base pointer returned by mmap
     | layout      +------+
     | quota       |      |
     | next page   |+-----v------+
     +-------------+| ptr        +-------+  ptr always points to the offset of the base pointer
                    | size       |       |  size without the offset
                    | available  |       |  true if the chunk is free
             +------+ next       |       |  points to the next chunk, or NULL if it is the last
             |      | ~~~~~~~~   <-------+
             |      | ~~~~~~~    |
             |      | ~~~~~~~~   |
             |      | ~~~~~~~~~  |
             |      | ~~~~~~~    |
             +------> ptr        +-------+
                    | size       |       |
                    | available  |       |
        NULL <------+ next       |       |
                    |            <-------+
                    |            |
                    |            |
                    |            |
                    |            |
                    |            |
                    +------------+

 */


    assert(arena != NULL);
    assert(arena->layout == NULL);
    assert(page_size > sizeof(t_chunk));
    arena->layout = mmap(NULL, page_size, PROT_NONE, MAP_ANON, -1, 0);
    chunk_init(&arena->mem, arena->mem, page_size);
    arena->n_alloced = 0;
    arena->quota = page_size;
    arena->next = NULL;
}


t_ptr_chunk             page_alloc_tiny_chunk(t_ptr_page page, size_t size) {
    t_ptr_chunk     chunk;
    t_ptr_chunk     tmp;
    size_t          tmp_size;
    size_t          internal_size;

    assert(size <= TINY_ZONE_MAX_SIZE);
    chunk = page->layout;
    internal_size = size_offset(size);
    while (!chunk->available || (chunk->available && chunk->size < internal_size))
    {
        chunk = chunk->next;
        assert(chunk != NULL);
    }

    tmp_size = chunk->size;

/*

  Memory layout after successful allocation

   - allocation cases, not all allocations utilise the whole available memory
   - a fragment is a memory that is unusable because is
   - allocation might have a prev except when it is the page header
      ==> every allocation is compare with base pointer of the page->layout and INFO

     +-------------+                         +-------------+
     | INFO        +-----+                   | INFO        +------next-----> NULL
     +-------------+     |                   +-------------+
     | ~~~~~~~~~   |     |                   | ~~~~~~~~~~  |
     | ~~~~~~~~~~  |     |                   | ~~~~~~~~~~~ |
     | ~~~~~~~~~~~ |    next                 | ~~~~~~~~    |
     | ~~~~~~~~    |     |                   | ~~~~~~~~~   |
     | ~~~~~~~~~   |     |                   | ~~~~~~~~~~  |
     | ~~~~~~~~    |     |                   | ~~~~~~~~    |
     +-------------+ <---+                   +-------------+
     | INFO        +-------next---> NULL     | +++++++++++ |
     +-------------+                         | +++++++++++ |
     | ___________ |                         | +++++++++++ | fragment
     | ___________ |  remaining memory       | +++++++++++ |
     +-------------+                         +-------------+

             ~~~~~~~~ indicates the allocated memory for the pointer
             ________ indicates the remaining memory (= size + offset)
             ++++++++ indicates a fragmant, which is unused memory

     - invariant (1): the remainig memory > offset
       => property: fragmant < offset

     - invariant (2): the remaining memory will follow to the next allocation
                      except when it's a leaf, then it is NULL terminated
*/

    chunk->size = size;
    chunk->available = false;
    if (chunk->next == NULL)
    {
/*


  I) allocate in a leaf slot


  before allocation

     +-------------+
     | INFO        |
     +-------------+  ----------------------------- alloc info #1  <-------------- available memory
     | ___________ |                                     |
     | ___________ |                                     |
     | ___________ |                                     |
     | ___________ |                                    next
     | ___________ |                                     |
     | ___________ |                                     |
     | ___________ |                                     |
     +-------------+                                     |
                                                         |
                                                         +-----> NULL

  after allocation

     +-------------+
     | INFO        |
     +-------------+  ----------------------------- alloc info #1
     | ~~~~~~~~~~~ |                                     |
     | ~~~~~~~~    |                                     |
     | ~~~~~~      |                                     |
     | ~~~~~~~~~~  |                                    next
     +-------------+                                     |
     | INFO        |  ------------------------------ alloc info #2   <-------------- available memory
     +-------------+                                     |
     | ___________ |                                     |
     +-------------+                                    next
                                                         |
                                                         +-----> NULL

*/

        if (tmp_size - internal_size > TINY_ZONE_MAX_SIZE)
        {
            chunk->next = chunk->ptr + size;
            tmp = chunk->next;

            tmp->ptr = tmp + sizeof(t_chunk);
            tmp->size = tmp_size - internal_size - sizeof(t_chunk);
            tmp->available = true;
            tmp->next = NULL;
        }
        else
        {
            assert(false && "memory leftover");
        }
    }
    else
    {

/*


  II) allocate within



  before allocation

     +-------------+
     | INFO        |
     +-------------+  ----------------------------- alloc info #1  <-------------- available memory
     | ___________ |                                     |
     | ___________ |                                     |
     | ___________ |                                     |
     | ___________ |                                    next
     | ___________ |                                     |
     | ___________ |                                     |
     | ___________ |                                     v
     +-------------+  ----------------------------- alloc info #2
     | INFO        |                                     |
     +-------------+                                     |
     | ~~~~~~~     |                                    next
     +-------------+                                     |
                                                         |
                                                         +-----> NULL
  after allocation

     +-------------+  ----------------------------- page info
     | INFO        |
     +-------------+  ----------------------------- alloc info #1
     +-------------+                                     |
     | ~~~~~~~~~~  |                                     |
     | ~~~~~~~~~   |                                    next
     | ~~~~~       |                                     |
     | ~~~~~~~~    |                                     v
     +-------------+  ----------------------------- alloc info #3  <-------------- available memory
     | INFO        |                                     |
     +-------------+                                    next
     | ___________ |   remaining memory                  |
     | ___________ |                                     v
     +-------------+  ----------------------------- alloc info #2
     | INFO        |                                     |
     +-------------+                                     |
     | ~~~~~~~     |                                    next
     +-------------+                                     |
                                                         |
                                                         +-----> NULL

 */
    }

    return chunk;
}

t_ptr_chunk             page_alloc_chunk(t_ptr_page page, size_t size) {
    return NULL;
}

t_ptr_chunk             page_alloc_large_chunk(t_ptr_page page, size_t size) {
    return NULL;
}


#define DEFAULT_SIZE_FACTOR 16

void get_arena_info(struct arena_info *info, size_t size) {
    assert(info != NULL);
    info->page_size = getpagesize();
    if (current_alloc_size <= TINY_CHUNK) {
        info->index = 0;
    } else if (current_alloc_size <= CHUNK_SIZE) {
        info->index = 1;
        info->page_size *= 2;
    } else {
        info->index = 2;
        int tmp = current_alloc_size % page_size;
        current_alloc_size /= page_size;
        info->page_size *= (current_alloc_size + (tmp != 0));
    }
}


int     arena_init(size_t current_alloc_size) {
    struct page_info info;

    get_page_info(&info, current_alloc_size);
    if (g_arena[info.index].mem != NULL) {
        page_init(&g_arena[info.index], info.page_size);
    }
    return index;
}

void        show_alloc_mem() {

}

t_page  g_arena[3] = {
    {NULL,0,0,NULL}, {NULL,0,0,NULL}, {NULL,0,0,NULL},
};
