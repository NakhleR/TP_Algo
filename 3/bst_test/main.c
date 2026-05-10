#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"
#include "op.h"
#include "scale.h"

#define VALUE_MAX 9999

#define VALUE_LITT "X"

#define BST   "BST"
#define BST_L "Binary search tree"

typedef struct {
  scale *s;
  bst *t;
  bool verbose;
  opvalue refarray;
} cntxt;

int get(size_t index, cntxt *context);

opreturn add_eop(cntxt *context);
opreturn rem_cul(cntxt *context);
opreturn number(cntxt *context);
opreturn presence(cntxt *context);
opreturn rank(cntxt *context);
opreturn distance(cntxt *context);
opreturn repr_graphic(cntxt *context);
opreturn height(cntxt *context);
opreturn size(cntxt *context);
opreturn verbose_switch(cntxt *context);
opreturn verbose_status(cntxt *context);

#define DEOPI_VALUE(op, actpre, actpost, fun)                                  \
  {                                                                            \
    .oper = op,                                                                \
    .help = actpre " " VALUE_LITT " " actpost,                                 \
    .param = {                                                                 \
      { .lbl = VALUE_LITT, .get = (int (*)(size_t, void *)) get },             \
    },                                                                         \
    .exec = (opreturn (*)(void *)) fun,                                        \
  }

