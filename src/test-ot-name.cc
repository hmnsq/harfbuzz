/*
 * Copyright © 2018  Google, Inc.
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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-ot.h"

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#endif

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  यदि (argc != 2) {
    ख_लिखो (मानक_त्रुटि, "usage: %s font-file\n", argv[0]);
    निकास (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  अचिन्हित पूर्णाङ्क गन्ती = 0;

#ifndef HB_NO_NAME
  अचर hb_ot_name_entry_t *entries = hb_ot_name_list_names (face, &गन्ती);

  लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
  {
    म_लिखो ("%u	%s	",
	    entries[i].name_id,
	    hb_language_to_string (entries[i].language));

    वर्ण buf[64];
    अचिन्हित पूर्णाङ्क buf_size = परिमाण (buf);
    hb_ot_name_get_utf8 (face,
			 entries[i].name_id,
			 entries[i].language,
			 &buf_size,
			 buf);

    म_लिखो ("%s\n", buf);
  }
#endif

  hb_face_destroy (face);

  फर्काऊ गन्ती ? 0 : 1;
}
