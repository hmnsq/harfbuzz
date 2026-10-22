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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_POST_جدول_V2_SUBSET_HH
#وصف_ڪريو HB_OT_POST_جدول_V2_SUBSET_HH

#ضمن "hb-open-type.hh"
#ضمن "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

نطاق OT {
سانچو<قسم_نالو Iterator>
HB_اندروني منطقي post_V2_پڇ::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        ثابت فراغ* _post) ثابت
{
  TRACE_SERIALIZE (هذا);
  خودڪار *ٻاهريون = c->شروعات_embed (هذا);
  إذا (unlikely (!c->check_success (ٻاهريون))) return_trace (خطأ);
  إذا (!ٻاهريون->glyph_Name_اشاريو.serialize (c, + it
                                         | hb_map (hb_ٻيو)))
      return_trace (خطأ);

  hb_سيٽ_t copied_indices;
  لكل (ثابت خودڪار& _ : + it )
  {
    بنانشان glyph_id = _.پهريون;
    بنانشان new_اشاريو = _.second;

    إذا (new_اشاريو < 258) استمر;
    إذا (copied_indices.has (new_اشاريو)) استمر;
    copied_indices.جوڙيو (new_اشاريو);

    hb_بائيٽون_t s = reinterpret_cast<ثابت post::accelerator_t*> (_post)->ڳوليو_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_سائيز<HBUINT8> (HBUINT8::static_سائيز * (s.ڊيگهائي + 1));
    إذا (unlikely (!o)) return_trace (خطأ);
    إذا (!c->check_سونپيو (o[0], s.ڊيگهائي, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)) return_trace (خطأ);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_سائيز * s.ڊيگهائي);
  }

  return_trace (سچ);
}

HB_اندروني منطقي post_V2_پڇ::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (هذا);

  ثابت hb_map_t &ابتو_glyph_map = *c->plan->ابتو_glyph_map;
  بنانشان انگ_glyphs = c->plan->انگ_output_glyphs ();
  hb_map_t old_new_اشاريو_map, old_gid_new_اشاريو_map;
  بنانشان i = 0;

  post::accelerator_t _post (c->plan->ذريعو);

  hb_hashmap_t<hb_بائيٽون_t, uint32_t, سچ> glyph_name_منزل_new_اشاريو;

  old_new_اشاريو_map.alloc (انگ_glyphs);
  old_gid_new_اشاريو_map.alloc (انگ_glyphs);
  glyph_name_منزل_new_اشاريو.alloc (انگ_glyphs);

  لكل (خودڪار _ : c->plan->new_منزل_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    بنانشان old_اشاريو = glyph_Name_اشاريو[old_gid];

    بنانشان new_اشاريو;
    ثابت uint32_t *new_index2;
    إذا (old_اشاريو <= 257)
      new_اشاريو = old_اشاريو;
    وإلا إذا (old_new_اشاريو_map.has (old_اشاريو, &new_index2))
      new_اشاريو = *new_index2;
    وإلا
    {
      hb_بائيٽون_t s = _post.ڳوليو_glyph_name (old_gid);
      new_اشاريو = glyph_name_منزل_new_اشاريو.get (s);
      إذا (new_اشاريو == (بنانشان)-1)
      {
        صحيح standard_glyph_اشاريو = -1;
        لكل (بنانشان i = 0; i < format1_نالا_ڊيگهائي; i++)
        {
          إذا (s == format1_نالا (i))
          {
            standard_glyph_اشاريو = i;
            اكسر;
          }
        }

        إذا (standard_glyph_اشاريو == -1)
        {
          new_اشاريو = 258 + i;
          i++;
        }
        وإلا
        { new_اشاريو = standard_glyph_اشاريو; }
        glyph_name_منزل_new_اشاريو.سيٽ (s, new_اشاريو);
      }
      old_new_اشاريو_map.سيٽ (old_اشاريو, new_اشاريو);
    }
    old_gid_new_اشاريو_map.سيٽ (old_gid, new_اشاريو);
  }

  إذا (old_gid_new_اشاريو_map.اندريون_error())
    return_trace (خطأ);

  خودڪار اشاريو_iter =
  + hb_range (انگ_glyphs)
  | hb_map_retains_ترتيب_ڏيڻ ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              بنانشان new_اشاريو = 0;
                              إذا (ابتو_glyph_map.has (new_gid, &old_gid)) {
                                new_اشاريو = old_gid_new_اشاريو_map.get (*old_gid);
                                أرجع hb_pair_t<بنانشان, بنانشان> (*old_gid, new_اشاريو);
                              }
                              أرجع hb_pair_t<بنانشان, بنانشان> (new_gid, new_اشاريو);
                            })
  ;

  return_trace (serialize (c->serializer, اشاريو_iter, &_post));
}

} /* namespace OT */
#جيڪڏهن_پورو /* HB_OT_POST_TABLE_V2SUBSET_HH */
