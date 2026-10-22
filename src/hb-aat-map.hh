/*
 * Copyright © 2018  Google, Inc.
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

#अगर_अघोषित HB_AAT_MAP_HH
#घोषणा HB_AAT_MAP_HH

#समावेश "hb.hh"


काष्ठा hb_aat_map_t
{
  मित्र काष्ठा hb_aat_map_builder_t;

  खुला:
  काष्ठा range_flags_t
  {
    hb_mask_t flags;
    अचिन्हित cluster_first;
    अचिन्हित cluster_last; // end - 1
  };

  खुला:
  hb_vector_t<hb_sorted_vector_t<range_flags_t>> chain_flags;
};

काष्ठा hb_aat_map_builder_t
{
  खुला:

  HB_INTERNAL hb_aat_map_builder_t (hb_face_t *face_,
				    स्थिर hb_segment_properties_t props_) :
				      face (face_),
				      props (props_) {}

  HB_INTERNAL व्योम add_feature (स्थिर hb_feature_t &feature);

  HB_INTERNAL व्योम compile (hb_aat_map_t  &m);

  खुला:
  काष्ठा feature_info_t
  {
    hb_aat_layout_feature_type_t  type;
    hb_aat_layout_feature_selector_t  setting;
    bool is_exclusive;
    अचिन्हित  seq; /* For stable sorting only. */

    HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
    {
      स्थिर feature_info_t *a = (स्थिर feature_info_t *) pa;
      स्थिर feature_info_t *b = (स्थिर feature_info_t *) pb;
      अगर (a->type != b->type) वापस (a->type < b->type ? -1 : 1);
      अगर (!a->is_exclusive &&
	  (a->setting & ~1) != (b->setting & ~1)) वापस (a->setting < b->setting ? -1 : 1);
	    वापस (a->seq < b->seq ? -1 : a->seq > b->seq ? 1 : 0);
    }

    /* compares type & setting only */
    पूर्णांक cmp (स्थिर feature_info_t& f) स्थिर
    {
      वापस (f.type != type) ? (f.type < type ? -1 : 1) :
	     (f.setting != setting) ? (f.setting < setting ? -1 : 1) : 0;
    }
  };

  काष्ठा feature_range_t
  {
    feature_info_t info;
    अचिन्हित start;
    अचिन्हित end;
  };

  निजी:
  काष्ठा feature_event_t
  {
    अचिन्हित पूर्णांक सूचकांक_2;
    bool start;
    feature_info_t feature;

    HB_INTERNAL अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb) {
      स्थिर feature_event_t *a = (स्थिर feature_event_t *) pa;
      स्थिर feature_event_t *b = (स्थिर feature_event_t *) pb;
      वापस a->सूचकांक_2 < b->सूचकांक_2 ? -1 : a->सूचकांक_2 > b->सूचकांक_2 ? 1 :
	     a->start < b->start ? -1 : a->start > b->start ? 1 :
	     feature_info_t::cmp (&a->feature, &b->feature);
    }
  };

  खुला:
  hb_face_t *face;
  hb_segment_properties_t props;

  खुला:
  hb_sorted_vector_t<feature_range_t> features;
  hb_sorted_vector_t<feature_info_t> current_features;
  अचिन्हित range_first = HB_FEATURE_GLOBAL_START;
  अचिन्हित range_last = HB_FEATURE_GLOBAL_END;
};


#पूर्ण_अगर /* HB_AAT_MAP_HH */
