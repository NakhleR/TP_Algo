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
  long int wgt;
  int prev;
  union {
    int next[2];
    int val;
  };
} hitem;

static hitem items[MAX_ITEMS];
static int n_items;

static char codes[SYMBOL_COUNT][SYMBOL_COUNT + 1];
static char code_stack[SYMBOL_COUNT + 1];
static int code_depth;

static void emit_value(FILE *out, int val) {
  for (int b = BITS_PER_VAL - 1; b >= 0; --b) {
    fputc(((val >> b) & 1) ? '1' : '0', out);
  }
}

static void emit_tree(FILE *out, int idx) {
  if (idx < SYMBOL_COUNT) {
    fputc('1', out);
    emit_value(out, items[idx].val);
    return;
  }
  fputc('0', out);
  emit_tree(out, items[idx].next[0]);
  emit_tree(out, items[idx].next[1]);
}

static void assign_codes(int idx) {
  if (idx < SYMBOL_COUNT) {
    code_stack[code_depth] = '\0';
    memcpy(codes[idx], code_stack, (size_t) (code_depth + 1));
    return;
  }
  code_stack[code_depth++] = '0';
  assign_codes(items[idx].next[0]);
  code_depth -= 1;
  code_stack[code_depth++] = '1';
  assign_codes(items[idx].next[1]);
  code_depth -= 1;
}

static int build_tree(void) {
  int active[SYMBOL_COUNT];
  int n_active = 0;
  for (int i = 0; i < SYMBOL_COUNT; ++i) {
    if (items[i].wgt > 0) {
      active[n_active++] = i;
    }
  }
  for (int i = 1; i < n_active; ++i) {
    int x = active[i];
    int j = i;
    while (j > 0 && items[active[j - 1]].wgt > items[x].wgt) {
      active[j] = active[j - 1];
      --j;
    }
    active[j] = x;
  }
  while (n_active > 1) {
    int a = active[0];
    int b = active[1];
    int idx = n_items++;
    items[idx].wgt = items[a].wgt + items[b].wgt;
    items[idx].prev = -1;
    items[idx].next[0] = a;
    items[idx].next[1] = b;
    items[a].prev = idx;
    items[b].prev = idx;
    for (int i = 2; i < n_active; ++i) {
      active[i - 2] = active[i];
    }
    n_active -= 2;
    int j = n_active;
    while (j > 0 && items[active[j - 1]].wgt > items[idx].wgt) {
      active[j] = active[j - 1];
      --j;
    }
    active[j] = idx;
    ++n_active;
  }
  return n_active == 1 ? active[0] : -1;
}

static void print_help(const char *progname) {
  printf("Usage: %s SRC [DEST]\n\n", progname);
  printf("Simulate Huffman compression of input file SRC by producing"
      " digits \"0\" and \"1\".\n");
  printf("The output is written to DEST file if specified, to standard"
      " output otherwise.\n");
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
  for (int i = 0; i < SYMBOL_COUNT; ++i) {
    items[i].wgt = 0;
    items[i].prev = -1;
    items[i].val = i;
  }
  n_items = SYMBOL_COUNT;
  FILE *src = fopen(argv[1], "rb");
  if (src == nullptr) {
    fprintf(stderr, "*** Error: Cannot open %s\n", argv[1]);
    return EXIT_FAILURE;
  }
  int c;
  while ((c = fgetc(src)) != EOF) {
    items[c].wgt += 1;
  }
  if (ferror(src)) {
    fprintf(stderr, "*** Error: read error on %s\n", argv[1]);
    fclose(src);
    return EXIT_FAILURE;
  }
  fclose(src);
  items[EOT_INDEX].wgt = 1;
  int root = build_tree();
  if (root < 0) {
    fprintf(stderr, "*** Error: empty alphabet\n");
    return EXIT_FAILURE;
  }
  code_depth = 0;
  if (root < SYMBOL_COUNT) {
    codes[root][0] = '0';
    codes[root][1] = '\0';
  } else {
    assign_codes(root);
  }
  FILE *dst = (argc == 3) ? fopen(argv[2], "wb") : stdout;
  if (dst == nullptr) {
    fprintf(stderr, "*** Error: Cannot open %s\n", argv[2]);
    return EXIT_FAILURE;
  }
  emit_tree(dst, root);
  src = fopen(argv[1], "rb");
  if (src == nullptr) {
    fprintf(stderr, "*** Error: Cannot reopen %s\n", argv[1]);
    if (dst != stdout) {
      fclose(dst);
    }
    return EXIT_FAILURE;
  }
  while ((c = fgetc(src)) != EOF) {
    fputs(codes[c], dst);
  }
  if (ferror(src)) {
    fprintf(stderr, "*** Error: read error on %s\n", argv[1]);
    fclose(src);
    if (dst != stdout) {
      fclose(dst);
    }
    return EXIT_FAILURE;
  }
  fclose(src);
  fputs(codes[EOT_INDEX], dst);
  if (dst != stdout) {
    fclose(dst);
  }
  return EXIT_SUCCESS;
}
