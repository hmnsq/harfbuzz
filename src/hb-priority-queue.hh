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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-vector.hh"

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
কাঠামো hb_priority_queue_t
{
 সর্বজনীন:
  প্রকারলেখ hb_pair_t<K, unsigned> item_t;

 ব্যক্তিগত:
  hb_vector_t<item_t> heap;

 সর্বজনীন:

  hb_priority_queue_t () = পূর্বনির্ধারিত;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    জন্য (পূর্ণসংখ্যা i = (heap.দৈর্ঘ্য / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  শূন্য reset () { heap.clear (); }

  যৌক্তিক in_error () ধ্রুবক { ফেরত heap.in_error (); }

  যৌক্তিক alloc (unsigned আকার_2)
  { ফেরত heap.alloc (আকার_2); }

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  শূন্য insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    যদি (unlikely (heap.in_error ())) ফেরত;
    bubble_up (heap.দৈর্ঘ্য - 1);
  }

#ifndef HB_OPTIMIZE_আকার
  HB_ALWAYS_INLINE
#endif
  item_t বের_করো_minimum ()
  {
    প্রতিজ্ঞা (!is_empty ());

    item_t ফলাফল = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.দৈর্ঘ্য - 1];
    heap.resize (heap.দৈর্ঘ্য - 1);

    যদি (!is_empty ())
      bubble_down (0);

    ফেরত ফলাফল;
  }

  ধ্রুবক item_t& minimum ()
  {
    ফেরত heap[0];
  }

  যৌক্তিক is_empty () ধ্রুবক { ফেরত heap.দৈর্ঘ্য == 0; }
  explicit operator যৌক্তিক () ধ্রুবক { ফেরত !is_empty (); }
  unsigned পূর্ণসংখ্যা get_population () ধ্রুবক { ফেরত heap.দৈর্ঘ্য; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); ফেরত *এই; }

 ব্যক্তিগত:

  স্থির constexpr unsigned parent (unsigned সূচক_2)
  {
    ফেরত (সূচক_2 - 1) / 2;
  }

  স্থির constexpr unsigned বাম_child (unsigned সূচক_2)
  {
    ফেরত 2 * সূচক_2 + 1;
  }

  স্থির constexpr unsigned ডান_child (unsigned সূচক_2)
  {
    ফেরত 2 * সূচক_2 + 2;
  }

  HB_ALWAYS_INLINE
  শূন্য bubble_down (unsigned সূচক_2)
  {
    repeat:
    প্রতিজ্ঞা (সূচক_2 < heap.দৈর্ঘ্য);

    unsigned left = বাম_child (সূচক_2);
    unsigned ডান = ডান_child (সূচক_2);

    যৌক্তিক has_বাম = left < heap.দৈর্ঘ্য;
    যদি (!has_বাম)
      // If there's no left, then there's also no right.
      ফেরত;

    যৌক্তিক has_ডান = ডান < heap.দৈর্ঘ্য;
    যদি (heap.arrayZ[সূচক_2].first <= heap.arrayZ[left].first
        && (!has_ডান || heap.arrayZ[সূচক_2].first <= heap.arrayZ[ডান].first))
      ফেরত;

    unsigned child;
    যদি (!has_ডান || heap.arrayZ[left].first < heap.arrayZ[ডান].first)
      child = left;
    অন্যথা
      child = ডান;

    swap (সূচক_2, child);
    সূচক_2 = child;
    যাও repeat;
  }

  HB_ALWAYS_INLINE
  শূন্য bubble_up (unsigned সূচক_2)
  {
    repeat:
    প্রতিজ্ঞা (সূচক_2 < heap.দৈর্ঘ্য);

    যদি (সূচক_2 == 0) ফেরত;

    unsigned parent_সূচক = parent (সূচক_2);
    যদি (heap.arrayZ[parent_সূচক].first <= heap.arrayZ[সূচক_2].first)
      ফেরত;

    swap (সূচক_2, parent_সূচক);
    সূচক_2 = parent_সূচক;
    যাও repeat;
  }

  শূন্য swap (unsigned a, unsigned b) noexcept
  {
    প্রতিজ্ঞা (a < heap.দৈর্ঘ্য);
    প্রতিজ্ঞা (b < heap.দৈর্ঘ্য);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
