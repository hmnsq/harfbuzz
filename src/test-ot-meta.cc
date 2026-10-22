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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_NO_OPEN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ hb_blob_create_from_file_or_fail(x)  hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ()
#ਜੇ_ਅੰਤ

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਜੇ (argc != 2) {
    fprintf (stderr, "usage: %s font-file\n", argv[0]);
    ਬਾਹਰ_ਨਿਕਲੋ (1);
  }

  hb_blob_t *blob = hb_blob_create_from_file_or_fail (argv[1]);
  hb_always_assert (blob);
  hb_face_t *face = hb_face_create (blob, 0 /* first face */);
  hb_blob_destroy (blob);
  blob = ਖਾਲੀ_ਸੂਚਕ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = 0;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_META
  ਗਿਣਤੀ_2 = hb_ot_meta_get_ਇੰਦਰਾਜ_ਟੈਗ_ਸੂਚੀ (face, 0, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);

  hb_ot_meta_ਟੈਗ_t *ਟੈਗ_ਸੂਚੀ = (hb_ot_meta_ਟੈਗ_t *)
			   ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਆਕਾਰ (hb_ot_meta_ਟੈਗ_t) * ਗਿਣਤੀ_2);
  hb_ot_meta_get_ਇੰਦਰਾਜ_ਟੈਗ_ਸੂਚੀ (face, 0, &ਗਿਣਤੀ_2, ਟੈਗ_ਸੂਚੀ);
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਗਿਣਤੀ_2; ++i)
  {
    hb_blob_t *ਇੰਦਰਾਜ = hb_ot_meta_reference_ਇੰਦਰਾਜ (face, ਟੈਗ_ਸੂਚੀ[i]);
    ਲਿਖੋ ("%c%c%c%c, size: %u: %.*s\n",
	    HB_UNTAG (ਟੈਗ_ਸੂਚੀ[i]), hb_blob_get_ਲੰਮਾਈ (ਇੰਦਰਾਜ),
	    (ਪੂਰਨ) hb_blob_get_ਲੰਮਾਈ (ਇੰਦਰਾਜ), hb_blob_get_ਅੰਕੜੇ (ਇੰਦਰਾਜ, ਖਾਲੀ_ਸੂਚਕ));
    hb_blob_destroy (ਇੰਦਰਾਜ);
  }
  ਮੁਕਤ (ਟੈਗ_ਸੂਚੀ);
#ਜੇ_ਅੰਤ

  hb_face_destroy (face);

  ਵਾਪਸ !ਗਿਣਤੀ_2;
}
