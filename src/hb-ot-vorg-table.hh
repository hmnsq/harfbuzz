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

#अगर_अघोषित HB_OT_VORG_TABLE_HH
#घोषणा HB_OT_VORG_TABLE_HH

#समावेश "hb-open-type.hh"
#समावेश "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#घोषणा HB_OT_TAG_VORG HB_TAG('V','O','R','G')

namespace OT {

ढाँचा <typename Types>
काष्ठा VertOriginMetric
{
  पूर्णांक cmp (hb_codepoint_t g) स्थिर { वापस glyph.cmp (g); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  खुला:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  खुला:
  DEFINE_आकार_STATIC (Types::HBGlyphID::static_आकार + FWORD::static_आकार);
};

ढाँचा <typename Types>
काष्ठा VORGFormat1_2
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  bool has_तथ्य () स्थिर { वापस version.to_int (); }

  HB_ALWAYS_INLINE
  पूर्णांक get_y_origin (hb_codepoint_t glyph) स्थिर
  {
    अचिन्हित पूर्णांक i;
    अगर (!vertYOrigins.bfind (glyph, &i))
      वापस defaultVertOriginY;
    वापस vertYOrigins[i].vertOriginY;
  }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    अगर (unlikely (!c->extend_min ((*यह))))  वापस;

    यह->version.major = Types::आकार == 2 ? 1 : 2;
    यह->version.minor = 0;

    यह->defaultVertOriginY = defaultVertOriginY;
    यह->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *vorg_अभाज्य = c->serializer->start_embed<VORGFormat1_2> ();
    अगर (unlikely (!c->serializer->check_success (vorg_अभाज्य))) return_trace (false);

    स्वतः it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (स्थिर VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_मूल्य_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		वापस metric;
	      })
    ;

    /* serialize the new table */
    vorg_अभाज्य->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  version.major == (Types::आकार == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  रक्षित:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::ढाँचा SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  खुला:
  DEFINE_आकार_ARRAY(6 + Types::HBUINT::static_आकार, vertYOrigins);
};

काष्ठा VORG
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  bool has_तथ्य () स्थिर { वापस u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  पूर्णांक get_y_origin (hb_codepoint_t glyph) स्थिर
  {
    चयन (u.version.v.major) {
    हाल 1: वापस u.version1.get_y_origin (glyph);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 2: वापस u.version2.get_y_origin (glyph);
#पूर्ण_अगर
    शेष: वापस 0;
    }
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    चयन (u.version.v.major) {
    हाल 1: return_trace (u.version1.subset (c));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 2: return_trace (u.version2.subset (c));
#पूर्ण_अगर
    शेष: return_trace (false);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!u.version.v.sanitize (c))) return_trace (false);
    hb_barrier ();
    चयन (u.version.v.major) {
    हाल 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#पूर्ण_अगर
    शेष: return_trace (false);
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#अगर_अघोषित HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#पूर्ण_अगर
  } u;

  खुला:
  DEFINE_आकार_UNION (4, version.v);
};
} /* namespace OT */

#पूर्ण_अगर /* HB_OT_VORG_TABLE_HH */
