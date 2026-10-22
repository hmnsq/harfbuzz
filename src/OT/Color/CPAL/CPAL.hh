/*
 * Copyright © 2016  Google, Inc.
 * Copyright © 2018  Ebrahim Byagowi
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
 * Google Author(s): Sascha Brawer
 */

#నిర్వచితం_కాకపోతే OT_COLOR_CPAL_CPAL_HH
#నిర్వచించు OT_COLOR_CPAL_CPAL_HH

#అన్తర్భావ "../../../hb-open-type.hh"
#అన్తర్భావ "../../../hb-ot-color.h"
#అన్తర్భావ "../../../hb-ot-name.h"


/*
 * CPAL -- Color Palette
 * https://docs.microsoft.com/en-us/typography/opentype/spec/cpal
 */
#నిర్వచించు HB_OT_ట్యాగ్_CPAL HB_ట్యాగ్('C','P','A','L')

నామాకాశ OT {


నిర్మాణం CPALV1_తోక
{
  మిత్ర నిర్మాణం CPAL;

  అంతరంగ:
  hb_ot_color_palette_flags_t get_palette_flags (స్థిరరాశి శూన్య *పునాది,
						 గుర్తులేని పూర్ణాంకం palette_సూచీ,
						 గుర్తులేని పూర్ణాంకం palette_లెక్క) స్థిరరాశి
  {
    ఒకవేళ (!paletteFlagsZ) తిరిగి HB_OT_COLOR_PALETTE_FLAG_DEFAULT;
    తిరిగి (hb_ot_color_palette_flags_t) (uint32_t)
	   (పునాది+paletteFlagsZ).as_array (palette_లెక్క)[palette_సూచీ];
  }

  hb_ot_name_id_t get_palette_name_id (స్థిరరాశి శూన్య *పునాది,
				       గుర్తులేని పూర్ణాంకం palette_సూచీ,
				       గుర్తులేని పూర్ణాంకం palette_లెక్క) స్థిరరాశి
  {
    ఒకవేళ (!paletteLabelsZ) తిరిగి HB_OT_NAME_ID_INVALID;
    తిరిగి (పునాది+paletteLabelsZ).as_array (palette_లెక్క)[palette_సూచీ];
  }

  hb_ot_name_id_t get_color_name_id (స్థిరరాశి శూన్య *పునాది,
				     గుర్తులేని పూర్ణాంకం color_సూచీ,
				     గుర్తులేని పూర్ణాంకం color_లెక్క) స్థిరరాశి
  {
    ఒకవేళ (!colorLabelsZ) తిరిగి HB_OT_NAME_ID_INVALID;
    తిరిగి (పునాది+colorLabelsZ).as_array (color_లెక్క)[color_సూచీ];
  }

  బహిరంగ:
  శూన్య collect_name_ids (స్థిరరాశి శూన్య *పునాది,
                         గుర్తులేని palette_లెక్క,
                         గుర్తులేని color_లెక్క,
                         స్థిరరాశి hb_map_t *color_సూచీ_map,
                         hb_సమితి_t *nameids_గమ్యం_retain /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (paletteLabelsZ)
    {
      + (పునాది+paletteLabelsZ).as_array (palette_లెక్క)
      | hb_sink (nameids_గమ్యం_retain)
      ;
    }

    ఒకవేళ (colorLabelsZ)
    {
      స్థిరరాశి hb_array_t<స్థిరరాశి NameID> colorLabels = (పునాది+colorLabelsZ).as_array (color_లెక్క);
      కొరకు (గుర్తులేని i = 0; i < color_లెక్క; i++)
      {
        ఒకవేళ (!color_సూచీ_map->has (i)) కొనసాగు;
        nameids_గమ్యం_retain->కూడు (colorLabels[i]);
      }
    }
  }

  తార్కిక serialize (hb_serialize_context_t *c,
                  గుర్తులేని palette_లెక్క,
                  గుర్తులేని color_లెక్క,
                  స్థిరరాశి శూన్య *పునాది,
                  స్థిరరాశి hb_map_t *color_సూచీ_map) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->allocate_కొలత<CPALV1_తోక> (static_కొలత);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    బహిర్గతం->paletteFlagsZ = 0;
    ఒకవేళ (paletteFlagsZ)
      బహిర్గతం->paletteFlagsZ.serialize_copy (c, paletteFlagsZ, పునాది, 0, hb_serialize_context_t::తల, palette_లెక్క);

    బహిర్గతం->paletteLabelsZ = 0;
    ఒకవేళ (paletteLabelsZ)
      బహిర్గతం->paletteLabelsZ.serialize_copy (c, paletteLabelsZ, పునాది, 0, hb_serialize_context_t::తల, palette_లెక్క);

    స్థిరరాశి hb_array_t<స్థిరరాశి NameID> colorLabels = (పునాది+colorLabelsZ).as_array (color_లెక్క);
    ఒకవేళ (colorLabelsZ)
    {
      c->push ();
      కొరకు (గుర్తులేని i = 0; i < color_లెక్క; i++)
      {
        ఒకవేళ (!color_సూచీ_map->has (i)) కొనసాగు;
        ఒకవేళ (!c->copy<NameID> (colorLabels[i]))
        {
          c->తీయి_discard ();
          return_trace (అబద్ధం);
        }
      }
      c->కూడు_link (బహిర్గతం->colorLabelsZ, c->తీయి_pack ());
    }
    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c,
		 స్థిరరాశి శూన్య *పునాది,
		 గుర్తులేని పూర్ణాంకం palette_లెక్క,
		 గుర్తులేని పూర్ణాంకం color_లెక్క) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  (!paletteFlagsZ  || (పునాది+paletteFlagsZ).sanitize (c, palette_లెక్క)) &&
		  (!paletteLabelsZ || (పునాది+paletteLabelsZ).sanitize (c, palette_లెక్క)) &&
		  (!colorLabelsZ   || (పునాది+colorLabelsZ).sanitize (c, color_లెక్క)));
  }

  రక్షిత:
  // TODO(garretrieger): these offsets can hold nulls so we should not be using non-null offsets
  //                     here. Currently they are needed since UnsizedArrayOf doesn't define null_size
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<HBUINT32>>
		paletteFlagsZ;		/* Offset from the beginning of CPAL table to
					 * the Palette Type Array. Set to 0 if no array
					 * is provided. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<NameID>>
		paletteLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the palette labels array. Set to 0 if no
					 * array is provided. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<NameID>>
		colorLabelsZ;		/* Offset from the beginning of CPAL table to
					 * the color labels array. Set to 0
					 * if no array is provided. */
  బహిరంగ:
  DEFINE_కొలత_STATIC (12);
};

