/* Prettyprint various things for debugging.
 */

/*

	Copyright (C) 1991-2003 The National Gallery

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

 */

/*

	These files are distributed with VIPS - http://www.vips.ecs.soton.ac.uk

 */

#include "nip4.h"

/* A lot of this file is just for debugging. Uncomment to enable all the
 * debugging code.
 */

/*
 */
#define VERBOSE
#define DEBUG

/* Dump a binary operator.
 */
char *
decode_BinOp(BinOp op)
{
	switch (op) {
	case BI_NONE:
		return "(none)";
	case BI_ADD:
		return "+";
	case BI_SUB:
		return "-";
	case BI_POW:
		return "**";
	case BI_REM:
		return "%";
	case BI_LSHIFT:
		return "<<";
	case BI_RSHIFT:
		return ">>";
	case BI_SELECT:
		return "?";
	case BI_DIV:
		return "/";
	case BI_JOIN:
		return "++";
	case BI_COMMA:
		return ",";
	case BI_DOT:
		return ".";
	case BI_MUL:
		return "*";
	case BI_LAND:
		return "&&";
	case BI_LOR:
		return "||";
	case BI_BAND:
		return "&";
	case BI_BOR:
		return "|";
	case BI_EOR:
		return "^";
	case BI_EQ:
		return "==";
	case BI_NOTEQ:
		return "!=";
	case BI_PEQ:
		return "===";
	case BI_PNOTEQ:
		return "!==";
	case BI_LESS:
		return "<";
	case BI_LESSEQ:
		return "<=";
	case BI_IF:
		return "if_then_else";
	case BI_CONS:
		return ":";

	default:
		g_assert(FALSE);

		/* Keep gcc happy.
		 */
		return NULL;
	}
}

/* Dump a unary operator.
 */
char *
decode_UnOp(UnOp op)
{
	switch (op) {
	case UN_NONE:
		return "(none)";
	case UN_CSCHAR:
		return "(signed char)";
	case UN_CUCHAR:
		return "(unsigned char)";
	case UN_CSSHORT:
		return "(signed short)";
	case UN_CUSHORT:
		return "(unsigned short)";
	case UN_CSINT:
		return "(signed int)";
	case UN_CUINT:
		return "(unsigned int)";
	case UN_CFLOAT:
		return "(float)";
	case UN_CDOUBLE:
		return "(double)";
	case UN_CCOMPLEX:
		return "(complex)";
	case UN_CDCOMPLEX:
		return "(double complex)";
	case UN_MINUS:
		return "-";
	case UN_NEG:
		return "!";
	case UN_COMPLEMENT:
		return "~";
	case UN_PLUS:
		return "+";

	default:
		g_assert(FALSE);

		/* Keep gcc happy.
		 */
		return NULL;
	}
}

/* Decode a node tag.
 */
char *
decode_NodeType(NodeType tag)
{
	switch (tag) {
	case TAG_APPL:
		return "TAG_APPL";
	case TAG_CONS:
		return "TAG_CONS";
	case TAG_FREE:
		return "TAG_FREE";
	case TAG_DOUBLE:
		return "TAG_DOUBLE";
	case TAG_COMPLEX:
		return "TAG_COMPLEX";
	case TAG_CLASS:
		return "TAG_CLASS";
	case TAG_GEN:
		return "TAG_GEN";
	case TAG_FILE:
		return "TAG_FILE";
	case TAG_SHARED:
		return "TAG_SHARED";
	case TAG_REFERENCE:
		return "TAG_REFERENCE";

	default:
		g_assert(FALSE);

		/* Keep gcc happy.
		 */
		return NULL;
	}
}

/* Decode a CombinatorType.
 */
char *
decode_CombinatorType(CombinatorType comb)
{
	switch (comb) {
	case COMB_S:
		return "S";
	case COMB_SL:
		return "Sl";
	case COMB_SR:
		return "Sr";
	case COMB_I:
		return "I";
	case COMB_K:
		return "K";
	case COMB_GEN:
		return "G";
	default:
		g_assert(FALSE);

		/* Keep gcc happy.
		 */
		return NULL;
	}
}

/* Decode a symbol type.
 */
