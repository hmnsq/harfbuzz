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

#جے_بنا_تعریف HB_ثنائی_ہندسہ_سیٹ_INVERTIBLE_HH
#تعریف_کرو HB_ثنائی_ہندسہ_سیٹ_INVERTIBLE_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set.hh"


بنية hb_ثنائی_ہندسہ_سیٹ_invertible_t
{
  hb_ثنائی_ہندسہ_سیٹ_t s;
  منطقي inverted = خطأ;

  hb_ثنائی_ہندسہ_سیٹ_invertible_t () = افتراضي;
  hb_ثنائی_ہندسہ_سیٹ_invertible_t (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t& o) = افتراضي;
  hb_ثنائی_ہندسہ_سیٹ_invertible_t (hb_ثنائی_ہندسہ_سیٹ_invertible_t&& other)  noexcept : hb_ثنائی_ہندسہ_سیٹ_invertible_t () { hb_وٹاندرا (*هذا, other); }
  hb_ثنائی_ہندسہ_سیٹ_invertible_t& عامل= (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t& o) = افتراضي;
  hb_ثنائی_ہندسہ_سیٹ_invertible_t& عامل= (hb_ثنائی_ہندسہ_سیٹ_invertible_t&& other)  noexcept { hb_وٹاندرا (*هذا, other); أرجع *هذا; }
  یار فراغ swap (hb_ثنائی_ہندسہ_سیٹ_invertible_t &a, hb_ثنائی_ہندسہ_سیٹ_invertible_t &b) noexcept
  {
    إذا (likely (!a.s.successful || !b.s.successful))
      أرجع;
    hb_وٹاندرا (a.inverted, b.inverted);
    hb_وٹاندرا (a.s, b.s);
  }

  فراغ init () { s.init (); inverted = خطأ; }
  فراغ fini () { s.fini (); }
  فراغ err () { s.err (); }
  منطقي وچ_error () ثابت { أرجع s.وچ_error (); }
  واضح عامل منطقي () ثابت { أرجع !is_empty (); }

  فراغ alloc (بنانشانی sz) { s.alloc (sz); }
  فراغ reset ()
  {
    s.reset ();
    inverted = خطأ;
  }
  فراغ clear ()
  {
    s.clear ();
    إذا (likely (s.successful))
      inverted = خطأ;
  }
  فراغ invert ()
  {
    إذا (likely (s.successful))
      inverted = !inverted;
  }

  منطقي is_inverted () ثابت
  {
    أرجع inverted;
  }

  منطقي is_empty () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    أرجع v == INVALID;
  }
  uint32_t hash () ثابت { أرجع s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    أرجع v;
  }
  hb_codepoint_t get_max () ثابت
  {
    hb_codepoint_t v = INVALID;
    پچھلا (&v);
    أرجع v;
  }
  بنانشانی صحيح get_population () ثابت
  { أرجع inverted ? INVALID - s.get_population () : s.get_population (); }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.get_singleton (codepoint);
    إذا (s.get_population () != INVALID - 1)
      أرجع خطأ;
    *codepoint = get_min ();
    أرجع *codepoint != INVALID;
  }


  فراغ جوڑو (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.جوڑو (g); }
  فراغ جوڑو_بٹ (hb_codepoint_t g, uint64_t بٹ)
  { unlikely (inverted) ? s.del_بٹ (g, بٹ) : s.جوڑو_بٹ (g, بٹ); }
  منطقي جوڑو_range (hb_codepoint_t a, hb_codepoint_t b)
  { أرجع unlikely (inverted) ? ((فراغ) s.del_range (a, b), سچ) : s.جوڑو_range (a, b); }

  سانچا <قسم_ناں T>
  فراغ جوڑو_array (ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  { inverted ? s.del_array (array, گنتی, stride) : s.جوڑو_array (array, گنتی, stride); }
  سانچا <قسم_ناں T>
  فراغ جوڑو_array (ثابت hb_array_t<ثابت T>& arr) { جوڑو_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچا <قسم_ناں T>
  منطقي جوڑو_sorted_array (ثابت T *array, بنانشانی صحيح گنتی, بنانشانی صحيح stride=حجم(T))
  { أرجع inverted ? s.del_sorted_array (array, گنتی, stride) : s.جوڑو_sorted_array (array, گنتی, stride); }
  سانچا <قسم_ناں T>
  منطقي جوڑو_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جوڑو_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { unlikely (inverted) ? s.جوڑو (g) : s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (فراغ) s.جوڑو_range (a, b) : s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { أرجع s.get (g) ^ inverted; }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي عامل [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي عامل () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_ثنائی_ہندسہ_سیٹ_invertible_t& عامل << (hb_codepoint_t v)
  { جوڑو (v); أرجع *هذا; }
  hb_ثنائی_ہندسہ_سیٹ_invertible_t& عامل << (ثابت hb_codepoint_pair_t& range)
  { جوڑو_range (range.پہلا, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other) ثابت
  { أرجع inverted || other.inverted || s.intersects (other.s); }

  منطقي intersects (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other) ثابت
  {
    إذا (likely (!inverted))
      أرجع unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    إذا (likely (!other.inverted))
      أرجع !other.s.is_subset (s);

    بنانشانی population = s.get_population ();
    بنانشانی other_population = other.s.get_population ();
    إذا ((uint64_t) population + other_population < INVALID)
      أرجع سچ;

    أرجع population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  منطقي intersects (hb_codepoint_t پہلا, hb_codepoint_t آخری) ثابت
  {
    hb_codepoint_t c = پہلا - 1;
    أرجع next (&c) && c <= آخری;
  }

  فراغ سیٹ (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other)
  {
    s.سیٹ (other.s);
    إذا (likely (s.successful))
      inverted = other.inverted;
  }

  منطقي is_equal (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other) ثابت
  {
    إذا (likely (inverted == other.inverted))
      أرجع s.is_equal (other.s);
    وإلا
    {
      إذا (get_population () != other.get_population ())
	أرجع خطأ;

      /* TODO Add iter_ranges() and use here. */
      خودکار it1 = iter ();
      خودکار it2 = other.iter ();
      أرجع hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { أرجع _.پہلا == _.second; }));
    }
  }

  منطقي is_subset (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &larger_سیٹ) ثابت
  {
    إذا (unlikely (inverted != larger_سیٹ.inverted))
    {
      إذا (inverted)
	أرجع hb_all (iter (), larger_سیٹ.s);
      وإلا
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	أرجع !s.intersects (larger_سیٹ.s);
    }
    وإلا
      أرجع unlikely (inverted) ? larger_سیٹ.s.is_subset (s) : s.is_subset (larger_سیٹ.s);
  }

  محمي:
  سانچا <قسم_ناں Op>
  فراغ process (ثابت Op& op,
		ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other,
		hb_سمتیہ_t<بنانشانی> *workspace = خالی_سوچک)
  { s.process (op, other.s, workspace); }
  عام:
  فراغ union_ (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other)
  {
    إذا (likely (inverted == other.inverted))
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_and, other);
      وإلا
	process (hb_bitwise_or, other); /* Main branch. */
    }
    وإلا
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_gt, other);
      وإلا
	process (hb_bitwise_lt, other);
    }
    إذا (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  فراغ intersect (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other,
		  hb_سمتیہ_t<بنانشانی> *workspace = خالی_سوچک)
  {
    إذا (likely (inverted == other.inverted))
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      وإلا
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    وإلا
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      وإلا
	process (hb_bitwise_gt, other, workspace);
    }
    إذا (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  فراغ گھٹاؤ (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other)
  {
    إذا (likely (inverted == other.inverted))
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_lt, other);
      وإلا
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    وإلا
    {
      إذا (unlikely (inverted))
	process (hb_bitwise_or, other);
      وإلا
	process (hb_bitwise_and, other);
    }
    إذا (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  فراغ متشاکل_difference (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    إذا (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.next (codepoint);

    خودکار old = *codepoint;
    إذا (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    خودکار v = old;
    s.next (&v);
    إذا (old + 1 < v)
    {
      *codepoint = old + 1;
      أرجع سچ;
    }

    v = old;
    s.اگلا_range (&old, &v);

    *codepoint = v + 1;
    أرجع *codepoint != INVALID;
  }
  منطقي اگلا_بٹ (hb_codepoint_t *codepoint, uint64_t *بٹ) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.اگلا_بٹ (codepoint, بٹ);

    hb_codepoint_t ادھار;
    إذا (unlikely (*codepoint == INVALID))
      ادھار = 0;
    وإلا
    {
      ادھار = *codepoint & ~hb_ثنائی_ہندسہ_ورقہ_t::ELT_نقاب;
      إذا (unlikely (ادھار >= INVALID - hb_ثنائی_ہندسہ_ورقہ_t::ELT_بٹ + 1))
	اذهب پورا;
      ادھار += hb_ثنائی_ہندسہ_ورقہ_t::ELT_بٹ;
    }

    طالما (سچ)
    {
      hb_codepoint_t cursor = ادھار ? ادھار - hb_ثنائی_ہندسہ_ورقہ_t::ELT_بٹ : INVALID;
      uint64_t excluded = 0;
      إذا (!s.اگلا_بٹ (&cursor, &excluded) || cursor != ادھار)
	excluded = 0;
      *بٹ = ~excluded;
      منطقي آخری = unlikely (ادھار >= INVALID - hb_ثنائی_ہندسہ_ورقہ_t::ELT_بٹ + 1);
      إذا (آخری)
	*بٹ &= UINT64_MAX >> 1;
      إذا (*بٹ)
      {
	*codepoint = ادھار;
	أرجع سچ;
      }
      إذا (unlikely (آخری))
	اكسر;
      ادھار += hb_ثنائی_ہندسہ_ورقہ_t::ELT_بٹ;
    }

  پورا:
    *codepoint = INVALID;
    *بٹ = 0;
    أرجع خطأ;
  }
  منطقي پچھلا (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.پچھلا (codepoint);

    خودکار old = *codepoint;
    إذا (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    خودکار v = old;
    s.پچھلا (&v);

    إذا (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      أرجع سچ;
    }

    v = old;
    s.پچھلا_range (&v, &old);

    *codepoint = v - 1;
    أرجع *codepoint != INVALID;
  }
  منطقي اگلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.اگلا_range (پہلا, آخری);

    إذا (!next (آخری))
    {
      *آخری = *پہلا = INVALID;
      أرجع خطأ;
    }

    *پہلا = *آخری;
    s.next (آخری);
    --*آخری;
    أرجع سچ;
  }
  منطقي پچھلا_range (hb_codepoint_t *پہلا, hb_codepoint_t *آخری) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.پچھلا_range (پہلا, آخری);

    إذا (!پچھلا (پہلا))
    {
      *آخری = *پہلا = INVALID;
      أرجع خطأ;
    }

    *آخری = *پہلا;
    s.پچھلا (پہلا);
    ++*پہلا;
    أرجع سچ;
  }

  بنانشانی صحيح اگلا_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *باہرلا,
			  بنانشانی صحيح    ناپ) ثابت
  {
    أرجع inverted ? s.اگلا_many_inverted (codepoint, باہرلا, ناپ)
		    : s.اگلا_many (codepoint, باہرلا, ناپ);
  }

  ساكن constexpr hb_codepoint_t INVALID = hb_ثنائی_ہندسہ_سیٹ_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = سچ;
    ساكن constexpr منطقي has_fast_len = سچ;
    iter_t (ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t &s_ = Null (hb_ثنائی_ہندسہ_سیٹ_invertible_t),
    منطقي init = سچ) : s (&s_), v (INVALID), بچیا (0),
				     have_len (!init), ادھار (INVALID), بٹ (0)
    {
      إذا (init)
	advance ();
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { أرجع v; }
    منطقي __more__ () ثابت { أرجع v != INVALID; }
    فراغ __next__ ()
    {
      بچیا--;
      advance ();
    }
    فراغ __prev__ ()
    {
      إذا (s->پچھلا (&v))
      {
	متزامن ();
	بچیا++;
      }
      وإلا
      {
	ادھار = INVALID;
	بٹ = 0;
      }
    }
    بنانشانی __len__ () ثابت
    {
      إذا (unlikely (!have_len))
      {
	بچیا += s->get_population ();
	have_len = سچ;
      }
      أرجع بچیا;
    }
    iter_t سرا () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي عامل != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_ثنائی_ہندسہ_سیٹ_invertible_t *s;
    hb_codepoint_t v;
    بدلنجوگ بنانشانی بچیا;
    بدلنجوگ منطقي have_len;
    hb_codepoint_t ادھار;
    uint64_t بٹ;

    فراغ advance ()
    {
      إذا (likely (بٹ))
      {
	v = ادھار + hb_ctz (بٹ);
	بٹ &= بٹ - 1;
      }
      وإلا إذا (s->اگلا_بٹ (&ادھار, &بٹ))
      {
	v = ادھار + hb_ctz (بٹ);
	بٹ &= بٹ - 1;
      }
      وإلا
	v = INVALID;
    }

    فراغ متزامن ()
    {
      ادھار = v & ~hb_ثنائی_ہندسہ_ورقہ_t::ELT_نقاب;
      hb_codepoint_t cursor = ادھار ? ادھار - hb_ثنائی_ہندسہ_ورقہ_t::ELT_بٹ : INVALID;
      s->اگلا_بٹ (&cursor, &بٹ);
      بنانشانی صحيح bit = v & (hb_ثنائی_ہندسہ_ورقہ_t::ELT_بٹ - 1);
      بٹ = bit == hb_ثنائی_ہندسہ_ورقہ_t::ELT_بٹ - 1 ? 0 : بٹ >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  عامل iter_t () ثابت { أرجع iter (); }
};


#جے_انت /* HB_BIT_SET_INVERTIBLE_HH */
