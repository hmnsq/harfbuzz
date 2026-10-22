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

#অন্তর্ভাব "hb-open-type.hh"
#অন্তর্ভাব "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

নামাকাশ OT {
template<typename Iterator>
HB_INTERNAL যৌক্তিক post_V2_লেজ::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        ধ্রুবক শূন্য* _post) ধ্রুবক
{
  TRACE_SERIALIZE (এই);
  auto *out = c->start_embed (এই);
  যদি (unlikely (!c->check_success (out))) return_trace (মিথ্যা);
  যদি (!out->glyph_Name_সূচক.serialize (c, + it
                                         | hb_map (hb_second)))
      return_trace (মিথ্যা);

  hb_set_t copied_indices;
  জন্য (ধ্রুবক auto& _ : + it )
  {
    unsigned glyph_id = _.first;
    unsigned new_সূচক = _.second;

    যদি (new_সূচক < 258) চালু;
    যদি (copied_indices.has (new_সূচক)) চালু;
    copied_indices.add (new_সূচক);

    hb_bytes_t s = reinterpret_cast<ধ্রুবক post::accelerator_t*> (_post)->find_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_আকার<HBUINT8> (HBUINT8::static_আকার * (s.দৈর্ঘ্য + 1));
    যদি (unlikely (!o)) return_trace (মিথ্যা);
    যদি (!c->check_assign (o[0], s.দৈর্ঘ্য, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (মিথ্যা);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_আকার * s.দৈর্ঘ্য);
  }

  return_trace (সত্য);
}

HB_INTERNAL যৌক্তিক post_V2_লেজ::subset (hb_subset_context_t *c) ধ্রুবক
{
  TRACE_SUBSET (এই);

  ধ্রুবক hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
  unsigned অঙ্ক_glyphs = c->plan->অঙ্ক_output_glyphs ();
  hb_map_t old_new_সূচক_map, old_gid_new_সূচক_map;
  unsigned i = 0;

  post::accelerator_t _post (c->plan->source);

  hb_hashmap_t<hb_bytes_t, uint32_t, সত্য> glyph_name_to_new_সূচক;

  old_new_সূচক_map.alloc (অঙ্ক_glyphs);
  old_gid_new_সূচক_map.alloc (অঙ্ক_glyphs);
  glyph_name_to_new_সূচক.alloc (অঙ্ক_glyphs);

  জন্য (auto _ : c->plan->new_to_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    unsigned old_সূচক = glyph_Name_সূচক[old_gid];

    unsigned new_সূচক;
    ধ্রুবক uint32_t *new_index2;
    যদি (old_সূচক <= 257)
      new_সূচক = old_সূচক;
    অন্যথা যদি (old_new_সূচক_map.has (old_সূচক, &new_index2))
      new_সূচক = *new_index2;
    অন্যথা
    {
      hb_bytes_t s = _post.find_glyph_name (old_gid);
      new_সূচক = glyph_name_to_new_সূচক.get (s);
      যদি (new_সূচক == (unsigned)-1)
      {
        পূর্ণসংখ্যা standard_glyph_সূচক = -1;
        জন্য (unsigned i = 0; i < format1_names_দৈর্ঘ্য; i++)
        {
          যদি (s == format1_names (i))
          {
            standard_glyph_সূচক = i;
            বিরতি;
          }
        }

        যদি (standard_glyph_সূচক == -1)
        {
          new_সূচক = 258 + i;
          i++;
        }
        অন্যথা
        { new_সূচক = standard_glyph_সূচক; }
        glyph_name_to_new_সূচক.set (s, new_সূচক);
      }
      old_new_সূচক_map.set (old_সূচক, new_সূচক);
    }
    old_gid_new_সূচক_map.set (old_gid, new_সূচক);
  }

  যদি (old_gid_new_সূচক_map.in_error())
    return_trace (মিথ্যা);

  auto সূচক_iter =
  + hb_range (অঙ্ক_glyphs)
  | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              unsigned new_সূচক = 0;
                              যদি (reverse_glyph_map.has (new_gid, &old_gid)) {
                                new_সূচক = old_gid_new_সূচক_map.get (*old_gid);
                                ফেরত hb_pair_t<unsigned, unsigned> (*old_gid, new_সূচক);
                              }
                              ফেরত hb_pair_t<unsigned, unsigned> (new_gid, new_সূচক);
                            })
  ;

  return_trace (serialize (c->serializer, সূচক_iter, &_post));
}

} /* namespace OT */
#endif /* HB_OT_POST_TABLE_V2SUBSET_HH */
