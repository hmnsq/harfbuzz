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

#അന്തര്ഭാവ "hb-open-type.hh"
#അന്തര്ഭാവ "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

നാമാകാശ OT {
template<typename Iterator>
HB_INTERNAL തർക്കം postV2Tail::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        സ്ഥിരം ശൂന്യം* _post) സ്ഥിരം
{
  TRACE_SERIALIZE (ഇത്);
  auto *out = c->start_embed (ഇത്);
  എങ്കിൽ (unlikely (!c->check_success (out))) return_trace (അസത്യം);
  എങ്കിൽ (!out->glyphNameIndex.serialize (c, + it
                                         | hb_map (hb_second)))
      return_trace (അസത്യം);

  hb_set_t copied_indices;
  വേണ്ടി (സ്ഥിരം auto& _ : + it )
  {
    unsigned glyph_id = _.first;
    unsigned new_index = _.second;

    എങ്കിൽ (new_index < 258) തുടരുക;
    എങ്കിൽ (copied_indices.has (new_index)) തുടരുക;
    copied_indices.add (new_index);

    hb_bytes_t s = reinterpret_cast<സ്ഥിരം post::accelerator_t*> (_post)->find_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_size<HBUINT8> (HBUINT8::static_size * (s.length + 1));
    എങ്കിൽ (unlikely (!o)) return_trace (അസത്യം);
    എങ്കിൽ (!c->check_assign (o[0], s.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (അസത്യം);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_size * s.length);
  }

  return_trace (സത്യം);
}

HB_INTERNAL തർക്കം postV2Tail::subset (hb_subset_context_t *c) സ്ഥിരം
{
  TRACE_SUBSET (ഇത്);

  സ്ഥിരം hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
  unsigned അക്കം_glyphs = c->plan->അക്കം_output_glyphs ();
  hb_map_t old_new_index_map, old_gid_new_index_map;
  unsigned i = 0;

  post::accelerator_t _post (c->plan->source);

  hb_hashmap_t<hb_bytes_t, uint32_t, സത്യം> glyph_name_to_new_index;

  old_new_index_map.alloc (അക്കം_glyphs);
  old_gid_new_index_map.alloc (അക്കം_glyphs);
  glyph_name_to_new_index.alloc (അക്കം_glyphs);

  വേണ്ടി (auto _ : c->plan->new_to_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    unsigned old_index = glyphNameIndex[old_gid];

    unsigned new_index;
    സ്ഥിരം uint32_t *new_index2;
    എങ്കിൽ (old_index <= 257)
      new_index = old_index;
    അല്ലെങ്കിൽ എങ്കിൽ (old_new_index_map.has (old_index, &new_index2))
      new_index = *new_index2;
    അല്ലെങ്കിൽ
    {
      hb_bytes_t s = _post.find_glyph_name (old_gid);
      new_index = glyph_name_to_new_index.get (s);
      എങ്കിൽ (new_index == (unsigned)-1)
      {
        പൂർണ്ണസംഖ്യ standard_glyph_index = -1;
        വേണ്ടി (unsigned i = 0; i < format1_names_length; i++)
        {
          എങ്കിൽ (s == format1_names (i))
          {
            standard_glyph_index = i;
            നിർത്തുക;
          }
        }

        എങ്കിൽ (standard_glyph_index == -1)
        {
          new_index = 258 + i;
          i++;
        }
        അല്ലെങ്കിൽ
        { new_index = standard_glyph_index; }
        glyph_name_to_new_index.set (s, new_index);
      }
      old_new_index_map.set (old_index, new_index);
    }
    old_gid_new_index_map.set (old_gid, new_index);
  }

  എങ്കിൽ (old_gid_new_index_map.in_error())
    return_trace (അസത്യം);

  auto index_iter =
  + hb_range (അക്കം_glyphs)
  | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              unsigned new_index = 0;
                              എങ്കിൽ (reverse_glyph_map.has (new_gid, &old_gid)) {
                                new_index = old_gid_new_index_map.get (*old_gid);
                                തിരികെ hb_pair_t<unsigned, unsigned> (*old_gid, new_index);
                              }
                              തിരികെ hb_pair_t<unsigned, unsigned> (new_gid, new_index);
                            })
  ;

  return_trace (serialize (c->serializer, index_iter, &_post));
}

} /* namespace OT */
#endif /* HB_OT_POST_TABLE_V2SUBSET_HH */
