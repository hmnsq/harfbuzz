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

#ifndef HB_BIT_SET_INVERTIBLE_HH
#define HB_BIT_SET_INVERTIBLE_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set.hh"


بنية hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  منطقي inverted = ܕܓܠܐ;

  hb_bit_set_invertible_t () = افتراضي;
  hb_bit_set_invertible_t (ثابت hb_bit_set_invertible_t& o) = افتراضي;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*هذا, other); }
  hb_bit_set_invertible_t& operator= (ثابت hb_bit_set_invertible_t& o) = افتراضي;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*هذا, other); ܦܢܐ *هذا; }
  friend فراغ swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    ܐܢ (likely (!a.s.successful || !b.s.successful))
      ܦܢܐ;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  فراغ init () { s.init (); inverted = ܕܓܠܐ; }
  فراغ fini () { s.fini (); }
  فراغ err () { s.err (); }
  منطقي in_error () ثابت { ܦܢܐ s.in_error (); }
  explicit operator منطقي () ثابت { ܦܢܐ !is_empty (); }

  فراغ alloc (unsigned sz) { s.alloc (sz); }
  فراغ reset ()
  {
    s.reset ();
    inverted = ܕܓܠܐ;
  }
  فراغ clear ()
  {
    s.clear ();
    ܐܢ (likely (s.successful))
      inverted = ܕܓܠܐ;
  }
  فراغ invert ()
  {
    ܐܢ (likely (s.successful))
      inverted = !inverted;
  }

  منطقي is_inverted () ثابت
  {
    ܦܢܐ inverted;
  }

  منطقي is_empty () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ܦܢܐ v == INVALID;
  }
  uint32_t hash () ثابت { ܦܢܐ s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () ثابت
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    ܦܢܐ v;
  }
  hb_codepoint_t get_max () ثابت
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    ܦܢܐ v;
  }
  unsigned صحيح get_population () ثابت
  { ܦܢܐ inverted ? INVALID - s.get_population () : s.get_population (); }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت
  {
    ܐܢ (likely (!inverted))
      ܦܢܐ s.get_singleton (codepoint);
    ܐܢ (s.get_population () != INVALID - 1)
      ܦܢܐ ܕܓܠܐ;
    *codepoint = get_min ();
    ܦܢܐ *codepoint != INVALID;
  }


  فراغ add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  فراغ add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  منطقي add_range (hb_codepoint_t a, hb_codepoint_t b)
  { ܦܢܐ unlikely (inverted) ? ((فراغ) s.del_range (a, b), ܫܪܪܐ) : s.add_range (a, b); }

  template <typename T>
  فراغ add_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { inverted ? s.del_array (array, count, stride) : s.add_array (array, count, stride); }
  template <typename T>
  فراغ add_array (ثابت hb_array_t<ثابت T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  منطقي add_sorted_array (ثابت T *array, unsigned صحيح count, unsigned صحيح stride=حجم(T))
  { ܦܢܐ inverted ? s.del_sorted_array (array, count, stride) : s.add_sorted_array (array, count, stride); }
  template <typename T>
  منطقي add_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { ܦܢܐ add_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (فراغ) s.add_range (a, b) : s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { ܦܢܐ s.get (g) ^ inverted; }
  منطقي may_have (hb_codepoint_t g) ثابت { ܦܢܐ get (g); }

  /* Has interface. */
  منطقي operator [] (hb_codepoint_t k) ثابت { ܦܢܐ get (k); }
  منطقي has (hb_codepoint_t k) ثابت { ܦܢܐ (*هذا)[k]; }
  /* Predicate. */
  منطقي operator () (hb_codepoint_t k) ثابت { ܦܢܐ has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); ܦܢܐ *هذا; }
  hb_bit_set_invertible_t& operator << (ثابت hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ܦܢܐ *هذا; }

  منطقي may_intersect (ثابت hb_bit_set_invertible_t &other) ثابت
  { ܦܢܐ inverted || other.inverted || s.intersects (other.s); }

  منطقي intersects (ثابت hb_bit_set_invertible_t &other) ثابت
  {
    ܐܢ (likely (!inverted))
      ܦܢܐ unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    ܐܢ (likely (!other.inverted))
      ܦܢܐ !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    ܐܢ ((uint64_t) population + other_population < INVALID)
      ܦܢܐ ܫܪܪܐ;

    ܦܢܐ population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  منطقي intersects (hb_codepoint_t first, hb_codepoint_t last) ثابت
  {
    hb_codepoint_t c = first - 1;
    ܦܢܐ next (&c) && c <= last;
  }

  فراغ set (ثابت hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    ܐܢ (likely (s.successful))
      inverted = other.inverted;
  }

  منطقي is_equal (ثابت hb_bit_set_invertible_t &other) ثابت
  {
    ܐܢ (likely (inverted == other.inverted))
      ܦܢܐ s.is_equal (other.s);
    ܐܚܪܢܐ
    {
      ܐܢ (get_population () != other.get_population ())
	ܦܢܐ ܕܓܠܐ;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      ܦܢܐ hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { ܦܢܐ _.first == _.second; }));
    }
  }

  منطقي is_subset (ثابت hb_bit_set_invertible_t &larger_set) ثابت
  {
    ܐܢ (unlikely (inverted != larger_set.inverted))
    {
      ܐܢ (inverted)
	ܦܢܐ hb_all (iter (), larger_set.s);
      ܐܚܪܢܐ
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	ܦܢܐ !s.intersects (larger_set.s);
    }
    ܐܚܪܢܐ
      ܦܢܐ unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  محمي:
  template <typename Op>
  فراغ process (ثابت Op& op,
		ثابت hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  عام:
  فراغ union_ (ثابت hb_bit_set_invertible_t &other)
  {
    ܐܢ (likely (inverted == other.inverted))
    {
      ܐܢ (unlikely (inverted))
	process (hb_bitwise_and, other);
      ܐܚܪܢܐ
	process (hb_bitwise_or, other); /* Main branch. */
    }
    ܐܚܪܢܐ
    {
      ܐܢ (unlikely (inverted))
	process (hb_bitwise_gt, other);
      ܐܚܪܢܐ
	process (hb_bitwise_lt, other);
    }
    ܐܢ (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  فراغ intersect (ثابت hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    ܐܢ (likely (inverted == other.inverted))
    {
      ܐܢ (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      ܐܚܪܢܐ
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    ܐܚܪܢܐ
    {
      ܐܢ (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      ܐܚܪܢܐ
	process (hb_bitwise_gt, other, workspace);
    }
    ܐܢ (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  فراغ subtract (ثابت hb_bit_set_invertible_t &other)
  {
    ܐܢ (likely (inverted == other.inverted))
    {
      ܐܢ (unlikely (inverted))
	process (hb_bitwise_lt, other);
      ܐܚܪܢܐ
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    ܐܚܪܢܐ
    {
      ܐܢ (unlikely (inverted))
	process (hb_bitwise_or, other);
      ܐܚܪܢܐ
	process (hb_bitwise_and, other);
    }
    ܐܢ (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  فراغ symmetric_difference (ثابت hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    ܐܢ (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  منطقي next (hb_codepoint_t *codepoint) ثابت
  {
    ܐܢ (likely (!inverted))
      ܦܢܐ s.next (codepoint);

    auto old = *codepoint;
    ܐܢ (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      ܦܢܐ ܕܓܠܐ;
    }

    auto v = old;
    s.next (&v);
    ܐܢ (old + 1 < v)
    {
      *codepoint = old + 1;
      ܦܢܐ ܫܪܪܐ;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    ܦܢܐ *codepoint != INVALID;
  }
  منطقي next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ثابت
  {
    ܐܢ (likely (!inverted))
      ܦܢܐ s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    ܐܢ (unlikely (*codepoint == INVALID))
      base = 0;
    ܐܚܪܢܐ
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      ܐܢ (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	اذهب done;
      base += hb_bit_page_t::ELT_BITS;
    }

    ܟܡܐ (ܫܪܪܐ)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      ܐܢ (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      منطقي last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      ܐܢ (last)
	*bits &= UINT64_MAX >> 1;
      ܐܢ (*bits)
      {
	*codepoint = base;
	ܦܢܐ ܫܪܪܐ;
      }
      ܐܢ (unlikely (last))
	اكسر;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ܦܢܐ ܕܓܠܐ;
  }
  منطقي previous (hb_codepoint_t *codepoint) ثابت
  {
    ܐܢ (likely (!inverted))
      ܦܢܐ s.previous (codepoint);

    auto old = *codepoint;
    ܐܢ (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      ܦܢܐ ܕܓܠܐ;
    }

    auto v = old;
    s.previous (&v);

    ܐܢ (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      ܦܢܐ ܫܪܪܐ;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    ܦܢܐ *codepoint != INVALID;
  }
  منطقي next_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    ܐܢ (likely (!inverted))
      ܦܢܐ s.next_range (first, last);

    ܐܢ (!next (last))
    {
      *last = *first = INVALID;
      ܦܢܐ ܕܓܠܐ;
    }

    *first = *last;
    s.next (last);
    --*last;
    ܦܢܐ ܫܪܪܐ;
  }
  منطقي previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  {
    ܐܢ (likely (!inverted))
      ܦܢܐ s.previous_range (first, last);

    ܐܢ (!previous (first))
    {
      *last = *first = INVALID;
      ܦܢܐ ܕܓܠܐ;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    ܦܢܐ ܫܪܪܐ;
  }

  unsigned صحيح next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned صحيح    size) ثابت
  {
    ܦܢܐ inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  ساكن constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  بنية iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ساكن constexpr منطقي is_sorted_iterator = ܫܪܪܐ;
    ساكن constexpr منطقي has_fast_len = ܫܪܪܐ;
    iter_t (ثابت hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    منطقي init = ܫܪܪܐ) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      ܐܢ (init)
	advance ();
    }

    تعريفنوع hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ثابت { ܦܢܐ v; }
    منطقي __more__ () ثابت { ܦܢܐ v != INVALID; }
    فراغ __next__ ()
    {
      remaining--;
      advance ();
    }
    فراغ __prev__ ()
    {
      ܐܢ (s->previous (&v))
      {
	متزامن ();
	remaining++;
      }
      ܐܚܪܢܐ
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () ثابت
    {
      ܐܢ (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = ܫܪܪܐ;
      }
      ܦܢܐ remaining;
    }
    iter_t end () ثابت { ܦܢܐ iter_t (*s, ܕܓܠܐ); }
    منطقي operator != (ثابت iter_t& o) ثابت
    { ܦܢܐ v != o.v; }

    محمي:
    ثابت hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable منطقي have_len;
    hb_codepoint_t base;
    uint64_t bits;

    فراغ advance ()
    {
      ܐܢ (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      ܐܚܪܢܐ ܐܢ (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      ܐܚܪܢܐ
	v = INVALID;
    }

    فراغ متزامن ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned صحيح bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () ثابت { ܦܢܐ iter_t (*هذا); }
  operator iter_t () ثابت { ܦܢܐ iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */
