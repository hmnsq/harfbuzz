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

#அந்தர்பாவ "hb-open-type.hh"
#அந்தர்பாவ "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#define HB_OT_TAG_VORG HB_TAG('V','O','R','G')

நாமாகாஶ OT {

template <typename Types>
கட்டமைப்பு VertOriginMetric
{
  முழு cmp (hb_codepoint_t g) மாறிலி { திரும்பு glyph.cmp (g); }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது));
  }

  பொது:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  பொது:
  DEFINE_SIZE_STATIC (Types::HBGlyphID::static_size + FWORD::static_size);
};

template <typename Types>
கட்டமைப்பு VORGFormat1_2
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  தருக்க has_data () மாறிலி { திரும்பு version.to_int (); }

  HB_ALWAYS_INLINE
  முழு get_y_origin (hb_codepoint_t glyph) மாறிலி
  {
    unsigned முழு i;
    எனில் (!vertYOrigins.bfind (glyph, &i))
      திரும்பு defaultVertOriginY;
    திரும்பு vertYOrigins[i].vertOriginY;
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  வெறுமை serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    எனில் (unlikely (!c->extend_min ((*இது))))  திரும்பு;

    இது->version.major = Types::size == 2 ? 1 : 2;
    இது->version.minor = 0;

    இது->defaultVertOriginY = defaultVertOriginY;
    இது->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    auto *vorg_பகாஎண் = c->serializer->start_embed<VORGFormat1_2> ();
    எனில் (unlikely (!c->serializer->check_success (vorg_பகாஎண்))) return_trace (பொய்);

    auto it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (மாறிலி VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_மதிப்பு_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		திரும்பு metric;
	      })
    ;

    /* serialize the new table */
    vorg_பகாஎண்->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (உண்மை);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  பாதுகாக்கப்பட்ட:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::template SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  பொது:
  DEFINE_SIZE_ARRAY(6 + Types::HBUINT::static_size, vertYOrigins);
};

கட்டமைப்பு VORG
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  தருக்க has_data () மாறிலி { திரும்பு u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  முழு get_y_origin (hb_codepoint_t glyph) மாறிலி
  {
    தேர்வு (u.version.v.major) {
    நிலை 1: திரும்பு u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    நிலை 2: திரும்பு u.version2.get_y_origin (glyph);
#endif
    இயல்பு: திரும்பு 0;
    }
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    தேர்வு (u.version.v.major) {
    நிலை 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    நிலை 2: return_trace (u.version2.subset (c));
#endif
    இயல்பு: return_trace (பொய்);
    }
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    எனில் (unlikely (!u.version.v.sanitize (c))) return_trace (பொய்);
    hb_barrier ();
    தேர்வு (u.version.v.major) {
    நிலை 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    நிலை 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    இயல்பு: return_trace (பொய்);
    }
  }

  பாதுகாக்கப்பட்ட:
  ஒன்றிப்பு {
  கட்டமைப்பு {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  பொது:
  DEFINE_SIZE_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
