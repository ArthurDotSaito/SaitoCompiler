#include "compiler.h"
#include "helpers/vector.h"
#include <assert.h>

struct vector *node_vector = NULL;
struct vector *node_vector_root = NULL;

void node_set_vector(struct vector *vec, struct vecto *root_vec)
{
    node_vector = vec;
    node_vector_root = root_vec;
}

void node_push(struct node *node)
{

    vector_push(node_vector, &node);
}

struct node *node_peek_or_null()
{
    return vector_back_ptr_or_null(node_vector);
}

struct node *node_peek()
{
    return *(struct node **)vector_back_ptr(node_vector);
}

struct node *node_pop()
{
    struct node *last_node = vector_back_ptr(node_vector);
}