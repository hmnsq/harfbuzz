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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-vector.hh"

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
ഘടന hb_priority_queue_t
{
 പൊതു:
  തരനിർവചനം hb_pair_t<K, unsigned> item_t;

 സ്വകാര്യ:
  hb_vector_t<item_t> heap;

 പൊതു:

  hb_priority_queue_t () = സ്ഥിരസ്ഥിതി;
  hb_priority_queue_t (hb_vector_t<item_t>&& other) : heap (std::move (other))
  {
    // Heapify the vector.
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = (heap.length / 2) - 1; i >= 0; i--)
      bubble_down (i);
  }

  ശൂന്യം reset () { heap.clear (); }

  തർക്കം in_error () സ്ഥിരം { തിരികെ heap.in_error (); }

  തർക്കം alloc (unsigned size)
  { തിരികെ heap.alloc (size); }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  ശൂന്യം insert (K priority, unsigned value)
  {
    heap.push (item_t (priority, value));
    എങ്കിൽ (unlikely (heap.in_error ())) തിരികെ;
    bubble_up (heap.length - 1);
  }

#ifndef HB_OPTIMIZE_SIZE
  HB_ALWAYS_INLINE
#endif
  item_t എടുക്കുക_minimum ()
  {
    പ്രതിജ്ഞാ (!is_empty ());

    item_t result = heap.arrayZ[0];

    heap.arrayZ[0] = heap.arrayZ[heap.length - 1];
    heap.resize (heap.length - 1);

    എങ്കിൽ (!is_empty ())
      bubble_down (0);

    തിരികെ result;
  }

  സ്ഥിരം item_t& minimum ()
  {
    തിരികെ heap[0];
  }

  തർക്കം is_empty () സ്ഥിരം { തിരികെ heap.length == 0; }
  explicit operator തർക്കം () സ്ഥിരം { തിരികെ !is_empty (); }
  unsigned പൂർണ്ണസംഖ്യ get_population () സ്ഥിരം { തിരികെ heap.length; }

  /* Sink interface. */
  hb_priority_queue_t& operator << (item_t item)
  { insert (item.first, item.second); തിരികെ *ഇത്; }

 സ്വകാര്യ:

  സ്ഥിതം constexpr unsigned parent (unsigned index)
  {
    തിരികെ (index - 1) / 2;
  }

  സ്ഥിതം constexpr unsigned ഇടത്_child (unsigned index)
  {
    തിരികെ 2 * index + 1;
  }

  സ്ഥിതം constexpr unsigned വലത്_child (unsigned index)
  {
    തിരികെ 2 * index + 2;
  }

  HB_ALWAYS_INLINE
  ശൂന്യം bubble_down (unsigned index)
  {
    repeat:
    പ്രതിജ്ഞാ (index < heap.length);

    unsigned left = ഇടത്_child (index);
    unsigned വലത് = വലത്_child (index);

    തർക്കം has_ഇടത് = left < heap.length;
    എങ്കിൽ (!has_ഇടത്)
      // If there's no left, then there's also no right.
      തിരികെ;

    തർക്കം has_വലത് = വലത് < heap.length;
    എങ്കിൽ (heap.arrayZ[index].first <= heap.arrayZ[left].first
        && (!has_വലത് || heap.arrayZ[index].first <= heap.arrayZ[വലത്].first))
      തിരികെ;

    unsigned child;
    എങ്കിൽ (!has_വലത് || heap.arrayZ[left].first < heap.arrayZ[വലത്].first)
      child = left;
    അല്ലെങ്കിൽ
      child = വലത്;

    swap (index, child);
    index = child;
    പോകുക repeat;
  }

  HB_ALWAYS_INLINE
  ശൂന്യം bubble_up (unsigned index)
  {
    repeat:
    പ്രതിജ്ഞാ (index < heap.length);

    എങ്കിൽ (index == 0) തിരികെ;

    unsigned parent_index = parent (index);
    എങ്കിൽ (heap.arrayZ[parent_index].first <= heap.arrayZ[index].first)
      തിരികെ;

    swap (index, parent_index);
    index = parent_index;
    പോകുക repeat;
  }

  ശൂന്യം swap (unsigned a, unsigned b) noexcept
  {
    പ്രതിജ്ഞാ (a < heap.length);
    പ്രതിജ്ഞാ (b < heap.length);
    hb_swap (heap.arrayZ[a], heap.arrayZ[b]);
  }
};

#endif /* HB_PRIORITY_QUEUE_HH */
