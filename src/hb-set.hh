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

#include "hb.hh"
#include "hb-bit-set-invertible.hh"
#include "hb-bit-vector.hh" // Just to include


template <typename impl_t>
sa_nracanaa hb_sparseset_t
{
  static constexpr bool realloc_move = true;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (const hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (const hb_sparseset_t& other) { set (other); ཕྱིར *this; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); ཕྱིར *this; }
  friend void swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    ཆེད་དུ (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (const Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *this);
  }

  void init ()
  {
    hb_object_init (this);
    s.init ();
  }
  void fini ()
  {
    hb_object_fini (this);
    s.fini ();
  }

  explicit operator bool () const { ཕྱིར !is_empty (); }

  void err () { s.err (); }
  bool in_error () const { ཕྱིར s.in_error (); }

  void alloc (unsigned sz) { s.alloc (sz); }
  void reset () { s.reset (); }
  void clear () { s.clear (); }
  void invert () { s.invert (); }
  bool is_inverted () const { ཕྱིར s.is_inverted (); }
  bool is_empty () const { ཕྱིར s.is_empty (); }
  uint32_t hash () const { ཕྱིར s.hash (); }

  void add (hb_codepoint_t g) { s.add (g); }
  void add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  bool add_range (hb_codepoint_t first, hb_codepoint_t last) { ཕྱིར s.add_range (first, last); }

  template <typename T>
  void add_array (const T *array, unsigned ཧྲིལ count, unsigned ཧྲིལ stride=parimaa_na(T))
  { s.add_array (array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool add_sorted_array (const T *array, unsigned ཧྲིལ count, unsigned ཧྲིལ stride=parimaa_na(T))
  { ཕྱིར s.add_sorted_array (array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { ཕྱིར add_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g) { s.del (g); }
  void del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  bool get (hb_codepoint_t g) const { ཕྱིར s.get (g); }
  bool may_have (hb_codepoint_t g) const { ཕྱིར get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { ཕྱིར get (k); }
  bool has (hb_codepoint_t k) const { ཕྱིར (*this)[k]; }

  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { ཕྱིར has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); ཕྱིར *this; }
  hb_sparseset_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ཕྱིར *this; }

  bool may_intersect (const hb_sparseset_t &other) const
  { ཕྱིར s.may_intersect (other.s); }

  bool intersects (const hb_sparseset_t &other) const
  { ཕྱིར s.intersects (other.s); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  { ཕྱིར s.intersects (first, last); }

  void set (const hb_sparseset_t &other) { s.set (other.s); }

  bool is_equal (const hb_sparseset_t &other) const { ཕྱིར s.is_equal (other.s); }
  bool operator == (const hb_set_t &other) const { ཕྱིར is_equal (other); }
  bool operator != (const hb_set_t &other) const { ཕྱིར !is_equal (other); }

  bool is_subset (const hb_sparseset_t &larger_set) const { ཕྱིར s.is_subset (larger_set.s); }

  void union_ (const hb_sparseset_t &other) { s.union_ (other.s); }
  void intersect (const hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  void subtract (const hb_sparseset_t &other) { s.subtract (other.s); }
  void symmetric_difference (const hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  bool next (hb_codepoint_t *codepoint) const { ཕྱིར s.next (codepoint); }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  { ཕྱིར s.next_bits (codepoint, bits); }
  bool previous (hb_codepoint_t *codepoint) const { ཕྱིར s.previous (codepoint); }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  { ཕྱིར s.next_range (first, last); }
  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  { ཕྱིར s.previous_range (first, last); }
  unsigned ཧྲིལ next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned ཧྲིལ size) const
  { ཕྱིར s.next_many (codepoint, out, size); }

  unsigned ཧྲིལ get_population () const { ཕྱིར s.get_population (); }
  bool get_singleton (hb_codepoint_t *codepoint) const { ཕྱིར s.get_singleton (codepoint); }
  hb_codepoint_t get_min () const { ཕྱིར s.get_min (); }
  hb_codepoint_t get_max () const { ཕྱིར s.get_max (); }

  static constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () const { ཕྱིར iter_t (this->s); }
  operator iter_t () const { ཕྱིར iter (); }
};

sa_nracanaa hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = default;
  hb_set_t () : sparseset () {};
  hb_set_t (const hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (const hb_set_t&) = default;
  hb_set_t& operator = (hb_set_t&&) = default;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (const Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); ཕྱིར *this; }
  hb_set_t& operator << (const hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); ཕྱིར *this; }
};

static_assert (hb_set_t::INVALID == HB_SET_VALUE_INVALID, "");


#endif /* HB_SET_HH */
