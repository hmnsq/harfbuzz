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
  पूर्णांक cmp (hb_codepoint_t g) स्थिरांक { परत glyph.cmp (g); }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
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

  तार्किक has_data () स्थिरांक { परत version.to_int (); }

  HB_ALWAYS_INLINE
  पूर्णांक get_y_origin (hb_codepoint_t glyph) स्थिरांक
  {
    अचिन्हित पूर्णांक i;
    जर (!vertYOrigins.bfind (glyph, &i))
      परत defaultVertOriginY;
    परत vertYOrigins[i].vertOriginY;
  }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  Iterator it,
		  FWORD defaultVertOriginY)
  {

    जर (unlikely (!c->extend_min ((*हे))))  परत;

    हे->version.major = Types::size == 2 ? 1 : 2;
    हे->version.minor = 0;

    हे->defaultVertOriginY = defaultVertOriginY;
    हे->vertYOrigins.len = it.len ();

    c->copy_all (it);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *vorg_मूळसंख्या = c->serializer->start_embed<VORGFormat1_2> ();
    जर (unlikely (!c->serializer->check_success (vorg_मूळसंख्या))) return_trace (असत्य);

    स्वतः it =
    + vertYOrigins.as_array ()
    | hb_filter (c->plan->glyphset (), &VertOriginMetric<Types>::glyph)
    | hb_map ([&] (स्थिरांक VertOriginMetric<Types>& _)
	      {
		hb_codepoint_t new_glyph = HB_SET_मूल्य_INVALID;
		c->plan->new_gid_for_old_gid (_.glyph, &new_glyph);

		VertOriginMetric<Types> metric;
		metric.glyph = new_glyph;
		metric.vertOriginY = _.vertOriginY;
		परत metric;
	      })
    ;

    /* serialize the new table */
    vorg_मूळसंख्या->serialize (c->serializer, it, defaultVertOriginY);
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  version.major == (Types::size == 2 ? 1 : 2) &&
		  vertYOrigins.sanitize (c));
  }

  संरक्षित:
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

  तार्किक has_data () स्थिरांक { परत u.version.v.to_int (); }

  HB_ALWAYS_INLINE
  पूर्णांक get_y_origin (hb_codepoint_t glyph) स्थिरांक
  {
    निवड (u.version.v.major) {
    स्थिती 1: परत u.version1.get_y_origin (glyph);
#ifndef HB_NO_BEYOND_64K
    स्थिती 2: परत u.version2.get_y_origin (glyph);
#endif
    पूर्वनिर्धारित: परत 0;
    }
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    निवड (u.version.v.major) {
    स्थिती 1: return_trace (u.version1.subset (c));
#ifndef HB_NO_BEYOND_64K
    स्थिती 2: return_trace (u.version2.subset (c));
#endif
    पूर्वनिर्धारित: return_trace (असत्य);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!u.version.v.sanitize (c))) return_trace (असत्य);
    hb_barrier ();
    निवड (u.version.v.major) {
    स्थिती 1: hb_barrier (); return_trace (u.version1.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    स्थिती 2: hb_barrier (); return_trace (u.version2.sanitize (c));
#endif
    पूर्वनिर्धारित: return_trace (असत्य);
    }
  }

  संरक्षित:
  संघ {
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
