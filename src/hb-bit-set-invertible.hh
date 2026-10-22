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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-bit-set.hh"


ഘടന hb_bit_set_invertible_t
{
  hb_bit_set_t s;
  തർക്കം inverted = അസത്യം;

  hb_bit_set_invertible_t () = സ്ഥിരസ്ഥിതി;
  hb_bit_set_invertible_t (സ്ഥിരം hb_bit_set_invertible_t& o) = സ്ഥിരസ്ഥിതി;
  hb_bit_set_invertible_t (hb_bit_set_invertible_t&& other)  noexcept : hb_bit_set_invertible_t () { hb_swap (*ഇത്, other); }
  hb_bit_set_invertible_t& operator= (സ്ഥിരം hb_bit_set_invertible_t& o) = സ്ഥിരസ്ഥിതി;
  hb_bit_set_invertible_t& operator= (hb_bit_set_invertible_t&& other)  noexcept { hb_swap (*ഇത്, other); തിരികെ *ഇത്; }
  friend ശൂന്യം swap (hb_bit_set_invertible_t &a, hb_bit_set_invertible_t &b) noexcept
  {
    എങ്കിൽ (likely (!a.s.successful || !b.s.successful))
      തിരികെ;
    hb_swap (a.inverted, b.inverted);
    hb_swap (a.s, b.s);
  }

  ശൂന്യം init () { s.init (); inverted = അസത്യം; }
  ശൂന്യം fini () { s.fini (); }
  ശൂന്യം err () { s.err (); }
  തർക്കം in_error () സ്ഥിരം { തിരികെ s.in_error (); }
  explicit operator തർക്കം () സ്ഥിരം { തിരികെ !is_empty (); }

  ശൂന്യം alloc (unsigned sz) { s.alloc (sz); }
  ശൂന്യം reset ()
  {
    s.reset ();
    inverted = അസത്യം;
  }
  ശൂന്യം clear ()
  {
    s.clear ();
    എങ്കിൽ (likely (s.successful))
      inverted = അസത്യം;
  }
  ശൂന്യം invert ()
  {
    എങ്കിൽ (likely (s.successful))
      inverted = !inverted;
  }

  തർക്കം is_inverted () സ്ഥിരം
  {
    തിരികെ inverted;
  }

  തർക്കം is_empty () സ്ഥിരം
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    തിരികെ v == INVALID;
  }
  uint32_t hash () സ്ഥിരം { തിരികെ s.hash () ^ (uint32_t) inverted; }

  hb_codepoint_t get_min () സ്ഥിരം
  {
    hb_codepoint_t v = INVALID;
    next (&v);
    തിരികെ v;
  }
  hb_codepoint_t get_max () സ്ഥിരം
  {
    hb_codepoint_t v = INVALID;
    previous (&v);
    തിരികെ v;
  }
  unsigned പൂർണ്ണസംഖ്യ get_population () സ്ഥിരം
  { തിരികെ inverted ? INVALID - s.get_population () : s.get_population (); }
  തർക്കം get_singleton (hb_codepoint_t *codepoint) സ്ഥിരം
  {
    എങ്കിൽ (likely (!inverted))
      തിരികെ s.get_singleton (codepoint);
    എങ്കിൽ (s.get_population () != INVALID - 1)
      തിരികെ അസത്യം;
    *codepoint = get_min ();
    തിരികെ *codepoint != INVALID;
  }


  ശൂന്യം add (hb_codepoint_t g) { unlikely (inverted) ? s.del (g) : s.add (g); }
  ശൂന്യം add_bits (hb_codepoint_t g, uint64_t bits)
  { unlikely (inverted) ? s.del_bits (g, bits) : s.add_bits (g, bits); }
  തർക്കം add_range (hb_codepoint_t a, hb_codepoint_t b)
  { തിരികെ unlikely (inverted) ? ((ശൂന്യം) s.del_range (a, b), സത്യം) : s.add_range (a, b); }

  template <typename T>
  ശൂന്യം add_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  { inverted ? s.del_array (array, എണ്ണം, stride) : s.add_array (array, എണ്ണം, stride); }
  template <typename T>
  ശൂന്യം add_array (സ്ഥിരം hb_array_t<സ്ഥിരം T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  തർക്കം add_sorted_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  { തിരികെ inverted ? s.del_sorted_array (array, എണ്ണം, stride) : s.add_sorted_array (array, എണ്ണം, stride); }
  template <typename T>
  തർക്കം add_sorted_array (സ്ഥിരം hb_sorted_array_t<സ്ഥിരം T>& arr) { തിരികെ add_sorted_array (&arr, arr.len ()); }

  ശൂന്യം del (hb_codepoint_t g) { unlikely (inverted) ? s.add (g) : s.del (g); }
  ശൂന്യം del_range (hb_codepoint_t a, hb_codepoint_t b)
  { unlikely (inverted) ? (ശൂന്യം) s.add_range (a, b) : s.del_range (a, b); }

  തർക്കം get (hb_codepoint_t g) സ്ഥിരം { തിരികെ s.get (g) ^ inverted; }
  തർക്കം may_have (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }

  /* Has interface. */
  തർക്കം operator [] (hb_codepoint_t k) സ്ഥിരം { തിരികെ get (k); }
  തർക്കം has (hb_codepoint_t k) സ്ഥിരം { തിരികെ (*ഇത്)[k]; }
  /* Predicate. */
  തർക്കം operator () (hb_codepoint_t k) സ്ഥിരം { തിരികെ has (k); }

  /* Sink interface. */
  hb_bit_set_invertible_t& operator << (hb_codepoint_t v)
  { add (v); തിരികെ *ഇത്; }
  hb_bit_set_invertible_t& operator << (സ്ഥിരം hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); തിരികെ *ഇത്; }

  തർക്കം may_intersect (സ്ഥിരം hb_bit_set_invertible_t &other) സ്ഥിരം
  { തിരികെ inverted || other.inverted || s.intersects (other.s); }

  തർക്കം intersects (സ്ഥിരം hb_bit_set_invertible_t &other) സ്ഥിരം
  {
    എങ്കിൽ (likely (!inverted))
      തിരികെ unlikely (other.inverted) ? !s.is_subset (other.s)
				       : s.intersects (other.s);
    എങ്കിൽ (likely (!other.inverted))
      തിരികെ !other.s.is_subset (s);

    unsigned population = s.get_population ();
    unsigned other_population = other.s.get_population ();
    എങ്കിൽ ((uint64_t) population + other_population < INVALID)
      തിരികെ സത്യം;

    തിരികെ population >= other_population
	 ? !hb_all (iter (), other.s)
	 : !hb_all (other.iter (), s);
  }

  തർക്കം intersects (hb_codepoint_t first, hb_codepoint_t last) സ്ഥിരം
  {
    hb_codepoint_t c = first - 1;
    തിരികെ next (&c) && c <= last;
  }

  ശൂന്യം set (സ്ഥിരം hb_bit_set_invertible_t &other)
  {
    s.set (other.s);
    എങ്കിൽ (likely (s.successful))
      inverted = other.inverted;
  }

  തർക്കം is_equal (സ്ഥിരം hb_bit_set_invertible_t &other) സ്ഥിരം
  {
    എങ്കിൽ (likely (inverted == other.inverted))
      തിരികെ s.is_equal (other.s);
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (get_population () != other.get_population ())
	തിരികെ അസത്യം;

      /* TODO Add iter_ranges() and use here. */
      auto it1 = iter ();
      auto it2 = other.iter ();
      തിരികെ hb_all (+ hb_zip (it1, it2)
		     | hb_map ([](hb_codepoint_pair_t _) { തിരികെ _.first == _.second; }));
    }
  }

  തർക്കം is_subset (സ്ഥിരം hb_bit_set_invertible_t &larger_set) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (inverted != larger_set.inverted))
    {
      എങ്കിൽ (inverted)
	തിരികെ hb_all (iter (), larger_set.s);
      അല്ലെങ്കിൽ
        // larger set is inverted so larger_set.s is the set of things that are not present
        // in larger_set, therefore if s has any of those it can't be a subset.
	തിരികെ !s.intersects (larger_set.s);
    }
    അല്ലെങ്കിൽ
      തിരികെ unlikely (inverted) ? larger_set.s.is_subset (s) : s.is_subset (larger_set.s);
  }

  സംരക്ഷിത:
  template <typename Op>
  ശൂന്യം process (സ്ഥിരം Op& op,
		സ്ഥിരം hb_bit_set_invertible_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  { s.process (op, other.s, workspace); }
  പൊതു:
  ശൂന്യം union_ (സ്ഥിരം hb_bit_set_invertible_t &other)
  {
    എങ്കിൽ (likely (inverted == other.inverted))
    {
      എങ്കിൽ (unlikely (inverted))
	process (hb_bitwise_and, other);
      അല്ലെങ്കിൽ
	process (hb_bitwise_or, other); /* Main branch. */
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (unlikely (inverted))
	process (hb_bitwise_gt, other);
      അല്ലെങ്കിൽ
	process (hb_bitwise_lt, other);
    }
    എങ്കിൽ (likely (s.successful))
      inverted = inverted || other.inverted;
  }
  ശൂന്യം intersect (സ്ഥിരം hb_bit_set_invertible_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  {
    എങ്കിൽ (likely (inverted == other.inverted))
    {
      എങ്കിൽ (unlikely (inverted))
	process (hb_bitwise_or, other, workspace);
      അല്ലെങ്കിൽ
	process (hb_bitwise_and, other, workspace); /* Main branch. */
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (unlikely (inverted))
	process (hb_bitwise_lt, other, workspace);
      അല്ലെങ്കിൽ
	process (hb_bitwise_gt, other, workspace);
    }
    എങ്കിൽ (likely (s.successful))
      inverted = inverted && other.inverted;
  }
  ശൂന്യം subtract (സ്ഥിരം hb_bit_set_invertible_t &other)
  {
    എങ്കിൽ (likely (inverted == other.inverted))
    {
      എങ്കിൽ (unlikely (inverted))
	process (hb_bitwise_lt, other);
      അല്ലെങ്കിൽ
	process (hb_bitwise_gt, other); /* Main branch. */
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (unlikely (inverted))
	process (hb_bitwise_or, other);
      അല്ലെങ്കിൽ
	process (hb_bitwise_and, other);
    }
    എങ്കിൽ (likely (s.successful))
      inverted = inverted && !other.inverted;
  }
  ശൂന്യം symmetric_difference (സ്ഥിരം hb_bit_set_invertible_t &other)
  {
    process (hb_bitwise_xor, other);
    എങ്കിൽ (likely (s.successful))
      inverted = inverted ^ other.inverted;
  }

  തർക്കം next (hb_codepoint_t *codepoint) സ്ഥിരം
  {
    എങ്കിൽ (likely (!inverted))
      തിരികെ s.next (codepoint);

    auto old = *codepoint;
    എങ്കിൽ (unlikely (old + 1 == INVALID))
    {
      *codepoint = INVALID;
      തിരികെ അസത്യം;
    }

    auto v = old;
    s.next (&v);
    എങ്കിൽ (old + 1 < v)
    {
      *codepoint = old + 1;
      തിരികെ സത്യം;
    }

    v = old;
    s.next_range (&old, &v);

    *codepoint = v + 1;
    തിരികെ *codepoint != INVALID;
  }
  തർക്കം next_bits (hb_codepoint_t *codepoint, uint64_t *bits) സ്ഥിരം
  {
    എങ്കിൽ (likely (!inverted))
      തിരികെ s.next_bits (codepoint, bits);

    hb_codepoint_t base;
    എങ്കിൽ (unlikely (*codepoint == INVALID))
      base = 0;
    അല്ലെങ്കിൽ
    {
      base = *codepoint & ~hb_bit_page_t::ELT_MASK;
      എങ്കിൽ (unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1))
	പോകുക done;
      base += hb_bit_page_t::ELT_BITS;
    }

    ആകുമ്പോൾ (സത്യം)
    {
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      uint64_t excluded = 0;
      എങ്കിൽ (!s.next_bits (&cursor, &excluded) || cursor != base)
	excluded = 0;
      *bits = ~excluded;
      തർക്കം last = unlikely (base >= INVALID - hb_bit_page_t::ELT_BITS + 1);
      എങ്കിൽ (last)
	*bits &= UINT64_MAX >> 1;
      എങ്കിൽ (*bits)
      {
	*codepoint = base;
	തിരികെ സത്യം;
      }
      എങ്കിൽ (unlikely (last))
	നിർത്തുക;
      base += hb_bit_page_t::ELT_BITS;
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    തിരികെ അസത്യം;
  }
  തർക്കം previous (hb_codepoint_t *codepoint) സ്ഥിരം
  {
    എങ്കിൽ (likely (!inverted))
      തിരികെ s.previous (codepoint);

    auto old = *codepoint;
    എങ്കിൽ (unlikely (old - 1 == INVALID))
    {
      *codepoint = INVALID;
      തിരികെ അസത്യം;
    }

    auto v = old;
    s.previous (&v);

    എങ്കിൽ (old - 1 > v || v == INVALID)
    {
      *codepoint = old - 1;
      തിരികെ സത്യം;
    }

    v = old;
    s.previous_range (&v, &old);

    *codepoint = v - 1;
    തിരികെ *codepoint != INVALID;
  }
  തർക്കം next_range (hb_codepoint_t *first, hb_codepoint_t *last) സ്ഥിരം
  {
    എങ്കിൽ (likely (!inverted))
      തിരികെ s.next_range (first, last);

    എങ്കിൽ (!next (last))
    {
      *last = *first = INVALID;
      തിരികെ അസത്യം;
    }

    *first = *last;
    s.next (last);
    --*last;
    തിരികെ സത്യം;
  }
  തർക്കം previous_range (hb_codepoint_t *first, hb_codepoint_t *last) സ്ഥിരം
  {
    എങ്കിൽ (likely (!inverted))
      തിരികെ s.previous_range (first, last);

    എങ്കിൽ (!previous (first))
    {
      *last = *first = INVALID;
      തിരികെ അസത്യം;
    }

    *last = *first;
    s.previous (first);
    ++*first;
    തിരികെ സത്യം;
  }

  unsigned പൂർണ്ണസംഖ്യ next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned പൂർണ്ണസംഖ്യ    size) സ്ഥിരം
  {
    തിരികെ inverted ? s.next_many_inverted (codepoint, out, size)
		    : s.next_many (codepoint, out, size);
  }

  സ്ഥിതം constexpr hb_codepoint_t INVALID = hb_bit_set_t::INVALID;

  /*
   * Iterator implementation.
   * Mutating the set invalidates its iterators.
   */
  ഘടന iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    സ്ഥിതം constexpr തർക്കം is_sorted_iterator = സത്യം;
    സ്ഥിതം constexpr തർക്കം has_fast_len = സത്യം;
    iter_t (സ്ഥിരം hb_bit_set_invertible_t &s_ = Null (hb_bit_set_invertible_t),
    തർക്കം init = സത്യം) : s (&s_), v (INVALID), remaining (0),
				     have_len (!init), base (INVALID), bits (0)
    {
      എങ്കിൽ (init)
	advance ();
    }

    തരനിർവചനം hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () സ്ഥിരം { തിരികെ v; }
    തർക്കം __more__ () സ്ഥിരം { തിരികെ v != INVALID; }
    ശൂന്യം __next__ ()
    {
      remaining--;
      advance ();
    }
    ശൂന്യം __prev__ ()
    {
      എങ്കിൽ (s->previous (&v))
      {
	സമന്വയം ();
	remaining++;
      }
      അല്ലെങ്കിൽ
      {
	base = INVALID;
	bits = 0;
      }
    }
    unsigned __len__ () സ്ഥിരം
    {
      എങ്കിൽ (unlikely (!have_len))
      {
	remaining += s->get_population ();
	have_len = സത്യം;
      }
      തിരികെ remaining;
    }
    iter_t end () സ്ഥിരം { തിരികെ iter_t (*s, അസത്യം); }
    തർക്കം operator != (സ്ഥിരം iter_t& o) സ്ഥിരം
    { തിരികെ v != o.v; }

    സംരക്ഷിത:
    സ്ഥിരം hb_bit_set_invertible_t *s;
    hb_codepoint_t v;
    mutable unsigned remaining;
    mutable തർക്കം have_len;
    hb_codepoint_t base;
    uint64_t bits;

    ശൂന്യം advance ()
    {
      എങ്കിൽ (likely (bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (s->next_bits (&base, &bits))
      {
	v = base + hb_ctz (bits);
	bits &= bits - 1;
      }
      അല്ലെങ്കിൽ
	v = INVALID;
    }

    ശൂന്യം സമന്വയം ()
    {
      base = v & ~hb_bit_page_t::ELT_MASK;
      hb_codepoint_t cursor = base ? base - hb_bit_page_t::ELT_BITS : INVALID;
      s->next_bits (&cursor, &bits);
      unsigned പൂർണ്ണസംഖ്യ bit = v & (hb_bit_page_t::ELT_BITS - 1);
      bits = bit == hb_bit_page_t::ELT_BITS - 1 ? 0 : bits >> (bit + 1) << (bit + 1);
    }
  };
  iter_t iter () സ്ഥിരം { തിരികെ iter_t (*ഇത്); }
  operator iter_t () സ്ഥിരം { തിരികെ iter (); }
};


#endif /* HB_BIT_SET_INVERTIBLE_HH */
