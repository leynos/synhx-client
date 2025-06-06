/* sha.c - Implementation of the Secure Hash Algorithm
 *
 * Copyright (C) 1995, A.M. Kuchling
 *
 * Distribute and use freely; there are no restrictions on further
 * dissemination and usage except those imposed by the laws of your
 * country of residence.
 *

void sha_copy(struct sha_ctx *dest, struct sha_ctx *src) {
  dest->count_l = src->count_l;
  dest->count_h = src->count_h;
  for (i = 0; i < SHA_DIGESTLEN; i++)
    dest->digest[i] = src->digest[i];
  for (i = 0; i < src->index; i++)
#define f1(x, y, z) (z ^ (x & (y ^ z))) /* Rounds  0-19 */
#define f2(x, y, z) (x ^ y ^ z)         /* Rounds 20-39 */
#define f3(x, y, z) ((x & y) | (z & (x | y))) /* Rounds 40-59 */
#define f4(x, y, z) (x ^ y ^ z)               /* Rounds 60-79 */

#define K1 0x5A827999L /* Rounds  0-19 */
#define K2 0x6ED9EBA1L /* Rounds 20-39 */
#define K3 0x8F1BBCDCL /* Rounds 40-59 */
#define K4 0xCA62C1D6L /* Rounds 60-79 */
#define h0init 0x67452301L
#define h1init 0xEFCDAB89L
#define h2init 0x98BADCFEL
#define h3init 0x10325476L
#define h4init 0xC3D2E1F0L
#define ROTL(n, X) (((X) << (n)) | ((X) >> (32 - (n))))

#define expand(W, i)                                                           \
  (W[i & 15] = ROTL(                                                           \
       1, (W[i & 15] ^ W[(i - 14) & 15] ^ W[(i - 8) & 15] ^ W[(i - 3) & 15])))
#define subRound(a, b, c, d, e, f, k, data)                                    \
  (e += ROTL(5, a) + f(b, c, d) + k + data, b = ROTL(30, b))
