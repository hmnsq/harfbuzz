/*
 * Copyright © 2021  Google, Inc.
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
 */

#اگر_تعریف_نشده HB_OT_POST_جدول_V2_SUBSET_HH
#تعریف_کن HB_OT_POST_جدول_V2_SUBSET_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

نطاق OT {
قالب<نام_نوع Iterator>
HB_داخلی منطقي post_V2_دم::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        ثابت فراغ* _post) ثابت
{
  TRACE_SERIALIZE (هذا);
  خودکار *بیرون = c->شروع_embed (هذا);
  إذا (unlikely (!c->check_success (بیرون))) return_trace (خطأ);
  إذا (!بیرون->glyph_Name_شاخص.serialize (c, + it
                                         | hb_map (hb_دومی)))
      return_trace (خطأ);

  hb_مجموعه_t copied_indices;
  لكل (ثابت خودکار& _ : + it )
  {
    بیعلامت glyph_id = _.اولی;
    بیعلامت new_شاخص = _.second;

    إذا (new_شاخص < 258) استمر;
    إذا (copied_indices.has (new_شاخص)) استمر;
    copied_indices.جمع_کن (new_شاخص);

    hb_بایتها_t s = reinterpret_cast<ثابت post::accelerator_t*> (_post)->پیدا_کن_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_اندازه<HBUINT8> (HBUINT8::static_اندازه * (s.درازی + 1));
    إذا (unlikely (!o)) return_trace (خطأ);
    إذا (!c->check_تخصیص_بده (o[0], s.درازی, HB_SERIALIZE_ERROR_INT_سرریز)) return_trace (خطأ);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_اندازه * s.درازی);
  }

  return_trace (درست);
}

HB_داخلی منطقي post_V2_دم::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);

  ثابت hb_map_t &معکوس_glyph_map = *c->plan->معکوس_glyph_map;
  بیعلامت رقم_glyphs = c->plan->رقم_output_glyphs ();
  hb_map_t old_new_شاخص_map, old_gid_new_شاخص_map;
  بیعلامت i = 0;

  post::accelerator_t _post (c->plan->منبع);

  hb_hashmap_t<hb_بایتها_t, uint32_t, درست> glyph_name_مقصد_new_شاخص;

  old_new_شاخص_map.alloc (رقم_glyphs);
  old_gid_new_شاخص_map.alloc (رقم_glyphs);
  glyph_name_مقصد_new_شاخص.alloc (رقم_glyphs);

  لكل (خودکار _ : c->plan->new_مقصد_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    بیعلامت old_شاخص = glyph_Name_شاخص[old_gid];

    بیعلامت new_شاخص;
    ثابت uint32_t *new_index2;
    إذا (old_شاخص <= 257)
      new_شاخص = old_شاخص;
    وإلا إذا (old_new_شاخص_map.has (old_شاخص, &new_index2))
      new_شاخص = *new_index2;
    وإلا
    {
      hb_بایتها_t s = _post.پیدا_کن_glyph_name (old_gid);
      new_شاخص = glyph_name_مقصد_new_شاخص.get (s);
      إذا (new_شاخص == (بیعلامت)-1)
      {
        صحيح standard_glyph_شاخص = -1;
        لكل (بیعلامت i = 0; i < format1_نامها_درازی; i++)
        {
          إذا (s == format1_نامها (i))
          {
            standard_glyph_شاخص = i;
            اكسر;
          }
        }

        إذا (standard_glyph_شاخص == -1)
        {
          new_شاخص = 258 + i;
          i++;
        }
        وإلا
        { new_شاخص = standard_glyph_شاخص; }
        glyph_name_مقصد_new_شاخص.مجموعه (s, new_شاخص);
      }
      old_new_شاخص_map.مجموعه (old_شاخص, new_شاخص);
    }
    old_gid_new_شاخص_map.مجموعه (old_gid, new_شاخص);
  }

  إذا (old_gid_new_شاخص_map.درون_error())
    return_trace (خطأ);

  خودکار شاخص_iter =
  + hb_range (رقم_glyphs)
  | hb_map_retains_ترتیبدهی ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              بیعلامت new_شاخص = 0;
                              إذا (معکوس_glyph_map.has (new_gid, &old_gid)) {
                                new_شاخص = old_gid_new_شاخص_map.get (*old_gid);
                                أرجع hb_pair_t<بیعلامت, بیعلامت> (*old_gid, new_شاخص);
                              }
                              أرجع hb_pair_t<بیعلامت, بیعلامت> (new_gid, new_شاخص);
                            })
  ;

  return_trace (serialize (c->serializer, شاخص_iter, &_post));
}

} /* namespace OT */
#پایان_اگر /* HB_OT_POST_TABLE_V2SUBSET_HH */
