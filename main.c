#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define TEST_MODE 3

static map_c_t langMap;
int main()
{
#if (TEST_MODE == 1)       //�ַ�������:�����ַ�����ַ[����]
    map_init(&langMap, MAP_TYPE_CHAR_PTR, 0, 8);
    map_set(&langMap, "test", "1234", 0);
    char **ret = map_get(&langMap, "test");
    printf("%x %x = %s\r\n", "1234", *ret, *ret);
#elif (TEST_MODE == 2)     //�ַ�������:�����ַ�����ֵmap_node_t������ڴ���(��Ҫָ������)
    map_init(&langMap, MAP_TYPE_CHAR_PTR, 1, 8);
    map_set(&langMap, "test", "1234", sizeof("1234"));
    char *ret = map_get(&langMap, "test");
    printf("%x %x = %s\r\n", "1234", ret, ret);
#elif (TEST_MODE == 3)     //int����:�������ֵ�ֵ��map_node_t��[����]
    map_init(&langMap, MAP_TYPE_INT, 0, 8);
    map_set(&langMap, "test", 123, 0);
    int *ret = map_get(&langMap, "test");
    printf("%x = %d\r\n", *ret, *ret);
#elif (TEST_MODE == 4)     //int����:����int������ֵ��map_node_t��
    const int a = 123;
    map_init(&langMap, MAP_TYPE_INT, 1, 8);
    map_set(&langMap, "test", &a, 0);
    int *ret = map_get(&langMap, "test");
    printf("%x %x = %d\r\n", &a, *ret, *ret);
#elif (TEST_MODE == 5)     //int����:����int�����ĵ�ַ
    const int a = 123;
    map_init(&langMap, MAP_TYPE_INT, 0, 8);
    map_set(&langMap, "test", &a, 0);
    int **ret = map_get(&langMap, "test");
    printf("%x %x = %d\r\n", &a, *ret, **ret);
#elif (TEST_MODE == 6)     //char����:�����ַ���ֵ��map_node_t��[����]
    map_init(&langMap, MAP_TYPE_CHAR, 0, 8);
    map_set(&langMap, "test", 'a', 0);
    char *ret = map_get(&langMap, "test");
    printf("%x = %c\r\n", *ret, *ret);
#elif (TEST_MODE == 7)     //double����:����double������ַ��map_node_t��
    const double a = 3.14;
    map_init(&langMap, MAP_TYPE_DOUBLE, 0, 8);
    map_set(&langMap, "test", &a, 0);
    double **ret = map_get(&langMap, "test");
    printf("%x %x = %lf\r\n", &a, *ret, **ret);
#elif (TEST_MODE == 8)     //double����:����double������ֵ��map_node_t��
    const double a = 3.14;
    map_init(&langMap, MAP_TYPE_DOUBLE, 1, 8);
    map_set(&langMap, "test", &a, 0);
    double *ret = map_get(&langMap, "test");
    printf("%x %x = %lf\r\n", &a, ret, *ret);
#else
    //1.float����:����ͬdouble
    //2.void *����:�������һ���Ǳ����ַ,����map_init���һ������Ϊ0
#endif
    return 0;
}

