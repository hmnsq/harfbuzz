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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-bit-set-invertible.hh"
#অন্তর্ভাব "hb-bit-vector.hh" // Just to include


template <typename impl_t>
কাঠামো hb_sparseset_t
{
  স্থির constexpr যৌক্তিক realloc_move = সত্য;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ধ্রুবক hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (ধ্রুবক hb_sparseset_t& other) { set (other); ফেরত *এই; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); ফেরত *এই; }
  friend শূন্য swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    জন্য (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ধ্রুবক Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *এই);
  }

  শূন্য init ()
  {
    hb_object_init (এই);
    s.init ();
  }
  শূন্য fini ()
  {
    hb_object_fini (এই);
    s.fini ();
  }

  explicit operator যৌক্তিক () ধ্রুবক { ফেরত !is_empty (); }

  শূন্য err () { s.err (); }
  যৌক্তিক in_error () ধ্রুবক { ফেরত s.in_error (); }

  শূন্য alloc (unsigned sz) { s.alloc (sz); }
  শূন্য reset () { s.reset (); }
  শূন্য clear () { s.clear (); }
  শূন্য invert () { s.invert (); }
  যৌক্তিক is_inverted () ধ্রুবক { ফেরত s.is_inverted (); }
  যৌক্তিক is_empty () ধ্রুবক { ফেরত s.is_empty (); }
  uint32_t hash () ধ্রুবক { ফেরত s.hash (); }

  শূন্য add (hb_codepoint_t g) { s.add (g); }
  শূন্য add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  যৌক্তিক add_range (hb_codepoint_t first, hb_codepoint_t last) { ফেরত s.add_range (first, last); }

  template <typename T>
  শূন্য add_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  { s.add_array (array, গুনতি, stride); }
  template <typename T>
  শূন্য add_array (ধ্রুবক hb_array_t<ধ্রুবক T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  যৌক্তিক add_sorted_array (ধ্রুবক T *array, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride=আকার(T))
  { ফেরত s.add_sorted_array (array, গুনতি, stride); }
  template <typename T>
  যৌক্তিক add_sorted_array (ধ্রুবক hb_sorted_array_t<ধ্রুবক T>& arr) { ফেরত add_sorted_array (&arr, arr.len ()); }

  শূন্য del (hb_codepoint_t g) { s.del (g); }
  শূন্য del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  যৌক্তিক get (hb_codepoint_t g) ধ্রুবক { ফেরত s.get (g); }
  যৌক্তিক may_have (hb_codepoint_t g) ধ্রুবক { ফেরত get (g); }

  /* Has interface. */
  যৌক্তিক operator [] (hb_codepoint_t k) ধ্রুবক { ফেরত get (k); }
  যৌক্তিক has (hb_codepoint_t k) ধ্রুবক { ফেরত (*এই)[k]; }

  /* Predicate. */
  যৌক্তিক operator () (hb_codepoint_t k) ধ্রুবক { ফেরত has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); ফেরত *এই; }
  hb_sparseset_t& operator << (ধ্রুবক hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ফেরত *এই; }

  যৌক্তিক may_intersect (ধ্রুবক hb_sparseset_t &other) ধ্রুবক
  { ফেরত s.may_intersect (other.s); }

  যৌক্তিক intersects (ধ্রুবক hb_sparseset_t &other) ধ্রুবক
  { ফেরত s.intersects (other.s); }

  যৌক্তিক intersects (hb_codepoint_t first, hb_codepoint_t last) ধ্রুবক
  { ফেরত s.intersects (first, last); }

  শূন্য set (ধ্রুবক hb_sparseset_t &other) { s.set (other.s); }

  যৌক্তিক is_equal (ধ্রুবক hb_sparseset_t &other) ধ্রুবক { ফেরত s.is_equal (other.s); }
  যৌক্তিক operator == (ধ্রুবক hb_set_t &other) ধ্রুবক { ফেরত is_equal (other); }
  যৌক্তিক operator != (ধ্রুবক hb_set_t &other) ধ্রুবক { ফেরত !is_equal (other); }

  যৌক্তিক is_subset (ধ্রুবক hb_sparseset_t &larger_set) ধ্রুবক { ফেরত s.is_subset (larger_set.s); }

  শূন্য union_ (ধ্রুবক hb_sparseset_t &other) { s.union_ (other.s); }
  শূন্য intersect (ধ্রুবক hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  শূন্য subtract (ধ্রুবক hb_sparseset_t &other) { s.subtract (other.s); }
  শূন্য symmetric_difference (ধ্রুবক hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  যৌক্তিক next (hb_codepoint_t *codepoint) ধ্রুবক { ফেরত s.next (codepoint); }
  যৌক্তিক পরবর্তী_bits (hb_codepoint_t *codepoint, uint64_t *bits) ধ্রুবক
  { ফেরত s.পরবর্তী_bits (codepoint, bits); }
  যৌক্তিক previous (hb_codepoint_t *codepoint) ধ্রুবক { ফেরত s.previous (codepoint); }
  যৌক্তিক পরবর্তী_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুবক
  { ফেরত s.পরবর্তী_range (first, last); }
  যৌক্তিক previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুবক
  { ফেরত s.previous_range (first, last); }
  unsigned পূর্ণসংখ্যা পরবর্তী_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned পূর্ণসংখ্যা আকার_2) ধ্রুবক
  { ফেরত s.পরবর্তী_many (codepoint, out, আকার_2); }

  unsigned পূর্ণসংখ্যা get_population () ধ্রুবক { ফেরত s.get_population (); }
  যৌক্তিক get_singleton (hb_codepoint_t *codepoint) ধ্রুবক { ফেরত s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ধ্রুবক { ফেরত s.get_min (); }
  hb_codepoint_t get_max () ধ্রুবক { ফেরত s.get_max (); }

  স্থির constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () ধ্রুবক { ফেরত iter_t (এই->s); }
  operator iter_t () ধ্রুবক { ফেরত iter (); }
};

কাঠামো hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = পূর্বনির্ধারিত;
  hb_set_t () : sparseset () {};
  hb_set_t (ধ্রুবক hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (ধ্রুবক hb_set_t&) = পূর্বনির্ধারিত;
  hb_set_t& operator = (hb_set_t&&) = পূর্বনির্ধারিত;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (ধ্রুবক Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); ফেরত *এই; }
  hb_set_t& operator << (ধ্রুবক hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); ফেরত *এই; }
};

static_assert (hb_set_t::INVALID == HB_SET_মান_INVALID, "");


#endif /* HB_SET_HH */
