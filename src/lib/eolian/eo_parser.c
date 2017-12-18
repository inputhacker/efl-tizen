#include <assert.h>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "eo_parser.h"
#include "eolian_priv.h"

#define CASE_LOCK(ls, var, msg) \
   if (has_##var) \
     eo_lexer_syntax_error(ls, "double " msg); \
   has_##var = EINA_TRUE;

#define FILL_BASE(exp, ls, l, c) \
   (exp).file = eina_stringshare_ref(ls->filename); \
   (exp).line = l; \
   (exp).column = c;

#define FILL_DOC(ls, def, docf) \
   if (ls->t.token == TOK_DOC) \
     { \
        def->docf = ls->t.value.doc; \
        ls->t.value.doc = NULL; \
        eo_lexer_get(ls); \
     }

static void
error_expected(Eo_Lexer *ls, int token)
{
   char  buf[256];
   char tbuf[256];
   eo_lexer_token_to_str(token, tbuf);
   snprintf(buf, sizeof(buf), "'%s' expected", tbuf);
   eo_lexer_syntax_error(ls, buf);
}

static Eina_Bool
test_next(Eo_Lexer *ls, int token)
{
   if (ls->t.token == token)
     {
        eo_lexer_get(ls);
        return EINA_TRUE;
     }
   return EINA_FALSE;
}

static void
check(Eo_Lexer *ls, int token)
{
   if (ls->t.token != token)
     error_expected(ls, token);
}

static void
check_kw(Eo_Lexer *ls, int kw)
{
   if (ls->t.kw != kw)
     error_expected(ls, TOK_VALUE + kw);
}

static void
check_next(Eo_Lexer *ls, int token)
{
   check(ls, token);
   eo_lexer_get(ls);
}

static void
check_kw_next(Eo_Lexer *ls, int kw)
{
   check_kw(ls, kw);
   eo_lexer_get(ls);
}

static void
check_match(Eo_Lexer *ls, int what, int who, int where, int col)
{
   if (!test_next(ls, what))
     {
        if (where == ls->line_number)
          error_expected(ls, what);
        else
          {
             char  buf[256];
             char tbuf[256];
             char vbuf[256];
             eo_lexer_token_to_str(what, tbuf);
             eo_lexer_token_to_str(who , vbuf);
             snprintf(buf, sizeof(buf),
                      "'%s' expected (to close '%s' at line %d, column %d)",
                      tbuf, vbuf, where, col);
             eo_lexer_syntax_error(ls, buf);
          }
     }
}

static Eina_Strbuf *
push_strbuf(Eo_Lexer *ls)
{
   Eina_Strbuf *buf = eina_strbuf_new();
   ls->tmp.str_bufs = eina_list_prepend(ls->tmp.str_bufs, buf);
   return buf;
}

static void
pop_strbuf(Eo_Lexer *ls)
{
   Eina_Strbuf *buf = eina_list_data_get(ls->tmp.str_bufs);
   eina_strbuf_free(buf);
   ls->tmp.str_bufs = eina_list_remove_list(ls->tmp.str_bufs, ls->tmp.str_bufs);
}

static Eolian_Type *
push_type(Eo_Lexer *ls)
{
   Eolian_Type *def = calloc(1, sizeof(Eolian_Type));
   ls->tmp.type_defs = eina_list_prepend(ls->tmp.type_defs, def);
   return def;
}

static Eolian_Typedecl *
push_typedecl(Eo_Lexer *ls)
{
   Eolian_Typedecl *def = calloc(1, sizeof(Eolian_Typedecl));
   ls->tmp.type_decls = eina_list_prepend(ls->tmp.type_decls, def);
   return def;
}

static void
pop_type(Eo_Lexer *ls)
{
   eolian_object_ref((Eolian_Object *)eina_list_data_get(ls->tmp.type_defs));
   ls->tmp.type_defs = eina_list_remove_list(ls->tmp.type_defs, ls->tmp.type_defs);
}

static void
pop_typedecl(Eo_Lexer *ls)
{
   eolian_object_ref((Eolian_Object *)eina_list_data_get(ls->tmp.type_decls));
   ls->tmp.type_decls = eina_list_remove_list(ls->tmp.type_decls, ls->tmp.type_decls);
}

static Eina_Stringshare *
push_str(Eo_Lexer *ls, const char *val)
{
   Eina_Stringshare *shr = eina_stringshare_add(val);
   ls->tmp.strs = eina_list_prepend(ls->tmp.strs, shr);
   return shr;
}

static void
pop_str(Eo_Lexer *ls)
{
   ls->tmp.strs = eina_list_remove_list(ls->tmp.strs, ls->tmp.strs);
}

static Eina_Bool
compare_class_file(const char *fn1, const char *fn2)
{
   return !strcmp(fn1, fn2);
}

static const char *declnames[] = {
    "class", "type alias", "struct", "enum", "variable"
};

static void
redef_error(Eo_Lexer *ls, Eolian_Declaration *decl, Eolian_Declaration_Type newt)
{
   Eolian_Object *obj = (Eolian_Object *)decl->data;
   char buf[256], fbuf[256] = { '\0' };
   if (ls->filename != obj->file)
     snprintf(fbuf, sizeof(fbuf), "%s:%d:%d", obj->file, obj->line, obj->column);
   else
     snprintf(fbuf, sizeof(fbuf), "%d:%d", obj->line, obj->column);

   if (newt != decl->type)
     snprintf(buf, sizeof(buf), "%s '%s' redefined as %s (originally at %s)",
              declnames[decl->type], decl->name, declnames[newt], fbuf);
   else
     snprintf(buf, sizeof(buf), "%s '%s' redefined (originally at %s)",
              declnames[decl->type], decl->name, fbuf);

   eo_lexer_syntax_error(ls, buf);
}

static Eina_Strbuf *
parse_name(Eo_Lexer *ls, Eina_Strbuf *buf)
{
   check(ls, TOK_VALUE);
   if (eo_lexer_is_type_keyword(ls->t.kw))
     eo_lexer_syntax_error(ls, "invalid name");
   eina_strbuf_reset(buf);
   for (;;)
     {
        eina_strbuf_append(buf, ls->t.value.s);
        eo_lexer_get(ls);
        if (ls->t.token != '.') break;
        eo_lexer_get(ls);
        eina_strbuf_append(buf, ".");
        check(ls, TOK_VALUE);
        if (eo_lexer_is_type_keyword(ls->t.kw))
          eo_lexer_syntax_error(ls, "invalid name");
     }
   return buf;
}

static void
_fill_name(const char *input, Eina_Stringshare **full_name,
           Eina_Stringshare **name, Eina_List **namespaces)
{
   char *fname = strdup(input);
   char *sname = fname, *dot = fname;
   *full_name = input;
   do
     {
        dot = strchr(dot, '.');
        if (dot)
          {
             *dot = '\0';
             *namespaces = eina_list_append(*namespaces,
                                            eina_stringshare_add(sname));
             ++dot;
             sname = dot;
          }
     }
   while (dot);
   *name = eina_stringshare_add(sname);
   free(fname);
}

static Eolian_Expression *
push_expr(Eo_Lexer *ls)
{
   Eolian_Expression *def = calloc(1, sizeof(Eolian_Expression));
   ls->tmp.expr_defs = eina_list_prepend(ls->tmp.expr_defs, def);
   return def;
}

static void
pop_expr(Eo_Lexer *ls)
{
   eolian_object_ref((Eolian_Object *)eina_list_data_get(ls->tmp.expr_defs));
   ls->tmp.expr_defs = eina_list_remove_list(ls->tmp.expr_defs, ls->tmp.expr_defs);
}

static Eolian_Binary_Operator
get_binop_id(int tok)
{
   switch (tok)
     {
      case '+': return EOLIAN_BINOP_ADD;
      case '-': return EOLIAN_BINOP_SUB;
      case '*': return EOLIAN_BINOP_MUL;
      case '/': return EOLIAN_BINOP_DIV;
      case '%': return EOLIAN_BINOP_MOD;

      case TOK_EQ: return EOLIAN_BINOP_EQ;
      case TOK_NQ: return EOLIAN_BINOP_NQ;
      case '>'   : return EOLIAN_BINOP_GT;
      case '<'   : return EOLIAN_BINOP_LT;
      case TOK_GE: return EOLIAN_BINOP_GE;
      case TOK_LE: return EOLIAN_BINOP_LE;

      case TOK_AND: return EOLIAN_BINOP_AND;
      case TOK_OR : return EOLIAN_BINOP_OR;

      case '&': return EOLIAN_BINOP_BAND;
      case '|': return EOLIAN_BINOP_BOR;
      case '^': return EOLIAN_BINOP_BXOR;

      case TOK_LSH: return EOLIAN_BINOP_LSH;
      case TOK_RSH: return EOLIAN_BINOP_RSH;

      default: return EOLIAN_BINOP_INVALID;
     }
}

static Eolian_Unary_Operator
get_unop_id(int tok)
{
   switch (tok)
     {
      case '-': return EOLIAN_UNOP_UNM;
      case '+': return EOLIAN_UNOP_UNP;
      case '!': return EOLIAN_UNOP_NOT;
      case '~': return EOLIAN_UNOP_BNOT;

      default: return EOLIAN_UNOP_INVALID;
     }
}

static const int binprec[] = {
   -1, /* invalid */

   8, /* + */
   8, /* - */
   9, /* * */
   9, /* / */
   9, /* % */

   3, /* == */
   3, /* != */
   3, /* >  */
   3, /* <  */
   3, /* >= */
   3, /* <= */

   2, /* && */
   1, /* || */

   6, /* &  */
   4, /* |  */
   5, /* ^  */
   7, /* << */
   7  /* >> */
};

#define UNARY_PRECEDENCE 10

static Eolian_Expression *parse_expr_bin(Eo_Lexer *ls, int min_prec);
static Eolian_Expression *parse_expr(Eo_Lexer *ls);

static Eolian_Expression *
parse_expr_simple(Eo_Lexer *ls)
{
   Eolian_Expression *expr;
   Eolian_Unary_Operator unop = get_unop_id(ls->t.token);
   if (unop != EOLIAN_UNOP_INVALID)
     {
        int line = ls->line_number, col = ls->column;
        eo_lexer_get(ls);
        Eolian_Expression *exp = parse_expr_bin(ls, UNARY_PRECEDENCE);
        pop_expr(ls);
        expr = push_expr(ls);
        FILL_BASE(expr->base, ls, line, col);
        expr->unop = unop;
        expr->type = EOLIAN_EXPR_UNARY;
        expr->expr = exp;
        return expr;
     }
   switch (ls->t.token)
     {
      case TOK_NUMBER:
        {
           int line = ls->line_number, col = ls->column;
           expr = push_expr(ls);
           FILL_BASE(expr->base, ls, line, col);
           expr->type = ls->t.kw + 1; /* map Numbers from lexer to expr type */
           memcpy(&expr->value, &ls->t.value, sizeof(expr->value));
           eo_lexer_get(ls);
           break;
        }
      case TOK_STRING:
        {
           int line = ls->line_number, col = ls->column;
           expr = push_expr(ls);
           FILL_BASE(expr->base, ls, line, col);
           expr->type = EOLIAN_EXPR_STRING;
           expr->value.s = eina_stringshare_ref(ls->t.value.s);
           eo_lexer_get(ls);
           break;
        }
      case TOK_CHAR:
        {
           int line = ls->line_number, col = ls->column;
           expr = push_expr(ls);
           FILL_BASE(expr->base, ls, line, col);
           expr->type = EOLIAN_EXPR_CHAR;
           expr->value.c = ls->t.value.c;
           eo_lexer_get(ls);
           break;
        }
      case TOK_VALUE:
        {
           int line = ls->line_number, col = ls->column;
           switch (ls->t.kw)
             {
              case KW_true:
              case KW_false:
                {
                   expr = push_expr(ls);
                   expr->type = EOLIAN_EXPR_BOOL;
                   expr->value.b = (ls->t.kw == KW_true);
                   eo_lexer_get(ls);
                   break;
                }
              case KW_null:
                {
                   expr = push_expr(ls);
                   expr->type = EOLIAN_EXPR_NULL;
                   eo_lexer_get(ls);
                   break;
                }
              default:
                {
                   Eina_Strbuf *buf = push_strbuf(ls);
                   expr = push_expr(ls);
                   expr->type = EOLIAN_EXPR_NAME;
                   parse_name(ls, buf);
                   expr->value.s = eina_stringshare_add(eina_strbuf_string_get
                       (buf));
                   pop_strbuf(ls);
                   break;
                }
             }
           FILL_BASE(expr->base, ls, line, col);
           break;
        }
      case '(':
        {
           int line = ls->line_number, col = ls->column;
           eo_lexer_get(ls);
           expr = parse_expr(ls);
           check_match(ls, ')', '(', line, col);
           break;
        }
      default:
        expr = NULL; /* shut up compiler */
        eo_lexer_syntax_error(ls, "unexpected symbol");
        break;
     }

   return expr;
}

static Eolian_Expression *
parse_expr_bin(Eo_Lexer *ls, int min_prec)
{
   int line = ls->line_number, col = ls->column;
   Eolian_Expression *lhs = parse_expr_simple(ls);
   for (;;)
     {
        Eolian_Expression *rhs, *bin;
        Eolian_Binary_Operator op = get_binop_id(ls->t.token);
        int prec = binprec[op];
        if ((op == EOLIAN_BINOP_INVALID) || (prec < 0) || (prec < min_prec))
          break;
        eo_lexer_get(ls);
        rhs = parse_expr_bin(ls, prec + 1);
        pop_expr(ls);
        pop_expr(ls);
        bin = push_expr(ls);
        FILL_BASE(bin->base, ls, line, col);
        bin->binop = op;
        bin->type = EOLIAN_EXPR_BINARY;
        bin->lhs = lhs;
        bin->rhs = rhs;
        lhs = bin;
     }
   return lhs;
}

static Eolian_Expression *
parse_expr(Eo_Lexer *ls)
{
   return parse_expr_bin(ls, 1);
}

static Eolian_Type *parse_type_void(Eo_Lexer *ls);

static Eolian_Type *
parse_type(Eo_Lexer *ls)
{
   Eolian_Type *ret;
   eo_lexer_context_push(ls);
   ret = parse_type_void(ls);
   if (ret->type == EOLIAN_TYPE_VOID)
     {
        eo_lexer_context_restore(ls);
        eo_lexer_syntax_error(ls, "non-void type expected");
     }
   eo_lexer_context_pop(ls);
   return ret;
}

static void
_struct_field_free(Eolian_Struct_Type_Field *def)
{
   if (def->base.file) eina_stringshare_del(def->base.file);
   if (def->name) eina_stringshare_del(def->name);
   database_type_del(def->type);
   database_doc_del(def->doc);
   free(def);
}

static Eolian_Typedecl *
parse_struct(Eo_Lexer *ls, const char *name, Eina_Bool is_extern,
             int line, int column, const char *freefunc)
{
   int bline = ls->line_number, bcolumn = ls->column;
   Eolian_Typedecl *def = push_typedecl(ls);
   def->is_extern = is_extern;
   if (name) _fill_name(name, &def->full_name, &def->name, &def->namespaces);
   def->type = EOLIAN_TYPEDECL_STRUCT;
   def->fields = eina_hash_string_small_new(EINA_FREE_CB(_struct_field_free));
   def->freefunc = freefunc;
   pop_str(ls);
   check_next(ls, '{');
   FILL_DOC(ls, def, doc);
   while (ls->t.token != '}')
     {
        const char *fname;
        Eolian_Struct_Type_Field *fdef;
        Eolian_Type *tp;
        int fline = ls->line_number, fcol = ls->column;
        check(ls, TOK_VALUE);
        if (eina_hash_find(def->fields, ls->t.value.s))
          eo_lexer_syntax_error(ls, "double field definition");
        fdef = calloc(1, sizeof(Eolian_Struct_Type_Field));
        fname = eina_stringshare_ref(ls->t.value.s);
        eina_hash_add(def->fields, fname, fdef);
        def->field_list = eina_list_append(def->field_list, fdef);
        eolian_object_ref(&fdef->base);
        eo_lexer_get(ls);
        check_next(ls, ':');
        tp = parse_type(ls);
        FILL_BASE(fdef->base, ls, fline, fcol);
        fdef->type = tp;
        fdef->name = eina_stringshare_ref(fname);
        pop_type(ls);
        if ((fdef->type->owned = (ls->t.kw == KW_at_owned)))
          eo_lexer_get(ls);
        check_next(ls, ';');
        FILL_DOC(ls, fdef, doc);
     }
   check_match(ls, '}', '{', bline, bcolumn);
   FILL_BASE(def->base, ls, line, column);
   if (name) database_struct_add(ls->state, def);
   return def;
}

static void
_enum_field_free(Eolian_Enum_Type_Field *def)
{
   if (def->base.file) eina_stringshare_del(def->base.file);
   if (def->name) eina_stringshare_del(def->name);
   database_expr_del(def->value);
   database_doc_del(def->doc);
   free(def);
}

static Eolian_Typedecl *
parse_enum(Eo_Lexer *ls, const char *name, Eina_Bool is_extern,
           int line, int column)
{
   int bline = ls->line_number, bcolumn = ls->column;
   Eolian_Typedecl *def = push_typedecl(ls);
   def->is_extern = is_extern;
   _fill_name(name, &def->full_name, &def->name, &def->namespaces);
   def->type = EOLIAN_TYPEDECL_ENUM;
   def->fields = eina_hash_string_small_new(EINA_FREE_CB(_enum_field_free));
   check_next(ls, '{');
   FILL_DOC(ls, def, doc);
   if (ls->t.token == TOK_VALUE && ls->t.kw == KW_legacy)
     {
         if (eo_lexer_lookahead(ls) == ':')
           {
              /* consume keyword */
              eo_lexer_get(ls);
              /* consume colon */
              eo_lexer_get(ls);
              check(ls, TOK_VALUE);
              def->legacy = eina_stringshare_ref(ls->t.value.s);
              eo_lexer_get(ls);
              check_next(ls, ';');
           }
     }
   Eolian_Enum_Type_Field *prev_fl = NULL;
   int fl_nadd = 0;
   for (;;)
     {
        const char *fname;
        Eolian_Enum_Type_Field *fdef;
        int fline = ls->line_number, fcol = ls->column;
        check(ls, TOK_VALUE);
        if (eina_hash_find(def->fields, ls->t.value.s))
          eo_lexer_syntax_error(ls, "double field definition");
        fdef = calloc(1, sizeof(Eolian_Enum_Type_Field));
        fname = eina_stringshare_ref(ls->t.value.s);
        eina_hash_add(def->fields, fname, fdef);
        def->field_list = eina_list_append(def->field_list, fdef);
        eolian_object_ref(&fdef->base);
        eo_lexer_get(ls);
        FILL_BASE(fdef->base, ls, fline, fcol);
        fdef->base_enum = def;
        fdef->name = eina_stringshare_ref(fname);
        if (ls->t.token != '=')
          {
             if (!prev_fl)
               {
                  Eolian_Expression *eexp = push_expr(ls);
                  FILL_BASE(eexp->base, ls, -1, -1);
                  eexp->type = EOLIAN_EXPR_INT;
                  eexp->value.i = 0;
                  fdef->value = eexp;
                  fdef->is_public_value = EINA_TRUE;
                  pop_expr(ls);
                  prev_fl = fdef;
                  fl_nadd = 0;
               }
             else
               {
                  Eolian_Expression *rhs = push_expr(ls),
                                    *bin = push_expr(ls);
                  FILL_BASE(rhs->base, ls, -1, -1);
                  FILL_BASE(bin->base, ls, -1, -1);

                  rhs->type = EOLIAN_EXPR_INT;
                  rhs->value.i = ++fl_nadd;

                  bin->type = EOLIAN_EXPR_BINARY;
                  bin->binop = EOLIAN_BINOP_ADD;
                  bin->lhs = prev_fl->value;
                  bin->rhs = rhs;
                  bin->weak_lhs = EINA_TRUE;

                  fdef->value = bin;
               }
          }
        else
          {
             ls->expr_mode = EINA_TRUE;
             eo_lexer_get(ls);
             fdef->value = parse_expr(ls);
             fdef->is_public_value = EINA_TRUE;
             ls->expr_mode = EINA_FALSE;
             prev_fl = fdef;
             fl_nadd = 0;
             pop_expr(ls);
          }
        Eina_Bool want_next = (ls->t.token == ',');
        if (want_next)
          eo_lexer_get(ls);
        FILL_DOC(ls, fdef, doc);
        if (!want_next || ls->t.token == '}')
          break;
     }
   check_match(ls, '}', '{', bline, bcolumn);
   FILL_BASE(def->base, ls, line, column);
   if (name) database_enum_add(ls->state, def);
   return def;
}

static void
parse_struct_attrs(Eo_Lexer *ls, Eina_Bool is_enum, Eina_Bool *is_extern,
                   const char **freefunc)
{
   Eina_Bool has_extern = EINA_FALSE, has_free = EINA_FALSE;
   *freefunc = NULL;
   *is_extern = EINA_FALSE;
   for (;;) switch (ls->t.kw)
     {
      case KW_at_extern:
        CASE_LOCK(ls, extern, "@extern qualifier")
        eo_lexer_get(ls);
        *is_extern = EINA_TRUE;
        break;
      case KW_at_free:
        {
           CASE_LOCK(ls, free, "@free qualifier")
           if (is_enum)
             eo_lexer_syntax_error(ls, "enums cannot have @free");
           eo_lexer_get(ls);
           int pline = ls->line_number, pcol = ls->column;
           check_next(ls, '(');
           check(ls, TOK_VALUE);
           *freefunc = push_str(ls, ls->t.value.s);
           eo_lexer_get(ls);
           check_match(ls, ')', '(', pline, pcol);
           break;
        }
      default:
        return;
     }
}

static Eolian_Class *
_parse_dep(Eo_Lexer *ls, const char *fname, const char *name)
{
   if (eina_hash_find(ls->state->parsing, fname))
     return NULL;
   Eolian_Class *cl = NULL;
   if (!eo_parser_database_fill(ls->unit, fname, EINA_FALSE, &cl) || !cl)
     {
        char buf[PATH_MAX];
        eo_lexer_context_restore(ls);
        snprintf(buf, sizeof(buf), "error parsing dependency '%s'", name);
        eo_lexer_syntax_error(ls, buf);
     }
   return cl;
}

static Eolian_Type *
parse_type_void(Eo_Lexer *ls)
{
   Eolian_Type *def;
   Eina_Strbuf *buf;
   int line = ls->line_number, col = ls->column;
   switch (ls->t.kw)
     {
      case KW_const:
        {
           int pline, pcol;
           eo_lexer_get(ls);
           pline = ls->line_number;
           pcol = ls->column;
           check_next(ls, '(');
           def = parse_type_void(ls);
           FILL_BASE(def->base, ls, line, col);
           def->is_const = EINA_TRUE;
           check_match(ls, ')', '(', pline, pcol);
           return def;
        }
      case KW_ptr:
        {
           int pline, pcol;
           eo_lexer_get(ls);
           pline = ls->line_number;
           pcol = ls->column;
           check_next(ls, '(');
           def = parse_type_void(ls);
           FILL_BASE(def->base, ls, line, col);
           def->is_ptr = EINA_TRUE;
           check_match(ls, ')', '(', pline, pcol);
           return def;
        }
      case KW_legacy:
        {
           int pline, pcol;
           eo_lexer_get(ls);
           pline = ls->line_number;
           pcol = ls->column;
           check_next(ls, '(');
           def = parse_type_void(ls);
           FILL_BASE(def->base, ls, line, col);
           def->legacy = EINA_TRUE;
           check_match(ls, ')', '(', pline, pcol);
           return def;
        }
      case KW_free:
        {
           int pline, pcolumn;
           eo_lexer_get(ls);
           pline = ls->line_number;
           pcolumn = ls->column;
           check_next(ls, '(');
           def = parse_type_void(ls);
           check_next(ls, ',');
           check(ls, TOK_VALUE);
           def->freefunc = eina_stringshare_ref(ls->t.value.s);
           eo_lexer_get(ls);
           FILL_BASE(def->base, ls, line, col);
           check_match(ls, ')', '(', pline, pcolumn);
           return def;
        }
      default:
        break;
     }
   def = push_type(ls);
   FILL_BASE(def->base, ls, line, col);
   if (ls->t.kw == KW_void)
     {
        def->type = EOLIAN_TYPE_VOID;
        def->btype = EOLIAN_TYPE_BUILTIN_VOID;
        eo_lexer_get(ls);
     }
   else if (ls->t.kw == KW___undefined_type)
     {
        def->type = EOLIAN_TYPE_UNDEFINED;
        eo_lexer_get(ls);
     }
   else
     {
        int tpid = ls->t.kw;
        def->type = EOLIAN_TYPE_REGULAR;
        check(ls, TOK_VALUE);
        if (eo_lexer_is_type_keyword(ls->t.kw))
          {
             def->btype = ls->t.kw - KW_byte + 1;
             _fill_name(eina_stringshare_ref(ls->t.value.s), &def->full_name,
                        &def->name, &def->namespaces);
             eo_lexer_get(ls);
             if (tpid >= KW_accessor && tpid <= KW_future)
               {
                  int bline = ls->line_number, bcol = ls->column;
                  check_next(ls, '<');
                  if (tpid == KW_future)
                    def->base_type = parse_type_void(ls);
                  else
                    def->base_type = parse_type(ls);
                  pop_type(ls);
                  if ((def->base_type->owned = (ls->t.kw == KW_at_owned)))
                    eo_lexer_get(ls);
                  if (tpid == KW_hash)
                    {
                       check_next(ls, ',');
                       def->base_type->next_type = parse_type(ls);
                       pop_type(ls);
                       if ((def->base_type->next_type->owned = (ls->t.kw == KW_at_owned)))
                         eo_lexer_get(ls);
                    }
                  else if((tpid == KW_future) && test_next(ls, ','))
                    {
                       def->base_type->next_type = parse_type_void(ls);
                       pop_type(ls);
                    }
                  check_match(ls, '>', '<', bline, bcol);
               }
          }
        else
          {
             const char *bnm, *nm;
             char *fnm;
             buf = push_strbuf(ls);
             eo_lexer_context_push(ls);
             parse_name(ls, buf);
             nm = eina_strbuf_string_get(buf);
             bnm = eina_stringshare_ref(ls->filename);
             fnm = database_class_to_filename(nm);
             if (!compare_class_file(bnm, fnm))
               {
                  const char *fname = eina_hash_find(ls->state->filenames_eo, fnm);
                  eina_stringshare_del(bnm);
                  free(fnm);
                  if (fname)
                    {
                       _parse_dep(ls, fname, nm);
                       def->type = EOLIAN_TYPE_CLASS;
                    }
               }
             else
               {
                  eina_stringshare_del(bnm);
                  free(fnm);
                  def->type = EOLIAN_TYPE_CLASS;
               }
             _fill_name(eina_stringshare_add(nm), &def->full_name, &def->name,
                        &def->namespaces);
             eo_lexer_context_pop(ls);
             pop_strbuf(ls);
          }
     }
   return def;
}

static Eolian_Typedecl *
parse_typedef(Eo_Lexer *ls)
{
   Eolian_Declaration *decl;
   Eolian_Typedecl *def = push_typedecl(ls);
   Eina_Bool has_extern;
   const char *freefunc;
   Eina_Strbuf *buf;
   eo_lexer_get(ls);
   parse_struct_attrs(ls, EINA_FALSE, &has_extern, &freefunc);
   def->freefunc = freefunc;
   pop_str(ls);
   def->type = EOLIAN_TYPEDECL_ALIAS;
   def->is_extern = has_extern;
   buf = push_strbuf(ls);
   eo_lexer_context_push(ls);
   FILL_BASE(def->base, ls, ls->line_number, ls->column);
   parse_name(ls, buf);
   _fill_name(eina_stringshare_add(eina_strbuf_string_get(buf)),
              &def->full_name, &def->name, &def->namespaces);
   decl = (Eolian_Declaration *)eina_hash_find(ls->state->unit.decls, def->full_name);
   if (decl)
     {
        eo_lexer_context_restore(ls);
        redef_error(ls, decl, EOLIAN_DECL_ALIAS);
     }
   eo_lexer_context_pop(ls);
   check_next(ls, ':');
   def->base_type = parse_type(ls);
   pop_type(ls);
   check_next(ls, ';');
   FILL_DOC(ls, def, doc);
   pop_strbuf(ls);
   return def;
}

static Eolian_Variable *
parse_variable(Eo_Lexer *ls, Eina_Bool global)
{
   Eolian_Declaration *decl;
   Eolian_Variable *def = calloc(1, sizeof(Eolian_Variable));
   Eina_Strbuf *buf;
   ls->tmp.var = def;
   eo_lexer_get(ls);
   if (ls->t.kw == KW_at_extern)
     {
        def->is_extern = EINA_TRUE;
        eo_lexer_get(ls);
     }
   def->type = global ? EOLIAN_VAR_GLOBAL : EOLIAN_VAR_CONSTANT;
   buf = push_strbuf(ls);
   eo_lexer_context_push(ls);
   FILL_BASE(def->base, ls, ls->line_number, ls->column);
   parse_name(ls, buf);
   _fill_name(eina_stringshare_add(eina_strbuf_string_get(buf)),
              &def->full_name, &def->name, &def->namespaces);
   decl = (Eolian_Declaration *)eina_hash_find(ls->state->unit.decls, def->full_name);
   if (decl)
     {
        eo_lexer_context_restore(ls);
        redef_error(ls, decl, EOLIAN_DECL_VAR);
     }
   eo_lexer_context_pop(ls);
   check_next(ls, ':');
   def->base_type = parse_type(ls);
   pop_type(ls);
   /* constants are required to have a value */
   if (!global)
     check(ls, '=');
   /* globals can optionally have a value */
   if (ls->t.token == '=')
     {
        ls->expr_mode = EINA_TRUE;
        eo_lexer_get(ls);
        def->value = parse_expr(ls);
        ls->expr_mode = EINA_FALSE;
        pop_expr(ls);
     }
   check_next(ls, ';');
   FILL_DOC(ls, def, doc);
   pop_strbuf(ls);
   return def;
}

typedef struct _Eo_Ret_Def
{
   Eolian_Type *type;
   Eolian_Documentation *doc;
   Eolian_Expression *default_ret_val;
   Eina_Bool warn_unused: 1;
   Eina_Bool owned: 1;
} Eo_Ret_Def;

static void
parse_return(Eo_Lexer *ls, Eo_Ret_Def *ret, Eina_Bool allow_void,
             Eina_Bool allow_def, Eina_Bool is_funcptr)
{
   eo_lexer_get(ls);
   check_next(ls, ':');
   if (allow_void)
     ret->type = parse_type_void(ls);
   else
     ret->type = parse_type(ls);
   ret->doc = NULL;
   ret->default_ret_val = NULL;
   ret->warn_unused = EINA_FALSE;
   ret->owned = EINA_FALSE;
   if (allow_def && (ls->t.token == '('))
     {
        int line = ls->line_number, col = ls->column;
        ls->expr_mode = EINA_TRUE;
        eo_lexer_get(ls);
        ret->default_ret_val = parse_expr(ls);
        ls->expr_mode = EINA_FALSE;
        check_match(ls, ')', '(', line, col);
     }
   Eina_Bool has_warn_unused = EINA_FALSE, has_owned = EINA_FALSE;
   if (!is_funcptr) for (;;) switch (ls->t.kw)
     {
      case KW_at_warn_unused:
        CASE_LOCK(ls, warn_unused, "warn_unused qualifier");
        ret->warn_unused = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_owned:
        CASE_LOCK(ls, owned, "owned qualifier");
        ret->owned = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      default:
        goto end;
     }
end:
   check_next(ls, ';');
   FILL_DOC(ls, ret, doc);
}

static void
parse_param(Eo_Lexer *ls, Eina_List **params, Eina_Bool allow_inout,
            Eina_Bool is_vals)
{
   Eina_Bool has_nonull   = EINA_FALSE, has_optional = EINA_FALSE,
             has_nullable = EINA_FALSE, has_owned    = EINA_FALSE;
   Eina_Bool cref = (ls->t.kw == KW_at_cref);
   Eolian_Function_Parameter *par = calloc(1, sizeof(Eolian_Function_Parameter));
   par->param_dir = EOLIAN_IN_PARAM;
   FILL_BASE(par->base, ls, ls->line_number, ls->column);
   *params = eina_list_append(*params, par);
   eolian_object_ref(&par->base);
   if (cref || (allow_inout && (ls->t.kw == KW_at_in)))
     {
        par->param_dir = EOLIAN_IN_PARAM;
        eo_lexer_get(ls);
     }
   else if (allow_inout && ls->t.kw == KW_at_out)
     {
        par->param_dir = EOLIAN_OUT_PARAM;
        eo_lexer_get(ls);
     }
   else if (allow_inout && ls->t.kw == KW_at_inout)
     {
        par->param_dir = EOLIAN_INOUT_PARAM;
        eo_lexer_get(ls);
     }
   else par->param_dir = EOLIAN_IN_PARAM;
   check(ls, TOK_VALUE);
   par->name = eina_stringshare_ref(ls->t.value.s);
   eo_lexer_get(ls);
   check_next(ls, ':');
   if (par->param_dir == EOLIAN_OUT_PARAM || par->param_dir == EOLIAN_INOUT_PARAM)
     par->type = parse_type_void(ls);
   else
     par->type = parse_type(ls);
   pop_type(ls);
   if ((is_vals || (par->param_dir == EOLIAN_OUT_PARAM)) && (ls->t.token == '('))
     {
        int line = ls->line_number, col = ls->column;
        ls->expr_mode = EINA_TRUE;
        eo_lexer_get(ls);
        par->value = parse_expr(ls);
        ls->expr_mode = EINA_FALSE;
        pop_expr(ls);
        check_match(ls, ')', '(', line, col);
     }
   if (cref)
     {
        par->type->is_const = EINA_TRUE;
        par->type->is_ptr = EINA_TRUE;
     }
   for (;;) switch (ls->t.kw)
     {
      case KW_at_nonull:
        if (has_nullable)
          eo_lexer_syntax_error(ls, "both nullable and nonull specified");
        CASE_LOCK(ls, nonull, "nonull qualifier")
        par->nonull = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_optional:
        CASE_LOCK(ls, optional, "optional qualifier");
        par->optional = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_nullable:
        if (has_nonull)
          eo_lexer_syntax_error(ls, "both nullable and nonull specified");
        CASE_LOCK(ls, nullable, "nullable qualifier");
        par->nullable = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_owned:
        CASE_LOCK(ls, owned, "owned qualifier");
        par->type->owned = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      default:
        goto end;
     }
end:
   check_next(ls, ';');
   FILL_DOC(ls, par, doc);
}

static void
parse_legacy(Eo_Lexer *ls, const char **out)
{
   eo_lexer_get(ls);
   check_next(ls, ':');
   check(ls, TOK_VALUE);
   *out = eina_stringshare_ref(ls->t.value.s);
   eo_lexer_get(ls);
   check_next(ls, ';');
}

static void
parse_params(Eo_Lexer *ls, Eina_List **params, Eina_Bool allow_inout,
             Eina_Bool is_vals)
{
   int line, col;
   eo_lexer_get(ls);
   line = ls->line_number, col = ls->column;
   check_next(ls, '{');
   while (ls->t.token != '}')
     parse_param(ls, params, allow_inout, is_vals);
   check_match(ls, '}', '{', line, col);
}

static void
parse_accessor(Eo_Lexer *ls, Eolian_Function *prop)
{
   int line, col;
   Eina_Bool has_return = EINA_FALSE, has_legacy = EINA_FALSE,
             has_eo     = EINA_FALSE, has_keys   = EINA_FALSE,
             has_values = EINA_FALSE, has_protected = EINA_FALSE,
             has_virtp  = EINA_FALSE;
   Eina_Bool is_get = (ls->t.kw == KW_get);
   if (is_get)
     {
        if (prop->base.file)
          eina_stringshare_del(prop->base.file);
        FILL_BASE(prop->base, ls, ls->line_number, ls->column);
        if (prop->type == EOLIAN_PROP_SET)
          prop->type = EOLIAN_PROPERTY;
        else
          prop->type = EOLIAN_PROP_GET;
     }
   else
     {
        FILL_BASE(prop->set_base, ls, ls->line_number, ls->column);
        if (prop->type == EOLIAN_PROP_GET)
          prop->type = EOLIAN_PROPERTY;
        else
          prop->type = EOLIAN_PROP_SET;
     }
   eo_lexer_get(ls);
   for (;;) switch (ls->t.kw)
     {
      case KW_at_pure_virtual:
        CASE_LOCK(ls, virtp, "pure_virtual qualifier");
        if (is_get) prop->impl->get_pure_virtual = EINA_TRUE;
        else prop->impl->set_pure_virtual = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_protected:
        CASE_LOCK(ls, protected, "protected qualifier");
        if (is_get) prop->get_scope = EOLIAN_SCOPE_PROTECTED;
        else prop->set_scope = EOLIAN_SCOPE_PROTECTED;
        eo_lexer_get(ls);
        break;
      default:
        goto parse_accessor;
     }
parse_accessor:
   line = ls->line_number;
   col = ls->column;
   check_next(ls, '{');
   if ((ls->t.token == TOK_DOC) && !prop->impl->common_doc)
     {
        if (getenv("EOLIAN_PROPERTY_DOC_WARN"))
          {
             _eolian_log_line(prop->base.file, line, col,
                              "%s doc without property doc for '%s.%s'",
                              is_get ? "getter" : "setter",
                              ls->tmp.kls->full_name, prop->name);
          }
     }
   if (is_get)
     {
        FILL_DOC(ls, prop->impl, get_doc);
     }
   else
     {
        FILL_DOC(ls, prop->impl, set_doc);
     }
   for (;;) switch (ls->t.kw)
     {
      case KW_return:
        CASE_LOCK(ls, return, "return")
        Eo_Ret_Def ret;
        parse_return(ls, &ret, is_get, EINA_TRUE, EINA_FALSE);
        pop_type(ls);
        if (ret.default_ret_val) pop_expr(ls);
        if (is_get)
          {
             prop->get_ret_type = ret.type;
             prop->get_return_doc = ret.doc;
             prop->get_ret_val = ret.default_ret_val;
             prop->get_return_warn_unused = ret.warn_unused;
             prop->get_ret_type->owned = ret.owned;
          }
        else
          {
             prop->set_ret_type = ret.type;
             prop->set_return_doc = ret.doc;
             prop->set_ret_val = ret.default_ret_val;
             prop->set_return_warn_unused = ret.warn_unused;
             prop->set_ret_type->owned = ret.owned;
          }
        break;
      case KW_legacy:
        CASE_LOCK(ls, legacy, "legacy name")
        if (is_get)
          parse_legacy(ls, &prop->get_legacy);
        else
          parse_legacy(ls, &prop->set_legacy);
        break;
      case KW_eo:
        CASE_LOCK(ls, eo, "eo name")
        eo_lexer_get(ls);
        check_next(ls, ':');
        check_kw_next(ls, KW_null);
        check_next(ls, ';');
        if (is_get)
          prop->get_only_legacy = EINA_TRUE;
        else
          prop->set_only_legacy = EINA_TRUE;
        break;
      case KW_keys:
        {
           Eina_List **stor;
           CASE_LOCK(ls, keys, "keys definition")
           stor = is_get ? &prop->prop_keys_get : &prop->prop_keys_set;
           parse_params(ls, stor, EINA_FALSE, EINA_FALSE);
           break;
        }
      case KW_values:
        {
           Eina_List **stor;
           CASE_LOCK(ls, values, "values definition")
           stor = is_get ? &prop->prop_values_get : &prop->prop_values_set;
           parse_params(ls, stor, EINA_FALSE, EINA_TRUE);
           break;
        }
      default:
        goto end;
     }
end:
   check_match(ls, '}', '{', line, col);
}

static void
_func_pure_virtual_set(Eo_Lexer *ls, Eolian_Function *foo_id, Eina_Bool virt)
{
   if (ls->tmp.kls->type != EOLIAN_CLASS_INTERFACE && !virt)
     return;

   if (foo_id->type == EOLIAN_PROP_GET || foo_id->type == EOLIAN_METHOD)
     foo_id->impl->get_pure_virtual = EINA_TRUE;
   else if (foo_id->type == EOLIAN_PROP_SET)
     foo_id->impl->set_pure_virtual = EINA_TRUE;
   else if (foo_id->type == EOLIAN_PROPERTY)
     foo_id->impl->get_pure_virtual = foo_id->impl->set_pure_virtual = EINA_TRUE;
}

static void
parse_property(Eo_Lexer *ls)
{
   int line, col;
   Eolian_Function *prop = NULL;
   Eolian_Implement *impl = NULL;
   Eina_Bool has_get       = EINA_FALSE, has_set    = EINA_FALSE,
             has_keys      = EINA_FALSE, has_values = EINA_FALSE,
             has_protected = EINA_FALSE, has_class  = EINA_FALSE,
             has_beta      = EINA_FALSE, has_virtp  = EINA_FALSE;
   prop = calloc(1, sizeof(Eolian_Function));
   prop->klass = ls->tmp.kls;
   prop->type = EOLIAN_UNRESOLVED;
   prop->get_scope = prop->set_scope = EOLIAN_SCOPE_PUBLIC;
   FILL_BASE(prop->base, ls, ls->line_number, ls->column);
   impl = calloc(1, sizeof(Eolian_Implement));
   impl->klass = ls->tmp.kls;
   impl->foo_id = prop;
   FILL_BASE(impl->base, ls, ls->line_number, ls->column);
   prop->impl = impl;
   ls->tmp.kls->properties = eina_list_append(ls->tmp.kls->properties, prop);
   ls->tmp.kls->implements = eina_list_append(ls->tmp.kls->implements, impl);
   eolian_object_ref(&prop->base);
   eolian_object_ref(&impl->base);
   check(ls, TOK_VALUE);
   if (ls->t.kw == KW_get || ls->t.kw == KW_set)
     {
        eo_lexer_syntax_error(ls, "reserved keyword as property name");
        return;
     }
   prop->name = eina_stringshare_ref(ls->t.value.s);
   impl->full_name = eina_stringshare_printf("%s.%s", ls->tmp.kls->full_name, prop->name);
   eo_lexer_get(ls);
   for (;;) switch (ls->t.kw)
     {
      case KW_at_protected:
        CASE_LOCK(ls, protected, "protected qualifier")
        prop->get_scope = prop->set_scope = EOLIAN_SCOPE_PROTECTED;
        eo_lexer_get(ls);
        break;
      case KW_at_class:
        CASE_LOCK(ls, class, "class qualifier");
        prop->is_class = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_beta:
        CASE_LOCK(ls, beta, "beta qualifier");
        prop->is_beta = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_pure_virtual:
        CASE_LOCK(ls, virtp, "pure_virtual qualifier");
        eo_lexer_get(ls);
        break;
      default:
        goto body;
     }
body:
   line = ls->line_number;
   col = ls->column;
   check_next(ls, '{');
   FILL_DOC(ls, prop->impl, common_doc);
   for (;;) switch (ls->t.kw)
     {
      case KW_get:
        CASE_LOCK(ls, get, "get definition")
        impl->is_prop_get = EINA_TRUE;
        parse_accessor(ls, prop);
        break;
      case KW_set:
        CASE_LOCK(ls, set, "set definition")
        impl->is_prop_set = EINA_TRUE;
        parse_accessor(ls, prop);
        break;
      case KW_keys:
        CASE_LOCK(ls, keys, "keys definition")
        parse_params(ls, &prop->prop_keys, EINA_FALSE, EINA_FALSE);
        break;
      case KW_values:
        CASE_LOCK(ls, values, "values definition")
        parse_params(ls, &prop->prop_values, EINA_FALSE, EINA_TRUE);
        break;
      default:
        goto end;
     }
end:
   check_match(ls, '}', '{', line, col);
   if (!has_get && !has_set)
     {
        prop->type = EOLIAN_PROPERTY;
        impl->is_prop_get = impl->is_prop_set = EINA_TRUE;
     }
   _func_pure_virtual_set(ls, prop, has_virtp);
}

static Eolian_Typedecl*
parse_function_pointer(Eo_Lexer *ls)
{
   int bline, bcol;
   int line = ls->line_number, col = ls->column;

   Eolian_Typedecl *def = push_typedecl(ls);
   Eina_Strbuf *buf = push_strbuf(ls);
   Eolian_Function *meth = NULL;

   Eina_Bool has_params = EINA_FALSE,
             has_return = EINA_FALSE;

   eo_lexer_get(ls);

   def->type = EOLIAN_TYPEDECL_FUNCTION_POINTER;
   def->is_extern = (ls->t.kw == KW_at_extern);
   if (def->is_extern)
     eo_lexer_get(ls);

   parse_name(ls, buf);
   _fill_name(eina_stringshare_add(eina_strbuf_string_get(buf)),
           &def->full_name, &def->name, &def->namespaces);

   pop_strbuf(ls);

   meth = calloc(1, sizeof(Eolian_Function));
   meth->klass = NULL;
   meth->type = EOLIAN_FUNCTION_POINTER;
   meth->get_scope = meth->set_scope = EOLIAN_SCOPE_PUBLIC;
   meth->name = eina_stringshare_ref(def->name);

   def->function_pointer = meth;
   eolian_object_ref(&meth->base);

   meth->is_beta = (ls->t.kw == KW_at_beta);
   if (meth->is_beta)
     eo_lexer_get(ls);

   bline = ls->line_number;
   bcol = ls->column;
   check_next(ls, '{');
   FILL_DOC(ls, def, doc);
   for (;;) switch (ls->t.kw)
     {
      case KW_return:
        CASE_LOCK(ls, return, "return");
        Eo_Ret_Def ret;
        parse_return(ls, &ret, EINA_FALSE, EINA_FALSE, EINA_TRUE);
        pop_type(ls);
        meth->get_ret_type = ret.type;
        meth->get_return_doc = ret.doc;
        meth->get_ret_val = NULL;
        meth->get_return_warn_unused = EINA_FALSE;
        break;
      case KW_params:
        CASE_LOCK(ls, params, "params definition");
        parse_params(ls, &meth->params, EINA_TRUE, EINA_FALSE);
        break;
      default:
        goto end;
     }
end:
   check_match(ls, '}', '{', bline, bcol);
   check_next(ls, ';');
   FILL_BASE(def->base, ls, line, col);
   FILL_BASE(meth->base, ls, line, col);
   return def;
}

static void
parse_method(Eo_Lexer *ls)
{
   int line, col;
   Eolian_Function *meth = NULL;
   Eolian_Implement *impl = NULL;
   Eina_Bool has_const       = EINA_FALSE, has_params = EINA_FALSE,
             has_return      = EINA_FALSE, has_legacy = EINA_FALSE,
             has_protected   = EINA_FALSE, has_class  = EINA_FALSE,
             has_eo          = EINA_FALSE, has_beta   = EINA_FALSE,
             has_virtp       = EINA_FALSE;
   meth = calloc(1, sizeof(Eolian_Function));
   meth->klass = ls->tmp.kls;
   meth->type = EOLIAN_METHOD;
   meth->get_scope = meth->set_scope = EOLIAN_SCOPE_PUBLIC;
   FILL_BASE(meth->base, ls, ls->line_number, ls->column);
   impl = calloc(1, sizeof(Eolian_Implement));
   impl->klass = ls->tmp.kls;
   impl->foo_id = meth;
   FILL_BASE(impl->base, ls, ls->line_number, ls->column);
   meth->impl = impl;
   ls->tmp.kls->methods = eina_list_append(ls->tmp.kls->methods, meth);
   ls->tmp.kls->implements = eina_list_append(ls->tmp.kls->implements, impl);
   eolian_object_ref(&meth->base);
   eolian_object_ref(&impl->base);
   check(ls, TOK_VALUE);
   if (ls->t.kw == KW_get || ls->t.kw == KW_set)
     {
        eo_lexer_syntax_error(ls, "reserved keyword as method name");
        return;
     }
   meth->name = eina_stringshare_ref(ls->t.value.s);
   impl->full_name = eina_stringshare_printf("%s.%s", ls->tmp.kls->full_name, meth->name);
   eo_lexer_get(ls);
   for (;;) switch (ls->t.kw)
     {
      case KW_at_protected:
        CASE_LOCK(ls, protected, "protected qualifier")
        meth->get_scope = meth->set_scope = EOLIAN_SCOPE_PROTECTED;
        eo_lexer_get(ls);
        break;
      case KW_at_const:
        CASE_LOCK(ls, const, "const qualifier")
        meth->obj_is_const = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_class:
        CASE_LOCK(ls, class, "class qualifier");
        meth->is_class = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_beta:
        CASE_LOCK(ls, beta, "beta qualifier");
        meth->is_beta = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_pure_virtual:
        CASE_LOCK(ls, virtp, "pure_virtual qualifier");
        eo_lexer_get(ls);
        break;
      default:
        goto body;
     }
body:
   line = ls->line_number;
   col = ls->column;
   check_next(ls, '{');
   FILL_DOC(ls, meth->impl, common_doc);
   for (;;) switch (ls->t.kw)
     {
      case KW_return:
        CASE_LOCK(ls, return, "return")
        Eo_Ret_Def ret;
        parse_return(ls, &ret, EINA_FALSE, EINA_TRUE, EINA_FALSE);
        pop_type(ls);
        if (ret.default_ret_val) pop_expr(ls);
        meth->get_ret_type = ret.type;
        meth->get_return_doc = ret.doc;
        meth->get_ret_val = ret.default_ret_val;
        meth->get_return_warn_unused = ret.warn_unused;
        meth->get_ret_type->owned = ret.owned;
        break;
      case KW_legacy:
        CASE_LOCK(ls, legacy, "legacy name")
        parse_legacy(ls, &meth->get_legacy);
        break;
      case KW_eo:
        CASE_LOCK(ls, eo, "eo name")
        eo_lexer_get(ls);
        check_next(ls, ':');
        check_kw_next(ls, KW_null);
        check_next(ls, ';');
        meth->get_only_legacy = EINA_TRUE;
        break;
      case KW_params:
        CASE_LOCK(ls, params, "params definition")
        parse_params(ls, &meth->params, EINA_TRUE, EINA_FALSE);
        break;
      default:
        goto end;
     }
end:
   check_match(ls, '}', '{', line, col);
   _func_pure_virtual_set(ls, meth, has_virtp);
}

static void
parse_part(Eo_Lexer *ls)
{
   Eolian_Part *part = calloc(1, sizeof(Eolian_Part));
   ls->tmp.kls->parts = eina_list_append(ls->tmp.kls->parts, part);
   eolian_object_ref(&part->base);
   check(ls, TOK_VALUE);
   part->name = eina_stringshare_ref(ls->t.value.s);
   eo_lexer_get(ls);
   check_next(ls, ':');
   Eina_Strbuf *buf = push_strbuf(ls);
   eo_lexer_context_push(ls);
   parse_name(ls, buf);
   const char *nm = eina_strbuf_string_get(buf);
   const char *bnm = eina_stringshare_ref(ls->filename);
   char *fnm = database_class_to_filename(nm);
   if (!compare_class_file(bnm, fnm))
     {
        Eolian_Class *dep = NULL;
        const char *fname = eina_hash_find(ls->state->filenames_eo, fnm);
        eina_stringshare_del(bnm);
        free(fnm);
        if (fname)
          dep = _parse_dep(ls, fname, nm);
        if (!dep)
          {
             char ebuf[PATH_MAX];
             eo_lexer_context_restore(ls);
             snprintf(ebuf, sizeof(ebuf), "unknown class '%s'", nm);
             eo_lexer_syntax_error(ls, ebuf);
             return;
          }
        part->klass = dep;
     }
   else
     part->klass = ls->tmp.kls;
   pop_strbuf(ls);
   check_next(ls, ';');
   FILL_DOC(ls, part, doc);
}

static void
parse_implement(Eo_Lexer *ls, Eina_Bool iface)
{
   Eina_Strbuf *buf = NULL;
   Eolian_Implement *impl = NULL;
   int iline = ls->line_number, icol = ls->column;
   if (iface)
     check_kw(ls, KW_class);
   if (ls->t.kw == KW_class)
     {
        eo_lexer_get(ls);
        check_next(ls, '.');
        if (ls->t.kw == KW_destructor)
          {
             ls->tmp.kls->class_dtor_enable = EINA_TRUE;
             eo_lexer_get(ls);
          }
        else
          {
             check_kw_next(ls, KW_constructor);
             ls->tmp.kls->class_ctor_enable = EINA_TRUE;
          }
        check_next(ls, ';');
        return;
     }
   Eina_Bool glob_auto = EINA_FALSE, glob_empty = EINA_FALSE;
   switch (ls->t.kw)
     {
        case KW_at_auto:
          glob_auto = EINA_TRUE;
          eo_lexer_get(ls);
          break;
        case KW_at_empty:
          glob_empty = EINA_TRUE;
          eo_lexer_get(ls);
          break;
        default:
          break;
     }
   if (ls->t.token == '.')
     {
        eo_lexer_get(ls);
        if (ls->t.token != TOK_VALUE)
          eo_lexer_syntax_error(ls, "name expected");
        Eina_Stringshare *iname = eina_stringshare_printf("%s.%s",
                                                          ls->tmp.kls->full_name,
                                                          ls->t.value.s);
        Eina_List *l;
        Eolian_Implement *fimp;
        EINA_LIST_FOREACH(ls->tmp.kls->implements, l, fimp)
          if (iname == fimp->full_name)
            {
               impl = fimp;
               break;
            }
        eina_stringshare_del(iname);
        if (!impl)
          {
             eo_lexer_syntax_error(ls, "implement of non-existent function");
             return;
          }
        eo_lexer_get(ls);
        goto propbeg;
     }
   else
     {
        impl = calloc(1, sizeof(Eolian_Implement));
        FILL_BASE(impl->base, ls, iline, icol);
        ls->tmp.kls->implements = eina_list_append(ls->tmp.kls->implements, impl);
        eolian_object_ref(&impl->base);
     }
   if (ls->t.token != TOK_VALUE)
     eo_lexer_syntax_error(ls, "class name expected");
   buf = push_strbuf(ls);
   eina_strbuf_append(buf, ls->t.value.s);
   eo_lexer_get(ls);
   check_next(ls, '.');
   if (ls->t.token != TOK_VALUE)
     eo_lexer_syntax_error(ls, "name or constructor/destructor expected");
   for (;;)
     {
        if ((ls->t.kw == KW_constructor) || (ls->t.kw == KW_destructor))
          {
             eina_strbuf_append_char(buf, '.');
             eina_strbuf_append(buf, eo_lexer_keyword_str_get(ls->t.kw));
             eo_lexer_get(ls);
             check(ls, ';');
             goto propbeg;
          }
        eina_strbuf_append_char(buf, '.');
        check(ls, TOK_VALUE);
        eina_strbuf_append(buf, ls->t.value.s);
        eo_lexer_get(ls);
        if (ls->t.token != '.') break;
        eo_lexer_get(ls);
     }
propbeg:
   if (ls->t.token == '{')
     {
        Eina_Bool has_get = EINA_FALSE, has_set = EINA_FALSE;
        eo_lexer_get(ls);
        FILL_DOC(ls, impl, common_doc);
        for (;;) switch (ls->t.kw)
          {
           case KW_get:
             CASE_LOCK(ls, get, "get specifier");
             eo_lexer_get(ls);
             impl->is_prop_get = EINA_TRUE;
             impl->get_auto = glob_auto;
             impl->get_empty = glob_empty;
             if (ls->t.kw == KW_at_auto)
               {
                  impl->get_auto = EINA_TRUE;
                  eo_lexer_get(ls);
               }
             else if (ls->t.kw == KW_at_empty)
               {
                  impl->get_empty = EINA_TRUE;
                  eo_lexer_get(ls);
               }
             check_next(ls, ';');
             FILL_DOC(ls, impl, get_doc);
             break;
           case KW_set:
             CASE_LOCK(ls, set, "set specifier");
             eo_lexer_get(ls);
             impl->is_prop_set = EINA_TRUE;
             impl->set_auto = glob_auto;
             impl->set_empty = glob_empty;
             if (ls->t.kw == KW_at_auto)
               {
                  impl->set_auto = EINA_TRUE;
                  eo_lexer_get(ls);
               }
             else if (ls->t.kw == KW_at_empty)
               {
                  impl->set_empty = EINA_TRUE;
                  eo_lexer_get(ls);
               }
             check_next(ls, ';');
             FILL_DOC(ls, impl, set_doc);
             break;
           default:
             goto propend;
          }
propend:
        if (!has_get && !has_set)
          eo_lexer_syntax_error(ls, "property implements need at least get or set specified");
        check_next(ls, '}');
     }
   else
     {
        if (glob_auto)
          impl->get_auto = impl->set_auto = EINA_TRUE;
        if (glob_empty)
          impl->get_empty = impl->set_empty = EINA_TRUE;
        check_next(ls, ';');
        FILL_DOC(ls, impl, common_doc);
     }
   if (buf)
     {
        impl->full_name = eina_stringshare_add(eina_strbuf_string_get(buf));
        pop_strbuf(ls);
     }
}

static void
parse_constructor(Eo_Lexer *ls)
{
   Eina_Strbuf *buf = NULL;
   Eolian_Constructor *ctor = NULL;
   ctor = calloc(1, sizeof(Eolian_Constructor));
   FILL_BASE(ctor->base, ls, ls->line_number, ls->column);
   ls->tmp.kls->constructors = eina_list_append(ls->tmp.kls->constructors, ctor);
   eolian_object_ref(&ctor->base);
   if (ls->t.token == '.')
     {
        check_next(ls, '.');
        if (ls->t.token != TOK_VALUE)
          eo_lexer_syntax_error(ls, "name expected");
        ctor->full_name = eina_stringshare_printf("%s.%s",
                                                  ls->tmp.kls->full_name,
                                                  ls->t.value.s);
        eo_lexer_get(ls);
        if (ls->t.kw == KW_at_optional)
          {
             eo_lexer_get(ls);
             ctor->is_optional = EINA_TRUE;
          }
        check_next(ls, ';');
        return;
     }
   check(ls, TOK_VALUE);
   buf = push_strbuf(ls);
   eina_strbuf_append(buf, ls->t.value.s);
   eo_lexer_get(ls);
   check_next(ls, '.');
   check(ls, TOK_VALUE);
   for (;;)
     {
        eina_strbuf_append_char(buf, '.');
        check(ls, TOK_VALUE);
        eina_strbuf_append(buf, ls->t.value.s);
        eo_lexer_get(ls);
        if (ls->t.token != '.') break;
        eo_lexer_get(ls);
     }
   if (ls->t.kw == KW_at_optional)
     {
        eo_lexer_get(ls);
        ctor->is_optional = EINA_TRUE;
     }
   check_next(ls, ';');
   ctor->full_name = eina_stringshare_add(eina_strbuf_string_get(buf));
   pop_strbuf(ls);
}

static void
parse_event(Eo_Lexer *ls)
{
   Eolian_Event *ev = calloc(1, sizeof(Eolian_Event));
   FILL_BASE(ev->base, ls, ls->line_number, ls->column);
   ev->scope = EOLIAN_SCOPE_PUBLIC;
   Eina_Strbuf *buf = push_strbuf(ls);
   ls->tmp.kls->events = eina_list_append(ls->tmp.kls->events, ev);
   eolian_object_ref(&ev->base);
   check(ls, TOK_VALUE);
   eina_strbuf_append(buf, ls->t.value.s);
   eo_lexer_get(ls);
   while (ls->t.token == ',')
     {
        eo_lexer_get(ls);
        check(ls, TOK_VALUE);
        eina_strbuf_append_char(buf, ',');
        eina_strbuf_append(buf, ls->t.value.s);
        eo_lexer_get(ls);
     }
   ev->name = eina_stringshare_add(eina_strbuf_string_get(buf));
   pop_strbuf(ls);
   Eina_Bool has_scope = EINA_FALSE, has_beta = EINA_FALSE,
             has_hot   = EINA_FALSE, has_restart = EINA_FALSE,
             has_owned = EINA_FALSE;
   for (;;) switch (ls->t.kw)
     {
      case KW_at_private:
      case KW_at_protected:
        CASE_LOCK(ls, scope, "scope qualifier")
        ev->scope = (ls->t.kw == KW_at_private)
                     ? EOLIAN_SCOPE_PRIVATE
                     : EOLIAN_SCOPE_PROTECTED;
        eo_lexer_get(ls);
        break;
      case KW_at_beta:
        CASE_LOCK(ls, beta, "beta qualifier")
        ev->is_beta = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_hot:
        CASE_LOCK(ls, hot, "hot qualifier");
        ev->is_hot = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_restart:
        CASE_LOCK(ls, restart, "restart qualifier");
        ev->is_restart = EINA_TRUE;
        eo_lexer_get(ls);
        break;
      case KW_at_owned:
        CASE_LOCK(ls, owned, "owned qualifier");
        eo_lexer_get(ls);
        break;
      default:
        goto end;
     }
end:
   if (ls->t.token == ':')
     {
        eo_lexer_get(ls);
        ev->type = parse_type(ls);
        ev->type->owned = has_owned;
        pop_type(ls);
     }
   check(ls, ';');
   eo_lexer_get(ls);
   FILL_DOC(ls, ev, doc);
   ev->klass = ls->tmp.kls;
}

static void
parse_methods(Eo_Lexer *ls)
{
   int line, col;
   eo_lexer_get(ls);
   line = ls->line_number, col = ls->column;
   check_next(ls, '{');
   while (ls->t.token != '}')
     {
        if (ls->t.kw == KW_at_property)
          {
             eo_lexer_get(ls);
             parse_property(ls);
             continue;
          }
        parse_method(ls);
     }
   check_match(ls, '}', '{', line, col);
}

static void
parse_parts(Eo_Lexer *ls)
{
   int line, col;
   eo_lexer_get(ls);
   line = ls->line_number, col = ls->column;
   check_next(ls, '{');
   while (ls->t.token != '}')
     parse_part(ls);
   check_match(ls, '}', '{', line, col);
}

static void
parse_implements(Eo_Lexer *ls, Eina_Bool iface)
{
   int line, col;
   eo_lexer_get(ls);
   line = ls->line_number, col = ls->column;
   check_next(ls, '{');
   while (ls->t.token != '}')
     parse_implement(ls, iface);
   check_match(ls, '}', '{', line, col);
}

static void
parse_constructors(Eo_Lexer *ls)
{
   int line, col;
   eo_lexer_get(ls);
   line = ls->line_number, col = ls->column;
   check_next(ls, '{');
   while (ls->t.token != '}')
     parse_constructor(ls);
   check_match(ls, '}', '{', line, col);
}

static void
parse_events(Eo_Lexer *ls)
{
   int line, col;
   eo_lexer_get(ls);
   line = ls->line_number;
   col = ls->column;
   check(ls, '{');
   eo_lexer_get(ls);
   while (ls->t.token != '}')
     parse_event(ls);
   check_match(ls, '}', '{', line, col);
}

static void
_validate_pfx(Eo_Lexer *ls)
{
   char ebuf[PATH_MAX];
   check(ls, TOK_VALUE);
   const char *str = ls->t.value.s;
   if ((*str != '_') && ((*str < 'a') || (*str > 'z')))
     goto error;
   for (++str; *str; ++str)
     {
        if (*str == '_')
          continue;
        if ((*str >= 'a') && (*str <= 'z'))
          continue;
        if ((*str >= '0') && (*str <= '9'))
          continue;
        goto error;
     }
   return;
error:
   snprintf(ebuf, sizeof(ebuf), "invalid prefix '%s'", ls->t.value.s);
   eo_lexer_syntax_error(ls, ebuf);
}

static void
parse_class_body(Eo_Lexer *ls, Eolian_Class_Type type)
{
   Eina_Bool has_legacy_prefix = EINA_FALSE,
             has_eo_prefix     = EINA_FALSE,
             has_event_prefix  = EINA_FALSE,
             has_data          = EINA_FALSE,
             has_methods       = EINA_FALSE,
             has_parts         = EINA_FALSE,
             has_implements    = EINA_FALSE,
             has_constructors  = EINA_FALSE,
             has_events        = EINA_FALSE;
   FILL_DOC(ls, ls->tmp.kls, doc);
   if (type == EOLIAN_CLASS_INTERFACE)
     {
        ls->tmp.kls->data_type = eina_stringshare_add("null");
     }
   for (;;) switch (ls->t.kw)
     {
      case KW_legacy_prefix:
        CASE_LOCK(ls, legacy_prefix, "legacy prefix definition")
        eo_lexer_get(ls);
        check_next(ls, ':');
        _validate_pfx(ls);
        ls->tmp.kls->legacy_prefix = eina_stringshare_ref(ls->t.value.s);
        eo_lexer_get(ls);
        check_next(ls, ';');
        break;
      case KW_eo_prefix:
        CASE_LOCK(ls, eo_prefix, "eo prefix definition")
        eo_lexer_get(ls);
        check_next(ls, ':');
        _validate_pfx(ls);
        ls->tmp.kls->eo_prefix = eina_stringshare_ref(ls->t.value.s);
        eo_lexer_get(ls);
        check_next(ls, ';');
        break;
      case KW_event_prefix:
        CASE_LOCK(ls, event_prefix, "event prefix definition")
        eo_lexer_get(ls);
        check_next(ls, ':');
        _validate_pfx(ls);
        ls->tmp.kls->ev_prefix = eina_stringshare_ref(ls->t.value.s);
        eo_lexer_get(ls);
        check_next(ls, ';');
        break;
      case KW_data:
        if (type == EOLIAN_CLASS_INTERFACE) return;
        CASE_LOCK(ls, data, "data definition")
        eo_lexer_get(ls);
        check_next(ls, ':');
        check(ls, TOK_VALUE);
        ls->tmp.kls->data_type = eina_stringshare_ref(ls->t.value.s);
        eo_lexer_get(ls);
        check_next(ls, ';');
        break;
      case KW_methods:
        CASE_LOCK(ls, methods, "methods definition")
        parse_methods(ls);
        break;
      case KW_parts:
        CASE_LOCK(ls, parts, "parts definition")
        parse_parts(ls);
        break;
      case KW_implements:
        CASE_LOCK(ls, implements, "implements definition")
        parse_implements(ls, type == EOLIAN_CLASS_INTERFACE);
        break;
      case KW_constructors:
        if (type == EOLIAN_CLASS_INTERFACE || type == EOLIAN_CLASS_MIXIN)
          return;
        CASE_LOCK(ls, constructors, "constructors definition")
        parse_constructors(ls);
        break;
      case KW_events:
        CASE_LOCK(ls, events, "events definition")
        parse_events(ls);
        break;
      default:
        return;
     }
}

static void
_inherit_dep(Eo_Lexer *ls, Eina_Strbuf *buf, Eina_Bool check_inherit,
             Eolian_Class_Type type)
{
   const char *fname, *iname;
   char *fnm;
   eina_strbuf_reset(buf);
   eo_lexer_context_push(ls);
   parse_name(ls, buf);
   iname = eina_strbuf_string_get(buf);
   fnm = database_class_to_filename(iname);
   if (compare_class_file(fnm, ls->filename))
     {
        char ebuf[PATH_MAX];
        free(fnm);
        eo_lexer_context_restore(ls);
        snprintf(ebuf, sizeof(ebuf), "class '%s' cannot inherit from itself",
                 iname);
        eo_lexer_syntax_error(ls, ebuf);
        return; /* unreachable (longjmp above), make static analysis shut up */
     }
   fname = eina_hash_find(ls->state->filenames_eo, fnm);
   free(fnm);
   if (!fname)
     {
        char ebuf[PATH_MAX];
        eo_lexer_context_restore(ls);
        snprintf(ebuf, sizeof(ebuf), "unknown inherit '%s'", iname);
        eo_lexer_syntax_error(ls, ebuf);
     }
   Eolian_Class *dep = _parse_dep(ls, fname, iname);
   if (!dep)
     {
        char ebuf[PATH_MAX];
        eo_lexer_context_restore(ls);
        snprintf(ebuf, sizeof(ebuf), "unknown inherit '%s'. Incorrect case?", iname);
        eo_lexer_syntax_error(ls, ebuf);
        return;
     }
   if (check_inherit) switch (type)
     {
      case EOLIAN_CLASS_REGULAR:
      case EOLIAN_CLASS_ABSTRACT:
        if (dep->type != EOLIAN_CLASS_REGULAR && dep->type != EOLIAN_CLASS_ABSTRACT)
          {
             char ebuf[PATH_MAX];
             eo_lexer_context_restore(ls);
             snprintf(ebuf, sizeof(ebuf), "regular classes ('%s') cannot inherit from non-regular classes ('%s')",
                      ls->tmp.kls->full_name, iname);
             eo_lexer_syntax_error(ls, ebuf);
          }
        break;
      case EOLIAN_CLASS_MIXIN:
      case EOLIAN_CLASS_INTERFACE:
        if (dep->type != EOLIAN_CLASS_MIXIN && dep->type != EOLIAN_CLASS_INTERFACE)
          {
             char ebuf[PATH_MAX];
             eo_lexer_context_restore(ls);
             snprintf(ebuf, sizeof(ebuf), "non-regular classes ('%s') cannot inherit from regular classes ('%s')",
                      ls->tmp.kls->full_name, iname);
             eo_lexer_syntax_error(ls, ebuf);
          }
        break;
      default:
        break;
     }
   ls->tmp.kls->inherits = eina_list_append(ls->tmp.kls->inherits, dep);
   dep->toplevel = EINA_FALSE;
   eo_lexer_context_pop(ls);
}

static void
parse_class(Eo_Lexer *ls, Eolian_Class_Type type)
{
   Eolian_Declaration *decl;
   const char *bnm;
   char *fnm;
   Eina_Bool same;
   int line, col;
   Eina_Strbuf *buf = push_strbuf(ls);
   ls->tmp.kls = calloc(1, sizeof(Eolian_Class));
   ls->tmp.kls->toplevel = EINA_TRUE;
   FILL_BASE(ls->tmp.kls->base, ls, ls->line_number, ls->column);
   eo_lexer_get(ls);
   ls->tmp.kls->type = type;
   eo_lexer_context_push(ls);
   parse_name(ls, buf);
   bnm = eina_stringshare_ref(ls->filename);
   fnm = database_class_to_filename(eina_strbuf_string_get(buf));
   same = compare_class_file(bnm, fnm);
   eina_stringshare_del(bnm);
   free(fnm);
   if (!same)
     {
        eo_lexer_context_restore(ls);
        eo_lexer_syntax_error(ls, "class and file names differ");
     }
   _fill_name(eina_stringshare_add(eina_strbuf_string_get(buf)),
              &ls->tmp.kls->full_name, &ls->tmp.kls->name,
              &ls->tmp.kls->namespaces);
   decl = (Eolian_Declaration *)eina_hash_find(ls->state->unit.decls, ls->tmp.kls->full_name);
   if (decl)
     {
        eo_lexer_context_restore(ls);
        redef_error(ls, decl, EOLIAN_DECL_CLASS);
     }
   eo_lexer_context_pop(ls);
   pop_strbuf(ls);
   if (ls->t.token != '{')
     {
        line = ls->line_number;
        col = ls->column;
        check_next(ls, '(');
        if (ls->t.token != ')')
          {
              Eina_Strbuf *ibuf = push_strbuf(ls);
              _inherit_dep(ls, ibuf, EINA_TRUE, type);
              while (test_next(ls, ','))
                _inherit_dep(ls, ibuf, EINA_FALSE, type);
              pop_strbuf(ls);
          }
        check_match(ls, ')', '(', line, col);
     }
   line = ls->line_number;
   col = ls->column;
   check_next(ls, '{');
   parse_class_body(ls, type);
   check_match(ls, '}', '{', line, col);
}

static Eina_Bool
parse_unit(Eo_Lexer *ls, Eina_Bool eot)
{
   switch (ls->t.kw)
     {
      case KW_abstract:
        if (eot) goto def;
        parse_class(ls, EOLIAN_CLASS_ABSTRACT);
        goto found_class;
      case KW_class:
        if (eot) goto def;
        parse_class(ls, EOLIAN_CLASS_REGULAR);
        goto found_class;
      case KW_mixin:
        if (eot) goto def;
        parse_class(ls, EOLIAN_CLASS_MIXIN);
        goto found_class;
      case KW_interface:
        if (eot) goto def;
        parse_class(ls, EOLIAN_CLASS_INTERFACE);
        goto found_class;
      case KW_import:
        {
           Eina_Strbuf *buf = push_strbuf(ls);
           const char *found = NULL;
           char errbuf[PATH_MAX];
           eo_lexer_get(ls);
           check(ls, TOK_VALUE);
           eina_strbuf_append(buf, ls->t.value.s);
           eina_strbuf_append(buf, ".eot");
           if (!(found = eina_hash_find(ls->state->filenames_eot, eina_strbuf_string_get(buf))))
             {
                size_t buflen = eina_strbuf_length_get(buf);
                eina_strbuf_remove(buf, buflen - 1, buflen);
                if (!(found = eina_hash_find(ls->state->filenames_eo, eina_strbuf_string_get(buf))))
                  {
                     pop_strbuf(ls);
                     snprintf(errbuf, sizeof(errbuf),
                              "unknown import '%s'", ls->t.value.s);
                     eo_lexer_syntax_error(ls, errbuf);
                  }
             }
           eina_hash_set(ls->state->defer, eina_strbuf_string_get(buf), found);
           pop_strbuf(ls);
           eo_lexer_get(ls);
           check_next(ls, ';');
           break;
        }
      case KW_type:
        {
           database_type_add(ls->state, parse_typedef(ls));
           pop_typedecl(ls);
           break;
        }
      case KW_function:
        {
           database_type_add(ls->state, parse_function_pointer(ls));
           pop_typedecl(ls);
           break;
        }
      case KW_const:
      case KW_var:
        {
           database_var_add(ls->state, parse_variable(ls, ls->t.kw == KW_var));
           eolian_object_ref(&ls->tmp.var->base);
           ls->tmp.var = NULL;
           break;
        }
      case KW_struct:
      case KW_enum:
        {
           Eina_Bool is_enum = (ls->t.kw == KW_enum);
           const char *name;
           int line, col;
           Eolian_Declaration *decl;
           Eina_Bool has_extern;
           const char *freefunc;
           Eina_Strbuf *buf;
           eo_lexer_get(ls);
           parse_struct_attrs(ls, is_enum, &has_extern, &freefunc);
           buf = push_strbuf(ls);
           eo_lexer_context_push(ls);
           line = ls->line_number;
           col = ls->column;
           parse_name(ls, buf);
           name = eina_stringshare_add(eina_strbuf_string_get(buf));
           decl = (Eolian_Declaration *)eina_hash_find(ls->state->unit.decls, name);
           if (decl)
             {
                eina_stringshare_del(name);
                eo_lexer_context_restore(ls);
                redef_error(ls, decl, is_enum ? EOLIAN_DECL_ENUM : EOLIAN_DECL_STRUCT);
             }
           eo_lexer_context_pop(ls);
           pop_strbuf(ls);
           if (!is_enum && ls->t.token == ';')
             {
                Eolian_Typedecl *def = push_typedecl(ls);
                def->is_extern = has_extern;
                def->type = EOLIAN_TYPEDECL_STRUCT_OPAQUE;
                def->freefunc = freefunc;
                pop_str(ls);
                _fill_name(name, &def->full_name, &def->name, &def->namespaces);
                eo_lexer_get(ls);
                FILL_DOC(ls, def, doc);
                FILL_BASE(def->base, ls, line, col);
                database_struct_add(ls->state, def);
                pop_typedecl(ls);
                break;
             }
           if (is_enum)
             parse_enum(ls, name, has_extern, line, col);
           else
             parse_struct(ls, name, has_extern, line, col, freefunc);
           pop_typedecl(ls);
           break;
        }
      def:
      default:
        eo_lexer_syntax_error(ls, "invalid token");
        break;
     }
   return EINA_FALSE;
found_class:
   database_decl_add(ls->state, ls->tmp.kls->full_name, EOLIAN_DECL_CLASS,
                     ls->tmp.kls->base.file, ls->tmp.kls);
   return EINA_TRUE;
}

static void
parse_chunk(Eo_Lexer *ls, Eina_Bool eot)
{
   while (ls->t.token >= 0)
     /* set eot to EINA_TRUE so that we only allow parsing of one class */
     if (parse_unit(ls, eot))
       eot = EINA_TRUE;
}

const Eolian_Class *
_get_impl_class(const Eolian_Class *cl, const char *cln)
{
   if (!cl || !strcmp(cl->full_name, cln))
     return cl;
   Eina_List *l;
   Eolian_Class *icl;
   EINA_LIST_FOREACH(cl->inherits, l, icl)
     {
        /* we can do a depth first search, it's easier and doesn't matter
         * which part of the inheritance tree we find the class in
         */
        const Eolian_Class *fcl = _get_impl_class(icl, cln);
        if (fcl)
          return fcl;
     }
   return NULL;
}

#define _eo_parser_log(_base, ...) \
   _eolian_log_line((_base)->file, (_base)->line, (_base)->column, __VA_ARGS__)

static Eina_Bool
_db_fill_implement(Eolian_Class *cl, Eolian_Implement *impl)
{
   Eolian_Function_Type ftype = EOLIAN_METHOD;

   if (impl->is_prop_get && impl->is_prop_set)
     ftype = EOLIAN_PROPERTY;
   else if (impl->is_prop_get)
     ftype = EOLIAN_PROP_GET;
   else if (impl->is_prop_set)
     ftype = EOLIAN_PROP_SET;

   size_t imlen = strlen(impl->full_name);
   char *clbuf = alloca(imlen + 1);
   memcpy(clbuf, impl->full_name, imlen + 1);

   char *ldot = strrchr(clbuf, '.');
   if (!ldot)
     return EINA_FALSE; /* unreachable in practice, for static analysis */

   *ldot = '\0'; /* split between class name and func name */
   const char *clname = clbuf;
   const char *fnname = ldot + 1;

   const Eolian_Class *tcl = _get_impl_class(cl, clname);
   if (!tcl)
     {
        _eo_parser_log(&impl->base, "class '%s' not found within the inheritance tree of '%s'",
                       clname, cl->full_name);
        return EINA_FALSE;
     }

   impl->klass = tcl;

   const Eolian_Function *fid = eolian_class_function_get_by_name(tcl, fnname, EOLIAN_UNRESOLVED);
   if (!fid)
     {
        _eo_parser_log(&impl->base, "function '%s' not known in class '%s'", fnname, clname);
        return EINA_FALSE;
     }

   Eolian_Function_Type aftype = eolian_function_type_get(fid);

   Eina_Bool auto_empty = (impl->get_auto || impl->get_empty);

   /* match implement type against function type */
   if (ftype == EOLIAN_PROPERTY)
     {
        /* property */
        if (aftype != EOLIAN_PROPERTY)
          {
             _eo_parser_log(&impl->base, "function '%s' is not a complete property", fnname);
             return EINA_FALSE;
          }
        auto_empty = auto_empty && (impl->set_auto || impl->set_empty);
     }
   else if (ftype == EOLIAN_PROP_SET)
     {
        /* setter */
        if ((aftype != EOLIAN_PROP_SET) && (aftype != EOLIAN_PROPERTY))
          {
             _eo_parser_log(&impl->base, "function '%s' doesn't have a setter", fnname);
             return EINA_FALSE;
          }
        auto_empty = (impl->set_auto || impl->set_empty);
     }
   else if (ftype == EOLIAN_PROP_GET)
     {
        /* getter */
        if ((aftype != EOLIAN_PROP_GET) && (aftype != EOLIAN_PROPERTY))
          {
             _eo_parser_log(&impl->base, "function '%s' doesn't have a getter", fnname);
             return EINA_FALSE;
          }
     }
   else if (aftype != EOLIAN_METHOD)
     {
        _eo_parser_log(&impl->base, "function '%s' is not a method", fnname);
        return EINA_FALSE;
     }

   if ((fid->klass == cl) && !auto_empty)
     {
        /* only allow explicit implements from other classes, besides auto and
         * empty... also prevents pure virtuals from being implemented
         */
        _eo_parser_log(&impl->base, "invalid implement '%s'", impl->full_name);
        return EINA_FALSE;
     }

   impl->foo_id = fid;

   return EINA_TRUE;
}

static Eina_Bool
_db_fill_implements(Eolian_Class *cl)
{
   Eolian_Implement *impl;
   Eina_List *l;

   Eina_Bool ret = EINA_TRUE;

   Eina_Hash *th = eina_hash_string_small_new(NULL),
             *pth = eina_hash_string_small_new(NULL);
   EINA_LIST_FOREACH(cl->implements, l, impl)
     {
        Eina_Bool prop = (impl->is_prop_get || impl->is_prop_set);
        if (eina_hash_find(prop ? pth : th, impl->full_name))
          {
             _eo_parser_log(&impl->base, "duplicate implement '%s'", impl->full_name);
             ret = EINA_FALSE;
             goto end;
          }
        if (impl->klass != cl)
          {
             if (!_db_fill_implement(cl, impl))
               {
                  ret = EINA_FALSE;
                  goto end;
               }
             if (eolian_function_is_constructor(impl->foo_id, impl->klass))
               database_function_constructor_add((Eolian_Function *)impl->foo_id, cl);
          }
        if ((impl->klass != cl) && !_db_fill_implement(cl, impl))
          {
             ret = EINA_FALSE;
             goto end;
          }
        eina_hash_add(prop ? pth : th, impl->full_name, impl->full_name);
     }

end:
   eina_hash_free(th);
   eina_hash_free(pth);
   return ret;
}

static Eina_Bool
_db_fill_ctors(Eolian_Class *cl)
{
   Eolian_Constructor *ctor;
   Eina_List *l;

   Eina_Bool ret = EINA_TRUE;

   Eina_Hash *th = eina_hash_string_small_new(NULL);
   EINA_LIST_FOREACH(cl->constructors, l, ctor)
     {
        if (eina_hash_find(th, ctor->full_name))
          {
             _eo_parser_log(&ctor->base, "duplicate ctor '%s'", ctor->full_name);
             ret = EINA_FALSE;
             goto end;
          }
        const char *ldot = strrchr(ctor->full_name, '.');
        if (!ldot)
          {
             ret = EINA_FALSE;
             goto end;
          }
        char *cnbuf = alloca(ldot - ctor->full_name + 1);
        memcpy(cnbuf, ctor->full_name, ldot - ctor->full_name);
        cnbuf[ldot - ctor->full_name] = '\0';
        const Eolian_Class *tcl = _get_impl_class(cl, cnbuf);
        if (!tcl)
          {
             _eo_parser_log(&ctor->base, "class '%s' not found within the inheritance tree of '%s'",
                            cnbuf, cl->full_name);
             ret = EINA_FALSE;
             goto end;
          }
        ctor->klass = tcl;
        const Eolian_Function *cfunc = eolian_constructor_function_get(ctor);
        if (!cfunc)
          {
             _eo_parser_log(&ctor->base, "unable to find function '%s'", ctor->full_name);
             ret = EINA_FALSE;
             goto end;
          }
        database_function_constructor_add((Eolian_Function *)cfunc, tcl);
        eina_hash_add(th, ctor->full_name, ctor->full_name);
     }

end:
   eina_hash_free(th);
   return ret;
}

Eolian_Unit *
eo_parser_database_fill(Eolian_Unit *parent, const char *filename, Eina_Bool eot, Eolian_Class **fcl)
{
   Eolian_Unit *ret = NULL;
   Eolian_Class *cl = eina_hash_find(parent->state->parsed, filename);
   if (cl)
     {
        if (!eot && fcl) *fcl = cl;
        const char *fsl = strrchr(filename, '/');
        const char *bsl = strrchr(filename, '\\');
        const char *fname = NULL;
        if (fsl || bsl)
          fname = eina_stringshare_add((fsl > bsl) ? (fsl + 1) : (bsl + 1));
        if (fname)
          {
             ret = eina_hash_find(parent->state->units, fname);
             eina_stringshare_del(fname);
             return ret;
          }
        return NULL;
     }

   eina_hash_set(parent->state->parsing, filename, (void *)EINA_TRUE);

   Eo_Lexer *ls = eo_lexer_new(parent->state, filename);
   if (!ls)
     {
        _eolian_log("unable to create lexer for file '%s'", filename);
        goto error;
     }

   /* read first token */
   eo_lexer_get(ls);

   if (setjmp(ls->err_jmp))
     goto error;

   parse_chunk(ls, eot);
   if (eot) goto done;

   if (!(cl = ls->tmp.kls))
     {
        _eolian_log("eolian: no class for file '%s'", filename);
        goto error;
     }
   ls->tmp.kls = NULL;

   if (!_db_fill_implements(cl))
     goto error;

   if (!_db_fill_ctors(cl))
     goto error;

   eina_hash_set(ls->state->unit.classes, cl->full_name, cl);
   eina_hash_set(ls->state->classes_f, cl->base.file, cl);
   eolian_object_ref(&cl->base);

   if (fcl) *fcl = cl;

done:
   ret = ls->unit;
   eina_hash_set(ls->state->parsed, filename, eot ? (void *)EINA_TRUE : cl);
   eina_hash_set(ls->state->parsing, filename, (void *)EINA_FALSE);
   eina_hash_add(parent->children, filename, ret);

   eo_lexer_free(ls);
   return ret;

error:
   eina_hash_set(ls->state->parsing, filename, (void *)EINA_FALSE);
   eo_lexer_free(ls);
   return NULL;
}
