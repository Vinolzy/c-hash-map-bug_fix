/**
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef MAP_H
#define MAP_H

#include <string.h>
#include "stdlib.h"

#define MAP_VERSION "0.1.0"

struct map_node_t;
typedef struct map_node_t map_node_t;

#define MAP_MALLOC malloc
#define MAP_FREE free

typedef struct {
  map_node_t **buckets;
  unsigned nbuckets, nnodes;
} map_base_t;

typedef struct {
  unsigned bucketidx;
  map_node_t *node;
} map_iter_t;

typedef enum{
    MAP_TYPE_VOID_PTR,    //void *
    MAP_TYPE_CHAR_PTR,    //char *
    MAP_TYPE_INT,         //int
    MAP_TYPE_CHAR,        //char
    MAP_TYPE_FLOAT,       //float
    MAP_TYPE_DOUBLE,      //double
}MAP_TYPE;

typedef struct{
    map_base_t base;
    unsigned char typeSize;
    unsigned char isCpyAddr; //1-拷贝地址里的内容(set时需要指定大小),0-还是拷贝值
}map_c_t;

#define map_deinit(m)\
  map_deinit_(&(m)->base)


#define map_remove(m, key)\
  map_remove_(&(m)->base, key)


#define map_iter(m)\
  map_iter_()


#define map_next(m, iter)\
  map_next_(&(m)->base, iter)


void map_deinit_(map_base_t *m);
void *map_get(map_c_t *m, const char *key);
int map_set(map_c_t *m, const char *key, void *value, int vsize);
void map_remove_(map_base_t *m, const char *key);
map_iter_t map_iter_(void);
const char *map_next_(map_base_t *m, map_iter_t *iter);
void map_init(map_c_t *instance, MAP_TYPE type, unsigned char isCpyAddr, unsigned int nbuckets);


//typedef map_t(void*) map_void_t;
//typedef map_t(char*) map_str_t;
//typedef map_t(int) map_int_t;
//typedef map_t(char) map_char_t;
//typedef map_t(float) map_float_t;
//typedef map_t(double) map_double_t;

#endif
