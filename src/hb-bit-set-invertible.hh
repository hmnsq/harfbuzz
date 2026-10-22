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

#که_نه_تعریف_شوی HB_بټ_سېټ_INVERTIBLE_HH
#تعریف_کړه HB_بټ_سېټ_INVERTIBLE_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set.hh"


بنية hb_بټ_سېټ_invertible_t
{
  hb_بټ_سېټ_t s;
  منطقي inverted = خطأ;

  hb_بټ_سېټ_invertible_t () = افتراضي;
  hb_بټ_سېټ_invertible_t (ثابت hb_بټ_سېټ_invertible_t& o) = افتراضي;
  hb_بټ_سېټ_invertible_t (hb_بټ_سېټ_invertible_t&& other)  noexcept : hb_بټ_سېټ_invertible_t () { hb_بدلون (*هذا, other); }
  hb_بټ_سېټ_invertible_t& چلوونکی= (ثابت hb_بټ_سېټ_invertible_t& o) = افتراضي;
  hb_بټ_سېټ_invertible_t& چلوونکی= (hb_بټ_سېټ_invertible_t&& other)  noexcept { hb_بدلون (*هذا, other); أرجع *هذا; }
  ملګری فراغ swap (hb_بټ_سېټ_invertible_t &a, hb_بټ_سېټ_invertible_t &b) noexcept
  {
    إذا (likely (!a.s.successful || !b.s.successful))
      أرجع;
    hb_بدلون (a.inverted, b.inverted);
    hb_بدلون (a.s, b.s);
  }

  فراغ init () { s.init (); inverted = خطأ; }
  فراغ fini () { s.fini (); }
  فراغ err () { s.err (); }
  منطقي دننه_error () ثابت { أرجع s.دننه_error (); }
  څرګند چلوونکی منطقي () ثابت { أرجع !is_empty (); }

  فراغ alloc (بینښه sz) { s.alloc (sz); }
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
    پخوانی (&v);
    أرجع v;
  }
  بینښه صحيح get_population () ثابت
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


  فراغ جمع_کړه (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.جمع_کړه (g); }
  فراغ جمع_کړه_بټونه (hb_codepoint_t g, uint64_t بټونه)
  { unlikely (inverted) ? s.del_بټونه (g, بټونه) : s.جمع_کړه_بټونه (g, بټونه); }
  منطقي جمع_کړه_range (hb_codepoint_t a, hb_codepoint_t b)
  { أرجع unlikely (inverted) ? ((فراغ) s.del_range (a, b), ریښتیا) : s.جمع_کړه_range (a, b); }

  کينډۍ <ډول_نوم T>
  فراغ جمع_کړه_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  { inverted ? s.del_array (array, شمېر, stride) : s.جمع_کړه_array (array, شمېر, stride); }
  کينډۍ <ډول_نوم T>
  فراغ جمع_کړه_array (ثابت hb_array_t<ثابت T>& arr) { جمع_کړه_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  کينډۍ <ډول_نوم T>
  منطقي جمع_کړه_sorted_array (ثابت T *array, بینښه صحيح شمېر, بینښه صحيح stride=حجم(T))
  { أرجع inverted ? s.del_sorted_array (array, شمېر, stride) : s.جمع_کړه_sorted_array (array, شمېر, stride); }
  کينډۍ <ډول_نوم T>
  منطقي جمع_کړه_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع جمع_کړه_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { unlikely (inverted) ? s.جمع_کړه (g) : s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (فراغ) s.جمع_کړه_range (a, b) : s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { أرجع s.get (g) ^ inverted; }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي چلوونکی [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Predicate. */
  منطقي چلوونکی () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_بټ_سېټ_invertible_t& چلوونکی << (hb_codepoint_t v)
  { جمع_کړه (v); أرجع *هذا; }
  hb_بټ_سېټ_invertible_t& چلوونکی << (ثابت hb_codepoint_pair_t& range)
  { جمع_کړه_range (range.لومړی, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_بټ_سېټ_invertible_t &other) ثابت
  { أرجع inverted || other.inverted || s.intersects (other.s); }

  منطقي intersects (ثابت hb_بټ_سېټ_invertible_t &other) ثابت
  {
    إذا (likely (!inverted))
      أرجع unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    إذا (likely (!other.inverted))
      أرجع !other.s.is_subset (s);

    بینښه population = s.get_population ();
    بینښه other_population = other.s.get_population ();
    إذا ((uint64_t) population + other_population < INVALID)
      أرجع ریښتیا;

    أرجع population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  منطقي intersects (hb_codepoint_t لومړی, hb_codepoint_t وروستی) ثابت
  {
    hb_codepoint_t c = لومړی - 1;
    أرجع next (&c) && c <= وروستی;
  }

  فراغ سېټ (ثابت hb_بټ_سېټ_invertible_t &other)
  {
    s.سېټ (other.s);
    إذا (likely (s.successful))
      inverted = other.inverted;
  }

  منطقي is_equal (ثابت hb_بټ_سېټ_invertible_t &other) ثابت
  {
    إذا (likely (inverted == other.inverted))
      أرجع s.is_equal (other.s);
    وإلا
    {
      إذا (get_population () != other.get_population ())
	أرجع خطأ;

      /* TODO Add iter_ranges() and use here. */
      خپلکاری it1 = iter ();
      خپلکاری it2 = other.iter ();
      أرجع hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { أرجع _.لومړی == _.second; }));
    }
  }

  منطقي is_subset (ثابت hb_بټ_سېټ_invertible_t &larger_سېټ) ثابت
  {
    إذا (unlikely (inverted != larger_سېټ.inverted))
    {
      إذا (inverted)
	أرجع hb_all (iter (), larger_سېټ.s);
      وإلا
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	أرجع !s.intersects (larger_سېټ.s);
    }
    وإلا
      أرجع unlikely (inverted) ? larger_سېټ.s.is_subset (s) : s.is_subset (larger_سېټ.s);
  }

  محمي:
  کينډۍ <ډول_نوم Op>
  فراغ process (ثابت Op& op,
		ثابت hb_بټ_سېټ_invertible_t &other,
		hb_ویکټور_t<بینښه> *workspace = تش_اشاره)
  { s.process (op, other.s, workspace); }
  عام:
  فراغ union_ (ثابت hb_بټ_سېټ_invertible_t &other)
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
  فراغ intersect (ثابت hb_بټ_سېټ_invertible_t &other,
		  hb_ویکټور_t<بینښه> *workspace = تش_اشاره)
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
  فراغ کمول (ثابت hb_بټ_سېټ_invertible_t &other)
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
  فراغ متناظر_difference (ثابت hb_بټ_سېټ_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    إذا (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.next (codepoint);

    خپلکاری old = *codepoint;
    إذا (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    خپلکاری v = old;
    s.next (&v);
    إذا (old + 1 < v)
    {
      *codepoint = old + 1;
      أرجع ریښتیا;
    }

    v = old;
    s.بل_range (&old, &v);

    *codepoint = v + 1;
    أرجع *codepoint != INVALID;
  }
  منطقي بل_بټونه (hb_codepoint_t *codepoint, uint64_t *بټونه) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.بل_بټونه (codepoint, بټونه);

    hb_codepoint_t بنسټ;
    إذا (unlikely (*codepoint == INVALID))
      بنسټ = 0;
    وإلا
    {
      بنسټ = *codepoint & ~hb_بټ_پاڼه_t::ELT_ماسک;
      إذا (unlikely (بنسټ >= INVALID - hb_بټ_پاڼه_t::ELT_بټونه + 1))
	اذهب بشپړ;
      بنسټ += hb_بټ_پاڼه_t::ELT_بټونه;
    }

    طالما (ریښتیا)
    {
      hb_codepoint_t cursor = بنسټ ? بنسټ - hb_بټ_پاڼه_t::ELT_بټونه : INVALID;
      uint64_t excluded = 0;
      إذا (!s.بل_بټونه (&cursor, &excluded) || cursor != بنسټ)
	excluded = 0;
      *بټونه = ~excluded;
      منطقي وروستی = unlikely (بنسټ >= INVALID - hb_بټ_پاڼه_t::ELT_بټونه + 1);
      إذا (وروستی)
	*بټونه &= UINT64_MAX >> 1;
      إذا (*بټونه)
      {
	*codepoint = بنسټ;
	أرجع ریښتیا;
      }
      إذا (unlikely (وروستی))
	اكسر;
      بنسټ += hb_بټ_پاڼه_t::ELT_بټونه;
    }

  بشپړ:
    *codepoint = INVALID;
    *بټونه = 0;
    أرجع خطأ;
  }
  منطقي پخوانی (hb_codepoint_t *codepoint) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.پخوانی (codepoint);

    خپلکاری old = *codepoint;
    إذا (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      أرجع خطأ;
    }

    خپلکاری v = old;
    s.پخوانی (&v);

    إذا (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      أرجع ریښتیا;
    }

    v = old;
    s.پخوانی_range (&v, &old);

    *codepoint = v - 1;
    أرجع *codepoint != INVALID;
  }
  منطقي بل_range (hb_codepoint_t *لومړی, hb_codepoint_t *وروستی) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.بل_range (لومړی, وروستی);

    إذا (!next (وروستی))
    {
      *وروستی = *لومړی = INVALID;
      أرجع خطأ;
    }

    *لومړی = *وروستی;
    s.next (وروستی);
    --*وروستی;
    أرجع ریښتیا;
  }
  منطقي پخوانی_range (hb_codepoint_t *لومړی, hb_codepoint_t *وروستی) ثابت
  {
    إذا (likely (!inverted))
      أرجع s.پخوانی_range (لومړی, وروستی);

    إذا (!پخوانی (لومړی))
    {
      *وروستی = *لومړی = INVALID;
      أرجع خطأ;
    }

    *وروستی = *لومړی;
    s.پخوانی (لومړی);
    ++*لومړی;
    أرجع ریښتیا;
  }

  بینښه صحيح بل_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *بهر,
			  بینښه صحيح    اندازه) ثابت
  {
    أرجع inverted ? s.بل_many_inverted (codepoint, بهر, اندازه)
		    : s.بل_many (codepoint, بهر, اندازه);
  }

  ساكن constexpr hb_codepoint_t INVALID = hb_بټ_سېټ_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = ریښتیا;
    ساكن constexpr منطقي has_fast_len = ریښتیا;
    iter_t (ثابت hb_بټ_سېټ_invertible_t &s_ = Null (hb_بټ_سېټ_invertible_t),
    منطقي init = ریښتیا) : s (&s_), v (INVALID), پاتې_شوی (0),
				     have_len (!init), بنسټ (INVALID), بټونه (0)
    {
      إذا (init)
	advance ();
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { أرجع v; }
    منطقي __more__ () ثابت { أرجع v != INVALID; }
    فراغ __next__ ()
    {
      پاتې_شوی--;
      advance ();
    }
    فراغ __prev__ ()
    {
      إذا (s->پخوانی (&v))
      {
	متزامن ();
	پاتې_شوی++;
      }
      وإلا
      {
	بنسټ = INVALID;
	بټونه = 0;
      }
    }
    بینښه __len__ () ثابت
    {
      إذا (unlikely (!have_len))
      {
	پاتې_شوی += s->get_population ();
	have_len = ریښتیا;
      }
      أرجع پاتې_شوی;
    }
    iter_t پای_ټکی () ثابت { أرجع iter_t (*s, خطأ); }
    منطقي چلوونکی != (ثابت iter_t& o) ثابت
    { أرجع v != o.v; }

    محمي:
    ثابت hb_بټ_سېټ_invertible_t *s;
    hb_codepoint_t v;
    بدلیدونکی بینښه پاتې_شوی;
    بدلیدونکی منطقي have_len;
    hb_codepoint_t بنسټ;
    uint64_t بټونه;

    فراغ advance ()
    {
      إذا (likely (بټونه))
      {
	v = بنسټ + hb_ctz (بټونه);
	بټونه &= بټونه - 1;
      }
      وإلا إذا (s->بل_بټونه (&بنسټ, &بټونه))
      {
	v = بنسټ + hb_ctz (بټونه);
	بټونه &= بټونه - 1;
      }
      وإلا
	v = INVALID;
    }

    فراغ متزامن ()
    {
      بنسټ = v & ~hb_بټ_پاڼه_t::ELT_ماسک;
      hb_codepoint_t cursor = بنسټ ? بنسټ - hb_بټ_پاڼه_t::ELT_بټونه : INVALID;
      s->بل_بټونه (&cursor, &بټونه);
      بینښه صحيح bit = v & (hb_بټ_پاڼه_t::ELT_بټونه - 1);
      بټونه = bit == hb_بټ_پاڼه_t::ELT_بټونه - 1 ? 0 : بټونه >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ثابت { أرجع iter_t (*هذا); }
  چلوونکی iter_t () ثابت { أرجع iter (); }
};


#د_که_پای /* HB_BIT_SET_INVERTIBLE_HH */
