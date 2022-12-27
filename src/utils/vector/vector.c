#include "vector.h"

#include <stdlib.h>
#include <string.h>

static void* vector_at_ (const vector* v, const size_t at){
    return (void*)((char*)v->data + (at * v->item_size));
}

vector *vector_create(const size_t item_size, const vec_it_dealloc_type deallocator){

    vector* new_vector;

    if (item_size == 0){
        return NULL;
    }

    new_vector = (vector*)malloc(sizeof(vector));

    if (!new_vector){
        return NULL;
    }

    if (!vector_init(new_vector,item_size,deallocator)){
        free(new_vector);
        return NULL;
    }

    return new_vector;
}

int vector_init(vector *v, const size_t item_size, const vec_it_dealloc_type deallocator){

    if (!v || item_size==0){
        return 0;
    }

    v->item_size=item_size;
    v->deallocator=deallocator;
    v->count=0;
    v->capacity=0;
    v->data=NULL;

    if (!vector_realloc(v, VECTOR_INIT_SIZE)){
        return 0;
    }

    /* can be used as string*/
    *((char*)v->data) = 0;


    return 1;

}

int vector_realloc(vector *v, const size_t new_capacity){
    void* new_data;

    if (!v || new_capacity<vector_count(v)){
        return 0;
    }

/*
    new_data = malloc(v->item_size*new_capacity);

    if (!new_data){
        return 0;
    }

    memcpy(new_data,v->data, v->item_size * v->count);

    free(v->data);
*/
    new_data = realloc(v->data, v->item_size*new_capacity);

    if (!new_data){
        return 0;
    }

    v->data = new_data;
    v->capacity = new_capacity;

    return 1;

}

void *vector_at(const vector *v, const size_t at){
    if (at>=vector_count(v)){
        return NULL;
    }

    return vector_at_(v,at);
}

int vector_push_back(vector *v, const void *item){
    if (!v){
        return 0;
    }

    if (vector_count(v)>=vector_capacity(v)){
       if( !vector_realloc(v,v->capacity * VECTOR_SIZE_MULT)){
        return 0;
       }
    }

    memcpy(vector_at_(v,v->count), item, v->item_size);

    ++v->count;

    return 1;

}

void vector_destroy(vector **v){
    
    size_t i;
    if (!v || !*v){
        return;
    }

    if ((*v)->deallocator){
        for (i=0;i<(*v)->count;++i){
            ((*v)->deallocator)(vector_at_(*v,i));
        }
    }

    if ((*v)->data){
        free((*v)->data);
    }

    free(*v);

    *v=NULL;
}


size_t vector_capacity(const vector *v) {
    return v ? v->capacity : 0; 
}

size_t vector_count(const vector *v) {
    return v ? v->count : 0;
}

int vector_isempty(const vector *v)  {
    return vector_count(v) == 0;
}

void *vector_giveup(vector *v) {
    void *data;

    if (vector_isempty(v)) {
        return NULL;
    }

    data = v->data;

    vector_init(v, v->item_size, v->deallocator);

    return data;
}

int vector_push(vector* v, const void* item, const size_t index){
    if (!v || (index>=v->count)){
        return 0;
    }

    if (v->deallocator){
        (v->deallocator)(vector_at(v,index));
    }

    memcpy(vector_at_(v,index), item, v->item_size);

    return 1;
}


vector* vector_init_data (const size_t item_size, const vec_it_dealloc_type deallocator, void** data, int data_count){
    vector* new_vector;
    size_t i;

    if (!data){
        return NULL;
    }

    new_vector = vector_create(item_size, deallocator);
    if (!new_vector){
        return NULL;
    }

    for (i=0; i<data_count;++i){
        if (!data[i]){
            vector_destroy(&new_vector);
            return NULL;
        }
        vector_push_back(new_vector, data[i]);
    }

    return new_vector;
}
