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
  hb_bit_set_t& operator= (const hb_bit_set_t& other) { set (other); ᨀᨘᨒ *this; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*this, other); ᨀᨘᨒ *this; }
  friend void swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    ᨀᨘ (likely (!a.successful || !b.successful))
      ᨀᨘᨒ;
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
    puur_naa_nka cmp (const page_map_t &o) const { ᨀᨘᨒ cmp (o.major); }
    puur_naa_nka cmp (uint32_t o_major) const { ᨀᨘᨒ (puur_naa_nka) o_major - (puur_naa_nka) major; }

    uint32_t major;
    uint32_t index;
  };

  bool successful = true; /* Allocations successful */
  mutable unsigned puur_naa_nka population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  void err () { ᨀᨘ (successful) successful = false; } /* TODO Remove */
  bool in_error () const { ᨀᨘᨒ !successful; }

  bool resize (unsigned puur_naa_nka count, bool clear = true, bool exact_size = false)
  {
    ᨀᨘ (unlikely (!successful)) ᨀᨘᨒ false;

    ᨀᨘ (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = true; // Most sets are small and local

    ᨀᨘ (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, false)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = false;
      ᨀᨘᨒ false;
    }
    ᨀᨘᨒ true;
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
    ᨀᨘᨒ *this;
  }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ᨀᨘ (!pages.length && !population) ᨀᨘᨒ;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  bool is_empty () const
  {
    unsigned puur_naa_nka count = pages.length;
    for (unsigned puur_naa_nka i = 0; i < count; i++)
      ᨀᨘ (!pages[i].is_empty ())
	ᨀᨘᨒ false;
    ᨀᨘᨒ true;
  }
  explicit operator bool () const { ᨀᨘᨒ !is_empty (); }

  uint32_t hash () const
  {
    uint32_t h = 0;
    for (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      ᨀᨘ (unlikely (page.is_empty ())) continue;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    ᨀᨘᨒ h;
  }

  private:
  void dirty () { population = UINT_MAX; }
  public:

  void add (hb_codepoint_t g)
  {
    ᨀᨘ (unlikely (!successful)) ᨀᨘᨒ;
    ᨀᨘ (unlikely (g == INVALID)) ᨀᨘᨒ;
    dirty ();
    page_t *page = page_for (g, true); ᨀᨘ (unlikely (!page)) ᨀᨘᨒ;
    page->add (g);
  }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  {
    ᨀᨘ (unlikely (!successful) || unlikely (!bits)) ᨀᨘᨒ;
    assert (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); ᨀᨘ (unlikely (!page)) ᨀᨘᨒ;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ᨀᨘ (unlikely (!successful)) ᨀᨘᨒ true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ᨀᨘ (unlikely (a > b || a == INVALID || b == INVALID)) ᨀᨘᨒ false;
    dirty ();
    unsigned puur_naa_nka ma = get_major (a);
    unsigned puur_naa_nka mb = get_major (b);
    ᨀᨘ (ma == mb)
    {
      page_t *page = page_for (a, true); ᨀᨘ (unlikely (!page)) ᨀᨘᨒ false;
      page->add_range (a, b);
    }
    ᨀᨘᨁ
    {
      page_t *page = page_for (a, true); ᨀᨘ (unlikely (!page)) ᨀᨘᨒ false;
      page->add_range (a, major_start (ma + 1) - 1);

      for (unsigned puur_naa_nka m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); ᨀᨘ (unlikely (!page)) ᨀᨘᨒ false;
	page->init1 ();
      }

      page = page_for (b, true); ᨀᨘ (unlikely (!page)) ᨀᨘᨒ false;
      page->add_range (major_start (mb), b);
    }
    ᨀᨘᨒ true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  static anta_hstha const Type& StructAtOffsetUnaligned(const void *P, unsigned puur_naa_nka offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    ᨀᨘᨒ * reinterpret_cast<const Type*> ((const var_na *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  void set_array (bool v, const T *array, unsigned puur_naa_nka count, unsigned puur_naa_nka stride=parimaa_na(T))
  {
    ᨀᨘ (unlikely (!successful)) ᨀᨘᨒ;
    ᨀᨘ (!count) ᨀᨘᨒ;
    dirty ();
    hb_codepoint_t g = *array;
    ᨀᨛ (count)
    {
      unsigned puur_naa_nka m = get_major (g);
      page_t *page = page_for (g, v); ᨀᨘ (unlikely (v && !page)) ᨀᨘᨒ;
      unsigned puur_naa_nka start = major_start (m);
      unsigned puur_naa_nka end = major_start (m + 1);
      do
      {
        ᨀᨘ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ᨀᨛ (count && (g = *array, start <= g && g < end));
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
    ᨀᨘ (unlikely (!successful)) ᨀᨘᨒ true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ᨀᨘ (unlikely (!count)) ᨀᨘᨒ true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ᨀᨛ (count)
    {
      unsigned puur_naa_nka m = get_major (g);
      page_t *page = page_for (g, v); ᨀᨘ (unlikely (v && !page)) ᨀᨘᨒ false;
      unsigned puur_naa_nka end = major_start (m + 1);
      do
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ᨀᨘ (g < last_g) ᨀᨘᨒ false;
	last_g = g;

        ᨀᨘ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ᨀᨛ (count && (g = *array, g < end));
    }
    ᨀᨘᨒ true;
  }

  template <typename T>
  bool add_sorted_array (const T *array, unsigned puur_naa_nka count, unsigned puur_naa_nka stride=parimaa_na(T))
  { ᨀᨘᨒ set_sorted_array (true, array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { ᨀᨘᨒ add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  bool del_sorted_array (const T *array, unsigned puur_naa_nka count, unsigned puur_naa_nka stride=parimaa_na(T))
  { ᨀᨘᨒ set_sorted_array (false, array, count, stride); }
  template <typename T>
  bool del_sorted_array (const hb_sorted_array_t<const T>& arr) { ᨀᨘᨒ del_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g)
  {
    ᨀᨘ (unlikely (!successful)) ᨀᨘᨒ;
    page_t *page = page_for (g);
    ᨀᨘ (!page)
      ᨀᨘᨒ;
    dirty ();
    page->del (g);
  }
  void del_bits (hb_codepoint_t g, uint64_t bits)
  {
    ᨀᨘ (unlikely (!successful) || unlikely (!bits)) ᨀᨘᨒ;
    assert (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    ᨀᨘ (!page)
      ᨀᨘᨒ;
    dirty ();
    page->del_bits (g, bits);
  }

  private:
  void del_pages (puur_naa_nka ds, puur_naa_nka de)
  {
    ᨀᨘ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      ᨀᨘ (unlikely (!allocate_compact_workspace (compact_workspace))) ᨀᨘᨒ;

      unsigned puur_naa_nka write_index = 0;
      for (unsigned puur_naa_nka i = 0; i < page_map.length; i++)
      {
	puur_naa_nka m = (puur_naa_nka) page_map.arrayZ[i].major;
	ᨀᨘ (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  public:
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ᨀᨘ (unlikely (!successful)) ᨀᨘᨒ;
    ᨀᨘ (unlikely (a > b || a == INVALID)) ᨀᨘᨒ;
    dirty ();
    unsigned puur_naa_nka ma = get_major (a);
    unsigned puur_naa_nka mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    puur_naa_nka ds = (a == major_start (ma))? (puur_naa_nka) ma: (puur_naa_nka) (ma + 1);
    puur_naa_nka de = (b + 1 == major_start (mb + 1))? (puur_naa_nka) mb: ((puur_naa_nka) mb - 1);
    ᨀᨘ (ds > de || (puur_naa_nka) ma < ds)
    {
      page_t *page = page_for (a);
      ᨀᨘ (page)
      {
	ᨀᨘ (ma == mb)
	  page->del_range (a, b);
	ᨀᨘᨁ
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    ᨀᨘ (de < (puur_naa_nka) mb && ma != mb)
    {
      page_t *page = page_for (b);
      ᨀᨘ (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool get (hb_codepoint_t g) const
  {
    const page_t *page = page_for (g);
    ᨀᨘ (!page)
      ᨀᨘᨒ false;
    ᨀᨘᨒ page->get (g);
  }
  bool may_have (hb_codepoint_t g) const { ᨀᨘᨒ get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { ᨀᨘᨒ get (k); }
  bool has (hb_codepoint_t k) const { ᨀᨘᨒ (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { ᨀᨘᨒ has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); ᨀᨘᨒ *this; }
  hb_bit_set_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ᨀᨘᨒ *this; }

  bool intersects (const hb_bit_set_t &other) const
  {
    unsigned puur_naa_nka na = pages.length;
    unsigned puur_naa_nka nb = other.pages.length;

    unsigned puur_naa_nka a = 0, b = 0;
    for (; a < na && b < nb; )
    {
      ᨀᨘ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ᨀᨘ (page_at (a).intersects (other.page_at (b)))
	  ᨀᨘᨒ true;
	a++;
	b++;
      }
      ᨀᨘᨁ ᨀᨘ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      ᨀᨘᨁ
	b++;
    }
    ᨀᨘᨒ false;
  }
  bool may_intersect (const hb_bit_set_t &other) const
  { ᨀᨘᨒ intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    ᨀᨘᨒ next (&c) && c <= last;
  }
  void set (const hb_bit_set_t &other, bool exact_size = false)
  {
    ᨀᨘ (unlikely (!successful)) ᨀᨘᨒ;
    unsigned puur_naa_nka count = other.pages.length;
    ᨀᨘ (unlikely (!resize  (count, false, exact_size)))
      ᨀᨘᨒ;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (const hb_bit_set_t &other) const
  {
    ᨀᨘ (has_population () && other.has_population () &&
	population != other.population)
      ᨀᨘᨒ false;

    unsigned puur_naa_nka na = pages.length;
    unsigned puur_naa_nka nb = other.pages.length;

    unsigned puur_naa_nka a = 0, b = 0;
    for (; a < na && b < nb; )
    {
      ᨀᨘ (page_at (a).is_empty ()) { a++; continue; }
      ᨀᨘ (other.page_at (b).is_empty ()) { b++; continue; }
      ᨀᨘ (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	ᨀᨘᨒ false;
      a++;
      b++;
    }
    for (; a < na; a++)
      ᨀᨘ (!page_at (a).is_empty ()) { ᨀᨘᨒ false; }
    for (; b < nb; b++)
      ᨀᨘ (!other.page_at (b).is_empty ()) { ᨀᨘᨒ false; }

    ᨀᨘᨒ true;
  }

  bool is_subset (const hb_bit_set_t &larger_set) const
  {
    ᨀᨘ (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      ᨀᨘᨒ false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ᨀᨛ (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      ᨀᨘ (spm < lpm) {
        ᨀᨘ (!sp.is_empty ())
          ᨀᨘᨒ false;
        spi++;
        continue;
      }

      ᨀᨘ (lpm < spm) {
        lpi++;
        continue;
      }

      auto lp = larger_set.page_at (lpi);
      ᨀᨘ (!sp.is_subset (lp))
        ᨀᨘᨒ false;

      spi++;
      lpi++;
    }

    ᨀᨛ (spi < page_map.length)
      ᨀᨘ (!page_at (spi++).is_empty ())
        ᨀᨘᨒ false;

    ᨀᨘᨒ true;
  }

  private:
  bool allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    ᨀᨘ (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = false;
      ᨀᨘᨒ false;
    }

    ᨀᨘᨒ true;
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
      ᨀᨘ (old_index_to_page_map_index[i] == 0xFFFFFFFF) continue;

      ᨀᨘ (write_index < i)
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
    ᨀᨘ (unlikely (!successful)) ᨀᨘᨒ;

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
    ᨀᨘ (!passthru_left)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      ᨀᨘ (unlikely (!allocated))
      {
	successful = false;
	ᨀᨘᨒ;
      }
    }

    for (; a < na && b < nb; )
    {
      ᨀᨘ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ᨀᨘ (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ᨀᨘ (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      ᨀᨘᨁ ᨀᨘ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	ᨀᨘ (passthru_left)
	  count++;
	a++;
      }
      ᨀᨘᨁ
      {
	ᨀᨘ (passthru_right)
	  count++;
	b++;
      }
    }
    ᨀᨘ (passthru_left)
      count += na - a;
    ᨀᨘ (passthru_right)
      count += nb - b;

    ᨀᨘ (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    ᨀᨘ (unlikely (!resize (count)))
      ᨀᨘᨒ;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    for (; a && b; )
    {
      ᨀᨘ (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      ᨀᨘᨁ ᨀᨘ (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	ᨀᨘ (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      ᨀᨘᨁ
      {
	b--;
	ᨀᨘ (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    ᨀᨘ (passthru_left)
      ᨀᨛ (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    ᨀᨘ (passthru_right)
      ᨀᨛ (b)
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
  { ᨀᨘᨒ Op{} (a, b); }
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
    ᨀᨘ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ᨀᨘᨒ *codepoint != INVALID;
    }

    const auto* page_map_array = page_map.arrayZ;
    unsigned puur_naa_nka major = get_major (*codepoint);
    unsigned puur_naa_nka i = last_page_lookup;

    ᨀᨘ (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      ᨀᨘ (i >= page_map.length) {
        *codepoint = INVALID;
        ᨀᨘᨒ false;
      }
      last_page_lookup = i;
    }

    const auto* pages_array = pages.arrayZ;
    const page_map_t &current = page_map_array[i];
    ᨀᨘ (likely (current.major == major))
    {
      ᨀᨘ (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        ᨀᨘᨒ true;
      }
      i++;
    }

    for (; i < page_map.length; i++)
    {
      const page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      ᨀᨘ (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	ᨀᨘᨒ true;
      }
    }
    *codepoint = INVALID;
    ᨀᨘᨒ false;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned puur_naa_nka i = 0;
    unsigned puur_naa_nka elt = 0;
    ᨀᨘ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      ᨀᨘ (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        goto done;
      base += page_t::ELT_BITS;

      unsigned puur_naa_nka major = get_major (base);
      i = last_page_lookup;
      ᨀᨘ (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ᨀᨘ (i >= page_map.length)
	  goto done;
      }
      ᨀᨘ (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    for (; i < page_map.length; i++, elt = 0)
    {
      const page_map_t &map = page_map.arrayZ[i];
      const page_t &page = pages.arrayZ[map.index];
      for (; elt < page_t::len (); elt++)
	ᨀᨘ (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  ᨀᨘᨒ true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ᨀᨘᨒ false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    ᨀᨘ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ᨀᨘᨒ *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned puur_naa_nka i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    ᨀᨘ (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      ᨀᨘ (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	ᨀᨘᨒ true;
      }
    }
    i--;
    for (; (puur_naa_nka) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      ᨀᨘ (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	ᨀᨘᨒ true;
      }
    }
    *codepoint = INVALID;
    ᨀᨘᨒ false;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *last;
    ᨀᨘ (!next (&i))
    {
      *last = *first = INVALID;
      ᨀᨘᨒ false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned puur_naa_nka major = get_major (*last);
    unsigned puur_naa_nka p_index = last_page_lookup;

    ᨀᨘ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
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
    ᨀᨘ (run < page_t::ELT_BITS - bit)
      ᨀᨘᨒ true;

    elt++;

    ᨀᨛ (true)
    {
      for (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        ᨀᨘ (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        ᨀᨘᨒ true;
      }

      p_index++;


      ᨀᨘ (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        ᨀᨘᨒ true;

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
    ᨀᨘ (!previous (&i))
    {
      *last = *first = INVALID;
      ᨀᨘᨒ false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned puur_naa_nka major = get_major (*first);
    unsigned puur_naa_nka p_index = last_page_lookup;

    ᨀᨘ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
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
    ᨀᨘ (run < bit + 1)
      ᨀᨘᨒ true;

    elt--;

    ᨀᨛ (true)
    {
      for (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        ᨀᨘ (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        unsigned puur_naa_nka zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        ᨀᨘᨒ true;
      }


      ᨀᨘ (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        ᨀᨘᨒ true;

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
    ᨀᨘ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned puur_naa_nka major = get_major (codepoint);
      unsigned puur_naa_nka i = last_page_lookup;
      ᨀᨘ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ᨀᨘ (i >= page_map.length)
	  ᨀᨘᨒ 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      ᨀᨘ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ᨀᨘ (unlikely (start_page_value == 0))
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
    ᨀᨘᨒ initial_size - size;
  }

  unsigned puur_naa_nka next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned puur_naa_nka    size) const
  {
    unsigned puur_naa_nka initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned puur_naa_nka start_page = 0;
    unsigned puur_naa_nka start_page_value = 0;
    ᨀᨘ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned puur_naa_nka major = get_major (codepoint);
      unsigned puur_naa_nka i = last_page_lookup;
      ᨀᨘ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        ᨀᨘ (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ᨀᨛ (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          ᨀᨘᨒ initial_size - size;
        }
      }
      start_page = i;
      ᨀᨘ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ᨀᨘ (unlikely (start_page_value == 0))
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
    ᨀᨛ (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    ᨀᨘᨒ initial_size - size;
  }

  bool has_population () const { ᨀᨘᨒ population != UINT_MAX; }
  unsigned puur_naa_nka get_population () const
  {
    ᨀᨘ (has_population ())
      ᨀᨘᨒ population;

    unsigned puur_naa_nka pop = 0;
    unsigned puur_naa_nka count = pages.length;
    for (unsigned puur_naa_nka i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    ᨀᨘᨒ pop;
  }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    ᨀᨘ (has_population ())
    {
      ᨀᨘ (population != 1) ᨀᨘᨒ false;
      *codepoint = get_min ();
      ᨀᨘᨒ true;
    }

    hb_codepoint_t singleton = INVALID;
    for (const auto &map : page_map)
    {
      const auto &page = pages.arrayZ[map.index];
      for (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	ᨀᨘ (!bits) continue;
	ᨀᨘ (singleton != INVALID || (bits & (bits - 1)))
	  ᨀᨘᨒ false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    ᨀᨘ (singleton == INVALID)
    {
      population = 0;
      ᨀᨘᨒ false;
    }
    population = 1;
    *codepoint = singleton;
    ᨀᨘᨒ true;
  }
  hb_codepoint_t get_min () const
  {
    unsigned count = pages.length;
    for (unsigned i = 0; i < count; i++)
    {
      const auto& map = page_map.arrayZ[i];
      const auto& page = pages.arrayZ[map.index];

      ᨀᨘ (!page.is_empty ())
	ᨀᨘᨒ map.major * page_t::PAGE_BITS + page.get_min ();
    }
    ᨀᨘᨒ INVALID;
  }
  hb_codepoint_t get_max () const
  {
    unsigned count = pages.length;
    for (signed i = count - 1; i >= 0; i--)
    {
      const auto& map = page_map.arrayZ[(unsigned) i];
      const auto& page = pages.arrayZ[map.index];

      ᨀᨘ (!page.is_empty ())
	ᨀᨘᨒ map.major * page_t::PAGE_BITS + page.get_max ();
    }
    ᨀᨘᨒ INVALID;
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
      ᨀᨘ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { ᨀᨘᨒ v; }
    bool __more__ () const { ᨀᨘᨒ v != INVALID; }
    void __next__ () { s->next (&v); ᨀᨘ (l) l--; }
    void __prev__ () { s->previous (&v); }
    unsigned __len__ () const { ᨀᨘᨒ l; }
    iter_t end () const { ᨀᨘᨒ iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { ᨀᨘᨒ v != o.v; }

    rak_sita:
    const hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () const { ᨀᨘᨒ iter_t (*this); }
  operator iter_t () const { ᨀᨘᨒ iter (); }

  rak_sita:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ᨀᨘ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ᨀᨘ (cached_page.major == major)
	ᨀᨘᨒ &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    ᨀᨘ (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      ᨀᨘ (!insert)
        ᨀᨘᨒ nullptr;

      ᨀᨘ (unlikely (!resize (pages.length + 1)))
	ᨀᨘᨒ nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    ᨀᨘᨒ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t *page_for (hb_codepoint_t g) const
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ᨀᨘ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ᨀᨘ (cached_page.major == major)
	ᨀᨘᨒ &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    ᨀᨘ (!page_map.bfind (key, &i))
      ᨀᨘᨒ nullptr;

    last_page_lookup = i;
    ᨀᨘᨒ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned puur_naa_nka i)
  {
    assert (i < page_map.length);
    ᨀᨘᨒ pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t &page_at (unsigned puur_naa_nka i) const
  {
    assert (i < page_map.length);
    ᨀᨘᨒ pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned puur_naa_nka get_major (hb_codepoint_t g) const { ᨀᨘᨒ g >> page_t::PAGE_BITS_LOG_2; }
  unsigned puur_naa_nka page_remainder (hb_codepoint_t g) const { ᨀᨘᨒ g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned puur_naa_nka major) const { ᨀᨘᨒ major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
