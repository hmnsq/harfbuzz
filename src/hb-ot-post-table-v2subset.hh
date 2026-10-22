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

#నిర్వచితం_కాకపోతే HB_OT_POST_పట్టిక_V2_SUBSET_HH
#నిర్వచించు HB_OT_POST_పట్టిక_V2_SUBSET_HH

#అన్తర్భావ "hb-open-type.hh"
#అన్తర్భావ "hb-ot-post-table.hh"

/*
 * post -- PostScript
 * https://docs.microsoft.com/en-us/typography/opentype/spec/post
 */

నామాకాశ OT {
మూస<రకం_పేరు Iterator>
HB_అంతర్గత తార్కిక post_V2_తోక::serialize (hb_serialize_context_t *c,
                                        Iterator it,
                                        స్థిరరాశి శూన్య* _post) స్థిరరాశి
{
  TRACE_SERIALIZE (ఇది);
  స్వయంచాలక *బహిర్గతం = c->ప్రారంభం_embed (ఇది);
  ఒకవేళ (unlikely (!c->check_success (బహిర్గతం))) return_trace (అబద్ధం);
  ఒకవేళ (!బహిర్గతం->glyph_Name_సూచీ.serialize (c, + it
                                         | hb_map (hb_రెండవ)))
      return_trace (అబద్ధం);

  hb_సమితి_t copied_indices;
  కొరకు (స్థిరరాశి స్వయంచాలక& _ : + it )
  {
    గుర్తులేని glyph_id = _.మొదటి;
    గుర్తులేని new_సూచీ = _.second;

    ఒకవేళ (new_సూచీ < 258) కొనసాగు;
    ఒకవేళ (copied_indices.has (new_సూచీ)) కొనసాగు;
    copied_indices.కూడు (new_సూచీ);

    hb_బైట్లు_t s = reinterpret_cast<స్థిరరాశి post::accelerator_t*> (_post)->కనుగొను_glyph_name (glyph_id);
    HBUINT8 *o = c->allocate_కొలత<HBUINT8> (HBUINT8::static_కొలత * (s.నిడివి + 1));
    ఒకవేళ (unlikely (!o)) return_trace (అబద్ధం);
    ఒకవేళ (!c->check_కేటాయించు (o[0], s.నిడివి, HB_SERIALIZE_ERROR_INT_అధికప్రవాహం)) return_trace (అబద్ధం);
    hb_memcpy (o+1, s.arrayZ, HBUINT8::static_కొలత * s.నిడివి);
  }

  return_trace (నిజం);
}

HB_అంతర్గత తార్కిక post_V2_తోక::subset (hb_subset_context_t *c) స్థిరరాశి
{
  TRACE_SUBSET (ఇది);

  స్థిరరాశి hb_map_t &తిరగేయి_glyph_map = *c->plan->తిరగేయి_glyph_map;
  గుర్తులేని అంకె_glyphs = c->plan->అంకె_output_glyphs ();
  hb_map_t old_new_సూచీ_map, old_gid_new_సూచీ_map;
  గుర్తులేని i = 0;

  post::accelerator_t _post (c->plan->ఆధారం);

  hb_hashmap_t<hb_బైట్లు_t, uint32_t, నిజం> glyph_name_గమ్యం_new_సూచీ;

  old_new_సూచీ_map.alloc (అంకె_glyphs);
  old_gid_new_సూచీ_map.alloc (అంకె_glyphs);
  glyph_name_గమ్యం_new_సూచీ.alloc (అంకె_glyphs);

  కొరకు (స్వయంచాలక _ : c->plan->new_గమ్యం_old_gid_list)
  {
    hb_codepoint_t old_gid = _.second;
    గుర్తులేని old_సూచీ = glyph_Name_సూచీ[old_gid];

    గుర్తులేని new_సూచీ;
    స్థిరరాశి uint32_t *new_index2;
    ఒకవేళ (old_సూచీ <= 257)
      new_సూచీ = old_సూచీ;
    లేకపోతే ఒకవేళ (old_new_సూచీ_map.has (old_సూచీ, &new_index2))
      new_సూచీ = *new_index2;
    లేకపోతే
    {
      hb_బైట్లు_t s = _post.కనుగొను_glyph_name (old_gid);
      new_సూచీ = glyph_name_గమ్యం_new_సూచీ.get (s);
      ఒకవేళ (new_సూచీ == (గుర్తులేని)-1)
      {
        పూర్ణాంకం standard_glyph_సూచీ = -1;
        కొరకు (గుర్తులేని i = 0; i < format1_పేర్లు_నిడివి; i++)
        {
          ఒకవేళ (s == format1_పేర్లు (i))
          {
            standard_glyph_సూచీ = i;
            ఆపు;
          }
        }

        ఒకవేళ (standard_glyph_సూచీ == -1)
        {
          new_సూచీ = 258 + i;
          i++;
        }
        లేకపోతే
        { new_సూచీ = standard_glyph_సూచీ; }
        glyph_name_గమ్యం_new_సూచీ.సమితి (s, new_సూచీ);
      }
      old_new_సూచీ_map.సమితి (old_సూచీ, new_సూచీ);
    }
    old_gid_new_సూచీ_map.సమితి (old_gid, new_సూచీ);
  }

  ఒకవేళ (old_gid_new_సూచీ_map.లోపల_error())
    return_trace (అబద్ధం);

  స్వయంచాలక సూచీ_iter =
  + hb_range (అంకె_glyphs)
  | hb_map_retains_క్రమీకరణ ([&](hb_codepoint_t new_gid)
                            {
                              hb_codepoint_t *old_gid;
                              /* use 0 for retain-gid holes, which refers to the name .notdef,
                               * as the glyphNameIndex entry for that glyph ID."*/
                              గుర్తులేని new_సూచీ = 0;
                              ఒకవేళ (తిరగేయి_glyph_map.has (new_gid, &old_gid)) {
                                new_సూచీ = old_gid_new_సూచీ_map.get (*old_gid);
                                తిరిగి hb_pair_t<గుర్తులేని, గుర్తులేని> (*old_gid, new_సూచీ);
                              }
                              తిరిగి hb_pair_t<గుర్తులేని, గుర్తులేని> (new_gid, new_సూచీ);
                            })
  ;

  return_trace (serialize (c->serializer, సూచీ_iter, &_post));
}

} /* namespace OT */
#అయితే_ముగింపు /* HB_OT_POST_TABLE_V2SUBSET_HH */
