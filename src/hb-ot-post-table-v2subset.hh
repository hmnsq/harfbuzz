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

#अगर_अघोषित HB_OT_POST_TABLE_V2SUBSET_HH
#घोषणा HB_OT_POST_TABLE_V2SUBSET_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

namespace OT {
ढाँचा<typename Iterator>
HB_INTERNAL bool post_V2_पुच्छ::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        स्थिर व्योम* _post) स्थिर
{
  TRACE_SERIALIZE (यह);
  स्वतः *out = c->start_embed (यह);
  अगर (unlikely (!c->check_success (out))) return_trace (false);
  अगर (!out->glyph_Name_सूचकांक.serialize (c, + it
                                         | hb_map (hb_second)))
      return_trace (false);

  hb_set_t copied_indices;
  क्रम (स्थिर स्वतः& _ : + it )
  {
    अचिन्हित glyph_id = _.first;
    अचिन्हित new_सूचकांक = _.second;

    अगर (new_सूचकांक < 258) जारी;
    अगर (copied_indices.has (new_सूचकांक)) जारी;
    copied_indices.add (new_सूचकांक);

    hb_bytes_t s = reinterpret_cast<स्थिर post::accelerator_t*> (_post)->find_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_आकार<HBUINT8> (HBUINT8::static_आकार * (s.लंबाई + 1));
    अगर (unlikely (!o)) return_trace (false);
    अगर (!c->check_assign (o[0], s.लंबाई, HB_SERIALIZE_ERROR_INT_OVERFLOW)) return_trace (false);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_आकार * s.लंबाई);
  }

  return_trace (true);
}

HB_INTERNAL bool post_V2_पुच्छ::subset (hb_subset_context_t *c) स्थिर
{
  TRACE_SUBSET (यह);

  स्थिर hb_map_t &reverse_glyph_map = *c->plan->reverse_glyph_map;
  अचिन्हित अंक_glyphs = c->plan->अंक_output_glyphs ();
  hb_map_t old_new_सूचकांक_map, old_gid_new_सूचकांक_map;
  अचिन्हित i = 0;

  post::accelerator_t _post (c->plan->source);

  hb_hashmap_t<hb_bytes_t, uint32_t, true> glyph_name_to_new_सूचकांक;

  old_new_सूचकांक_map.alloc (अंक_glyphs);
  old_gid_new_सूचकांक_map.alloc (अंक_glyphs);
  glyph_name_to_new_सूचकांक.alloc (अंक_glyphs);

  क्रम (स्वतः _ : c->plan->new_to_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    अचिन्हित old_सूचकांक = glyph_Name_सूचकांक[old_gid];

    अचिन्हित new_सूचकांक;
    स्थिर uint32_t *new_index2;
    अगर (old_सूचकांक <= 257)
      new_सूचकांक = old_सूचकांक;
    अन्यथा अगर (old_new_सूचकांक_map.has (old_सूचकांक, &new_index2))
      new_सूचकांक = *new_index2;
    अन्यथा
    {
      hb_bytes_t s = _post.find_glyph_name (old_gid);
      new_सूचकांक = glyph_name_to_new_सूचकांक.लो (s);
      अगर (new_सूचकांक == (अचिन्हित)-1)
      {
        पूर्णांक standard_glyph_सूचकांक = -1;
        क्रम (अचिन्हित i = 0; i < format1_names_लंबाई; i++)
        {
          अगर (s == format1_names (i))
          {
            standard_glyph_सूचकांक = i;
            अवरोध;
          }
        }

        अगर (standard_glyph_सूचकांक == -1)
        {
          new_सूचकांक = 258 + i;
          i++;
        }
        अन्यथा
        { new_सूचकांक = standard_glyph_सूचकांक; }
        glyph_name_to_new_सूचकांक.set (s, new_सूचकांक);
      }
      old_new_सूचकांक_map.set (old_सूचकांक, new_सूचकांक);
    }
    old_gid_new_सूचकांक_map.set (old_gid, new_सूचकांक);
  }

  अगर (old_gid_new_सूचकांक_map.in_error())
    return_trace (false);

  स्वतः सूचकांक_iter =
  + hb_range (अंक_glyphs)
  | hb_map_retains_sorting ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              अचिन्हित new_सूचकांक = 0;
                              अगर (reverse_glyph_map.has (new_gid, &old_gid)) {
                                new_सूचकांक = old_gid_new_सूचकांक_map.लो (*old_gid);
                                वापस hb_pair_t<अचिन्हित, अचिन्हित> (*old_gid, new_सूचकांक);
                              }
                              वापस hb_pair_t<अचिन्हित, अचिन्हित> (new_gid, new_सूचकांक);
                            })
  ;

  return_trace (serialize (c->serializer, सूचकांक_iter, &_post));
}

} /* namespace OT */
#पूर्ण_अगर /* HB_OT_POST_TABLE_V2SUBSET_HH */
