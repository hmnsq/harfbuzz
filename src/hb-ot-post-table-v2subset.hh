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

#جے_بنا_تعریف HB_OT_POST_جدول_V2_SUBSET_HH
#تعریف_کرو HB_OT_POST_جدول_V2_SUBSET_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

نطاق OT {
سانچا<قسم_ناں Iterator>
HB_اندرونی منطقي post_V2_پوچھ::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        ثابت فراغ* _post) ثابت
{
  TRACE_SERIALIZE (هذا);
  خودکار *باہرلا = c->شروع_embed (هذا);
  إذا (unlikely (!c->check_success (باہرلا))) return_trace (خطأ);
  إذا (!باہرلا->glyph_Name_اشاریہ.serialize (c, + it
                                         | hb_map (hb_دوجا)))
      return_trace (خطأ);

  hb_سیٹ_t copied_indices;
  لكل (ثابت خودکار& _ : + it )
  {
    بنانشانی glyph_id = _.پہلا;
    بنانشانی new_اشاریہ = _.second;

    إذا (new_اشاریہ < 258) استمر;
    إذا (copied_indices.has (new_اشاریہ)) استمر;
    copied_indices.جوڑو (new_اشاریہ);

    hb_بائٹاں_t s = reinterpret_cast<ثابت post::accelerator_t*> (_post)->لبھو_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_ناپ<HBUINT8> (HBUINT8::static_ناپ * (s.لمائی + 1));
    إذا (unlikely (!o)) return_trace (خطأ);
    إذا (!c->check_سونپو (o[0], s.لمائی, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)) return_trace (خطأ);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_ناپ * s.لمائی);
  }

  return_trace (سچ);
}

HB_اندرونی منطقي post_V2_پوچھ::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);

  ثابت hb_map_t &الٹ_glyph_map = *c->plan->الٹ_glyph_map;
  بنانشانی عدد_glyphs = c->plan->عدد_output_glyphs ();
  hb_map_t old_new_اشاریہ_map, old_gid_new_اشاریہ_map;
  بنانشانی i = 0;

  post::accelerator_t _post (c->plan->سروت);

  hb_hashmap_t<hb_بائٹاں_t, uint32_t, سچ> glyph_name_منزل_new_اشاریہ;

  old_new_اشاریہ_map.alloc (عدد_glyphs);
  old_gid_new_اشاریہ_map.alloc (عدد_glyphs);
  glyph_name_منزل_new_اشاریہ.alloc (عدد_glyphs);

  لكل (خودکار _ : c->plan->new_منزل_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    بنانشانی old_اشاریہ = glyph_Name_اشاریہ[old_gid];

    بنانشانی new_اشاریہ;
    ثابت uint32_t *new_index2;
    إذا (old_اشاریہ <= 257)
      new_اشاریہ = old_اشاریہ;
    وإلا إذا (old_new_اشاریہ_map.has (old_اشاریہ, &new_index2))
      new_اشاریہ = *new_index2;
    وإلا
    {
      hb_بائٹاں_t s = _post.لبھو_glyph_name (old_gid);
      new_اشاریہ = glyph_name_منزل_new_اشاریہ.get (s);
      إذا (new_اشاریہ == (بنانشانی)-1)
      {
        صحيح standard_glyph_اشاریہ = -1;
        لكل (بنانشانی i = 0; i < format1_ناں_لمائی; i++)
        {
          إذا (s == format1_ناں (i))
          {
            standard_glyph_اشاریہ = i;
            اكسر;
          }
        }

        إذا (standard_glyph_اشاریہ == -1)
        {
          new_اشاریہ = 258 + i;
          i++;
        }
        وإلا
        { new_اشاریہ = standard_glyph_اشاریہ; }
        glyph_name_منزل_new_اشاریہ.سیٹ (s, new_اشاریہ);
      }
      old_new_اشاریہ_map.سیٹ (old_اشاریہ, new_اشاریہ);
    }
    old_gid_new_اشاریہ_map.سیٹ (old_gid, new_اشاریہ);
  }

  إذا (old_gid_new_اشاریہ_map.وچ_error())
    return_trace (خطأ);

  خودکار اشاریہ_iter =
  + hb_range (عدد_glyphs)
  | hb_map_retains_چھانٹی ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              بنانشانی new_اشاریہ = 0;
                              إذا (الٹ_glyph_map.has (new_gid, &old_gid)) {
                                new_اشاریہ = old_gid_new_اشاریہ_map.get (*old_gid);
                                أرجع hb_pair_t<بنانشانی, بنانشانی> (*old_gid, new_اشاریہ);
                              }
                              أرجع hb_pair_t<بنانشانی, بنانشانی> (new_gid, new_اشاریہ);
                            })
  ;

  return_trace (serialize (c->serializer, اشاریہ_iter, &_post));
}

} /* namespace OT */
#جے_انت /* HB_OT_POST_TABLE_V2SUBSET_HH */
