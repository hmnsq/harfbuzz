/*
 * Copyright © 2020  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਤਰਜੀਹ_ਕਤਾਰ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਤਰਜੀਹ_ਕਤਾਰ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector.hh"

/*
 * hb_priority_queue_t
 *
 * Priority queue implemented as a binary heap. Supports extract minimum
 * and insert operations.
 *
 * The priority queue is implemented as a binary heap, which is a complete
 * binary tree. The root of the tree is the minimum element. The heap
 * property is that the priority of a node is less than or equal to the
 * priority of its children. The heap is stored in an array, with the
 * children of node i stored at indices 2i + 1 and 2i + 2.
 */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ K>
ਬਣਤਰ hb_ਤਰਜੀਹ_ਕਤਾਰ_t
{
 ਜਨਤਕ:
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ hb_pair_t<K, ਬਿਨਾਨਿਸ਼ਾਨੀ> ਵਸਤੂ_t;

 ਨਿਜੀ:
  hb_ਸਦਿਸ਼_t<ਵਸਤੂ_t> ਢੇਰ_2;

 ਜਨਤਕ:

  hb_ਤਰਜੀਹ_ਕਤਾਰ_t () = ਮੂਲ;
  hb_ਤਰਜੀਹ_ਕਤਾਰ_t (hb_ਸਦਿਸ਼_t<ਵਸਤੂ_t>&& other) : ਢੇਰ_2 (std::move (other))
  {
    // Heapify the vector.
    ਲਈ (ਪੂਰਨ i = (ਢੇਰ_2.ਲੰਮਾਈ / 2) - 1; i >= 0; i--)
      bubble_ਥੱਲੇ (i);
  }

  ਖਾਲੀ reset () { ਢੇਰ_2.clear (); }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ { ਵਾਪਸ ਢੇਰ_2.ਵਿੱਚ_error (); }

  ਤਾਰਕਿਕ alloc (ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ)
  { ਵਾਪਸ ਢੇਰ_2.alloc (ਮਾਪ); }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਖਾਲੀ insert (K ਤਰਜੀਹ, ਬਿਨਾਨਿਸ਼ਾਨੀ value)
  {
    ਢੇਰ_2.push (ਵਸਤੂ_t (ਤਰਜੀਹ, value));
    ਜੇ (unlikely (ਢੇਰ_2.ਵਿੱਚ_error ())) ਵਾਪਸ;
    bubble_ਉੱਪਰ (ਢੇਰ_2.ਲੰਮਾਈ - 1);
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_OPTIMIZE_ਮਾਪ
  HB_ALWAYS_INLINE
#ਜੇ_ਅੰਤ
  ਵਸਤੂ_t ਕੱਢੋ_minimum ()
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (!is_empty ());

    ਵਸਤੂ_t ਨਤੀਜਾ = ਢੇਰ_2.arrayZ[0];

    ਢੇਰ_2.arrayZ[0] = ਢੇਰ_2.arrayZ[ਢੇਰ_2.ਲੰਮਾਈ - 1];
    ਢੇਰ_2.resize (ਢੇਰ_2.ਲੰਮਾਈ - 1);

    ਜੇ (!is_empty ())
      bubble_ਥੱਲੇ (0);

    ਵਾਪਸ ਨਤੀਜਾ;
  }

  ਅਚਲ ਵਸਤੂ_t& minimum ()
  {
    ਵਾਪਸ ਢੇਰ_2[0];
  }

  ਤਾਰਕਿਕ is_empty () ਅਚਲ { ਵਾਪਸ ਢੇਰ_2.ਲੰਮਾਈ == 0; }
  ਸਪਸ਼ਟ ਸੰਚਾਲਕ ਤਾਰਕਿਕ () ਅਚਲ { ਵਾਪਸ !is_empty (); }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_population () ਅਚਲ { ਵਾਪਸ ਢੇਰ_2.ਲੰਮਾਈ; }

  /* Sink interface. */
  hb_ਤਰਜੀਹ_ਕਤਾਰ_t& ਸੰਚਾਲਕ << (ਵਸਤੂ_t ਵਸਤੂ)
  { insert (ਵਸਤੂ.ਪਹਿਲਾ, ਵਸਤੂ.second); ਵਾਪਸ *ਇਹ; }

 ਨਿਜੀ:

  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2)
  {
    ਵਾਪਸ (ਸੂਚਕਾਂਕ_2 - 1) / 2;
  }

  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਖੱਬਾ_ਬੱਚਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2)
  {
    ਵਾਪਸ 2 * ਸੂਚਕਾਂਕ_2 + 1;
  }

  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੱਜਾ_ਬੱਚਾ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2)
  {
    ਵਾਪਸ 2 * ਸੂਚਕਾਂਕ_2 + 2;
  }

  HB_ALWAYS_INLINE
  ਖਾਲੀ bubble_ਥੱਲੇ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2)
  {
    repeat:
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸੂਚਕਾਂਕ_2 < ਢੇਰ_2.ਲੰਮਾਈ);

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਖੱਬੇ = ਖੱਬਾ_ਬੱਚਾ (ਸੂਚਕਾਂਕ_2);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੱਜੇ = ਸੱਜਾ_ਬੱਚਾ (ਸੂਚਕਾਂਕ_2);

    ਤਾਰਕਿਕ has_ਖੱਬਾ = ਖੱਬੇ < ਢੇਰ_2.ਲੰਮਾਈ;
    ਜੇ (!has_ਖੱਬਾ)
      // If there's no left, then there's also no right.
      ਵਾਪਸ;

    ਤਾਰਕਿਕ has_ਸੱਜਾ = ਸੱਜੇ < ਢੇਰ_2.ਲੰਮਾਈ;
    ਜੇ (ਢੇਰ_2.arrayZ[ਸੂਚਕਾਂਕ_2].ਪਹਿਲਾ <= ਢੇਰ_2.arrayZ[ਖੱਬੇ].ਪਹਿਲਾ
        && (!has_ਸੱਜਾ || ਢੇਰ_2.arrayZ[ਸੂਚਕਾਂਕ_2].ਪਹਿਲਾ <= ਢੇਰ_2.arrayZ[ਸੱਜੇ].ਪਹਿਲਾ))
      ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਬੱਚਾ;
    ਜੇ (!has_ਸੱਜਾ || ਢੇਰ_2.arrayZ[ਖੱਬੇ].ਪਹਿਲਾ < ਢੇਰ_2.arrayZ[ਸੱਜੇ].ਪਹਿਲਾ)
      ਬੱਚਾ = ਖੱਬੇ;
    ਨਹੀਂਤਾਂ
      ਬੱਚਾ = ਸੱਜੇ;

    swap (ਸੂਚਕਾਂਕ_2, ਬੱਚਾ);
    ਸੂਚਕਾਂਕ_2 = ਬੱਚਾ;
    ਜਾਓ repeat;
  }

  HB_ALWAYS_INLINE
  ਖਾਲੀ bubble_ਉੱਪਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਸੂਚਕਾਂਕ_2)
  {
    repeat:
    ਪ੍ਰਤਿਜ੍ਞਾ (ਸੂਚਕਾਂਕ_2 < ਢੇਰ_2.ਲੰਮਾਈ);

    ਜੇ (ਸੂਚਕਾਂਕ_2 == 0) ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪਾ_ਸੂਚਕਾਂਕ = ਮਾਪਾ (ਸੂਚਕਾਂਕ_2);
    ਜੇ (ਢੇਰ_2.arrayZ[ਮਾਪਾ_ਸੂਚਕਾਂਕ].ਪਹਿਲਾ <= ਢੇਰ_2.arrayZ[ਸੂਚਕਾਂਕ_2].ਪਹਿਲਾ)
      ਵਾਪਸ;

    swap (ਸੂਚਕਾਂਕ_2, ਮਾਪਾ_ਸੂਚਕਾਂਕ);
    ਸੂਚਕਾਂਕ_2 = ਮਾਪਾ_ਸੂਚਕਾਂਕ;
    ਜਾਓ repeat;
  }

  ਖਾਲੀ swap (ਬਿਨਾਨਿਸ਼ਾਨੀ a, ਬਿਨਾਨਿਸ਼ਾਨੀ b) noexcept
  {
    ਪ੍ਰਤਿਜ੍ਞਾ (a < ਢੇਰ_2.ਲੰਮਾਈ);
    ਪ੍ਰਤਿਜ੍ਞਾ (b < ਢੇਰ_2.ਲੰਮਾਈ);
    hb_ਅਦਲਾ_ਬਦਲੀ (ਢੇਰ_2.arrayZ[a], ਢੇਰ_2.arrayZ[b]);
  }
};

#ਜੇ_ਅੰਤ /* HB_PRIORITY_QUEUE_HH */
