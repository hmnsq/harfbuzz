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

#అన్తర్భావ "hb.hh"

#నిర్వచితమైతే HAVE_GLIB

#అన్తర్భావ "hb-glib.h"

#అన్తర్భావ "hb-machinery.hh"


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
hb_glib_script_గమ్యం_script (GUnicodeScript script)
{
  తిరిగి (hb_script_t) g_unicode_script_గమ్యం_iso15924 (script);
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
  తిరిగి g_unicode_script_from_iso15924 (script);
}


స్థిర hb_unicode_combining_class_t
hb_glib_unicode_combining_class (hb_unicode_funcs_t *ufuncs HB_వాడని,
				 hb_codepoint_t      unicode,
				 శూన్య               *user_దత్తాంశం HB_వాడని)

{
  తిరిగి (hb_unicode_combining_class_t) g_unichar_combining_class (unicode);
}

స్థిర hb_unicode_general_category_t
hb_glib_unicode_general_category (hb_unicode_funcs_t *ufuncs HB_వాడని,
				  hb_codepoint_t      unicode,
				  శూన్య               *user_దత్తాంశం HB_వాడని)

{
  /* hb_unicode_general_category_t and GUnicodeType are identical */
  తిరిగి (hb_unicode_general_category_t) g_unichar_type (unicode);
}

స్థిర hb_codepoint_t
hb_glib_unicode_mirroring (hb_unicode_funcs_t *ufuncs HB_వాడని,
			   hb_codepoint_t      unicode,
			   శూన్య               *user_దత్తాంశం HB_వాడని)
{
  g_unichar_get_mirror_char (unicode, &unicode);
  తిరిగి unicode;
}

స్థిర hb_script_t
hb_glib_unicode_script (hb_unicode_funcs_t *ufuncs HB_వాడని,
			hb_codepoint_t      unicode,
			శూన్య               *user_దత్తాంశం HB_వాడని)
{
  తిరిగి hb_glib_script_గమ్యం_script (g_unichar_get_script (unicode));
}

స్థిర hb_bool_t
hb_glib_unicode_compose (hb_unicode_funcs_t *ufuncs HB_వాడని,
			 hb_codepoint_t      a,
			 hb_codepoint_t      b,
			 hb_codepoint_t     *ab,
			 శూన్య               *user_దత్తాంశం HB_వాడని)
{
  తిరిగి g_unichar_compose (a, b, ab);
}

స్థిర hb_bool_t
hb_glib_unicode_decompose (hb_unicode_funcs_t *ufuncs HB_వాడని,
			   hb_codepoint_t      ab,
			   hb_codepoint_t     *a,
			   hb_codepoint_t     *b,
			   శూన్య               *user_దత్తాంశం HB_వాడని)
{
  తిరిగి g_unichar_decompose (ab, a, b);
}


స్థిర అంతఃవరుస శూన్య free_static_glib_funcs ();

స్థిర నిర్మాణం hb_glib_unicode_funcs_lazy_loader_t : hb_unicode_funcs_lazy_loader_t<hb_glib_unicode_funcs_lazy_loader_t>
{
  స్థిర hb_unicode_funcs_t *create ()
  {
    hb_unicode_funcs_t *funcs = hb_unicode_funcs_create (శూన్యసూచిక);

    hb_unicode_funcs_సమితి_combining_class_func (funcs, hb_glib_unicode_combining_class, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_general_category_func (funcs, hb_glib_unicode_general_category, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_mirroring_func (funcs, hb_glib_unicode_mirroring, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_script_func (funcs, hb_glib_unicode_script, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_compose_func (funcs, hb_glib_unicode_compose, శూన్యసూచిక, శూన్యసూచిక);
    hb_unicode_funcs_సమితి_decompose_func (funcs, hb_glib_unicode_decompose, శూన్యసూచిక, శూన్యసూచిక);

    hb_unicode_funcs_make_immutable (funcs);

    hb_atexit (free_static_glib_funcs);

    తిరిగి funcs;
  }
} static_glib_funcs;

స్థిర అంతఃవరుస
శూన్య free_static_glib_funcs ()
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
  తిరిగి static_glib_funcs.get_unconst ();
}



#ఒకవేళ GLIB_CHECK_సంస్కరణ(2,31,10)

స్థిర శూన్య
hb_g_బైట్లు_unref (శూన్య *దత్తాంశం)
{
  g_బైట్లు_unref ((GBytes *) దత్తాంశం);
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
  gsize కొలత = 0;
  gconstpointer దత్తాంశం = g_బైట్లు_get_దత్తాంశం (gbytes, &కొలత);
  తిరిగి hb_blob_create ((స్థిరరాశి అక్షరం *) దత్తాంశం,
			 కొలత,
			 HB_జ్ఞాపకం_బహుళకం_READONLY,
			 g_బైట్లు_ref (gbytes),
			 hb_g_బైట్లు_unref);
}
#అయితే_ముగింపు


#అయితే_ముగింపు
