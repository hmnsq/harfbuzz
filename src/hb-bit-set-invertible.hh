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

#اگر_غیر_تعریف_شدہ HB_ثنائی_ہندسہ_سیٹ_INVERTIBLE_HH
#تعریف_کرو HB_ثنائی_ہندسہ_سیٹ_INVERTIBLE_HH

#شامل "hb.hh"
#شامل "hb-bit-set.hh"


ساخت hb_ثنائی_ہندسہ_سیٹ_invertible_t
{
  hb_ثنائی_ہندسہ_سیٹ_t s;
  منطقی inverted = جھوٹ;

  hb_ثنائی_ہندسہ_سیٹ_invertible_t () = طےشدہ;
  hb_ثنائی_ہندسہ_سیٹ_invertible_t (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t& o) = طےشدہ;
  hb_ثنائی_ہندسہ_سیٹ_invertible_t (hb_ثنائی_ہندسہ_سیٹ_invertible_t&& other)  noexcept : hb_ثنائی_ہندسہ_سیٹ_invertible_t () { hb_تبادلہ (*یہ, other); }
  hb_ثنائی_ہندسہ_سیٹ_invertible_t& عامل= (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t& o) = طےشدہ;
  hb_ثنائی_ہندسہ_سیٹ_invertible_t& عامل= (hb_ثنائی_ہندسہ_سیٹ_invertible_t&& other)  noexcept { hb_تبادلہ (*یہ, other); واپس *یہ; }
  دوست خالی swap (hb_ثنائی_ہندسہ_سیٹ_invertible_t &a, hb_ثنائی_ہندسہ_سیٹ_invertible_t &b) noexcept
  {
    اگر (likely (!a.s.successful || !b.s.successful))
      واپس;
    hb_تبادلہ (a.inverted, b.inverted);
    hb_تبادلہ (a.s, b.s);
  }

  خالی init () { s.init (); inverted = جھوٹ; }
  خالی fini () { s.fini (); }
  خالی err () { s.err (); }
  منطقی اندر_error () ثابت { واپس s.اندر_error (); }
  صریح عامل منطقی () ثابت { واپس !is_empty (); }

  خالی alloc (غیرعلامتی sz) { s.alloc (sz); }
  خالی reset ()
  {
    s.reset ();
    inverted = جھوٹ;
  }
  خالی clear ()
  {
    s.clear ();
    اگر (likely (s.successful))
      inverted = جھوٹ;
  }
  خالی invert ()
  {
    اگر (likely (s.successful))
      inverted = !inverted;
  }

  منطقی is_inverted () ثابت
  {
    واپس inverted;
  }

  منطقی is_empty () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    واپس v == INVALID;
  }
  uint32_t hash () ثابت { واپس s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    واپس v;
  }
  hb_codepoint_t get_max () ثابت
  {
    hb_codepoint_t v = INVALID;
    پچھلا (&v);
    واپس v;
  }
  غیرعلامتی صحیح get_population () ثابت
  { واپس inverted ? INVALID - s.get_population () : s.get_population (); }
  منطقی get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    اگر (likely (!inverted))
      واپس s.get_singleton (codepoint);
    اگر (s.get_population () != INVALID - 1)
      واپس جھوٹ;
    *codepoint = get_min ();
    واپس *codepoint != INVALID;
  }


  خالی جوڑو (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.جوڑو (g); }
  خالی جوڑو_بٹ (hb_codepoint_t g, uint64_t بٹ)
  { unlikely (inverted) ? s.del_بٹ (g, بٹ) : s.جوڑو_بٹ (g, بٹ); }
  منطقی جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  { واپس unlikely (inverted) ? ((خالی) s.del_range (a, b), سچ) : s.جوڑو_range (a, b); }

  سانچہ <قسم_نام T>
  خالی جوڑو_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  { inverted ? s.del_array (array, گنتی, stride) : s.جوڑو_array (array, گنتی, stride); }
  سانچہ <قسم_نام T>
  خالی جوڑو_array (ثابت hb_array_t<ثابت T>& arr) { جوڑو_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچہ <قسم_نام T>
  منطقی جوڑو_sorted_array (ثابت T *array, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride=حجم(T))
  { واپس inverted ? s.del_sorted_array (array, گنتی, stride) : s.جوڑو_sorted_array (array, گنتی, stride); }
  سانچہ <قسم_نام T>
  منطقی جوڑو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { واپس جوڑو_sorted_array (&arr, arr.len ()); }

  خالی del (hb_codepoint_t g) { unlikely (inverted) ? s.جوڑو (g) : s.del (g); }
  خالی del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (خالی) s.جوڑو_range (a, b) : s.del_range (a, b); }

  منطقی get (hb_codepoint_t g) ثابت { واپس s.get (g) ^ inverted; }
  منطقی may_have (hb_codepoint_t g) ثابت { واپس get (g); }

  /* Has interface. */
  منطقی عامل [] (hb_codepoint_t k) ثابت { واپس get (k); }
  منطقی has (hb_codepoint_t k) ثابت { واپس (*یہ)[k]; }
  /* Predicate. */
  منطقی عامل () (hb_codepoint_t k) ثابت { واپس has (k); }

  /* Sink interface. */
  hb_ثنائی_ہندسہ_سیٹ_invertible_t& عامل << (hb_codepoint_t v)
  { جوڑو (v); واپس *یہ; }
  hb_ثنائی_ہندسہ_سیٹ_invertible_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { جوڑو_range (range.پہلا, range.second); واپس *یہ; }

  منطقی may_intersect (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other) ثابت
  { واپس inverted || other.inverted || s.intersects (other.s); }

  منطقی intersects (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other) ثابت
  {
    اگر (likely (!inverted))
      واپس unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    اگر (likely (!other.inverted))
      واپس !other.s.is_subset (s);

    غیرعلامتی population = s.get_population ();
    غیرعلامتی other_population = other.s.get_population ();
    اگر ((uint64_t) population + other_population < INVALID)
      واپس سچ;

    واپس population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  منطقی intersects (hb_codepoint_t پہلا, hb_codepoint_t آخری) ثابت
  {
    hb_codepoint_t c = پہلا - 1;
    واپس next (&c) && c <= آخری;
  }

  خالی سیٹ (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other)
  {
    s.سیٹ (other.s);
    اگر (likely (s.successful))
      inverted = other.inverted;
  }

  منطقی is_equal (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other) ثابت
  {
    اگر (likely (inverted == other.inverted))
      واپس s.is_equal (other.s);
    ورنہ
    {
      اگر (get_population () != other.get_population ())
	واپس جھوٹ;

      /* TODO Add iter_ranges() and use here. */
      خودکار it1 = iter ();
      خودکار it2 = other.iter ();
      واپس hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { واپس _.پہلا == _.second; }));
    }
  }

  منطقی is_subset (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &larger_سیٹ) ثابت
  {
    اگر (unlikely (inverted != larger_سیٹ.inverted))
    {
      اگر (inverted)
	واپس hb_all (iter (), larger_سیٹ.s);
      ورنہ
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	واپس !s.intersects (larger_سیٹ.s);
    }
    ورنہ
      واپس unlikely (inverted) ? larger_سیٹ.s.is_subset (s) : s.is_subset (larger_سیٹ.s);
  }

  محفوظ:
  سانچہ <قسم_نام Op>
  خالی process (ثابت Op& op,
		ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other,
		hb_سمتیہ_t<غیرعلامتی> *workspace = خالی_اشاریہ)
  { s.process (op, other.s, workspace); }
  عوامی:
  خالی union_ (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other)
  {
    اگر (likely (inverted == other.inverted))
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_and, other);
      ورنہ
	process (hb_bitwise_or, other); /* Main branch. */
    }
    ورنہ
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_gt, other);
      ورنہ
	process (hb_bitwise_lt, other);
    }
    اگر (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  خالی intersect (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other,
		  hb_سمتیہ_t<غیرعلامتی> *workspace = خالی_اشاریہ)
  {
    اگر (likely (inverted == other.inverted))
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      ورنہ
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    ورنہ
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      ورنہ
	process (hb_bitwise_gt, other, workspace);
    }
    اگر (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  خالی تفریق (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other)
  {
    اگر (likely (inverted == other.inverted))
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_lt, other);
      ورنہ
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    ورنہ
    {
      اگر (unlikely (inverted))
	process (hb_bitwise_or, other);
      ورنہ
	process (hb_bitwise_and, other);
    }
    اگر (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  خالی متشاکل_difference (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    اگر (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  منطقی next (hb_codepoint_t *codepoint) ثابت
  {
    اگر (likely (!inverted))
      واپس s.next (codepoint);

    خودکار old = *codepoint;
    اگر (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      واپس جھوٹ;
    }

    خودکار v = old;
    s.next (&v);
    اگر (old + 1 < v)
    {
      *codepoint = old + 1;
      واپس سچ;
    }

    v = old;
    s.اگلا_range (&old, &v);

    *codepoint = v + 1;
    واپس *codepoint != INVALID;
  }
  منطقی اگلا_بٹ (hb_codepoint_t *codepoint, uint64_t *بٹ) ثابت
  {
    اگر (likely (!inverted))
      واپس s.اگلا_بٹ (codepoint, بٹ);

    hb_codepoint_t بنیاد;
    اگر (unlikely (*codepoint == INVALID))
      بنیاد = 0;
    ورنہ
    {
      بنیاد = *codepoint & ~hb_ثنائی_ہندسہ_صفحہ_t::ELT_نقاب;
      اگر (unlikely (بنیاد >= INVALID - hb_ثنائی_ہندسہ_صفحہ_t::ELT_بٹ + 1))
	جاؤ مکمل;
      بنیاد += hb_ثنائی_ہندسہ_صفحہ_t::ELT_بٹ;
    }

    جب (سچ)
    {
      hb_codepoint_t cursor = بنیاد ? بنیاد - hb_ثنائی_ہندسہ_صفحہ_t::ELT_بٹ : INVALID;
      uint64_t excluded = 0;
      اگر (!s.اگلا_بٹ (&cursor, &excluded) || cursor != بنیاد)
	excluded = 0;
      *بٹ = ~excluded;
      منطقی آخری = unlikely (بنیاد >= INVALID - hb_ثنائی_ہندسہ_صفحہ_t::ELT_بٹ + 1);
      اگر (آخری)
	*بٹ &= UINT64_MAX >> 1;
      اگر (*بٹ)
      {
	*codepoint = بنیاد;
	واپس سچ;
      }
      اگر (unlikely (آخری))
	توڑو;
      بنیاد += hb_ثنائی_ہندسہ_صفحہ_t::ELT_بٹ;
    }

  مکمل:
    *codepoint = INVALID;
    *بٹ = 0;
    واپس جھوٹ;
  }
  منطقی پچھلا (hb_codepoint_t *codepoint) ثابت
  {
    اگر (likely (!inverted))
      واپس s.پچھلا (codepoint);

    خودکار old = *codepoint;
    اگر (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      واپس جھوٹ;
    }

    خودکار v = old;
    s.پچھلا (&v);

    اگر (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      واپس سچ;
    }

    v = old;
    s.پچھلا_range (&v, &old);

    *codepoint = v - 1;
    واپس *codepoint != INVALID;
  }
  منطقی اگلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  {
    اگر (likely (!inverted))
      واپس s.اگلا_range (پہلا, آخری);

    اگر (!next (آخری))
    {
      *آخری = *پہلا = INVALID;
      واپس جھوٹ;
    }

    *پہلا = *آخری;
    s.next (آخری);
    --*آخری;
    واپس سچ;
  }
  منطقی پچھلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  {
    اگر (likely (!inverted))
      واپس s.پچھلا_range (پہلا, آخری);

    اگر (!پچھلا (پہلا))
    {
      *آخری = *پہلا = INVALID;
      واپس جھوٹ;
    }

    *آخری = *پہلا;
    s.پچھلا (پہلا);
    ++*پہلا;
    واپس سچ;
  }

  غیرعلامتی صحیح اگلا_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *برآمد,
			  غیرعلامتی صحیح    سائز) ثابت
  {
    واپس inverted ? s.اگلا_many_inverted (codepoint, برآمد, سائز)
		    : s.اگلا_many (codepoint, برآمد, سائز);
  }

  ساکن constexpr hb_codepoint_t INVALID = hb_ثنائی_ہندسہ_سیٹ_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  ساخت iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساکن constexpr منطقی is_sorted_iterator = سچ;
    ساکن constexpr منطقی has_fast_len = سچ;
    iter_t (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &s_ = Null (hb_ثنائی_ہندسہ_سیٹ_invertible_t),
    منطقی init = سچ) : s (&s_), v (INVALID), بقایا (0),
				     have_len (!init), بنیاد (INVALID), بٹ (0)
    {
      اگر (init)
	advance ();
    }

    قسمتعریف hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { واپس v; }
    منطقی __more__ () ثابت { واپس v != INVALID; }
    خالی __next__ ()
    {
      بقایا--;
      advance ();
    }
    خالی __prev__ ()
    {
      اگر (s->پچھلا (&v))
      {
	ہمآہنگ ();
	بقایا++;
      }
      ورنہ
      {
	بنیاد = INVALID;
	بٹ = 0;
      }
    }
    غیرعلامتی __len__ () ثابت
    {
      اگر (unlikely (!have_len))
      {
	بقایا += s->get_population ();
	have_len = سچ;
      }
      واپس بقایا;
    }
    iter_t انتہا () ثابت { واپس iter_t (*s, جھوٹ); }
    منطقی عامل != (ثابت iter_t& o) ثابت
    { واپس v != o.v; }

    محفوظ:
    ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t *s;
    hb_codepoint_t v;
    تغیرپذیر غیرعلامتی بقایا;
    تغیرپذیر منطقی have_len;
    hb_codepoint_t بنیاد;
    uint64_t بٹ;

    خالی advance ()
    {
      اگر (likely (بٹ))
      {
	v = بنیاد + hb_ctz (بٹ);
	بٹ &= بٹ - 1;
      }
      ورنہ اگر (s->اگلا_بٹ (&بنیاد, &بٹ))
      {
	v = بنیاد + hb_ctz (بٹ);
	بٹ &= بٹ - 1;
      }
      ورنہ
	v = INVALID;
    }

    خالی ہمآہنگ ()
    {
      بنیاد = v & ~hb_ثنائی_ہندسہ_صفحہ_t::ELT_نقاب;
      hb_codepoint_t cursor = بنیاد ? بنیاد - hb_ثنائی_ہندسہ_صفحہ_t::ELT_بٹ : INVALID;
      s->اگلا_بٹ (&cursor, &بٹ);
      غیرعلامتی صحیح bit = v & (hb_ثنائی_ہندسہ_صفحہ_t::ELT_بٹ - 1);
      بٹ = bit == hb_ثنائی_ہندسہ_صفحہ_t::ELT_بٹ - 1 ? 0 : بٹ >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ثابت { واپس iter_t (*یہ); }
  عامل iter_t () ثابت { واپس iter (); }
};


#اگر_ختم /* HB_BIT_SET_INVERTIBLE_HH */
