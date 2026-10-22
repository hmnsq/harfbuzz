/*
 * Copyright © 2019  Ebrahim Byagowi
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
 */

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-ot.h"

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  যদি (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    exit (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  unsigned পূর্ণসংখ্যা গুনতি = 0;

#ifndef HB_NO_META
  গুনতি = hb_ot_meta_get_entry_tags (face, 0, nullptr, nullptr);

  hb_ot_meta_tag_t *tags = (hb_ot_meta_tag_t *)
			   malloc (আকার (hb_ot_meta_tag_t) * গুনতি);
  hb_ot_meta_get_entry_tags (face, 0, &গুনতি, tags);
  জন্য (unsigned i = 0; i < গুনতি; ++i)
  {
    hb_blob_t *entry = hb_ot_meta_reference_entry (face, tags[i]);
    printf ("%c%c%c%c, size: %u: %.*s\n",
	    HB_UNTAG (tags[i]), hb_blob_get_দৈর্ঘ্য (entry),
	    (পূর্ণসংখ্যা) hb_blob_get_দৈর্ঘ্য (entry), hb_blob_get_তথ্য (entry, nullptr));
    hb_blob_destroy (entry);
  }
  মুক্ত (tags);
#endif

  hb_face_destroy (face);

  ফেরত !গুনতি;
}
