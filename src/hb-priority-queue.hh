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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-vector.hh"

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
கட்டமைப்பு hb_priority_queue_t
{
 பொது:
  வகைவரை hb_pair_t<K, unsigned> item_t;

 தனிப்பட்ட:
  hb_vector_t<item_t> heap;

 பொது:

  hb_priority_queue_t () = இயல்பு;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    க்கு (முழு i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  வெறுமை reset () { heap.clear (); }

  தருக்க in_error () மாறிலி { திரும்பு heap.in_error (); }

  தருக்க alloc (unsigned size)
  { திரும்பு heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  வெறுமை insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    எனில் (unlikely (heap.in_error ())) திரும்பு;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t எடு_minimum ()
  {
    ப்ரதிஜ்ஞா (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    எனில் (!is_empty ())
      bubble_down (0);

    திரும்பு result;
  }

  மாறிலி item_t& minimum ()
  {
    திரும்பு heap[0];
  }

  தருக்க is_empty () மாறிலி { திரும்பு heap.length == 0; }
  explicit operator தருக்க () மாறிலி { திரும்பு !is_empty (); }
  unsigned முழு get_population () மாறிலி { திரும்பு heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); திரும்பு *இது; }

 தனிப்பட்ட:

  நிலையான constexpr unsigned parent (unsigned index)
  {
    திரும்பு (index - 1) / 2;
  }

  நிலையான constexpr unsigned இடது_child (unsigned index)
  {
    திரும்பு 2 * index + 1;
  }

  நிலையான constexpr unsigned வலது_child (unsigned index)
  {
    திரும்பு 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  வெறுமை bubble_down (unsigned index)
  {
    repeat:
    ப்ரதிஜ்ஞா (index < heap.length);

    unsigned left = இடது_child (index);
    unsigned வலது = வலது_child (index);

    தருக்க has_இடது = left < heap.length;
    எனில் (!has_இடது)
      // If there's no left, then there's also no right.
      திரும்பு;

    தருக்க has_வலது = வலது < heap.length;
    எனில் (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_வலது || heap.arrayZ[index].first <= heap.arrayZ[வலது].first))
      திரும்பு;

    unsigned child;
    எனில் (!has_வலது || heap.arrayZ[left].first < heap.arrayZ[வலது].first)
      child = left;
    இல்லையெனில்
      child = வலது;

    swap (index, child);
    index = child;
    செல் repeat;
  }

  HB_ALWAYS_INLINE
  வெறுமை bubble_up (unsigned index)
  {
    repeat:
    ப்ரதிஜ்ஞா (index < heap.length);

    எனில் (index == 0) திரும்பு;

    unsigned parent_index = parent (index);
    எனில் (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      திரும்பு;

    swap (index, parent_index);
    index = parent_index;
    செல் repeat;
  }

  வெறுமை swap (unsigned a, unsigned b) noexcept
  {
    ப்ரதிஜ்ஞா (a < heap.length);
    ப்ரதிஜ்ஞா (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
