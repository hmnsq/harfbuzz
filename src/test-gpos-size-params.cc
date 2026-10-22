/*
 * Copyright © 2010,2011  Google, Inc.
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

#समावेश "hb.hh"

#समावेश "hb.h"
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

  /* Create the face */
  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = nullptr;

  bool ret = true;

#अगर_अघोषित HB_NO_LAYOUT_FEATURE_PARAMS
  अचिन्हित पूर्णांक p[5];
  ret = hb_ot_layout_get_आकार_params (face, p, p+1, (p+2), p+3, p+4);
  म_लिखो ("%g %u %u %g %g\n", p[0]/10., p[1], p[2], p[3]/10., p[4]/10.);
#पूर्ण_अगर

  hb_face_destroy (face);

  वापस !ret;
}
