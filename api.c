
// https://www.draw.io/#Hdoki-nordic%2Fdraw.io%2Fdrafts%2FUntitled%20Diagram.drawio

/* COMMENT */

#ifndef SERIALIZABLE
#define SERIALIZABLE
#endif

/** @brief THE COMMENT
 * 
 * next line
 *    indent
 * 
 * @noop SERIALIZABLE
 * @SERIALIZABLE
 * \noop SERIALIZABLE
 * \SERIALIZABLE
 */
int func(int x, int *buf[])
{
	SERIALIZABLE;
	printf("func: %d\n", x);
}

/* SERIALIZABLE */
int func(int x, int *buf[]);

#ifdef undefthink

#include <stdio.h>

typedef int (*callback_t)(int);
typedef int (*callback_buf_t)(char* buf, int size);

#ifndef SERIALIZE
#define SERIALIZE(...)
#endif

#define _SERIALIZE(...)
#define CALL_CALLBACK(type, callback, ...) (callback)(__VA_ARGS__)

#define CALL_CALLBACK_NO_ARG(type, callback) type#_CALL(callback)
#define CALL_CALLBACK(type, callback, ...) type#_CALL(__VA_ARGS__, callback)

int func(int x)
{
	SERIALIZE();
	printf("func: %d\n", x);
}

SERIALIZE(CALLBACK(callback_t));

SERIALIZE(CALLBACK(callback_buf_t));
SERIALIZE(ARRAY_SIZE(buf, size));
SERIALIZE(OUT(buf));


int callback_test(callback_t callback, int x)
{
	return CALL_CALLBACK(callback_t, callback, x);
}


int buftest(int *arr, int size)
{
	//SERIALIZE(ARRAY_SIZE(arr, size));
	//SERIALIZE(OUT(arr));
}

#endif

/* COMMENT2 */
