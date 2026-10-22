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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਸੈੱਟ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਸੈੱਟ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bit-set-invertible.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-bit-vector.hh" // Just to include


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ impl_t>
ਬਣਤਰ hb_sparseset_t
{
  ਸਥਿਰ constexpr ਤਾਰਕਿਕ realloc_move = ਸੱਚ;

  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;
  impl_t s;

  hb_sparseset_t () { init (); }
  ~hb_sparseset_t () { fini (); }

  hb_sparseset_t (ਅਚਲ hb_sparseset_t& other) : hb_sparseset_t () { ਸੈੱਟ (other); }
  hb_sparseset_t (hb_sparseset_t&& other)  noexcept : hb_sparseset_t () { s = std::move (other.s); }
  hb_sparseset_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_sparseset_t& other) { ਸੈੱਟ (other); ਵਾਪਸ *ਇਹ; }
  hb_sparseset_t& ਸੰਚਾਲਕ = (hb_sparseset_t&& other)  noexcept { s = std::move (other.s); ਵਾਪਸ *ਇਹ; }
  ਮਿੱਤਰ ਖਾਲੀ swap (hb_sparseset_t& a, hb_sparseset_t& b)  noexcept { hb_ਅਦਲਾ_ਬਦਲੀ (a.s, b.s); }

  hb_sparseset_t (std::initializer_list<hb_codepoint_t> lst) : hb_sparseset_t ()
  {
    ਲਈ (ਸਵੈਚਾਲਿਤ&& ਵਸਤੂ : lst)
      ਜੋੜੋ (ਵਸਤੂ);
  }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
           hb_requires (hb_is_iterable (Iterable))>
  hb_sparseset_t (ਅਚਲ Iterable &o) : hb_sparseset_t ()
  {
    hb_copy (o, *ਇਹ);
  }

  ਖਾਲੀ init ()
  {
    hb_object_init (ਇਹ);
    s.init ();
  }
  ਖਾਲੀ fini ()
  {
    hb_object_fini (ਇਹ);
    s.fini ();
  }

  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ !is_empty (); }

  ਖਾਲੀ err () { s.err (); }
  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ s.ਵਿੱਚ_error (); }

  ਖਾਲੀ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ sz) { s.alloc (sz); }
  ਖਾਲੀ reset () { s.reset (); }
  ਖਾਲੀ clear () { s.clear (); }
  ਖਾਲੀ invert () { s.invert (); }
  ਤਾਰਕਿਕ is_inverted () ਅਚਲ { ਵਾਪਸ s.is_inverted (); }
  ਤਾਰਕਿਕ is_empty () ਅਚਲ { ਵਾਪਸ s.is_empty (); }
  uint32_t hash () ਅਚਲ { ਵਾਪਸ s.hash (); }

  ਖਾਲੀ ਜੋੜੋ (hb_codepoint_t g) { s.ਜੋੜੋ (g); }
  ਖਾਲੀ ਜੋੜੋ_ਬਿੱਟ (hb_codepoint_t g, uint64_t ਬਿੱਟ) { s.ਜੋੜੋ_ਬਿੱਟ (g, ਬਿੱਟ); }
  ਤਾਰਕਿਕ ਜੋੜੋ_range (hb_codepoint_t ਪਹਿਲਾ, hb_codepoint_t ਆਖਰੀ) { ਵਾਪਸ s.ਜੋੜੋ_range (ਪਹਿਲਾ, ਆਖਰੀ); }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਜੋੜੋ_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  { s.ਜੋੜੋ_array (array, ਗਿਣਤੀ_2, stride); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਖਾਲੀ ਜੋੜੋ_array (ਅਚਲ hb_array_t<ਅਚਲ T>& arr) { ਜੋੜੋ_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਜੋੜੋ_sorted_array (ਅਚਲ T *array, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride=ਆਕਾਰ(T))
  { ਵਾਪਸ s.ਜੋੜੋ_sorted_array (array, ਗਿਣਤੀ_2, stride); }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  ਤਾਰਕਿਕ ਜੋੜੋ_sorted_array (ਅਚਲ hb_sorted_array_t<ਅਚਲ T>& arr) { ਵਾਪਸ ਜੋੜੋ_sorted_array (&arr, arr.len ()); }

  ਖਾਲੀ del (hb_codepoint_t g) { s.del (g); }
  ਖਾਲੀ del_range (hb_codepoint_t a, hb_codepoint_t b) { s.del_range (a, b); }

  ਤਾਰਕਿਕ get (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ s.get (g); }
  ਤਾਰਕਿਕ may_have (hb_codepoint_t g) ਅਚਲ { ਵਾਪਸ get (g); }

  /* Has interface. */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ [] (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ get (k); }
  ਤਾਰਕਿਕ has (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ (*ਇਹ)[k]; }

  /* Predicate. */
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ () (hb_codepoint_t k) ਅਚਲ { ਵਾਪਸ has (k); }

  /* Sink interface. */
  hb_sparseset_t& ਸੰਚਾਲਕ << (hb_codepoint_t v)
  { ਜੋੜੋ (v); ਵਾਪਸ *ਇਹ; }
  hb_sparseset_t& ਸੰਚਾਲਕ << (ਅਚਲ hb_codepoint_pair_t& range)
  { ਜੋੜੋ_range (range.ਪਹਿਲਾ, range.second); ਵਾਪਸ *ਇਹ; }

  ਤਾਰਕਿਕ may_intersect (ਅਚਲ hb_sparseset_t &other) ਅਚਲ
  { ਵਾਪਸ s.may_intersect (other.s); }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_sparseset_t &other) ਅਚਲ
  { ਵਾਪਸ s.intersects (other.s); }

  ਤਾਰਕਿਕ intersects (hb_codepoint_t ਪਹਿਲਾ, hb_codepoint_t ਆਖਰੀ) ਅਚਲ
  { ਵਾਪਸ s.intersects (ਪਹਿਲਾ, ਆਖਰੀ); }

  ਖਾਲੀ ਸੈੱਟ (ਅਚਲ hb_sparseset_t &other) { s.ਸੈੱਟ (other.s); }

  ਤਾਰਕਿਕ is_equal (ਅਚਲ hb_sparseset_t &other) ਅਚਲ { ਵਾਪਸ s.is_equal (other.s); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ਸੈੱਟ_t &other) ਅਚਲ { ਵਾਪਸ is_equal (other); }
  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ਸੈੱਟ_t &other) ਅਚਲ { ਵਾਪਸ !is_equal (other); }

  ਤਾਰਕਿਕ is_subset (ਅਚਲ hb_sparseset_t &larger_ਸੈੱਟ) ਅਚਲ { ਵਾਪਸ s.is_subset (larger_ਸੈੱਟ.s); }

  ਖਾਲੀ union_ (ਅਚਲ hb_sparseset_t &other) { s.union_ (other.s); }
  ਖਾਲੀ intersect (ਅਚਲ hb_sparseset_t &other,
		  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> *workspace = ਖਾਲੀ_ਸੂਚਕ)
  { s.intersect (other.s, workspace); }
  ਖਾਲੀ ਘਟਾਓ (ਅਚਲ hb_sparseset_t &other) { s.ਘਟਾਓ (other.s); }
  ਖਾਲੀ ਸਮਮਿਤ_difference (ਅਚਲ hb_sparseset_t &other) { s.ਸਮਮਿਤ_difference (other.s); }

  ਤਾਰਕਿਕ next (hb_codepoint_t *codepoint) ਅਚਲ { ਵਾਪਸ s.next (codepoint); }
  ਤਾਰਕਿਕ ਅਗਲਾ_ਬਿੱਟ (hb_codepoint_t *codepoint, uint64_t *ਬਿੱਟ) ਅਚਲ
  { ਵਾਪਸ s.ਅਗਲਾ_ਬਿੱਟ (codepoint, ਬਿੱਟ); }
  ਤਾਰਕਿਕ ਪਿਛਲਾ (hb_codepoint_t *codepoint) ਅਚਲ { ਵਾਪਸ s.ਪਿਛਲਾ (codepoint); }
  ਤਾਰਕਿਕ ਅਗਲਾ_range (hb_codepoint_t *ਪਹਿਲਾ, hb_codepoint_t *ਆਖਰੀ) ਅਚਲ
  { ਵਾਪਸ s.ਅਗਲਾ_range (ਪਹਿਲਾ, ਆਖਰੀ); }
  ਤਾਰਕਿਕ ਪਿਛਲਾ_range (hb_codepoint_t *ਪਹਿਲਾ, hb_codepoint_t *ਆਖਰੀ) ਅਚਲ
  { ਵਾਪਸ s.ਪਿਛਲਾ_range (ਪਹਿਲਾ, ਆਖਰੀ); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਗਲਾ_many (hb_codepoint_t codepoint, hb_codepoint_t *ਬਾਹਰਲਾ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ) ਅਚਲ
  { ਵਾਪਸ s.ਅਗਲਾ_many (codepoint, ਬਾਹਰਲਾ, ਮਾਪ); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_population () ਅਚਲ { ਵਾਪਸ s.get_population (); }
  ਤਾਰਕਿਕ get_singleton (hb_codepoint_t *codepoint) ਅਚਲ { ਵਾਪਸ s.get_singleton (codepoint); }
  hb_codepoint_t get_min () ਅਚਲ { ਵਾਪਸ s.get_min (); }
  hb_codepoint_t get_max () ਅਚਲ { ਵਾਪਸ s.get_max (); }

  ਸਥਿਰ constexpr hb_codepoint_t INVALID = impl_t::INVALID;

  /*
   * Iterator implementation.
   */
  ਵਰਤੋਂ iter_t = ਕਿਸਮ_ਨਾਂ impl_t::iter_t;
  iter_t iter () ਅਚਲ { ਵਾਪਸ iter_t (ਇਹ->s); }
  ਸੰਚਾਲਕ iter_t () ਅਚਲ { ਵਾਪਸ iter (); }
};

ਬਣਤਰ hb_ਸੈੱਟ_t : hb_sparseset_t<hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t>
{
  ਵਰਤੋਂ sparseset = hb_sparseset_t<hb_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ_invertible_t>;

  ~hb_ਸੈੱਟ_t () = ਮੂਲ;
  hb_ਸੈੱਟ_t () : sparseset () {};
  hb_ਸੈੱਟ_t (ਅਚਲ hb_ਸੈੱਟ_t &o) : sparseset ((sparseset &) o) {};
  hb_ਸੈੱਟ_t (hb_ਸੈੱਟ_t&& o)  noexcept : sparseset (std::move ((sparseset &) o)) {}
  hb_ਸੈੱਟ_t& ਸੰਚਾਲਕ = (ਅਚਲ hb_ਸੈੱਟ_t&) = ਮੂਲ;
  hb_ਸੈੱਟ_t& ਸੰਚਾਲਕ = (hb_ਸੈੱਟ_t&&) = ਮੂਲ;
  hb_ਸੈੱਟ_t (std::initializer_list<hb_codepoint_t> lst) : sparseset (lst) {}
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterable,
	    hb_requires (hb_is_iterable (Iterable))>
  hb_ਸੈੱਟ_t (ਅਚਲ Iterable &o) : sparseset (o) {}

  hb_ਸੈੱਟ_t& ਸੰਚਾਲਕ << (hb_codepoint_t v)
  { sparseset::ਸੰਚਾਲਕ<< (v); ਵਾਪਸ *ਇਹ; }
  hb_ਸੈੱਟ_t& ਸੰਚਾਲਕ << (ਅਚਲ hb_codepoint_pair_t& range)
  { sparseset::ਸੰਚਾਲਕ<< (range); ਵਾਪਸ *ਇਹ; }
};

static_assert (hb_ਸੈੱਟ_t::INVALID == HB_ਸੈੱਟ_ਮੁੱਲ_INVALID, "");


#ਜੇ_ਅੰਤ /* HB_SET_HH */
