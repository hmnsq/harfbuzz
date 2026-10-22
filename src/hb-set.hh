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

#अगर_अघोषित HB_SET_HH
#घोषणा HB_SET_HH

#समावेश "hb.hh"
#समावेश "hb-bit-set-invertible.hh"
#समावेश "hb-bit-vector.hh" // Just to include


ढाँचा <typename impl_t>
काष्ठा hb_sparseset_t
{
  अटल constexpr bool realloc_move = true;

  hb_object_header_t header;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (स्थिर hb_sparseset_t& other) : hb_sparseset_t () { set (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& चालक = (स्थिर hb_sparseset_t& other) { set (other); वापस *यह; }
  hb_sparseset_t& चालक = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); वापस *यह; }
  मित्र व्योम swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_swap (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    क्रम (स्वतः&& item : lst)
      add (item);
  }
  ढाँचा <typename Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (स्थिर Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *यह);
  }

  व्योम init ()
  {
    hb_object_init (यह);
    s.init ();
  }
  व्योम fini ()
  {
    hb_object_fini (यह);
    s.fini ();
  }

  explicit चालक bool () स्थिर { वापस !is_empty (); }

  व्योम err () { s.err (); }
  bool in_error () स्थिर { वापस s.in_error (); }

  व्योम alloc (अचिन्हित sz) { s.alloc (sz); }
  व्योम reset () { s.reset (); }
  व्योम सफा () { s.सफा (); }
  व्योम invert () { s.invert (); }
  bool is_inverted () स्थिर { वापस s.is_inverted (); }
  bool is_empty () स्थिर { वापस s.is_empty (); }
  uint32_t hash () स्थिर { वापस s.hash (); }

  व्योम add (hb_codepoint_t g) { s.add (g); }
  व्योम add_bits (hb_codepoint_t g, uint64_t bits) { s.add_bits (g, bits); }
  bool add_range (hb_codepoint_t first, hb_codepoint_t last) { वापस s.add_range (first, last); }

  ढाँचा <typename T>
  व्योम add_array (स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  { s.add_array (array, गिनती, stride); }
  ढाँचा <typename T>
  व्योम add_array (स्थिर hb_array_t<स्थिर T>& arr) { add_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename T>
  bool add_sorted_array (स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  { वापस s.add_sorted_array (array, गिनती, stride); }
  ढाँचा <typename T>
  bool add_sorted_array (स्थिर hb_sorted_array_t<स्थिर T>& arr) { वापस add_sorted_array (&arr, arr.len ()); }

  व्योम del (hb_codepoint_t g) { s.del (g); }
  व्योम del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  bool लो (hb_codepoint_t g) स्थिर { वापस s.लो (g); }
  bool may_have (hb_codepoint_t g) स्थिर { वापस लो (g); }

  /* Has interface. */
  bool चालक [] (hb_codepoint_t k) स्थिर { वापस लो (k); }
  bool has (hb_codepoint_t k) स्थिर { वापस (*यह)[k]; }

  /* Predicate. */
  bool चालक () (hb_codepoint_t k) स्थिर { वापस has (k); }

  /* Sink interface. */
  hb_sparseset_t& चालक << (hb_codepoint_t v)
  { add (v); वापस *यह; }
  hb_sparseset_t& चालक << (स्थिर hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); वापस *यह; }

  bool may_intersect (स्थिर hb_sparseset_t &other) स्थिर
  { वापस s.may_intersect (other.s); }

  bool intersects (स्थिर hb_sparseset_t &other) स्थिर
  { वापस s.intersects (other.s); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) स्थिर
  { वापस s.intersects (first, last); }

  व्योम set (स्थिर hb_sparseset_t &other) { s.set (other.s); }

  bool is_equal (स्थिर hb_sparseset_t &other) स्थिर { वापस s.is_equal (other.s); }
  bool चालक == (स्थिर hb_set_t &other) स्थिर { वापस is_equal (other); }
  bool चालक != (स्थिर hb_set_t &other) स्थिर { वापस !is_equal (other); }

  bool is_subset (स्थिर hb_sparseset_t &larger_set) स्थिर { वापस s.is_subset (larger_set.s); }

  व्योम union_ (स्थिर hb_sparseset_t &other) { s.union_ (other.s); }
  व्योम intersect (स्थिर hb_sparseset_t &other,
		  hb_vector_t<अचिन्हित> *workspace = nullptr)
  { s.intersect (other.s, workspace); }
  व्योम subtract (स्थिर hb_sparseset_t &other) { s.subtract (other.s); }
  व्योम symmetric_difference (स्थिर hb_sparseset_t &other) { s.symmetric_difference (other.s); }

  bool next (hb_codepoint_t *codepoint) स्थिर { वापस s.next (codepoint); }
  bool अग्र_bits (hb_codepoint_t *codepoint, uint64_t *bits) स्थिर
  { वापस s.अग्र_bits (codepoint, bits); }
  bool previous (hb_codepoint_t *codepoint) स्थिर { वापस s.previous (codepoint); }
  bool अग्र_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिर
  { वापस s.अग्र_range (first, last); }
  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिर
  { वापस s.previous_range (first, last); }
  अचिन्हित पूर्णांक अग्र_many (hb_codepoint_t codepoint, hb_codepoint_t *out, अचिन्हित पूर्णांक आकार) स्थिर
  { वापस s.अग्र_many (codepoint, out, आकार); }

  अचिन्हित पूर्णांक get_population () स्थिर { वापस s.get_population (); }
  bool get_singleton (hb_codepoint_t *codepoint) स्थिर { वापस s.get_singleton (codepoint); }
  hb_codepoint_t get_min () स्थिर { वापस s.get_min (); }
  hb_codepoint_t get_max () स्थिर { वापस s.get_max (); }

  अटल constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  using iter_t = typename impl_t::iter_t;
  iter_t iter () स्थिर { वापस iter_t (यह->s); }
  चालक iter_t () स्थिर { वापस iter (); }
};

काष्ठा hb_set_t : hb_sparseset_t<hb_bit_set_invertible_t>
{
  using sparseset = hb_sparseset_t<hb_bit_set_invertible_t>;

  ~hb_set_t () = शेष;
  hb_set_t () : sparseset () {};
  hb_set_t (स्थिर hb_set_t &o) : sparseset ((sparseset &) o) {};
  hb_set_t (hb_set_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_set_t& चालक = (स्थिर hb_set_t&) = शेष;
  hb_set_t& चालक = (hb_set_t&&) = शेष;
  hb_set_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  ढाँचा <typename Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_set_t (स्थिर Iterable &o) : sparseset (o) {}

  hb_set_t& चालक << (hb_codepoint_t v)
  { sparseset::चालक<< (v); वापस *यह; }
  hb_set_t& चालक << (स्थिर hb_codepoint_pair_t& range)
  { sparseset::चालक<< (range); वापस *यह; }
};

static_assert (hb_set_t::INVALID == HB_SET_मूल्य_INVALID, "");


#पूर्ण_अगर /* HB_SET_HH */
