/*
 * Copyright © 2012  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_సమితి_DIGEST_HH
#నిర్వచించు HB_సమితి_DIGEST_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-machinery.hh"

/*
 * The set-digests implement "filters" that support "approximate
 * member query".  Conceptually these are like Bloom Filter and
 * Quotient Filter, however, much smaller, faster, and designed
 * to fit the requirements of our uses for glyph coverage queries.
 *
 * Our filters are highly accurate if the lookup covers fairly local
 * set of glyphs, but fully flooded and ineffective if coverage is
 * all over the place.
 *
 * The way these are used is that the filter is first populated by
 * a lookup's or subtable's Coverage table(s), and then when we
 * want to apply the lookup or subtable to a glyph, before trying
 * to apply, we ask the filter if the glyph may be covered. If it's
 * not, we return early.  We can also match a digest against another
 * digest.
 *
 * We use these filters at three levels:
 *   - If the digest for all the glyphs in the buffer as a whole
 *     does not match the digest for the lookup, skip the lookup.
 *   - For each glyph, if it doesn't match the lookup digest,
 *     skip it.
 *   - For each glyph, if it doesn't match the subtable digest,
 *     skip it.
 *
 * The filter we use is a combination of three bits-pattern
 * filters. A bits-pattern filter checks a number of bits (5 or 6)
 * of the input number (glyph-id in most cases) and checks whether
 * its pattern is amongst the patterns of any of the accepted values.
 * The accepted patterns are represented as a "long" integer. Each
 * check is done using four bitwise operations only.
 */

స్థిర constexpr గుర్తులేని hb_సమితి_digest_shifts[] = {4, 0, 6};

నిర్మాణం hb_సమితి_digest_t
{
  // No science in these. Intuition and testing only.
  ఉపయోగించి ముసుగు_t = uint64_t;

  స్థిర constexpr గుర్తులేని n = ARRAY_నిడివి_CONST (hb_సమితి_digest_shifts);
  స్థిర constexpr గుర్తులేని ముసుగు_బైట్లు = పరిమాణం (ముసుగు_t);
  స్థిర constexpr గుర్తులేని ముసుగు_బిట్లు = పరిమాణం (ముసుగు_t) * 8;
  స్థిర constexpr hb_codepoint_t mb1 = ముసుగు_బిట్లు - 1;
  స్థిర constexpr ముసుగు_t one = 1;
  స్థిర constexpr ముసుగు_t all = (ముసుగు_t) -1;

  శూన్య init ()
  { కొరకు (గుర్తులేని i = 0; i < n; i++) masks[i] = 0; }

  శూన్య clear () { init (); }

  స్థిర hb_సమితి_digest_t నిండిన ()
  {
    hb_సమితి_digest_t d;
    కొరకు (గుర్తులేని i = 0; i < n; i++) d.masks[i] = all;
    తిరిగి d;
  }

  శూన్య union_ (స్థిరరాశి hb_సమితి_digest_t &o)
  { కొరకు (గుర్తులేని i = 0; i < n; i++) masks[i] |= o.masks[i]; }

  తార్కిక కూడు_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    తార్కిక ret;

    ret = అబద్ధం;
    కొరకు (గుర్తులేని i = 0; i < n; i++)
      ఒకవేళ (masks[i] != all)
	ret = నిజం;
    ఒకవేళ (!ret) తిరిగి అబద్ధం;

    ret = అబద్ధం;
    కొరకు (గుర్తులేని i = 0; i < n; i++)
    {
      ముసుగు_t జరుపు = hb_సమితి_digest_shifts[i];
      ఒకవేళ ((b >> జరుపు) - (a >> జరుపు) >= mb1)
	masks[i] = all;
      లేకపోతే
      {
	ముసుగు_t ma = one << ((a >> జరుపు) & mb1);
	ముసుగు_t mb = one << ((b >> జరుపు) & mb1);
	masks[i] |= mb + (mb - ma) - (mb < ma);
	ret = నిజం;
      }
    }
    తిరిగి ret;
  }

  మూస <రకం_పేరు T>
  శూన్య కూడు_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
    {
      కూడు (*array);
      array = &Struct_At_స్థానభ్రంశం_Unaligned<T> ((స్థిరరాశి శూన్య *) array, stride);
    }
  }
  మూస <రకం_పేరు T>
  శూన్య కూడు_array (స్థిరరాశి hb_array_t<స్థిరరాశి T>& arr) { కూడు_array (&arr, arr.len ()); }
  మూస <రకం_పేరు T>
  తార్కిక కూడు_sorted_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  {
    కూడు_array (array, లెక్క, stride);
    తిరిగి నిజం;
  }
  మూస <రకం_పేరు T>
  తార్కిక కూడు_sorted_array (స్థిరరాశి hb_sorted_array_t<స్థిరరాశి T>& arr) { తిరిగి కూడు_sorted_array (&arr, arr.len ()); }

  తార్కిక క్రియాకారకం [] (hb_codepoint_t g) స్థిరరాశి
  { తిరిగి may_have (g); }


  శూన్య కూడు (hb_codepoint_t g)
  {
    కొరకు (గుర్తులేని i = 0; i < n; i++)
      masks[i] |= one << ((g >> hb_సమితి_digest_shifts[i]) & mb1);
  }

  HB_ALWAYS_INLINE
  తార్కిక may_have (hb_codepoint_t g) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < n; i++)
      ఒకవేళ (!(masks[i] & (one << ((g >> hb_సమితి_digest_shifts[i]) & mb1))))
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  తార్కిక may_intersect (స్థిరరాశి hb_సమితి_digest_t &o) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < n; i++)
      ఒకవేళ (!(masks[i] & o.masks[i]))
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  అంతరంగ:

  ముసుగు_t masks[n] = {};
};


#అయితే_ముగింపు /* HB_SET_DIGEST_HH */
