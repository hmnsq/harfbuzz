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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-vector.hh"

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
ସଂରଚନା hb_priority_queue_t
{
 ସର୍ବସାଧାରଣ:
  ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_pair_t<K, unsigned> item_t;

 ବ୍ୟକ୍ତିଗତ:
  hb_vector_t<item_t> heap;

 ସର୍ବସାଧାରଣ:

  hb_priority_queue_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  ଶୂନ୍ୟ reset () { heap.clear (); }

  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ heap.in_error (); }

  ତାର୍କିକ alloc (unsigned size)
  { ଫେରନ୍ତୁ heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ଶୂନ୍ୟ insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    ଯଦି (unlikely (heap.in_error ())) ଫେରନ୍ତୁ;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t କାଢ଼_minimum ()
  {
    ପ୍ରତିଜ୍ଞା (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    ଯଦି (!is_empty ())
      bubble_down (0);

    ଫେରନ୍ତୁ result;
  }

  ଅଚଳ item_t& minimum ()
  {
    ଫେରନ୍ତୁ heap[0];
  }

  ତାର୍କିକ is_empty () ଅଚଳ { ଫେରନ୍ତୁ heap.length == 0; }
  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ !is_empty (); }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_population () ଅଚଳ { ଫେରନ୍ତୁ heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); ଫେରନ୍ତୁ *ଏହି; }

 ବ୍ୟକ୍ତିଗତ:

  ସ୍ଥିର constexpr unsigned parent (unsigned index)
  {
    ଫେରନ୍ତୁ (index - 1) / 2;
  }

  ସ୍ଥିର constexpr unsigned ବାମ_child (unsigned index)
  {
    ଫେରନ୍ତୁ 2 * index + 1;
  }

  ସ୍ଥିର constexpr unsigned ଡାହାଣ_child (unsigned index)
  {
    ଫେରନ୍ତୁ 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ bubble_down (unsigned index)
  {
    repeat:
    ପ୍ରତିଜ୍ଞା (index < heap.length);

    unsigned left = ବାମ_child (index);
    unsigned ଡାହାଣ = ଡାହାଣ_child (index);

    ତାର୍କିକ has_ବାମ = left < heap.length;
    ଯଦି (!has_ବାମ)
      // If there's no left, then there's also no right.
      ଫେରନ୍ତୁ;

    ତାର୍କିକ has_ଡାହାଣ = ଡାହାଣ < heap.length;
    ଯଦି (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_ଡାହାଣ || heap.arrayZ[index].first <= heap.arrayZ[ଡାହାଣ].first))
      ଫେରନ୍ତୁ;

    unsigned child;
    ଯଦି (!has_ଡାହାଣ || heap.arrayZ[left].first < heap.arrayZ[ଡାହାଣ].first)
      child = left;
    ଅନ୍ୟଥା
      child = ଡାହାଣ;

    swap (index, child);
    index = child;
    ଯାଅ repeat;
  }

  HB_ALWAYS_INLINE
  ଶୂନ୍ୟ bubble_up (unsigned index)
  {
    repeat:
    ପ୍ରତିଜ୍ଞା (index < heap.length);

    ଯଦି (index == 0) ଫେରନ୍ତୁ;

    unsigned parent_index = parent (index);
    ଯଦି (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      ଫେରନ୍ତୁ;

    swap (index, parent_index);
    index = parent_index;
    ଯାଅ repeat;
  }

  ଶୂନ୍ୟ swap (unsigned a, unsigned b) noexcept
  {
    ପ୍ରତିଜ୍ଞା (a < heap.length);
    ପ୍ରତିଜ୍ଞା (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
