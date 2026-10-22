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

#ضمن "hb.hh"
#ضمن "hb-vector.hh"

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
بنية hb_priority_queue_t
{
 عام:
  تعريفنوع hb_pair_t<K, unsigned> item_t;

 خاص:
  hb_vector_t<item_t> heap;

 عام:

  hb_priority_queue_t () = افتراضي;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    لكل (صحيح i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  فراغ reset () { heap.clear (); }

  منطقي in_error () ثابت { ܦܢܐ heap.in_error (); }

  منطقي alloc (unsigned size)
  { ܦܢܐ heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  فراغ insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    ܐܢ (unlikely (heap.in_error ())) ܦܢܐ;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t pop_minimum ()
  {
    أكد (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    ܐܢ (!is_empty ())
      bubble_down (0);

    ܦܢܐ result;
  }

  ثابت item_t& minimum ()
  {
    ܦܢܐ heap[0];
  }

  منطقي is_empty () ثابت { ܦܢܐ heap.length == 0; }
  explicit operator منطقي () ثابت { ܦܢܐ !is_empty (); }
  unsigned صحيح get_population () ثابت { ܦܢܐ heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); ܦܢܐ *هذا; }

 خاص:

  ساكن constexpr unsigned parent (unsigned index)
  {
    ܦܢܐ (index - 1) / 2;
  }

  ساكن constexpr unsigned left_child (unsigned index)
  {
    ܦܢܐ 2 * index + 1;
  }

  ساكن constexpr unsigned right_child (unsigned index)
  {
    ܦܢܐ 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_down (unsigned index)
  {
    repeat:
    أكد (index < heap.length);

    unsigned left = left_child (index);
    unsigned right = right_child (index);

    منطقي has_left = left < heap.length;
    ܐܢ (!has_left)
      // If there's no left, then there's also no right.
      ܦܢܐ;

    منطقي has_right = right < heap.length;
    ܐܢ (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_right || heap.arrayZ[index].first <= heap.arrayZ[right].first))
      ܦܢܐ;

    unsigned child;
    ܐܢ (!has_right || heap.arrayZ[left].first < heap.arrayZ[right].first)
      child = left;
    ܐܚܪܢܐ
      child = right;

    swap (index, child);
    index = child;
    اذهب repeat;
  }

  HB_ALWAYS_INLINE
  فراغ bubble_up (unsigned index)
  {
    repeat:
    أكد (index < heap.length);

    ܐܢ (index == 0) ܦܢܐ;

    unsigned parent_index = parent (index);
    ܐܢ (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      ܦܢܐ;

    swap (index, parent_index);
    index = parent_index;
    اذهب repeat;
  }

  فراغ swap (unsigned a, unsigned b) noexcept
  {
    أكد (a < heap.length);
    أكد (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
