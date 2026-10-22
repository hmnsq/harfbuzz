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

#که_نه_تعریف_شوی HB_OT_POST_جدول_V2_SUBSET_HH
#تعریف_کړه HB_OT_POST_جدول_V2_SUBSET_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

نطاق OT {
کينډۍ<ډول_نوم Iterator>
HB_دننی منطقي post_V2_لکۍ::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        ثابت فراغ* _post) ثابت
{
  TRACE_SERIALIZE (هذا);
  خپلکاری *بهر = c->پیل_embed (هذا);
  إذا (unlikely (!c->check_success (بهر))) return_trace (خطأ);
  إذا (!بهر->glyph_Name_شاخص.serialize (c, + it
                                         | hb_map (hb_دوهم)))
      return_trace (خطأ);

  hb_سېټ_t copied_indices;
  لكل (ثابت خپلکاری& _ : + it )
  {
    بینښه glyph_id = _.لومړی;
    بینښه new_شاخص = _.second;

    إذا (new_شاخص < 258) استمر;
    إذا (copied_indices.has (new_شاخص)) استمر;
    copied_indices.جمع_کړه (new_شاخص);

    hb_بایټونه_t s = reinterpret_cast<ثابت post::accelerator_t*> (_post)->پیدا_کړه_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_اندازه<HBUINT8> (HBUINT8::static_اندازه * (s.اوږدتیا + 1));
    إذا (unlikely (!o)) return_trace (خطأ);
    إذا (!c->check_ټاکل (o[0], s.اوږدتیا, HB_SERIALIZE_ERROR_INT_ډکېدنه)) return_trace (خطأ);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_اندازه * s.اوږدتیا);
  }

  return_trace (ریښتیا);
}

HB_دننی منطقي post_V2_لکۍ::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);

  ثابت hb_map_t &سرچپه_glyph_map = *c->plan->سرچپه_glyph_map;
  بینښه عدد_glyphs = c->plan->عدد_output_glyphs ();
  hb_map_t old_new_شاخص_map, old_gid_new_شاخص_map;
  بینښه i = 0;

  post::accelerator_t _post (c->plan->سرچینه);

  hb_hashmap_t<hb_بایټونه_t, uint32_t, ریښتیا> glyph_name_منزل_new_شاخص;

  old_new_شاخص_map.alloc (عدد_glyphs);
  old_gid_new_شاخص_map.alloc (عدد_glyphs);
  glyph_name_منزل_new_شاخص.alloc (عدد_glyphs);

  لكل (خپلکاری _ : c->plan->new_منزل_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    بینښه old_شاخص = glyph_Name_شاخص[old_gid];

    بینښه new_شاخص;
    ثابت uint32_t *new_index2;
    إذا (old_شاخص <= 257)
      new_شاخص = old_شاخص;
    وإلا إذا (old_new_شاخص_map.has (old_شاخص, &new_index2))
      new_شاخص = *new_index2;
    وإلا
    {
      hb_بایټونه_t s = _post.پیدا_کړه_glyph_name (old_gid);
      new_شاخص = glyph_name_منزل_new_شاخص.get (s);
      إذا (new_شاخص == (بینښه)-1)
      {
        صحيح standard_glyph_شاخص = -1;
        لكل (بینښه i = 0; i < format1_نومونه_اوږدتیا; i++)
        {
          إذا (s == format1_نومونه (i))
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
        glyph_name_منزل_new_شاخص.سېټ (s, new_شاخص);
      }
      old_new_شاخص_map.سېټ (old_شاخص, new_شاخص);
    }
    old_gid_new_شاخص_map.سېټ (old_gid, new_شاخص);
  }

  إذا (old_gid_new_شاخص_map.دننه_error())
    return_trace (خطأ);

  خپلکاری شاخص_iter =
  + hb_range (عدد_glyphs)
  | hb_map_retains_ترتیبونه ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              بینښه new_شاخص = 0;
                              إذا (سرچپه_glyph_map.has (new_gid, &old_gid)) {
                                new_شاخص = old_gid_new_شاخص_map.get (*old_gid);
                                أرجع hb_pair_t<بینښه, بینښه> (*old_gid, new_شاخص);
                              }
                              أرجع hb_pair_t<بینښه, بینښه> (new_gid, new_شاخص);
                            })
  ;

  return_trace (serialize (c->serializer, شاخص_iter, &_post));
}

} /* namespace OT */
#د_که_پای /* HB_OT_POST_TABLE_V2SUBSET_HH */
