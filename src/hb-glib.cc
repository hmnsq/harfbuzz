/*
 * Copyright © 2009  Red Hat, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#अन्तर्भाव "hb.hh"

#ifdef HAVE_GLIB

#अन्तर्भाव "hb-glib.h"

#अन्तर्भाव "hb-machinery.hh"


/**
 * SECTION:hb-glib
 * @title: hb-glib
 * @short_description: GLib integration
 * @include: hb-glib.h
 *
 * Functions for using HarfBuzz with the GLib library.
 *
 * HarfBuzz supports using GLib to provide Unicode data, by attaching
 * GLib functions to the virtual methods in a #hb_unicode_funcs_t function
 * structure.
 **/


/**
 * hb_glib_script_to_script:
 * @script: The GUnicodeScript identifier to query
 *
 * Fetches the #hb_script_t script that corresponds to the
 * specified GUnicodeScript identifier.
 *
 * Return value: the #hb_script_t script found
 *
 * Since: 0.9.38
 **/
hb_script_t
hb_glib_script_to_script (GUnicodeScript script)
{
  प्रत्यागम (hb_script_t) g_unicode_script_to_iso15924 (script);
}

/**
 * hb_glib_script_from_script:
 * @script: The #hb_script_t to query
 *
 * Fetches the GUnicodeScript identifier that corresponds to the
 * specified #hb_script_t script.
 *
 * Return value: the GUnicodeScript identifier found
 *
 * Since: 0.9.38
 **/
GUnicodeScript
hb_glib_script_from_script (hb_script_t script)
{
  प्रत्यागम g_unicode_script_from_iso15924 (script);
}


स्थिर hb_unicode_combining_class_t
hb_glib_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				 hb_codepoint_t      unicode,
				 शून्य               *user_data HB_UNUSED)

{
  प्रत्यागम (hb_unicode_combining_class_t) g_unichar_combining_class (unicode);
}

स्थिर hb_unicode_general_category_t
hb_glib_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_UNUSED,
				  hb_codepoint_t      unicode,
				  शून्य               *user_data HB_UNUSED)

{
  /* hb_unicode_general_category_t and GUnicodeType are identical */
  प्रत्यागम (hb_unicode_general_category_t) g_unichar_type (unicode);
}

स्थिर hb_codepoint_t
hb_glib_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			   hb_codepoint_t      unicode,
			   शून्य               *user_data HB_UNUSED)
{
  g_unichar_get_mirror_char (unicode, &unicode);
  प्रत्यागम unicode;
}

स्थिर hb_script_t
hb_glib_unicode_script (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			hb_codepoint_t      unicode,
			शून्य               *user_data HB_UNUSED)
{
  प्रत्यागम hb_glib_script_to_script (g_unichar_get_script (unicode));
}

स्थिर hb_bool_t
hb_glib_unicode_compose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			 hb_codepoint_t      a,
			 hb_codepoint_t      b,
			 hb_codepoint_t     *ab,
			 शून्य               *user_data HB_UNUSED)
{
  प्रत्यागम g_unichar_compose (a, b, ab);
}

स्थिर hb_bool_t
hb_glib_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_UNUSED,
			   hb_codepoint_t      ab,
			   hb_codepoint_t     *a,
			   hb_codepoint_t     *b,
			   शून्य               *user_data HB_UNUSED)
{
  प्रत्यागम g_unichar_decompose (ab, a, b);
}


स्थिर अन्तःस्थ शून्य free_static_glib_funcs ();

स्थिर संरचना hb_glib_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_glib_unicode_funcs_lazy_loader_t>
{
  स्थिर hb_unicode_funcs_t *create ()
  {
    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (nullptr);

    hb_unicode_funcs_set_combining_class_func (funcs, hb_glib_unicode_combining_class, nullptr, nullptr);
    hb_unicode_funcs_set_general_category_func (funcs, hb_glib_unicode_general_category, nullptr, nullptr);
    hb_unicode_funcs_set_mirroring_func (funcs, hb_glib_unicode_mirroring, nullptr, nullptr);
    hb_unicode_funcs_set_script_func (funcs, hb_glib_unicode_script, nullptr, nullptr);
    hb_unicode_funcs_set_compose_func (funcs, hb_glib_unicode_compose, nullptr, nullptr);
    hb_unicode_funcs_set_decompose_func (funcs, hb_glib_unicode_decompose, nullptr, nullptr);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_glib_funcs);

    प्रत्यागम funcs;
  }
} static_glib_funcs;

स्थिर अन्तःस्थ
शून्य free_static_glib_funcs ()
{
  static_glib_funcs.free_instance ();
}

/**
 * hb_glib_get_unicode_funcs:
 *
 * Fetches a Unicode-functions structure that is populated
 * with the appropriate GLib function for each method.
 *
 * Return value: (transfer none): a pointer to the #hb_unicode_funcs_t Unicode-functions structure
 *
 * Since: 0.9.38
 **/
hb_unicode_funcs_t *
hb_glib_get_unicode_funcs ()
{
  प्रत्यागम static_glib_funcs.get_unconst ();
}



#यदि GLIB_CHECK_VERSION(2,31,10)

स्थिर शून्य
_hb_g_bytes_unref (शून्य *data)
{
  g_bytes_unref ((GBytes *) data);
}

/**
 * hb_glib_blob_create:
 * @gbytes: the GBytes structure to work upon
 *
 * Creates an #hb_blob_t blob from the specified
 * GBytes data structure.
 *
 * Return value: (transfer full): the new #hb_blob_t blob object
 *
 * Since: 0.9.38
 **/
hb_blob_t *
hb_glib_blob_create (GBytes *gbytes)
{
  gsize size = 0;
  gconstpointer data = g_bytes_get_data (gbytes, &size);
  प्रत्यागम hb_blob_create ((ध्रुव वर्ण *) data,
			 size,
			 HB_MEMORY_MODE_READONLY,
			 g_bytes_ref (gbytes),
			 _hb_g_bytes_unref);
}
#endif


#endif
