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
create_face (ثابت حرف *معلومات, بینښه اوږدتیا)
{
  hb_face_t *face = hb_face_builder_create ();
  hb_blob_t *blob = hb_blob_create (معلومات, اوږدتیا, HB_حافظه_ډېرپېښ_READONLY,
				    تش_اشاره, تش_اشاره);
  hb_always_assert (hb_face_builder_جمع_کړه_جدول (face, HB_OT_نښان_JSTF, blob));
  hb_blob_destroy (blob);
  أرجع face;
}

صحيح
اصلي (صحيح argc, حرف **argv)
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  hb_always_assert (jstf.get_اوږدتیا ());
  hb_always_assert (jstf->get_script_شمېر () == 1);
  hb_always_assert (jstf->get_script_نښان (0) == HB_نښان ('a','r','a','b'));
  بینښه شاخص_2;
  hb_always_assert (jstf->پیدا_کړه_script_شاخص (HB_نښان ('a','r','a','b'), &شاخص_2));
  hb_always_assert (شاخص_2 == 0);
  hb_always_assert (!jstf->get_script<OT::Layout::MediumTypes> (0).get_lang_sys_شمېر ());
#وإلا
  hb_always_assert (jstf.get_اوږدتیا ());
  hb_always_assert (!jstf->get_script_شمېر ());
#د_که_پای
  jstf.destroy ();
  hb_face_destroy (face);

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  face = create_face (معلومات, حجم (معلومات) - 1);
  jstf = hb_sanitize_context_t ().reference_جدول<OT::JSTF> (face);
  hb_always_assert (!jstf.get_اوږدتیا ());
  jstf.destroy ();
  hb_face_destroy (face);
#د_که_پای

  أرجع 0;
}