రకనిర్వచనం HBUINT32 BGRAColor;

నిర్మాణం CPAL
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_CPAL;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి అంకె_Palettes; }

  size_t get_కొలత () స్థిరరాశి
  { తిరిగి min_కొలత + అంకె_Palettes * పరిమాణం (color_రికార్డు_Indices_Z[0]); }

  గుర్తులేని పూర్ణాంకం get_palette_లెక్క () స్థిరరాశి { తిరిగి అంకె_Palettes; }
  గుర్తులేని పూర్ణాంకం   get_color_లెక్క () స్థిరరాశి { తిరిగి అంకె_Colors; }

  hb_ot_color_palette_flags_t get_palette_flags (గుర్తులేని పూర్ణాంకం palette_సూచీ) స్థిరరాశి
  { తిరిగి v1 ().get_palette_flags (ఇది, palette_సూచీ, అంకె_Palettes); }

  hb_ot_name_id_t get_palette_name_id (గుర్తులేని పూర్ణాంకం palette_సూచీ) స్థిరరాశి
  { తిరిగి v1 ().get_palette_name_id (ఇది, palette_సూచీ, అంకె_Palettes); }

  hb_ot_name_id_t get_color_name_id (గుర్తులేని పూర్ణాంకం color_సూచీ) స్థిరరాశి
  { తిరిగి v1 ().get_color_name_id (ఇది, color_సూచీ, అంకె_Colors); }

  hb_array_t<స్థిరరాశి BGRAColor> get_palette_colors (గుర్తులేని పూర్ణాంకం palette_సూచీ) స్థిరరాశి
  {
    ఒకవేళ (unlikely (palette_సూచీ >= అంకె_Palettes))
      తిరిగి hb_array_t<స్థిరరాశి BGRAColor> ();
    గుర్తులేని పూర్ణాంకం ప్రారంభం_సూచీ = color_రికార్డు_Indices_Z[palette_సూచీ];
    hb_array_t<స్థిరరాశి BGRAColor> all_colors ((ఇది+color_రికార్డులు_Z).arrayZ, అంకె_Color_రికార్డులు);
    తిరిగి all_colors.sub_array (ప్రారంభం_సూచీ, అంకె_Colors);
  }
  గుర్తులేని పూర్ణాంకం get_palette_colors (గుర్తులేని పూర్ణాంకం  palette_సూచీ,
				   గుర్తులేని పూర్ణాంకం  ప్రారంభం_స్థానభ్రంశం,
				   గుర్తులేని పూర్ణాంకం *color_లెక్క, /* IN/OUT.  May be NULL. */
				   hb_color_t   *colors       /* OUT.     May be NULL. */) స్థిరరాశి
  {
    ఒకవేళ (unlikely (palette_సూచీ >= అంకె_Palettes))
    {
      ఒకవేళ (color_లెక్క) *color_లెక్క = 0;
      తిరిగి 0;
    }
    గుర్తులేని పూర్ణాంకం ప్రారంభం_సూచీ = color_రికార్డు_Indices_Z[palette_సూచీ];
    hb_array_t<స్థిరరాశి BGRAColor> all_colors ((ఇది+color_రికార్డులు_Z).arrayZ, అంకె_Color_రికార్డులు);
    hb_array_t<స్థిరరాశి BGRAColor> palette_colors = all_colors.sub_array (ప్రారంభం_సూచీ,
								       అంకె_Colors);
    ఒకవేళ (color_లెక్క && colors)
    {
      + palette_colors.sub_array (ప్రారంభం_స్థానభ్రంశం, color_లెక్క)
      | hb_sink (hb_array (colors, *color_లెక్క))
      ;
    }
    తిరిగి అంకె_Colors;
  }

  శూన్య collect_name_ids (స్థిరరాశి hb_map_t *color_సూచీ_map,
                         hb_సమితి_t *nameids_గమ్యం_retain /*బహిర్గతం*/) స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ == 1)
    {
      hb_barrier ();
      v1 ().collect_name_ids (ఇది, అంకె_Palettes, అంకె_Colors, color_సూచీ_map, nameids_గమ్యం_retain);
    }
  }

  అంతరంగ:
  స్థిరరాశి CPALV1_తోక& v1 () స్థిరరాశి
  {
    ఒకవేళ (సంస్కరణ == 0) తిరిగి Null (CPALV1_తోక);
    hb_barrier ();
    తిరిగి StructAfter<CPALV1_తోక> (*ఇది);
  }

  బహిరంగ:
  తార్కిక serialize (hb_serialize_context_t *c,
                  స్థిరరాశి hb_array_t<స్థిరరాశి HBUINT16> &color_రికార్డు_indices,
                  స్థిరరాశి hb_array_t<స్థిరరాశి BGRAColor> &color_రికార్డులు,
                  స్థిరరాశి hb_సదిశ_t<గుర్తులేని>& మొదటి_color_సూచీ_for_layer,
                  స్థిరరాశి hb_map_t& మొదటి_color_గమ్యం_layer_సూచీ,
                  స్థిరరాశి hb_సమితి_t &retained_color_indices) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    // TODO(grieger): limit total final size.

    కొరకు (స్థిరరాశి స్వయంచాలక idx : color_రికార్డు_indices)
    {
      hb_codepoint_t layer_సూచీ = మొదటి_color_గమ్యం_layer_సూచీ[idx];

      HBUINT16 new_idx;
      new_idx = layer_సూచీ * retained_color_indices.get_population ();
      ఒకవేళ (!c->copy<HBUINT16> (new_idx)) return_trace (అబద్ధం);
    }

    c->push ();
    కొరకు (గుర్తులేని మొదటి_color_సూచీ : మొదటి_color_సూచీ_for_layer)
    {
      కొరకు (hb_codepoint_t color_సూచీ : retained_color_indices)
      {
        ఒకవేళ (!c->copy<BGRAColor> (color_రికార్డులు[మొదటి_color_సూచీ + color_సూచీ]))
        {
          c->తీయి_discard ();
          return_trace (అబద్ధం);
        }
      }
    }

    c->కూడు_link (color_రికార్డులు_Z, c->తీయి_pack ());
    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    ఒకవేళ (!అంకె_Palettes) return_trace (అబద్ధం);

    స్థిరరాశి hb_map_t *color_సూచీ_map = &c->plan->colr_palettes;
    ఒకవేళ (color_సూచీ_map->is_empty ()) return_trace (అబద్ధం);

    hb_సమితి_t retained_color_indices;
    కొరకు (స్థిరరాశి స్వయంచాలక _ : color_సూచీ_map->కీలకాలు ())
    {
      ఒకవేళ (_ == 0xFFFF) కొనసాగు;
      retained_color_indices.కూడు (_);
    }
    ఒకవేళ (retained_color_indices.is_empty ()) return_trace (అబద్ధం);

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);


    బహిర్గతం->సంస్కరణ = సంస్కరణ;
    బహిర్గతం->అంకె_Colors = retained_color_indices.get_population ();
    బహిర్గతం->అంకె_Palettes = అంకె_Palettes;

    hb_సదిశ_t<గుర్తులేని> మొదటి_color_సూచీ_for_layer;
    hb_map_t మొదటి_color_గమ్యం_layer_సూచీ;
    /* Two palettes that retain exactly the same color records can share the
     * same block of color records. This commonly happens after subsetting
     * when the colors that distinguished the palettes are dropped. Map from
     * a layer's retained colors to its index to find equal layers without
     * scanning all of them. The keys point into layer_retained_colors,
     * which keeps them alive. */
    hb_hashmap_t<hb_array_t<స్థిరరాశి BGRAColor>, గుర్తులేని> retained_colors_గమ్యం_layer_సూచీ;
    hb_సదిశ_t<hb_సదిశ_t<BGRAColor>> layer_retained_colors;

    స్థిరరాశి hb_array_t<స్థిరరాశి BGRAColor> all_color_రికార్డులు = (ఇది+color_రికార్డులు_Z).as_array (అంకె_Color_రికార్డులు);

    స్థిరరాశి hb_array_t<స్థిరరాశి HBUINT16> color_రికార్డు_Indices = color_రికార్డు_Indices_Z.as_array (అంకె_Palettes);
    కొరకు (స్థిరరాశి స్వయంచాలక మొదటి_color_రికార్డు_idx : color_రికార్డు_Indices)
    {
      ఒకవేళ (మొదటి_color_గమ్యం_layer_సూచీ.has (మొదటి_color_రికార్డు_idx)) కొనసాగు;

      hb_సదిశ_t<BGRAColor> retained_colors;
      ఒకవేళ (unlikely (!retained_colors.alloc (retained_color_indices.get_population ()))) return_trace (అబద్ధం);
      కొరకు (hb_codepoint_t color_సూచీ : retained_color_indices)
	retained_colors.push (all_color_రికార్డులు[మొదటి_color_రికార్డు_idx + color_సూచీ]);

      గుర్తులేని layer_సూచీ = మొదటి_color_సూచీ_for_layer.నిడివి;
      గుర్తులేని *candidate_layer_సూచీ = శూన్యసూచిక;
      ఒకవేళ (retained_colors_గమ్యం_layer_సూచీ.has (retained_colors.as_array (), &candidate_layer_సూచీ))
	layer_సూచీ = *candidate_layer_సూచీ;
      లేకపోతే
      {
	మొదటి_color_సూచీ_for_layer.push (మొదటి_color_రికార్డు_idx);
	ఒకవేళ (unlikely (!c->serializer->propagate_error (మొదటి_color_సూచీ_for_layer))) return_trace (అబద్ధం);

	ఒకవేళ (unlikely (!layer_retained_colors.నెట్టు_or_fail (std::move (retained_colors)))) return_trace (అబద్ధం);
	ఒకవేళ (unlikely (!retained_colors_గమ్యం_layer_సూచీ.సమితి (layer_retained_colors.తోక ().as_array (), layer_సూచీ)))
	  return_trace (అబద్ధం);
      }
      మొదటి_color_గమ్యం_layer_సూచీ.సమితి (మొదటి_color_రికార్డు_idx, layer_సూచీ);
      ఒకవేళ (unlikely (!c->serializer->propagate_error (మొదటి_color_గమ్యం_layer_సూచీ))) return_trace (అబద్ధం);
    }

    బహిర్గతం->అంకె_Color_రికార్డులు = మొదటి_color_సూచీ_for_layer.నిడివి
                           * retained_color_indices.get_population ();

    ఒకవేళ (!బహిర్గతం->serialize (c->serializer,
                         color_రికార్డు_Indices,
                         all_color_రికార్డులు,
                         మొదటి_color_సూచీ_for_layer,
                         మొదటి_color_గమ్యం_layer_సూచీ,
                         retained_color_indices))
      return_trace (అబద్ధం);

    ఒకవేళ (సంస్కరణ == 1)
    {
      hb_barrier ();
      return_trace (v1 ().serialize (c->serializer, అంకె_Palettes, అంకె_Colors, ఇది, color_సూచీ_map));
    }

    return_trace (నిజం);
  }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  hb_barrier () &&
		  (ఇది+color_రికార్డులు_Z).sanitize (c, అంకె_Color_రికార్డులు) &&
		  color_రికార్డు_Indices_Z.sanitize (c, అంకె_Palettes) &&
		  (సంస్కరణ == 0 || v1 ().sanitize (c, ఇది, అంకె_Palettes, అంకె_Colors)));
  }

  రక్షిత:
  HBUINT16	సంస్కరణ;		/* Table version number */
  /* Version 0 */
  HBUINT16	అంకె_Colors;		/* Number of colors in each palette. */
  HBUINT16	అంకె_Palettes;		/* Number of palettes in the table. */
  HBUINT16	అంకె_Color_రికార్డులు;	/* Total number of color records, combined for
					 * all palettes. */
  NNOffset32_గమ్యం<Unsized_Array_యొక్క<BGRAColor>>
		color_రికార్డులు_Z;		/* Offset from the beginning of CPAL table to
					 * the first ColorRecord. */
  Unsized_Array_యొక్క<HBUINT16>
		color_రికార్డు_Indices_Z;	/* Index of each palette’s first color record in
					 * the combined color record array. */
/*CPALV1Tail	v1;*/
  బహిరంగ:
  DEFINE_కొలత_ARRAY (12, color_రికార్డు_Indices_Z);
};

} /* namespace OT */


#అయితే_ముగింపు /* OT_COLOR_CPAL_CPAL_HH */
