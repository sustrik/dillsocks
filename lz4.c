/*

  Copyright (c) 2016 Martin Sustrik

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "lz4/lz4.h"

#include "bsock.h"
#include "dsock.h"
#include "utils.h"

static const int lz4_type_placeholder = 0;
static const void *lz4_type = &lz4_type_placeholder;
static void lz4_close(int s);
static int lz4_bsend(int s, const void *buf, size_t len,
    int64_t deadline);
static int lz4_brecv(int s, void *buf, size_t len,
    int64_t deadline);

struct lz4sock {
    struct bsockvfptrs vfptrs;
    int s;
};

int lz4attach(int s) {
    /* Check whether underlying socket is a bytestream. */
    if(dsock_slow(!hdata(s, bsock_type))) return -1;
    /* Create the object. */
    struct lz4sock *obj = malloc(sizeof(struct lz4sock));
    if(dsock_slow(!obj)) {errno = ENOMEM; return -1;}
    obj->vfptrs.hvfptrs.close = lz4_close;
    obj->vfptrs.type = lz4_type;
    obj->vfptrs.bsend = lz4_bsend;
    obj->vfptrs.brecv = lz4_brecv;
    obj->s = s;
    /* Create the handle. */
    int h = handle(bsock_type, obj, &obj->vfptrs.hvfptrs);
    if(dsock_slow(h < 0)) {
        int err = errno;
        free(obj);
        errno = err;
        return -1;
    }
    return h;
}

int lz4detach(int s) {
    struct lz4sock *obj = hdata(s, bsock_type);
    if(dsock_slow(obj && obj->vfptrs.type != lz4_type)) {
        errno = ENOTSUP; return -1;}
    int u = obj->s;
    free(obj);
    return u;
}

static int lz4_bsend(int s, const void *buf, size_t len,
      int64_t deadline) {
    struct lz4sock *obj = hdata(s, bsock_type);
    dsock_assert(obj->vfptrs.type == lz4_type);
    dsock_assert(0);
}

static int lz4_brecv(int s, void *buf, size_t len,
      int64_t deadline) {
    struct lz4sock *obj = hdata(s, bsock_type);
    dsock_assert(obj->vfptrs.type == lz4_type);
    dsock_assert(0);
} 

static void lz4_close(int s) {
    struct lz4sock *obj = hdata(s, bsock_type);
    dsock_assert(obj && obj->vfptrs.type == lz4_type);
    int rc = hclose(obj->s);
    dsock_assert(rc == 0);
    free(obj);
}