char *
decode_SymbolType(SymbolType t)
{
	switch (t) {
	case SYM_VALUE:
		return "SYM_VALUE";
	case SYM_PARAM:
		return "SYM_PARAM";
	case SYM_ZOMBIE:
		return "SYM_ZOMBIE";
	case SYM_WORKSPACE:
		return "SYM_WORKSPACE";
	case SYM_WORKSPACEROOT:
		return "SYM_WORKSPACEROOT";
	case SYM_ROOT:
		return "SYM_ROOT";
	case SYM_BUILTIN:
		return "SYM_BUILTIN";

	default:
		g_assert(FALSE);
		return NULL;
	}
}

/* Decode a symbol type into something users might like to see.
 */
char *
decode_SymbolType_user(SymbolType t)
{
	switch (t) {
	case SYM_VALUE:
		return _("value");
	case SYM_PARAM:
		return _("parameter");
	case SYM_ZOMBIE:
		return _("zombie");
	case SYM_WORKSPACE:
		return _("workspace");
	case SYM_WORKSPACEROOT:
		return _("workspace root");
	case SYM_ROOT:
		return _("root symbol");
	case SYM_BUILTIN:
		return _("built-in symbol");

	default:
		g_assert(FALSE);
		return NULL;
	}
}

/* From here on, just used for debugging.
 */
#ifdef DEBUG

#warning "DEBUG on in dump.c"

/* Do a tiny dump of a symbol. Just a few characters.
 */
void *
dump_tiny(Symbol *sym)
{
	char txt[100];
	VipsBuf buf = VIPS_BUF_STATIC(txt);

	printf("(%p) ", sym);
	symbol_qualified_name(sym, &buf);
	if (sym->dirty)
		printf("*");
	printf("%s %s; ",
		decode_SymbolType(sym->type), vips_buf_all(&buf));

	return NULL;
}

/* Dump a expr, tiny.
 */
static void *
dump_expr_tiny(Expr *expr)
{
	printf("(expr->sym->name = ");
	symbol_name_print(expr->sym);
	printf(") ");

	return NULL;
}

/* Dump a expr info.
 */
void
dump_expr(Expr *expr, int *indent)
{
	Symbol *sym = expr->sym;

	printf("%*cexpr (%p)->sym->name = \"%s\"\n", *indent, ' ',
		expr, IOBJECT(sym)->name);

	*indent += 2;

	if (expr->row)
		printf("%*c%s->row = (set)\n", *indent, ' ', IOBJECT(sym)->name);

	if (expr->compile)
		dump_compile(expr->compile, indent);

	printf("%*c", *indent, ' ');
	if (sym->dirty)
		printf("<symbol is dirty ... can't print root>\n");
	else if (!PEISNOVAL(&expr->root)) {
		printf("%s->expr->root = ", IOBJECT(sym)->name);
		pgraph(&expr->root);
	}

	if (expr->err)
		printf("%*c%s->expr->err = %s\n", *indent, ' ',
			IOBJECT(sym)->name, bool_to_char(expr->err));
	if (expr->error_top)
		printf("%*c%s->expr->error_top = \"%s\"\n", *indent, ' ',
			IOBJECT(sym)->name, expr->error_top);
	if (expr->error_sub)
		printf("%*c%s->expr->error_sub = \"%s\"\n", *indent, ' ',
			IOBJECT(sym)->name, expr->error_sub);

	*indent -= 2;
}

/* Dump a compile, tiny.
 */
static void *
dump_compile_tiny(Compile *compile)
{
	printf("(compile->sym->name = ");
	symbol_name_print(compile->sym);
	printf(") ");

	return NULL;
}

/* Dump a compile.
 */
