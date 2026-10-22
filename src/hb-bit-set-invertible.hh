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

#నిర్వచితం_కాకపోతే HB_బిట్టు_సమితి_INVERTIBLE_HH
#నిర్వచించు HB_బిట్టు_సమితి_INVERTIBLE_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-bit-set.hh"


నిర్మాణం hb_బిట్టు_సమితి_invertible_t
{
  hb_బిట్టు_సమితి_t s;
  తార్కిక inverted = అబద్ధం;

  hb_బిట్టు_సమితి_invertible_t () = అప్రమేయ;
  hb_బిట్టు_సమితి_invertible_t (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t& o) = అప్రమేయ;
  hb_బిట్టు_సమితి_invertible_t (hb_బిట్టు_సమితి_invertible_t&& other)  noexcept : hb_బిట్టు_సమితి_invertible_t () { hb_మార్పిడి (*ఇది, other); }
  hb_బిట్టు_సమితి_invertible_t& క్రియాకారకం= (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t& o) = అప్రమేయ;
  hb_బిట్టు_సమితి_invertible_t& క్రియాకారకం= (hb_బిట్టు_సమితి_invertible_t&& other)  noexcept { hb_మార్పిడి (*ఇది, other); తిరిగి *ఇది; }
  మిత్ర శూన్య swap (hb_బిట్టు_సమితి_invertible_t &a, hb_బిట్టు_సమితి_invertible_t &b) noexcept
  {
    ఒకవేళ (likely (!a.s.successful || !b.s.successful))
      తిరిగి;
    hb_మార్పిడి (a.inverted, b.inverted);
    hb_మార్పిడి (a.s, b.s);
  }

  శూన్య init () { s.init (); inverted = అబద్ధం; }
  శూన్య fini () { s.fini (); }
  శూన్య err () { s.err (); }
  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి s.లోపల_error (); }
  స్పష్టమైన క్రియాకారకం తార్కిక () స్థిరరాశి { తిరిగి !is_empty (); }

  శూన్య alloc (గుర్తులేని sz) { s.alloc (sz); }
  శూన్య reset ()
  {
    s.reset ();
    inverted = అబద్ధం;
  }
  శూన్య clear ()
  {
    s.clear ();
    ఒకవేళ (likely (s.successful))
      inverted = అబద్ధం;
  }
  శూన్య invert ()
  {
    ఒకవేళ (likely (s.successful))
      inverted = !inverted;
  }

  తార్కిక is_inverted () స్థిరరాశి
  {
    తిరిగి inverted;
  }

  తార్కిక is_empty () స్థిరరాశి
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    తిరిగి v == INVALID;
  }
  uint32_t hash () స్థిరరాశి { తిరిగి s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () స్థిరరాశి
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    తిరిగి v;
  }
  hb_codepoint_t get_max () స్థిరరాశి
  {
    hb_codepoint_t v = INVALID;
    మునుపటి (&v);
    తిరిగి v;
  }
  గుర్తులేని పూర్ణాంకం get_population () స్థిరరాశి
  { తిరిగి inverted ? INVALID - s.get_population () : s.get_population (); }
  తార్కిక get_singleton (hb_codepoint_t *codepoint) స్థిరరాశి
  {
    ఒకవేళ (likely (!inverted))
      తిరిగి s.get_singleton (codepoint);
    ఒకవేళ (s.get_population () != INVALID - 1)
      తిరిగి అబద్ధం;
    *codepoint = get_min ();
    తిరిగి *codepoint != INVALID;
  }


  శూన్య కూడు (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.కూడు (g); }
  శూన్య కూడు_బిట్లు (hb_codepoint_t g, uint64_t బిట్లు)
  { unlikely (inverted) ? s.del_బిట్లు (g, బిట్లు) : s.కూడు_బిట్లు (g, బిట్లు); }
  తార్కిక కూడు_range (hb_codepoint_t a, hb_codepoint_t b)
  { తిరిగి unlikely (inverted) ? ((శూన్య) s.del_range (a, b), నిజం) : s.కూడు_range (a, b); }

  మూస <రకం_పేరు T>
  శూన్య కూడు_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  { inverted ? s.del_array (array, లెక్క, stride) : s.కూడు_array (array, లెక్క, stride); }
  మూస <రకం_పేరు T>
  శూన్య కూడు_array (స్థిరరాశి hb_array_t<స్థిరరాశి T>& arr) { కూడు_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  మూస <రకం_పేరు T>
  తార్కిక కూడు_sorted_array (స్థిరరాశి T *array, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride=పరిమాణం(T))
  { తిరిగి inverted ? s.del_sorted_array (array, లెక్క, stride) : s.కూడు_sorted_array (array, లెక్క, stride); }
  మూస <రకం_పేరు T>
  తార్కిక కూడు_sorted_array (స్థిరరాశి hb_sorted_array_t<స్థిరరాశి T>& arr) { తిరిగి కూడు_sorted_array (&arr, arr.len ()); }

  శూన్య del (hb_codepoint_t g) { unlikely (inverted) ? s.కూడు (g) : s.del (g); }
  శూన్య del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (శూన్య) s.కూడు_range (a, b) : s.del_range (a, b); }

  తార్కిక get (hb_codepoint_t g) స్థిరరాశి { తిరిగి s.get (g) ^ inverted; }
  తార్కిక may_have (hb_codepoint_t g) స్థిరరాశి { తిరిగి get (g); }

  /* Has interface. */
  తార్కిక క్రియాకారకం [] (hb_codepoint_t k) స్థిరరాశి { తిరిగి get (k); }
  తార్కిక has (hb_codepoint_t k) స్థిరరాశి { తిరిగి (*ఇది)[k]; }
  /* Predicate. */
  తార్కిక క్రియాకారకం () (hb_codepoint_t k) స్థిరరాశి { తిరిగి has (k); }

  /* Sink interface. */
  hb_బిట్టు_సమితి_invertible_t& క్రియాకారకం << (hb_codepoint_t v)
  { కూడు (v); తిరిగి *ఇది; }
  hb_బిట్టు_సమితి_invertible_t& క్రియాకారకం << (స్థిరరాశి hb_codepoint_pair_t& range)
  { కూడు_range (range.మొదటి, range.second); తిరిగి *ఇది; }

  తార్కిక may_intersect (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &other) స్థిరరాశి
  { తిరిగి inverted || other.inverted || s.intersects (other.s); }

  తార్కిక intersects (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &other) స్థిరరాశి
  {
    ఒకవేళ (likely (!inverted))
      తిరిగి unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    ఒకవేళ (likely (!other.inverted))
      తిరిగి !other.s.is_subset (s);

    గుర్తులేని population = s.get_population ();
    గుర్తులేని other_population = other.s.get_population ();
    ఒకవేళ ((uint64_t) population + other_population < INVALID)
      తిరిగి నిజం;

    తిరిగి population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  తార్కిక intersects (hb_codepoint_t మొదటి, hb_codepoint_t చివరి) స్థిరరాశి
  {
    hb_codepoint_t c = మొదటి - 1;
    తిరిగి next (&c) && c <= చివరి;
  }

  శూన్య సమితి (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &other)
  {
    s.సమితి (other.s);
    ఒకవేళ (likely (s.successful))
      inverted = other.inverted;
  }

  తార్కిక is_equal (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &other) స్థిరరాశి
  {
    ఒకవేళ (likely (inverted == other.inverted))
      తిరిగి s.is_equal (other.s);
    లేకపోతే
    {
      ఒకవేళ (get_population () != other.get_population ())
	తిరిగి అబద్ధం;

      /* TODO Add iter_ranges() and use here. */
      స్వయంచాలక it1 = iter ();
      స్వయంచాలక it2 = other.iter ();
      తిరిగి hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { తిరిగి _.మొదటి == _.second; }));
    }
  }

  తార్కిక is_subset (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &larger_సమితి) స్థిరరాశి
  {
    ఒకవేళ (unlikely (inverted != larger_సమితి.inverted))
    {
      ఒకవేళ (inverted)
	తిరిగి hb_all (iter (), larger_సమితి.s);
      లేకపోతే
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	తిరిగి !s.intersects (larger_సమితి.s);
    }
    లేకపోతే
      తిరిగి unlikely (inverted) ? larger_సమితి.s.is_subset (s) : s.is_subset (larger_సమితి.s);
  }

  రక్షిత:
  మూస <రకం_పేరు Op>
  శూన్య process (స్థిరరాశి Op& op,
		స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &other,
		hb_సదిశ_t<గుర్తులేని> *workspace = శూన్యసూచిక)
  { s.process (op, other.s, workspace); }
  బహిరంగ:
  శూన్య union_ (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &other)
  {
    ఒకవేళ (likely (inverted == other.inverted))
    {
      ఒకవేళ (unlikely (inverted))
	process (hb_bitwise_and, other);
      లేకపోతే
	process (hb_bitwise_or, other); /* Main branch. */
    }
    లేకపోతే
    {
      ఒకవేళ (unlikely (inverted))
	process (hb_bitwise_gt, other);
      లేకపోతే
	process (hb_bitwise_lt, other);
    }
    ఒకవేళ (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  శూన్య intersect (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &other,
		  hb_సదిశ_t<గుర్తులేని> *workspace = శూన్యసూచిక)
  {
    ఒకవేళ (likely (inverted == other.inverted))
    {
      ఒకవేళ (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      లేకపోతే
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    లేకపోతే
    {
      ఒకవేళ (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      లేకపోతే
	process (hb_bitwise_gt, other, workspace);
    }
    ఒకవేళ (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  శూన్య వ్యవకలనం (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &other)
  {
    ఒకవేళ (likely (inverted == other.inverted))
    {
      ఒకవేళ (unlikely (inverted))
	process (hb_bitwise_lt, other);
      లేకపోతే
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    లేకపోతే
    {
      ఒకవేళ (unlikely (inverted))
	process (hb_bitwise_or, other);
      లేకపోతే
	process (hb_bitwise_and, other);
    }
    ఒకవేళ (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  శూన్య సౌష్ఠవ_difference (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    ఒకవేళ (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  తార్కిక next (hb_codepoint_t *codepoint) స్థిరరాశి
  {
    ఒకవేళ (likely (!inverted))
      తిరిగి s.next (codepoint);

    స్వయంచాలక old = *codepoint;
    ఒకవేళ (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      తిరిగి అబద్ధం;
    }

    స్వయంచాలక v = old;
    s.next (&v);
    ఒకవేళ (old + 1 < v)
    {
      *codepoint = old + 1;
      తిరిగి నిజం;
    }

    v = old;
    s.తదుపరి_range (&old, &v);

    *codepoint = v + 1;
    తిరిగి *codepoint != INVALID;
  }
  తార్కిక తదుపరి_బిట్లు (hb_codepoint_t *codepoint, uint64_t *బిట్లు) స్థిరరాశి
  {
    ఒకవేళ (likely (!inverted))
      తిరిగి s.తదుపరి_బిట్లు (codepoint, బిట్లు);

    hb_codepoint_t పునాది;
    ఒకవేళ (unlikely (*codepoint == INVALID))
      పునాది = 0;
    లేకపోతే
    {
      పునాది = *codepoint & ~hb_బిట్టు_పుట_t::ELT_ముసుగు;
      ఒకవేళ (unlikely (పునాది >= INVALID - hb_బిట్టు_పుట_t::ELT_బిట్లు + 1))
	వెళ్ళు పూర్తి;
      పునాది += hb_బిట్టు_పుట_t::ELT_బిట్లు;
    }

    అయితే (నిజం)
    {
      hb_codepoint_t cursor = పునాది ? పునాది - hb_బిట్టు_పుట_t::ELT_బిట్లు : INVALID;
      uint64_t excluded = 0;
      ఒకవేళ (!s.తదుపరి_బిట్లు (&cursor, &excluded) || cursor != పునాది)
	excluded = 0;
      *బిట్లు = ~excluded;
      తార్కిక చివరి = unlikely (పునాది >= INVALID - hb_బిట్టు_పుట_t::ELT_బిట్లు + 1);
      ఒకవేళ (చివరి)
	*బిట్లు &= UINT64_MAX >> 1;
      ఒకవేళ (*బిట్లు)
      {
	*codepoint = పునాది;
	తిరిగి నిజం;
      }
      ఒకవేళ (unlikely (చివరి))
	ఆపు;
      పునాది += hb_బిట్టు_పుట_t::ELT_బిట్లు;
    }

  పూర్తి:
    *codepoint = INVALID;
    *బిట్లు = 0;
    తిరిగి అబద్ధం;
  }
  తార్కిక మునుపటి (hb_codepoint_t *codepoint) స్థిరరాశి
  {
    ఒకవేళ (likely (!inverted))
      తిరిగి s.మునుపటి (codepoint);

    స్వయంచాలక old = *codepoint;
    ఒకవేళ (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      తిరిగి అబద్ధం;
    }

    స్వయంచాలక v = old;
    s.మునుపటి (&v);

    ఒకవేళ (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      తిరిగి నిజం;
    }

    v = old;
    s.మునుపటి_range (&v, &old);

    *codepoint = v - 1;
    తిరిగి *codepoint != INVALID;
  }
  తార్కిక తదుపరి_range (hb_codepoint_t *మొదటి, hb_codepoint_t *చివరి) స్థిరరాశి
  {
    ఒకవేళ (likely (!inverted))
      తిరిగి s.తదుపరి_range (మొదటి, చివరి);

    ఒకవేళ (!next (చివరి))
    {
      *చివరి = *మొదటి = INVALID;
      తిరిగి అబద్ధం;
    }

    *మొదటి = *చివరి;
    s.next (చివరి);
    --*చివరి;
    తిరిగి నిజం;
  }
  తార్కిక మునుపటి_range (hb_codepoint_t *మొదటి, hb_codepoint_t *చివరి) స్థిరరాశి
  {
    ఒకవేళ (likely (!inverted))
      తిరిగి s.మునుపటి_range (మొదటి, చివరి);

    ఒకవేళ (!మునుపటి (మొదటి))
    {
      *చివరి = *మొదటి = INVALID;
      తిరిగి అబద్ధం;
    }

    *చివరి = *మొదటి;
    s.మునుపటి (మొదటి);
    ++*మొదటి;
    తిరిగి నిజం;
  }

  గుర్తులేని పూర్ణాంకం తదుపరి_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *బహిర్గతం,
			  గుర్తులేని పూర్ణాంకం    కొలత) స్థిరరాశి
  {
    తిరిగి inverted ? s.తదుపరి_many_inverted (codepoint, బహిర్గతం, కొలత)
		    : s.తదుపరి_many (codepoint, బహిర్గతం, కొలత);
  }

  స్థిర constexpr hb_codepoint_t INVALID = hb_బిట్టు_సమితి_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  నిర్మాణం iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    స్థిర constexpr తార్కిక is_sorted_iterator = నిజం;
    స్థిర constexpr తార్కిక has_fast_len = నిజం;
    iter_t (స్థిరరాశి hb_బిట్టు_సమితి_invertible_t &s_ = Null (hb_బిట్టు_సమితి_invertible_t),
    తార్కిక init = నిజం) : s (&s_), v (INVALID), మిగిలిన (0),
				     have_len (!init), పునాది (INVALID), బిట్లు (0)
    {
      ఒకవేళ (init)
	advance ();
    }

    రకనిర్వచనం hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () స్థిరరాశి { తిరిగి v; }
    తార్కిక __more__ () స్థిరరాశి { తిరిగి v != INVALID; }
    శూన్య __next__ ()
    {
      మిగిలిన--;
      advance ();
    }
    శూన్య __prev__ ()
    {
      ఒకవేళ (s->మునుపటి (&v))
      {
	సమకాలీకరణ ();
	మిగిలిన++;
      }
      లేకపోతే
      {
	పునాది = INVALID;
	బిట్లు = 0;
      }
    }
    గుర్తులేని __len__ () స్థిరరాశి
    {
      ఒకవేళ (unlikely (!have_len))
      {
	మిగిలిన += s->get_population ();
	have_len = నిజం;
      }
      తిరిగి మిగిలిన;
    }
    iter_t చివర () స్థిరరాశి { తిరిగి iter_t (*s, అబద్ధం); }
    తార్కిక క్రియాకారకం != (స్థిరరాశి iter_t& o) స్థిరరాశి
    { తిరిగి v != o.v; }

    రక్షిత:
    స్థిరరాశి hb_బిట్టు_సమితి_invertible_t *s;
    hb_codepoint_t v;
    మార్చదగిన గుర్తులేని మిగిలిన;
    మార్చదగిన తార్కిక have_len;
    hb_codepoint_t పునాది;
    uint64_t బిట్లు;

    శూన్య advance ()
    {
      ఒకవేళ (likely (బిట్లు))
      {
	v = పునాది + hb_ctz (బిట్లు);
	బిట్లు &= బిట్లు - 1;
      }
      లేకపోతే ఒకవేళ (s->తదుపరి_బిట్లు (&పునాది, &బిట్లు))
      {
	v = పునాది + hb_ctz (బిట్లు);
	బిట్లు &= బిట్లు - 1;
      }
      లేకపోతే
	v = INVALID;
    }

    శూన్య సమకాలీకరణ ()
    {
      పునాది = v & ~hb_బిట్టు_పుట_t::ELT_ముసుగు;
      hb_codepoint_t cursor = పునాది ? పునాది - hb_బిట్టు_పుట_t::ELT_బిట్లు : INVALID;
      s->తదుపరి_బిట్లు (&cursor, &బిట్లు);
      గుర్తులేని పూర్ణాంకం bit = v & (hb_బిట్టు_పుట_t::ELT_బిట్లు - 1);
      బిట్లు = bit == hb_బిట్టు_పుట_t::ELT_బిట్లు - 1 ? 0 : బిట్లు >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () స్థిరరాశి { తిరిగి iter_t (*ఇది); }
  క్రియాకారకం iter_t () స్థిరరాశి { తిరిగి iter (); }
};


#అయితే_ముగింపు /* HB_BIT_SET_INVERTIBLE_HH */
