#include <assert.h>
#include <emmintrin.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "cubehash.h"

#define BUFSIZE  65536

#define ROUNDS      16
#define BLOCKSIZE   32
#define MAXHASHLEN 512

#define ADD  _mm_add_epi32
#define LOAD _mm_loadu_si128
#define ROTL _mm_slli_epi32
#define ROTR _mm_srli_epi32
#define SET  _mm_set_epi32
#define SHUF _mm_shuffle_epi32
#define XOR  _mm_xor_si128

static __m128i* _cubehash(FILE *input, int irounds, int frounds, int hashlen)
{
	uint8_t data[BUFSIZE];
	int datasize, done = false, eof = false, more = true;

	__m128i x0, x1, x2, x3, x4, x5, x6, x7, y0, y1, y2, y3;
	x0 = SET(0, ROUNDS, BLOCKSIZE, hashlen / 8);
	x1 = SET(0, 0, 0, 0);
	x2 = SET(0, 0, 0, 0);
	x3 = SET(0, 0, 0, 0);
	x4 = SET(0, 0, 0, 0);
	x5 = SET(0, 0, 0, 0);
	x6 = SET(0, 0, 0, 0);
	x7 = SET(0, 0, 0, 0);

	datasize = irounds / ROUNDS * BLOCKSIZE;
	memset(data, 0, datasize);

	while (!done)
	{
		__m128i *pos = (__m128i*) data, *end = (__m128i*) &data[datasize];

		while (pos < end)
		{
			x0 = XOR(x0, LOAD(pos++)); x1 = XOR(x1, LOAD(pos++));

			for (int i = 0; i < ROUNDS; i++)
			{
				x4 = ADD(x0, SHUF(x4, 0xb1));
				x5 = ADD(x1, SHUF(x5, 0xb1));
				x6 = ADD(x2, SHUF(x6, 0xb1));
				x7 = ADD(x3, SHUF(x7, 0xb1));
				y0 = x2; y1 = x3; y2 = x0; y3 = x1;
				x0 = XOR(ROTL(y0, 7), ROTR(y0, 25));
				x1 = XOR(ROTL(y1, 7), ROTR(y1, 25));
				x2 = XOR(ROTL(y2, 7), ROTR(y2, 25));
				x3 = XOR(ROTL(y3, 7), ROTR(y3, 25));
				x0 = XOR(x0, x4);
				x1 = XOR(x1, x5);
				x2 = XOR(x2, x6);
				x3 = XOR(x3, x7);

				x4 = ADD(x0, SHUF(x4, 0x4e));
				x5 = ADD(x1, SHUF(x5, 0x4e));
				x6 = ADD(x2, SHUF(x6, 0x4e));
				x7 = ADD(x3, SHUF(x7, 0x4e));
				y0 = x1; y1 = x0; y2 = x3; y3 = x2;
				x0 = XOR(ROTL(y0, 11), ROTR(y0, 21));
				x1 = XOR(ROTL(y1, 11), ROTR(y1, 21));
				x2 = XOR(ROTL(y2, 11), ROTR(y2, 21));
				x3 = XOR(ROTL(y3, 11), ROTR(y3, 21));
				x0 = XOR(x0, x4);
				x1 = XOR(x1, x5);
				x2 = XOR(x2, x6);
				x3 = XOR(x3, x7);
			}
		}

		done = !more;

		if (more)
		{
			if (eof)
			{
				datasize = frounds / ROUNDS * BLOCKSIZE;
				memset(data, 0, datasize);
				x7 = XOR(x7, SET(0, 1, 0, 0));
				more = false;
			}
			else
			{
				datasize = fread(data, 1, BUFSIZE, input);

				if (datasize < BUFSIZE)
				{
					int padsize = BLOCKSIZE - datasize % BLOCKSIZE;
					memset(&data[datasize], 0, padsize);
					data[datasize] = 0x80;
					datasize += padsize;
					eof = true;
				}
			}
		}
	}

	__m128i *hash = malloc(MAXHASHLEN / 8);
	hash[0] = x0; hash[1] = x1; hash[2] = x2; hash[3] = x3;
	return hash;
}

uint8_t * cubehash(FILE *input, int revision, int hashlen)
{
	switch (hashlen && hashlen <= MAXHASHLEN && hashlen % 8 == 0 ? revision : 0)
	{
		case 3:
			return (uint8_t *) _cubehash(input,  16,  32, hashlen);
		case 2:
			return (uint8_t *) _cubehash(input, 160, 160, hashlen);
		case 0:
		default:
			errno = EDOM;
			return NULL;
	}
}