void
dump_compile(Compile *compile, int *indent)
{
	Symbol *sym = compile->sym;

	printf("%*ccompile (%p)->sym->name = \"%s\"\n", *indent, ' ',
		compile, IOBJECT(sym)->name);

	*indent += 2;

#ifdef VERBOSE
	printf("%*c%s->is_klass = %s\n", *indent, ' ',
		IOBJECT(sym)->name, bool_to_char(compile->is_klass));
	printf("%*c%s->has_super = %s\n", *indent, ' ',
		IOBJECT(sym)->name, bool_to_char(compile->has_super));

	printf("%*c%s->text = \"%s\"\n", *indent, ' ',
		IOBJECT(sym)->name, compile->text);
	printf("%*c%s->prhstext = \"%s\"\n", *indent, ' ',
		IOBJECT(sym)->name, compile->prhstext);
	printf("%*c%s->rhstext = \"%s\"\n", *indent, ' ',
		IOBJECT(sym)->name, compile->rhstext);
#endif /*VERBOSE*/

	if (compile->tree) {
		printf("%*c%s->tree = \n", *indent, ' ', IOBJECT(sym)->name);
		*indent += 2;
		(void) dump_tree(compile->tree, *indent);
		*indent -= 2;
	}
#ifdef VERBOSE
	printf("%*c%s->treefrag = %d pointers\n", *indent, ' ',
		IOBJECT(sym)->name, g_slist_length(compile->treefrag));
#endif /*VERBOSE*/

#ifdef VERBOSE
	{
		char txt[100];
		VipsBuf buf = VIPS_BUF_STATIC(txt);

		printf("%*c%s->nparam = %d\n", *indent, ' ',
			IOBJECT(sym)->name, compile->nparam);
		printf("%*c%s->param = ", *indent, ' ', IOBJECT(sym)->name);
		(void) slist_map(compile->param, (SListMapFn) dump_tiny, NULL);
		printf("\n");
		printf("%*c%s->nsecret = %d\n", *indent, ' ',
			IOBJECT(sym)->name, compile->nsecret);
		printf("%*c%s->secret = ", *indent, ' ', IOBJECT(sym)->name);
		(void) slist_map(compile->secret, (SListMapFn) dump_tiny, NULL);
		printf("\n");
		printf("%*c%s->this = ", *indent, ' ', IOBJECT(sym)->name);
		if (compile->this)
			dump_tiny(compile->this);
		else
			printf("(null)");
		printf("\n");
		printf("%*c%s->super:", *indent, ' ', IOBJECT(sym)->name);
		if (compile->super)
			dump_tiny(compile->super);
		else
			printf("(null)");
		printf("\n");
		printf("%*c%s->children = ", *indent, ' ', IOBJECT(sym)->name);
		(void) slist_map(compile->children, (SListMapFn) dump_tiny, NULL);
		printf("\n");

		graph_element(compile->heap, &buf, &compile->base, FALSE);
		printf("%*c%s->base = %s\n", *indent, ' ',
			IOBJECT(sym)->name, vips_buf_all(&buf));
		if (compile->heap) {
			*indent += 2;
			iobject_dump(IOBJECT(compile->heap), *indent);
			*indent -= 2;
		}
	}
#endif /*VERBOSE*/

	if (icontainer_get_n_children(ICONTAINER(compile)) > 0) {
		printf("%*c%s contains:\n", *indent, ' ', IOBJECT(sym)->name);
		*indent += 2;
		(void) icontainer_map(ICONTAINER(compile),
			(icontainer_map_fn) dump_symbol, indent, NULL);
		*indent -= 2;
	}

	*indent -= 2;
}

/* Print a full symbol and all it's children.
 */
void *
dump_symbol(Symbol *sym, int *indent)
{
	printf("%*cnsym->name = ", *indent, ' ');
	(void) dump_tiny(sym);
	printf("\n");

	*indent += 2;

#ifdef VERBOSE
	printf("%*c%s->patch = %d pointers\n", *indent, ' ',
		IOBJECT(sym)->name, g_slist_length(sym->patch));
#endif /*VERBOSE*/

	if (sym->expr) {
		*indent += 2;
		dump_expr(sym->expr, indent);
		*indent -= 2;
	}

#ifdef VERBOSE
	printf("%*c%s->base = ", *indent, ' ', IOBJECT(sym)->name);
	if (!sym->dirty) {
		PElement root;

		PEPOINTE(&root, &sym->base);
		pgraph(&root);
	}
	else
		printf("<sym is dirty ... can't print>\n");

	printf("%*c%s->dirty = %s\n", *indent, ' ',
		IOBJECT(sym)->name, bool_to_char(sym->dirty));
	printf("%*c%s->parents = ", *indent, ' ', IOBJECT(sym)->name);
	(void) slist_map(sym->parents, (SListMapFn) dump_compile_tiny, NULL);
	printf("\n");

	/* Prints topchildren and topparents.
	 */
	dump_links(sym, indent);
	printf("%*c%s->ndirtychildren = %d\n", *indent, ' ',
		IOBJECT(sym)->name, sym->ndirtychildren);
	printf("%*c%s->leaf = %s\n", *indent, ' ',
		IOBJECT(sym)->name, bool_to_char(sym->leaf));
	printf("%*c%s->needs_codegen = %s\n", *indent, ' ',
		IOBJECT(sym)->name, bool_to_char(sym->needs_codegen));
	printf("%*c%s->generated = %s\n", *indent, ' ',
		IOBJECT(sym)->name, bool_to_char(sym->generated));

	if (!sym->generated && sym->next_def) {
		printf("%*c%s->next_def = ", *indent, ' ', IOBJECT(sym)->name);
		for (Symbol *p = sym->next_def; p; p = sym->next_def)
			printf("%s ", IOBJECT(sym)->name);
		printf("\n");
	}

	printf("%*c%s->tool = kit ", *indent, ' ', IOBJECT(sym)->name);
	if (sym->tool)
		dump_kit(sym->tool->kit, indent);
	else
		printf("<NULL>\n");
#endif /*VERBOSE*/

	*indent -= 2;

	return NULL;
}

