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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-bit-page.hh"

ସଂରଚନା hb_bit_set_t
{
  hb_bit_set_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  ~hb_bit_set_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

  hb_bit_set_t (ଅଚଳ hb_bit_set_t& other) : hb_bit_set_t () { set (other, ସତ୍ୟ); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*ଏହି, other); }
  hb_bit_set_t& operator= (ଅଚଳ hb_bit_set_t& other) { set (other); ଫେରନ୍ତୁ *ଏହି; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*ଏହି, other); ଫେରନ୍ତୁ *ଏହି; }
  friend ଶୂନ୍ୟ swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    ଯଦି (likely (!a.successful || !b.successful))
      ଫେରନ୍ତୁ;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  ଶୂନ୍ୟ init ()
  {
    successful = ସତ୍ୟ;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  ଶୂନ୍ୟ fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  ସଂରଚନା page_map_t
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (ଅଚଳ page_map_t &o) ଅଚଳ { ଫେରନ୍ତୁ cmp (o.major); }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cmp (uint32_t o_major) ଅଚଳ { ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) o_major - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) major; }

    uint32_t major;
    uint32_t index;
  };

  ତାର୍କିକ successful = ସତ୍ୟ; /* Allocations successful */
  mutable unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  ଶୂନ୍ୟ err () { ଯଦି (successful) successful = ମିଥ୍ୟା; } /* TODO Remove */
  ତାର୍କିକ in_error () ଅଚଳ { ଫେରନ୍ତୁ !successful; }

  ତାର୍କିକ resize (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, ତାର୍କିକ clear = ସତ୍ୟ, ତାର୍କିକ exact_size = ମିଥ୍ୟା)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଯଦି (pages.length < ଗଣନା_2 && (unsigned) pages.allocated < ଗଣନା_2 && ଗଣନା_2 <= 2)
      exact_size = ସତ୍ୟ; // Most sets are small and local

    ଯଦି (unlikely (!pages.resize_full (ଗଣନା_2, clear, exact_size) ||
	!page_map.resize_full (ଗଣନା_2, clear, ମିଥ୍ୟା)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = ମିଥ୍ୟା;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ଶୂନ୍ୟ alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = ସତ୍ୟ;
    clear ();
    ଫେରନ୍ତୁ *ଏହି;
  }

  ଶୂନ୍ୟ clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ଯଦି (!pages.length && !population) ଫେରନ୍ତୁ;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  ତାର୍କିକ is_empty () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = pages.length;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      ଯଦି (!pages[i].is_empty ())
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  explicit operator ତାର୍କିକ () ଅଚଳ { ଫେରନ୍ତୁ !is_empty (); }

  uint32_t hash () ଅଚଳ
  {
    uint32_t h = 0;
    ପାଇଁ (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      ଯଦି (unlikely (page.is_empty ())) ଜାରି;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    ଫେରନ୍ତୁ h;
  }

  ବ୍ୟକ୍ତିଗତ:
  ଶୂନ୍ୟ dirty () { population = UINT_MAX; }
  ସର୍ବସାଧାରଣ:

  ଶୂନ୍ୟ add (hb_codepoint_t g)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (g == INVALID)) ଫେରନ୍ତୁ;
    dirty ();
    page_t *page = page_for (g, ସତ୍ୟ); ଯଦି (unlikely (!page)) ଫେରନ୍ତୁ;
    page->add (g);
  }
  ଶୂନ୍ୟ add_bits (hb_codepoint_t g, uint64_t bits)
  {
    ଯଦି (unlikely (!successful) || unlikely (!bits)) ଫେରନ୍ତୁ;
    ପ୍ରତିଜ୍ଞା (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, ସତ୍ୟ); ଯଦି (unlikely (!page)) ଫେରନ୍ତୁ;
    page->add_bits (g, bits);
  }
  ତାର୍କିକ add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ ସତ୍ୟ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ଯଦି (unlikely (a > b || a == INVALID || b == INVALID)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    dirty ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ma = get_major (a);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mb = get_major (b);
    ଯଦି (ma == mb)
    {
      page_t *page = page_for (a, ସତ୍ୟ); ଯଦି (unlikely (!page)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      page->add_range (a, b);
    }
    ଅନ୍ୟଥା
    {
      page_t *page = page_for (a, ସତ୍ୟ); ଯଦି (unlikely (!page)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      page->add_range (a, major_start (ma + 1) - 1);

      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), ସତ୍ୟ); ଯଦି (unlikely (!page)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	page->init1 ();
      }

      page = page_for (b, ସତ୍ୟ); ଯଦି (unlikely (!page)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      page->add_range (major_start (mb), b);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ Type& StructAtOffsetUnaligned(ଅଚଳ ଶୂନ୍ୟ *P, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    ଫେରନ୍ତୁ * reinterpret_cast<ଅଚଳ Type*> ((ଅଚଳ ଅକ୍ଷର *) P + offset);
#pragma GCC diagnostic କାଢ଼
  }

  template <typename T>
  ଶୂନ୍ୟ set_array (ତାର୍କିକ v, ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ;
    ଯଦି (!ଗଣନା_2) ଫେରନ୍ତୁ;
    dirty ();
    hb_codepoint_t g = *array;
    ଯେତେବେଳେ (ଗଣନା_2)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = get_major (g);
      page_t *page = page_for (g, v); ଯଦି (unlikely (v && !page)) ଫେରନ୍ତୁ;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = major_start (m);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = major_start (m + 1);
      କର
      {
        ଯଦି (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	ଗଣନା_2--;
      }
      ଯେତେବେଳେ (ଗଣନା_2 && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  ଶୂନ୍ୟ add_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  { set_array (ସତ୍ୟ, array, ଗଣନା_2, stride); }
  template <typename T>
  ଶୂନ୍ୟ add_array (ଅଚଳ hb_array_t<ଅଚଳ T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  ଶୂନ୍ୟ del_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  { set_array (ମିଥ୍ୟା, array, ଗଣନା_2, stride); }
  template <typename T>
  ଶୂନ୍ୟ del_array (ଅଚଳ hb_array_t<ଅଚଳ T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  ତାର୍କିକ set_sorted_array (ତାର୍କିକ v, ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ ସତ୍ୟ; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ଯଦି (unlikely (!ଗଣନା_2)) ଫେରନ୍ତୁ ସତ୍ୟ;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ଯେତେବେଳେ (ଗଣନା_2)
    {
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = get_major (g);
      page_t *page = page_for (g, v); ଯଦି (unlikely (v && !page)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = major_start (m + 1);
      କର
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ଯଦି (g < last_g) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	last_g = g;

        ଯଦି (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	ଗଣନା_2--;
      }
      ଯେତେବେଳେ (ଗଣନା_2 && (g = *array, g < end));
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  template <typename T>
  ତାର୍କିକ add_sorted_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  { ଫେରନ୍ତୁ set_sorted_array (ସତ୍ୟ, array, ଗଣନା_2, stride); }
  template <typename T>
  ତାର୍କିକ add_sorted_array (ଅଚଳ hb_sorted_array_t<ଅଚଳ T>& arr) { ଫେରନ୍ତୁ add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  ତାର୍କିକ del_sorted_array (ଅଚଳ T *array, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride=ଆକାର(T))
  { ଫେରନ୍ତୁ set_sorted_array (ମିଥ୍ୟା, array, ଗଣନା_2, stride); }
  template <typename T>
  ତାର୍କିକ del_sorted_array (ଅଚଳ hb_sorted_array_t<ଅଚଳ T>& arr) { ଫେରନ୍ତୁ del_sorted_array (&arr, arr.len ()); }

  ଶୂନ୍ୟ del (hb_codepoint_t g)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ;
    page_t *page = page_for (g);
    ଯଦି (!page)
      ଫେରନ୍ତୁ;
    dirty ();
    page->del (g);
  }
  ଶୂନ୍ୟ del_bits (hb_codepoint_t g, uint64_t bits)
  {
    ଯଦି (unlikely (!successful) || unlikely (!bits)) ଫେରନ୍ତୁ;
    ପ୍ରତିଜ୍ଞା (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    ଯଦି (!page)
      ଫେରନ୍ତୁ;
    dirty ();
    page->del_bits (g, bits);
  }

  ବ୍ୟକ୍ତିଗତ:
  ଶୂନ୍ୟ del_pages (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ds, ପୂର୍ଣ୍ଣସଂଖ୍ୟା de)
  {
    ଯଦି (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      ଯଦି (unlikely (!allocate_compact_workspace (compact_workspace))) ଫେରନ୍ତୁ;

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_index = 0;
      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < page_map.length; i++)
      {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) page_map.arrayZ[i].major;
	ଯଦି (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  ସର୍ବସାଧାରଣ:
  ଶୂନ୍ୟ del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ;
    ଯଦି (unlikely (a > b || a == INVALID)) ଫେରନ୍ତୁ;
    dirty ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ma = get_major (a);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ds = (a == major_start (ma))? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ma: (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) (ma + 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା de = (b + 1 == major_start (mb + 1))? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mb: ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mb - 1);
    ଯଦି (ds > de || (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ma < ds)
    {
      page_t *page = page_for (a);
      ଯଦି (page)
      {
	ଯଦି (ma == mb)
	  page->del_range (a, b);
	ଅନ୍ୟଥା
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    ଯଦି (de < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) mb && ma != mb)
    {
      page_t *page = page_for (b);
      ଯଦି (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  ତାର୍କିକ get (hb_codepoint_t g) ଅଚଳ
  {
    ଅଚଳ page_t *page = page_for (g);
    ଯଦି (!page)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ page->get (g);
  }
  ତାର୍କିକ may_have (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ get (g); }

  /* Has interface. */
  ତାର୍କିକ operator [] (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ get (k); }
  ତାର୍କିକ has (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ (*ଏହି)[k]; }
  /* Predicate. */
  ତାର୍କିକ operator () (hb_codepoint_t k) ଅଚଳ { ଫେରନ୍ତୁ has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); ଫେରନ୍ତୁ *ଏହି; }
  hb_bit_set_t& operator << (ଅଚଳ hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ଫେରନ୍ତୁ *ଏହି; }

  ତାର୍କିକ intersects (ଅଚଳ hb_bit_set_t &other) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା na = pages.length;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nb = other.pages.length;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = 0, b = 0;
    ପାଇଁ (; a < na && b < nb; )
    {
      ଯଦି (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ଯଦି (page_at (a).intersects (other.page_at (b)))
	  ଫେରନ୍ତୁ ସତ୍ୟ;
	a++;
	b++;
      }
      ଅନ୍ୟଥା ଯଦି (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      ଅନ୍ୟଥା
	b++;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ may_intersect (ଅଚଳ hb_bit_set_t &other) ଅଚଳ
  { ଫେରନ୍ତୁ intersects (other); }

  ତାର୍କିକ intersects (hb_codepoint_t first, hb_codepoint_t last) ଅଚଳ
  {
    hb_codepoint_t c = first - 1;
    ଫେରନ୍ତୁ next (&c) && c <= last;
  }
  ଶୂନ୍ୟ set (ଅଚଳ hb_bit_set_t &other, ତାର୍କିକ exact_size = ମିଥ୍ୟା)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = other.pages.length;
    ଯଦି (unlikely (!resize  (ଗଣନା_2, ମିଥ୍ୟା, exact_size)))
      ଫେରନ୍ତୁ;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  ତାର୍କିକ is_equal (ଅଚଳ hb_bit_set_t &other) ଅଚଳ
  {
    ଯଦି (has_population () && other.has_population () &&
	population != other.population)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା na = pages.length;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nb = other.pages.length;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = 0, b = 0;
    ପାଇଁ (; a < na && b < nb; )
    {
      ଯଦି (page_at (a).is_empty ()) { a++; ଜାରି; }
      ଯଦି (other.page_at (b).is_empty ()) { b++; ଜାରି; }
      ଯଦି (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      a++;
      b++;
    }
    ପାଇଁ (; a < na; a++)
      ଯଦି (!page_at (a).is_empty ()) { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
    ପାଇଁ (; b < nb; b++)
      ଯଦି (!other.page_at (b).is_empty ()) { ଫେରନ୍ତୁ ମିଥ୍ୟା; }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ is_subset (ଅଚଳ hb_bit_set_t &larger_set) ଅଚଳ
  {
    ଯଦି (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ଯେତେବେଳେ (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      ଯଦି (spm < lpm) {
        ଯଦି (!sp.is_empty ())
          ଫେରନ୍ତୁ ମିଥ୍ୟା;
        spi++;
        ଜାରି;
      }

      ଯଦି (lpm < spm) {
        lpi++;
        ଜାରି;
      }

      auto lp = larger_set.page_at (lpi);
      ଯଦି (!sp.is_subset (lp))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      spi++;
      lpi++;
    }

    ଯେତେବେଳେ (spi < page_map.length)
      ଯଦି (!page_at (spi++).is_empty ())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ବ୍ୟକ୍ତିଗତ:
  ତାର୍କିକ allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    ଯଦି (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = ମିଥ୍ୟା;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  ଶୂନ୍ୟ compact (hb_vector_t<unsigned>& workspace,
                unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length)
  {
    ପ୍ରତିଜ୍ଞା(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    ପାଇଁ (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  ଶୂନ୍ୟ compact_pages (ଅଚଳ hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_index = 0;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < pages.length; i++)
    {
      ଯଦି (old_index_to_page_map_index[i] == 0xFFFFFFFF) ଜାରି;

      ଯଦି (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  ସର୍ବସାଧାରଣ:

  ଶୂନ୍ୟ process_ (hb_bit_page_t::vector_t (*op) (ଅଚଳ hb_bit_page_t::vector_t &, ଅଚଳ hb_bit_page_t::vector_t &),
		 ତାର୍କିକ passthru_ବାମ, ତାର୍କିକ passthru_ଡାହାଣ,
		 ଅଚଳ hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    ଯଦି (unlikely (!successful)) ଫେରନ୍ତୁ;

    dirty ();

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା na = pages.length;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା nb = other.pages.length;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_page = na;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0, new_ଗଣନା = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା a = 0, b = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    ଯଦି (!passthru_ବାମ)
    {
      ତାର୍କିକ allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      ଯଦି (unlikely (!allocated))
      {
	successful = ମିଥ୍ୟା;
	ଫେରନ୍ତୁ;
      }
    }

    ପାଇଁ (; a < na && b < nb; )
    {
      ଯଦି (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ଯଦି (!passthru_ବାମ)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ଯଦି (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	ଗଣନା_2++;
	a++;
	b++;
      }
      ଅନ୍ୟଥା ଯଦି (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	ଯଦି (passthru_ବାମ)
	  ଗଣନା_2++;
	a++;
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (passthru_ଡାହାଣ)
	  ଗଣନା_2++;
	b++;
      }
    }
    ଯଦି (passthru_ବାମ)
      ଗଣନା_2 += na - a;
    ଯଦି (passthru_ଡାହାଣ)
      ଗଣନା_2 += nb - b;

    ଯଦି (!passthru_ବାମ)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    ଯଦି (unlikely (!resize (ଗଣନା_2)))
      ଫେରନ୍ତୁ;

    new_ଗଣନା = ଗଣନା_2;

    /* Process in-place backward. */
    a = na;
    b = nb;
    ପାଇଁ (; a && b; )
    {
      ଯଦି (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	ଗଣନା_2--;
	page_map.arrayZ[ଗଣନା_2] = page_map.arrayZ[a];
	page_at (ଗଣନା_2).v = op (page_at (a).v, other.page_at (b).v);
	page_at (ଗଣନା_2).dirty ();
      }
      ଅନ୍ୟଥା ଯଦି (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	ଯଦି (passthru_ବାମ)
	{
	  ଗଣନା_2--;
	  page_map.arrayZ[ଗଣନା_2] = page_map.arrayZ[a];
	}
      }
      ଅନ୍ୟଥା
      {
	b--;
	ଯଦି (passthru_ଡାହାଣ)
	{
	  ଗଣନା_2--;
	  page_map.arrayZ[ଗଣନା_2].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[ଗଣନା_2].index = next_page++;
	  page_at (ଗଣନା_2) = other.page_at (b);
	}
      }
    }
    ଯଦି (passthru_ବାମ)
      ଯେତେବେଳେ (a)
      {
	a--;
	ଗଣନା_2--;
	page_map.arrayZ[ଗଣନା_2] = page_map.arrayZ[a];
      }
    ଯଦି (passthru_ଡାହାଣ)
      ଯେତେବେଳେ (b)
      {
	b--;
	ଗଣନା_2--;
	page_map.arrayZ[ଗଣନା_2].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[ଗଣନା_2].index = next_page++;
	page_at (ଗଣନା_2) = other.page_at (b);
      }
    ପ୍ରତିଜ୍ଞା (!ଗଣନା_2);
    resize (new_ଗଣନା);
  }
  template <typename Op>
  ସ୍ଥିର hb_bit_page_t::vector_t
  op_ (ଅଚଳ hb_bit_page_t::vector_t &a, ଅଚଳ hb_bit_page_t::vector_t &b)
  { ଫେରନ୍ତୁ Op{} (a, b); }
  template <typename Op>
  ଶୂନ୍ୟ process (ଅଚଳ Op& op,
		ଅଚଳ hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  ଶୂନ୍ୟ union_ (ଅଚଳ hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  ଶୂନ୍ୟ intersect (ଅଚଳ hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  ଶୂନ୍ୟ subtract (ଅଚଳ hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  ଶୂନ୍ୟ symmetric_difference (ଅଚଳ hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  ତାର୍କିକ next (hb_codepoint_t *codepoint) ଅଚଳ
  {
    ଯଦି (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ଫେରନ୍ତୁ *codepoint != INVALID;
    }

    ଅଚଳ auto* page_map_array = page_map.arrayZ;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା major = get_major (*codepoint);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = last_page_lookup;

    ଯଦି (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      ଯଦି (i >= page_map.length) {
        *codepoint = INVALID;
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }
      last_page_lookup = i;
    }

    ଅଚଳ auto* pages_array = pages.arrayZ;
    ଅଚଳ page_map_t &current = page_map_array[i];
    ଯଦି (likely (current.major == major))
    {
      ଯଦି (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        ଫେରନ୍ତୁ ସତ୍ୟ;
      }
      i++;
    }

    ପାଇଁ (; i < page_map.length; i++)
    {
      ଅଚଳ page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      ଯଦି (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }
    *codepoint = INVALID;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ଅଚଳ
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା elt = 0;
    ଯଦି (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      ଯଦି (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        ଯାଅ done;
      base += page_t::ELT_BITS;

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା major = get_major (base);
      i = last_page_lookup;
      ଯଦି (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ଯଦି (i >= page_map.length)
	  ଯାଅ done;
      }
      ଯଦି (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    ପାଇଁ (; i < page_map.length; i++, elt = 0)
    {
      ଅଚଳ page_map_t &map = page_map.arrayZ[i];
      ଅଚଳ page_t &page = pages.arrayZ[map.index];
      ପାଇଁ (; elt < page_t::len (); elt++)
	ଯଦି (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  ଫେରନ୍ତୁ ସତ୍ୟ;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ previous (hb_codepoint_t *codepoint) ଅଚଳ
  {
    ଯଦି (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ଫେରନ୍ତୁ *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    ଯଦି (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      ଯଦି (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }
    i--;
    ପାଇଁ (; (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      ଯଦି (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }
    *codepoint = INVALID;
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }
  ତାର୍କିକ next_range (hb_codepoint_t *first, hb_codepoint_t *last) ଅଚଳ
  {
    hb_codepoint_t i;

    i = *last;
    ଯଦି (!next (&i))
    {
      *last = *first = INVALID;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    *last = *first = i;

    ଅଚଳ auto* page_map_array = page_map.arrayZ;
    ଅଚଳ auto* pages_array = pages.arrayZ;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା major = get_major (*last);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା p_index = last_page_lookup;

    ଯଦି (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      ପ୍ରତିଜ୍ଞା(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rem = page_remainder (*last);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା elt = rem / page_t::ELT_BITS;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit = rem & page_t::ELT_MASK;

    ଅଚଳ page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    ଯଦି (run < page_t::ELT_BITS - bit)
      ଫେରନ୍ତୁ ସତ୍ୟ;

    elt++;

    ଯେତେବେଳେ (ସତ୍ୟ)
    {
      ପାଇଁ (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        ଯଦି (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          ଜାରି;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        ଫେରନ୍ତୁ ସତ୍ୟ;
      }

      p_index++;


      ଯଦି (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        ଫେରନ୍ତୁ ସତ୍ୟ;

      ଅଚଳ auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  ତାର୍କିକ previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ଅଚଳ
  {
    hb_codepoint_t i;

    i = *first;
    ଯଦି (!previous (&i))
    {
      *last = *first = INVALID;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    *last = *first = i;

    ଅଚଳ auto* page_map_array = page_map.arrayZ;
    ଅଚଳ auto* pages_array = pages.arrayZ;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା major = get_major (*first);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା p_index = last_page_lookup;

    ଯଦି (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      ପ୍ରତିଜ୍ଞା(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା rem = page_remainder (*first);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା elt = rem / page_t::ELT_BITS;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା bit = rem & page_t::ELT_MASK;

    ଅଚଳ page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    ଯଦି (run < bit + 1)
      ଫେରନ୍ତୁ ସତ୍ୟ;

    elt--;

    ଯେତେବେଳେ (ସତ୍ୟ)
    {
      ପାଇଁ (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        ଯଦି (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          ଜାରି;
        }
        uint64_t inv = ~w;
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        ଫେରନ୍ତୁ ସତ୍ୟ;
      }


      ଯଦି (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        ଫେରନ୍ତୁ ସତ୍ୟ;

      p_index--;
      ଅଚଳ auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    size) ଅଚଳ
  {
    // By default, start at the first bit of the first page of values.
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_page = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_page_ମୂଲ୍ୟ = 0;
    ଯଦି (unlikely (codepoint != INVALID))
    {
      ଅଚଳ auto* page_map_array = page_map.arrayZ;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା major = get_major (codepoint);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = last_page_lookup;
      ଯଦି (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ଯଦି (i >= page_map.length)
	  ଫେରନ୍ତୁ 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      ଯଦି (page_map_array[i].major == major)
      {
        start_page_ମୂଲ୍ୟ = page_remainder (codepoint + 1);
        ଯଦି (unlikely (start_page_ମୂଲ୍ୟ == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_ମୂଲ୍ୟ = 0;
        }
      }
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା initial_size = size;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = pages[page_map.arrayZ[i].index].ଲିଖ (base, start_page_ମୂଲ୍ୟ, out, size);
      out += n;
      size -= n;
      start_page_ମୂଲ୍ୟ = 0;
    }
    ଫେରନ୍ତୁ initial_size - size;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା    size) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_page = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_page_ମୂଲ୍ୟ = 0;
    ଯଦି (unlikely (codepoint != INVALID))
    {
      ଅଚଳ auto* page_map_array = page_map.arrayZ;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା major = get_major (codepoint);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = last_page_lookup;
      ଯଦି (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        ଯଦି (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ଯେତେବେଳେ (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          ଫେରନ୍ତୁ initial_size - size;
        }
      }
      start_page = i;
      ଯଦି (page_map_array[i].major == major)
      {
        start_page_ମୂଲ୍ୟ = page_remainder (codepoint + 1);
        ଯଦି (unlikely (start_page_ମୂଲ୍ୟ == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_ମୂଲ୍ୟ = 0;
        }
      }
    }

    hb_codepoint_t next_ମୂଲ୍ୟ = codepoint + 1;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_ମୂଲ୍ୟ, out, size, &next_ମୂଲ୍ୟ);
      out += n;
      size -= n;
      start_page_ମୂଲ୍ୟ = 0;
    }
    ଯେତେବେଳେ (next_ମୂଲ୍ୟ < HB_SET_ମୂଲ୍ୟ_INVALID && size) {
      *out++ = next_ମୂଲ୍ୟ++;
      size--;
    }
    ଫେରନ୍ତୁ initial_size - size;
  }

  ତାର୍କିକ has_population () ଅଚଳ { ଫେରନ୍ତୁ population != UINT_MAX; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_population () ଅଚଳ
  {
    ଯଦି (has_population ())
      ଫେରନ୍ତୁ population;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା କାଢ଼ = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = pages.length;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
      କାଢ଼ += pages[i].get_population ();

    population = କାଢ଼;
    ଫେରନ୍ତୁ କାଢ଼;
  }
  ତାର୍କିକ get_singleton (hb_codepoint_t *codepoint) ଅଚଳ
  {
    ଯଦି (has_population ())
    {
      ଯଦି (population != 1) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      *codepoint = get_min ();
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    hb_codepoint_t singleton = INVALID;
    ପାଇଁ (ଅଚଳ auto &map : page_map)
    {
      ଅଚଳ auto &page = pages.arrayZ[map.index];
      ପାଇଁ (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	ଯଦି (!bits) ଜାରି;
	ଯଦି (singleton != INVALID || (bits & (bits - 1)))
	  ଫେରନ୍ତୁ ମିଥ୍ୟା;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    ଯଦି (singleton == INVALID)
    {
      population = 0;
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    population = 1;
    *codepoint = singleton;
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
  hb_codepoint_t get_min () ଅଚଳ
  {
    unsigned ଗଣନା_2 = pages.length;
    ପାଇଁ (unsigned i = 0; i < ଗଣନା_2; i++)
    {
      ଅଚଳ auto& map = page_map.arrayZ[i];
      ଅଚଳ auto& page = pages.arrayZ[map.index];

      ଯଦି (!page.is_empty ())
	ଫେରନ୍ତୁ map.major * page_t::PAGE_BITS + page.get_min ();
    }
    ଫେରନ୍ତୁ INVALID;
  }
  hb_codepoint_t get_max () ଅଚଳ
  {
    unsigned ଗଣନା_2 = pages.length;
    ପାଇଁ (signed i = ଗଣନା_2 - 1; i >= 0; i--)
    {
      ଅଚଳ auto& map = page_map.arrayZ[(unsigned) i];
      ଅଚଳ auto& page = pages.arrayZ[map.index];

      ଯଦି (!page.is_empty ())
	ଫେରନ୍ତୁ map.major * page_t::PAGE_BITS + page.get_max ();
    }
    ଫେରନ୍ତୁ INVALID;
  }

  ସ୍ଥିର constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  ସଂରଚନା iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    ସ୍ଥିର constexpr ତାର୍କିକ is_sorted_iterator = ସତ୍ୟ;
    ସ୍ଥିର constexpr ତାର୍କିକ has_fast_len = ସତ୍ୟ;
    iter_t (ଅଚଳ hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    ତାର୍କିକ init = ସତ୍ୟ) : s (&s_), v (INVALID), l(0)
    {
      ଯଦି (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    ପ୍ରକାରବ୍ୟାଖ୍ୟା hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ଅଚଳ { ଫେରନ୍ତୁ v; }
    ତାର୍କିକ __more__ () ଅଚଳ { ଫେରନ୍ତୁ v != INVALID; }
    ଶୂନ୍ୟ __next__ () { s->next (&v); ଯଦି (l) l--; }
    ଶୂନ୍ୟ __prev__ () { s->previous (&v); }
    unsigned __len__ () ଅଚଳ { ଫେରନ୍ତୁ l; }
    iter_t end () ଅଚଳ { ଫେରନ୍ତୁ iter_t (*s, ମିଥ୍ୟା); }
    ତାର୍କିକ operator != (ଅଚଳ iter_t& o) ଅଚଳ
    { ଫେରନ୍ତୁ v != o.v; }

    ସୁରକ୍ଷିତ:
    ଅଚଳ hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () ଅଚଳ { ଫେରନ୍ତୁ iter_t (*ଏହି); }
  operator iter_t () ଅଚଳ { ଫେରନ୍ତୁ iter (); }

  ସୁରକ୍ଷିତ:

  page_t *page_for (hb_codepoint_t g, ତାର୍କିକ insert = ମିଥ୍ୟା)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ଯଦି (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ଯଦି (cached_page.major == major)
	ଫେରନ୍ତୁ &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    ଯଦି (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      ଯଦି (!insert)
        ଫେରନ୍ତୁ nullptr;

      ଯଦି (unlikely (!resize (pages.length + 1)))
	ଫେରନ୍ତୁ nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    ଫେରନ୍ତୁ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  ଅଚଳ page_t *page_for (hb_codepoint_t g) ଅଚଳ
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ଯଦି (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ଯଦି (cached_page.major == major)
	ଫେରନ୍ତୁ &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    ଯଦି (!page_map.bfind (key, &i))
      ଫେରନ୍ତୁ nullptr;

    last_page_lookup = i;
    ଫେରନ୍ତୁ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i)
  {
    ପ୍ରତିଜ୍ଞା (i < page_map.length);
    ଫେରନ୍ତୁ pages.arrayZ[page_map.arrayZ[i].index];
  }
  ଅଚଳ page_t &page_at (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  {
    ପ୍ରତିଜ୍ଞା (i < page_map.length);
    ଫେରନ୍ତୁ pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_major (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ g >> page_t::PAGE_BITS_LOG_2; }
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା page_remainder (hb_codepoint_t g) ଅଚଳ { ଫେରନ୍ତୁ g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା major) ଅଚଳ { ଫେରନ୍ତୁ major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
