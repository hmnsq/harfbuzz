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

#अन्तर्भाव "hb-open-type.hh"
#अन्तर्भाव "hb-ot-dual.hh"

/*
 * VORG -- Vertical Origin Table
 * https://docs.microsoft.com/en-us/typography/opentype/spec/vorg
 */
#define HB_OT_TAG_VORG HB_TAG('V','O','R','G')

नामाकाश OT {

ढाँचा <typename Types>
संरचना VertOriginMetric
{
  पूर्णाङ्क cmp (hb_codepoint_t g) ध्रुव { प्रत्यागम glyph.cmp (g); }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  सार्वजनिक:
  typename Types::HBGlyphID
		glyph;
  FWORD		vertOriginY;

  सार्वजनिक:
  DEFINE_SIZE_STATIC (Types::HBGlyphID::static_size + FWORD::static_size);
};

ढाँचा <typename Types>
संरचना VORGFormat1_2
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  तार्किक has_data () ध्रुव { प्रत्यागम version.to_int (); }

  HB_ALWAYS_INLINE
  पूर्णाङ्क get_y_origin (hb_codepoint_t glyph) ध्रुव
  {
    अचिन्हित पूर्णाङ्क i;
    यदि (!vertYOrigins.bfind (glyph, &i))
      प्रत्यागम defaultVertOriginY;
    प्रत्यागम vertYOrigins[i].vertOriginY;
  }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    यदि (unlikely (!c->extend_min ((*एतत्))))  प्रत्यागम;

    एतत्->version.major = Types::size == 2 ? 1 : 2;
    एतत्->version.minor = 0;

    एतत्->defaultVertOriginY = defaultVertOriginY;
    एतत्->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    स्वतः *vorg_अभाज्य = c->serializer->start_embed<VORGFormat1_2> ();
    यदि (unlikely (!c->serializer->check_success (vorg_अभाज्य))) return_trace (असत्य);

    स्वतः it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (ध्रुव VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_मूल्य_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		प्रत्यागम metric;
	      })
    ;

    /* serialize the new table */
    vorg_अभाज्य->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  रक्षित:
  FixedVersion<>version;	/* Version of VORG table. */
  FWORD		defaultVertOriginY;
				/* The default vertical origin. */
  typename Types::ढाँचा SortedArrayOf<VertOriginMetric<Types>>
		vertYOrigins;	/* The array of vertical origins. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY(6 + Types::HBUINT::static_size, vertYOrigins);
};

संरचना VORG
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_VORG;

  तार्किक has_data () ध्रुव { प्रत्यागम u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  पूर्णाङ्क get_y_origin (hb_codepoint_t glyph) ध्रुव
  {
    विकल्प (u.version.v.major) {
    पक्ष 1: प्रत्यागम u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: प्रत्यागम u.version2.get_y_origin (glyph);
#endif
    स्वभाव: प्रत्यागम 0;
    }
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    विकल्प (u.version.v.major) {
    पक्ष 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: return_trace (u.version2.subset (c));
#endif
    स्वभाव: return_trace (असत्य);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!u.version.v.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.version.v.major) {
    पक्ष 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    पक्ष 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    स्वभाव: return_trace (असत्य);
    }
  }

  रक्षित:
  सङ्घ {
  संरचना {
  FixedVersion<>v;		/* Version of VORG table. */
  }				version;
  VORGFormat1_2<Layout::SmallTypes>version1;
#ifndef HB_NO_BEYOND_64K
  VORGFormat1_2<Layout::MediumTypes>version2;
#endif
  } u;

  सार्वजनिक:
  DEFINE_SIZE_UNION (4, version.v);
};
} /* namespace OT */

#endif /* HB_OT_VORG_TABLE_HH */
