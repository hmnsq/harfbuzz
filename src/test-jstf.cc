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

#समावेश "hb.hh"
#समावेश "hb-ot-layout-jstf-table.hh"

अटल hb_face_t *
create_face (स्थिर अक्षर *तथ्य, अचिन्हित लंबाई)
{
  hb_face_t *face = hb_face_builder_create ();
  hb_blob_t *blob = hb_blob_create (तथ्य, लंबाई, HB_MEMORY_MODE_READONLY,
				    nullptr, nullptr);
  hb_always_assert (hb_face_builder_add_table (face, HB_OT_TAG_JSTF, blob));
  hb_blob_destroy (blob);
  वापस face;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अटल स्थिर अक्षर तथ्य[] = {
    0, 1, 0, 1,				/* version 1.1 */
    0, 0,				/* JstfScriptRecord count */
    0, 1,				/* JstfScriptRecord2 count */
    'a', 'r', 'a', 'b', 0, 0, 0, 16,	/* JstfScriptRecord2 */
    0, 0, 0, 10,			/* ExtenderGlyph2 offset */
    0, 0, 0, 0,				/* default JstfLangSys offset */
    0, 0,				/* JstfLangSysRecord count */
    0, 1, 1, 0, 1,			/* ExtenderGlyph2 */
  };

  hb_face_t *face = create_face (तथ्य, माप (तथ्य));
  hb_blob_ptr_t<OT::JSTF> jstf =
      hb_sanitize_context_t ().reference_table<OT::JSTF> (face);
#अगर_अघोषित HB_NO_BEYOND_64K
  hb_always_assert (jstf.get_लंबाई ());
  hb_always_assert (jstf->get_script_गिनती () == 1);
  hb_always_assert (jstf->get_script_tag (0) == HB_TAG ('a','r','a','b'));
  अचिन्हित सूचकांक_2;
  hb_always_assert (jstf->find_script_सूचकांक (HB_TAG ('a','r','a','b'), &सूचकांक_2));
  hb_always_assert (सूचकांक_2 == 0);
  hb_always_assert (!jstf->get_script<OT::Layout::MediumTypes> (0).get_lang_sys_गिनती ());
#अन्यथा
  hb_always_assert (jstf.get_लंबाई ());
  hb_always_assert (!jstf->get_script_गिनती ());
#पूर्ण_अगर
  jstf.destroy ();
  hb_face_destroy (face);

#अगर_अघोषित HB_NO_BEYOND_64K
  face = create_face (तथ्य, माप (तथ्य) - 1);
  jstf = hb_sanitize_context_t ().reference_table<OT::JSTF> (face);
  hb_always_assert (!jstf.get_लंबाई ());
  jstf.destroy ();
  hb_face_destroy (face);
#पूर्ण_अगर

  वापस 0;
}
