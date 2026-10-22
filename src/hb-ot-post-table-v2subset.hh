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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

नामाकाश OT {
ढाँचा<typename Iterator>
HB_INTERNAL तार्किक postV2Tail::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        अचर शून्य* _post) अचर
{
  TRACE_SERIALIZE (एतत्);
  स्वतः *out = c->start_embed (एतत्);
  यदि (unlikely (!c->check_success (out))) return_trace (असत्य);
  यदि (!out->glyphNameIndex.serialize (c, + it
                                         | hb_map (hb_second)))
      return_trace (असत्य);

  hb_set_t copied_indices;
  लागि (अचर स्वतः& _ : + it )
  {
    अचिन्हित glyph_id = _.first;
    अचिन्हित new_index = _.second;

    यदि (new_index < 258) जारी;
    यदि (copied_indices.has (new_index)) जारी;
    copied_indices.add (new_index);

    hb_bytes_t s = reinterpret_cast<अचर post::accelerator_t*> (_post)->find_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_size<HBUINT8> (HBUINT8::static_size * (s.length + 1));
    यदि (unlikely (!o)) return_trace (असत्य);
    यदि (!c->check_assign (o[0], s.length, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (असत्य);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_size * s.length);
  }

  return_trace (सत्य);
}

HB_INTERNAL तार्किक postV2Tail::subset (hb_subset_context_t *c) अचर
{
  TRACE_SUBSET (एतत्);

  अचर hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
  अचिन्हित अङ्क_glyphs = c->plan->अङ्क_output_glyphs ();
  hb_map_t old_new_index_map, old_gid_new_index_map;
  अचिन्हित i = 0;

  post::accelerator_t _post (c->plan->source);

  hb_hashmap_t<hb_bytes_t, uint32_t, सत्य> glyph_name_to_new_index;

  old_new_index_map.alloc (अङ्क_glyphs);
  old_gid_new_index_map.alloc (अङ्क_glyphs);
  glyph_name_to_new_index.alloc (अङ्क_glyphs);

  लागि (स्वतः _ : c->plan->new_to_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    अचिन्हित old_index = glyphNameIndex[old_gid];

    अचिन्हित new_index;
    अचर uint32_t *new_index2;
    यदि (old_index <= 257)
      new_index = old_index;
    अन्यथा यदि (old_new_index_map.has (old_index, &new_index2))
      new_index = *new_index2;
    अन्यथा
    {
      hb_bytes_t s = _post.find_glyph_name (old_gid);
      new_index = glyph_name_to_new_index.get (s);
      यदि (new_index == (अचिन्हित)-1)
      {
        पूर्णाङ्क standard_glyph_index = -1;
        लागि (अचिन्हित i = 0; i < format1_names_length; i++)
        {
          यदि (s == format1_names (i))
          {
            standard_glyph_index = i;
            रोक;
          }
        }

        यदि (standard_glyph_index == -1)
        {
          new_index = 258 + i;
          i++;
        }
        अन्यथा
        { new_index = standard_glyph_index; }
        glyph_name_to_new_index.set (s, new_index);
      }
      old_new_index_map.set (old_index, new_index);
    }
    old_gid_new_index_map.set (old_gid, new_index);
  }

  यदि (old_gid_new_index_map.in_error())
    return_trace (असत्य);

  स्वतः index_iter =
  + hb_range (अङ्क_glyphs)
  | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              अचिन्हित new_index = 0;
                              यदि (reverse_glyph_map.has (new_gid, &old_gid)) {
                                new_index = old_gid_new_index_map.get (*old_gid);
                                फर्काऊ hb_pair_t<अचिन्हित, अचिन्हित> (*old_gid, new_index);
                              }
                              फर्काऊ hb_pair_t<अचिन्हित, अचिन्हित> (new_gid, new_index);
                            })
  ;

  return_trace (serialize (c->serializer, index_iter, &_post));
}

} /* namespace OT */
#endif /* HB_OT_POST_TABLE_V2SUBSET_HH */
