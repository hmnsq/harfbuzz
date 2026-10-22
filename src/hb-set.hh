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

#شامل "hb.hh"
#شامل "hb-bit-set-invertible.hh"
#شامل "hb-bit-vector.hh" // Just to include


template <typename impl_t>
ساختار hb_sparseset_t
{
  ایستا constexpr منطقی realloc_move = درست;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ثابت hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (ثابت hb_sparseset_t& other) { set (other); بازگشت *این; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); بازگشت *این; }
  friend تهی swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    برای (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ثابت Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *این);
  }

  تهی init ()
  {
    hb_object_init (این);
    s.init ();
  }
  تهی fini ()
  {
    hb_object_fini (این);
    s.fini ();
  }

  explicit operator منطقی () ثابت { بازگشت !is_empty (); }

  تهی err () { s.err (); }
  منطقی in_error () ثابت { بازگشت s.in_error (); }

  تهی alloc (unsigned sz) { s.alloc (sz); }
  تهی reset () { s.reset (); }
  تهی clear () { s.clear (); }
  تهی invert () { s.invert (); }
  منطقی is_inverted () ثابت { بازگشت s.is_inverted (); }
  منطقی is_empty () ثابت { بازگشت s.is_empty (); }
  uint32_t hash () ثابت { بازگشت s.hash (); }

  تهی add (hb_codepoint_t g) { s.add (g); }
  تهی add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  منطقی add_range (hb_codepoint_t first, hb_codepoint_t last) { بازگشت s.add_range (first, last); }

  template <typename T>
  تهی add_array (ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  { s.add_array (array, شمار, stride); }
  template <typename T>
  تهی add_array (ثابت hb_array_t<ثابت T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  منطقی add_sorted_array (ثابت T *array, unsigned صحیح شمار, unsigned صحیح stride=اندازه(T))
  { بازگشت s.add_sorted_array (array, شمار, stride); }
  template <typename T>
  منطقی add_sorted_array (ثابت hb_sorted_array_t<ثابت T>& arr) { بازگشت add_sorted_array (&arr, arr.len ()); }

  تهی del (hb_codepoint_t g) { s.del (g); }
  تهی del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  منطقی get (hb_codepoint_t g) ثابت { بازگشت s.get (g); }
  منطقی may_have (hb_codepoint_t g) ثابت { بازگشت get (g); }

  /* Has interface. */
  منطقی operator [] (hb_codepoint_t k) ثابت { بازگشت get (k); }
  منطقی has (hb_codepoint_t k) ثابت { بازگشت (*این)[k]; }

  /* Predicate. */
  منطقی operator () (hb_codepoint_t k) ثابت { بازگشت has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); بازگشت *این; }
  hb_sparseset_t& operator << (ثابت hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); بازگشت *این; }

  منطقی may_intersect (ثابت hb_sparseset_t &other) ثابت
  { بازگشت s.may_intersect (other.s); }

  منطقی intersects (ثابت hb_sparseset_t &other) ثابت
  { بازگشت s.intersects (other.s); }

  منطقی intersects (hb_codepoint_t first, hb_codepoint_t last) ثابت
  { بازگشت s.intersects (first, last); }

  تهی set (ثابت hb_sparseset_t &other) { s.set (other.s); }

  منطقی is_equal (ثابت hb_sparseset_t &other) ثابت { بازگشت s.is_equal (other.s); }
  منطقی operator == (ثابت hb_set_t &other) ثابت { بازگشت is_equal (other); }
  منطقی operator != (ثابت hb_set_t &other) ثابت { بازگشت !is_equal (other); }

  منطقی is_subset (ثابت hb_sparseset_t &larger_set) ثابت { بازگشت s.is_subset (larger_set.s); }

  تهی union_ (ثابت hb_sparseset_t &other) { s.union_ (other.s); }
  تهی intersect (ثابت hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  تهی subtract (ثابت hb_sparseset_t &other) { s.subtract (other.s); }
  تهی symmetric_difference (ثابت hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  منطقی next (hb_codepoint_t *codepoint) ثابت { بازگشت s.next (codepoint); }
  منطقی next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ثابت
  { بازگشت s.next_bits (codepoint, bits); }
  منطقی previous (hb_codepoint_t *codepoint) ثابت { بازگشت s.previous (codepoint); }
  منطقی next_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  { بازگشت s.next_range (first, last); }
  منطقی previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ثابت
  { بازگشت s.previous_range (first, last); }
  unsigned صحیح next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned صحیح size) ثابت
  { بازگشت s.next_many (codepoint, out, size); }

  unsigned صحیح get_population () ثابت { بازگشت s.get_population (); }
  منطقی get_singleton (hb_codepoint_t *codepoint) ثابت { بازگشت s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ثابت { بازگشت s.get_min (); }
  hb_codepoint_t get_max () ثابت { بازگشت s.get_max (); }

  ایستا constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () ثابت { بازگشت iter_t (این->s); }
  operator iter_t () ثابت { بازگشت iter (); }
};

ساختار hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = پیشفرض;
  hb_set_t () : sparseset () {};
  hb_set_t (ثابت hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (ثابت hb_set_t&) = پیشفرض;
  hb_set_t& operator = (hb_set_t&&) = پیشفرض;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (ثابت Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); بازگشت *این; }
  hb_set_t& operator << (ثابت hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); بازگشت *این; }
};

static_assert (hb_set_t::INVALID == HB_SET_مقدار_INVALID, "");


#endif /* HB_SET_HH */
