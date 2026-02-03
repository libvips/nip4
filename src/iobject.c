// abstract base class for all nip objects

/*

	Copyright (C) 1991-2003 The National Gallery
	Copyright (C) 2004-2023 libvips.org

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
#define DEBUG
#define DEBUG_LEAK
 */

#include "nip4.h"

/* Our signals.
 */
enum {
	SIG_DESTROY, /* End lifetime */
	SIG_CHANGED, /* iObject has changed somehow */
	SIG_LAST
};

G_DEFINE_TYPE(iObject, iobject, G_TYPE_OBJECT)

static guint iobject_signals[SIG_LAST] = { 0 };

#ifdef DEBUG_LEAK
GSList *iobject_all = NULL;
#endif /*DEBUG_LEAK*/

/* Don't emit "destroy" immediately, do it from the _dispose handler, see
 * below.
 */
void *
iobject_destroy(iObject *iobject)
{
#ifdef DEBUG
	printf("iobject_destroy: ");
	iobject_print(iobject);
#endif /*DEBUG*/

	if (!iobject->in_destruction)
		g_object_run_dispose(G_OBJECT(iobject));

	return NULL;
}

void *
iobject_changed(iObject *iobject)
{
	g_return_val_if_fail(iobject != NULL, NULL);
	g_return_val_if_fail(IS_IOBJECT(iobject), NULL);

#ifdef DEBUG
	printf("iobject_changed: ");
	iobject_print(iobject);
#endif /*DEBUG*/

	g_signal_emit(G_OBJECT(iobject), iobject_signals[SIG_CHANGED], 0);

	return NULL;
}

void *
iobject_info(iObject *iobject, VipsBuf *buf, int indent)
{
	iObjectClass *iobject_class = IOBJECT_GET_CLASS(iobject);

	g_return_val_if_fail(iobject != NULL, NULL);
	g_return_val_if_fail(IS_IOBJECT(iobject), NULL);

	if (iobject_class->info)
		iobject_class->info(iobject, buf, indent);

	return NULL;
}

static void
iobject_dispose(GObject *gobject)
{
	iObject *iobject = IOBJECT(gobject);

#ifdef DEBUG
	printf("iobject_dispose: ");
	iobject_print(iobject);
#endif /*DEBUG*/

	if (!iobject->in_destruction) {
		iobject->in_destruction = TRUE;
		g_signal_emit(G_OBJECT(iobject), iobject_signals[SIG_DESTROY], 0);
		iobject->in_destruction = FALSE;
	}

	G_OBJECT_CLASS(iobject_parent_class)->dispose(gobject);
}

static void
iobject_finalize(GObject *gobject)
{
	iObject *iobject = IOBJECT(gobject);

#ifdef DEBUG
	printf("iobject_finalize: ");
	iobject_print(iobject);
#endif /*DEBUG*/

	/* Unlike glib, we allow floating objects to be finalized. Handy if a
	 * _new() fails. So don't assert( !iobject->floating );
	 */

	VIPS_FREE(iobject->name);
	VIPS_FREE(iobject->caption);

#ifdef DEBUG_LEAK
	iobject_all = g_slist_remove(iobject_all, iobject);
#endif /*DEBUG_LEAK*/

	G_OBJECT_CLASS(iobject_parent_class)->finalize(gobject);
}

static void
iobject_real_changed(iObject *iobject)
{
	iObjectClass *iobject_class = IOBJECT_GET_CLASS(iobject);

	if (iobject_class->generate_caption)
		VIPS_SETSTR(iobject->caption,
			iobject_class->generate_caption(iobject));
}

static void
iobject_real_info(iObject *iobject, VipsBuf *buf, int indent)
{
	if (iobject->name)
		vips_buf_appendf(buf, "%*cname = \"%s\"\n",  indent, ' ',
			iobject->name);
	if (iobject->caption)
		vips_buf_appendf(buf, "%*ccaption = \"%s\"\n",  indent, ' ',
			iobject->caption);
	vips_buf_appendf(buf, "%*ciObject :: \"%s\"\n",
		indent, ' ',
		G_OBJECT_TYPE_NAME(iobject));
}

