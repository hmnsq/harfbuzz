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

#شامل "hb.hh"
#شامل "hb-vector.hh"

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
ساختار hb_priority_queue_t
{
 عمومی:
  تعریفنوع hb_pair_t<K, unsigned> item_t;

 خصوصی:
  hb_vector_t<item_t> heap;

 عمومی:

  hb_priority_queue_t () = پیشفرض;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    برای (صحیح i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  تهی reset () { heap.clear (); }

  منطقی in_error () ثابت { بازگشت heap.in_error (); }

  منطقی alloc (unsigned size)
  { بازگشت heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  تهی insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    اگر (unlikely (heap.in_error ())) بازگشت;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t برداشت_minimum ()
  {
    أكد (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    اگر (!is_empty ())
      bubble_down (0);

    بازگشت result;
  }

  ثابت item_t& minimum ()
  {
    بازگشت heap[0];
  }

  منطقی is_empty () ثابت { بازگشت heap.length == 0; }
  explicit operator منطقی () ثابت { بازگشت !is_empty (); }
  unsigned صحیح get_population () ثابت { بازگشت heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); بازگشت *این; }

 خصوصی:

  ایستا constexpr unsigned parent (unsigned index)
  {
    بازگشت (index - 1) / 2;
  }

  ایستا constexpr unsigned چپ_child (unsigned index)
  {
    بازگشت 2 * index + 1;
  }

  ایستا constexpr unsigned راست_child (unsigned index)
  {
    بازگشت 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  تهی bubble_down (unsigned index)
  {
    repeat:
    أكد (index < heap.length);

    unsigned left = چپ_child (index);
    unsigned راست = راست_child (index);

    منطقی has_چپ = left < heap.length;
    اگر (!has_چپ)
      // If there's no left, then there's also no right.
      بازگشت;

    منطقی has_راست = راست < heap.length;
    اگر (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_راست || heap.arrayZ[index].first <= heap.arrayZ[راست].first))
      بازگشت;

    unsigned child;
    اگر (!has_راست || heap.arrayZ[left].first < heap.arrayZ[راست].first)
      child = left;
    وگرنه
      child = راست;

    swap (index, child);
    index = child;
    برو repeat;
  }

  HB_ALWAYS_INLINE
  تهی bubble_up (unsigned index)
  {
    repeat:
    أكد (index < heap.length);

    اگر (index == 0) بازگشت;

    unsigned parent_index = parent (index);
    اگر (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      بازگشت;

    swap (index, parent_index);
    index = parent_index;
    برو repeat;
  }

  تهی swap (unsigned a, unsigned b) noexcept
  {
    أكد (a < heap.length);
    أكد (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
