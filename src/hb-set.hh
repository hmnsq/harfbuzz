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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-bit-set-invertible.hh"
#അന്തര്ഭാവ "hb-bit-vector.hh" // Just to include


template <typename impl_t>
ഘടന hb_sparseset_t
{
  സ്ഥിതം constexpr തർക്കം realloc_move = സത്യം;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (സ്ഥിരം hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& operator = (സ്ഥിരം hb_sparseset_t& other) { set (other); തിരികെ *ഇത്; }
  hb_sparseset_t& operator = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); തിരികെ *ഇത്; }
  friend ശൂന്യം swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    വേണ്ടി (auto&& item : lst)
      add (item);
  }
  template <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (സ്ഥിരം Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *ഇത്);
  }

  ശൂന്യം init ()
  {
    hb_object_init (ഇത്);
    s.init ();
  }
  ശൂന്യം fini ()
  {
    hb_object_fini (ഇത്);
    s.fini ();
  }

  explicit operator തർക്കം () സ്ഥിരം { തിരികെ !is_empty (); }

  ശൂന്യം err () { s.err (); }
  തർക്കം in_error () സ്ഥിരം { തിരികെ s.in_error (); }

  ശൂന്യം alloc (unsigned sz) { s.alloc (sz); }
  ശൂന്യം reset () { s.reset (); }
  ശൂന്യം clear () { s.clear (); }
  ശൂന്യം invert () { s.invert (); }
  തർക്കം is_inverted () സ്ഥിരം { തിരികെ s.is_inverted (); }
  തർക്കം is_empty () സ്ഥിരം { തിരികെ s.is_empty (); }
  uint32_t hash () സ്ഥിരം { തിരികെ s.hash (); }

  ശൂന്യം add (hb_codepoint_t g) { s.add (g); }
  ശൂന്യം add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  തർക്കം add_range (hb_codepoint_t first, hb_codepoint_t last) { തിരികെ s.add_range (first, last); }

  template <typename T>
  ശൂന്യം add_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  { s.add_array (array, എണ്ണം, stride); }
  template <typename T>
  ശൂന്യം add_array (സ്ഥിരം hb_array_t<സ്ഥിരം T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  തർക്കം add_sorted_array (സ്ഥിരം T *array, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride=വലുപ്പം(T))
  { തിരികെ s.add_sorted_array (array, എണ്ണം, stride); }
  template <typename T>
  തർക്കം add_sorted_array (സ്ഥിരം hb_sorted_array_t<സ്ഥിരം T>& arr) { തിരികെ add_sorted_array (&arr, arr.len ()); }

  ശൂന്യം del (hb_codepoint_t g) { s.del (g); }
  ശൂന്യം del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  തർക്കം get (hb_codepoint_t g) സ്ഥിരം { തിരികെ s.get (g); }
  തർക്കം may_have (hb_codepoint_t g) സ്ഥിരം { തിരികെ get (g); }

  /* Has interface. */
  തർക്കം operator [] (hb_codepoint_t k) സ്ഥിരം { തിരികെ get (k); }
  തർക്കം has (hb_codepoint_t k) സ്ഥിരം { തിരികെ (*ഇത്)[k]; }

  /* Predicate. */
  തർക്കം operator () (hb_codepoint_t k) സ്ഥിരം { തിരികെ has (k); }

  /* Sink interface. */
  hb_sparseset_t& operator << (hb_codepoint_t v)
  { add (v); തിരികെ *ഇത്; }
  hb_sparseset_t& operator << (സ്ഥിരം hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); തിരികെ *ഇത്; }

  തർക്കം may_intersect (സ്ഥിരം hb_sparseset_t &other) സ്ഥിരം
  { തിരികെ s.may_intersect (other.s); }

  തർക്കം intersects (സ്ഥിരം hb_sparseset_t &other) സ്ഥിരം
  { തിരികെ s.intersects (other.s); }

  തർക്കം intersects (hb_codepoint_t first, hb_codepoint_t last) സ്ഥിരം
  { തിരികെ s.intersects (first, last); }

  ശൂന്യം set (സ്ഥിരം hb_sparseset_t &other) { s.set (other.s); }

  തർക്കം is_equal (സ്ഥിരം hb_sparseset_t &other) സ്ഥിരം { തിരികെ s.is_equal (other.s); }
  തർക്കം operator == (സ്ഥിരം hb_set_t &other) സ്ഥിരം { തിരികെ is_equal (other); }
  തർക്കം operator != (സ്ഥിരം hb_set_t &other) സ്ഥിരം { തിരികെ !is_equal (other); }

  തർക്കം is_subset (സ്ഥിരം hb_sparseset_t &larger_set) സ്ഥിരം { തിരികെ s.is_subset (larger_set.s); }

  ശൂന്യം union_ (സ്ഥിരം hb_sparseset_t &other) { s.union_ (other.s); }
  ശൂന്യം intersect (സ്ഥിരം hb_sparseset_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  ശൂന്യം subtract (സ്ഥിരം hb_sparseset_t &other) { s.subtract (other.s); }
  ശൂന്യം symmetric_difference (സ്ഥിരം hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  തർക്കം next (hb_codepoint_t *codepoint) സ്ഥിരം { തിരികെ s.next (codepoint); }
  തർക്കം next_bits (hb_codepoint_t *codepoint, uint64_t *bits) സ്ഥിരം
  { തിരികെ s.next_bits (codepoint, bits); }
  തർക്കം previous (hb_codepoint_t *codepoint) സ്ഥിരം { തിരികെ s.previous (codepoint); }
  തർക്കം next_range (hb_codepoint_t *first, hb_codepoint_t *last) സ്ഥിരം
  { തിരികെ s.next_range (first, last); }
  തർക്കം previous_range (hb_codepoint_t *first, hb_codepoint_t *last) സ്ഥിരം
  { തിരികെ s.previous_range (first, last); }
  unsigned പൂർണ്ണസംഖ്യ next_many (hb_codepoint_t codepoint, hb_codepoint_t *out, unsigned പൂർണ്ണസംഖ്യ size) സ്ഥിരം
  { തിരികെ s.next_many (codepoint, out, size); }

  unsigned പൂർണ്ണസംഖ്യ get_population () സ്ഥിരം { തിരികെ s.get_population (); }
  തർക്കം get_singleton (hb_codepoint_t *codepoint) സ്ഥിരം { തിരികെ s.get_singleton (codepoint); }
  hb_codepoint_t get_min () സ്ഥിരം { തിരികെ s.get_min (); }
  hb_codepoint_t get_max () സ്ഥിരം { തിരികെ s.get_max (); }

  സ്ഥിതം constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () സ്ഥിരം { തിരികെ iter_t (ഇത്->s); }
  operator iter_t () സ്ഥിരം { തിരികെ iter (); }
};

ഘടന hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = സ്ഥിരസ്ഥിതി;
  hb_set_t () : sparseset () {};
  hb_set_t (സ്ഥിരം hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& operator = (സ്ഥിരം hb_set_t&) = സ്ഥിരസ്ഥിതി;
  hb_set_t& operator = (hb_set_t&&) = സ്ഥിരസ്ഥിതി;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  template <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (സ്ഥിരം Iterable &o) : sparseset (o) {}

  hb_set_t& operator << (hb_codepoint_t v)
  { sparseset::operator<< (v); തിരികെ *ഇത്; }
  hb_set_t& operator << (സ്ഥിരം hb_codepoint_pair_t& range)
  { sparseset::operator<< (range); തിരികെ *ഇത്; }
};

static_assert (hb_set_t::INVALID == HB_SET_മൂല്യം_INVALID, "");


#endif /* HB_SET_HH */
