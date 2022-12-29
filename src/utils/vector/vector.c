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

vector* vector_copy (const vector* to_copy){
    vector* copy;

    if (!to_copy){
        return NULL;
    }

    copy = vector_create(to_copy->item_size, to_copy->deallocator);

    if (!copy){
        return NULL;
    }

    copy->count = to_copy->count;
    
    if (!vector_realloc(copy, to_copy->capacity)){
        free(copy);
        return NULL;
    }

    if(!memcpy(copy->data, to_copy->data, copy->item_size*copy->count)){
        vector_destroy(&copy);
        return NULL;
    }

    return copy;
}

int vector_reverse(vector* v){
    size_t i,len;
    void* tmp,*i1,*i2;

    #define EXIT_IF_NOT(expression)     \
        if (!(expression)) {            \
            free(tmp);   \
            return 0;                     \
        }

    if (!v){
        return 0;
    }

    tmp = malloc(v->item_size);

    if (!tmp){
        return 0;
    }

    len = v->count;

    for (i=0;i<len/2;++i){
        i1 = ((void*)((char*)v->data + (i * v->item_size)));
        i2 = ((void*)((char*)v->data + ((len-i-1) * v->item_size)));
        EXIT_IF_NOT(memcpy(tmp,i1,v->item_size));
        EXIT_IF_NOT(memcpy(i1,i2,v->item_size));
        EXIT_IF_NOT(memcpy(i2,tmp,v->item_size));
    }

    free(tmp);

    return 1;

}

int vector_trim(vector* v){
    void* new_data;

    if (!v){
        return 0;
    }

    if (vector_isempty(v)){
        return 1;
    }

    return vector_resize(v,v->count);
}

int vector_resize(vector* v, const size_t size){
    void* new_data;

    if (!v){
        return 0;
    }

    if (size > v->count){
        if (!vector_realloc(v,size)){
            return 0;
        }
    }

    v->count = size;

    return 1;

}

int vector_push_forward(vector* v, const void* item){
    if (!v || !item){
        return 0;
    }

    if (!vector_reverse(v)){
        return 0;
    }

    vector_push_back(v,item);

    if (!vector_reverse(v)){
        return 0;
    }

    return 1;
}
