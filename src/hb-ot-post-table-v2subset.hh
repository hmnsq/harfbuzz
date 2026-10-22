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

#اگر_غیر_تعریف_شدہ HB_OT_POST_جدول_V2_SUBSET_HH
#تعریف_کرو HB_OT_POST_جدول_V2_SUBSET_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

نامخلا OT {
سانچہ<قسم_نام Iterator>
HB_اندرونی منطقی post_V2_دم::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        ثابت خالی* _post) ثابت
{
  TRACE_SERIALIZE (یہ);
  خودکار *برآمد = c->آغاز_embed (یہ);
  اگر (unlikely (!c->check_success (برآمد))) return_trace (جھوٹ);
  اگر (!برآمد->glyph_Name_اشاریہ.serialize (c, + it
                                         | hb_map (hb_دوسرا)))
      return_trace (جھوٹ);

  hb_سیٹ_t copied_indices;
  برائے (ثابت خودکار& _ : + it )
  {
    غیرعلامتی glyph_id = _.پہلا;
    غیرعلامتی new_اشاریہ = _.second;

    اگر (new_اشاریہ < 258) جاری;
    اگر (copied_indices.has (new_اشاریہ)) جاری;
    copied_indices.جوڑو (new_اشاریہ);

    hb_بائٹیں_t s = reinterpret_cast<ثابت post::accelerator_t*> (_post)->ڈھونڈو_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_سائز<HBUINT8> (HBUINT8::static_سائز * (s.لمبائی + 1));
    اگر (unlikely (!o)) return_trace (جھوٹ);
    اگر (!c->check_تفویض (o[0], s.لمبائی, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)) return_trace (جھوٹ);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_سائز * s.لمبائی);
  }

  return_trace (سچ);
}

HB_اندرونی منطقی post_V2_دم::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (یہ);

  ثابت hb_map_t &الٹ_glyph_map = *c->plan->الٹ_glyph_map;
  غیرعلامتی عدد_glyphs = c->plan->عدد_output_glyphs ();
  hb_map_t old_new_اشاریہ_map, old_gid_new_اشاریہ_map;
  غیرعلامتی i = 0;

  post::accelerator_t _post (c->plan->ماخذ);

  hb_hashmap_t<hb_بائٹیں_t, uint32_t, سچ> glyph_name_مقصد_new_اشاریہ;

  old_new_اشاریہ_map.alloc (عدد_glyphs);
  old_gid_new_اشاریہ_map.alloc (عدد_glyphs);
  glyph_name_مقصد_new_اشاریہ.alloc (عدد_glyphs);

  برائے (خودکار _ : c->plan->new_مقصد_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    غیرعلامتی old_اشاریہ = glyph_Name_اشاریہ[old_gid];

    غیرعلامتی new_اشاریہ;
    ثابت uint32_t *new_index2;
    اگر (old_اشاریہ <= 257)
      new_اشاریہ = old_اشاریہ;
    ورنہ اگر (old_new_اشاریہ_map.has (old_اشاریہ, &new_index2))
      new_اشاریہ = *new_index2;
    ورنہ
    {
      hb_بائٹیں_t s = _post.ڈھونڈو_glyph_name (old_gid);
      new_اشاریہ = glyph_name_مقصد_new_اشاریہ.get (s);
      اگر (new_اشاریہ == (غیرعلامتی)-1)
      {
        صحیح standard_glyph_اشاریہ = -1;
        برائے (غیرعلامتی i = 0; i < format1_نام_لمبائی; i++)
        {
          اگر (s == format1_نام (i))
          {
            standard_glyph_اشاریہ = i;
            توڑو;
          }
        }

        اگر (standard_glyph_اشاریہ == -1)
        {
          new_اشاریہ = 258 + i;
          i++;
        }
        ورنہ
        { new_اشاریہ = standard_glyph_اشاریہ; }
        glyph_name_مقصد_new_اشاریہ.سیٹ (s, new_اشاریہ);
      }
      old_new_اشاریہ_map.سیٹ (old_اشاریہ, new_اشاریہ);
    }
    old_gid_new_اشاریہ_map.سیٹ (old_gid, new_اشاریہ);
  }

  اگر (old_gid_new_اشاریہ_map.اندر_error())
    return_trace (جھوٹ);

  خودکار اشاریہ_iter =
  + hb_range (عدد_glyphs)
  | hb_map_retains_ترتیب_کاری ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              غیرعلامتی new_اشاریہ = 0;
                              اگر (الٹ_glyph_map.has (new_gid, &old_gid)) {
                                new_اشاریہ = old_gid_new_اشاریہ_map.get (*old_gid);
                                واپس hb_pair_t<غیرعلامتی, غیرعلامتی> (*old_gid, new_اشاریہ);
                              }
                              واپس hb_pair_t<غیرعلامتی, غیرعلامتی> (new_gid, new_اشاریہ);
                            })
  ;

  return_trace (serialize (c->serializer, اشاریہ_iter, &_post));
}

} /* namespace OT */
#اگر_ختم /* HB_OT_POST_TABLE_V2SUBSET_HH */
