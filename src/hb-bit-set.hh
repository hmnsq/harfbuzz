/*
 * Copyright © 2012,2017  Google, Inc.
 * Copyright © 2021 Behdad Esfahbod
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
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_BIT_SET_HH
#define HB_BIT_SET_HH

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-bit-page.hh"

ಸಂರಚನಾ hb_bit_set_t
{
  hb_bit_set_t () = ಸ್ವಭಾವ;
  ~hb_bit_set_t () = ಸ್ವಭಾವ;

  hb_bit_set_t (ಧ್ರುವ hb_bit_set_t& other) : hb_bit_set_t () { set (other, ಸತ್ಯ); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*ಏತತ್, other); }
  hb_bit_set_t& operator= (ಧ್ರುವ hb_bit_set_t& other) { set (other); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*ಏತತ್, other); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  friend ಶೂನ್ಯ swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    ಯದಿ (likely (!a.successful || !b.successful))
      ಪ್ರತ್ಯಾಗಮ;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  ಶೂನ್ಯ init ()
  {
    successful = ಸತ್ಯ;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  ಶೂನ್ಯ fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  ಸಂರಚನಾ page_map_t
  {
    ಪೂರ್ಣಾಙ್ಕ cmp (ಧ್ರುವ page_map_t &o) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ cmp (o.major); }
    ಪೂರ್ಣಾಙ್ಕ cmp (uint32_t o_major) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ) o_major - (ಪೂರ್ಣಾಙ್ಕ) major; }

    uint32_t major;
    uint32_t index;
  };

  ತಾರ್ಕಿಕ successful = ಸತ್ಯ; /* Allocations successful */
  mutable unsigned ಪೂರ್ಣಾಙ್ಕ population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  ಶೂನ್ಯ err () { ಯದಿ (successful) successful = ಅಸತ್ಯ; } /* TODO Remove */
  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !successful; }

  ತಾರ್ಕಿಕ resize (unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, ತಾರ್ಕಿಕ clear = ಸತ್ಯ, ತಾರ್ಕಿಕ exact_size = ಅಸತ್ಯ)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಯದಿ (pages.length < ಎಣಿಕೆ && (unsigned) pages.allocated < ಎಣಿಕೆ && ಎಣಿಕೆ <= 2)
      exact_size = ಸತ್ಯ; // Most sets are small and local

    ಯದಿ (unlikely (!pages.resize_full (ಎಣಿಕೆ, clear, exact_size) ||
	!page_map.resize_full (ಎಣಿಕೆ, clear, ಅಸತ್ಯ)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಶೂನ್ಯ alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = ಸತ್ಯ;
    clear ();
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
  }

  ಶೂನ್ಯ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ಯದಿ (!pages.length && !population) ಪ್ರತ್ಯಾಗಮ;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  ತಾರ್ಕಿಕ is_empty () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = pages.length;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ಯದಿ (!pages[i].is_empty ())
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  explicit operator ತಾರ್ಕಿಕ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !is_empty (); }

  uint32_t hash () ಧ್ರುವ
  {
    uint32_t h = 0;
    ಪ್ರತಿ (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      ಯದಿ (unlikely (page.is_empty ())) ಅನುವರ್ತ;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    ಪ್ರತ್ಯಾಗಮ h;
  }

  ಗುಪ್ತ:
  ಶೂನ್ಯ dirty () { population = UINT_MAX; }
  ಸಾರ್ವಜನಿಕ:

  ಶೂನ್ಯ add (hb_codepoint_t g)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (g == INVALID)) ಪ್ರತ್ಯಾಗಮ;
    dirty ();
    page_t *page = page_for (g, ಸತ್ಯ); ಯದಿ (unlikely (!page)) ಪ್ರತ್ಯಾಗಮ;
    page->add (g);
  }
  ಶೂನ್ಯ add_bits (hb_codepoint_t g, uint64_t bits)
  {
    ಯದಿ (unlikely (!successful) || unlikely (!bits)) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿಜ್ಞಾ (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, ಸತ್ಯ); ಯದಿ (unlikely (!page)) ಪ್ರತ್ಯಾಗಮ;
    page->add_bits (g, bits);
  }
  ತಾರ್ಕಿಕ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ಯದಿ (unlikely (a > b || a == INVALID || b == INVALID)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    dirty ();
    unsigned ಪೂರ್ಣಾಙ್ಕ ma = get_major (a);
    unsigned ಪೂರ್ಣಾಙ್ಕ mb = get_major (b);
    ಯದಿ (ma == mb)
    {
      page_t *page = page_for (a, ಸತ್ಯ); ಯದಿ (unlikely (!page)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      page->add_range (a, b);
    }
    ಅನ್ಯಥಾ
    {
      page_t *page = page_for (a, ಸತ್ಯ); ಯದಿ (unlikely (!page)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      page->add_range (a, major_start (ma + 1) - 1);

      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), ಸತ್ಯ); ಯದಿ (unlikely (!page)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	page->init1 ();
      }

      page = page_for (b, ಸತ್ಯ); ಯದಿ (unlikely (!page)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      page->add_range (major_start (mb), b);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ Type& StructAtOffsetUnaligned(ಧ್ರುವ ಶೂನ್ಯ *P, unsigned ಪೂರ್ಣಾಙ್ಕ offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    ಪ್ರತ್ಯಾಗಮ * reinterpret_cast<ಧ್ರುವ Type*> ((ಧ್ರುವ ವರ್ಣ *) P + offset);
#pragma GCC diagnostic ತೆಗೆ
  }

  template <typename T>
  ಶೂನ್ಯ set_array (ತಾರ್ಕಿಕ v, ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (!ಎಣಿಕೆ) ಪ್ರತ್ಯಾಗಮ;
    dirty ();
    hb_codepoint_t g = *array;
    ಯಾವತ್ (ಎಣಿಕೆ)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ m = get_major (g);
      page_t *page = page_for (g, v); ಯದಿ (unlikely (v && !page)) ಪ್ರತ್ಯಾಗಮ;
      unsigned ಪೂರ್ಣಾಙ್ಕ start = major_start (m);
      unsigned ಪೂರ್ಣಾಙ್ಕ end = major_start (m + 1);
      ಕುರು
      {
        ಯದಿ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	ಎಣಿಕೆ--;
      }
      ಯಾವತ್ (ಎಣಿಕೆ && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  ಶೂನ್ಯ add_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  { set_array (ಸತ್ಯ, array, ಎಣಿಕೆ, stride); }
  template <typename T>
  ಶೂನ್ಯ add_array (ಧ್ರುವ hb_array_t<ಧ್ರುವ T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  ಶೂನ್ಯ del_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  { set_array (ಅಸತ್ಯ, array, ಎಣಿಕೆ, stride); }
  template <typename T>
  ಶೂನ್ಯ del_array (ಧ್ರುವ hb_array_t<ಧ್ರುವ T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  ತಾರ್ಕಿಕ set_sorted_array (ತಾರ್ಕಿಕ v, ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ಯದಿ (unlikely (!ಎಣಿಕೆ)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ಯಾವತ್ (ಎಣಿಕೆ)
    {
      unsigned ಪೂರ್ಣಾಙ್ಕ m = get_major (g);
      page_t *page = page_for (g, v); ಯದಿ (unlikely (v && !page)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      unsigned ಪೂರ್ಣಾಙ್ಕ end = major_start (m + 1);
      ಕುರು
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ಯದಿ (g < last_g) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	last_g = g;

        ಯದಿ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	ಎಣಿಕೆ--;
      }
      ಯಾವತ್ (ಎಣಿಕೆ && (g = *array, g < end));
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  template <typename T>
  ತಾರ್ಕಿಕ add_sorted_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  { ಪ್ರತ್ಯಾಗಮ set_sorted_array (ಸತ್ಯ, array, ಎಣಿಕೆ, stride); }
  template <typename T>
  ತಾರ್ಕಿಕ add_sorted_array (ಧ್ರುವ hb_sorted_array_t<ಧ್ರುವ T>& arr) { ಪ್ರತ್ಯಾಗಮ add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  ತಾರ್ಕಿಕ del_sorted_array (ಧ್ರುವ T *array, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride=ಪರಿಮಾಣ(T))
  { ಪ್ರತ್ಯಾಗಮ set_sorted_array (ಅಸತ್ಯ, array, ಎಣಿಕೆ, stride); }
  template <typename T>
  ತಾರ್ಕಿಕ del_sorted_array (ಧ್ರುವ hb_sorted_array_t<ಧ್ರುವ T>& arr) { ಪ್ರತ್ಯಾಗಮ del_sorted_array (&arr, arr.len ()); }

  ಶೂನ್ಯ del (hb_codepoint_t g)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ;
    page_t *page = page_for (g);
    ಯದಿ (!page)
      ಪ್ರತ್ಯಾಗಮ;
    dirty ();
    page->del (g);
  }
  ಶೂನ್ಯ del_bits (hb_codepoint_t g, uint64_t bits)
  {
    ಯದಿ (unlikely (!successful) || unlikely (!bits)) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿಜ್ಞಾ (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    ಯದಿ (!page)
      ಪ್ರತ್ಯಾಗಮ;
    dirty ();
    page->del_bits (g, bits);
  }

  ಗುಪ್ತ:
  ಶೂನ್ಯ del_pages (ಪೂರ್ಣಾಙ್ಕ ds, ಪೂರ್ಣಾಙ್ಕ de)
  {
    ಯದಿ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      ಯದಿ (unlikely (!allocate_compact_workspace (compact_workspace))) ಪ್ರತ್ಯಾಗಮ;

      unsigned ಪೂರ್ಣಾಙ್ಕ write_index = 0;
      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < page_map.length; i++)
      {
	ಪೂರ್ಣಾಙ್ಕ m = (ಪೂರ್ಣಾಙ್ಕ) page_map.arrayZ[i].major;
	ಯದಿ (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  ಸಾರ್ವಜನಿಕ:
  ಶೂನ್ಯ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ;
    ಯದಿ (unlikely (a > b || a == INVALID)) ಪ್ರತ್ಯಾಗಮ;
    dirty ();
    unsigned ಪೂರ್ಣಾಙ್ಕ ma = get_major (a);
    unsigned ಪೂರ್ಣಾಙ್ಕ mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    ಪೂರ್ಣಾಙ್ಕ ds = (a == major_start (ma))? (ಪೂರ್ಣಾಙ್ಕ) ma: (ಪೂರ್ಣಾಙ್ಕ) (ma + 1);
    ಪೂರ್ಣಾಙ್ಕ de = (b + 1 == major_start (mb + 1))? (ಪೂರ್ಣಾಙ್ಕ) mb: ((ಪೂರ್ಣಾಙ್ಕ) mb - 1);
    ಯದಿ (ds > de || (ಪೂರ್ಣಾಙ್ಕ) ma < ds)
    {
      page_t *page = page_for (a);
      ಯದಿ (page)
      {
	ಯದಿ (ma == mb)
	  page->del_range (a, b);
	ಅನ್ಯಥಾ
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    ಯದಿ (de < (ಪೂರ್ಣಾಙ್ಕ) mb && ma != mb)
    {
      page_t *page = page_for (b);
      ಯದಿ (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  ತಾರ್ಕಿಕ get (hb_codepoint_t g) ಧ್ರುವ
  {
    ಧ್ರುವ page_t *page = page_for (g);
    ಯದಿ (!page)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ page->get (g);
  }
  ತಾರ್ಕಿಕ may_have (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (g); }

  /* Has interface. */
  ತಾರ್ಕಿಕ operator [] (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get (k); }
  ತಾರ್ಕಿಕ has (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ (*ಏತತ್)[k]; }
  /* Predicate. */
  ತಾರ್ಕಿಕ operator () (hb_codepoint_t k) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }
  hb_bit_set_t& operator << (ಧ್ರುವ hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_bit_set_t &other) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ na = pages.length;
    unsigned ಪೂರ್ಣಾಙ್ಕ nb = other.pages.length;

    unsigned ಪೂರ್ಣಾಙ್ಕ a = 0, b = 0;
    ಪ್ರತಿ (; a < na && b < nb; )
    {
      ಯದಿ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ಯದಿ (page_at (a).intersects (other.page_at (b)))
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
	a++;
	b++;
      }
      ಅನ್ಯಥಾ ಯದಿ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      ಅನ್ಯಥಾ
	b++;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ may_intersect (ಧ್ರುವ hb_bit_set_t &other) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ intersects (other); }

  ತಾರ್ಕಿಕ intersects (hb_codepoint_t first, hb_codepoint_t last) ಧ್ರುವ
  {
    hb_codepoint_t c = first - 1;
    ಪ್ರತ್ಯಾಗಮ next (&c) && c <= last;
  }
  ಶೂನ್ಯ set (ಧ್ರುವ hb_bit_set_t &other, ತಾರ್ಕಿಕ exact_size = ಅಸತ್ಯ)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = other.pages.length;
    ಯದಿ (unlikely (!resize  (ಎಣಿಕೆ, ಅಸತ್ಯ, exact_size)))
      ಪ್ರತ್ಯಾಗಮ;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  ತಾರ್ಕಿಕ is_equal (ಧ್ರುವ hb_bit_set_t &other) ಧ್ರುವ
  {
    ಯದಿ (has_population () && other.has_population () &&
	population != other.population)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned ಪೂರ್ಣಾಙ್ಕ na = pages.length;
    unsigned ಪೂರ್ಣಾಙ್ಕ nb = other.pages.length;

    unsigned ಪೂರ್ಣಾಙ್ಕ a = 0, b = 0;
    ಪ್ರತಿ (; a < na && b < nb; )
    {
      ಯದಿ (page_at (a).is_empty ()) { a++; ಅನುವರ್ತ; }
      ಯದಿ (other.page_at (b).is_empty ()) { b++; ಅನುವರ್ತ; }
      ಯದಿ (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      a++;
      b++;
    }
    ಪ್ರತಿ (; a < na; a++)
      ಯದಿ (!page_at (a).is_empty ()) { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
    ಪ್ರತಿ (; b < nb; b++)
      ಯದಿ (!other.page_at (b).is_empty ()) { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ is_subset (ಧ್ರುವ hb_bit_set_t &larger_set) ಧ್ರುವ
  {
    ಯದಿ (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ಯಾವತ್ (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      ಯದಿ (spm < lpm) {
        ಯದಿ (!sp.is_empty ())
          ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        spi++;
        ಅನುವರ್ತ;
      }

      ಯದಿ (lpm < spm) {
        lpi++;
        ಅನುವರ್ತ;
      }

      auto lp = larger_set.page_at (lpi);
      ಯದಿ (!sp.is_subset (lp))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      spi++;
      lpi++;
    }

    ಯಾವತ್ (spi < page_map.length)
      ಯದಿ (!page_at (spi++).is_empty ())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಗುಪ್ತ:
  ತಾರ್ಕಿಕ allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    ಯದಿ (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = ಅಸತ್ಯ;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  ಶೂನ್ಯ compact (hb_vector_t<unsigned>& workspace,
                unsigned ಪೂರ್ಣಾಙ್ಕ length)
  {
    ಪ್ರತಿಜ್ಞಾ(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    ಪ್ರತಿ (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  ಶೂನ್ಯ compact_pages (ಧ್ರುವ hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ write_index = 0;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < pages.length; i++)
    {
      ಯದಿ (old_index_to_page_map_index[i] == 0xFFFFFFFF) ಅನುವರ್ತ;

      ಯದಿ (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  ಸಾರ್ವಜನಿಕ:

  ಶೂನ್ಯ process_ (hb_bit_page_t::vector_t (*op) (ಧ್ರುವ hb_bit_page_t::vector_t &, ಧ್ರುವ hb_bit_page_t::vector_t &),
		 ತಾರ್ಕಿಕ passthru_ಎಡ, ತಾರ್ಕಿಕ passthru_ಬಲ,
		 ಧ್ರುವ hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    ಯದಿ (unlikely (!successful)) ಪ್ರತ್ಯಾಗಮ;

    dirty ();

    unsigned ಪೂರ್ಣಾಙ್ಕ na = pages.length;
    unsigned ಪೂರ್ಣಾಙ್ಕ nb = other.pages.length;
    unsigned ಪೂರ್ಣಾಙ್ಕ next_page = na;

    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0, new_ಎಣಿಕೆ = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ a = 0, b = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    ಯದಿ (!passthru_ಎಡ)
    {
      ತಾರ್ಕಿಕ allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      ಯದಿ (unlikely (!allocated))
      {
	successful = ಅಸತ್ಯ;
	ಪ್ರತ್ಯಾಗಮ;
      }
    }

    ಪ್ರತಿ (; a < na && b < nb; )
    {
      ಯದಿ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ಯದಿ (!passthru_ಎಡ)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ಯದಿ (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	ಎಣಿಕೆ++;
	a++;
	b++;
      }
      ಅನ್ಯಥಾ ಯದಿ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	ಯದಿ (passthru_ಎಡ)
	  ಎಣಿಕೆ++;
	a++;
      }
      ಅನ್ಯಥಾ
      {
	ಯದಿ (passthru_ಬಲ)
	  ಎಣಿಕೆ++;
	b++;
      }
    }
    ಯದಿ (passthru_ಎಡ)
      ಎಣಿಕೆ += na - a;
    ಯದಿ (passthru_ಬಲ)
      ಎಣಿಕೆ += nb - b;

    ಯದಿ (!passthru_ಎಡ)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    ಯದಿ (unlikely (!resize (ಎಣಿಕೆ)))
      ಪ್ರತ್ಯಾಗಮ;

    new_ಎಣಿಕೆ = ಎಣಿಕೆ;

    /* Process in-place backward. */
    a = na;
    b = nb;
    ಪ್ರತಿ (; a && b; )
    {
      ಯದಿ (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	ಎಣಿಕೆ--;
	page_map.arrayZ[ಎಣಿಕೆ] = page_map.arrayZ[a];
	page_at (ಎಣಿಕೆ).v = op (page_at (a).v, other.page_at (b).v);
	page_at (ಎಣಿಕೆ).dirty ();
      }
      ಅನ್ಯಥಾ ಯದಿ (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	ಯದಿ (passthru_ಎಡ)
	{
	  ಎಣಿಕೆ--;
	  page_map.arrayZ[ಎಣಿಕೆ] = page_map.arrayZ[a];
	}
      }
      ಅನ್ಯಥಾ
      {
	b--;
	ಯದಿ (passthru_ಬಲ)
	{
	  ಎಣಿಕೆ--;
	  page_map.arrayZ[ಎಣಿಕೆ].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[ಎಣಿಕೆ].index = next_page++;
	  page_at (ಎಣಿಕೆ) = other.page_at (b);
	}
      }
    }
    ಯದಿ (passthru_ಎಡ)
      ಯಾವತ್ (a)
      {
	a--;
	ಎಣಿಕೆ--;
	page_map.arrayZ[ಎಣಿಕೆ] = page_map.arrayZ[a];
      }
    ಯದಿ (passthru_ಬಲ)
      ಯಾವತ್ (b)
      {
	b--;
	ಎಣಿಕೆ--;
	page_map.arrayZ[ಎಣಿಕೆ].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[ಎಣಿಕೆ].index = next_page++;
	page_at (ಎಣಿಕೆ) = other.page_at (b);
      }
    ಪ್ರತಿಜ್ಞಾ (!ಎಣಿಕೆ);
    resize (new_ಎಣಿಕೆ);
  }
  template <typename Op>
  ಸ್ಥಿರ hb_bit_page_t::vector_t
  op_ (ಧ್ರುವ hb_bit_page_t::vector_t &a, ಧ್ರುವ hb_bit_page_t::vector_t &b)
  { ಪ್ರತ್ಯಾಗಮ Op{} (a, b); }
  template <typename Op>
  ಶೂನ್ಯ process (ಧ್ರುವ Op& op,
		ಧ್ರುವ hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  ಶೂನ್ಯ union_ (ಧ್ರುವ hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  ಶೂನ್ಯ intersect (ಧ್ರುವ hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  ಶೂನ್ಯ subtract (ಧ್ರುವ hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  ಶೂನ್ಯ symmetric_difference (ಧ್ರುವ hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  ತಾರ್ಕಿಕ next (hb_codepoint_t *codepoint) ಧ್ರುವ
  {
    ಯದಿ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ಪ್ರತ್ಯಾಗಮ *codepoint != INVALID;
    }

    ಧ್ರುವ auto* page_map_array = page_map.arrayZ;
    unsigned ಪೂರ್ಣಾಙ್ಕ major = get_major (*codepoint);
    unsigned ಪೂರ್ಣಾಙ್ಕ i = last_page_lookup;

    ಯದಿ (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      ಯದಿ (i >= page_map.length) {
        *codepoint = INVALID;
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }
      last_page_lookup = i;
    }

    ಧ್ರುವ auto* pages_array = pages.arrayZ;
    ಧ್ರುವ page_map_t &current = page_map_array[i];
    ಯದಿ (likely (current.major == major))
    {
      ಯದಿ (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
      i++;
    }

    ಪ್ರತಿ (; i < page_map.length; i++)
    {
      ಧ್ರುವ page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      ಯದಿ (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }
    *codepoint = INVALID;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ಧ್ರುವ
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ elt = 0;
    ಯದಿ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      ಯದಿ (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        ಗಚ್ಛ done;
      base += page_t::ELT_BITS;

      unsigned ಪೂರ್ಣಾಙ್ಕ major = get_major (base);
      i = last_page_lookup;
      ಯದಿ (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ಯದಿ (i >= page_map.length)
	  ಗಚ್ಛ done;
      }
      ಯದಿ (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    ಪ್ರತಿ (; i < page_map.length; i++, elt = 0)
    {
      ಧ್ರುವ page_map_t &map = page_map.arrayZ[i];
      ಧ್ರುವ page_t &page = pages.arrayZ[map.index];
      ಪ್ರತಿ (; elt < page_t::len (); elt++)
	ಯದಿ (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ previous (hb_codepoint_t *codepoint) ಧ್ರುವ
  {
    ಯದಿ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ಪ್ರತ್ಯಾಗಮ *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned ಪೂರ್ಣಾಙ್ಕ i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    ಯದಿ (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      ಯದಿ (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }
    i--;
    ಪ್ರತಿ (; (ಪೂರ್ಣಾಙ್ಕ) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      ಯದಿ (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }
    *codepoint = INVALID;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }
  ತಾರ್ಕಿಕ next_range (hb_codepoint_t *first, hb_codepoint_t *last) ಧ್ರುವ
  {
    hb_codepoint_t i;

    i = *last;
    ಯದಿ (!next (&i))
    {
      *last = *first = INVALID;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    *last = *first = i;

    ಧ್ರುವ auto* page_map_array = page_map.arrayZ;
    ಧ್ರುವ auto* pages_array = pages.arrayZ;
    unsigned ಪೂರ್ಣಾಙ್ಕ major = get_major (*last);
    unsigned ಪೂರ್ಣಾಙ್ಕ p_index = last_page_lookup;

    ಯದಿ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      ಪ್ರತಿಜ್ಞಾ(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ rem = page_remainder (*last);
    unsigned ಪೂರ್ಣಾಙ್ಕ elt = rem / page_t::ELT_BITS;
    unsigned ಪೂರ್ಣಾಙ್ಕ bit = rem & page_t::ELT_MASK;

    ಧ್ರುವ page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned ಪೂರ್ಣಾಙ್ಕ run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    ಯದಿ (run < page_t::ELT_BITS - bit)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    elt++;

    ಯಾವತ್ (ಸತ್ಯ)
    {
      ಪ್ರತಿ (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        ಯದಿ (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          ಅನುವರ್ತ;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }

      p_index++;


      ಯದಿ (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      ಧ್ರುವ auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  ತಾರ್ಕಿಕ previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ಧ್ರುವ
  {
    hb_codepoint_t i;

    i = *first;
    ಯದಿ (!previous (&i))
    {
      *last = *first = INVALID;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    *last = *first = i;

    ಧ್ರುವ auto* page_map_array = page_map.arrayZ;
    ಧ್ರುವ auto* pages_array = pages.arrayZ;
    unsigned ಪೂರ್ಣಾಙ್ಕ major = get_major (*first);
    unsigned ಪೂರ್ಣಾಙ್ಕ p_index = last_page_lookup;

    ಯದಿ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      ಪ್ರತಿಜ್ಞಾ(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ rem = page_remainder (*first);
    ಪೂರ್ಣಾಙ್ಕ elt = rem / page_t::ELT_BITS;
    unsigned ಪೂರ್ಣಾಙ್ಕ bit = rem & page_t::ELT_MASK;

    ಧ್ರುವ page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned ಪೂರ್ಣಾಙ್ಕ run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    ಯದಿ (run < bit + 1)
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    elt--;

    ಯಾವತ್ (ಸತ್ಯ)
    {
      ಪ್ರತಿ (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        ಯದಿ (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          ಅನುವರ್ತ;
        }
        uint64_t inv = ~w;
        unsigned ಪೂರ್ಣಾಙ್ಕ zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }


      ಯದಿ (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

      p_index--;
      ಧ್ರುವ auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned ಪೂರ್ಣಾಙ್ಕ    size) ಧ್ರುವ
  {
    // By default, start at the first bit of the first page of values.
    unsigned ಪೂರ್ಣಾಙ್ಕ start_page = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ start_page_ಮೌಲ್ಯ = 0;
    ಯದಿ (unlikely (codepoint != INVALID))
    {
      ಧ್ರುವ auto* page_map_array = page_map.arrayZ;
      unsigned ಪೂರ್ಣಾಙ್ಕ major = get_major (codepoint);
      unsigned ಪೂರ್ಣಾಙ್ಕ i = last_page_lookup;
      ಯದಿ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ಯದಿ (i >= page_map.length)
	  ಪ್ರತ್ಯಾಗಮ 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      ಯದಿ (page_map_array[i].major == major)
      {
        start_page_ಮೌಲ್ಯ = page_remainder (codepoint + 1);
        ಯದಿ (unlikely (start_page_ಮೌಲ್ಯ == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_ಮೌಲ್ಯ = 0;
        }
      }
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ initial_size = size;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ಪೂರ್ಣಾಙ್ಕ n = pages[page_map.arrayZ[i].index].ಲಿಖ (base, start_page_ಮೌಲ್ಯ, out, size);
      out += n;
      size -= n;
      start_page_ಮೌಲ್ಯ = 0;
    }
    ಪ್ರತ್ಯಾಗಮ initial_size - size;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned ಪೂರ್ಣಾಙ್ಕ    size) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned ಪೂರ್ಣಾಙ್ಕ start_page = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ start_page_ಮೌಲ್ಯ = 0;
    ಯದಿ (unlikely (codepoint != INVALID))
    {
      ಧ್ರುವ auto* page_map_array = page_map.arrayZ;
      unsigned ಪೂರ್ಣಾಙ್ಕ major = get_major (codepoint);
      unsigned ಪೂರ್ಣಾಙ್ಕ i = last_page_lookup;
      ಯದಿ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        ಯದಿ (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ಯಾವತ್ (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          ಪ್ರತ್ಯಾಗಮ initial_size - size;
        }
      }
      start_page = i;
      ಯದಿ (page_map_array[i].major == major)
      {
        start_page_ಮೌಲ್ಯ = page_remainder (codepoint + 1);
        ಯದಿ (unlikely (start_page_ಮೌಲ್ಯ == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_ಮೌಲ್ಯ = 0;
        }
      }
    }

    hb_codepoint_t next_ಮೌಲ್ಯ = codepoint + 1;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ಪೂರ್ಣಾಙ್ಕ n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_ಮೌಲ್ಯ, out, size, &next_ಮೌಲ್ಯ);
      out += n;
      size -= n;
      start_page_ಮೌಲ್ಯ = 0;
    }
    ಯಾವತ್ (next_ಮೌಲ್ಯ < HB_SET_ಮೌಲ್ಯ_INVALID && size) {
      *out++ = next_ಮೌಲ್ಯ++;
      size--;
    }
    ಪ್ರತ್ಯಾಗಮ initial_size - size;
  }

  ತಾರ್ಕಿಕ has_population () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ population != UINT_MAX; }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_population () ಧ್ರುವ
  {
    ಯದಿ (has_population ())
      ಪ್ರತ್ಯಾಗಮ population;

    unsigned ಪೂರ್ಣಾಙ್ಕ ತೆಗೆ = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = pages.length;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
      ತೆಗೆ += pages[i].get_population ();

    population = ತೆಗೆ;
    ಪ್ರತ್ಯಾಗಮ ತೆಗೆ;
  }
  ತಾರ್ಕಿಕ get_singleton (hb_codepoint_t *codepoint) ಧ್ರುವ
  {
    ಯದಿ (has_population ())
    {
      ಯದಿ (population != 1) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      *codepoint = get_min ();
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    hb_codepoint_t singleton = INVALID;
    ಪ್ರತಿ (ಧ್ರುವ auto &map : page_map)
    {
      ಧ್ರುವ auto &page = pages.arrayZ[map.index];
      ಪ್ರತಿ (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	ಯದಿ (!bits) ಅನುವರ್ತ;
	ಯದಿ (singleton != INVALID || (bits & (bits - 1)))
	  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    ಯದಿ (singleton == INVALID)
    {
      population = 0;
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    population = 1;
    *codepoint = singleton;
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
  hb_codepoint_t get_min () ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = pages.length;
    ಪ್ರತಿ (unsigned i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ auto& map = page_map.arrayZ[i];
      ಧ್ರುವ auto& page = pages.arrayZ[map.index];

      ಯದಿ (!page.is_empty ())
	ಪ್ರತ್ಯಾಗಮ map.major * page_t::PAGE_BITS + page.get_min ();
    }
    ಪ್ರತ್ಯಾಗಮ INVALID;
  }
  hb_codepoint_t get_max () ಧ್ರುವ
  {
    unsigned ಎಣಿಕೆ = pages.length;
    ಪ್ರತಿ (signed i = ಎಣಿಕೆ - 1; i >= 0; i--)
    {
      ಧ್ರುವ auto& map = page_map.arrayZ[(unsigned) i];
      ಧ್ರುವ auto& page = pages.arrayZ[map.index];

      ಯದಿ (!page.is_empty ())
	ಪ್ರತ್ಯಾಗಮ map.major * page_t::PAGE_BITS + page.get_max ();
    }
    ಪ್ರತ್ಯಾಗಮ INVALID;
  }

  ಸ್ಥಿರ constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  ಸಂರಚನಾ iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ is_sorted_iterator = ಸತ್ಯ;
    ಸ್ಥಿರ constexpr ತಾರ್ಕಿಕ has_fast_len = ಸತ್ಯ;
    iter_t (ಧ್ರುವ hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    ತಾರ್ಕಿಕ init = ಸತ್ಯ) : s (&s_), v (INVALID), l(0)
    {
      ಯದಿ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    ಪ್ರಕಾರನಿರ್ದೇಶ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v; }
    ತಾರ್ಕಿಕ __more__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ v != INVALID; }
    ಶೂನ್ಯ __next__ () { s->next (&v); ಯದಿ (l) l--; }
    ಶೂನ್ಯ __prev__ () { s->previous (&v); }
    unsigned __len__ () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ l; }
    iter_t end () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter_t (*s, ಅಸತ್ಯ); }
    ತಾರ್ಕಿಕ operator != (ಧ್ರುವ iter_t& o) ಧ್ರುವ
    { ಪ್ರತ್ಯಾಗಮ v != o.v; }

    ರಕ್ಷಿತ:
    ಧ್ರುವ hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter_t (*ಏತತ್); }
  operator iter_t () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ iter (); }

  ರಕ್ಷಿತ:

  page_t *page_for (hb_codepoint_t g, ತಾರ್ಕಿಕ insert = ಅಸತ್ಯ)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ಯದಿ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ಯದಿ (cached_page.major == major)
	ಪ್ರತ್ಯಾಗಮ &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    ಯದಿ (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      ಯದಿ (!insert)
        ಪ್ರತ್ಯಾಗಮ nullptr;

      ಯದಿ (unlikely (!resize (pages.length + 1)))
	ಪ್ರತ್ಯಾಗಮ nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    ಪ್ರತ್ಯಾಗಮ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  ಧ್ರುವ page_t *page_for (hb_codepoint_t g) ಧ್ರುವ
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ಯದಿ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ಯದಿ (cached_page.major == major)
	ಪ್ರತ್ಯಾಗಮ &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    ಯದಿ (!page_map.bfind (key, &i))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    last_page_lookup = i;
    ಪ್ರತ್ಯಾಗಮ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned ಪೂರ್ಣಾಙ್ಕ i)
  {
    ಪ್ರತಿಜ್ಞಾ (i < page_map.length);
    ಪ್ರತ್ಯಾಗಮ pages.arrayZ[page_map.arrayZ[i].index];
  }
  ಧ್ರುವ page_t &page_at (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  {
    ಪ್ರತಿಜ್ಞಾ (i < page_map.length);
    ಪ್ರತ್ಯಾಗಮ pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned ಪೂರ್ಣಾಙ್ಕ get_major (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ g >> page_t::PAGE_BITS_LOG_2; }
  unsigned ಪೂರ್ಣಾಙ್ಕ page_remainder (hb_codepoint_t g) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned ಪೂರ್ಣಾಙ್ಕ major) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