void sha_init(struct sha_ctx *ctx) {
  ctx->digest[0] = h0init;
  ctx->digest[1] = h1init;
  ctx->digest[2] = h2init;
  ctx->digest[3] = h3init;
  ctx->digest[4] = h4init;


static void sha_transform(struct sha_ctx *ctx, u_int32_t *data) {
  u_int32_t A, B, C, D, E; /* Local vars */
  subRound(A, B, C, D, E, f1, K1, data[0]);
  subRound(E, A, B, C, D, f1, K1, data[1]);
  subRound(D, E, A, B, C, f1, K1, data[2]);
  subRound(C, D, E, A, B, f1, K1, data[3]);
  subRound(B, C, D, E, A, f1, K1, data[4]);
  subRound(A, B, C, D, E, f1, K1, data[5]);
  subRound(E, A, B, C, D, f1, K1, data[6]);
  subRound(D, E, A, B, C, f1, K1, data[7]);
  subRound(C, D, E, A, B, f1, K1, data[8]);
  subRound(B, C, D, E, A, f1, K1, data[9]);
  subRound(A, B, C, D, E, f1, K1, data[10]);
  subRound(E, A, B, C, D, f1, K1, data[11]);
  subRound(D, E, A, B, C, f1, K1, data[12]);
  subRound(C, D, E, A, B, f1, K1, data[13]);
  subRound(B, C, D, E, A, f1, K1, data[14]);
  subRound(A, B, C, D, E, f1, K1, data[15]);
  subRound(E, A, B, C, D, f1, K1, expand(data, 16));
  subRound(D, E, A, B, C, f1, K1, expand(data, 17));
  subRound(C, D, E, A, B, f1, K1, expand(data, 18));
  subRound(B, C, D, E, A, f1, K1, expand(data, 19));

  subRound(A, B, C, D, E, f2, K2, expand(data, 20));
  subRound(E, A, B, C, D, f2, K2, expand(data, 21));
  subRound(D, E, A, B, C, f2, K2, expand(data, 22));
  subRound(C, D, E, A, B, f2, K2, expand(data, 23));
  subRound(B, C, D, E, A, f2, K2, expand(data, 24));
  subRound(A, B, C, D, E, f2, K2, expand(data, 25));
  subRound(E, A, B, C, D, f2, K2, expand(data, 26));
  subRound(D, E, A, B, C, f2, K2, expand(data, 27));
  subRound(C, D, E, A, B, f2, K2, expand(data, 28));
  subRound(B, C, D, E, A, f2, K2, expand(data, 29));
  subRound(A, B, C, D, E, f2, K2, expand(data, 30));
  subRound(E, A, B, C, D, f2, K2, expand(data, 31));
  subRound(D, E, A, B, C, f2, K2, expand(data, 32));
  subRound(C, D, E, A, B, f2, K2, expand(data, 33));
  subRound(B, C, D, E, A, f2, K2, expand(data, 34));
  subRound(A, B, C, D, E, f2, K2, expand(data, 35));
  subRound(E, A, B, C, D, f2, K2, expand(data, 36));
  subRound(D, E, A, B, C, f2, K2, expand(data, 37));
  subRound(C, D, E, A, B, f2, K2, expand(data, 38));
  subRound(B, C, D, E, A, f2, K2, expand(data, 39));

  subRound(A, B, C, D, E, f3, K3, expand(data, 40));
  subRound(E, A, B, C, D, f3, K3, expand(data, 41));
  subRound(D, E, A, B, C, f3, K3, expand(data, 42));
  subRound(C, D, E, A, B, f3, K3, expand(data, 43));
  subRound(B, C, D, E, A, f3, K3, expand(data, 44));
  subRound(A, B, C, D, E, f3, K3, expand(data, 45));
  subRound(E, A, B, C, D, f3, K3, expand(data, 46));
  subRound(D, E, A, B, C, f3, K3, expand(data, 47));
  subRound(C, D, E, A, B, f3, K3, expand(data, 48));
  subRound(B, C, D, E, A, f3, K3, expand(data, 49));
  subRound(A, B, C, D, E, f3, K3, expand(data, 50));
  subRound(E, A, B, C, D, f3, K3, expand(data, 51));
  subRound(D, E, A, B, C, f3, K3, expand(data, 52));
  subRound(C, D, E, A, B, f3, K3, expand(data, 53));
  subRound(B, C, D, E, A, f3, K3, expand(data, 54));
  subRound(A, B, C, D, E, f3, K3, expand(data, 55));
  subRound(E, A, B, C, D, f3, K3, expand(data, 56));
  subRound(D, E, A, B, C, f3, K3, expand(data, 57));
  subRound(C, D, E, A, B, f3, K3, expand(data, 58));
  subRound(B, C, D, E, A, f3, K3, expand(data, 59));

  subRound(A, B, C, D, E, f4, K4, expand(data, 60));
  subRound(E, A, B, C, D, f4, K4, expand(data, 61));
  subRound(D, E, A, B, C, f4, K4, expand(data, 62));
  subRound(C, D, E, A, B, f4, K4, expand(data, 63));
  subRound(B, C, D, E, A, f4, K4, expand(data, 64));
  subRound(A, B, C, D, E, f4, K4, expand(data, 65));
  subRound(E, A, B, C, D, f4, K4, expand(data, 66));
  subRound(D, E, A, B, C, f4, K4, expand(data, 67));
  subRound(C, D, E, A, B, f4, K4, expand(data, 68));
  subRound(B, C, D, E, A, f4, K4, expand(data, 69));
  subRound(A, B, C, D, E, f4, K4, expand(data, 70));
  subRound(E, A, B, C, D, f4, K4, expand(data, 71));
  subRound(D, E, A, B, C, f4, K4, expand(data, 72));
  subRound(C, D, E, A, B, f4, K4, expand(data, 73));
  subRound(B, C, D, E, A, f4, K4, expand(data, 74));
  subRound(A, B, C, D, E, f4, K4, expand(data, 75));
  subRound(E, A, B, C, D, f4, K4, expand(data, 76));
  subRound(D, E, A, B, C, f4, K4, expand(data, 77));
  subRound(C, D, E, A, B, f4, K4, expand(data, 78));
  subRound(B, C, D, E, A, f4, K4, expand(data, 79));
#define EXTRACT_UCHAR(p) (*(unsigned char *)(p))
#define STRING2INT(s)                                                          \
  ((((((EXTRACT_UCHAR(s) << 8) | EXTRACT_UCHAR(s + 1)) << 8) |                 \
     EXTRACT_UCHAR(s + 2))                                                     \
    << 8) |                                                                    \
   EXTRACT_UCHAR(s + 3))
u_int32_t STRING2INT(u_int8_t *s) {

static void sha_block(struct sha_ctx *ctx, u_int8_t *block) {

  for (i = 0; i < SHA_DATALEN; i++, block += 4)

void sha_update(struct sha_ctx *ctx, u_int8_t *buffer, u_int32_t len) {
  if (ctx->index) { /* Try to fill partial block */
    unsigned left = SHA_DATASIZE - ctx->index;
    if (len < left) {
      memcpy(ctx->block + ctx->index, buffer, len);
      ctx->index += len;
      return; /* Finished */
    } else {
      memcpy(ctx->block + ctx->index, buffer, left);
      sha_block(ctx, ctx->block);
      buffer += left;
      len -= left;
  }
  while (len >= SHA_DATASIZE) {
    sha_block(ctx, buffer);
    buffer += SHA_DATASIZE;
    len -= SHA_DATASIZE;
  }
  if ((ctx->index = len)) /* This assignment is intended */
void sha_digest(struct sha_ctx *ctx, u_int8_t *s) {
  for (i = 0; i < SHA_DIGESTLEN; i++) {
    *s++ = ctx->digest[i] >> 24;
    *s++ = 0xff & (ctx->digest[i] >> 16);
    *s++ = 0xff & (ctx->digest[i] >> 8);
    *s++ = 0xff & ctx->digest[i];
  }

void sha_final(u_int8_t *s, struct sha_ctx *ctx) {

  for (; i & 3; i++)
    data[i] = STRING2INT(ctx->block + 4 * i);

  if (words > (SHA_DATALEN - 2)) { /* No room for length in this block. Process
                                    * it and pad with another one */
    for (i = words; i < SHA_DATALEN; i++)
      data[i] = 0;
    sha_transform(ctx, data);
    for (i = 0; i < (SHA_DATALEN - 2); i++)
      data[i] = 0;
  } else
    for (i = words; i < SHA_DATALEN - 2; i++)
  data[SHA_DATALEN - 2] = (ctx->count_h << 9) | (ctx->count_l >> 23);
  data[SHA_DATALEN - 1] = (ctx->count_l << 9) | (ctx->index << 3);
int sha_fd(int fd, size_t maxlen, u_int8_t *md) {
  struct sha_ctx ctx;
  char buffer[BLOCKSIZE + 72];
  size_t sum;

  sha_init(&ctx);

  for (;;) {
    ssize_t n;

    sum = 0;

    if (maxlen) {
      do {
        n = read(fd, buffer + sum,
                 (BLOCKSIZE - sum) > maxlen ? maxlen : (BLOCKSIZE - sum));
        sum += n;
        maxlen -= n;
        if (!maxlen)
          goto add_last;
      } while (sum < BLOCKSIZE && n > 0);
    } else {
      do {
        n = read(fd, buffer + sum, BLOCKSIZE - sum);
        sum += n;
      } while (sum < BLOCKSIZE && n > 0);
    }

    if (n == -1) {
      if (errno == EINTR)
        continue;
      return 1;
    }

    if (n == 0)
      break;

    sha_update(&ctx, buffer, sum);
  }
  if (sum > 0)
    sha_update(&ctx, buffer, sum);
  sha_final(md, &ctx);
  return 0;
  ctx->digest[1] += B;
  ctx->digest[2] += C;
  ctx->digest[3] += D;
  ctx->digest[4] += E;
}

#if 1

#ifndef EXTRACT_UCHAR
#define EXTRACT_UCHAR(p)  (*(unsigned char *)(p))
#endif

#define STRING2INT(s) ((((((EXTRACT_UCHAR(s) << 8)    \
			 | EXTRACT_UCHAR(s+1)) << 8)  \
			 | EXTRACT_UCHAR(s+2)) << 8)  \
			 | EXTRACT_UCHAR(s+3))
#else
u_int32_t STRING2INT(u_int8_t *s)
{
  u_int32_t r;
  int i;
  
  for (i = 0, r = 0; i < 4; i++, s++)
    r = (r << 8) | *s;
  return r;
}
#endif

static void sha_block(struct sha_ctx *ctx, u_int8_t *block)
{
  u_int32_t data[SHA_DATALEN];
  int i;
  
  /* Update block count */
  if (!++ctx->count_l)
    ++ctx->count_h;

  /* Endian independent conversion */
  for (i = 0; i<SHA_DATALEN; i++, block += 4)
    data[i] = STRING2INT(block);

  sha_transform(ctx, data);
}

void sha_update(struct sha_ctx *ctx, u_int8_t *buffer, u_int32_t len)
{
  if (ctx->index)
    { /* Try to fill partial block */
      unsigned left = SHA_DATASIZE - ctx->index;
      if (len < left)
	{
	  memcpy(ctx->block + ctx->index, buffer, len);
	  ctx->index += len;
	  return; /* Finished */
	}
      else
	{
	  memcpy(ctx->block + ctx->index, buffer, left);
	  sha_block(ctx, ctx->block);
	  buffer += left;
	  len -= left;
	}
    }
  while (len >= SHA_DATASIZE)
    {
      sha_block(ctx, buffer);
      buffer += SHA_DATASIZE;
      len -= SHA_DATASIZE;
    }
  if ((ctx->index = len))     /* This assignment is intended */
    /* Buffer leftovers */
    memcpy(ctx->block, buffer, len);
}

void sha_digest(struct sha_ctx *ctx, u_int8_t *s)
{
  int i;

  for (i = 0; i < SHA_DIGESTLEN; i++)
    {
      *s++ =         ctx->digest[i] >> 24;
      *s++ = 0xff & (ctx->digest[i] >> 16);
      *s++ = 0xff & (ctx->digest[i] >> 8);
      *s++ = 0xff &  ctx->digest[i];
    }
}
	  
/* Final wrapup - pad to SHA_DATASIZE-byte boundary with the bit pattern
   1 0* (64-bit count of bits processed, MSB-first) */

void sha_final(u_int8_t *s, struct sha_ctx *ctx)
{
  u_int32_t data[SHA_DATALEN];
  int i;
  int words;
  
  i = ctx->index;
  /* Set the first char of padding to 0x80.  This is safe since there is
     always at least one byte free */
  ctx->block[i++] = 0x80;

  /* Fill rest of word */
  for( ; i & 3; i++)
    ctx->block[i] = 0;

  /* i is now a multiple of the word size 4 */
  words = i >> 2;
  for (i = 0; i < words; i++)
    data[i] = STRING2INT(ctx->block + 4*i);
  
  if (words > (SHA_DATALEN-2))
    { /* No room for length in this block. Process it and
       * pad with another one */
      for (i = words ; i < SHA_DATALEN; i++)
	data[i] = 0;
      sha_transform(ctx, data);
      for (i = 0; i < (SHA_DATALEN-2); i++)
	data[i] = 0;
    }
  else
    for (i = words ; i < SHA_DATALEN - 2; i++)
      data[i] = 0;
  /* Theres 512 = 2^9 bits in one block */
  data[SHA_DATALEN-2] = (ctx->count_h << 9) | (ctx->count_l >> 23);
  data[SHA_DATALEN-1] = (ctx->count_l << 9) | (ctx->index << 3);
  sha_transform(ctx, data);

  sha_digest(ctx, s);
}

int
sha_fd (int fd, size_t maxlen, u_int8_t *md)
{
	struct sha_ctx ctx;
#define BLOCKSIZE 4096
	char buffer[BLOCKSIZE + 72];
	size_t sum;

	sha_init(&ctx);

	for (;;) {
		ssize_t n;

		sum = 0;

		if (maxlen) {
			do {
				n = read(fd, buffer + sum, (BLOCKSIZE - sum) > maxlen ? maxlen : (BLOCKSIZE - sum));
				sum += n;
				maxlen -= n;
				if (!maxlen)
					goto add_last;
			} while (sum < BLOCKSIZE && n > 0);
		} else {
			do {
				n = read(fd, buffer + sum, BLOCKSIZE - sum);
				sum += n;
			} while (sum < BLOCKSIZE && n > 0);
		}

		if (n == -1) {
			if (errno == EINTR)
				continue;
			return 1;
		}

		if (n == 0)
			break;

		sha_update(&ctx, buffer, sum);
	}

add_last:
	if (sum > 0)
		sha_update(&ctx, buffer, sum);

	sha_final(md, &ctx);

	return 0;
}
