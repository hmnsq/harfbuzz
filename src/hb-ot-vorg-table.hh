/*
 * Copyright © 2018 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#ifndef HB_OT_VORG_TABLE_HH
#define HB_OT_VORG_TABLE_HH

#ଅନ୍ତର୍ଭାଵ "hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#define HB_OT_TAG_VORG HB_TAG('V','O','R','G')

ନାମାକାଶ OT {

template <typename Types>
ସଂରଚନା VertOriginMetric
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ glyph.cmp (g); }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି));
  }

  ସର୍ବସାଧାରଣ:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_STATIC (Types::HBGlyphID::static_size + FWORD::static_size);
};

template <typename Types>
ସଂରଚନା VORGFormat1_2
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version.to_int (); }

  HB_ALWAYS_INLINE
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_y_origin (hb_codepoint_t glyph) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି (!vertYOrigins.bfind (glyph, &i))
      ଫେରନ୍ତୁ defaultVertOriginY;
    ଫେରନ୍ତୁ vertYOrigins[i].vertOriginY;
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  ଶୂନ୍ୟ serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    ଯଦି (unlikely (!c->extend_min ((*ଏହି))))  ଫେରନ୍ତୁ;

    ଏହି->version.major = Types::size == 2 ? 1 : 2;
    ଏହି->version.minor = 0;

    ଏହି->defaultVertOriginY = defaultVertOriginY;
    ଏହି->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    auto *vorg_ମୌଳିକ = c->serializer->start_embed<VORGFormat1_2> ();
    ଯଦି (unlikely (!c->serializer->check_success (vorg_ମୌଳିକ))) return_trace (ମିଥ୍ୟା);

    auto it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ଅଚଳ VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_ମୂଲ୍ୟ_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		ଫେରନ୍ତୁ metric;
	      })
    ;

    /* serialize the new table */
    vorg_ମୌଳିକ->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  ସୁରକ୍ଷିତ:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::template SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY(6 + Types::HBUINT::static_size, vertYOrigins);
};

ସଂରଚନା VORG
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_y_origin (hb_codepoint_t glyph) ଅଚଳ
  {
    ବାଛ (u.version.v.major) {
    ସ୍ଥିତି 1: ଫେରନ୍ତୁ u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 2: ଫେରନ୍ତୁ u.version2.get_y_origin (glyph);
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ 0;
    }
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ବାଛ (u.version.v.major) {
    ସ୍ଥିତି 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 2: return_trace (u.version2.subset (c));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: return_trace (ମିଥ୍ୟା);
    }
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    ଯଦି (unlikely (!u.version.v.sanitize (c))) return_trace (ମିଥ୍ୟା);
    hb_barrier ();
    ବାଛ (u.version.v.major) {
    ସ୍ଥିତି 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    ସ୍ଥିତି 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: return_trace (ମିଥ୍ୟା);
    }
  }

  ସୁରକ୍ଷିତ:
  ସଂଘ {
  ସଂରଚନା {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
