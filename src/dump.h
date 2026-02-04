/* Decls for dump.c
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

char *decode_BinOp(BinOp op);
char *decode_UnOp(UnOp op);
char *decode_NodeType(NodeType tag);
char *decode_CombinatorType(CombinatorType comb);
char *decode_SymbolType(SymbolType t);
char *decode_SymbolType_user(SymbolType t);

void *dump_tiny(Symbol *sym);
void *dump_symbol(Symbol *sym, int *indent);
void dump_expr(Expr *expr, int *indent);
void dump_compile(Compile *compile, int *indent);
void dump_symbol_table(void);
void *dump_kit(Toolkit *kit, int *indent);
Symbol *sym(char *name);
void psym(char *name);
void psymv(char *name);
void pgraph(PElement *graph);

void graph_heap(HeapNode *hn, int indent);
void graph_test(Heap *heap);

void *dump_tree(ParseNode *n, int indent);

void dump_links(Symbol *sym, int *indent);
void *dump_link(Link *link, int *indent);

void dump_symbol_heap(Symbol *sym);
