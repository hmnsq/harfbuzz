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

#অন্তর্ভাৱ "hb.hh"
#অন্তর্ভাৱ "hb-vector.hh"

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
সংরচনা hb_priority_queue_t
{
 সার্ৱজনিক:
  প্রকারনির্দেশ hb_pair_t<K, unsigned> item_t;

 গুপ্ত:
  hb_vector_t<item_t> heap;

 সার্ৱজনিক:

  hb_priority_queue_t () = স্ৱভাৱ;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    বাবে (পূর্ণাঙ্ক i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  শূন্য reset () { heap.clear (); }

  তার্কিক in_error () ধ্রুৱ { ঘূৰাই heap.in_error (); }

  তার্কিক alloc (unsigned size)
  { ঘূৰাই heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  শূন্য insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    যদি (unlikely (heap.in_error ())) ঘূৰাই;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t বেৰ_কৰো_minimum ()
  {
    প্রতিজ্ঞা (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    যদি (!is_empty ())
      bubble_down (0);

    ঘূৰাই result;
  }

  ধ্রুৱ item_t& minimum ()
  {
    ঘূৰাই heap[0];
  }

  তার্কিক is_empty () ধ্রুৱ { ঘূৰাই heap.length == 0; }
  explicit operator তার্কিক () ধ্রুৱ { ঘূৰাই !is_empty (); }
  unsigned পূর্ণাঙ্ক get_population () ধ্রুৱ { ঘূৰাই heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); ঘূৰাই *এতত্; }

 গুপ্ত:

  স্থির constexpr unsigned parent (unsigned index)
  {
    ঘূৰাই (index - 1) / 2;
  }

  স্থির constexpr unsigned বাম_child (unsigned index)
  {
    ঘূৰাই 2 * index + 1;
  }

  স্থির constexpr unsigned ডান_child (unsigned index)
  {
    ঘূৰাই 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  শূন্য bubble_down (unsigned index)
  {
    repeat:
    প্রতিজ্ঞা (index < heap.length);

    unsigned left = বাম_child (index);
    unsigned ডান = ডান_child (index);

    তার্কিক has_বাম = left < heap.length;
    যদি (!has_বাম)
      // If there's no left, then there's also no right.
      ঘূৰাই;

    তার্কিক has_ডান = ডান < heap.length;
    যদি (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_ডান || heap.arrayZ[index].first <= heap.arrayZ[ডান].first))
      ঘূৰাই;

    unsigned child;
    যদি (!has_ডান || heap.arrayZ[left].first < heap.arrayZ[ডান].first)
      child = left;
    অন্যথা
      child = ডান;

    swap (index, child);
    index = child;
    গচ্ছ repeat;
  }

  HB_ALWAYS_INLINE
  শূন্য bubble_up (unsigned index)
  {
    repeat:
    প্রতিজ্ঞা (index < heap.length);

    যদি (index == 0) ঘূৰাই;

    unsigned parent_index = parent (index);
    যদি (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      ঘূৰাই;

    swap (index, parent_index);
    index = parent_index;
    গচ্ছ repeat;
  }

  শূন্য swap (unsigned a, unsigned b) noexcept
  {
    প্রতিজ্ঞা (a < heap.length);
    প্রতিজ্ঞা (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
