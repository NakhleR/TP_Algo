#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ALPHABET_SIZE   (UCHAR_MAX + 1)
#define EOT_INDEX       ALPHABET_SIZE
#define SYMBOL_COUNT    (ALPHABET_SIZE + 1)
#define MAX_ITEMS       (2 * SYMBOL_COUNT - 1)
#define BITS_PER_VAL    9

typedef struct {
  enum {
    NODE,
    LEAF,
    XLEAF
  } mark;
  union {
    int next[2];
    int val;
  };
} hitem;

static hitem items[MAX_ITEMS];
static int n_items;

static int read_bit(FILE *src) {
  int c;
  while ((c = fgetc(src)) != EOF) {
    if (c == '0' || c == '1') {
      return c - '0';
    }
  }
  return -1;
}

static int parse_tree(FILE *src) {
  int b = read_bit(src);
  if (b < 0) {
    return -1;
  }
  int idx = n_items++;
  if (b == 0) {
    items[idx].mark = NODE;
    int l = parse_tree(src);
    if (l < 0) {
      return -1;
    }
    int r = parse_tree(src);
    if (r < 0) {
      return -1;
    }
    items[idx].next[0] = l;
    items[idx].next[1] = r;
    return idx;
  }
  int val = 0;
  for (int i = 0; i < BITS_PER_VAL; ++i) {
    int bit = read_bit(src);
    if (bit < 0) {
      return -1;
    }
    val = (val << 1) | bit;
  }
  items[idx].mark = (val == EOT_INDEX) ? XLEAF : LEAF;
  items[idx].val = val;
  return idx;
}

static void print_help(const char *progname) {
  printf("Usage: %s SRC [DEST]\n\n", progname);
  printf("Simulate Huffman decompression of input file SRC. SRC file shall"
      " have been\nproduced by an Huffman compression simulator and shall"
      " therefore only be\ncomposed of digits \"0\" and \"1\". The output is"
      " written to DEST file if specified,\nto standard output otherwise.\n");
}

int main(int argc, char *argv[]) {
  if (argc >= 2 && strcmp(argv[1], "--help") == 0) {
    print_help(argv[0]);
    return EXIT_SUCCESS;
  }
  if (argc < 2 || argc > 3) {
    fprintf(stderr, "Usage: %s SRC [DEST]\n", argv[0]);
    return EXIT_FAILURE;
  }
  FILE *src = fopen(argv[1], "rb");
  if (src == nullptr) {
    fprintf(stderr, "*** Error: Cannot open %s\n", argv[1]);
    return EXIT_FAILURE;
  }
  n_items = 0;
  int root = parse_tree(src);
  if (root < 0) {
    fprintf(stderr, "*** Error: malformed tree header\n");
    fclose(src);
    return EXIT_FAILURE;
  }
  FILE *dst = (argc == 3) ? fopen(argv[2], "wb") : stdout;
  if (dst == nullptr) {
    fprintf(stderr, "*** Error: Cannot open %s\n", argv[2]);
    fclose(src);
    return EXIT_FAILURE;
  }
  for (;;) {
    int idx = root;
    while (items[idx].mark == NODE) {
      int b = read_bit(src);
      if (b < 0) {
        fprintf(stderr, "*** Error: unexpected end of input\n");
        if (dst != stdout) {
          fclose(dst);
        }
        fclose(src);
        return EXIT_FAILURE;
      }
      idx = items[idx].next[b];
    }
    if (items[idx].mark == XLEAF) {
      break;
    }
    fputc(items[idx].val, dst);
  }
  if (dst != stdout) {
    fclose(dst);
  }
  fclose(src);
  return EXIT_SUCCESS;
}
