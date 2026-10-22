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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-vector.hh"

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
संरचना hb_priority_queue_t
{
 सार्वजनिक:
  प्रकारनिर्देश hb_pair_t<K, unsigned> item_t;

 गुप्त:
  hb_vector_t<item_t> heap;

 सार्वजनिक:

  hb_priority_queue_t () = स्वभाव;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    अत्थाय (पूर्णाङ्क i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  शून्य reset () { heap.clear (); }

  तार्किक in_error () ध्रुव { पटिनिवत्त heap.in_error (); }

  तार्किक alloc (unsigned size)
  { पटिनिवत्त heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  शून्य insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    सचे (unlikely (heap.in_error ())) पटिनिवत्त;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t निष्कासय_minimum ()
  {
    प्रतिज्ञा (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    सचे (!is_empty ())
      bubble_down (0);

    पटिनिवत्त result;
  }

  ध्रुव item_t& minimum ()
  {
    पटिनिवत्त heap[0];
  }

  तार्किक is_empty () ध्रुव { पटिनिवत्त heap.length == 0; }
  explicit operator तार्किक () ध्रुव { पटिनिवत्त !is_empty (); }
  unsigned पूर्णाङ्क get_population () ध्रुव { पटिनिवत्त heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); पटिनिवत्त *एतत्; }

 गुप्त:

  स्थिर constexpr unsigned parent (unsigned index)
  {
    पटिनिवत्त (index - 1) / 2;
  }

  स्थिर constexpr unsigned वामः_child (unsigned index)
  {
    पटिनिवत्त 2 * index + 1;
  }

  स्थिर constexpr unsigned दक्षिणः_child (unsigned index)
  {
    पटिनिवत्त 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  शून्य bubble_down (unsigned index)
  {
    repeat:
    प्रतिज्ञा (index < heap.length);

    unsigned left = वामः_child (index);
    unsigned दक्षिणः = दक्षिणः_child (index);

    तार्किक has_वामः = left < heap.length;
    सचे (!has_वामः)
      // If there's no left, then there's also no right.
      पटिनिवत्त;

    तार्किक has_दक्षिणः = दक्षिणः < heap.length;
    सचे (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_दक्षिणः || heap.arrayZ[index].first <= heap.arrayZ[दक्षिणः].first))
      पटिनिवत्त;

    unsigned child;
    सचे (!has_दक्षिणः || heap.arrayZ[left].first < heap.arrayZ[दक्षिणः].first)
      child = left;
    अञ्ञथा
      child = दक्षिणः;

    swap (index, child);
    index = child;
    गच्छ repeat;
  }

  HB_ALWAYS_INLINE
  शून्य bubble_up (unsigned index)
  {
    repeat:
    प्रतिज्ञा (index < heap.length);

    सचे (index == 0) पटिनिवत्त;

    unsigned parent_index = parent (index);
    सचे (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      पटिनिवत्त;

    swap (index, parent_index);
    index = parent_index;
    गच्छ repeat;
  }

  शून्य swap (unsigned a, unsigned b) noexcept
  {
    प्रतिज्ञा (a < heap.length);
    प्रतिज्ञा (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
