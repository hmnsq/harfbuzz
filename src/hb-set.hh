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

#ifndef HB_SET_HH
#define HB_SET_HH

#ضمن "hb.hh"
#ضمن "hb-bit-set-invertible.hh"
#ضمن "hb-bit-vector.hh" // Just to include


template <typename impl_t>
بنية hb_sparseset_t
{
  ساكن constexpr منطقي realloc_move = true;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ثابت hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (ثابت hb_sparseset_t& other) { set (other); أرجع *هذا; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); أرجع *هذا; }
  friend فراغ swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    لكل (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ثابت Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *هذا);
  }

  فراغ init ()
  {
    hb_object_init (هذا);
    s.init ();
  }
  فراغ fini ()
  {
    hb_object_fini (هذا);
    s.fini ();
  }

  explicit operator منطقي () ثابت { أرجع !is_empty (); }

  فراغ err () { s.err (); }
  منطقي in_error () ثابت { أرجع s.in_error (); }

  فراغ alloc (unsigned sz) { s.alloc (sz); }
  فراغ reset () { s.reset (); }
  فراغ clear () { s.clear (); }
  فراغ invert () { s.invert (); }
  منطقي is_inverted () ثابت { أرجع s.is_inverted (); }
  منطقي is_empty () ثابت { أرجع s.is_empty (); }
  uint32_t hash () ثابت { أرجع s.hash (); }

  فراغ add (hb_codepoint_t g) { s.add (g); }
  فراغ add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  منطقي add_range (hb_codepoint_t first, hb_codepoint_t last) { أرجع s.add_range (first, last); }

  template <typename T>
  فراغ add_array (ثابت T *array, unsigned صحيح گنتی, unsigned صحيح stride=حجم(T))
  { s.add_array (array, گنتی, stride); }
  template <typename T>
  فراغ add_array (ثابت hb_array_t<ثابت T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  منطقي add_sorted_array (ثابت T *array, unsigned صحيح گنتی, unsigned صحيح stride=حجم(T))
  { أرجع s.add_sorted_array (array, گنتی, stride); }
  template <typename T>
  منطقي add_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { أرجع add_sorted_array (&arr, arr.len ()); }

  فراغ del (hb_codepoint_t g) { s.del (g); }
  فراغ del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  منطقي get (hb_codepoint_t g) ثابت { أرجع s.get (g); }
  منطقي may_have (hb_codepoint_t g) ثابت { أرجع get (g); }

  /* Has interface. */
  منطقي operator [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }

  /* Predicate. */
  منطقي operator () (hb_codepoint_t k) ثابت { أرجع has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); أرجع *هذا; }
  hb_sparseset_t& operator << (ثابت hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); أرجع *هذا; }

  منطقي may_intersect (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.may_intersect (other.s); }

  منطقي intersects (ثابت hb_sparseset_t &other) ثابت
  { أرجع s.intersects (other.s); }

  منطقي intersects (hb_codepoint_t first, hb_codepoint_t last) ثابت
  { أرجع s.intersects (first, last); }

  فراغ set (ثابت hb_sparseset_t &other) { s.set (other.s); }

  منطقي is_equal (ثابت hb_sparseset_t &other) ثابت { أرجع s.is_equal (other.s); }
  منطقي operator == (ثابت hb_set_t &other) ثابت { أرجع is_equal (other); }
  منطقي operator != (ثابت hb_set_t &other) ثابت { أرجع !is_equal (other); }

  منطقي is_subset (ثابت hb_sparseset_t &larger_set) ثابت { أرجع s.is_subset (larger_set.s); }

  فراغ union_ (ثابت hb_sparseset_t &other) { s.union_ (other.s); }
  فراغ intersect (ثابت hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  فراغ subtract (ثابت hb_sparseset_t &other) { s.subtract (other.s); }
  فراغ symmetric_difference (ثابت hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  منطقي next (hb_codepoint_t *codepoint) ثابت { أرجع s.next (codepoint); }
  منطقي next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ثابت
  { أرجع s.next_bits (codepoint, bits); }
  منطقي previous (hb_codepoint_t *codepoint) ثابت { أرجع s.previous (codepoint); }
  منطقي next_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  { أرجع s.next_range (first, last); }
  منطقي previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  { أرجع s.previous_range (first, last); }
  unsigned صحيح next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned صحيح size) ثابت
  { أرجع s.next_many (codepoint, out, size); }

  unsigned صحيح get_population () ثابت { أرجع s.get_population (); }
  منطقي get_singleton (hb_codepoint_t *codepoint) ثابت { أرجع s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ثابت { أرجع s.get_min (); }
  hb_codepoint_t get_max () ثابت { أرجع s.get_max (); }

  ساكن constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () ثابت { أرجع iter_t (هذا->s); }
  operator iter_t () ثابت { أرجع iter (); }
};

بنية hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = افتراضي;
  hb_set_t () : sparseset () {};
  hb_set_t (ثابت hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (ثابت hb_set_t&) = افتراضي;
  hb_set_t& operator = (hb_set_t&&) = افتراضي;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (ثابت Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); أرجع *هذا; }
  hb_set_t& operator << (ثابت hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); أرجع *هذا; }
};

static_assert (hb_set_t::INVALID == HB_SET_قدر_INVALID, "");


#endif /* HB_SET_HH */