static void
iobject_class_init(iObjectClass *class)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS(class);

	gobject_class->dispose = iobject_dispose;
	gobject_class->finalize = iobject_finalize;

	class->changed = iobject_real_changed;
	class->info = iobject_real_info;
	class->generate_caption = NULL;

	class->user_name = _("object");

	/* Create signals.
	 */
	iobject_signals[SIG_DESTROY] = g_signal_new("destroy",
		G_TYPE_FROM_CLASS(gobject_class),
		G_SIGNAL_RUN_CLEANUP | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
		G_STRUCT_OFFSET(iObjectClass, destroy),
		NULL, NULL,
		g_cclosure_marshal_VOID__VOID,
		G_TYPE_NONE, 0);
	iobject_signals[SIG_CHANGED] = g_signal_new("changed",
		G_OBJECT_CLASS_TYPE(gobject_class),
		G_SIGNAL_RUN_FIRST,
		G_STRUCT_OFFSET(iObjectClass, changed),
		NULL, NULL,
		g_cclosure_marshal_VOID__VOID,
		G_TYPE_NONE, 0);
}

static void
iobject_init(iObject *iobject)
{
#ifdef DEBUG
	printf("iobject_init: ");
	iobject_print(iobject);
#endif /*DEBUG*/

	/* Init our instance fields.
	 */
	iobject->floating = TRUE;

#ifdef DEBUG_LEAK
	iobject_all = g_slist_prepend(iobject_all, iobject);
#endif /*DEBUG_LEAK*/
}

/* Test the name field ... handy with map.
 */
void *
iobject_test_name(iObject *iobject, const char *name)
{
	g_return_val_if_fail(iobject != NULL, NULL);
	g_return_val_if_fail(IS_IOBJECT(iobject), NULL);

	if (iobject->name &&
		strcmp(iobject->name, name) == 0)
		return iobject;

	return NULL;
}

void *
iobject_print(iObject *iobject)
{
	g_print("%s \"%s\" (%p)\n",
		G_OBJECT_TYPE_NAME(iobject), iobject->name, iobject);

	return NULL;
}

void
iobject_set(iObject *iobject, const char *name, const char *caption)
{
	gboolean changed = FALSE;

	g_return_if_fail(iobject != NULL);
	g_return_if_fail(IS_IOBJECT(iobject));

	if (name &&
		name != iobject->name) {
		VIPS_SETSTR(iobject->name, name);
		changed = TRUE;
	}
	if (caption &&
		caption != iobject->caption) {
		VIPS_SETSTR(iobject->caption, caption);
		changed = TRUE;
	}

	if (changed)
		iobject_changed(iobject);

#ifdef DEBUG
	printf("iobject_set: ");
	iobject_print(iobject);
#endif /*DEBUG*/
}

void
iobject_sink(iObject *iobject)
{
	g_assert(IS_IOBJECT(iobject));

	if (iobject->floating) {
		iobject->floating = FALSE;
		g_object_unref(G_OBJECT(iobject));
	}
}

void
iobject_ref_sink(iObject *iobject)
{
	g_assert(IS_IOBJECT(iobject));

	g_object_ref(G_OBJECT(iobject));
	iobject_sink(iobject);
}

void
iobject_dump(iObject *iobject, int indent)
{
	char txt[1000];
	VipsBuf buf = VIPS_BUF_STATIC(txt);

	iobject_info(iobject, &buf, indent);
	printf("%s", vips_buf_all(&buf));
}

const char *
iobject_get_user_name(iObject *object)
{
	const char *user_name = IOBJECT_GET_CLASS(object)->user_name;
	const char *class_name = IOBJECT_GET_CLASS_NAME(object);

	return user_name ? user_name : class_name;
}

void
iobject_leak(void)
{
#ifdef DEBUG_LEAK
	printf("iobject_leak: leak checking ...\n");
	if (iobject_all) {
		printf("leaked objects:\n");
		int n = 0;
		for (GSList *i = iobject_all; i; i = i->next) {
			printf("%d) ", n++);
			iobject_dump(IOBJECT(i->data));
		}
	}
#endif /*DEBUG_LEAK*/
}