#define DEOPI_VALUE_DISPLAY_ID(op, fun)                                        \
  DEOPI_VALUE(op, "display the " #fun " of", "in the " BST, fun)

#define DEOPI_BT(op, act, fun)                                                 \
  {                                                                            \
    .oper = op,                                                                \
    .help = act,                                                               \
    .param = {                                                                 \
      { .lbl = nullptr, .get = nullptr },                                      \
    },                                                                         \
    .exec = (opreturn (*)(void *)) fun,                                        \
  }

#define DEOPI_BT_DISPLAY_ID(op, fun)                                           \
  DEOPI_BT(op, "display the " #fun " of the " BST, fun)

#define OPER_HELP "?"

opitem itemarray[] = {
  DEOPI_VALUE('+', "add", "to the " BST, add_eop),
  DEOPI_VALUE('-', "remove", "from the " BST, rem_cul),
  DEOPI_VALUE_DISPLAY_ID('n', number),
  DEOPI_VALUE_DISPLAY_ID('p', presence),
  DEOPI_VALUE_DISPLAY_ID('r', rank),
  DEOPI_BT_DISPLAY_ID('d', distance),
  DEOPI_BT('g',
      "display a graphic representation of the " BST " obtained by a"
      " right-left infix depth-first traversal displayed from top to bottom."
      " Each node value is followed by the height of the associated subtree",
      repr_graphic),
  DEOPI_BT_DISPLAY_ID('h', height),
  DEOPI_BT_DISPLAY_ID('s', size),
  DEOPI_BT('V',
      "verbose switch. When on, each successful add or remove is followed by a"
      " graphic representation of the " BST ". The switch is off initially",
      verbose_switch),
  DEOPI_BT('v', "display verbose status", verbose_status),
  DEOPI_BT(OPER_HELP[0], "display this help", OP_ITEM_EXEC_HELP),
  DEOPI_BT('q', "quit", OP_ITEM_EXEC_EXIT),
};

int open(cntxt *context) {
  context->s = scale_open(VALUE_MAX);
  context->t = bst_empty(scale_compar);
  context->verbose = 0;
  if (context->s == nullptr || context->t == nullptr) {
    return OP_RETURN_CAPACITY_ERROR;
  }
  return OP_RETURN_SUCCESS;
}

void close(cntxt *context) {
  scale_close(&context->s);
  bst_dispose(&context->t);
  OP_MSG("%s", "May the " BST "-force be with you!");
}

#define STR(s)  #s
#define XSTR(s) STR(s)

#define TAB1  2
#define TAB2  8
#define TAB2_SPACES_BEFORE  2
#define TAB2_SPACES_AFTER   0

#define PROMPT "> "

const char *preprompt[] = {
  "Type '" OPER_HELP "' after the prompt '" PROMPT "' for help.",
  nullptr
};

const char *posthelp[] = {
  "Values are positive integers less than or equal to " XSTR(VALUE_MAX) ".",
  "When " VALUE_LITT " is not in the " BST ", the number and the rank"
  " displayed are those " VALUE_LITT " would have if added to the " BST ".",
  nullptr
};

#define OPT__SHORT  "-"
#define OPT__LONG   "--"
#define OPT_HELP_L  OPT__LONG "help"

int main(int argc, char *argv[]) {
  for (int k = 1; k < argc; ++k) {
    char *a = argv[k];
    if (strcmp(a, OPT__LONG) == 0) {
      if (k + 1 < argc) {
        ++k;
      }
    } else if (strlen(a) > strlen(OPT__LONG)
        && strncmp(a, OPT_HELP_L, strlen(a)) == 0) {
      printf("Usage: %s\n", argv[0]);
      printf(BST_L " on integers game.\n");
      return EXIT_SUCCESS;
    }
  }
  if (argc != 1) {
    fprintf(stderr, "Illegal number of parameters or unrecognized option.\n");
    fprintf(stderr, "Try '%s %s'.\n", argv[0], OPT_HELP_L);
    return EXIT_FAILURE;
  }
  cntxt ct;
  return op_execute(
        (opreturn (*)(void *)) open, (void (*)(void *)) close,
        itemarray, sizeof itemarray / sizeof *itemarray,
        TAB1, TAB2, TAB2_SPACES_BEFORE, TAB2_SPACES_AFTER,
        PROMPT,
        preprompt, posthelp,
        &ct);
}

int get(size_t index, cntxt *context) {
  return scale_get(context->s, &(context->refarray[index]));
}

void put(const void *p) {
  int x;
  scale_value(p, &x);
  printf("%d", x);
}

#define REF_VALUE(index, context, ref, x)                                      \
  const void *ref = context->refarray[index];                                  \
  int x;                                                                       \
  scale_value(ref, &x)

opreturn add_eop(cntxt *context) {
  REF_VALUE(0, context, ref, x);
  //OP_MSG("add '%d' to the " BST, x);
  //void *rs = bst_search(context->t, ref);
  //void *ra = bst_add_endofpath(context->t, ref);
  //if (rs == nullptr) {
  //if (ra == nullptr) {
  //return OP_RETURN_CAPACITY_ERROR;
  //}
  //if (ra != ref
  //|| bst_search(context->t, ref) != ref) {
  //OP_ERR(BST " module badly implemented (%s? %s?)\n\t%s",
  //"search", "add", "when trying to add a new value");
  //return OP_RETURN_EXIT;
  //}
  //if (context->verbose) {
  //bst_repr_graphic(context->t, put);
  //}
  //} else {
  //if (ra == nullptr
  //|| scale_compar(ra, rs) != 0
  //|| (rs = bst_search(context->t, ref)) == nullptr
  //|| scale_compar(ra, rs) != 0) {
  //OP_ERR(BST " module badly implemented (%s? %s?)\n\t%s",
  //"search", "add", "when trying to add a value already present");
  //return OP_RETURN_EXIT;
  //}
  //OP_WRN("'%d' already in the " BST, x);
  //}
  return OP_RETURN_SUCCESS;
}

opreturn rem_cul(cntxt *context) {
  REF_VALUE(0, context, ref, x);
  //OP_MSG("remove '%d' to the " BST, x);
  //void *rs = bst_search(context->t, ref);
  //void *rr = bst_remove_climbup_left(context->t, ref);
  //if (rs == nullptr) {
  //if (rr != nullptr) {
  //OP_ERR(BST " module badly implemented (%s? %s?)\n\t%s",
  //"search", "remove", "when trying to remove a value not present");
  //return OP_RETURN_EXIT;
  //}
  //OP_WRN("'%d' not in the " BST, x);
  //} else {
  //if (rr == nullptr
  //|| scale_compar(rr, rs) != 0
  //|| bst_search(context->t, ref) != nullptr) {
  //OP_ERR(BST " module badly implemented (%s? %s?)\n\t%s",
  //"search", "remove", "when trying to remove a present value");
  //return OP_RETURN_EXIT;
  //}
  //if (context->verbose) {
  //bst_repr_graphic(context->t, put);
  //}
  //}
  return OP_RETURN_SUCCESS;
}

opreturn number(cntxt *context) {
  REF_VALUE(0, context, ref, x);
  //OP_MSG("number of '%d' in the " BST " = %zu", x,
  //bst_number(context->t, ref));
  return OP_RETURN_SUCCESS;
}

opreturn rank(cntxt *context) {
  REF_VALUE(0, context, ref, x);
  //OP_MSG("rank of '%d' in the " BST " = %zu", x,
  //bst_rank(context->t, ref));
  return OP_RETURN_SUCCESS;
}

opreturn presence(cntxt *context) {
  REF_VALUE(0, context, __ref, __x);
  //void *rs = bst_search(context->t, __ref);
  //if (rs != nullptr && scale_compar(rs, __ref) != 0) {
  //OP_ERR(BST " module badly implemented (%s?)", "search");
  //return OP_RETURN_EXIT;
  //}
  //OP_MSG("presence of '%d' in the " BST " = %s", __x,
  //rs == nullptr ? "false" : "true");
  return OP_RETURN_SUCCESS;
}

#define DEFUN_BT_TO_SIZE_T(fun)                                                \
  opreturn fun(cntxt * context) {                                              \
    OP_MSG(#fun " of the " BST " = %zu", bst_ ## fun(context->t));             \
    return OP_RETURN_SUCCESS;                                                  \
  }

DEFUN_BT_TO_SIZE_T(distance)
DEFUN_BT_TO_SIZE_T(height)
DEFUN_BT_TO_SIZE_T(size)

opreturn repr_graphic([[maybe_unused]] cntxt *context) {
  //bst_repr_graphic(context->t, put);
  return OP_RETURN_SUCCESS;
}

opreturn verbose_switch(cntxt *context) {
  context->verbose = !context->verbose;
  return OP_RETURN_SUCCESS;
}

opreturn verbose_status(cntxt *context) {
  OP_MSG("verbose = %s", context->verbose ? "on" : "off");
  return OP_RETURN_SUCCESS;
}
