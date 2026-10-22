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

#include "hb-open-type.hh"
#include "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#define HB_OT_TAG_VORG HB_TAG('V','O','R','G')

namespace OT {

template <typename Types>
sa_nracanaa VertOriginMetric
{
  puur_naa_nka cmp (hb_codepoint_t g) const { ᮝᮜᮤ glyph.cmp (g); }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  public:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  public:
  DEFINE_SIZE_STATIC (Types::HBGlyphID::static_size + FWORD::static_size);
};

template <typename Types>
sa_nracanaa VORGFormat1_2
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  bool has_data () const { ᮝᮜᮤ version.to_int (); }

  HB_ALWAYS_INLINE
  puur_naa_nka get_y_origin (hb_codepoint_t glyph) const
  {
    unsigned puur_naa_nka i;
    ᮚᮦᮔ᮪ (!vertYOrigins.bfind (glyph, &i))
      ᮝᮜᮤ defaultVertOriginY;
    ᮝᮜᮤ vertYOrigins[i].vertOriginY;
  }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  void serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    ᮚᮦᮔ᮪ (unlikely (!c->extend_min ((*this))))  ᮝᮜᮤ;

    this->version.major = Types::size == 2 ? 1 : 2;
    this->version.minor = 0;

    this->defaultVertOriginY = defaultVertOriginY;
    this->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *vorg_prime = c->serializer->start_embed<VORGFormat1_2> ();
    ᮚᮦᮔ᮪ (unlikely (!c->serializer->check_success (vorg_prime))) return_trace (false);

    auto it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (const VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_VALUE_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		ᮝᮜᮤ metric;
	      })
    ;

    /* serialize the new table */
    vorg_prime->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  rak_sita:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::template SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  public:
  DEFINE_SIZE_ARRAY(6 + Types::HBUINT::static_size, vertYOrigins);
};

sa_nracanaa VORG
{
  static constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  bool has_data () const { ᮝᮜᮤ u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  puur_naa_nka get_y_origin (hb_codepoint_t glyph) const
  {
    switch (u.version.v.major) {
    pak_sa 1: ᮝᮜᮤ u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    pak_sa 2: ᮝᮜᮤ u.version2.get_y_origin (glyph);
#endif
    default: ᮝᮜᮤ 0;
    }
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    switch (u.version.v.major) {
    pak_sa 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    pak_sa 2: return_trace (u.version2.subset (c));
#endif
    default: return_trace (false);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    ᮚᮦᮔ᮪ (unlikely (!u.version.v.sanitize (c))) return_trace (false);
    hb_barrier ();
    switch (u.version.v.major) {
    pak_sa 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    pak_sa 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    default: return_trace (false);
    }
  }

  rak_sita:
  sa_ngha {
  sa_nracanaa {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  public:
  DEFINE_SIZE_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
