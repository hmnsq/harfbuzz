/*
 * Copyright © 2012,2017  Google, Inc.
 * Copyright © 2021 Behdad Esfahbod
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

#నిర్వచితం_కాకపోతే HB_బిట్టు_పుట_HH
#నిర్వచించు HB_బిట్టు_పుట_HH

#అన్తర్భావ "hb.hh"


/* Compiler-assisted vectorization. */

/* Type behaving similar to vectorized vars defined using __attribute__((vector_size(...))),
 * basically a fixed-size bitset. We can't use the compiler type because hb_vector_t cannot
 * guarantee alignment requirements. */
మూస <రకం_పేరు elt_t, గుర్తులేని పూర్ణాంకం byte_కొలత>
నిర్మాణం hb_సదిశ_కొలత_t
{
  elt_t& క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం i) { తిరిగి v[i]; }
  స్థిరరాశి elt_t& క్రియాకారకం [] (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి { తిరిగి v[i]; }

  శూన్య init0 ()
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (v); i++)
      v[i] = 0;
  }
  శూన్య init1 ()
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (v); i++)
      v[i] = (elt_t) -1;
  }

  మూస <రకం_పేరు Op>
  hb_సదిశ_కొలత_t process (స్థిరరాశి Op& op) స్థిరరాశి
  {
    hb_సదిశ_కొలత_t r;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (v); i++)
      r.v[i] = op (v[i]);
    తిరిగి r;
  }
  మూస <రకం_పేరు Op>
  hb_సదిశ_కొలత_t process (స్థిరరాశి Op& op, స్థిరరాశి hb_సదిశ_కొలత_t &o) స్థిరరాశి
  {
    hb_సదిశ_కొలత_t r;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (v); i++)
      r.v[i] = op (v[i], o.v[i]);
    తిరిగి r;
  }
  hb_సదిశ_కొలత_t క్రియాకారకం | (స్థిరరాశి hb_సదిశ_కొలత_t &o) స్థిరరాశి
  { తిరిగి process (hb_bitwise_or, o); }
  hb_సదిశ_కొలత_t క్రియాకారకం & (స్థిరరాశి hb_సదిశ_కొలత_t &o) స్థిరరాశి
  { తిరిగి process (hb_bitwise_and, o); }
  hb_సదిశ_కొలత_t క్రియాకారకం ^ (స్థిరరాశి hb_సదిశ_కొలత_t &o) స్థిరరాశి
  { తిరిగి process (hb_bitwise_xor, o); }
  hb_సదిశ_కొలత_t క్రియాకారకం ~ () స్థిరరాశి
  { తిరిగి process (hb_bitwise_neg); }

  క్రియాకారకం తార్కిక () స్థిరరాశి
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (v); i++)
      ఒకవేళ (v[i])
	తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }
  క్రియాకారకం గుర్తులేని పూర్ణాంకం () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం r = 0;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (v); i++)
      r += hb_popcount (v[i]);
    తిరిగి r;
  }
  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_సదిశ_కొలత_t &o) స్థిరరాశి
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < ARRAY_నిడివి (v); i++)
      ఒకవేళ (v[i] != o.v[i])
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  hb_array_t<స్థిరరాశి elt_t> iter () స్థిరరాశి
  { తిరిగి hb_array (v); }

  అంతరంగ:
  static_assert (0 == byte_కొలత % పరిమాణం (elt_t), "");
  elt_t v[byte_కొలత / పరిమాణం (elt_t)];
};


