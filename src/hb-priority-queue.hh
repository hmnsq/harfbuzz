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
  प्रकारव्याख्या hb_pair_t<K, अचिन्हित> item_t;

 खाजगी:
  hb_vector_t<item_t> heap;

 सार्वजनिक:

  hb_priority_queue_t () = पूर्वनिर्धारित;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    साठी (पूर्णांक i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  शून्य reset () { heap.clear (); }

  तार्किक in_error () स्थिरांक { परत heap.in_error (); }

  तार्किक alloc (अचिन्हित size)
  { परत heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  शून्य insert (K priority, अचिन्हित value)
  {
    heap.push (item_t (priority, value));
    जर (unlikely (heap.in_error ())) परत;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t काढा_minimum ()
  {
    प्रतिज्ञा (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    जर (!is_empty ())
      bubble_down (0);

    परत result;
  }

  स्थिरांक item_t& minimum ()
  {
    परत heap[0];
  }

  तार्किक is_empty () स्थिरांक { परत heap.length == 0; }
  explicit चालक तार्किक () स्थिरांक { परत !is_empty (); }
  अचिन्हित पूर्णांक get_population () स्थिरांक { परत heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& चालक << (item_t item)
  { insert (item.first, item.second); परत *हे; }

 खाजगी:

  स्थिर constexpr अचिन्हित parent (अचिन्हित index)
  {
    परत (index - 1) / 2;
  }

  स्थिर constexpr अचिन्हित डावा_child (अचिन्हित index)
  {
    परत 2 * index + 1;
  }

  स्थिर constexpr अचिन्हित उजवा_child (अचिन्हित index)
  {
    परत 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  शून्य bubble_down (अचिन्हित index)
  {
    repeat:
    प्रतिज्ञा (index < heap.length);

    अचिन्हित left = डावा_child (index);
    अचिन्हित उजवा = उजवा_child (index);

    तार्किक has_डावा = left < heap.length;
    जर (!has_डावा)
      // If there's no left, then there's also no right.
      परत;

    तार्किक has_उजवा = उजवा < heap.length;
    जर (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_उजवा || heap.arrayZ[index].first <= heap.arrayZ[उजवा].first))
      परत;

    अचिन्हित child;
    जर (!has_उजवा || heap.arrayZ[left].first < heap.arrayZ[उजवा].first)
      child = left;
    अन्यथा
      child = उजवा;

    swap (index, child);
    index = child;
    जा repeat;
  }

  HB_ALWAYS_INLINE
  शून्य bubble_up (अचिन्हित index)
  {
    repeat:
    प्रतिज्ञा (index < heap.length);

    जर (index == 0) परत;

    अचिन्हित parent_index = parent (index);
    जर (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      परत;

    swap (index, parent_index);
    index = parent_index;
    जा repeat;
  }

  शून्य swap (अचिन्हित a, अचिन्हित b) noexcept
  {
    प्रतिज्ञा (a < heap.length);
    प्रतिज्ञा (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
