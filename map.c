/**
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <stdlib.h>
#include <string.h>
#include "map.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct map_node_t {
  unsigned hash;
  void *value;
  map_node_t *next;
  /* char key[]; Ҳ�Ǵ��ڵ�,��map_newnode���з������ռ�*/
  /* char value[]; Ҳ�Ǵ��ڵ�*/
};

static unsigned map_hash(const char *str) {
  unsigned hash = 5381;
  while (*str) {
    hash = ((hash << 5) + hash) ^ *str++;
  }
//  printf("hash = %x\r\n", hash);
  return hash;
}


static map_node_t *map_newnode(const char *key, void *value, int vsize) {
  map_node_t *node;
  int ksize = strlen(key) + 1;
  // ����ϵͳ��λ��(sizeof(void*))����
  int voffset = ksize + ((sizeof(void*) - ksize) % sizeof(void*));
  node = MAP_MALLOC(sizeof(*node) + voffset + vsize); // vsizeΪֵ�Ĵ�С(sizeof),vsize���ö���?���ǽṹ�屾���Ͷ���
//  printf("node = %d %d %d %d\r\n", sizeof(*node), voffset, vsize, ksize);
  if (!node) return NULL;
  memcpy(node + 1, key, ksize);
  node->hash = map_hash(key);
  node->value = ((char*) (node + 1)) + voffset;
  memcpy(node->value, value, vsize);
  return node;
}


static int map_bucketidx(map_base_t *m, unsigned hash) {
  /* If the implementation is changed to allow a non-power-of-2 bucket count,
   * the line below should be changed to use mod instead of AND */
//   printf("hash = %x, nbuckets = %d\r\n", hash, m->nbuckets);
  return hash & (m->nbuckets - 1);
}


static void map_addnode(map_base_t *m, map_node_t *node) {
  int n = map_bucketidx(m, node->hash);
  node->next = m->buckets[n];
  m->buckets[n] = node;
}


static int map_resize(map_base_t *m, int nbuckets) {
  map_node_t *nodes, *node, *next;
  map_node_t **buckets;
  int i;
  printf("resize = %d\r\n", nbuckets);
  /* Chain all nodes together */
  nodes = NULL;
  i = m->nbuckets;
  while (i--) {
    node = (m->buckets)[i];
    while (node) {
      next = node->next;
      node->next = nodes;
      nodes = node;
      node = next;
    }
  }
  /* Reset buckets */
  if(m->buckets != NULL)
  {
    printf("free\r\n");
    MAP_FREE(m->buckets);
  }
  buckets = MAP_MALLOC(sizeof(*m->buckets) * nbuckets);
  if (buckets != NULL) {
    m->buckets = buckets;
    m->nbuckets = nbuckets;
  }
  if (m->buckets) {
    memset(m->buckets, 0, sizeof(*m->buckets) * m->nbuckets);
    /* Re-add nodes to buckets */
    node = nodes;
    while (node) {
      next = node->next;
      map_addnode(m, node);
      node = next;
    }
  }
  /* Return error code if realloc() failed */
  return (buckets == NULL) ? -1 : 0;
}


static map_node_t **map_getref(map_base_t *m, const char *key) {
  // ��ȡkey��hashֵ,�����ʵ��(����ϣֵ��ͬ��ô��?�������,strcmpƥ���ֵ)
  unsigned hash = map_hash(key);
  map_node_t **next;
  if (m->nbuckets > 0) {
    next = &m->buckets[map_bucketidx(m, hash)];
    while (*next) {
      if ((*next)->hash == hash && !strcmp((char*) (*next + 1), key)) {
        return next;
      }
      next = &(*next)->next;
    }
  }
//  printf("NULL\r\n");
  return NULL;
}


void map_deinit_(map_base_t *m) {
  map_node_t *next, *node;
  int i;
  i = m->nbuckets;
  while (i--) {
    node = m->buckets[i];
    while (node) {
      next = node->next;
      MAP_FREE(node);
      node = next;
    }
  }
  MAP_FREE(m->buckets);
}


void *map_get(map_c_t *m, const char *key) {
  map_node_t **next = map_getref(&m->base, key);
  return next ? (*next)->value : NULL;
}


// map_base_t��map_t�����Ľṹ���еĳ�Ա,keyΪ��,valueΪֵ,vsizeΪsizeof(value)
int map_set(map_c_t *m, const char *key, void *value, int vsize) {
  int n, err;
  map_node_t **next, *node;
  //��ʱ��value��ʾһ����ַ,�������ַ��ֵ�����ǵĲ���
  if(vsize == 0) //���˿�����ַ���ֵʱ,��Ҫ�����ݳ���,����ʱ��0����
  {
      vsize = m->typeSize;
  }
  /* Find & replace existing node:����Ѿ�������ȫ��ͬ�ļ�,���滻ֵ */
  next = map_getref(&m->base, key);
  if (next) {
    if(m->isCpyAddr)
    {
        memcpy((*next)->value, value, vsize);
    }else
    {
        memcpy((*next)->value, &value, m->typeSize);
    }
    return 0;
  }
  /* Add new node:��ӽڵ� */
  if(m->isCpyAddr)
  {
    node = map_newnode(key, value, vsize);
  }else
  {
    node = map_newnode(key, &value, m->typeSize);
  }

  if (node == NULL) goto fail;
  // һ���ж���buckets���ж���nodes?
  if (m->base.nnodes >= m->base.nbuckets) {
    n = (m->base.nbuckets > 0) ? (m->base.nbuckets << 1) : 1;
    err = map_resize(&m->base, n);
    if (err) goto fail;
  }
  map_addnode(&m->base, node);
  m->base.nnodes++;
  return 0;
  fail:
  if (node) MAP_FREE(node);
  return -1;
}


void map_remove_(map_base_t *m, const char *key) {
  map_node_t *node;
  map_node_t **next = map_getref(m, key);
  if (next) {
    node = *next;
    *next = (*next)->next;
    MAP_FREE(node);
    m->nnodes--;
  }
}


map_iter_t map_iter_(void) {
  map_iter_t iter;
  iter.bucketidx = -1;
  iter.node = NULL;
  return iter;
}


const char *map_next_(map_base_t *m, map_iter_t *iter) {
  if (iter->node) {
    iter->node = iter->node->next;
    if (iter->node == NULL) goto nextBucket;
  } else {
    nextBucket:
    do {
      if (++iter->bucketidx >= m->nbuckets) {
        return NULL;
      }
      iter->node = m->buckets[iter->bucketidx];
    } while (iter->node == NULL);
  }
  return (char*) (iter->node + 1);
}


void map_init(map_c_t *instance, MAP_TYPE type, unsigned char isCpyAddr, unsigned int nbuckets)
{
    memset(instance, 0, sizeof(map_c_t));
    switch(type)
    {
    case MAP_TYPE_VOID_PTR:{instance->typeSize = sizeof(void *);break;}
    case MAP_TYPE_CHAR_PTR:{instance->typeSize = sizeof(char *);break;}
    case MAP_TYPE_INT     :{instance->typeSize = sizeof(int);break;}
    case MAP_TYPE_CHAR    :{instance->typeSize = sizeof(char);break;}
    case MAP_TYPE_FLOAT   :{instance->typeSize = sizeof(float);break;}
    case MAP_TYPE_DOUBLE  :{instance->typeSize = sizeof(double);break;}
    default:break;
    }
    instance->isCpyAddr = isCpyAddr; //������ַ�������
    assert((nbuckets % 2) == 0);
    map_resize(&instance->base, nbuckets);
}
