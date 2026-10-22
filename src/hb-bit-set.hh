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

#include "hb.hh"
#include "hb-bit-page.hh"

sa_nracanaa hb_bit_set_t
{
  hb_bit_set_t () = default;
  ~hb_bit_set_t () = default;

  hb_bit_set_t (const hb_bit_set_t& other) : hb_bit_set_t () { set (other, true); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*this, other); }
  hb_bit_set_t& operator= (const hb_bit_set_t& other) { set (other); ᤀᤡᤴ *this; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*this, other); ᤀᤡᤴ *this; }
  friend void swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    ᤀᤡ᤺ (likely (!a.successful || !b.successful))
      ᤀᤡᤴ;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  void init ()
  {
    successful = true;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  void fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  sa_nracanaa page_map_t
  {
    puur_naa_nka cmp (const page_map_t &o) const { ᤀᤡᤴ cmp (o.major); }
    puur_naa_nka cmp (uint32_t o_major) const { ᤀᤡᤴ (puur_naa_nka) o_major - (puur_naa_nka) major; }

    uint32_t major;
    uint32_t index;
  };

  bool successful = true; /* Allocations successful */
  mutable unsigned puur_naa_nka population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  void err () { ᤀᤡ᤺ (successful) successful = false; } /* TODO Remove */
  bool in_error () const { ᤀᤡᤴ !successful; }

  bool resize (unsigned puur_naa_nka count, bool clear = true, bool exact_size = false)
  {
    ᤀᤡ᤺ (unlikely (!successful)) ᤀᤡᤴ false;

    ᤀᤡ᤺ (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = true; // Most sets are small and local

    ᤀᤡ᤺ (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, false)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = false;
      ᤀᤡᤴ false;
    }
    ᤀᤡᤴ true;
  }

  void alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = true;
    clear ();
    ᤀᤡᤴ *this;
  }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ᤀᤡ᤺ (!pages.length && !population) ᤀᤡᤴ;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  bool is_empty () const
  {
    unsigned puur_naa_nka count = pages.length;
    for (unsigned puur_naa_nka i = 0; i < count; i++)
      ᤀᤡ᤺ (!pages[i].is_empty ())
	ᤀᤡᤴ false;
    ᤀᤡᤴ true;
  }
  explicit operator bool () const { ᤀᤡᤴ !is_empty (); }

  uint32_t hash () const
  {
    uint32_t h = 0;
    for (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      ᤀᤡ᤺ (unlikely (page.is_empty ())) continue;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    ᤀᤡᤴ h;
  }

  private:
  void dirty () { population = UINT_MAX; }
  public:

  void add (hb_codepoint_t g)
  {
    ᤀᤡ᤺ (unlikely (!successful)) ᤀᤡᤴ;
    ᤀᤡ᤺ (unlikely (g == INVALID)) ᤀᤡᤴ;
    dirty ();
    page_t *page = page_for (g, true); ᤀᤡ᤺ (unlikely (!page)) ᤀᤡᤴ;
    page->add (g);
  }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  {
    ᤀᤡ᤺ (unlikely (!successful) || unlikely (!bits)) ᤀᤡᤴ;
    assert (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); ᤀᤡ᤺ (unlikely (!page)) ᤀᤡᤴ;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ᤀᤡ᤺ (unlikely (!successful)) ᤀᤡᤴ true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ᤀᤡ᤺ (unlikely (a > b || a == INVALID || b == INVALID)) ᤀᤡᤴ false;
    dirty ();
    unsigned puur_naa_nka ma = get_major (a);
    unsigned puur_naa_nka mb = get_major (b);
    ᤀᤡ᤺ (ma == mb)
    {
      page_t *page = page_for (a, true); ᤀᤡ᤺ (unlikely (!page)) ᤀᤡᤴ false;
      page->add_range (a, b);
    }
    ᤀᤡ᤺ᤆᤡ
    {
      page_t *page = page_for (a, true); ᤀᤡ᤺ (unlikely (!page)) ᤀᤡᤴ false;
      page->add_range (a, major_start (ma + 1) - 1);

      for (unsigned puur_naa_nka m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); ᤀᤡ᤺ (unlikely (!page)) ᤀᤡᤴ false;
	page->init1 ();
      }

      page = page_for (b, true); ᤀᤡ᤺ (unlikely (!page)) ᤀᤡᤴ false;
      page->add_range (major_start (mb), b);
    }
    ᤀᤡᤴ true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  static anta_hstha const Type& StructAtOffsetUnaligned(const void *P, unsigned puur_naa_nka offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    ᤀᤡᤴ * reinterpret_cast<const Type*> ((const var_na *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  void set_array (bool v, const T *array, unsigned puur_naa_nka count, unsigned puur_naa_nka stride=parimaa_na(T))
  {
    ᤀᤡ᤺ (unlikely (!successful)) ᤀᤡᤴ;
    ᤀᤡ᤺ (!count) ᤀᤡᤴ;
    dirty ();
    hb_codepoint_t g = *array;
    ᤀᤠ᤺ (count)
    {
      unsigned puur_naa_nka m = get_major (g);
      page_t *page = page_for (g, v); ᤀᤡ᤺ (unlikely (v && !page)) ᤀᤡᤴ;
      unsigned puur_naa_nka start = major_start (m);
      unsigned puur_naa_nka end = major_start (m + 1);
      do
      {
        ᤀᤡ᤺ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ᤀᤠ᤺ (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  void add_array (const T *array, unsigned puur_naa_nka count, unsigned puur_naa_nka stride=parimaa_na(T))
  { set_array (true, array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  void del_array (const T *array, unsigned puur_naa_nka count, unsigned puur_naa_nka stride=parimaa_na(T))
  { set_array (false, array, count, stride); }
  template <typename T>
  void del_array (const hb_array_t<const T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool set_sorted_array (bool v, const T *array, unsigned puur_naa_nka count, unsigned puur_naa_nka stride=parimaa_na(T))
  {
    ᤀᤡ᤺ (unlikely (!successful)) ᤀᤡᤴ true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ᤀᤡ᤺ (unlikely (!count)) ᤀᤡᤴ true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ᤀᤠ᤺ (count)
    {
      unsigned puur_naa_nka m = get_major (g);
      page_t *page = page_for (g, v); ᤀᤡ᤺ (unlikely (v && !page)) ᤀᤡᤴ false;
      unsigned puur_naa_nka end = major_start (m + 1);
      do
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ᤀᤡ᤺ (g < last_g) ᤀᤡᤴ false;
	last_g = g;

        ᤀᤡ᤺ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ᤀᤠ᤺ (count && (g = *array, g < end));
    }
    ᤀᤡᤴ true;
  }

  template <typename T>
  bool add_sorted_array (const T *array, unsigned puur_naa_nka count, unsigned puur_naa_nka stride=parimaa_na(T))
  { ᤀᤡᤴ set_sorted_array (true, array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { ᤀᤡᤴ add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  bool del_sorted_array (const T *array, unsigned puur_naa_nka count, unsigned puur_naa_nka stride=parimaa_na(T))
  { ᤀᤡᤴ set_sorted_array (false, array, count, stride); }
  template <typename T>
  bool del_sorted_array (const hb_sorted_array_t<const T>& arr) { ᤀᤡᤴ del_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g)
  {
    ᤀᤡ᤺ (unlikely (!successful)) ᤀᤡᤴ;
    page_t *page = page_for (g);
    ᤀᤡ᤺ (!page)
      ᤀᤡᤴ;
    dirty ();
    page->del (g);
  }
  void del_bits (hb_codepoint_t g, uint64_t bits)
  {
    ᤀᤡ᤺ (unlikely (!successful) || unlikely (!bits)) ᤀᤡᤴ;
    assert (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    ᤀᤡ᤺ (!page)
      ᤀᤡᤴ;
    dirty ();
    page->del_bits (g, bits);
  }

  private:
  void del_pages (puur_naa_nka ds, puur_naa_nka de)
  {
    ᤀᤡ᤺ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      ᤀᤡ᤺ (unlikely (!allocate_compact_workspace (compact_workspace))) ᤀᤡᤴ;

      unsigned puur_naa_nka write_index = 0;
      for (unsigned puur_naa_nka i = 0; i < page_map.length; i++)
      {
	puur_naa_nka m = (puur_naa_nka) page_map.arrayZ[i].major;
	ᤀᤡ᤺ (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  public:
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ᤀᤡ᤺ (unlikely (!successful)) ᤀᤡᤴ;
    ᤀᤡ᤺ (unlikely (a > b || a == INVALID)) ᤀᤡᤴ;
    dirty ();
    unsigned puur_naa_nka ma = get_major (a);
    unsigned puur_naa_nka mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    puur_naa_nka ds = (a == major_start (ma))? (puur_naa_nka) ma: (puur_naa_nka) (ma + 1);
    puur_naa_nka de = (b + 1 == major_start (mb + 1))? (puur_naa_nka) mb: ((puur_naa_nka) mb - 1);
    ᤀᤡ᤺ (ds > de || (puur_naa_nka) ma < ds)
    {
      page_t *page = page_for (a);
      ᤀᤡ᤺ (page)
      {
	ᤀᤡ᤺ (ma == mb)
	  page->del_range (a, b);
	ᤀᤡ᤺ᤆᤡ
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    ᤀᤡ᤺ (de < (puur_naa_nka) mb && ma != mb)
    {
      page_t *page = page_for (b);
      ᤀᤡ᤺ (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool get (hb_codepoint_t g) const
  {
    const page_t *page = page_for (g);
    ᤀᤡ᤺ (!page)
      ᤀᤡᤴ false;
    ᤀᤡᤴ page->get (g);
  }
  bool may_have (hb_codepoint_t g) const { ᤀᤡᤴ get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { ᤀᤡᤴ get (k); }
  bool has (hb_codepoint_t k) const { ᤀᤡᤴ (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { ᤀᤡᤴ has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); ᤀᤡᤴ *this; }
  hb_bit_set_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ᤀᤡᤴ *this; }

  bool intersects (const hb_bit_set_t &other) const
  {
    unsigned puur_naa_nka na = pages.length;
    unsigned puur_naa_nka nb = other.pages.length;

    unsigned puur_naa_nka a = 0, b = 0;
    for (; a < na && b < nb; )
    {
      ᤀᤡ᤺ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ᤀᤡ᤺ (page_at (a).intersects (other.page_at (b)))
	  ᤀᤡᤴ true;
	a++;
	b++;
      }
      ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      ᤀᤡ᤺ᤆᤡ
	b++;
    }
    ᤀᤡᤴ false;
  }
  bool may_intersect (const hb_bit_set_t &other) const
  { ᤀᤡᤴ intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    ᤀᤡᤴ next (&c) && c <= last;
  }
  void set (const hb_bit_set_t &other, bool exact_size = false)
  {
    ᤀᤡ᤺ (unlikely (!successful)) ᤀᤡᤴ;
    unsigned puur_naa_nka count = other.pages.length;
    ᤀᤡ᤺ (unlikely (!resize  (count, false, exact_size)))
      ᤀᤡᤴ;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (const hb_bit_set_t &other) const
  {
    ᤀᤡ᤺ (has_population () && other.has_population () &&
	population != other.population)
      ᤀᤡᤴ false;

    unsigned puur_naa_nka na = pages.length;
    unsigned puur_naa_nka nb = other.pages.length;

    unsigned puur_naa_nka a = 0, b = 0;
    for (; a < na && b < nb; )
    {
      ᤀᤡ᤺ (page_at (a).is_empty ()) { a++; continue; }
      ᤀᤡ᤺ (other.page_at (b).is_empty ()) { b++; continue; }
      ᤀᤡ᤺ (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	ᤀᤡᤴ false;
      a++;
      b++;
    }
    for (; a < na; a++)
      ᤀᤡ᤺ (!page_at (a).is_empty ()) { ᤀᤡᤴ false; }
    for (; b < nb; b++)
      ᤀᤡ᤺ (!other.page_at (b).is_empty ()) { ᤀᤡᤴ false; }

    ᤀᤡᤴ true;
  }

  bool is_subset (const hb_bit_set_t &larger_set) const
  {
    ᤀᤡ᤺ (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      ᤀᤡᤴ false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ᤀᤠ᤺ (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      ᤀᤡ᤺ (spm < lpm) {
        ᤀᤡ᤺ (!sp.is_empty ())
          ᤀᤡᤴ false;
        spi++;
        continue;
      }

      ᤀᤡ᤺ (lpm < spm) {
        lpi++;
        continue;
      }

      auto lp = larger_set.page_at (lpi);
      ᤀᤡ᤺ (!sp.is_subset (lp))
        ᤀᤡᤴ false;

      spi++;
      lpi++;
    }

    ᤀᤠ᤺ (spi < page_map.length)
      ᤀᤡ᤺ (!page_at (spi++).is_empty ())
        ᤀᤡᤴ false;

    ᤀᤡᤴ true;
  }

  private:
  bool allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    ᤀᤡ᤺ (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = false;
      ᤀᤡᤴ false;
    }

    ᤀᤡᤴ true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  void compact (hb_vector_t<unsigned>& workspace,
                unsigned puur_naa_nka length)
  {
    assert(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    for (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  void compact_pages (const hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned puur_naa_nka write_index = 0;
    for (unsigned puur_naa_nka i = 0; i < pages.length; i++)
    {
      ᤀᤡ᤺ (old_index_to_page_map_index[i] == 0xFFFFFFFF) continue;

      ᤀᤡ᤺ (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  public:

  void process_ (hb_bit_page_t::vector_t (*op) (const hb_bit_page_t::vector_t &, const hb_bit_page_t::vector_t &),
		 bool passthru_left, bool passthru_right,
		 const hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    ᤀᤡ᤺ (unlikely (!successful)) ᤀᤡᤴ;

    dirty ();

    unsigned puur_naa_nka na = pages.length;
    unsigned puur_naa_nka nb = other.pages.length;
    unsigned puur_naa_nka next_page = na;

    unsigned puur_naa_nka count = 0, newCount = 0;
    unsigned puur_naa_nka a = 0, b = 0;
    unsigned puur_naa_nka write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    ᤀᤡ᤺ (!passthru_left)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      ᤀᤡ᤺ (unlikely (!allocated))
      {
	successful = false;
	ᤀᤡᤴ;
      }
    }

    for (; a < na && b < nb; )
    {
      ᤀᤡ᤺ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ᤀᤡ᤺ (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ᤀᤡ᤺ (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	ᤀᤡ᤺ (passthru_left)
	  count++;
	a++;
      }
      ᤀᤡ᤺ᤆᤡ
      {
	ᤀᤡ᤺ (passthru_right)
	  count++;
	b++;
      }
    }
    ᤀᤡ᤺ (passthru_left)
      count += na - a;
    ᤀᤡ᤺ (passthru_right)
      count += nb - b;

    ᤀᤡ᤺ (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    ᤀᤡ᤺ (unlikely (!resize (count)))
      ᤀᤡᤴ;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    for (; a && b; )
    {
      ᤀᤡ᤺ (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	ᤀᤡ᤺ (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      ᤀᤡ᤺ᤆᤡ
      {
	b--;
	ᤀᤡ᤺ (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    ᤀᤡ᤺ (passthru_left)
      ᤀᤠ᤺ (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    ᤀᤡ᤺ (passthru_right)
      ᤀᤠ᤺ (b)
      {
	b--;
	count--;
	page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[count].index = next_page++;
	page_at (count) = other.page_at (b);
      }
    assert (!count);
    resize (newCount);
  }
  template <typename Op>
  static hb_bit_page_t::vector_t
  op_ (const hb_bit_page_t::vector_t &a, const hb_bit_page_t::vector_t &b)
  { ᤀᤡᤴ Op{} (a, b); }
  template <typename Op>
  void process (const Op& op,
		const hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  void union_ (const hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  void intersect (const hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  void subtract (const hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  void symmetric_difference (const hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  bool next (hb_codepoint_t *codepoint) const
  {
    ᤀᤡ᤺ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ᤀᤡᤴ *codepoint != INVALID;
    }

    const auto* page_map_array = page_map.arrayZ;
    unsigned puur_naa_nka major = get_major (*codepoint);
    unsigned puur_naa_nka i = last_page_lookup;

    ᤀᤡ᤺ (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      ᤀᤡ᤺ (i >= page_map.length) {
        *codepoint = INVALID;
        ᤀᤡᤴ false;
      }
      last_page_lookup = i;
    }

    const auto* pages_array = pages.arrayZ;
    const page_map_t &current = page_map_array[i];
    ᤀᤡ᤺ (likely (current.major == major))
    {
      ᤀᤡ᤺ (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        ᤀᤡᤴ true;
      }
      i++;
    }

    for (; i < page_map.length; i++)
    {
      const page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      ᤀᤡ᤺ (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	ᤀᤡᤴ true;
      }
    }
    *codepoint = INVALID;
    ᤀᤡᤴ false;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned puur_naa_nka i = 0;
    unsigned puur_naa_nka elt = 0;
    ᤀᤡ᤺ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      ᤀᤡ᤺ (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        goto done;
      base += page_t::ELT_BITS;

      unsigned puur_naa_nka major = get_major (base);
      i = last_page_lookup;
      ᤀᤡ᤺ (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ᤀᤡ᤺ (i >= page_map.length)
	  goto done;
      }
      ᤀᤡ᤺ (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    for (; i < page_map.length; i++, elt = 0)
    {
      const page_map_t &map = page_map.arrayZ[i];
      const page_t &page = pages.arrayZ[map.index];
      for (; elt < page_t::len (); elt++)
	ᤀᤡ᤺ (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  ᤀᤡᤴ true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ᤀᤡᤴ false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    ᤀᤡ᤺ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ᤀᤡᤴ *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned puur_naa_nka i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    ᤀᤡ᤺ (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      ᤀᤡ᤺ (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	ᤀᤡᤴ true;
      }
    }
    i--;
    for (; (puur_naa_nka) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      ᤀᤡ᤺ (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	ᤀᤡᤴ true;
      }
    }
    *codepoint = INVALID;
    ᤀᤡᤴ false;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *last;
    ᤀᤡ᤺ (!next (&i))
    {
      *last = *first = INVALID;
      ᤀᤡᤴ false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned puur_naa_nka major = get_major (*last);
    unsigned puur_naa_nka p_index = last_page_lookup;

    ᤀᤡ᤺ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned puur_naa_nka rem = page_remainder (*last);
    unsigned puur_naa_nka elt = rem / page_t::ELT_BITS;
    unsigned puur_naa_nka bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned puur_naa_nka run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    ᤀᤡ᤺ (run < page_t::ELT_BITS - bit)
      ᤀᤡᤴ true;

    elt++;

    ᤀᤠ᤺ (true)
    {
      for (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        ᤀᤡ᤺ (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        ᤀᤡᤴ true;
      }

      p_index++;


      ᤀᤡ᤺ (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        ᤀᤡᤴ true;

      const auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *first;
    ᤀᤡ᤺ (!previous (&i))
    {
      *last = *first = INVALID;
      ᤀᤡᤴ false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned puur_naa_nka major = get_major (*first);
    unsigned puur_naa_nka p_index = last_page_lookup;

    ᤀᤡ᤺ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned puur_naa_nka rem = page_remainder (*first);
    puur_naa_nka elt = rem / page_t::ELT_BITS;
    unsigned puur_naa_nka bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned puur_naa_nka run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    ᤀᤡ᤺ (run < bit + 1)
      ᤀᤡᤴ true;

    elt--;

    ᤀᤠ᤺ (true)
    {
      for (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        ᤀᤡ᤺ (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        unsigned puur_naa_nka zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        ᤀᤡᤴ true;
      }


      ᤀᤡ᤺ (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        ᤀᤡᤴ true;

      p_index--;
      const auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned puur_naa_nka next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned puur_naa_nka    size) const
  {
    // By default, start at the first bit of the first page of values.
    unsigned puur_naa_nka start_page = 0;
    unsigned puur_naa_nka start_page_value = 0;
    ᤀᤡ᤺ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned puur_naa_nka major = get_major (codepoint);
      unsigned puur_naa_nka i = last_page_lookup;
      ᤀᤡ᤺ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ᤀᤡ᤺ (i >= page_map.length)
	  ᤀᤡᤴ 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      ᤀᤡ᤺ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ᤀᤡ᤺ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned puur_naa_nka initial_size = size;
    for (unsigned puur_naa_nka i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned puur_naa_nka n = pages[page_map.arrayZ[i].index].write (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ᤀᤡᤴ initial_size - size;
  }

  unsigned puur_naa_nka next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned puur_naa_nka    size) const
  {
    unsigned puur_naa_nka initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned puur_naa_nka start_page = 0;
    unsigned puur_naa_nka start_page_value = 0;
    ᤀᤡ᤺ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned puur_naa_nka major = get_major (codepoint);
      unsigned puur_naa_nka i = last_page_lookup;
      ᤀᤡ᤺ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        ᤀᤡ᤺ (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ᤀᤠ᤺ (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          ᤀᤡᤴ initial_size - size;
        }
      }
      start_page = i;
      ᤀᤡ᤺ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ᤀᤡ᤺ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    for (unsigned puur_naa_nka i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned puur_naa_nka n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ᤀᤠ᤺ (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    ᤀᤡᤴ initial_size - size;
  }

  bool has_population () const { ᤀᤡᤴ population != UINT_MAX; }
  unsigned puur_naa_nka get_population () const
  {
    ᤀᤡ᤺ (has_population ())
      ᤀᤡᤴ population;

    unsigned puur_naa_nka pop = 0;
    unsigned puur_naa_nka count = pages.length;
    for (unsigned puur_naa_nka i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    ᤀᤡᤴ pop;
  }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    ᤀᤡ᤺ (has_population ())
    {
      ᤀᤡ᤺ (population != 1) ᤀᤡᤴ false;
      *codepoint = get_min ();
      ᤀᤡᤴ true;
    }

    hb_codepoint_t singleton = INVALID;
    for (const auto &map : page_map)
    {
      const auto &page = pages.arrayZ[map.index];
      for (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	ᤀᤡ᤺ (!bits) continue;
	ᤀᤡ᤺ (singleton != INVALID || (bits & (bits - 1)))
	  ᤀᤡᤴ false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    ᤀᤡ᤺ (singleton == INVALID)
    {
      population = 0;
      ᤀᤡᤴ false;
    }
    population = 1;
    *codepoint = singleton;
    ᤀᤡᤴ true;
  }
  hb_codepoint_t get_min () const
  {
    unsigned count = pages.length;
    for (unsigned i = 0; i < count; i++)
    {
      const auto& map = page_map.arrayZ[i];
      const auto& page = pages.arrayZ[map.index];

      ᤀᤡ᤺ (!page.is_empty ())
	ᤀᤡᤴ map.major * page_t::PAGE_BITS + page.get_min ();
    }
    ᤀᤡᤴ INVALID;
  }
  hb_codepoint_t get_max () const
  {
    unsigned count = pages.length;
    for (signed i = count - 1; i >= 0; i--)
    {
      const auto& map = page_map.arrayZ[(unsigned) i];
      const auto& page = pages.arrayZ[map.index];

      ᤀᤡ᤺ (!page.is_empty ())
	ᤀᤡᤴ map.major * page_t::PAGE_BITS + page.get_max ();
    }
    ᤀᤡᤴ INVALID;
  }

  static constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  sa_nracanaa iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    static constexpr bool has_fast_len = true;
    iter_t (const hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    bool init = true) : s (&s_), v (INVALID), l(0)
    {
      ᤀᤡ᤺ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { ᤀᤡᤴ v; }
    bool __more__ () const { ᤀᤡᤴ v != INVALID; }
    void __next__ () { s->next (&v); ᤀᤡ᤺ (l) l--; }
    void __prev__ () { s->previous (&v); }
    unsigned __len__ () const { ᤀᤡᤴ l; }
    iter_t end () const { ᤀᤡᤴ iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { ᤀᤡᤴ v != o.v; }

    rak_sita:
    const hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () const { ᤀᤡᤴ iter_t (*this); }
  operator iter_t () const { ᤀᤡᤴ iter (); }

  rak_sita:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ᤀᤡ᤺ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ᤀᤡ᤺ (cached_page.major == major)
	ᤀᤡᤴ &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    ᤀᤡ᤺ (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      ᤀᤡ᤺ (!insert)
        ᤀᤡᤴ nullptr;

      ᤀᤡ᤺ (unlikely (!resize (pages.length + 1)))
	ᤀᤡᤴ nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    ᤀᤡᤴ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t *page_for (hb_codepoint_t g) const
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ᤀᤡ᤺ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ᤀᤡ᤺ (cached_page.major == major)
	ᤀᤡᤴ &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    ᤀᤡ᤺ (!page_map.bfind (key, &i))
      ᤀᤡᤴ nullptr;

    last_page_lookup = i;
    ᤀᤡᤴ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned puur_naa_nka i)
  {
    assert (i < page_map.length);
    ᤀᤡᤴ pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t &page_at (unsigned puur_naa_nka i) const
  {
    assert (i < page_map.length);
    ᤀᤡᤴ pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned puur_naa_nka get_major (hb_codepoint_t g) const { ᤀᤡᤴ g >> page_t::PAGE_BITS_LOG_2; }
  unsigned puur_naa_nka page_remainder (hb_codepoint_t g) const { ᤀᤡᤴ g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned puur_naa_nka major) const { ᤀᤡᤴ major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
