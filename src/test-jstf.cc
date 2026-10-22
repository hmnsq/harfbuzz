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

#ضمن "hb.hh"
#ضمن "hb-ot-layout-jstf-table.hh"

ساكن hb_face_t *
create_face (ثابت حرف *معلومات, بنانشان ڊيگهائي)
{
  hb_face_t *face = hb_face_builder_create ();
  hb_blob_t *blob = hb_blob_create (معلومات, ڊيگهائي, HB_يادگيري_گهڻائي_READONLY,
				    خالي_اشارو, خالي_اشارو);
  hb_always_assert (hb_face_builder_جوڙيو_جدول (face, HB_OT_ٽيگ_JSTF, blob));
  hb_blob_destroy (blob);
  أرجع face;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  ساكن ثابت حرف معلومات[] = {
    0, 1, 0, 1,				/* version 1.1 */
    0, 0,				/* JstfScriptRecord count */
    0, 1,				/* JstfScriptRecord2 count */
    'a', 'r', 'a', 'b', 0, 0, 0, 16,	/* JstfScriptRecord2 */
    0, 0, 0, 10,			/* ExtenderGlyph2 offset */
    0, 0, 0, 0,				/* default JstfLangSys offset */
    0, 0,				/* JstfLangSysRecord count */
    0, 1, 1, 0, 1,			/* ExtenderGlyph2 */
  };

  hb_face_t *face = create_face (معلومات, حجم (معلومات));
  hb_blob_ptr_t<OT::JSTF> jstf =
      hb_sanitize_context_t ().reference_جدول<OT::JSTF> (face);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  hb_always_assert (jstf.get_ڊيگهائي ());
  hb_always_assert (jstf->get_script_ڳڻپ () == 1);
  hb_always_assert (jstf->get_script_ٽيگ (0) == HB_ٽيگ ('a','r','a','b'));
  بنانشان اشاريو_2;
  hb_always_assert (jstf->ڳوليو_script_اشاريو (HB_ٽيگ ('a','r','a','b'), &اشاريو_2));
  hb_always_assert (اشاريو_2 == 0);
  hb_always_assert (!jstf->get_script<OT::Layout::MediumTypes> (0).get_lang_sys_ڳڻپ ());
#وإلا
  hb_always_assert (jstf.get_ڊيگهائي ());
  hb_always_assert (!jstf->get_script_ڳڻپ ());
#جيڪڏهن_پورو
  jstf.destroy ();
  hb_face_destroy (face);

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  face = create_face (معلومات, حجم (معلومات) - 1);
  jstf = hb_sanitize_context_t ().reference_جدول<OT::JSTF> (face);
  hb_always_assert (!jstf.get_ڊيگهائي ());
  jstf.destroy ();
  hb_face_destroy (face);
#جيڪڏهن_پورو

  أرجع 0;
}
