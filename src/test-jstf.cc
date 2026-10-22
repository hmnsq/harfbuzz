/*
 * Copyright © 2026  Behdad Esfahbod
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
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-layout-jstf-table.hh"

ਸਥਿਰ hb_face_t *
create_face (ਅਚਲ ਅੱਖਰ *ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾਈ)
{
  hb_face_t *face = hb_face_builder_create ();
  hb_blob_t *blob = hb_blob_create (ਅੰਕੜੇ, ਲੰਮਾਈ, HB_ਸਮ੍ਰਿਤੀ_ਬਹੁਲਕ_READONLY,
				    ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
  hb_always_assert (hb_face_builder_ਜੋੜੋ_ਸਾਰਣੀ (face, HB_OT_ਟੈਗ_JSTF, blob));
  hb_blob_destroy (blob);
  ਵਾਪਸ face;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਸਥਿਰ ਅਚਲ ਅੱਖਰ ਅੰਕੜੇ[] = {
    0, 1, 0, 1,				/* version 1.1 */
    0, 0,				/* JstfScriptRecord count */
    0, 1,				/* JstfScriptRecord2 count */
    'a', 'r', 'a', 'b', 0, 0, 0, 16,	/* JstfScriptRecord2 */
    0, 0, 0, 10,			/* ExtenderGlyph2 offset */
    0, 0, 0, 0,				/* default JstfLangSys offset */
    0, 0,				/* JstfLangSysRecord count */
    0, 1, 1, 0, 1,			/* ExtenderGlyph2 */
  };

  hb_face_t *face = create_face (ਅੰਕੜੇ, ਆਕਾਰ (ਅੰਕੜੇ));
  hb_blob_ptr_t<OT::JSTF> jstf =
      hb_sanitize_context_t ().reference_ਸਾਰਣੀ<OT::JSTF> (face);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  hb_always_assert (jstf.get_ਲੰਮਾਈ ());
  hb_always_assert (jstf->get_script_ਗਿਣਤੀ () == 1);
  hb_always_assert (jstf->get_script_ਟੈਗ (0) == HB_ਟੈਗ ('a','r','a','b'));
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2;
  hb_always_assert (jstf->ਲੱਭੋ_script_ਸੂਚਕਾਂਕ (HB_ਟੈਗ ('a','r','a','b'), &ਸੂਚਕਾਂਕ_2));
  hb_always_assert (ਸੂਚਕਾਂਕ_2 == 0);
  hb_always_assert (!jstf->get_script<OT::Layout::MediumTypes> (0).get_lang_sys_ਗਿਣਤੀ ());
#ਨਹੀਂਤਾਂ
  hb_always_assert (jstf.get_ਲੰਮਾਈ ());
  hb_always_assert (!jstf->get_script_ਗਿਣਤੀ ());
#ਜੇ_ਅੰਤ
  jstf.destroy ();
  hb_face_destroy (face);

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  face = create_face (ਅੰਕੜੇ, ਆਕਾਰ (ਅੰਕੜੇ) - 1);
  jstf = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<OT::JSTF> (face);
  hb_always_assert (!jstf.get_ਲੰਮਾਈ ());
  jstf.destroy ();
  hb_face_destroy (face);
#ਜੇ_ਅੰਤ

  ਵਾਪਸ 0;
}
