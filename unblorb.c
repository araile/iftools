/*
 * UNBLORB
 * Extract the executable data from a Blorb file.
 *
 * Useful info:
 *  - http://www.eblong.com/zarf/blorb/blorb.html
 *  - Wikipedia: "Blorb", "Interchange File Format", "Z-machine"
 *
 * Copyright (c) 2010, inky <http://github.com/inky>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int bigendian;

void ub_init_endianness()
{
    int i = 1;
    char *p = (char *)&i;
    bigendian = (p[0] == 0);
    //fprintf(stderr, "bigendian = %d\n", bigendian);
}

/**
 * Read and return a 4-byte big-endian number from stream.
 */
unsigned long ub_flong(FILE *stream)
{
    unsigned long i, j;
    char *a = (char *)&i, *b = (char *)&j;
    fread(a, 4, 1, stream);
    if (!bigendian) {
        b[0] = a[3];
        b[1] = a[2];
        b[2] = a[1];
        b[3] = a[0];
        return j;
    } else {
        return i;
    }
}

/**
 * Find the first 'FORM' chunk and seek to the start of its content.
 * Return the content length, or EOF if not found.
 */
long ub_find_form(FILE *stream)
{
    char t[4];
    unsigned long len = 0;
    while (1) {
        fread(t, 4, 1, stream);
        printf("%lu\n", ub_flong(stream));
        if (strcmp(t, "FORM") == 0) {
            puts("Found FORM!");
        }
        if (feof(stream)) return EOF;
        break;
    };
    return len;
}

/**
 * And finally...
 */
int main()
{
    ub_init_endianness();

    ub_find_form(stdin);

    return EXIT_SUCCESS;
}
