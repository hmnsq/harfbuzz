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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-vector.hh"

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
સંરચના hb_priority_queue_t
{
 જાહેર:
  પ્રકારવ્યાખ્યા hb_pair_t<K, unsigned> item_t;

 ખાનગી:
  hb_vector_t<item_t> heap;

 જાહેર:

  hb_priority_queue_t () = મૂળભૂત;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    માટે (પૂર્ણાંક i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  શૂન્ય reset () { heap.clear (); }

  તાર્કિક in_error () અચલ { પાછા heap.in_error (); }

  તાર્કિક alloc (unsigned size)
  { પાછા heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  શૂન્ય insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    જો (unlikely (heap.in_error ())) પાછા;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t કાઢો_minimum ()
  {
    પ્રતિજ્ઞા (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    જો (!is_empty ())
      bubble_down (0);

    પાછા result;
  }

  અચલ item_t& minimum ()
  {
    પાછા heap[0];
  }

  તાર્કિક is_empty () અચલ { પાછા heap.length == 0; }
  explicit operator તાર્કિક () અચલ { પાછા !is_empty (); }
  unsigned પૂર્ણાંક get_population () અચલ { પાછા heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); પાછા *આ; }

 ખાનગી:

  સ્થિર constexpr unsigned parent (unsigned index)
  {
    પાછા (index - 1) / 2;
  }

  સ્થિર constexpr unsigned ડાબું_child (unsigned index)
  {
    પાછા 2 * index + 1;
  }

  સ્થિર constexpr unsigned જમણું_child (unsigned index)
  {
    પાછા 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  શૂન્ય bubble_down (unsigned index)
  {
    repeat:
    પ્રતિજ્ઞા (index < heap.length);

    unsigned left = ડાબું_child (index);
    unsigned જમણું = જમણું_child (index);

    તાર્કિક has_ડાબું = left < heap.length;
    જો (!has_ડાબું)
      // If there's no left, then there's also no right.
      પાછા;

    તાર્કિક has_જમણું = જમણું < heap.length;
    જો (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_જમણું || heap.arrayZ[index].first <= heap.arrayZ[જમણું].first))
      પાછા;

    unsigned child;
    જો (!has_જમણું || heap.arrayZ[left].first < heap.arrayZ[જમણું].first)
      child = left;
    નહીંતર
      child = જમણું;

    swap (index, child);
    index = child;
    જાઓ repeat;
  }

  HB_ALWAYS_INLINE
  શૂન્ય bubble_up (unsigned index)
  {
    repeat:
    પ્રતિજ્ઞા (index < heap.length);

    જો (index == 0) પાછા;

    unsigned parent_index = parent (index);
    જો (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      પાછા;

    swap (index, parent_index);
    index = parent_index;
    જાઓ repeat;
  }

  શૂન્ય swap (unsigned a, unsigned b) noexcept
  {
    પ્રતિજ્ઞા (a < heap.length);
    પ્રતિજ્ઞા (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
