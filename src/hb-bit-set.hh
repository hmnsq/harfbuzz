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

#నిర్వచితం_కాకపోతే HB_బిట్టు_సమితి_HH
#నిర్వచించు HB_బిట్టు_సమితి_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-bit-page.hh"

నిర్మాణం hb_బిట్టు_సమితి_t
{
  hb_బిట్టు_సమితి_t () = అప్రమేయ;
  ~hb_బిట్టు_సమితి_t () = అప్రమేయ;

  hb_బిట్టు_సమితి_t (స్థిరరాశి hb_బిట్టు_సమితి_t& other) : hb_బిట్టు_సమితి_t () { సమితి (other, నిజం); }
  hb_బిట్టు_సమితి_t ( hb_బిట్టు_సమితి_t&& other)  noexcept : hb_బిట్టు_సమితి_t () { hb_మార్పిడి (*ఇది, other); }
  hb_బిట్టు_సమితి_t& క్రియాకారకం= (స్థిరరాశి hb_బిట్టు_సమితి_t& other) { సమితి (other); తిరిగి *ఇది; }
  hb_బిట్టు_సమితి_t& క్రియాకారకం= (hb_బిట్టు_సమితి_t&& other)  noexcept { hb_మార్పిడి (*ఇది, other); తిరిగి *ఇది; }
  మిత్ర శూన్య swap (hb_బిట్టు_సమితి_t &a, hb_బిట్టు_సమితి_t &b) noexcept
  {
    ఒకవేళ (likely (!a.successful || !b.successful))
      తిరిగి;
    hb_మార్పిడి (a.population, b.population);
    hb_మార్పిడి (a.చివరి_పుట_lookup, b.చివరి_పుట_lookup);
    hb_మార్పిడి (a.పుట_map, b.పుట_map);
    hb_మార్పిడి (a.పుటలు, b.పుటలు);
  }

  శూన్య init ()
  {
    successful = నిజం;
    population = 0;
    చివరి_పుట_lookup = 0;
    పుట_map.init ();
    పుటలు.init ();
  }
  శూన్య fini ()
  {
    పుట_map.fini ();
    పుటలు.fini ();
  }

  ఉపయోగించి పుట_t = hb_బిట్టు_పుట_t;
  నిర్మాణం పుట_map_t
  {
    పూర్ణాంకం cmp (స్థిరరాశి పుట_map_t &o) స్థిరరాశి { తిరిగి cmp (o.major); }
    పూర్ణాంకం cmp (uint32_t o_major) స్థిరరాశి { తిరిగి (పూర్ణాంకం) o_major - (పూర్ణాంకం) major; }

    uint32_t major;
    uint32_t సూచీ_2;
  };

  తార్కిక successful = నిజం; /* Allocations successful */
  మార్చదగిన గుర్తులేని పూర్ణాంకం population = 0;
  మార్చదగిన hb_atomic_t<గుర్తులేని> చివరి_పుట_lookup = 0;
  hb_sorted_సదిశ_t<పుట_map_t> పుట_map;
  hb_సదిశ_t<పుట_t> పుటలు;

  శూన్య err () { ఒకవేళ (successful) successful = అబద్ధం; } /* TODO Remove */
  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి !successful; }

  తార్కిక resize (గుర్తులేని పూర్ణాంకం లెక్క, తార్కిక clear = నిజం, తార్కిక exact_కొలత = అబద్ధం)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి అబద్ధం;

    ఒకవేళ (పుటలు.నిడివి < లెక్క && (గుర్తులేని) పుటలు.allocated < లెక్క && లెక్క <= 2)
      exact_కొలత = నిజం; // Most sets are small and local

    ఒకవేళ (unlikely (!పుటలు.resize_నిండిన (లెక్క, clear, exact_కొలత) ||
	!పుట_map.resize_నిండిన (లెక్క, clear, అబద్ధం)))
    {
      పుటలు.resize_నిండిన (పుట_map.నిడివి, clear, exact_కొలత);
      successful = అబద్ధం;
      తిరిగి అబద్ధం;
    }
    తిరిగి నిజం;
  }

  శూన్య alloc (గుర్తులేని sz)
  {
    sz >>= (పుట_t::పుట_బిట్లు_LOG_2 - 1);
    పుటలు.alloc (sz);
    పుట_map.alloc (sz);
  }

  hb_బిట్టు_సమితి_t& reset ()
  {
    successful = నిజం;
    clear ();
    తిరిగి *ఇది;
  }

  శూన్య clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ఒకవేళ (!పుటలు.నిడివి && !population) తిరిగి;
    పుటలు.clear ();
    పుట_map.clear ();
    population = 0;
  }
  తార్కిక is_empty () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం లెక్క = పుటలు.నిడివి;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      ఒకవేళ (!పుటలు[i].is_empty ())
	తిరిగి అబద్ధం;
    తిరిగి నిజం;
  }
  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి !is_empty (); }

  uint32_t hash () స్థిరరాశి
  {
    uint32_t h = 0;
    కొరకు (స్వయంచాలక &map : పుట_map)
    {
      స్వయంచాలక &పుట = పుటలు.arrayZ[map.సూచీ_2];
      ఒకవేళ (unlikely (పుట.is_empty ())) కొనసాగు;
      h = h * 31 + hb_hash (map.major) + hb_hash (పుట);
    }
    తిరిగి h;
  }

  అంతరంగ:
  శూన్య dirty () { population = UINT_MAX; }
  బహిరంగ:

  శూన్య కూడు (hb_codepoint_t g)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి;
    ఒకవేళ (unlikely (g == INVALID)) తిరిగి;
    dirty ();
    పుట_t *పుట = పుట_for (g, నిజం); ఒకవేళ (unlikely (!పుట)) తిరిగి;
    పుట->కూడు (g);
  }
  శూన్య కూడు_బిట్లు (hb_codepoint_t g, uint64_t బిట్లు)
  {
    ఒకవేళ (unlikely (!successful) || unlikely (!బిట్లు)) తిరిగి;
    ప్రతిజ్ఞా (!(g & (పుట_t::ELT_బిట్లు - 1)));
    dirty ();
    పుట_t *పుట = పుట_for (g, నిజం); ఒకవేళ (unlikely (!పుట)) తిరిగి;
    పుట->కూడు_బిట్లు (g, బిట్లు);
  }
  తార్కిక కూడు_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి నిజం; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ఒకవేళ (unlikely (a > b || a == INVALID || b == INVALID)) తిరిగి అబద్ధం;
    dirty ();
    గుర్తులేని పూర్ణాంకం ma = get_major (a);
    గుర్తులేని పూర్ణాంకం mb = get_major (b);
    ఒకవేళ (ma == mb)
    {
      పుట_t *పుట = పుట_for (a, నిజం); ఒకవేళ (unlikely (!పుట)) తిరిగి అబద్ధం;
      పుట->కూడు_range (a, b);
    }
    లేకపోతే
    {
      పుట_t *పుట = పుట_for (a, నిజం); ఒకవేళ (unlikely (!పుట)) తిరిగి అబద్ధం;
      పుట->కూడు_range (a, major_ప్రారంభం (ma + 1) - 1);

      కొరకు (గుర్తులేని పూర్ణాంకం m = ma + 1; m < mb; m++)
      {
	పుట = పుట_for (major_ప్రారంభం (m), నిజం); ఒకవేళ (unlikely (!పుట)) తిరిగి అబద్ధం;
	పుట->init1 ();
      }

      పుట = పుట_for (b, నిజం); ఒకవేళ (unlikely (!పుట)) తిరిగి అబద్ధం;
      పుట->కూడు_range (major_ప్రారంభం (mb), b);
    }
    తిరిగి నిజం;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  మూస<రకం_పేరు Type>
  స్థిర అంతఃవరుస స్థిరరాశి Type& Struct_At_స్థానభ్రంశం_Unaligned(స్థిరరాశి శూన్య *P, గుర్తులేని పూర్ణాంకం స్థానభ్రంశం)
  {
#సూచన GCC diagnostic push
#సూచన GCC diagnostic ignored "-Wcast-align"
    తిరిగి * reinterpret_cast<స్థిరరాశి Type*> ((స్థిరరాశి అక్షరం *) P + స్థానభ్రంశం);
#సూచన GCC diagnostic తీయి
  }

  మూస <రకం_పేరు T>
  శూన్య సమితి_array (తార్కిక v, స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి;
    ఒకవేళ (!లెక్క) తిరిగి;
    dirty ();
    hb_codepoint_t g = *array;
    అయితే (లెక్క)
    {
      గుర్తులేని పూర్ణాంకం m = get_major (g);
      పుట_t *పుట = పుట_for (g, v); ఒకవేళ (unlikely (v && !పుట)) తిరిగి;
      గుర్తులేని పూర్ణాంకం ప్రారంభం = major_ప్రారంభం (m);
      గుర్తులేని పూర్ణాంకం చివర = major_ప్రారంభం (m + 1);
      చేయి
      {
        ఒకవేళ (g != INVALID && (v || పుట)) /* The v check is to optimize out the page check if v is true. */
	  పుట->సమితి (g, v);

	array = &Struct_At_స్థానభ్రంశం_Unaligned<T> (array, stride);
	లెక్క--;
      }
      అయితే (లెక్క && (g = *array, ప్రారంభం <= g && g < చివర));
    }
  }

  మూస <రకం_పేరు T>
  శూన్య కూడు_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  { సమితి_array (నిజం, array, లెక్క, stride); }
  మూస <రకం_పేరు T>
  శూన్య కూడు_array (స్థిరరాశి hb_array_t<స్థిరరాశి T>& arr) { కూడు_array (&arr, arr.len ()); }

  మూస <రకం_పేరు T>
  శూన్య del_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  { సమితి_array (అబద్ధం, array, లెక్క, stride); }
  మూస <రకం_పేరు T>
  శూన్య del_array (స్థిరరాశి hb_array_t<స్థిరరాశి T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  మూస <రకం_పేరు T>
  తార్కిక సమితి_sorted_array (తార్కిక v, స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి నిజం; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ఒకవేళ (unlikely (!లెక్క)) తిరిగి నిజం;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t చివరి_g = g;
    అయితే (లెక్క)
    {
      గుర్తులేని పూర్ణాంకం m = get_major (g);
      పుట_t *పుట = పుట_for (g, v); ఒకవేళ (unlikely (v && !పుట)) తిరిగి అబద్ధం;
      గుర్తులేని పూర్ణాంకం చివర = major_ప్రారంభం (m + 1);
      చేయి
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ఒకవేళ (g < చివరి_g) తిరిగి అబద్ధం;
	చివరి_g = g;

        ఒకవేళ (g != INVALID && (v || పుట)) /* The v check is to optimize out the page check if v is true. */
	  పుట->సమితి (g, v);

	array = &Struct_At_స్థానభ్రంశం_Unaligned<T> (array, stride);
	లెక్క--;
      }
      అయితే (లెక్క && (g = *array, g < చివర));
    }
    తిరిగి నిజం;
  }

  మూస <రకం_పేరు T>
  తార్కిక కూడు_sorted_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  { తిరిగి సమితి_sorted_array (నిజం, array, లెక్క, stride); }
  మూస <రకం_పేరు T>
  తార్కిక కూడు_sorted_array (స్థిరరాశి hb_sorted_array_t<స్థిరరాశి T>& arr) { తిరిగి కూడు_sorted_array (&arr, arr.len ()); }

  మూస <రకం_పేరు T>
  తార్కిక del_sorted_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  { తిరిగి సమితి_sorted_array (అబద్ధం, array, లెక్క, stride); }
  మూస <రకం_పేరు T>
  తార్కిక del_sorted_array (స్థిరరాశి hb_sorted_array_t<స్థిరరాశి T>& arr) { తిరిగి del_sorted_array (&arr, arr.len ()); }

  శూన్య del (hb_codepoint_t g)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి;
    పుట_t *పుట = పుట_for (g);
    ఒకవేళ (!పుట)
      తిరిగి;
    dirty ();
    పుట->del (g);
  }
  శూన్య del_బిట్లు (hb_codepoint_t g, uint64_t బిట్లు)
  {
    ఒకవేళ (unlikely (!successful) || unlikely (!బిట్లు)) తిరిగి;
    ప్రతిజ్ఞా (!(g & (పుట_t::ELT_బిట్లు - 1)));
    పుట_t *పుట = పుట_for (g);
    ఒకవేళ (!పుట)
      తిరిగి;
    dirty ();
    పుట->del_బిట్లు (g, బిట్లు);
  }

  అంతరంగ:
  శూన్య del_పుటలు (పూర్ణాంకం ds, పూర్ణాంకం de)
  {
    ఒకవేళ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_సదిశ_t<గుర్తులేని> compact_workspace;
      ఒకవేళ (unlikely (!allocate_compact_workspace (compact_workspace))) తిరిగి;

      గుర్తులేని పూర్ణాంకం write_సూచీ = 0;
      కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < పుట_map.నిడివి; i++)
      {
	పూర్ణాంకం m = (పూర్ణాంకం) పుట_map.arrayZ[i].major;
	ఒకవేళ (m < ds || de < m)
	  పుట_map.arrayZ[write_సూచీ++] = పుట_map.arrayZ[i];
      }
      compact (compact_workspace, write_సూచీ);
      resize (write_సూచీ);
    }
  }


  బహిరంగ:
  శూన్య del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి;
    ఒకవేళ (unlikely (a > b || a == INVALID)) తిరిగి;
    dirty ();
    గుర్తులేని పూర్ణాంకం ma = get_major (a);
    గుర్తులేని పూర్ణాంకం mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    పూర్ణాంకం ds = (a == major_ప్రారంభం (ma))? (పూర్ణాంకం) ma: (పూర్ణాంకం) (ma + 1);
    పూర్ణాంకం de = (b + 1 == major_ప్రారంభం (mb + 1))? (పూర్ణాంకం) mb: ((పూర్ణాంకం) mb - 1);
    ఒకవేళ (ds > de || (పూర్ణాంకం) ma < ds)
    {
      పుట_t *పుట = పుట_for (a);
      ఒకవేళ (పుట)
      {
	ఒకవేళ (ma == mb)
	  పుట->del_range (a, b);
	లేకపోతే
	  పుట->del_range (a, major_ప్రారంభం (ma + 1) - 1);
      }
    }
    ఒకవేళ (de < (పూర్ణాంకం) mb && ma != mb)
    {
      పుట_t *పుట = పుట_for (b);
      ఒకవేళ (పుట)
	పుట->del_range (major_ప్రారంభం (mb), b);
    }
    del_పుటలు (ds, de);
  }

  తార్కిక get (hb_codepoint_t g) స్థిరరాశి
  {
    స్థిరరాశి పుట_t *పుట = పుట_for (g);
    ఒకవేళ (!పుట)
      తిరిగి అబద్ధం;
    తిరిగి పుట->get (g);
  }
  తార్కిక may_have (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }

  /* Has interface. */
  తార్కిక క్రియాకారకం [] (hb_codepoint_t k) స్థిరరాశి { తిరిగి get (k); }
  తార్కిక has (hb_codepoint_t k) స్థిరరాశి { తిరిగి (*ఇది)[k]; }
  /* Predicate. */
  తార్కిక క్రియాకారకం () (hb_codepoint_t k) స్థిరరాశి { తిరిగి has (k); }

  /* Sink interface. */
  hb_బిట్టు_సమితి_t& క్రియాకారకం << (hb_codepoint_t v)
  { కూడు (v); తిరిగి *ఇది; }
  hb_బిట్టు_సమితి_t& క్రియాకారకం << (స్థిరరాశి hb_codepoint_pair_t& range)
  { కూడు_range (range.మొదటి, range.second); తిరిగి *ఇది; }

  తార్కిక intersects (స్థిరరాశి hb_బిట్టు_సమితి_t &other) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం na = పుటలు.నిడివి;
    గుర్తులేని పూర్ణాంకం nb = other.పుటలు.నిడివి;

    గుర్తులేని పూర్ణాంకం a = 0, b = 0;
    కొరకు (; a < na && b < nb; )
    {
      ఒకవేళ (పుట_map.arrayZ[a].major == other.పుట_map.arrayZ[b].major)
      {
	ఒకవేళ (పుట_at (a).intersects (other.పుట_at (b)))
	  తిరిగి నిజం;
	a++;
	b++;
      }
      లేకపోతే ఒకవేళ (పుట_map.arrayZ[a].major < other.పుట_map.arrayZ[b].major)
	a++;
      లేకపోతే
	b++;
    }
    తిరిగి అబద్ధం;
  }
  తార్కిక may_intersect (స్థిరరాశి hb_బిట్టు_సమితి_t &other) స్థిరరాశి
  { తిరిగి intersects (other); }

  తార్కిక intersects (hb_codepoint_t మొదటి, hb_codepoint_t చివరి) స్థిరరాశి
  {
    hb_codepoint_t c = మొదటి - 1;
    తిరిగి next (&c) && c <= చివరి;
  }
  శూన్య సమితి (స్థిరరాశి hb_బిట్టు_సమితి_t &other, తార్కిక exact_కొలత = అబద్ధం)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి;
    గుర్తులేని పూర్ణాంకం లెక్క = other.పుటలు.నిడివి;
    ఒకవేళ (unlikely (!resize  (లెక్క, అబద్ధం, exact_కొలత)))
      తిరిగి;
    population = other.population;

    పుట_map = other.పుట_map;
    పుటలు = other.పుటలు;
  }

  తార్కిక is_equal (స్థిరరాశి hb_బిట్టు_సమితి_t &other) స్థిరరాశి
  {
    ఒకవేళ (has_population () && other.has_population () &&
	population != other.population)
      తిరిగి అబద్ధం;

    గుర్తులేని పూర్ణాంకం na = పుటలు.నిడివి;
    గుర్తులేని పూర్ణాంకం nb = other.పుటలు.నిడివి;

    గుర్తులేని పూర్ణాంకం a = 0, b = 0;
    కొరకు (; a < na && b < nb; )
    {
      ఒకవేళ (పుట_at (a).is_empty ()) { a++; కొనసాగు; }
      ఒకవేళ (other.పుట_at (b).is_empty ()) { b++; కొనసాగు; }
      ఒకవేళ (పుట_map.arrayZ[a].major != other.పుట_map.arrayZ[b].major ||
	  !పుట_at (a).is_equal (other.పుట_at (b)))
	తిరిగి అబద్ధం;
      a++;
      b++;
    }
    కొరకు (; a < na; a++)
      ఒకవేళ (!పుట_at (a).is_empty ()) { తిరిగి అబద్ధం; }
    కొరకు (; b < nb; b++)
      ఒకవేళ (!other.పుట_at (b).is_empty ()) { తిరిగి అబద్ధం; }

    తిరిగి నిజం;
  }

  తార్కిక is_subset (స్థిరరాశి hb_బిట్టు_సమితి_t &larger_సమితి) స్థిరరాశి
  {
    ఒకవేళ (has_population () && larger_సమితి.has_population () &&
	population > larger_సమితి.population)
      తిరిగి అబద్ధం;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    అయితే (spi < పుట_map.నిడివి && lpi < larger_సమితి.పుట_map.నిడివి)
    {
      uint32_t spm = పుట_map.arrayZ[spi].major;
      uint32_t lpm = larger_సమితి.పుట_map.arrayZ[lpi].major;
      స్వయంచాలక sp = పుట_at (spi);

      ఒకవేళ (spm < lpm) {
        ఒకవేళ (!sp.is_empty ())
          తిరిగి అబద్ధం;
        spi++;
        కొనసాగు;
      }

      ఒకవేళ (lpm < spm) {
        lpi++;
        కొనసాగు;
      }

      స్వయంచాలక lp = larger_సమితి.పుట_at (lpi);
      ఒకవేళ (!sp.is_subset (lp))
        తిరిగి అబద్ధం;

      spi++;
      lpi++;
    }

    అయితే (spi < పుట_map.నిడివి)
      ఒకవేళ (!పుట_at (spi++).is_empty ())
        తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  అంతరంగ:
  తార్కిక allocate_compact_workspace (hb_సదిశ_t<గుర్తులేని>& workspace)
  {
    ఒకవేళ (unlikely (!workspace.resize_exact (పుటలు.నిడివి)))
    {
      successful = అబద్ధం;
      తిరిగి అబద్ధం;
    }

    తిరిగి నిజం;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  శూన్య compact (hb_సదిశ_t<గుర్తులేని>& workspace,
                గుర్తులేని పూర్ణాంకం నిడివి)
  {
    ప్రతిజ్ఞా(workspace.నిడివి == పుటలు.నిడివి);
    hb_సదిశ_t<గుర్తులేని>& old_సూచీ_గమ్యం_పుట_map_సూచీ = workspace;

    hb_fill (old_సూచీ_గమ్యం_పుట_map_సూచీ.writer(), 0xFFFFFFFF);
    కొరకు (గుర్తులేని i = 0; i < నిడివి; i++)
      old_సూచీ_గమ్యం_పుట_map_సూచీ[పుట_map[i].సూచీ_2] =  i;

    compact_పుటలు (old_సూచీ_గమ్యం_పుట_map_సూచీ);
  }
  శూన్య compact_పుటలు (స్థిరరాశి hb_సదిశ_t<గుర్తులేని>& old_సూచీ_గమ్యం_పుట_map_సూచీ)
  {
    గుర్తులేని పూర్ణాంకం write_సూచీ = 0;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < పుటలు.నిడివి; i++)
    {
      ఒకవేళ (old_సూచీ_గమ్యం_పుట_map_సూచీ[i] == 0xFFFFFFFF) కొనసాగు;

      ఒకవేళ (write_సూచీ < i)
	పుటలు[write_సూచీ] = పుటలు[i];

      పుట_map[old_సూచీ_గమ్యం_పుట_map_సూచీ[i]].సూచీ_2 = write_సూచీ;
      write_సూచీ++;
    }
  }
  బహిరంగ:

  శూన్య process_ (hb_బిట్టు_పుట_t::సదిశ_t (*op) (స్థిరరాశి hb_బిట్టు_పుట_t::సదిశ_t &, స్థిరరాశి hb_బిట్టు_పుట_t::సదిశ_t &),
		 తార్కిక passthru_ఎడమ, తార్కిక passthru_కుడి,
		 స్థిరరాశి hb_బిట్టు_సమితి_t &other,
		 hb_సదిశ_t<గుర్తులేని> *workspace = శూన్యసూచిక)
  {
    ఒకవేళ (unlikely (!successful)) తిరిగి;

    dirty ();

    గుర్తులేని పూర్ణాంకం na = పుటలు.నిడివి;
    గుర్తులేని పూర్ణాంకం nb = other.పుటలు.నిడివి;
    గుర్తులేని పూర్ణాంకం తదుపరి_పుట = na;

    గుర్తులేని పూర్ణాంకం లెక్క = 0, new_లెక్క = 0;
    గుర్తులేని పూర్ణాంకం a = 0, b = 0;
    గుర్తులేని పూర్ణాంకం write_సూచీ = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_సదిశ_t<గుర్తులేని> local_workspace;
    hb_సదిశ_t<గుర్తులేని> &compact_workspace = workspace ? *workspace : local_workspace;
    ఒకవేళ (!passthru_ఎడమ)
    {
      తార్కిక allocated = workspace ? compact_workspace.resize (పుటలు.నిడివి)
				 : allocate_compact_workspace (compact_workspace);
      ఒకవేళ (unlikely (!allocated))
      {
	successful = అబద్ధం;
	తిరిగి;
      }
    }

    కొరకు (; a < na && b < nb; )
    {
      ఒకవేళ (పుట_map.arrayZ[a].major == other.పుట_map.arrayZ[b].major)
      {
	ఒకవేళ (!passthru_ఎడమ)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ఒకవేళ (write_సూచీ < a)
	    పుట_map.arrayZ[write_సూచీ] = పుట_map.arrayZ[a];
	  write_సూచీ++;
	}

	లెక్క++;
	a++;
	b++;
      }
      లేకపోతే ఒకవేళ (పుట_map.arrayZ[a].major < other.పుట_map.arrayZ[b].major)
      {
	ఒకవేళ (passthru_ఎడమ)
	  లెక్క++;
	a++;
      }
      లేకపోతే
      {
	ఒకవేళ (passthru_కుడి)
	  లెక్క++;
	b++;
      }
    }
    ఒకవేళ (passthru_ఎడమ)
      లెక్క += na - a;
    ఒకవేళ (passthru_కుడి)
      లెక్క += nb - b;

    ఒకవేళ (!passthru_ఎడమ)
    {
      na  = write_సూచీ;
      తదుపరి_పుట = write_సూచీ;
      compact (compact_workspace, write_సూచీ);
    }

    ఒకవేళ (unlikely (!resize (లెక్క)))
      తిరిగి;

    new_లెక్క = లెక్క;

    /* Process in-place backward. */
    a = na;
    b = nb;
    కొరకు (; a && b; )
    {
      ఒకవేళ (పుట_map.arrayZ[a - 1].major == other.పుట_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	లెక్క--;
	పుట_map.arrayZ[లెక్క] = పుట_map.arrayZ[a];
	పుట_at (లెక్క).v = op (పుట_at (a).v, other.పుట_at (b).v);
	పుట_at (లెక్క).dirty ();
      }
      లేకపోతే ఒకవేళ (పుట_map.arrayZ[a - 1].major > other.పుట_map.arrayZ[b - 1].major)
      {
	a--;
	ఒకవేళ (passthru_ఎడమ)
	{
	  లెక్క--;
	  పుట_map.arrayZ[లెక్క] = పుట_map.arrayZ[a];
	}
      }
      లేకపోతే
      {
	b--;
	ఒకవేళ (passthru_కుడి)
	{
	  లెక్క--;
	  పుట_map.arrayZ[లెక్క].major = other.పుట_map.arrayZ[b].major;
	  పుట_map.arrayZ[లెక్క].సూచీ_2 = తదుపరి_పుట++;
	  పుట_at (లెక్క) = other.పుట_at (b);
	}
      }
    }
    ఒకవేళ (passthru_ఎడమ)
      అయితే (a)
      {
	a--;
	లెక్క--;
	పుట_map.arrayZ[లెక్క] = పుట_map.arrayZ[a];
      }
    ఒకవేళ (passthru_కుడి)
      అయితే (b)
      {
	b--;
	లెక్క--;
	పుట_map.arrayZ[లెక్క].major = other.పుట_map.arrayZ[b].major;
	పుట_map.arrayZ[లెక్క].సూచీ_2 = తదుపరి_పుట++;
	పుట_at (లెక్క) = other.పుట_at (b);
      }
    ప్రతిజ్ఞా (!లెక్క);
    resize (new_లెక్క);
  }
  మూస <రకం_పేరు Op>
  స్థిర hb_బిట్టు_పుట_t::సదిశ_t
  op_ (స్థిరరాశి hb_బిట్టు_పుట_t::సదిశ_t &a, స్థిరరాశి hb_బిట్టు_పుట_t::సదిశ_t &b)
  { తిరిగి Op{} (a, b); }
  మూస <రకం_పేరు Op>
  శూన్య process (స్థిరరాశి Op& op,
		స్థిరరాశి hb_బిట్టు_సమితి_t &other,
		hb_సదిశ_t<గుర్తులేని> *workspace = శూన్యసూచిక)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  శూన్య union_ (స్థిరరాశి hb_బిట్టు_సమితి_t &other) { process (hb_bitwise_or, other); }
  శూన్య intersect (స్థిరరాశి hb_బిట్టు_సమితి_t &other,
		  hb_సదిశ_t<గుర్తులేని> *workspace = శూన్యసూచిక)
  { process (hb_bitwise_and, other, workspace); }
  శూన్య వ్యవకలనం (స్థిరరాశి hb_బిట్టు_సమితి_t &other) { process (hb_bitwise_gt, other); }
  శూన్య సౌష్ఠవ_difference (స్థిరరాశి hb_బిట్టు_సమితి_t &other) { process (hb_bitwise_xor, other); }

  తార్కిక next (hb_codepoint_t *codepoint) స్థిరరాశి
  {
    ఒకవేళ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      తిరిగి *codepoint != INVALID;
    }

    స్థిరరాశి స్వయంచాలక* పుట_map_array = పుట_map.arrayZ;
    గుర్తులేని పూర్ణాంకం major = get_major (*codepoint);
    గుర్తులేని పూర్ణాంకం i = చివరి_పుట_lookup;

    ఒకవేళ (unlikely (i >= పుట_map.నిడివి || పుట_map_array[i].major != major))
    {
      పుట_map.bfind (major, &i, HB_NOT_దొరికింది_STORE_CLOSEST);
      ఒకవేళ (i >= పుట_map.నిడివి) {
        *codepoint = INVALID;
        తిరిగి అబద్ధం;
      }
      చివరి_పుట_lookup = i;
    }

    స్థిరరాశి స్వయంచాలక* పుటలు_array = పుటలు.arrayZ;
    స్థిరరాశి పుట_map_t &current = పుట_map_array[i];
    ఒకవేళ (likely (current.major == major))
    {
      ఒకవేళ (పుటలు_array[current.సూచీ_2].next (codepoint))
      {
        *codepoint += current.major * పుట_t::పుట_బిట్లు;
        తిరిగి నిజం;
      }
      i++;
    }

    కొరకు (; i < పుట_map.నిడివి; i++)
    {
      స్థిరరాశి పుట_map_t &current = పుట_map_array[i];
      hb_codepoint_t m = పుటలు_array[current.సూచీ_2].get_min ();
      ఒకవేళ (m != INVALID)
      {
	*codepoint = current.major * పుట_t::పుట_బిట్లు + m;
        చివరి_పుట_lookup = i;
	తిరిగి నిజం;
      }
    }
    *codepoint = INVALID;
    తిరిగి అబద్ధం;
  }
  తార్కిక తదుపరి_బిట్లు (hb_codepoint_t *codepoint, uint64_t *బిట్లు) స్థిరరాశి
  {
    static_assert (పుట_t::ELT_బిట్లు == 64, "");

    గుర్తులేని పూర్ణాంకం i = 0;
    గుర్తులేని పూర్ణాంకం elt = 0;
    ఒకవేళ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t పునాది = *codepoint & ~పుట_t::ELT_ముసుగు;
      ఒకవేళ (unlikely (పునాది >= INVALID - పుట_t::ELT_బిట్లు + 1))
        వెళ్ళు పూర్తి;
      పునాది += పుట_t::ELT_బిట్లు;

      గుర్తులేని పూర్ణాంకం major = get_major (పునాది);
      i = చివరి_పుట_lookup;
      ఒకవేళ (unlikely (i >= పుట_map.నిడివి || పుట_map.arrayZ[i].major != major))
      {
	పుట_map.bfind (major, &i, HB_NOT_దొరికింది_STORE_CLOSEST);
	ఒకవేళ (i >= పుట_map.నిడివి)
	  వెళ్ళు పూర్తి;
      }
      ఒకవేళ (పుట_map.arrayZ[i].major == major)
	elt = పుట_remainder (పునాది) / పుట_t::ELT_బిట్లు;
    }

    కొరకు (; i < పుట_map.నిడివి; i++, elt = 0)
    {
      స్థిరరాశి పుట_map_t &map = పుట_map.arrayZ[i];
      స్థిరరాశి పుట_t &పుట = పుటలు.arrayZ[map.సూచీ_2];
      కొరకు (; elt < పుట_t::len (); elt++)
	ఒకవేళ (పుట.v[elt])
	{
	  *codepoint = major_ప్రారంభం (map.major) + elt * పుట_t::ELT_బిట్లు;
	  *బిట్లు = పుట.v[elt];
	  చివరి_పుట_lookup = i;
	  తిరిగి నిజం;
	}
    }

  పూర్తి:
    *codepoint = INVALID;
    *బిట్లు = 0;
    తిరిగి అబద్ధం;
  }
  తార్కిక మునుపటి (hb_codepoint_t *codepoint) స్థిరరాశి
  {
    ఒకవేళ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      తిరిగి *codepoint != INVALID;
    }

    పుట_map_t map = {get_major (*codepoint), 0};
    గుర్తులేని పూర్ణాంకం i;
    పుట_map.bfind (map, &i, HB_NOT_దొరికింది_STORE_CLOSEST);
    ఒకవేళ (i < పుట_map.నిడివి && పుట_map.arrayZ[i].major == map.major)
    {
      ఒకవేళ (పుటలు[పుట_map.arrayZ[i].సూచీ_2].మునుపటి (codepoint))
      {
	*codepoint += పుట_map.arrayZ[i].major * పుట_t::పుట_బిట్లు;
	తిరిగి నిజం;
      }
    }
    i--;
    కొరకు (; (పూర్ణాంకం) i >= 0; i--)
    {
      hb_codepoint_t m = పుటలు.arrayZ[పుట_map.arrayZ[i].సూచీ_2].get_max ();
      ఒకవేళ (m != INVALID)
      {
	*codepoint = పుట_map.arrayZ[i].major * పుట_t::పుట_బిట్లు + m;
	తిరిగి నిజం;
      }
    }
    *codepoint = INVALID;
    తిరిగి అబద్ధం;
  }
  తార్కిక తదుపరి_range (hb_codepoint_t *మొదటి, hb_codepoint_t *చివరి) స్థిరరాశి
  {
    hb_codepoint_t i;

    i = *చివరి;
    ఒకవేళ (!next (&i))
    {
      *చివరి = *మొదటి = INVALID;
      తిరిగి అబద్ధం;
    }

    *చివరి = *మొదటి = i;

    స్థిరరాశి స్వయంచాలక* పుట_map_array = పుట_map.arrayZ;
    స్థిరరాశి స్వయంచాలక* పుటలు_array = పుటలు.arrayZ;
    గుర్తులేని పూర్ణాంకం major = get_major (*చివరి);
    గుర్తులేని పూర్ణాంకం p_సూచీ = చివరి_పుట_lookup;

    ఒకవేళ (unlikely (p_సూచీ >= పుట_map.నిడివి || పుట_map_array[p_సూచీ].major != major))
    {
      పుట_map.bfind (major, &p_సూచీ, HB_NOT_దొరికింది_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      ప్రతిజ్ఞా(p_సూచీ < పుట_map.నిడివి && పుట_map_array[p_సూచీ].major == major);
      చివరి_పుట_lookup = p_సూచీ;
    }

    గుర్తులేని పూర్ణాంకం rem = పుట_remainder (*చివరి);
    గుర్తులేని పూర్ణాంకం elt = rem / పుట_t::ELT_బిట్లు;
    గుర్తులేని పూర్ణాంకం bit = rem & పుట_t::ELT_ముసుగు;

    స్థిరరాశి పుట_t *పుట = &పుటలు_array[పుట_map_array[p_సూచీ].సూచీ_2];
    గుర్తులేని పూర్ణాంకం నిరంతరం = hb_ctz (~(పుట->v[elt] >> bit));
    *చివరి += నిరంతరం - 1;
    ఒకవేళ (నిరంతరం < పుట_t::ELT_బిట్లు - bit)
      తిరిగి నిజం;

    elt++;

    అయితే (నిజం)
    {
      కొరకు (; elt < పుట_t::len (); elt++)
      {
        uint64_t w = పుట->v[elt];
        ఒకవేళ (w == (uint64_t) -1)
        {
          *చివరి += పుట_t::ELT_బిట్లు;
          కొనసాగు;
        }
        uint64_t inv = ~w;
        *చివరి += hb_ctz (inv);
        తిరిగి నిజం;
      }

      p_సూచీ++;


      ఒకవేళ (p_సూచీ >= పుట_map.నిడివి || పుట_map_array[p_సూచీ].major != major + 1)
        తిరిగి నిజం;

      స్థిరరాశి స్వయంచాలక& నమోదు_2 = పుట_map_array[p_సూచీ];
      major = నమోదు_2.major;
      చివరి_పుట_lookup = p_సూచీ;
      పుట = &పుటలు_array[నమోదు_2.సూచీ_2];
      elt = 0;
    }
  }

  తార్కిక మునుపటి_range (hb_codepoint_t *మొదటి, hb_codepoint_t *చివరి) స్థిరరాశి
  {
    hb_codepoint_t i;

    i = *మొదటి;
    ఒకవేళ (!మునుపటి (&i))
    {
      *చివరి = *మొదటి = INVALID;
      తిరిగి అబద్ధం;
    }

    *చివరి = *మొదటి = i;

    స్థిరరాశి స్వయంచాలక* పుట_map_array = పుట_map.arrayZ;
    స్థిరరాశి స్వయంచాలక* పుటలు_array = పుటలు.arrayZ;
    గుర్తులేని పూర్ణాంకం major = get_major (*మొదటి);
    గుర్తులేని పూర్ణాంకం p_సూచీ = చివరి_పుట_lookup;

    ఒకవేళ (unlikely (p_సూచీ >= పుట_map.నిడివి || పుట_map_array[p_సూచీ].major != major))
    {
      పుట_map.bfind (major, &p_సూచీ, HB_NOT_దొరికింది_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      ప్రతిజ్ఞా(p_సూచీ < పుట_map.నిడివి && పుట_map_array[p_సూచీ].major == major);
      చివరి_పుట_lookup = p_సూచీ;
    }

    గుర్తులేని పూర్ణాంకం rem = పుట_remainder (*మొదటి);
    పూర్ణాంకం elt = rem / పుట_t::ELT_బిట్లు;
    గుర్తులేని పూర్ణాంకం bit = rem & పుట_t::ELT_ముసుగు;

    స్థిరరాశి పుట_t *పుట = &పుటలు_array[పుట_map_array[p_సూచీ].సూచీ_2];
    uint64_t inv = ~(పుట->v[elt] << (63 - bit));
    గుర్తులేని పూర్ణాంకం నిరంతరం = hb_బిట్టు_storage (inv) ? (పుట_t::ELT_బిట్లు - hb_బిట్టు_storage (inv)) : పుట_t::ELT_బిట్లు;
    *మొదటి -= నిరంతరం - 1;
    ఒకవేళ (నిరంతరం < bit + 1)
      తిరిగి నిజం;

    elt--;

    అయితే (నిజం)
    {
      కొరకు (; elt >= 0; elt--)
      {
        uint64_t w = పుట->v[elt];
        ఒకవేళ (w == (uint64_t) -1)
        {
          *మొదటి -= పుట_t::ELT_బిట్లు;
          కొనసాగు;
        }
        uint64_t inv = ~w;
        గుర్తులేని పూర్ణాంకం zeros = hb_బిట్టు_storage (inv) ? (పుట_t::ELT_బిట్లు - hb_బిట్టు_storage (inv)) : పుట_t::ELT_బిట్లు;
        *మొదటి -= zeros;
        తిరిగి నిజం;
      }


      ఒకవేళ (p_సూచీ == 0 || పుట_map_array[p_సూచీ - 1].major != major - 1)
        తిరిగి నిజం;

      p_సూచీ--;
      స్థిరరాశి స్వయంచాలక& నమోదు_2 = పుట_map_array[p_సూచీ];

      major = నమోదు_2.major;
      చివరి_పుట_lookup = p_సూచీ;
      పుట = &పుటలు_array[నమోదు_2.సూచీ_2];
      elt = పుట_t::len () - 1;
    }
  }

  గుర్తులేని పూర్ణాంకం తదుపరి_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *బహిర్గతం,
			  గుర్తులేని పూర్ణాంకం    కొలత) స్థిరరాశి
  {
    // By default, start at the first bit of the first page of values.
    గుర్తులేని పూర్ణాంకం ప్రారంభం_పుట = 0;
    గుర్తులేని పూర్ణాంకం ప్రారంభం_పుట_విలువ = 0;
    ఒకవేళ (unlikely (codepoint != INVALID))
    {
      స్థిరరాశి స్వయంచాలక* పుట_map_array = పుట_map.arrayZ;
      గుర్తులేని పూర్ణాంకం major = get_major (codepoint);
      గుర్తులేని పూర్ణాంకం i = చివరి_పుట_lookup;
      ఒకవేళ (unlikely (i >= పుట_map.నిడివి || పుట_map_array[i].major != major))
      {
	పుట_map.bfind (major, &i, HB_NOT_దొరికింది_STORE_CLOSEST);
	ఒకవేళ (i >= పుట_map.నిడివి)
	  తిరిగి 0;  // codepoint is greater than our max element.
      }
      ప్రారంభం_పుట = i;
      ఒకవేళ (పుట_map_array[i].major == major)
      {
        ప్రారంభం_పుట_విలువ = పుట_remainder (codepoint + 1);
        ఒకవేళ (unlikely (ప్రారంభం_పుట_విలువ == 0))
        {
          // The export-after value was last in the page. Start on next page.
          ప్రారంభం_పుట++;
          ప్రారంభం_పుట_విలువ = 0;
        }
      }
    }

    గుర్తులేని పూర్ణాంకం initial_కొలత = కొలత;
    కొరకు (గుర్తులేని పూర్ణాంకం i = ప్రారంభం_పుట; i < పుట_map.నిడివి && కొలత; i++)
    {
      uint32_t పునాది = major_ప్రారంభం (పుట_map.arrayZ[i].major);
      గుర్తులేని పూర్ణాంకం n = పుటలు[పుట_map.arrayZ[i].సూచీ_2].లిఖ (పునాది, ప్రారంభం_పుట_విలువ, బహిర్గతం, కొలత);
      బహిర్గతం += n;
      కొలత -= n;
      ప్రారంభం_పుట_విలువ = 0;
    }
    తిరిగి initial_కొలత - కొలత;
  }

  గుర్తులేని పూర్ణాంకం తదుపరి_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *బహిర్గతం,
				   గుర్తులేని పూర్ణాంకం    కొలత) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం initial_కొలత = కొలత;
    // By default, start at the first bit of the first page of values.
    గుర్తులేని పూర్ణాంకం ప్రారంభం_పుట = 0;
    గుర్తులేని పూర్ణాంకం ప్రారంభం_పుట_విలువ = 0;
    ఒకవేళ (unlikely (codepoint != INVALID))
    {
      స్థిరరాశి స్వయంచాలక* పుట_map_array = పుట_map.arrayZ;
      గుర్తులేని పూర్ణాంకం major = get_major (codepoint);
      గుర్తులేని పూర్ణాంకం i = చివరి_పుట_lookup;
      ఒకవేళ (unlikely (i >= పుట_map.నిడివి || పుట_map_array[i].major != major))
      {
        పుట_map.bfind(major, &i, HB_NOT_దొరికింది_STORE_CLOSEST);
        ఒకవేళ (unlikely (i >= పుట_map.నిడివి))
        {
          // codepoint is greater than our max element.
          అయితే (++codepoint != INVALID && కొలత)
          {
            *బహిర్గతం++ = codepoint;
            కొలత--;
          }
          తిరిగి initial_కొలత - కొలత;
        }
      }
      ప్రారంభం_పుట = i;
      ఒకవేళ (పుట_map_array[i].major == major)
      {
        ప్రారంభం_పుట_విలువ = పుట_remainder (codepoint + 1);
        ఒకవేళ (unlikely (ప్రారంభం_పుట_విలువ == 0))
        {
          // The export-after value was last in the page. Start on next page.
          ప్రారంభం_పుట++;
          ప్రారంభం_పుట_విలువ = 0;
        }
      }
    }

    hb_codepoint_t తదుపరి_విలువ = codepoint + 1;
    కొరకు (గుర్తులేని పూర్ణాంకం i=ప్రారంభం_పుట; i<పుట_map.నిడివి && కొలత; i++)
    {
      uint32_t పునాది = major_ప్రారంభం (పుట_map.arrayZ[i].major);
      గుర్తులేని పూర్ణాంకం n = పుటలు[పుట_map.arrayZ[i].సూచీ_2].write_inverted (పునాది, ప్రారంభం_పుట_విలువ, బహిర్గతం, కొలత, &తదుపరి_విలువ);
      బహిర్గతం += n;
      కొలత -= n;
      ప్రారంభం_పుట_విలువ = 0;
    }
    అయితే (తదుపరి_విలువ < HB_సమితి_విలువ_INVALID && కొలత) {
      *బహిర్గతం++ = తదుపరి_విలువ++;
      కొలత--;
    }
    తిరిగి initial_కొలత - కొలత;
  }

  తార్కిక has_population () స్థిరరాశి { తిరిగి population != UINT_MAX; }
  గుర్తులేని పూర్ణాంకం get_population () స్థిరరాశి
  {
    ఒకవేళ (has_population ())
      తిరిగి population;

    గుర్తులేని పూర్ణాంకం తీయి = 0;
    గుర్తులేని పూర్ణాంకం లెక్క = పుటలు.నిడివి;
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++)
      తీయి += పుటలు[i].get_population ();

    population = తీయి;
    తిరిగి తీయి;
  }
  తార్కిక get_singleton (hb_codepoint_t *codepoint) స్థిరరాశి
  {
    ఒకవేళ (has_population ())
    {
      ఒకవేళ (population != 1) తిరిగి అబద్ధం;
      *codepoint = get_min ();
      తిరిగి నిజం;
    }

    hb_codepoint_t singleton = INVALID;
    కొరకు (స్థిరరాశి స్వయంచాలక &map : పుట_map)
    {
      స్థిరరాశి స్వయంచాలక &పుట = పుటలు.arrayZ[map.సూచీ_2];
      కొరకు (గుర్తులేని i = 0; i < పుట_t::len (); i++)
      {
	పుట_t::elt_t బిట్లు = పుట.v[i];
	ఒకవేళ (!బిట్లు) కొనసాగు;
	ఒకవేళ (singleton != INVALID || (బిట్లు & (బిట్లు - 1)))
	  తిరిగి అబద్ధం;
	singleton = map.major * పుట_t::పుట_బిట్లు +
		    i * పుట_t::ELT_బిట్లు + hb_ctz (బిట్లు);
      }
    }

    ఒకవేళ (singleton == INVALID)
    {
      population = 0;
      తిరిగి అబద్ధం;
    }
    population = 1;
    *codepoint = singleton;
    తిరిగి నిజం;
  }
  hb_codepoint_t get_min () స్థిరరాశి
  {
    గుర్తులేని లెక్క = పుటలు.నిడివి;
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
    {
      స్థిరరాశి స్వయంచాలక& map = పుట_map.arrayZ[i];
      స్థిరరాశి స్వయంచాలక& పుట = పుటలు.arrayZ[map.సూచీ_2];

      ఒకవేళ (!పుట.is_empty ())
	తిరిగి map.major * పుట_t::పుట_బిట్లు + పుట.get_min ();
    }
    తిరిగి INVALID;
  }
  hb_codepoint_t get_max () స్థిరరాశి
  {
    గుర్తులేని లెక్క = పుటలు.నిడివి;
    కొరకు (గుర్తుగల i = లెక్క - 1; i >= 0; i--)
    {
      స్థిరరాశి స్వయంచాలక& map = పుట_map.arrayZ[(గుర్తులేని) i];
      స్థిరరాశి స్వయంచాలక& పుట = పుటలు.arrayZ[map.సూచీ_2];

      ఒకవేళ (!పుట.is_empty ())
	తిరిగి map.major * పుట_t::పుట_బిట్లు + పుట.get_max ();
    }
    తిరిగి INVALID;
  }

  స్థిర constexpr hb_codepoint_t INVALID = పుట_t::INVALID;

  /*
   * Iterator implementation.
   */
  నిర్మాణం iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    స్థిర constexpr తార్కిక is_sorted_iterator = నిజం;
    స్థిర constexpr తార్కిక has_fast_len = నిజం;
    iter_t (స్థిరరాశి hb_బిట్టు_సమితి_t &s_ = Null (hb_బిట్టు_సమితి_t),
	    తార్కిక init = నిజం) : s (&s_), v (INVALID), l(0)
    {
      ఒకవేళ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    రకనిర్వచనం hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () స్థిరరాశి { తిరిగి v; }
    తార్కిక __more__ () స్థిరరాశి { తిరిగి v != INVALID; }
    శూన్య __next__ () { s->next (&v); ఒకవేళ (l) l--; }
    శూన్య __prev__ () { s->మునుపటి (&v); }
    గుర్తులేని __len__ () స్థిరరాశి { తిరిగి l; }
    iter_t చివర () స్థిరరాశి { తిరిగి iter_t (*s, అబద్ధం); }
    తార్కిక క్రియాకారకం != (స్థిరరాశి iter_t& o) స్థిరరాశి
    { తిరిగి v != o.v; }

    రక్షిత:
    స్థిరరాశి hb_బిట్టు_సమితి_t *s;
    hb_codepoint_t v;
    గుర్తులేని l;
  };
  iter_t iter () స్థిరరాశి { తిరిగి iter_t (*ఇది); }
  క్రియాకారకం iter_t () స్థిరరాశి { తిరిగి iter (); }

  రక్షిత:

  పుట_t *పుట_for (hb_codepoint_t g, తార్కిక insert = అబద్ధం)
  {
    గుర్తులేని major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    గుర్తులేని i = చివరి_పుట_lookup;
    ఒకవేళ (likely (i < పుట_map.నిడివి))
    {
      స్వయంచాలక &cached_పుట = పుట_map.arrayZ[i];
      ఒకవేళ (cached_పుట.major == major)
	తిరిగి &పుటలు.arrayZ[cached_పుట.సూచీ_2];
    }

    పుట_map_t map = {major, పుటలు.నిడివి};
    ఒకవేళ (!పుట_map.bfind (map, &i, HB_NOT_దొరికింది_STORE_CLOSEST))
    {
      ఒకవేళ (!insert)
        తిరిగి శూన్యసూచిక;

      ఒకవేళ (unlikely (!resize (పుటలు.నిడివి + 1)))
	తిరిగి శూన్యసూచిక;

      పుటలు.arrayZ[map.సూచీ_2].init0 ();
      memmove (పుట_map.arrayZ + i + 1,
	       పుట_map.arrayZ + i,
	       (పుట_map.నిడివి - 1 - i) * పుట_map.అంశం_కొలత);
      పుట_map.arrayZ[i] = map;
    }

    చివరి_పుట_lookup = i;
    తిరిగి &పుటలు.arrayZ[పుట_map.arrayZ[i].సూచీ_2];
  }
  స్థిరరాశి పుట_t *పుట_for (hb_codepoint_t g) స్థిరరాశి
  {
    గుర్తులేని major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    గుర్తులేని i = చివరి_పుట_lookup;
    ఒకవేళ (likely (i < పుట_map.నిడివి))
    {
      స్వయంచాలక &cached_పుట = పుట_map.arrayZ[i];
      ఒకవేళ (cached_పుట.major == major)
	తిరిగి &పుటలు.arrayZ[cached_పుట.సూచీ_2];
    }

    పుట_map_t key = {major};
    ఒకవేళ (!పుట_map.bfind (key, &i))
      తిరిగి శూన్యసూచిక;

    చివరి_పుట_lookup = i;
    తిరిగి &పుటలు.arrayZ[పుట_map.arrayZ[i].సూచీ_2];
  }
  పుట_t &పుట_at (గుర్తులేని పూర్ణాంకం i)
  {
    ప్రతిజ్ఞా (i < పుట_map.నిడివి);
    తిరిగి పుటలు.arrayZ[పుట_map.arrayZ[i].సూచీ_2];
  }
  స్థిరరాశి పుట_t &పుట_at (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి
  {
    ప్రతిజ్ఞా (i < పుట_map.నిడివి);
    తిరిగి పుటలు.arrayZ[పుట_map.arrayZ[i].సూచీ_2];
  }
  గుర్తులేని పూర్ణాంకం get_major (hb_codepoint_t g) స్థిరరాశి { తిరిగి g >> పుట_t::పుట_బిట్లు_LOG_2; }
  గుర్తులేని పూర్ణాంకం పుట_remainder (hb_codepoint_t g) స్థిరరాశి { తిరిగి g & పుట_t::పుట_BITMASK; }
  hb_codepoint_t major_ప్రారంభం (గుర్తులేని పూర్ణాంకం major) స్థిరరాశి { తిరిగి major << పుట_t::పుట_బిట్లు_LOG_2; }
};


#అయితే_ముగింపు /* HB_BIT_SET_HH */
