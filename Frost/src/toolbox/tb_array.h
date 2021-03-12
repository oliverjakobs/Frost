#ifndef TB_ARRAY_H
#define TB_ARRAY_H

#include <stdlib.h>

#define TB_ARRAY_HDR_ELEM	size_t
#define TB_ARRAY_HDR_SIZE	2 * sizeof(TB_ARRAY_HDR_ELEM)

#define tb_array__max(a, b) ((a) >= (b) ? (a) : (b))

#define tb_array__hdr(b) ((size_t*)(void*)(b) - 2)
#define tb_array__cap(b) tb_array__hdr(b)[0]
#define tb_array__len(b) tb_array__hdr(b)[1]

#define tb_array_len(b) ((b) ? tb_array__len(b) : 0)
#define tb_array_cap(b) ((b) ? tb_array__cap(b) : 0)

#define tb_array_resize(b, n)   (*((void**)&(b)) = tb_array__resize((b), (n), sizeof(*(b))))
#define tb_array_reserve(b, n)  (*((void**)&(b)) = tb_array__reserve((b), (n), sizeof(*(b))))
#define tb_array_grow(b, n)     (*((void**)&(b)) = tb_array__grow((b), (n), sizeof(*(b))))
#define tb_array_free(b)        ((b) ? (free(tb_array__hdr(b)), (b) = NULL) : 0);

#define tb_array_push(b, v) (tb_array_grow((b), 1), (b)[tb_array__len(b)++] = (v))

#define tb_array_pack(b)    (tb_array_resize((b), tb_array_len(b)))
#define tb_array_clear(b)   ((b) ? tb_array__len(b) = 0 : 0)

#define tb_array_last(b)    ((b) + tb_array_len(b))
#define tb_array_sizeof(b)  (tb_array_len(b) * sizeof(*(b)))

static void* tb_array__resize(void* buf, size_t new_cap, size_t elem_size)
{
    size_t* hdr = realloc(buf ? tb_array__hdr(buf) : NULL, TB_ARRAY_HDR_SIZE + (new_cap * elem_size));

    if (!hdr) return NULL; /* out of memory */

    hdr[0] = new_cap;
    if (!buf) hdr[1] = 0;

    return hdr + 2;
}

static void* tb_array__grow(void* buf, size_t increment, size_t elem_size)
{
    if (buf && tb_array__len(buf) + increment < tb_array__cap(buf))
        return buf;

    size_t new_size = tb_array_len(buf) + increment;
    size_t new_cap = tb_array__max((buf ? 2 * tb_array__cap(buf) : 1), new_size);

    return tb_array__resize(buf, new_cap, elem_size);
}

static void* tb_array__reserve(void* buf, size_t reserve, size_t elem_size)
{
    if (buf && reserve < tb_array__cap(buf))
        return buf;

    return tb_array__resize(buf, reserve, elem_size);
}

#endif /* !TB_ARRAY_H */
