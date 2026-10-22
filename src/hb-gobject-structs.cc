/*
 * Copyright © 2011  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#شامل "hb.hh"

#اگر_تعریف_شدہ HAVE_GOBJECT


/*
 * SECTION:hb-gobject
 * @title: hb-gobject
 * @short_description: GObject integration support
 * @include: hb-gobject.h
 *
 * Support for using HarfBuzz with the GObject library to provide
 * type data.
 *
 * The types and functions listed here are solely a linkage between
 * HarfBuzz's public data types and the GTypes used by the GObject framework.
 * HarfBuzz uses GObject introspection to generate its Python bindings 
 * (and potentially other language bindings); client programs should never need
 * to access the GObject-integration mechanics.
 *
 * For client programs using the GNOME and GTK software stack, please see the
 * GLib and FreeType integration pages.
 **/


/* g++ didn't like older gtype.h gcc-only code path. */
#شامل <glib.h>

#شامل "hb-gobject.h"

#تعریف_کرو HB_DEFINE_BOXED_TYPE(name,copy_func,free_func) \
GType \
hb_gobject_##name##_get_type () \
{ \
   ساکن gsize type_id = 0; \
   اگر (g_once_init_enter (&type_id)) { \
      GType id = g_boxed_type_رجسٹر_static (g_intern_static_string ("hb_" #name "_t"), \
					       (GBoxedCopyFunc) copy_func, \
					       (GBoxedFreeFunc) free_func); \
      g_once_init_leave (&type_id, id); \
   } \
   واپس type_id; \
}

#تعریف_کرو HB_DEFINE_OBJECT_TYPE(name) \
	HB_DEFINE_BOXED_TYPE (name, hb_##name##_reference, hb_##name##_destroy)

#تعریف_کرو HB_DEFINE_قدر_TYPE(name) \
	ساکن hb_##name##_t *_hb_##name##_reference (ثابت hb_##name##_t *l) \
	{ \
	  hb_##name##_t *c = (hb_##name##_t *) hb_calloc (1, حجم (hb_##name##_t)); \
	  اگر (unlikely (!c)) واپس خالی_اشاریہ; \
	  *c = *l; \
	  واپس c; \
	} \
	ساکن خالی _hb_##name##_destroy (hb_##name##_t *l) { hb_free (l); } \
	HB_DEFINE_BOXED_TYPE (name, _hb_##name##_reference, _hb_##name##_destroy)

HB_DEFINE_OBJECT_TYPE (بفر)
HB_DEFINE_OBJECT_TYPE (blob)
HB_DEFINE_OBJECT_TYPE (draw_funcs)
HB_DEFINE_OBJECT_TYPE (paint_funcs)
HB_DEFINE_OBJECT_TYPE (face)
HB_DEFINE_OBJECT_TYPE (font)
HB_DEFINE_OBJECT_TYPE (font_funcs)
HB_DEFINE_OBJECT_TYPE (سیٹ)
HB_DEFINE_OBJECT_TYPE (map)
HB_DEFINE_OBJECT_TYPE (shape_plan)
HB_DEFINE_OBJECT_TYPE (unicode_funcs)
HB_DEFINE_قدر_TYPE (feature)
HB_DEFINE_قدر_TYPE (glyph_info)
HB_DEFINE_قدر_TYPE (glyph_position)
HB_DEFINE_قدر_TYPE (segment_properties)
HB_DEFINE_قدر_TYPE (draw_state)
HB_DEFINE_قدر_TYPE (color_stop)
HB_DEFINE_قدر_TYPE (color_سطر)
HB_DEFINE_قدر_TYPE (user_ڈیٹا_کلید)

HB_DEFINE_قدر_TYPE (ot_var_axis_info)
HB_DEFINE_قدر_TYPE (ot_math_glyph_variant)
HB_DEFINE_قدر_TYPE (ot_math_glyph_part)


#اگر_ختم