/* Pretty print the whole of the symbol table.
 */
void
dump_symbol_table(void)
{
	int indent = 0;
	(void) icontainer_map(ICONTAINER(symbol_root->expr->compile),
		(icontainer_map_fn) dump_symbol, &indent, NULL);
}

/* Tiny dump a tool.
 */
static void *
dump_tiny_tool(Tool *tool)
{
	switch (tool->type) {
	case TOOL_SEP:
		printf("<separator> ");
		break;

	case TOOL_SYM:
		dump_tiny(tool->sym);
		break;

	default:
		g_assert(FALSE);
	}

	return NULL;
}

/* Print out the syms in a kit.
 */
void *
dump_kit(Toolkit *kit, int *indent)
{
	printf("%*ckit->name = %s; ", *indent, ' ', IOBJECT(kit)->name);
	printf("%*c%s contains: ", *indent, ' ', IOBJECT(kit)->name);
	icontainer_map(ICONTAINER(kit),
		(icontainer_map_fn) dump_tiny_tool, NULL, NULL);
	printf("\n");

	return NULL;
}

/* Easy find-a-symbol for gdb.
 */
Symbol *
sym(char *name)
{
	return compile_lookup(symbol_root->expr->compile, name);
}

/* Print from a name.
 */
void
psym(char *name)
{
	Symbol *s;
	int indent;

	indent = 0;
	if ((s = sym(name)))
		(void) dump_symbol(s, &indent);
	else
		printf("Symbol \"%s\" not found\n", name);
}

/* Print scrap of graph.
 */
void
pgraph(PElement *graph)
{
	char txt[10240];
	VipsBuf buf = VIPS_BUF_STATIC(txt);

	graph_pelement(reduce_context->heap, &buf, graph, TRUE);
	printf("%s\n", vips_buf_all(&buf));
}

/* Print symbol value from name.
 */
void
psymv(char *name)
{
	Symbol *s;

	if ((s = sym(name))) {
		char txt[1024];
		VipsBuf buf = VIPS_BUF_STATIC(txt);

		graph_pelement(reduce_context->heap,
			&buf, &s->expr->root, TRUE);
		printf("%s = %s\n", name, vips_buf_all(&buf));
	}
}

/* Pretty-print an element.
 */
static void
print_element(EType type, void *arg, int indent)
{
	switch (type) {
	case ELEMENT_NOVAL:
		printf("no value (%d)\n", GPOINTER_TO_INT(arg));
		break;

	case ELEMENT_NODE:
		printf("node ->\n");
		graph_heap(arg, indent + 1);
		break;

	case ELEMENT_SYMBOL:
		printf("symbol \"%s\"", IOBJECT(arg)->name);
		break;

	case ELEMENT_SYMREF:
		printf("symref \"%s\"", IOBJECT(arg)->name);
		break;

	case ELEMENT_COMPILEREF:
		printf("compileref ");
		compile_name_print(COMPILE(arg));
		break;

	case ELEMENT_CONSTRUCTOR:
		printf("constructor \"%s\"", IOBJECT(arg)->name);
		break;

	case ELEMENT_CHAR:
		printf("char \"%c\"", GPOINTER_TO_UINT(arg));
		break;

	case ELEMENT_BOOL:
		printf("bool \"%s\"", bool_to_char((gboolean) GPOINTER_TO_UINT(arg)));
		break;

	case ELEMENT_BINOP:
		printf("binop \"%s\"", decode_BinOp((BinOp) GPOINTER_TO_INT(arg)));
		break;

	case ELEMENT_UNOP:
		printf("unop \"%s\"", decode_UnOp((UnOp) GPOINTER_TO_INT(arg)));
		break;

	case ELEMENT_COMB:
		printf("combinator \"%s\"",
			decode_CombinatorType((CombinatorType) GPOINTER_TO_INT(arg)));
		break;

	case ELEMENT_TAG:
		printf("tag \"%s\"", (char *) arg);
		break;

	case ELEMENT_MANAGED:
		printf("Managed* %p", arg);
		break;

	case ELEMENT_ELIST:
		printf("empty-list []");
		break;

	default:
		g_assert(FALSE);
	}
}

