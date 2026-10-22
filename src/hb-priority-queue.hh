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
ढाँचा <typename K>
संरचना hb_priority_queue_t
{
 सार्वजनिक:
  प्रकारनिर्देश hb_pair_t<K, अचिन्हित> item_t;

 गुप्त:
  hb_vector_t<item_t> heap;

 सार्वजनिक:

  hb_priority_queue_t () = स्वभाव;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    लागि (पूर्णाङ्क i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  शून्य reset () { heap.clear (); }

  तार्किक in_error () अचर { फर्काऊ heap.in_error (); }

  तार्किक alloc (अचिन्हित size)
  { फर्काऊ heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  शून्य insert (K priority, अचिन्हित value)
  {
    heap.push (item_t (priority, value));
    यदि (unlikely (heap.in_error ())) फर्काऊ;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t झिक_minimum ()
  {
    प्रतिज्ञा (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    यदि (!is_empty ())
      bubble_down (0);

    फर्काऊ result;
  }

  अचर item_t& minimum ()
  {
    फर्काऊ heap[0];
  }

  तार्किक is_empty () अचर { फर्काऊ heap.length == 0; }
  explicit चालक तार्किक () अचर { फर्काऊ !is_empty (); }
  अचिन्हित पूर्णाङ्क get_population () अचर { फर्काऊ heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& चालक << (item_t item)
  { insert (item.first, item.second); फर्काऊ *एतत्; }

 गुप्त:

  स्थिर constexpr अचिन्हित parent (अचिन्हित index)
  {
    फर्काऊ (index - 1) / 2;
  }

  स्थिर constexpr अचिन्हित बायाँ_child (अचिन्हित index)
  {
    फर्काऊ 2 * index + 1;
  }

  स्थिर constexpr अचिन्हित दायाँ_child (अचिन्हित index)
  {
    फर्काऊ 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  शून्य bubble_down (अचिन्हित index)
  {
    repeat:
    प्रतिज्ञा (index < heap.length);

    अचिन्हित left = बायाँ_child (index);
    अचिन्हित दायाँ = दायाँ_child (index);

    तार्किक has_बायाँ = left < heap.length;
    यदि (!has_बायाँ)
      // If there's no left, then there's also no right.
      फर्काऊ;

    तार्किक has_दायाँ = दायाँ < heap.length;
    यदि (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_दायाँ || heap.arrayZ[index].first <= heap.arrayZ[दायाँ].first))
      फर्काऊ;

    अचिन्हित child;
    यदि (!has_दायाँ || heap.arrayZ[left].first < heap.arrayZ[दायाँ].first)
      child = left;
    अन्यथा
      child = दायाँ;

    swap (index, child);
    index = child;
    गच्छ repeat;
  }

  HB_ALWAYS_INLINE
  शून्य bubble_up (अचिन्हित index)
  {
    repeat:
    प्रतिज्ञा (index < heap.length);

    यदि (index == 0) फर्काऊ;

    अचिन्हित parent_index = parent (index);
    यदि (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      फर्काऊ;

    swap (index, parent_index);
    index = parent_index;
    गच्छ repeat;
  }

  शून्य swap (अचिन्हित a, अचिन्हित b) noexcept
  {
    प्रतिज्ञा (a < heap.length);
    प्रतिज्ञा (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
