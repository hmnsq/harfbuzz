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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_GLIB

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-glib.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"


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
hb_glib_script_ਮੰਜ਼ਿਲ_script (GUnicodeScript script)
{
  ਵਾਪਸ (hb_script_t) g_unicode_script_ਮੰਜ਼ਿਲ_iso15924 (script);
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
  ਵਾਪਸ g_unicode_script_from_iso15924 (script);
}


ਸਥਿਰ hb_unicode_combining_class_t
hb_glib_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
				 hb_codepoint_t      unicode,
				 ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)

{
  ਵਾਪਸ (hb_unicode_combining_class_t) g_unichar_combining_class (unicode);
}

ਸਥਿਰ hb_unicode_general_category_t
hb_glib_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
				  hb_codepoint_t      unicode,
				  ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)

{
  /* hb_unicode_general_category_t and GUnicodeType are identical */
  ਵਾਪਸ (hb_unicode_general_category_t) g_unichar_type (unicode);
}

ਸਥਿਰ hb_codepoint_t
hb_glib_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
			   hb_codepoint_t      unicode,
			   ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  g_unichar_get_mirror_char (unicode, &unicode);
  ਵਾਪਸ unicode;
}

ਸਥਿਰ hb_script_t
hb_glib_unicode_script (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
			hb_codepoint_t      unicode,
			ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ hb_glib_script_ਮੰਜ਼ਿਲ_script (g_unichar_get_script (unicode));
}

ਸਥਿਰ hb_bool_t
hb_glib_unicode_compose (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
			 hb_codepoint_t      a,
			 hb_codepoint_t      b,
			 hb_codepoint_t     *ab,
			 ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ g_unichar_compose (a, b, ab);
}

ਸਥਿਰ hb_bool_t
hb_glib_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_ਅਣਵਰਤਿਆ,
			   hb_codepoint_t      ab,
			   hb_codepoint_t     *a,
			   hb_codepoint_t     *b,
			   ਖਾਲੀ               *user_ਅੰਕੜੇ HB_ਅਣਵਰਤਿਆ)
{
  ਵਾਪਸ g_unichar_decompose (ab, a, b);
}


ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_glib_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_glib_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_glib_unicode_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_unicode_funcs_t *create ()
  {
    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (ਖਾਲੀ_ਸੂਚਕ);

    hb_unicode_funcs_ਸੈੱਟ_combining_class_func (funcs, hb_glib_unicode_combining_class, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_general_category_func (funcs, hb_glib_unicode_general_category, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_mirroring_func (funcs, hb_glib_unicode_mirroring, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_script_func (funcs, hb_glib_unicode_script, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_compose_func (funcs, hb_glib_unicode_compose, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_unicode_funcs_ਸੈੱਟ_decompose_func (funcs, hb_glib_unicode_decompose, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_glib_funcs);

    ਵਾਪਸ funcs;
  }
} static_glib_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_glib_funcs ()
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
  ਵਾਪਸ static_glib_funcs.get_unconst ();
}



#ਜੇ GLIB_CHECK_ਸੰਸਕਰਣ(2,31,10)

ਸਥਿਰ ਖਾਲੀ
hb_g_ਬਾਈਟਾਂ_unref (ਖਾਲੀ *ਅੰਕੜੇ)
{
  g_ਬਾਈਟਾਂ_unref ((GBytes *) ਅੰਕੜੇ);
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
  gsize ਮਾਪ = 0;
  gconstpointer ਅੰਕੜੇ = g_ਬਾਈਟਾਂ_get_ਅੰਕੜੇ (gbytes, &ਮਾਪ);
  ਵਾਪਸ hb_blob_create ((ਅਚਲ ਅੱਖਰ *) ਅੰਕੜੇ,
			 ਮਾਪ,
			 HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY,
			 g_ਬਾਈਟਾਂ_ref (gbytes),
			 hb_g_ਬਾਈਟਾਂ_unref);
}
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ
