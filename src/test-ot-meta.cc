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

#समावेश "hb.hh"
#समावेश "hb-ot.h"

#अगर_घोषित HB_NO_OPEN
#घोषणा hb_blob_create_from_file_or_fail(x)  hb_blob_get_empty ()
#पूर्ण_अगर

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अगर (argc != 2) {
    ख_लिखो (मानक_त्रुटि, "usage: %s font-file\n", argv[0]);
    निकास (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  अचिन्हित पूर्णांक गिनती = 0;

#अगर_अघोषित HB_NO_META
  गिनती = hb_ot_meta_get_entry_tags (face, 0, nullptr, nullptr);

  hb_ot_meta_tag_t *tags = (hb_ot_meta_tag_t *)
			   दो_स्मृति (माप (hb_ot_meta_tag_t) * गिनती);
  hb_ot_meta_get_entry_tags (face, 0, &गिनती, tags);
  क्रम (अचिन्हित i = 0; i < गिनती; ++i)
  {
    hb_blob_t *entry = hb_ot_meta_reference_entry (face, tags[i]);
    म_लिखो ("%c%c%c%c, size: %u: %.*s\n",
	    HB_UNTAG (tags[i]), hb_blob_get_लंबाई (entry),
	    (पूर्णांक) hb_blob_get_लंबाई (entry), hb_blob_get_तथ्य (entry, nullptr));
    hb_blob_destroy (entry);
  }
  मुक्त (tags);
#पूर्ण_अगर

  hb_face_destroy (face);

  वापस !गिनती;
}
