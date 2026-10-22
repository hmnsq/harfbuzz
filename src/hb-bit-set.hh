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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-bit-page.hh"

সংরচনা hb_bit_set_t
{
  hb_bit_set_t () = স্বভাব;
  ~hb_bit_set_t () = স্বভাব;

  hb_bit_set_t (ধ্রুব hb_bit_set_t& other) : hb_bit_set_t () { set (other, সত্য); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*এতত্, other); }
  hb_bit_set_t& operator= (ধ্রুব hb_bit_set_t& other) { set (other); প্রত্যাগম *এতত্; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*এতত্, other); প্রত্যাগম *এতত্; }
  friend শূন্য swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    যদি (likely (!a.successful || !b.successful))
      প্রত্যাগম;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  শূন্য init ()
  {
    successful = সত্য;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  শূন্য fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  সংরচনা page_map_t
  {
    পূর্ণাঙ্ক cmp (ধ্রুব page_map_t &o) ধ্রুব { প্রত্যাগম cmp (o.major); }
    পূর্ণাঙ্ক cmp (uint32_t o_major) ধ্রুব { প্রত্যাগম (পূর্ণাঙ্ক) o_major - (পূর্ণাঙ্ক) major; }

    uint32_t major;
    uint32_t index;
  };

  তার্কিক successful = সত্য; /* Allocations successful */
  mutable unsigned পূর্ণাঙ্ক population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  শূন্য err () { যদি (successful) successful = অসত্য; } /* TODO Remove */
  তার্কিক in_error () ধ্রুব { প্রত্যাগম !successful; }

  তার্কিক resize (unsigned পূর্ণাঙ্ক গুনতি, তার্কিক clear = সত্য, তার্কিক exact_size = অসত্য)
  {
    যদি (unlikely (!successful)) প্রত্যাগম অসত্য;

    যদি (pages.length < গুনতি && (unsigned) pages.allocated < গুনতি && গুনতি <= 2)
      exact_size = সত্য; // Most sets are small and local

    যদি (unlikely (!pages.resize_full (গুনতি, clear, exact_size) ||
	!page_map.resize_full (গুনতি, clear, অসত্য)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = অসত্য;
      প্রত্যাগম অসত্য;
    }
    প্রত্যাগম সত্য;
  }

  শূন্য alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = সত্য;
    clear ();
    প্রত্যাগম *এতত্;
  }

  শূন্য clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    যদি (!pages.length && !population) প্রত্যাগম;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  তার্কিক is_empty () ধ্রুব
  {
    unsigned পূর্ণাঙ্ক গুনতি = pages.length;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      যদি (!pages[i].is_empty ())
	প্রত্যাগম অসত্য;
    প্রত্যাগম সত্য;
  }
  explicit operator তার্কিক () ধ্রুব { প্রত্যাগম !is_empty (); }

  uint32_t hash () ধ্রুব
  {
    uint32_t h = 0;
    প্রতি (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      যদি (unlikely (page.is_empty ())) অনুবর্ত;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    প্রত্যাগম h;
  }

  গুপ্ত:
  শূন্য dirty () { population = UINT_MAX; }
  সার্বজনিক:

  শূন্য add (hb_codepoint_t g)
  {
    যদি (unlikely (!successful)) প্রত্যাগম;
    যদি (unlikely (g == INVALID)) প্রত্যাগম;
    dirty ();
    page_t *page = page_for (g, সত্য); যদি (unlikely (!page)) প্রত্যাগম;
    page->add (g);
  }
  শূন্য add_bits (hb_codepoint_t g, uint64_t bits)
  {
    যদি (unlikely (!successful) || unlikely (!bits)) প্রত্যাগম;
    প্রতিজ্ঞা (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, সত্য); যদি (unlikely (!page)) প্রত্যাগম;
    page->add_bits (g, bits);
  }
  তার্কিক add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    যদি (unlikely (!successful)) প্রত্যাগম সত্য; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    যদি (unlikely (a > b || a == INVALID || b == INVALID)) প্রত্যাগম অসত্য;
    dirty ();
    unsigned পূর্ণাঙ্ক ma = get_major (a);
    unsigned পূর্ণাঙ্ক mb = get_major (b);
    যদি (ma == mb)
    {
      page_t *page = page_for (a, সত্য); যদি (unlikely (!page)) প্রত্যাগম অসত্য;
      page->add_range (a, b);
    }
    অন্যথা
    {
      page_t *page = page_for (a, সত্য); যদি (unlikely (!page)) প্রত্যাগম অসত্য;
      page->add_range (a, major_start (ma + 1) - 1);

      প্রতি (unsigned পূর্ণাঙ্ক m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), সত্য); যদি (unlikely (!page)) প্রত্যাগম অসত্য;
	page->init1 ();
      }

      page = page_for (b, সত্য); যদি (unlikely (!page)) প্রত্যাগম অসত্য;
      page->add_range (major_start (mb), b);
    }
    প্রত্যাগম সত্য;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  স্থির অন্তঃস্থ ধ্রুব Type& StructAtOffsetUnaligned(ধ্রুব শূন্য *P, unsigned পূর্ণাঙ্ক offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    প্রত্যাগম * reinterpret_cast<ধ্রুব Type*> ((ধ্রুব বর্ণ *) P + offset);
#pragma GCC diagnostic বের_করো
  }

  template <typename T>
  শূন্য set_array (তার্কিক v, ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  {
    যদি (unlikely (!successful)) প্রত্যাগম;
    যদি (!গুনতি) প্রত্যাগম;
    dirty ();
    hb_codepoint_t g = *array;
    যাবত্ (গুনতি)
    {
      unsigned পূর্ণাঙ্ক m = get_major (g);
      page_t *page = page_for (g, v); যদি (unlikely (v && !page)) প্রত্যাগম;
      unsigned পূর্ণাঙ্ক start = major_start (m);
      unsigned পূর্ণাঙ্ক end = major_start (m + 1);
      কুরু
      {
        যদি (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	গুনতি--;
      }
      যাবত্ (গুনতি && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  শূন্য add_array (ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  { set_array (সত্য, array, গুনতি, stride); }
  template <typename T>
  শূন্য add_array (ধ্রুব hb_array_t<ধ্রুব T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  শূন্য del_array (ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  { set_array (অসত্য, array, গুনতি, stride); }
  template <typename T>
  শূন্য del_array (ধ্রুব hb_array_t<ধ্রুব T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  তার্কিক set_sorted_array (তার্কিক v, ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  {
    যদি (unlikely (!successful)) প্রত্যাগম সত্য; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    যদি (unlikely (!গুনতি)) প্রত্যাগম সত্য;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    যাবত্ (গুনতি)
    {
      unsigned পূর্ণাঙ্ক m = get_major (g);
      page_t *page = page_for (g, v); যদি (unlikely (v && !page)) প্রত্যাগম অসত্য;
      unsigned পূর্ণাঙ্ক end = major_start (m + 1);
      কুরু
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	যদি (g < last_g) প্রত্যাগম অসত্য;
	last_g = g;

        যদি (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	গুনতি--;
      }
      যাবত্ (গুনতি && (g = *array, g < end));
    }
    প্রত্যাগম সত্য;
  }

  template <typename T>
  তার্কিক add_sorted_array (ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  { প্রত্যাগম set_sorted_array (সত্য, array, গুনতি, stride); }
  template <typename T>
  তার্কিক add_sorted_array (ধ্রুব hb_sorted_array_t<ধ্রুব T>& arr) { প্রত্যাগম add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  তার্কিক del_sorted_array (ধ্রুব T *array, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride=পরিমাণ(T))
  { প্রত্যাগম set_sorted_array (অসত্য, array, গুনতি, stride); }
  template <typename T>
  তার্কিক del_sorted_array (ধ্রুব hb_sorted_array_t<ধ্রুব T>& arr) { প্রত্যাগম del_sorted_array (&arr, arr.len ()); }

  শূন্য del (hb_codepoint_t g)
  {
    যদি (unlikely (!successful)) প্রত্যাগম;
    page_t *page = page_for (g);
    যদি (!page)
      প্রত্যাগম;
    dirty ();
    page->del (g);
  }
  শূন্য del_bits (hb_codepoint_t g, uint64_t bits)
  {
    যদি (unlikely (!successful) || unlikely (!bits)) প্রত্যাগম;
    প্রতিজ্ঞা (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    যদি (!page)
      প্রত্যাগম;
    dirty ();
    page->del_bits (g, bits);
  }

  গুপ্ত:
  শূন্য del_pages (পূর্ণাঙ্ক ds, পূর্ণাঙ্ক de)
  {
    যদি (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      যদি (unlikely (!allocate_compact_workspace (compact_workspace))) প্রত্যাগম;

      unsigned পূর্ণাঙ্ক write_index = 0;
      প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < page_map.length; i++)
      {
	পূর্ণাঙ্ক m = (পূর্ণাঙ্ক) page_map.arrayZ[i].major;
	যদি (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  সার্বজনিক:
  শূন্য del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    যদি (unlikely (!successful)) প্রত্যাগম;
    যদি (unlikely (a > b || a == INVALID)) প্রত্যাগম;
    dirty ();
    unsigned পূর্ণাঙ্ক ma = get_major (a);
    unsigned পূর্ণাঙ্ক mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    পূর্ণাঙ্ক ds = (a == major_start (ma))? (পূর্ণাঙ্ক) ma: (পূর্ণাঙ্ক) (ma + 1);
    পূর্ণাঙ্ক de = (b + 1 == major_start (mb + 1))? (পূর্ণাঙ্ক) mb: ((পূর্ণাঙ্ক) mb - 1);
    যদি (ds > de || (পূর্ণাঙ্ক) ma < ds)
    {
      page_t *page = page_for (a);
      যদি (page)
      {
	যদি (ma == mb)
	  page->del_range (a, b);
	অন্যথা
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    যদি (de < (পূর্ণাঙ্ক) mb && ma != mb)
    {
      page_t *page = page_for (b);
      যদি (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  তার্কিক get (hb_codepoint_t g) ধ্রুব
  {
    ধ্রুব page_t *page = page_for (g);
    যদি (!page)
      প্রত্যাগম অসত্য;
    প্রত্যাগম page->get (g);
  }
  তার্কিক may_have (hb_codepoint_t g) ধ্রুব { প্রত্যাগম get (g); }

  /* Has interface. */
  তার্কিক operator [] (hb_codepoint_t k) ধ্রুব { প্রত্যাগম get (k); }
  তার্কিক has (hb_codepoint_t k) ধ্রুব { প্রত্যাগম (*এতত্)[k]; }
  /* Predicate. */
  তার্কিক operator () (hb_codepoint_t k) ধ্রুব { প্রত্যাগম has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); প্রত্যাগম *এতত্; }
  hb_bit_set_t& operator << (ধ্রুব hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); প্রত্যাগম *এতত্; }

  তার্কিক intersects (ধ্রুব hb_bit_set_t &other) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক na = pages.length;
    unsigned পূর্ণাঙ্ক nb = other.pages.length;

    unsigned পূর্ণাঙ্ক a = 0, b = 0;
    প্রতি (; a < na && b < nb; )
    {
      যদি (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	যদি (page_at (a).intersects (other.page_at (b)))
	  প্রত্যাগম সত্য;
	a++;
	b++;
      }
      অন্যথা যদি (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      অন্যথা
	b++;
    }
    প্রত্যাগম অসত্য;
  }
  তার্কিক may_intersect (ধ্রুব hb_bit_set_t &other) ধ্রুব
  { প্রত্যাগম intersects (other); }

  তার্কিক intersects (hb_codepoint_t first, hb_codepoint_t last) ধ্রুব
  {
    hb_codepoint_t c = first - 1;
    প্রত্যাগম next (&c) && c <= last;
  }
  শূন্য set (ধ্রুব hb_bit_set_t &other, তার্কিক exact_size = অসত্য)
  {
    যদি (unlikely (!successful)) প্রত্যাগম;
    unsigned পূর্ণাঙ্ক গুনতি = other.pages.length;
    যদি (unlikely (!resize  (গুনতি, অসত্য, exact_size)))
      প্রত্যাগম;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  তার্কিক is_equal (ধ্রুব hb_bit_set_t &other) ধ্রুব
  {
    যদি (has_population () && other.has_population () &&
	population != other.population)
      প্রত্যাগম অসত্য;

    unsigned পূর্ণাঙ্ক na = pages.length;
    unsigned পূর্ণাঙ্ক nb = other.pages.length;

    unsigned পূর্ণাঙ্ক a = 0, b = 0;
    প্রতি (; a < na && b < nb; )
    {
      যদি (page_at (a).is_empty ()) { a++; অনুবর্ত; }
      যদি (other.page_at (b).is_empty ()) { b++; অনুবর্ত; }
      যদি (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	প্রত্যাগম অসত্য;
      a++;
      b++;
    }
    প্রতি (; a < na; a++)
      যদি (!page_at (a).is_empty ()) { প্রত্যাগম অসত্য; }
    প্রতি (; b < nb; b++)
      যদি (!other.page_at (b).is_empty ()) { প্রত্যাগম অসত্য; }

    প্রত্যাগম সত্য;
  }

  তার্কিক is_subset (ধ্রুব hb_bit_set_t &larger_set) ধ্রুব
  {
    যদি (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      প্রত্যাগম অসত্য;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    যাবত্ (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      যদি (spm < lpm) {
        যদি (!sp.is_empty ())
          প্রত্যাগম অসত্য;
        spi++;
        অনুবর্ত;
      }

      যদি (lpm < spm) {
        lpi++;
        অনুবর্ত;
      }

      auto lp = larger_set.page_at (lpi);
      যদি (!sp.is_subset (lp))
        প্রত্যাগম অসত্য;

      spi++;
      lpi++;
    }

    যাবত্ (spi < page_map.length)
      যদি (!page_at (spi++).is_empty ())
        প্রত্যাগম অসত্য;

    প্রত্যাগম সত্য;
  }

  গুপ্ত:
  তার্কিক allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    যদি (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = অসত্য;
      প্রত্যাগম অসত্য;
    }

    প্রত্যাগম সত্য;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  শূন্য compact (hb_vector_t<unsigned>& workspace,
                unsigned পূর্ণাঙ্ক length)
  {
    প্রতিজ্ঞা(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    প্রতি (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  শূন্য compact_pages (ধ্রুব hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned পূর্ণাঙ্ক write_index = 0;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < pages.length; i++)
    {
      যদি (old_index_to_page_map_index[i] == 0xFFFFFFFF) অনুবর্ত;

      যদি (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  সার্বজনিক:

  শূন্য process_ (hb_bit_page_t::vector_t (*op) (ধ্রুব hb_bit_page_t::vector_t &, ধ্রুব hb_bit_page_t::vector_t &),
		 তার্কিক passthru_বাম, তার্কিক passthru_ডান,
		 ধ্রুব hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    যদি (unlikely (!successful)) প্রত্যাগম;

    dirty ();

    unsigned পূর্ণাঙ্ক na = pages.length;
    unsigned পূর্ণাঙ্ক nb = other.pages.length;
    unsigned পূর্ণাঙ্ক next_page = na;

    unsigned পূর্ণাঙ্ক গুনতি = 0, new_গুনতি = 0;
    unsigned পূর্ণাঙ্ক a = 0, b = 0;
    unsigned পূর্ণাঙ্ক write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    যদি (!passthru_বাম)
    {
      তার্কিক allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      যদি (unlikely (!allocated))
      {
	successful = অসত্য;
	প্রত্যাগম;
      }
    }

    প্রতি (; a < na && b < nb; )
    {
      যদি (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	যদি (!passthru_বাম)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  যদি (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	গুনতি++;
	a++;
	b++;
      }
      অন্যথা যদি (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	যদি (passthru_বাম)
	  গুনতি++;
	a++;
      }
      অন্যথা
      {
	যদি (passthru_ডান)
	  গুনতি++;
	b++;
      }
    }
    যদি (passthru_বাম)
      গুনতি += na - a;
    যদি (passthru_ডান)
      গুনতি += nb - b;

    যদি (!passthru_বাম)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    যদি (unlikely (!resize (গুনতি)))
      প্রত্যাগম;

    new_গুনতি = গুনতি;

    /* Process in-place backward. */
    a = na;
    b = nb;
    প্রতি (; a && b; )
    {
      যদি (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	গুনতি--;
	page_map.arrayZ[গুনতি] = page_map.arrayZ[a];
	page_at (গুনতি).v = op (page_at (a).v, other.page_at (b).v);
	page_at (গুনতি).dirty ();
      }
      অন্যথা যদি (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	যদি (passthru_বাম)
	{
	  গুনতি--;
	  page_map.arrayZ[গুনতি] = page_map.arrayZ[a];
	}
      }
      অন্যথা
      {
	b--;
	যদি (passthru_ডান)
	{
	  গুনতি--;
	  page_map.arrayZ[গুনতি].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[গুনতি].index = next_page++;
	  page_at (গুনতি) = other.page_at (b);
	}
      }
    }
    যদি (passthru_বাম)
      যাবত্ (a)
      {
	a--;
	গুনতি--;
	page_map.arrayZ[গুনতি] = page_map.arrayZ[a];
      }
    যদি (passthru_ডান)
      যাবত্ (b)
      {
	b--;
	গুনতি--;
	page_map.arrayZ[গুনতি].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[গুনতি].index = next_page++;
	page_at (গুনতি) = other.page_at (b);
      }
    প্রতিজ্ঞা (!গুনতি);
    resize (new_গুনতি);
  }
  template <typename Op>
  স্থির hb_bit_page_t::vector_t
  op_ (ধ্রুব hb_bit_page_t::vector_t &a, ধ্রুব hb_bit_page_t::vector_t &b)
  { প্রত্যাগম Op{} (a, b); }
  template <typename Op>
  শূন্য process (ধ্রুব Op& op,
		ধ্রুব hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  শূন্য union_ (ধ্রুব hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  শূন্য intersect (ধ্রুব hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  শূন্য subtract (ধ্রুব hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  শূন্য symmetric_difference (ধ্রুব hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  তার্কিক next (hb_codepoint_t *codepoint) ধ্রুব
  {
    যদি (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      প্রত্যাগম *codepoint != INVALID;
    }

    ধ্রুব auto* page_map_array = page_map.arrayZ;
    unsigned পূর্ণাঙ্ক major = get_major (*codepoint);
    unsigned পূর্ণাঙ্ক i = last_page_lookup;

    যদি (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      যদি (i >= page_map.length) {
        *codepoint = INVALID;
        প্রত্যাগম অসত্য;
      }
      last_page_lookup = i;
    }

    ধ্রুব auto* pages_array = pages.arrayZ;
    ধ্রুব page_map_t &current = page_map_array[i];
    যদি (likely (current.major == major))
    {
      যদি (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        প্রত্যাগম সত্য;
      }
      i++;
    }

    প্রতি (; i < page_map.length; i++)
    {
      ধ্রুব page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      যদি (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	প্রত্যাগম সত্য;
      }
    }
    *codepoint = INVALID;
    প্রত্যাগম অসত্য;
  }
  তার্কিক next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ধ্রুব
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned পূর্ণাঙ্ক i = 0;
    unsigned পূর্ণাঙ্ক elt = 0;
    যদি (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      যদি (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        গচ্ছ done;
      base += page_t::ELT_BITS;

      unsigned পূর্ণাঙ্ক major = get_major (base);
      i = last_page_lookup;
      যদি (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	যদি (i >= page_map.length)
	  গচ্ছ done;
      }
      যদি (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    প্রতি (; i < page_map.length; i++, elt = 0)
    {
      ধ্রুব page_map_t &map = page_map.arrayZ[i];
      ধ্রুব page_t &page = pages.arrayZ[map.index];
      প্রতি (; elt < page_t::len (); elt++)
	যদি (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  প্রত্যাগম সত্য;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    প্রত্যাগম অসত্য;
  }
  তার্কিক previous (hb_codepoint_t *codepoint) ধ্রুব
  {
    যদি (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      প্রত্যাগম *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned পূর্ণাঙ্ক i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    যদি (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      যদি (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	প্রত্যাগম সত্য;
      }
    }
    i--;
    প্রতি (; (পূর্ণাঙ্ক) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      যদি (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	প্রত্যাগম সত্য;
      }
    }
    *codepoint = INVALID;
    প্রত্যাগম অসত্য;
  }
  তার্কিক next_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুব
  {
    hb_codepoint_t i;

    i = *last;
    যদি (!next (&i))
    {
      *last = *first = INVALID;
      প্রত্যাগম অসত্য;
    }

    *last = *first = i;

    ধ্রুব auto* page_map_array = page_map.arrayZ;
    ধ্রুব auto* pages_array = pages.arrayZ;
    unsigned পূর্ণাঙ্ক major = get_major (*last);
    unsigned পূর্ণাঙ্ক p_index = last_page_lookup;

    যদি (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      প্রতিজ্ঞা(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned পূর্ণাঙ্ক rem = page_remainder (*last);
    unsigned পূর্ণাঙ্ক elt = rem / page_t::ELT_BITS;
    unsigned পূর্ণাঙ্ক bit = rem & page_t::ELT_MASK;

    ধ্রুব page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned পূর্ণাঙ্ক run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    যদি (run < page_t::ELT_BITS - bit)
      প্রত্যাগম সত্য;

    elt++;

    যাবত্ (সত্য)
    {
      প্রতি (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        যদি (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          অনুবর্ত;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        প্রত্যাগম সত্য;
      }

      p_index++;


      যদি (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        প্রত্যাগম সত্য;

      ধ্রুব auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  তার্কিক previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ধ্রুব
  {
    hb_codepoint_t i;

    i = *first;
    যদি (!previous (&i))
    {
      *last = *first = INVALID;
      প্রত্যাগম অসত্য;
    }

    *last = *first = i;

    ধ্রুব auto* page_map_array = page_map.arrayZ;
    ধ্রুব auto* pages_array = pages.arrayZ;
    unsigned পূর্ণাঙ্ক major = get_major (*first);
    unsigned পূর্ণাঙ্ক p_index = last_page_lookup;

    যদি (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      প্রতিজ্ঞা(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned পূর্ণাঙ্ক rem = page_remainder (*first);
    পূর্ণাঙ্ক elt = rem / page_t::ELT_BITS;
    unsigned পূর্ণাঙ্ক bit = rem & page_t::ELT_MASK;

    ধ্রুব page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned পূর্ণাঙ্ক run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    যদি (run < bit + 1)
      প্রত্যাগম সত্য;

    elt--;

    যাবত্ (সত্য)
    {
      প্রতি (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        যদি (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          অনুবর্ত;
        }
        uint64_t inv = ~w;
        unsigned পূর্ণাঙ্ক zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        প্রত্যাগম সত্য;
      }


      যদি (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        প্রত্যাগম সত্য;

      p_index--;
      ধ্রুব auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned পূর্ণাঙ্ক next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned পূর্ণাঙ্ক    size) ধ্রুব
  {
    // By default, start at the first bit of the first page of values.
    unsigned পূর্ণাঙ্ক start_page = 0;
    unsigned পূর্ণাঙ্ক start_page_মান = 0;
    যদি (unlikely (codepoint != INVALID))
    {
      ধ্রুব auto* page_map_array = page_map.arrayZ;
      unsigned পূর্ণাঙ্ক major = get_major (codepoint);
      unsigned পূর্ণাঙ্ক i = last_page_lookup;
      যদি (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	যদি (i >= page_map.length)
	  প্রত্যাগম 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      যদি (page_map_array[i].major == major)
      {
        start_page_মান = page_remainder (codepoint + 1);
        যদি (unlikely (start_page_মান == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_মান = 0;
        }
      }
    }

    unsigned পূর্ণাঙ্ক initial_size = size;
    প্রতি (unsigned পূর্ণাঙ্ক i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned পূর্ণাঙ্ক n = pages[page_map.arrayZ[i].index].লিখ (base, start_page_মান, out, size);
      out += n;
      size -= n;
      start_page_মান = 0;
    }
    প্রত্যাগম initial_size - size;
  }

  unsigned পূর্ণাঙ্ক next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned পূর্ণাঙ্ক    size) ধ্রুব
  {
    unsigned পূর্ণাঙ্ক initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned পূর্ণাঙ্ক start_page = 0;
    unsigned পূর্ণাঙ্ক start_page_মান = 0;
    যদি (unlikely (codepoint != INVALID))
    {
      ধ্রুব auto* page_map_array = page_map.arrayZ;
      unsigned পূর্ণাঙ্ক major = get_major (codepoint);
      unsigned পূর্ণাঙ্ক i = last_page_lookup;
      যদি (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        যদি (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          যাবত্ (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          প্রত্যাগম initial_size - size;
        }
      }
      start_page = i;
      যদি (page_map_array[i].major == major)
      {
        start_page_মান = page_remainder (codepoint + 1);
        যদি (unlikely (start_page_মান == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_মান = 0;
        }
      }
    }

    hb_codepoint_t next_মান = codepoint + 1;
    প্রতি (unsigned পূর্ণাঙ্ক i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned পূর্ণাঙ্ক n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_মান, out, size, &next_মান);
      out += n;
      size -= n;
      start_page_মান = 0;
    }
    যাবত্ (next_মান < HB_SET_মান_INVALID && size) {
      *out++ = next_মান++;
      size--;
    }
    প্রত্যাগম initial_size - size;
  }

  তার্কিক has_population () ধ্রুব { প্রত্যাগম population != UINT_MAX; }
  unsigned পূর্ণাঙ্ক get_population () ধ্রুব
  {
    যদি (has_population ())
      প্রত্যাগম population;

    unsigned পূর্ণাঙ্ক বের_করো = 0;
    unsigned পূর্ণাঙ্ক গুনতি = pages.length;
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
      বের_করো += pages[i].get_population ();

    population = বের_করো;
    প্রত্যাগম বের_করো;
  }
  তার্কিক get_singleton (hb_codepoint_t *codepoint) ধ্রুব
  {
    যদি (has_population ())
    {
      যদি (population != 1) প্রত্যাগম অসত্য;
      *codepoint = get_min ();
      প্রত্যাগম সত্য;
    }

    hb_codepoint_t singleton = INVALID;
    প্রতি (ধ্রুব auto &map : page_map)
    {
      ধ্রুব auto &page = pages.arrayZ[map.index];
      প্রতি (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	যদি (!bits) অনুবর্ত;
	যদি (singleton != INVALID || (bits & (bits - 1)))
	  প্রত্যাগম অসত্য;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    যদি (singleton == INVALID)
    {
      population = 0;
      প্রত্যাগম অসত্য;
    }
    population = 1;
    *codepoint = singleton;
    প্রত্যাগম সত্য;
  }
  hb_codepoint_t get_min () ধ্রুব
  {
    unsigned গুনতি = pages.length;
    প্রতি (unsigned i = 0; i < গুনতি; i++)
    {
      ধ্রুব auto& map = page_map.arrayZ[i];
      ধ্রুব auto& page = pages.arrayZ[map.index];

      যদি (!page.is_empty ())
	প্রত্যাগম map.major * page_t::PAGE_BITS + page.get_min ();
    }
    প্রত্যাগম INVALID;
  }
  hb_codepoint_t get_max () ধ্রুব
  {
    unsigned গুনতি = pages.length;
    প্রতি (signed i = গুনতি - 1; i >= 0; i--)
    {
      ধ্রুব auto& map = page_map.arrayZ[(unsigned) i];
      ধ্রুব auto& page = pages.arrayZ[map.index];

      যদি (!page.is_empty ())
	প্রত্যাগম map.major * page_t::PAGE_BITS + page.get_max ();
    }
    প্রত্যাগম INVALID;
  }

  স্থির constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  সংরচনা iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    স্থির constexpr তার্কিক is_sorted_iterator = সত্য;
    স্থির constexpr তার্কিক has_fast_len = সত্য;
    iter_t (ধ্রুব hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    তার্কিক init = সত্য) : s (&s_), v (INVALID), l(0)
    {
      যদি (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    প্রকারনির্দেশ hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ধ্রুব { প্রত্যাগম v; }
    তার্কিক __more__ () ধ্রুব { প্রত্যাগম v != INVALID; }
    শূন্য __next__ () { s->next (&v); যদি (l) l--; }
    শূন্য __prev__ () { s->previous (&v); }
    unsigned __len__ () ধ্রুব { প্রত্যাগম l; }
    iter_t end () ধ্রুব { প্রত্যাগম iter_t (*s, অসত্য); }
    তার্কিক operator != (ধ্রুব iter_t& o) ধ্রুব
    { প্রত্যাগম v != o.v; }

    রক্ষিত:
    ধ্রুব hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () ধ্রুব { প্রত্যাগম iter_t (*এতত্); }
  operator iter_t () ধ্রুব { প্রত্যাগম iter (); }

  রক্ষিত:

  page_t *page_for (hb_codepoint_t g, তার্কিক insert = অসত্য)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    যদি (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      যদি (cached_page.major == major)
	প্রত্যাগম &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    যদি (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      যদি (!insert)
        প্রত্যাগম nullptr;

      যদি (unlikely (!resize (pages.length + 1)))
	প্রত্যাগম nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    প্রত্যাগম &pages.arrayZ[page_map.arrayZ[i].index];
  }
  ধ্রুব page_t *page_for (hb_codepoint_t g) ধ্রুব
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    যদি (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      যদি (cached_page.major == major)
	প্রত্যাগম &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    যদি (!page_map.bfind (key, &i))
      প্রত্যাগম nullptr;

    last_page_lookup = i;
    প্রত্যাগম &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned পূর্ণাঙ্ক i)
  {
    প্রতিজ্ঞা (i < page_map.length);
    প্রত্যাগম pages.arrayZ[page_map.arrayZ[i].index];
  }
  ধ্রুব page_t &page_at (unsigned পূর্ণাঙ্ক i) ধ্রুব
  {
    প্রতিজ্ঞা (i < page_map.length);
    প্রত্যাগম pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned পূর্ণাঙ্ক get_major (hb_codepoint_t g) ধ্রুব { প্রত্যাগম g >> page_t::PAGE_BITS_LOG_2; }
  unsigned পূর্ণাঙ্ক page_remainder (hb_codepoint_t g) ধ্রুব { প্রত্যাগম g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned পূর্ণাঙ্ক major) ধ্রুব { প্রত্যাগম major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
