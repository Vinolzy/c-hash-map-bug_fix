#include <stdio.h>
#include <stdlib.h>
#include "map.h"

#define TEST_MODE 3

static map_c_t langMap;
int main()
{
#if (TEST_MODE == 1)       //字符串测试:拷贝字符串地址[常用]
    map_init(&langMap, MAP_TYPE_CHAR_PTR, 0, 8);
    map_set(&langMap, "test", "1234", 0);
    char **ret = map_get(&langMap, "test");
    printf("%x %x = %s\r\n", "1234", *ret, *ret);
#elif (TEST_MODE == 2)     //字符串测试:拷贝字符串的值map_node_t后面的内存中(需要指定长度)
    map_init(&langMap, MAP_TYPE_CHAR_PTR, 1, 8);
    map_set(&langMap, "test", "1234", sizeof("1234"));
    char *ret = map_get(&langMap, "test");
    printf("%x %x = %s\r\n", "1234", ret, ret);
#elif (TEST_MODE == 3)     //int测试:保存数字的值到map_node_t后[常用]
    map_init(&langMap, MAP_TYPE_INT, 0, 8);
    map_set(&langMap, "test", 123, 0);
    int *ret = map_get(&langMap, "test");
    printf("%x = %d\r\n", *ret, *ret);
#elif (TEST_MODE == 4)     //int测试:拷贝int变量的值到map_node_t后
    const int a = 123;
    map_init(&langMap, MAP_TYPE_INT, 1, 8);
    map_set(&langMap, "test", &a, 0);
    int *ret = map_get(&langMap, "test");
    printf("%x %x = %d\r\n", &a, *ret, *ret);
#elif (TEST_MODE == 5)     //int测试:保存int变量的地址
    const int a = 123;
    map_init(&langMap, MAP_TYPE_INT, 0, 8);
    map_set(&langMap, "test", &a, 0);
    int **ret = map_get(&langMap, "test");
    printf("%x %x = %d\r\n", &a, *ret, **ret);
#elif (TEST_MODE == 6)     //char测试:拷贝字符的值到map_node_t后[常用]
    map_init(&langMap, MAP_TYPE_CHAR, 0, 8);
    map_set(&langMap, "test", 'a', 0);
    char *ret = map_get(&langMap, "test");
    printf("%x = %c\r\n", *ret, *ret);
#elif (TEST_MODE == 7)     //double测试:保存double变量地址到map_node_t后
    const double a = 3.14;
    map_init(&langMap, MAP_TYPE_DOUBLE, 0, 8);
    map_set(&langMap, "test", &a, 0);
    double **ret = map_get(&langMap, "test");
    printf("%x %x = %lf\r\n", &a, *ret, **ret);
#elif (TEST_MODE == 8)     //double测试:拷贝double变量的值到map_node_t后
    const double a = 3.14;
    map_init(&langMap, MAP_TYPE_DOUBLE, 1, 8);
    map_set(&langMap, "test", &a, 0);
    double *ret = map_get(&langMap, "test");
    printf("%x %x = %lf\r\n", &a, ret, *ret);
#else
    //1.float类型:代码同double
    //2.void *类型:这种情况一般是保存地址,所以map_init最后一个参数为0
#endif
    return 0;
}