/* Pretty-print a heap graph.
 */
void
graph_heap(HeapNode *hn, int indent)
{
	if (!hn)
		return;

	printf("%*c", indent, ' ');
	printf("Node: ");

	printf("serial = %d, ", hn->flgs & FLAG_SERIAL);

	if (hn->flgs & FLAG_PRINT)
		printf("print, ");
	else
		printf("noprint, ");

	if (hn->flgs & FLAG_DEBUG)
		printf("debug, ");
	else
		printf("nodebug, ");

	if (hn->flgs & FLAG_MARK)
		printf("marked, ");
	else
		printf("notmarked, ");

	printf("%s ", decode_NodeType(hn->type));

	switch (hn->type) {
	case TAG_APPL:
	case TAG_CONS:
		printf("\n");
		printf("%*c", indent, ' ');
		printf("left:  ");
		print_element(hn->ltype, hn->body.ptrs.left, indent);

		printf("\n");
		printf("%*c", indent, ' ');
		printf("right: ");
		print_element(hn->rtype, hn->body.ptrs.right, indent);
		printf("\n");

		break;

	case TAG_DOUBLE:
		printf("real \"%g\"\n", hn->body.num);
		break;

	case TAG_CLASS:
		printf("instance-of-class \"%s\"\n",
			IOBJECT(hn->body.ptrs.left)->name);
		printf(" secrets=(");
		print_element(GETRIGHT(hn)->ltype,
			GETRIGHT(hn)->body.ptrs.left, indent);
		printf(") members=(");
		print_element(GETRIGHT(hn)->rtype,
			GETRIGHT(hn)->body.ptrs.right, indent);
		printf(")\n");
		break;

	case TAG_COMPLEX:
		printf("complex \"(%g,%g)\"\n",
			GETLEFT(hn)->body.num, GETRIGHT(hn)->body.num);
		break;

	case TAG_GEN:
		printf("list generator start=%g next=%g final=%g\n",
			GETLEFT(hn)->body.num,
			GETLEFT(GETRIGHT(hn))->body.num,
			GETRIGHT(GETRIGHT(hn))->body.num);
		break;

	case TAG_FILE:
		printf("list generator file=%s\n",
			MANAGEDFILE(GETLEFT(hn))->file->fname);
		break;

	case TAG_FREE:
	default:
		g_assert(FALSE);
	}
}

/* Pretty-print a const.
 */
static void
dump_parseconst(ParseConst *pc)
{
	switch (pc->type) {
	case PARSE_CONST_NUM:
		printf("%G", pc->val.num);
		break;

	case PARSE_CONST_COMPLEX:
		printf("%Gj", pc->val.num);

	case PARSE_CONST_STR:
		printf("\"%s\"", pc->val.str);
		break;

	case PARSE_CONST_BOOL:
		printf("%s", bool_to_char(pc->val.bol));
		break;

	case PARSE_CONST_CHAR:
		printf("'%c'", pc->val.ch);
		break;

	case PARSE_CONST_ELIST:
		printf("[]");
		break;

	default:
		g_assert(FALSE);
	}
}

static void *
dump_list_element(ParseNode *n, int *indent)
{
	printf("%*c", *indent, ' ');
	printf("ITEM\n");
	dump_tree(n, *indent + 2);

	return NULL;
}

/* Dump a parse tree.
 */
