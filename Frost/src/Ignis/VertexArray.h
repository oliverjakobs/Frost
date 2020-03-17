#ifndef IGNIS_VERTEX_ARRAY_H
#define IGNIS_VERTEX_ARRAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Core/Buffer.h"

typedef struct
{
    GLenum type;
    GLsizei count;
    GLboolean normalized;
} ignis_buffer_element;

typedef struct
{
    GLuint name;
    GLuint vertex_attrib_index;

    /* dynamic buffer array */
    ignis_buffer* array_buffers;
    size_t buffer_used;
    size_t buffer_count;

    ignis_buffer element_buffer;
} ignis_vertex_array;

ignis_vertex_array* ignisGenerateVertexArray();
void ignisDeleteVertexArray(ignis_vertex_array* vertex_array);

void ignisBindVertexArray(ignis_vertex_array* vertex_array);
void ignisUnbindVertexArray(ignis_vertex_array* vertex_array);

void ignisAddArrayBuffer(ignis_vertex_array* vertex_array, GLsizeiptr size, const void* data, GLenum usage);
void ignisAddArrayBufferLayout(ignis_vertex_array* vertex_array, GLsizeiptr size, const void* data, GLenum usage, ignis_buffer_element* elements, size_t count);
void ignisLoadElementBuffer(ignis_vertex_array* vertex_array, GLuint* indices, size_t count, GLenum usage);

#ifdef __cplusplus
}
#endif

#endif /* IGNIS_VERTEX_ARRAY_H */