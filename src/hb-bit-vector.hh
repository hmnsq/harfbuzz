/*
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
 * Author(s): Behdad Esfahbod
 */

#నిర్వచితం_కాకపోతే HB_బిట్టు_సదిశ_HH
#నిర్వచించు HB_బిట్టు_సదిశ_HH

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-atomic.hh"

నిర్మాణం hb_min_max_t
{
  శూన్య కూడు (hb_codepoint_t v) { min_v = hb_min (min_v, v); max_v = hb_max (max_v, v); }
  శూన్య కూడు_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    min_v = hb_min (min_v, a);
    max_v = hb_max (max_v, b);
  }

  మూస <రకం_పేరు సమితి_t>
  శూన్య union_ (స్థిరరాశి సమితి_t &సమితి)
  {
    hb_codepoint_t సమితి_min = సమితి.get_min ();
    ఒకవేళ (unlikely (సమితి_min == HB_CODEPOINT_INVALID))
      తిరిగి;
    hb_codepoint_t సమితి_max = సమితి.get_max ();
    min_v = hb_min (min_v, సమితి_min);
    max_v = hb_max (max_v, సమితి_max);
  }

  hb_codepoint_t get_min () స్థిరరాశి { తిరిగి min_v; }
  hb_codepoint_t get_max () స్థిరరాశి { తిరిగి max_v; }

  అంతరంగ:
  hb_codepoint_t min_v = HB_CODEPOINT_INVALID;
  hb_codepoint_t max_v = 0;
};

మూస <తార్కిక atomic = అబద్ధం>
నిర్మాణం hb_బిట్టు_సదిశ_t
{
  ఉపయోగించి int_t = uint64_t;
  ఉపయోగించి elt_t = రకం_పేరు std::conditional<atomic, hb_atomic_t<int_t>, int_t>::type;

  hb_బిట్టు_సదిశ_t () = తొలగించు;
  hb_బిట్టు_సదిశ_t (స్థిరరాశి hb_బిట్టు_సదిశ_t &other) = తొలగించు;
  hb_బిట్టు_సదిశ_t &క్రియాకారకం= (స్థిరరాశి hb_బిట్టు_సదిశ_t &other) = తొలగించు;

  // Move
  hb_బిట్టు_సదిశ_t (hb_బిట్టు_సదిశ_t &&other)
		: min_v (other.min_v), max_v (other.max_v), లెక్క (other.లెక్క), elts (other.elts)
  {
    other.min_v = other.max_v = other.లెక్క = 0;
    other.elts = శూన్యసూచిక;
  }
  hb_బిట్టు_సదిశ_t &క్రియాకారకం= (hb_బిట్టు_సదిశ_t &&other)
  {
    hb_మార్పిడి (min_v, other.min_v);
    hb_మార్పిడి (max_v, other.max_v);
    hb_మార్పిడి (లెక్క, other.లెక్క);
    hb_మార్పిడి (elts, other.elts);
    తిరిగి *ఇది;
  }

  hb_బిట్టు_సదిశ_t (గుర్తులేని min_v, గుర్తులేని max_v)
    : min_v (min_v), max_v (max_v)
  {
    ఒకవేళ (unlikely (min_v >= max_v))
    {
      min_v = max_v = లెక్క = 0;
      తిరిగి;
    }

    గుర్తులేని అంకె = (max_v - min_v + పరిమాణం (int_t) * 8) / (పరిమాణం (int_t) * 8);
    elts = (elt_t *) hb_calloc (అంకె, పరిమాణం (int_t));
    ఒకవేళ (unlikely (!elts))
    {
      min_v = max_v = లెక్క = 0;
      తిరిగి;
    }

    లెక్క = max_v - min_v + 1;
  }
  ~hb_బిట్టు_సదిశ_t ()
  {
    hb_free (elts);
  }

  శూన్య కూడు (hb_codepoint_t g) { elt (g) |= ముసుగు (g); }
  శూన్య del (hb_codepoint_t g) { elt (g) &= ~ముసుగు (g); }
  శూన్య సమితి (hb_codepoint_t g, తార్కిక value) { ఒకవేళ (value) కూడు (g); లేకపోతే del (g); }
  తార్కిక get (hb_codepoint_t g) స్థిరరాశి { తిరిగి elt (g) & ముసుగు (g); }
  తార్కిక has (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }
  తార్కిక may_have (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }

  తార్కిక క్రియాకారకం [] (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }
  తార్కిక క్రియాకారకం () (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }

  శూన్య కూడు_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ఒకవేళ (unlikely (!లెక్క || a > b || a < min_v || b > max_v))
      తిరిగి;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ఒకవేళ (la == lb)
      *la |= (ముసుగు (b) << 1) - ముసుగు(a);
    లేకపోతే
    {
      *la |= ~(ముసుగు (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (అక్షరం *) lb - (అక్షరం *) la);

      *lb |= ((ముసుగు (b) << 1) - 1llu);
    }
  }
  శూన్య del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ఒకవేళ (unlikely (!లెక్క || a > b || a < min_v || b > max_v))
      తిరిగి;

    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ఒకవేళ (la == lb)
      *la &= ~((ముసుగు (b) << 1llu) - ముసుగు(a));
    లేకపోతే
    {
      *la &= ముసుగు (a) - 1;
      la++;

      hb_memset (la, 0, (అక్షరం *) lb - (అక్షరం *) la);

      *lb &= ~((ముసుగు (b) << 1) - 1llu);
    }
  }
  శూన్య సమితి_range (hb_codepoint_t a, hb_codepoint_t b, తార్కిక v)
  { ఒకవేళ (v) కూడు_range (a, b); లేకపోతే del_range (a, b); }

  మూస <రకం_పేరు సమితి_t>
  శూన్య union_ (స్థిరరాశి సమితి_t &సమితి)
  {
    కొరకు (hb_codepoint_t g : సమితి)
      కూడు (g);
  }

  స్థిర స్థిరరాశి గుర్తులేని పూర్ణాంకం ELT_బిట్లు = పరిమాణం (elt_t) * 8;
  స్థిర constexpr గుర్తులేని ELT_ముసుగు = ELT_బిట్లు - 1;

  స్థిర constexpr elt_t zero = 0;

  elt_t &elt (hb_codepoint_t g)
  {
    g -= min_v;
    ఒకవేళ (unlikely (g >= లెక్క))
      తిరిగి Crap(elt_t);
    తిరిగి elts[g / ELT_బిట్లు];
  }
  స్థిరరాశి elt_t& elt (hb_codepoint_t g) స్థిరరాశి
  {
    g -= min_v;
    ఒకవేళ (unlikely (g >= లెక్క))
      తిరిగి Null(elt_t);
    తిరిగి elts[g / ELT_బిట్లు];
  }

  స్థిర constexpr int_t ముసుగు (hb_codepoint_t g) { తిరిగి elt_t (1) << (g & ELT_ముసుగు); }

  hb_codepoint_t min_v = 0, max_v = 0, లెక్క = 0;
  elt_t *elts = శూన్యసూచిక;
};


#అయితే_ముగింపు /* HB_BIT_VECTOR_HH */
