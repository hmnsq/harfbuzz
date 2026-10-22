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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OT_POST_ਸਾਰਣੀ_V2_SUBSET_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_POST_ਸਾਰਣੀ_V2_SUBSET_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

ਨਾਮਾਕਾਸ਼ OT {
ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator>
HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ post_V2_ਪੂਛ::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        ਅਚਲ ਖਾਲੀ* _post) ਅਚਲ
{
  TRACE_SERIALIZE (ਇਹ);
  ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->ਸ਼ੁਰੂ_embed (ਇਹ);
  ਜੇ (unlikely (!c->check_success (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
  ਜੇ (!ਬਾਹਰਲਾ->glyph_Name_ਸੂਚਕਾਂਕ.serialize (c, + it
                                         | hb_map (hb_ਦੂਜਾ)))
      return_trace (ਝੂਠ);

  hb_ਸੈੱਟ_t copied_indices;
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& _ : + it )
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_id = _.ਪਹਿਲਾ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਸੂਚਕਾਂਕ = _.second;

    ਜੇ (new_ਸੂਚਕਾਂਕ < 258) ਜਾਰੀ;
    ਜੇ (copied_indices.has (new_ਸੂਚਕਾਂਕ)) ਜਾਰੀ;
    copied_indices.ਜੋੜੋ (new_ਸੂਚਕਾਂਕ);

    hb_ਬਾਈਟਾਂ_t s = reinterpret_cast<ਅਚਲ post::accelerator_t*> (_post)->ਲੱਭੋ_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_ਮਾਪ<HBUINT8> (HBUINT8::static_ਮਾਪ * (s.ਲੰਮਾਈ + 1));
    ਜੇ (unlikely (!o)) return_trace (ਝੂਠ);
    ਜੇ (!c->check_ਸੌਂਪੋ (o[0], s.ਲੰਮਾਈ, HB_SERIALIZE_ERROR_INT_ਉਛਾਲ)) return_trace (ਝੂਠ);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_ਮਾਪ * s.ਲੰਮਾਈ);
  }

  return_trace (ਸੱਚ);
}

HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ post_V2_ਪੂਛ::subset (hb_subset_context_t *c) ਅਚਲ
{
  TRACE_SUBSET (ਇਹ);

  ਅਚਲ hb_map_t &ਉਲਟ_glyph_map = *c->plan->ਉਲਟ_glyph_map;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs = c->plan->ਅੰਕ_output_glyphs ();
  hb_map_t old_new_ਸੂਚਕਾਂਕ_map, old_gid_new_ਸੂਚਕਾਂਕ_map;
  ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;

  post::accelerator_t _post (c->plan->ਸਰੋਤ);

  hb_hashmap_t<hb_ਬਾਈਟਾਂ_t, uint32_t, ਸੱਚ> glyph_name_ਮੰਜ਼ਿਲ_new_ਸੂਚਕਾਂਕ;

  old_new_ਸੂਚਕਾਂਕ_map.alloc (ਅੰਕ_glyphs);
  old_gid_new_ਸੂਚਕਾਂਕ_map.alloc (ਅੰਕ_glyphs);
  glyph_name_ਮੰਜ਼ਿਲ_new_ਸੂਚਕਾਂਕ.alloc (ਅੰਕ_glyphs);

  ਲਈ (ਸਵੈਚਾਲਿਤ _ : c->plan->new_ਮੰਜ਼ਿਲ_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਸੂਚਕਾਂਕ = glyph_Name_ਸੂਚਕਾਂਕ[old_gid];

    ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਸੂਚਕਾਂਕ;
    ਅਚਲ uint32_t *new_index2;
    ਜੇ (old_ਸੂਚਕਾਂਕ <= 257)
      new_ਸੂਚਕਾਂਕ = old_ਸੂਚਕਾਂਕ;
    ਨਹੀਂਤਾਂ ਜੇ (old_new_ਸੂਚਕਾਂਕ_map.has (old_ਸੂਚਕਾਂਕ, &new_index2))
      new_ਸੂਚਕਾਂਕ = *new_index2;
    ਨਹੀਂਤਾਂ
    {
      hb_ਬਾਈਟਾਂ_t s = _post.ਲੱਭੋ_glyph_name (old_gid);
      new_ਸੂਚਕਾਂਕ = glyph_name_ਮੰਜ਼ਿਲ_new_ਸੂਚਕਾਂਕ.get (s);
      ਜੇ (new_ਸੂਚਕਾਂਕ == (ਬਿਨਾਨਿਸ਼ਾਨੀ)-1)
      {
        ਪੂਰਨ standard_glyph_ਸੂਚਕਾਂਕ = -1;
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < format1_ਨਾਂ_ਲੰਮਾਈ; i++)
        {
          ਜੇ (s == format1_ਨਾਂ (i))
          {
            standard_glyph_ਸੂਚਕਾਂਕ = i;
            ਰੋਕੋ;
          }
        }

        ਜੇ (standard_glyph_ਸੂਚਕਾਂਕ == -1)
        {
          new_ਸੂਚਕਾਂਕ = 258 + i;
          i++;
        }
        ਨਹੀਂਤਾਂ
        { new_ਸੂਚਕਾਂਕ = standard_glyph_ਸੂਚਕਾਂਕ; }
        glyph_name_ਮੰਜ਼ਿਲ_new_ਸੂਚਕਾਂਕ.ਸੈੱਟ (s, new_ਸੂਚਕਾਂਕ);
      }
      old_new_ਸੂਚਕਾਂਕ_map.ਸੈੱਟ (old_ਸੂਚਕਾਂਕ, new_ਸੂਚਕਾਂਕ);
    }
    old_gid_new_ਸੂਚਕਾਂਕ_map.ਸੈੱਟ (old_gid, new_ਸੂਚਕਾਂਕ);
  }

  ਜੇ (old_gid_new_ਸੂਚਕਾਂਕ_map.ਵਿੱਚ_error())
    return_trace (ਝੂਠ);

  ਸਵੈਚਾਲਿਤ ਸੂਚਕਾਂਕ_iter =
  + hb_range (ਅੰਕ_glyphs)
  | hb_map_retains_ਛਾਂਟੀ ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਸੂਚਕਾਂਕ = 0;
                              ਜੇ (ਉਲਟ_glyph_map.has (new_gid, &old_gid)) {
                                new_ਸੂਚਕਾਂਕ = old_gid_new_ਸੂਚਕਾਂਕ_map.get (*old_gid);
                                ਵਾਪਸ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> (*old_gid, new_ਸੂਚਕਾਂਕ);
                              }
                              ਵਾਪਸ hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਬਿਨਾਨਿਸ਼ਾਨੀ> (new_gid, new_ਸੂਚਕਾਂਕ);
                            })
  ;

  return_trace (serialize (c->serializer, ਸੂਚਕਾਂਕ_iter, &_post));
}

} /* namespace OT */
#ਜੇ_ਅੰਤ /* HB_OT_POST_TABLE_V2SUBSET_HH */