నిర్మాణం hb_బిట్టు_పుట_t
{
  hb_బిట్టు_పుట_t () { init0 (); }

  శూన్య init0 () { v.init0 (); population = 0; }
  శూన్య init1 () { v.init1 (); population = పుట_బిట్లు; }

  శూన్య dirty () { population = UINT_MAX; }

  స్థిర అంతఃవరుస constexpr గుర్తులేని len ()
  { తిరిగి ARRAY_నిడివి_CONST (v); }

  క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి !is_empty (); }
  తార్కిక is_empty () స్థిరరాశి
  {
    ఒకవేళ (has_population ()) తిరిగి !population;
    తార్కిక ఖాళీలు_2 = !v;
    ఒకవేళ (ఖాళీలు_2) population = 0;
    తిరిగి ఖాళీలు_2;
  }
  uint32_t hash () స్థిరరాశి
  {
    తిరిగి hb_బైట్లు_t ((స్థిరరాశి అక్షరం *) &v, పరిమాణం (v)).hash ();
  }

  శూన్య కూడు (hb_codepoint_t g) { elt (g) |= ముసుగు (g); dirty (); }
  శూన్య కూడు_బిట్లు (hb_codepoint_t g, uint64_t బిట్లు) { elt (g) |= బిట్లు; dirty (); }
  శూన్య del (hb_codepoint_t g) { elt (g) &= ~ముసుగు (g); dirty (); }
  శూన్య del_బిట్లు (hb_codepoint_t g, uint64_t బిట్లు) { elt (g) &= ~బిట్లు; dirty (); }
  శూన్య సమితి (hb_codepoint_t g, తార్కిక value) { ఒకవేళ (value) కూడు (g); లేకపోతే del (g); }
  తార్కిక get (hb_codepoint_t g) స్థిరరాశి { తిరిగి elt (g) & ముసుగు (g); }
  తార్కిక may_have (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }

  తార్కిక క్రియాకారకం [] (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }
  తార్కిక క్రియాకారకం () (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }
  తార్కిక has (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }

  శూన్య కూడు_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ఒకవేళ (la == lb)
      *la |= (ముసుగు (b) << 1) - ముసుగు(a);
    లేకపోతే ఒకవేళ (likely (la < lb))
    {
      *la |= ~(ముసుగు (a) - 1llu);
      la++;

      hb_memset (la, 0xff, (అక్షరం *) lb - (అక్షరం *) la);

      *lb |= ((ముసుగు (b) << 1) - 1llu);
    }
    లేకపోతే
      తిరిగి;
    dirty ();
  }
  శూన్య del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    elt_t *la = &elt (a);
    elt_t *lb = &elt (b);
    ఒకవేళ (la == lb)
      *la &= ~((ముసుగు (b) << 1llu) - ముసుగు(a));
    లేకపోతే ఒకవేళ (likely (la < lb))
    {
      *la &= ముసుగు (a) - 1;
      la++;

      hb_memset (la, 0, (అక్షరం *) lb - (అక్షరం *) la);

      *lb &= ~((ముసుగు (b) << 1) - 1llu);
    }
    లేకపోతే
      తిరిగి;
    dirty ();
  }
  శూన్య సమితి_range (hb_codepoint_t a, hb_codepoint_t b, తార్కిక v)
  { ఒకవేళ (v) కూడు_range (a, b); లేకపోతే del_range (a, b); }


  // Writes out page values to the array p. Returns the number of values
  // written. At most size codepoints will be written.
  గుర్తులేని పూర్ణాంకం లిఖ (uint32_t        పునాది,
		      గుర్తులేని పూర్ణాంకం    ప్రారంభం_విలువ,
		      hb_codepoint_t *p,
		      గుర్తులేని పూర్ణాంకం    కొలత) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం ప్రారంభం_v = ప్రారంభం_విలువ / ELT_బిట్లు;
    గుర్తులేని పూర్ణాంకం ప్రారంభం_బిట్టు = ప్రారంభం_విలువ & ELT_ముసుగు;
    గుర్తులేని పూర్ణాంకం లెక్క = 0;
    కొరకు (గుర్తులేని i = ప్రారంభం_v; i < len () && లెక్క < కొలత; i++)
    {
      elt_t బిట్లు = v[i];
      uint32_t v_పునాది = పునాది | (i * ELT_బిట్లు);
      కొరకు (గుర్తులేని పూర్ణాంకం j = ప్రారంభం_బిట్టు; j < ELT_బిట్లు && లెక్క < కొలత; j++)
      {
	ఒకవేళ ((elt_t(1) << j) & బిట్లు) {
	  *p++ = v_పునాది | j;
	  లెక్క++;
	}
      }
      ప్రారంభం_బిట్టు = 0;
    }
    తిరిగి లెక్క;
  }

  // Writes out the values NOT in this page to the array p. Returns the
  // number of values written. At most size codepoints will be written.
  // Returns the number of codepoints written. next_value holds the next value
  // that should be written (if not present in this page). This is used to fill
  // any missing value gaps between this page and the previous page, if any.
  // next_value is updated to one more than the last value present in this page.
  గుర్తులేని పూర్ణాంకం write_inverted (uint32_t        పునాది,
			       గుర్తులేని పూర్ణాంకం    ప్రారంభం_విలువ,
			       hb_codepoint_t *p,
			       గుర్తులేని పూర్ణాంకం    కొలత,
			       hb_codepoint_t *తదుపరి_విలువ) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం ప్రారంభం_v = ప్రారంభం_విలువ / ELT_బిట్లు;
    గుర్తులేని పూర్ణాంకం ప్రారంభం_బిట్టు = ప్రారంభం_విలువ & ELT_ముసుగు;
    గుర్తులేని పూర్ణాంకం లెక్క = 0;
    కొరకు (గుర్తులేని i = ప్రారంభం_v; i < len () && లెక్క < కొలత; i++)
    {
      elt_t బిట్లు = v[i];
      uint32_t v_స్థానభ్రంశం = i * ELT_బిట్లు;
      కొరకు (గుర్తులేని పూర్ణాంకం j = ప్రారంభం_బిట్టు; j < ELT_బిట్లు && లెక్క < కొలత; j++)
      {
	ఒకవేళ ((elt_t(1) << j) & బిట్లు)
	{
	  hb_codepoint_t value = పునాది | v_స్థానభ్రంశం | j;
	  // Emit all the missing values from next_value up to value - 1.
	  కొరకు (hb_codepoint_t k = *తదుపరి_విలువ; k < value && లెక్క < కొలత; k++)
	  {
	    *p++ = k;
	    లెక్క++;
	  }
	  // Skip over this value;
	  *తదుపరి_విలువ = value + 1;
	}
      }
      ప్రారంభం_బిట్టు = 0;
    }
    తిరిగి లెక్క;
  }

  తార్కిక క్రియాకారకం == (స్థిరరాశి hb_బిట్టు_పుట_t &other) స్థిరరాశి { తిరిగి is_equal (other); }
  తార్కిక is_equal (స్థిరరాశి hb_బిట్టు_పుట_t &other) స్థిరరాశి { తిరిగి v == other.v; }
  తార్కిక intersects (స్థిరరాశి hb_బిట్టు_పుట_t &other) స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < len (); i++)
      ఒకవేళ (v[i] & other.v[i])
	తిరిగి నిజం;
    తిరిగి అబద్ధం;
  }
  తార్కిక may_intersect (స్థిరరాశి hb_బిట్టు_పుట_t &other) స్థిరరాశి
  { తిరిగి intersects (other); }

  తార్కిక క్రియాకారకం <= (స్థిరరాశి hb_బిట్టు_పుట_t &larger_పుట) స్థిరరాశి { తిరిగి is_subset (larger_పుట); }
  తార్కిక is_subset (స్థిరరాశి hb_బిట్టు_పుట_t &larger_పుట) స్థిరరాశి
  {
    ఒకవేళ (has_population () && larger_పుట.has_population () &&
	population > larger_పుట.population)
      తిరిగి అబద్ధం;

    కొరకు (గుర్తులేని i = 0; i < len (); i++)
      ఒకవేళ (~larger_పుట.v[i] & v[i])
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }

  తార్కిక has_population () స్థిరరాశి { తిరిగి population != UINT_MAX; }
  గుర్తులేని get_population () స్థిరరాశి
  {
    ఒకవేళ (has_population ()) తిరిగి population;
    తిరిగి population = v;
  }

  తార్కిక next (hb_codepoint_t *codepoint) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం m = (*codepoint + 1) & ముసుగు_2;
    ఒకవేళ (!m)
    {
      *codepoint = INVALID;
      తిరిగి అబద్ధం;
    }
    గుర్తులేని పూర్ణాంకం i = m / ELT_బిట్లు;
    గుర్తులేని పూర్ణాంకం j = m & ELT_ముసుగు;

    స్థిరరాశి elt_t vv = v[i] & ~((elt_t (1) << j) - 1);
    కొరకు (స్థిరరాశి elt_t *p = &vv; i < len (); p = ((స్థిరరాశి elt_t *) &v[0]) + (++i))
      ఒకవేళ (*p)
      {
	*codepoint = i * ELT_బిట్లు + elt_get_min (*p);
	తిరిగి నిజం;
      }

    *codepoint = INVALID;
    తిరిగి అబద్ధం;
  }
  తార్కిక మునుపటి (hb_codepoint_t *codepoint) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం m = (*codepoint - 1) & ముసుగు_2;
    ఒకవేళ (m == ముసుగు_2)
    {
      *codepoint = INVALID;
      తిరిగి అబద్ధం;
    }
    గుర్తులేని పూర్ణాంకం i = m / ELT_బిట్లు;
    గుర్తులేని పూర్ణాంకం j = m & ELT_ముసుగు;

    /* Fancy mask to avoid shifting by elt_t bitsize, which is undefined. */
    స్థిరరాశి elt_t ముసుగు = j < 8 * పరిమాణం (elt_t) - 1 ?
		       ((elt_t (1) << (j + 1)) - 1) :
		       (elt_t) -1;
    స్థిరరాశి elt_t vv = v[i] & ముసుగు;
    స్థిరరాశి elt_t *p = &vv;
    అయితే (నిజం)
    {
      ఒకవేళ (*p)
      {
	*codepoint = i * ELT_బిట్లు + elt_get_max (*p);
	తిరిగి నిజం;
      }
      ఒకవేళ ((పూర్ణాంకం) i <= 0) ఆపు;
      p = &v[--i];
    }

    *codepoint = INVALID;
    తిరిగి అబద్ధం;
  }
  hb_codepoint_t get_min () స్థిరరాశి
  {
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < len (); i++)
      ఒకవేళ (v[i])
	తిరిగి i * ELT_బిట్లు + elt_get_min (v[i]);
    తిరిగి INVALID;
  }
  hb_codepoint_t get_max () స్థిరరాశి
  {
    కొరకు (పూర్ణాంకం i = len () - 1; i >= 0; i--)
      ఒకవేళ (v[i])
	తిరిగి i * ELT_బిట్లు + elt_get_max (v[i]);
    తిరిగి INVALID;
  }

  /*
   * Iterator implementation.
   */
  నిర్మాణం iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    స్థిర constexpr తార్కిక is_sorted_iterator = నిజం;
    iter_t (స్థిరరాశి hb_బిట్టు_పుట_t &s_ = Null (hb_బిట్టు_పుట_t), తార్కిక init = నిజం) : s (&s_), v (INVALID)
    {
      ఒకవేళ (init)
	v = s->get_min ();
    }

    రకనిర్వచనం hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () స్థిరరాశి { తిరిగి v; }
    తార్కిక __more__ () స్థిరరాశి { తిరిగి v != INVALID; }
    శూన్య __next__ () {
       s->next (&v);
    }
    శూన్య __prev__ () { s->మునుపటి (&v); }
    iter_t చివర () స్థిరరాశి { తిరిగి iter_t (*s, అబద్ధం); }
    తార్కిక క్రియాకారకం != (స్థిరరాశి iter_t& o) స్థిరరాశి
    { తిరిగి v != o.v; }

    రక్షిత:
    స్థిరరాశి hb_బిట్టు_పుట_t *s;
    hb_codepoint_t v;
  };
  iter_t iter () స్థిరరాశి { తిరిగి iter_t (*ఇది); }
  క్రియాకారకం iter_t () స్థిరరాశి { తిరిగి iter (); }

  స్థిర constexpr hb_codepoint_t INVALID = HB_సమితి_విలువ_INVALID;

  రకనిర్వచనం గుర్తులేని దీర్ఘ దీర్ఘ elt_t;
  స్థిర constexpr గుర్తులేని పుట_బిట్లు_LOG_2 = 9; // 512 bits
  స్థిర constexpr గుర్తులేని పుట_బిట్లు = 1 << పుట_బిట్లు_LOG_2;
  static_assert (1 << పుట_బిట్లు_LOG_2 == పుట_బిట్లు, "");
  static_assert ((పుట_బిట్లు & ((పుట_బిట్లు) - 1)) == 0, "");
  స్థిర constexpr గుర్తులేని పుట_BITMASK = పుట_బిట్లు - 1;

  స్థిర గుర్తులేని పూర్ణాంకం elt_get_min (స్థిరరాశి elt_t &elt) { తిరిగి hb_ctz (elt); }
  స్థిర గుర్తులేని పూర్ణాంకం elt_get_max (స్థిరరాశి elt_t &elt) { తిరిగి hb_బిట్టు_storage (elt) - 1; }

  రకనిర్వచనం hb_సదిశ_కొలత_t<elt_t, పుట_బిట్లు / 8> సదిశ_t;

  స్థిర constexpr గుర్తులేని ELT_బిట్లు = పరిమాణం (elt_t) * 8;
  స్థిర constexpr గుర్తులేని ELT_ముసుగు = ELT_బిట్లు - 1;

  స్థిర constexpr గుర్తులేని బిట్లు_2 = పరిమాణం (సదిశ_t) * 8;
  స్థిర constexpr గుర్తులేని ముసుగు_2 = బిట్లు_2 - 1;
  static_assert ((గుర్తులేని) పుట_బిట్లు == (గుర్తులేని) బిట్లు_2, "");

  elt_t &elt (hb_codepoint_t g) { తిరిగి v[(g & ముసుగు_2) / ELT_బిట్లు]; }
  స్థిరరాశి elt_t& elt (hb_codepoint_t g) స్థిరరాశి { తిరిగి v[(g & ముసుగు_2) / ELT_బిట్లు]; }
  స్థిర constexpr elt_t ముసుగు (hb_codepoint_t g) { తిరిగి elt_t (1) << (g & ELT_ముసుగు); }

  మార్చదగిన గుర్తులేని population;
  సదిశ_t v;
};


#అయితే_ముగింపు /* HB_BIT_PAGE_HH */
