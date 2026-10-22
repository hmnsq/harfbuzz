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

#ifndef HB_OT_POST_TABLE_V2SUBSET_HH
#define HB_OT_POST_TABLE_V2SUBSET_HH

#شامل "hb-open-type.hh"
#شامل "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

فضاینام OT {
template<typename Iterator>
HB_INTERNAL منطقی postV2Tail::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        ثابت تهی* _post) ثابت
{
  TRACE_SERIALIZE (این);
  auto *out = c->start_embed (این);
  اگر (unlikely (!c->check_success (out))) return_trace (نادرست);
  اگر (!out->glyphNameIndex.serialize (c, + it
                                         | hb_map (hb_second)))
      return_trace (نادرست);

  hb_set_t copied_indices;
  برای (ثابت auto& _ : + it )
  {
    unsigned glyph_id = _.first;
    unsigned new_index = _.second;

    اگر (new_index < 258) ادامه;
    اگر (copied_indices.has (new_index)) ادامه;
    copied_indices.add (new_index);

    hb_bytes_t s = reinterpret_cast<ثابت post::accelerator_t*> (_post)->find_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_size<HBUINT8> (HBUINT8::static_size * (s.length + 1));
    اگر (unlikely (!o)) return_trace (نادرست);
    اگر (!c->check_assign (o[0], s.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (نادرست);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_size * s.length);
  }

  return_trace (درست);
}

HB_INTERNAL منطقی postV2Tail::subset (hb_subset_context_t *c) ثابت
{
  TRACE_SUBSET (این);

  ثابت hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
  unsigned رقم_glyphs = c->plan->رقم_output_glyphs ();
  hb_map_t old_new_index_map, old_gid_new_index_map;
  unsigned i = 0;

  post::accelerator_t _post (c->plan->source);

  hb_hashmap_t<hb_bytes_t, uint32_t, درست> glyph_name_to_new_index;

  old_new_index_map.alloc (رقم_glyphs);
  old_gid_new_index_map.alloc (رقم_glyphs);
  glyph_name_to_new_index.alloc (رقم_glyphs);

  برای (auto _ : c->plan->new_to_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    unsigned old_index = glyphNameIndex[old_gid];

    unsigned new_index;
    ثابت uint32_t *new_index2;
    اگر (old_index <= 257)
      new_index = old_index;
    وگرنه اگر (old_new_index_map.has (old_index, &new_index2))
      new_index = *new_index2;
    وگرنه
    {
      hb_bytes_t s = _post.find_glyph_name (old_gid);
      new_index = glyph_name_to_new_index.get (s);
      اگر (new_index == (unsigned)-1)
      {
        صحیح standard_glyph_index = -1;
        برای (unsigned i = 0; i < format1_names_length; i++)
        {
          اگر (s == format1_names (i))
          {
            standard_glyph_index = i;
            بشکن;
          }
        }

        اگر (standard_glyph_index == -1)
        {
          new_index = 258 + i;
          i++;
        }
        وگرنه
        { new_index = standard_glyph_index; }
        glyph_name_to_new_index.set (s, new_index);
      }
      old_new_index_map.set (old_index, new_index);
    }
    old_gid_new_index_map.set (old_gid, new_index);
  }

  اگر (old_gid_new_index_map.in_error())
    return_trace (نادرست);

  auto index_iter =
  + hb_range (رقم_glyphs)
  | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              unsigned new_index = 0;
                              اگر (reverse_glyph_map.has (new_gid, &old_gid)) {
                                new_index = old_gid_new_index_map.get (*old_gid);
                                بازگشت hb_pair_t<unsigned, unsigned> (*old_gid, new_index);
                              }
                              بازگشت hb_pair_t<unsigned, unsigned> (new_gid, new_index);
                            })
  ;

  return_trace (serialize (c->serializer, index_iter, &_post));
}

} /* namespace OT */
#endif /* HB_OT_POST_TABLE_V2SUBSET_HH */
