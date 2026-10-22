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

#ifndef HB_PRIORITY_QUEUE_HH
#define HB_PRIORITY_QUEUE_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-vector.hh"

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
template <typename K>
ಸಂರಚನಾ hb_priority_queue_t
{
 ಸಾರ್ವಜನಿಕ:
  ಪ್ರಕಾರನಿರ್ದೇಶ hb_pair_t<K, unsigned> item_t;

 ಗುಪ್ತ:
  hb_vector_t<item_t> heap;

 ಸಾರ್ವಜನಿಕ:

  hb_priority_queue_t () = ಸ್ವಭಾವ;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  ಶೂನ್ಯ reset () { heap.clear (); }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ heap.in_error (); }

  ತಾರ್ಕಿಕ alloc (unsigned size)
  { ಪ್ರತ್ಯಾಗಮ heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ಶೂನ್ಯ insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    ಯದಿ (unlikely (heap.in_error ())) ಪ್ರತ್ಯಾಗಮ;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t ತೆಗೆ_minimum ()
  {
    ಪ್ರತಿಜ್ಞಾ (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    ಯದಿ (!is_empty ())
      bubble_down (0);

    ಪ್ರತ್ಯಾಗಮ result;
  }

  ಧ್ರುವ item_t& minimum ()
  {
    ಪ್ರತ್ಯಾಗಮ heap[0];
  }

  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ heap.length == 0; }
  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_empty (); }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_population () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

 ಗುಪ್ತ:

  ಸ್ಥಿರ constexpr unsigned parent (unsigned index)
  {
    ಪ್ರತ್ಯಾಗಮ (index - 1) / 2;
  }

  ಸ್ಥಿರ constexpr unsigned ಎಡ_child (unsigned index)
  {
    ಪ್ರತ್ಯಾಗಮ 2 * index + 1;
  }

  ಸ್ಥಿರ constexpr unsigned ಬಲ_child (unsigned index)
  {
    ಪ್ರತ್ಯಾಗಮ 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  ಶೂನ್ಯ bubble_down (unsigned index)
  {
    repeat:
    ಪ್ರತಿಜ್ಞಾ (index < heap.length);

    unsigned left = ಎಡ_child (index);
    unsigned ಬಲ = ಬಲ_child (index);

    ತಾರ್ಕಿಕ has_ಎಡ = left < heap.length;
    ಯದಿ (!has_ಎಡ)
      // If there's no left, then there's also no right.
      ಪ್ರತ್ಯಾಗಮ;

    ತಾರ್ಕಿಕ has_ಬಲ = ಬಲ < heap.length;
    ಯದಿ (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_ಬಲ || heap.arrayZ[index].first <= heap.arrayZ[ಬಲ].first))
      ಪ್ರತ್ಯಾಗಮ;

    unsigned child;
    ಯದಿ (!has_ಬಲ || heap.arrayZ[left].first < heap.arrayZ[ಬಲ].first)
      child = left;
    ಅನ್ಯಥಾ
      child = ಬಲ;

    swap (index, child);
    index = child;
    ಗಚ್ಛ repeat;
  }

  HB_ALWAYS_INLINE
  ಶೂನ್ಯ bubble_up (unsigned index)
  {
    repeat:
    ಪ್ರತಿಜ್ಞಾ (index < heap.length);

    ಯದಿ (index == 0) ಪ್ರತ್ಯಾಗಮ;

    unsigned parent_index = parent (index);
    ಯದಿ (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      ಪ್ರತ್ಯಾಗಮ;

    swap (index, parent_index);
    index = parent_index;
    ಗಚ್ಛ repeat;
  }

  ಶೂನ್ಯ swap (unsigned a, unsigned b) noexcept
  {
    ಪ್ರತಿಜ್ಞಾ (a < heap.length);
    ಪ್ರತಿಜ್ಞಾ (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
