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

#define FMT_ZCOD  1
#define FMT_GLUL  2
#define FMT_TAD2  3
#define FMT_TAD3  4
#define FMT_HUGO  5
#define FMT_ALAN  6
#define FMT_ADRI  7
#define FMT_LEVE  8
#define FMT_AGT   9
#define FMT_MAGS 10
#define FMT_ADVS 11
#define FMT_EXEC 12

#define ipad(i) (i + (i % 2))

static int bigendian;

void init_endianness()
{
    int i = 1;
    char *p = (char *)&i;
    bigendian = (p[0] == 0);
    //fprintf(stderr, "bigendian = %d\n", bigendian);
}

/**
 * Read and return a 4-byte big-endian number from stream.
 */
unsigned long flong(FILE *stream)
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
 * Find an executable resource chunk and seek to the start of its content.
 * Return the content length, or EOF if not found.
 * Find the blorb resources chunk and seek to the start of its content.
 * Return the content length, or EOF if not found.
 */
long find_exec(FILE *stream)
{
    char type[5] = "BLAP";
    unsigned long len = 0, read = 0, fmt = 0, limit;

    // first find a "FORM" chunk with record type "IFRS"
    while (!len) {
        fread(type, 4, 1, stream);
        len = flong(stream);
        if (strcmp(type, "FORM") == 0 && len > 4) {
            fread(type, 4, 1, stream);
            len -= 4;
            if (strcmp(type, "IFRS") != 0) {
                fseek(stream, ipad(len), SEEK_CUR);
                len = 0;
            }
        } else {
            fseek(stream, ipad(len), SEEK_CUR);
            len = 0;
        }
        if (feof(stream)) return EOF;
    };

    if (len) {
        limit = len;
        len = 0;
    } else return 0;

    // now find an executable resource chunk within this FORM chunk
    while (!len && read < limit) {
        fread(type, 4, 1, stream);
        len = flong(stream);
        read += 4 + len;

        if      (strcmp(type, "ZCOD") == 0) fmt = FMT_ZCOD;
        else if (strcmp(type, "GLUL") == 0) fmt = FMT_GLUL;
        else if (strcmp(type, "TAD2") == 0) fmt = FMT_TAD2;
        else if (strcmp(type, "TAD3") == 0) fmt = FMT_TAD3;
        else if (strcmp(type, "HUGO") == 0) fmt = FMT_HUGO;
        else if (strcmp(type, "ALAN") == 0) fmt = FMT_ALAN;
        else if (strcmp(type, "ADRI") == 0) fmt = FMT_ADRI;
        else if (strcmp(type, "LEVE") == 0) fmt = FMT_LEVE;
        else if (strcmp(type, "AGT ") == 0) fmt = FMT_AGT ;
        else if (strcmp(type, "MAGS") == 0) fmt = FMT_MAGS;
        else if (strcmp(type, "ADVS") == 0) fmt = FMT_ADVS;
        else if (strcmp(type, "EXEC") == 0) fmt = FMT_EXEC;

        if (!fmt) {
            // not an executable chunk; skip to the next chunk
            fseek(stream, ipad(len), SEEK_CUR);
            read += len;
            len = 0;
        }
        if (feof(stream)) return EOF;
    }
    return len;
}

int main()
{
    char *s;
    unsigned long len, buf;
    init_endianness();

    // find the executable data
    len = find_exec(stdin);
    if (!len || len == EOF) return EXIT_FAILURE;

    // write this data to stdout
    buf = 1024 * 10;
    s = malloc(sizeof(char) * buf);
    while (len) {
        if (buf > len) buf = len;
        fprintf(stderr, "length %lu; reading/writing %lu byte(s)\n", len, buf);
        fread(s, buf, 1, stdin);
        fwrite(s, buf, 1, stdout);
        if (len >= buf) len -= buf;
        else len = 0;
    }

    fprintf(stderr, "DONE\n");
    return EXIT_SUCCESS;
}