void *
dump_tree(ParseNode *n, int indent)
{
	switch (n->type) {
	case NODE_NONE:
		printf("%*c", indent, ' ');
		printf("node->type == NODE_NONE\n");
		break;

	case NODE_APPLY:
		printf("%*c", indent, ' ');
		printf("Function application\n");
		printf("%*c", indent, ' ');
		printf("LHS\n");
		(void) dump_tree(n->arg1, indent + 2);
		printf("%*c", indent, ' ');
		printf("RHS\n");
		(void) dump_tree(n->arg2, indent + 2);
		break;

	case NODE_CLASS:
		printf("%*c", indent, ' ');
		printf("Class: ");
		(void) dump_compile_tiny(n->klass);
		printf("\n");
		break;

	case NODE_LEAF:
		printf("%*c", indent, ' ');
		printf("Leaf symbol (%p): ", n->leaf);
		(void) dump_tiny(n->leaf);
		printf("\n");
		break;

	case NODE_TAG:
		printf("%*c", indent, ' ');
		printf("Tag: %s\n", n->tag);
		break;

	case NODE_BINOP:
		printf("%*c", indent, ' ');
		printf("Binary operator %s\n", decode_BinOp(n->biop));
		printf("%*c", indent, ' ');
		printf("LHS\n");
		(void) dump_tree(n->arg1, indent + 2);
		printf("%*c", indent, ' ');
		printf("RHS\n");
		(void) dump_tree(n->arg2, indent + 2);
		break;

	case NODE_UOP:
		printf("%*c", indent, ' ');
		printf("Unary operator %s:\n", decode_UnOp(n->uop));
		(void) dump_tree(n->arg1, indent + 2);
		break;

	case NODE_CONST:
		printf("%*c", indent, ' ');
		printf("Constant ");
		dump_parseconst(&n->con);
		printf("\n");
		break;

	case NODE_GENERATOR:
		printf("%*c", indent, ' ');
		printf("List generator\n");
		printf("%*c", indent, ' ');
		printf("Start:\n");
		(void) dump_tree(n->arg1, indent + 2);
		if (n->arg2) {
			printf("%*c", indent, ' ');
			printf("Next:\n");
			(void) dump_tree(n->arg2, indent + 2);
		}
		if (n->arg3) {
			printf("%*c", indent, ' ');
			printf("End:\n");
			(void) dump_tree(n->arg3, indent + 2);
		}
		break;

	case NODE_COMPOSE:
		printf("%*c", indent, ' ');
		printf("Function compose\n");
		printf("LHS\n");
		(void) dump_tree(n->arg1, indent + 2);
		printf("RHS\n");
		(void) dump_tree(n->arg2, indent + 2);
		break;

	case NODE_LISTCONST:
	case NODE_SUPER:
		printf("%*c", indent, ' ');
		if (n->type == NODE_LISTCONST)
			printf("List constant\n");
		else
			printf("Superclass construct\n");

		slist_map(n->elist, (SListMapFn) dump_list_element, &indent);
		break;

	default:
		g_assert(FALSE);
	}

	return NULL;
}

static void *
dump_link_expr(LinkExpr *le)
{
	dump_expr_tiny(le->expr);
	printf(" count = %d ; ", le->count);

	return NULL;
}

void *
dump_link(Link *link, int *indent)
{
	printf("%*clink->parent = ", *indent, ' ');
	symbol_name_print(link->parent);
	if (link->parent->dirty)
		printf("(dirty)");
	printf("\n");

	printf("%*clink->child = ", *indent, ' ');
	symbol_name_print(link->child);
	if (link->child->dirty)
		printf("(dirty)");
	printf("\n");

	printf("%*clink->serial = %d\n", *indent, ' ', link->serial);

	printf("%*clink->static_links = ", *indent, ' ');
	slist_map(link->static_links, (SListMapFn) dump_link_expr, NULL);
	printf("\n");
	printf("%*clink->dynamic_links = ", *indent, ' ');
	slist_map(link->dynamic_links, (SListMapFn) dump_link_expr, NULL);
	printf("\n");

	return NULL;
}

void
dump_links(Symbol *sym, int *indent)
{
	printf("%*c", *indent, ' ');
	symbol_name_print(sym);
	printf("->topchildren = \n");
	*indent += 2;
	slist_map(sym->topchildren, (SListMapFn) dump_link, indent);
	*indent -= 2;

	printf("%*c", *indent, ' ');
	symbol_name_print(sym);
	printf("->topparents = \n");
	*indent += 2;
	slist_map(sym->topparents, (SListMapFn) dump_link, indent);
	*indent -= 2;
}

void
dump_symbol_heap(Symbol *sym)
{
	printf("symbol ");
	symbol_name_print(sym);
	printf("has graph:\n");
	if (sym->expr)
		pgraph(&sym->expr->root);
	printf("\n");
}

#endif /*DEBUG*/
