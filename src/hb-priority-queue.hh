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

#include "hb.hh"
#include "hb-vector.hh"

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
struct hb_priority_queue_t
{
 public:
  typedef hb_pair_t<K, unsigned> item_t;

 private:
  hb_vector_t<item_t> heap;

 public:

  hb_priority_queue_t () = default;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    为 (整数 i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  void reset () { heap.clear (); }

  bool in_error () const { 返回 heap.in_error (); }

  bool alloc (unsigned size)
  { 返回 heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  void insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    如果 (unlikely (heap.in_error ())) 返回;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t pop_minimum ()
  {
    assert (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    如果 (!is_empty ())
      bubble_down (0);

    返回 result;
  }

  const item_t& minimum ()
  {
    返回 heap[0];
  }

  bool is_empty () const { 返回 heap.length == 0; }
  explicit operator bool () const { 返回 !is_empty (); }
  unsigned 整数 get_population () const { 返回 heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); 返回 *this; }

 private:

  static constexpr unsigned parent (unsigned index)
  {
    返回 (index - 1) / 2;
  }

  static constexpr unsigned left_child (unsigned index)
  {
    返回 2 * index + 1;
  }

  static constexpr unsigned right_child (unsigned index)
  {
    返回 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  void bubble_down (unsigned index)
  {
    repeat:
    assert (index < heap.length);

    unsigned left = left_child (index);
    unsigned right = right_child (index);

    bool has_left = left < heap.length;
    如果 (!has_left)
      // If there's no left, then there's also no right.
      返回;

    bool has_right = right < heap.length;
    如果 (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_right || heap.arrayZ[index].first <= heap.arrayZ[right].first))
      返回;

    unsigned child;
    如果 (!has_right || heap.arrayZ[left].first < heap.arrayZ[right].first)
      child = left;
    否则
      child = right;

    swap (index, child);
    index = child;
    goto repeat;
  }

  HB_ALWAYS_INLINE
  void bubble_up (unsigned index)
  {
    repeat:
    assert (index < heap.length);

    如果 (index == 0) 返回;

    unsigned parent_index = parent (index);
    如果 (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      返回;

    swap (index, parent_index);
    index = parent_index;
    goto repeat;
  }

  void swap (unsigned a, unsigned b) noexcept
  {
    assert (a < heap.length);
    assert (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
