/*
 * Copyright © 2010  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#अगर_अघोषित HB_OT_SHAPE_HH
#घोषणा HB_OT_SHAPE_HH

#समावेश "hb.hh"

#समावेश "hb-ot-map.hh"
#समावेश "hb-aat-map.hh"


HB_INTERNAL bool
_hb_ot_layout_table_get_feature_variations_state (hb_face_t             *face,
						   hb_tag_t               table_tag,
						   स्थिर पूर्णांक             *coords,
						   अचिन्हित पूर्णांक           अंक_coords,
						   अचिन्हित पूर्णांक          *variations_सूचकांक,
						   hb_vector_t<अचिन्हित> *lookup_conditions);

HB_INTERNAL bool
_hb_ot_layout_feature_get_lookup_variations (hb_face_t *face,
                                             hb_tag_t table_tag,
                                             अचिन्हित feature_सूचकांक,
                                             अचिन्हित variations_सूचकांक,
                                             hb_sorted_array_t<स्थिर अचिन्हित> conditions,
                                             hb_set_t *lookup_indexes);

काष्ठा hb_ot_shape_plan_कुंजी_t
{
  अचिन्हित पूर्णांक variations_सूचकांक[2];
  /* Sorted offsets of distinct true conditions within FeatureVariations.
   * Different outcomes may conservatively separate equivalent lookup sets. */
  hb_vector_t<अचिन्हित> lookup_conditions[2];

  bool init (hb_face_t *face,
	     स्थिर पूर्णांक *coords,
	     अचिन्हित   अंक_coords)
  {
    क्रम (अचिन्हित पूर्णांक table_सूचकांक = 0; table_सूचकांक < 2; table_सूचकांक++)
      अगर (unlikely (!_hb_ot_layout_table_get_feature_variations_state (
			face,
			table_tags[table_सूचकांक],
			coords,
			अंक_coords,
			&variations_सूचकांक[table_सूचकांक],
			&lookup_conditions[table_सूचकांक])))
      {
	क्रम (स्वतः &v : lookup_conditions)
	  v.fini ();
	वापस false;
      }
    वापस true;
  }

  व्योम fini ()
  {
    क्रम (स्वतः &v : lookup_conditions)
      v.fini ();
  }

  bool get_lookup_variations (hb_face_t *face,
                              अचिन्हित पूर्णांक table_सूचकांक,
			      अचिन्हित पूर्णांक feature_सूचकांक,
			      hb_set_t *lookups) स्थिर
  {
    वापस _hb_ot_layout_feature_get_lookup_variations (
	face, table_tags[table_सूचकांक], feature_सूचकांक, variations_सूचकांक[table_सूचकांक],
	lookup_conditions[table_सूचकांक].as_sorted_array (), lookups);
  }

  bool equal (स्थिर hb_ot_shape_plan_कुंजी_t *other)
  {
    वापस variations_सूचकांक[0] == other->variations_सूचकांक[0] &&
	   variations_सूचकांक[1] == other->variations_सूचकांक[1] &&
	   lookup_conditions[0] == other->lookup_conditions[0] &&
	   lookup_conditions[1] == other->lookup_conditions[1];
  }
};


काष्ठा hb_shape_plan_कुंजी_t;

काष्ठा hb_ot_shape_plan_t
{
  ~hb_ot_shape_plan_t () { fini (); }

  hb_segment_properties_t props;
  स्थिर काष्ठा hb_ot_shaper_t *shaper;
  hb_ot_map_t map;
  hb_aat_map_t aat_map;
  स्थिर व्योम *तथ्य;
#अगर_अघोषित HB_NO_OT_SHAPE_FRACTIONS
  hb_mask_t frac_mask, numr_mask, dnom_mask;
#अन्यथा
  अटल constexpr hb_mask_t frac_mask = 0;
  अटल constexpr hb_mask_t numr_mask = 0;
  अटल constexpr hb_mask_t dnom_mask = 0;
#पूर्ण_अगर
  hb_mask_t rtlm_mask;
#अगर_अघोषित HB_NO_OT_KERN
  hb_mask_t kern_mask;
#अन्यथा
  अटल constexpr hb_mask_t kern_mask = 0;
#पूर्ण_अगर

#अगर_अघोषित HB_NO_OT_KERN
  bool requested_kerning : 1;
#अन्यथा
  अटल constexpr bool requested_kerning = false;
#पूर्ण_अगर
#अगर_अघोषित HB_NO_OT_SHAPE_FRACTIONS
  bool has_frac : 1;
#अन्यथा
  अटल constexpr bool has_frac = false;
#पूर्ण_अगर
  bool has_vert : 1;
  bool has_gpos_mark : 1;
  bool zero_marks : 1;
  bool fallback_glyph_classes : 1;
  bool fallback_mark_positioning : 1;
  bool adjust_mark_positioning_when_zeroing : 1;

  bool apply_gpos : 1;
#अगर_अघोषित HB_NO_OT_KERN
  bool apply_kern : 1;
#अन्यथा
  अटल constexpr bool apply_kern = false;
#पूर्ण_अगर
  bool apply_fallback_kern : 1;
#अगर_अघोषित HB_NO_AAT_SHAPE
  bool apply_kerx : 1;
  bool apply_morx : 1;
  bool apply_trak : 1;
#अन्यथा
  अटल constexpr bool apply_kerx = false;
  अटल constexpr bool apply_morx = false;
  अटल constexpr bool apply_trak = false;
#पूर्ण_अगर

  व्योम collect_lookups (hb_tag_t table_tag, hb_set_t *lookups) स्थिर
  {
    अचिन्हित पूर्णांक table_सूचकांक;
    चयन (table_tag) {
      हाल HB_OT_TAG_GSUB: table_सूचकांक = 0; अवरोध;
      हाल HB_OT_TAG_GPOS: table_सूचकांक = 1; अवरोध;
      शेष: वापस;
    }
    map.collect_lookups (table_सूचकांक, lookups);
  }

  HB_INTERNAL bool init0 (hb_face_t                     *face,
			  स्थिर hb_shape_plan_कुंजी_t     *key);
  HB_INTERNAL व्योम fini ();

  HB_INTERNAL व्योम substitute (hb_font_t *font, hb_बफर_t *बफर) स्थिर;
  HB_INTERNAL व्योम position (hb_font_t *font, hb_बफर_t *बफर) स्थिर;
};

काष्ठा hb_shape_plan_t;

काष्ठा hb_ot_shape_planner_t
{
  /* In the order that they are filled in. */
  hb_face_t *face;
  hb_segment_properties_t props;
  hb_ot_map_builder_t map;
#अगर_अघोषित HB_NO_AAT_SHAPE
  hb_aat_map_builder_t aat_map;
  bool apply_morx : 1;
#अन्यथा
  अटल constexpr bool apply_morx = false;
#पूर्ण_अगर
  bool script_zero_marks : 1;
  bool script_fallback_position : 1;
  स्थिर काष्ठा hb_ot_shaper_t *shaper;

  HB_INTERNAL hb_ot_shape_planner_t (hb_face_t                     *face,
				     स्थिर hb_segment_properties_t &props);

  HB_INTERNAL व्योम compile (hb_ot_shape_plan_t           &plan,
			    स्थिर hb_ot_shape_plan_कुंजी_t &key);
};


#पूर्ण_अगर /* HB_OT_SHAPE_HH */
