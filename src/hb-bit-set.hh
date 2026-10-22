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
  hb_bit_set_t& operator= (const hb_bit_set_t& other) { set (other); ត្រឡប់ *this; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*this, other); ត្រឡប់ *this; }
  friend void swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    ប្រសិនបើ (likely (!a.successful || !b.successful))
      ត្រឡប់;
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
    ចំនួនគត់ cmp (const page_map_t &o) const { ត្រឡប់ cmp (o.major); }
    ចំនួនគត់ cmp (uint32_t o_major) const { ត្រឡប់ (ចំនួនគត់) o_major - (ចំនួនគត់) major; }

    uint32_t major;
    uint32_t index;
  };

  bool successful = true; /* Allocations successful */
  mutable unsigned ចំនួនគត់ population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  void err () { ប្រសិនបើ (successful) successful = false; } /* TODO Remove */
  bool in_error () const { ត្រឡប់ !successful; }

  bool resize (unsigned ចំនួនគត់ count, bool clear = true, bool exact_size = false)
  {
    ប្រសិនបើ (unlikely (!successful)) ត្រឡប់ false;

    ប្រសិនបើ (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = true; // Most sets are small and local

    ប្រសិនបើ (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, false)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = false;
      ត្រឡប់ false;
    }
    ត្រឡប់ true;
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
    ត្រឡប់ *this;
  }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ប្រសិនបើ (!pages.length && !population) ត្រឡប់;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  bool is_empty () const
  {
    unsigned ចំនួនគត់ count = pages.length;
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < count; i++)
      ប្រសិនបើ (!pages[i].is_empty ())
	ត្រឡប់ false;
    ត្រឡប់ true;
  }
  explicit operator bool () const { ត្រឡប់ !is_empty (); }

  uint32_t hash () const
  {
    uint32_t h = 0;
    សម្រាប់ (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      ប្រសិនបើ (unlikely (page.is_empty ())) continue;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    ត្រឡប់ h;
  }

  private:
  void dirty () { population = UINT_MAX; }
  public:

  void add (hb_codepoint_t g)
  {
    ប្រសិនបើ (unlikely (!successful)) ត្រឡប់;
    ប្រសិនបើ (unlikely (g == INVALID)) ត្រឡប់;
    dirty ();
    page_t *page = page_for (g, true); ប្រសិនបើ (unlikely (!page)) ត្រឡប់;
    page->add (g);
  }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  {
    ប្រសិនបើ (unlikely (!successful) || unlikely (!bits)) ត្រឡប់;
    assert (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); ប្រសិនបើ (unlikely (!page)) ត្រឡប់;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ប្រសិនបើ (unlikely (!successful)) ត្រឡប់ true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ប្រសិនបើ (unlikely (a > b || a == INVALID || b == INVALID)) ត្រឡប់ false;
    dirty ();
    unsigned ចំនួនគត់ ma = get_major (a);
    unsigned ចំនួនគត់ mb = get_major (b);
    ប្រសិនបើ (ma == mb)
    {
      page_t *page = page_for (a, true); ប្រសិនបើ (unlikely (!page)) ត្រឡប់ false;
      page->add_range (a, b);
    }
    បើមិនដូច្នេះ
    {
      page_t *page = page_for (a, true); ប្រសិនបើ (unlikely (!page)) ត្រឡប់ false;
      page->add_range (a, major_start (ma + 1) - 1);

      សម្រាប់ (unsigned ចំនួនគត់ m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); ប្រសិនបើ (unlikely (!page)) ត្រឡប់ false;
	page->init1 ();
      }

      page = page_for (b, true); ប្រសិនបើ (unlikely (!page)) ត្រឡប់ false;
      page->add_range (major_start (mb), b);
    }
    ត្រឡប់ true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  static anta_hstha const Type& StructAtOffsetUnaligned(const void *P, unsigned ចំនួនគត់ offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    ត្រឡប់ * reinterpret_cast<const Type*> ((const var_na *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  void set_array (bool v, const T *array, unsigned ចំនួនគត់ count, unsigned ចំនួនគត់ stride=parimaa_na(T))
  {
    ប្រសិនបើ (unlikely (!successful)) ត្រឡប់;
    ប្រសិនបើ (!count) ត្រឡប់;
    dirty ();
    hb_codepoint_t g = *array;
    ខណៈពេល (count)
    {
      unsigned ចំនួនគត់ m = get_major (g);
      page_t *page = page_for (g, v); ប្រសិនបើ (unlikely (v && !page)) ត្រឡប់;
      unsigned ចំនួនគត់ start = major_start (m);
      unsigned ចំនួនគត់ end = major_start (m + 1);
      do
      {
        ប្រសិនបើ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ខណៈពេល (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  void add_array (const T *array, unsigned ចំនួនគត់ count, unsigned ចំនួនគត់ stride=parimaa_na(T))
  { set_array (true, array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  void del_array (const T *array, unsigned ចំនួនគត់ count, unsigned ចំនួនគត់ stride=parimaa_na(T))
  { set_array (false, array, count, stride); }
  template <typename T>
  void del_array (const hb_array_t<const T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool set_sorted_array (bool v, const T *array, unsigned ចំនួនគត់ count, unsigned ចំនួនគត់ stride=parimaa_na(T))
  {
    ប្រសិនបើ (unlikely (!successful)) ត្រឡប់ true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ប្រសិនបើ (unlikely (!count)) ត្រឡប់ true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ខណៈពេល (count)
    {
      unsigned ចំនួនគត់ m = get_major (g);
      page_t *page = page_for (g, v); ប្រសិនបើ (unlikely (v && !page)) ត្រឡប់ false;
      unsigned ចំនួនគត់ end = major_start (m + 1);
      do
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ប្រសិនបើ (g < last_g) ត្រឡប់ false;
	last_g = g;

        ប្រសិនបើ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ខណៈពេល (count && (g = *array, g < end));
    }
    ត្រឡប់ true;
  }

  template <typename T>
  bool add_sorted_array (const T *array, unsigned ចំនួនគត់ count, unsigned ចំនួនគត់ stride=parimaa_na(T))
  { ត្រឡប់ set_sorted_array (true, array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { ត្រឡប់ add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  bool del_sorted_array (const T *array, unsigned ចំនួនគត់ count, unsigned ចំនួនគត់ stride=parimaa_na(T))
  { ត្រឡប់ set_sorted_array (false, array, count, stride); }
  template <typename T>
  bool del_sorted_array (const hb_sorted_array_t<const T>& arr) { ត្រឡប់ del_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g)
  {
    ប្រសិនបើ (unlikely (!successful)) ត្រឡប់;
    page_t *page = page_for (g);
    ប្រសិនបើ (!page)
      ត្រឡប់;
    dirty ();
    page->del (g);
  }
  void del_bits (hb_codepoint_t g, uint64_t bits)
  {
    ប្រសិនបើ (unlikely (!successful) || unlikely (!bits)) ត្រឡប់;
    assert (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    ប្រសិនបើ (!page)
      ត្រឡប់;
    dirty ();
    page->del_bits (g, bits);
  }

  private:
  void del_pages (ចំនួនគត់ ds, ចំនួនគត់ de)
  {
    ប្រសិនបើ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      ប្រសិនបើ (unlikely (!allocate_compact_workspace (compact_workspace))) ត្រឡប់;

      unsigned ចំនួនគត់ write_index = 0;
      សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < page_map.length; i++)
      {
	ចំនួនគត់ m = (ចំនួនគត់) page_map.arrayZ[i].major;
	ប្រសិនបើ (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  public:
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ប្រសិនបើ (unlikely (!successful)) ត្រឡប់;
    ប្រសិនបើ (unlikely (a > b || a == INVALID)) ត្រឡប់;
    dirty ();
    unsigned ចំនួនគត់ ma = get_major (a);
    unsigned ចំនួនគត់ mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    ចំនួនគត់ ds = (a == major_start (ma))? (ចំនួនគត់) ma: (ចំនួនគត់) (ma + 1);
    ចំនួនគត់ de = (b + 1 == major_start (mb + 1))? (ចំនួនគត់) mb: ((ចំនួនគត់) mb - 1);
    ប្រសិនបើ (ds > de || (ចំនួនគត់) ma < ds)
    {
      page_t *page = page_for (a);
      ប្រសិនបើ (page)
      {
	ប្រសិនបើ (ma == mb)
	  page->del_range (a, b);
	បើមិនដូច្នេះ
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    ប្រសិនបើ (de < (ចំនួនគត់) mb && ma != mb)
    {
      page_t *page = page_for (b);
      ប្រសិនបើ (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool get (hb_codepoint_t g) const
  {
    const page_t *page = page_for (g);
    ប្រសិនបើ (!page)
      ត្រឡប់ false;
    ត្រឡប់ page->get (g);
  }
  bool may_have (hb_codepoint_t g) const { ត្រឡប់ get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { ត្រឡប់ get (k); }
  bool has (hb_codepoint_t k) const { ត្រឡប់ (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { ត្រឡប់ has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); ត្រឡប់ *this; }
  hb_bit_set_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ត្រឡប់ *this; }

  bool intersects (const hb_bit_set_t &other) const
  {
    unsigned ចំនួនគត់ na = pages.length;
    unsigned ចំនួនគត់ nb = other.pages.length;

    unsigned ចំនួនគត់ a = 0, b = 0;
    សម្រាប់ (; a < na && b < nb; )
    {
      ប្រសិនបើ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ប្រសិនបើ (page_at (a).intersects (other.page_at (b)))
	  ត្រឡប់ true;
	a++;
	b++;
      }
      បើមិនដូច្នេះ ប្រសិនបើ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      បើមិនដូច្នេះ
	b++;
    }
    ត្រឡប់ false;
  }
  bool may_intersect (const hb_bit_set_t &other) const
  { ត្រឡប់ intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    ត្រឡប់ next (&c) && c <= last;
  }
  void set (const hb_bit_set_t &other, bool exact_size = false)
  {
    ប្រសិនបើ (unlikely (!successful)) ត្រឡប់;
    unsigned ចំនួនគត់ count = other.pages.length;
    ប្រសិនបើ (unlikely (!resize  (count, false, exact_size)))
      ត្រឡប់;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (const hb_bit_set_t &other) const
  {
    ប្រសិនបើ (has_population () && other.has_population () &&
	population != other.population)
      ត្រឡប់ false;

    unsigned ចំនួនគត់ na = pages.length;
    unsigned ចំនួនគត់ nb = other.pages.length;

    unsigned ចំនួនគត់ a = 0, b = 0;
    សម្រាប់ (; a < na && b < nb; )
    {
      ប្រសិនបើ (page_at (a).is_empty ()) { a++; continue; }
      ប្រសិនបើ (other.page_at (b).is_empty ()) { b++; continue; }
      ប្រសិនបើ (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	ត្រឡប់ false;
      a++;
      b++;
    }
    សម្រាប់ (; a < na; a++)
      ប្រសិនបើ (!page_at (a).is_empty ()) { ត្រឡប់ false; }
    សម្រាប់ (; b < nb; b++)
      ប្រសិនបើ (!other.page_at (b).is_empty ()) { ត្រឡប់ false; }

    ត្រឡប់ true;
  }

  bool is_subset (const hb_bit_set_t &larger_set) const
  {
    ប្រសិនបើ (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      ត្រឡប់ false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ខណៈពេល (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      ប្រសិនបើ (spm < lpm) {
        ប្រសិនបើ (!sp.is_empty ())
          ត្រឡប់ false;
        spi++;
        continue;
      }

      ប្រសិនបើ (lpm < spm) {
        lpi++;
        continue;
      }

      auto lp = larger_set.page_at (lpi);
      ប្រសិនបើ (!sp.is_subset (lp))
        ត្រឡប់ false;

      spi++;
      lpi++;
    }

    ខណៈពេល (spi < page_map.length)
      ប្រសិនបើ (!page_at (spi++).is_empty ())
        ត្រឡប់ false;

    ត្រឡប់ true;
  }

  private:
  bool allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    ប្រសិនបើ (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = false;
      ត្រឡប់ false;
    }

    ត្រឡប់ true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  void compact (hb_vector_t<unsigned>& workspace,
                unsigned ចំនួនគត់ length)
  {
    assert(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    សម្រាប់ (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  void compact_pages (const hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned ចំនួនគត់ write_index = 0;
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < pages.length; i++)
    {
      ប្រសិនបើ (old_index_to_page_map_index[i] == 0xFFFFFFFF) continue;

      ប្រសិនបើ (write_index < i)
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
    ប្រសិនបើ (unlikely (!successful)) ត្រឡប់;

    dirty ();

    unsigned ចំនួនគត់ na = pages.length;
    unsigned ចំនួនគត់ nb = other.pages.length;
    unsigned ចំនួនគត់ next_page = na;

    unsigned ចំនួនគត់ count = 0, newCount = 0;
    unsigned ចំនួនគត់ a = 0, b = 0;
    unsigned ចំនួនគត់ write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    ប្រសិនបើ (!passthru_left)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      ប្រសិនបើ (unlikely (!allocated))
      {
	successful = false;
	ត្រឡប់;
      }
    }

    សម្រាប់ (; a < na && b < nb; )
    {
      ប្រសិនបើ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ប្រសិនបើ (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ប្រសិនបើ (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      បើមិនដូច្នេះ ប្រសិនបើ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	ប្រសិនបើ (passthru_left)
	  count++;
	a++;
      }
      បើមិនដូច្នេះ
      {
	ប្រសិនបើ (passthru_right)
	  count++;
	b++;
      }
    }
    ប្រសិនបើ (passthru_left)
      count += na - a;
    ប្រសិនបើ (passthru_right)
      count += nb - b;

    ប្រសិនបើ (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    ប្រសិនបើ (unlikely (!resize (count)))
      ត្រឡប់;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    សម្រាប់ (; a && b; )
    {
      ប្រសិនបើ (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      បើមិនដូច្នេះ ប្រសិនបើ (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	ប្រសិនបើ (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      បើមិនដូច្នេះ
      {
	b--;
	ប្រសិនបើ (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    ប្រសិនបើ (passthru_left)
      ខណៈពេល (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    ប្រសិនបើ (passthru_right)
      ខណៈពេល (b)
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
  { ត្រឡប់ Op{} (a, b); }
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
    ប្រសិនបើ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ត្រឡប់ *codepoint != INVALID;
    }

    const auto* page_map_array = page_map.arrayZ;
    unsigned ចំនួនគត់ major = get_major (*codepoint);
    unsigned ចំនួនគត់ i = last_page_lookup;

    ប្រសិនបើ (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      ប្រសិនបើ (i >= page_map.length) {
        *codepoint = INVALID;
        ត្រឡប់ false;
      }
      last_page_lookup = i;
    }

    const auto* pages_array = pages.arrayZ;
    const page_map_t &current = page_map_array[i];
    ប្រសិនបើ (likely (current.major == major))
    {
      ប្រសិនបើ (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        ត្រឡប់ true;
      }
      i++;
    }

    សម្រាប់ (; i < page_map.length; i++)
    {
      const page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      ប្រសិនបើ (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	ត្រឡប់ true;
      }
    }
    *codepoint = INVALID;
    ត្រឡប់ false;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned ចំនួនគត់ i = 0;
    unsigned ចំនួនគត់ elt = 0;
    ប្រសិនបើ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      ប្រសិនបើ (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        goto done;
      base += page_t::ELT_BITS;

      unsigned ចំនួនគត់ major = get_major (base);
      i = last_page_lookup;
      ប្រសិនបើ (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ប្រសិនបើ (i >= page_map.length)
	  goto done;
      }
      ប្រសិនបើ (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    សម្រាប់ (; i < page_map.length; i++, elt = 0)
    {
      const page_map_t &map = page_map.arrayZ[i];
      const page_t &page = pages.arrayZ[map.index];
      សម្រាប់ (; elt < page_t::len (); elt++)
	ប្រសិនបើ (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  ត្រឡប់ true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ត្រឡប់ false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    ប្រសិនបើ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ត្រឡប់ *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned ចំនួនគត់ i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    ប្រសិនបើ (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      ប្រសិនបើ (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	ត្រឡប់ true;
      }
    }
    i--;
    សម្រាប់ (; (ចំនួនគត់) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      ប្រសិនបើ (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	ត្រឡប់ true;
      }
    }
    *codepoint = INVALID;
    ត្រឡប់ false;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *last;
    ប្រសិនបើ (!next (&i))
    {
      *last = *first = INVALID;
      ត្រឡប់ false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned ចំនួនគត់ major = get_major (*last);
    unsigned ចំនួនគត់ p_index = last_page_lookup;

    ប្រសិនបើ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ចំនួនគត់ rem = page_remainder (*last);
    unsigned ចំនួនគត់ elt = rem / page_t::ELT_BITS;
    unsigned ចំនួនគត់ bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned ចំនួនគត់ run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    ប្រសិនបើ (run < page_t::ELT_BITS - bit)
      ត្រឡប់ true;

    elt++;

    ខណៈពេល (true)
    {
      សម្រាប់ (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        ប្រសិនបើ (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        ត្រឡប់ true;
      }

      p_index++;


      ប្រសិនបើ (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        ត្រឡប់ true;

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
    ប្រសិនបើ (!previous (&i))
    {
      *last = *first = INVALID;
      ត្រឡប់ false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned ចំនួនគត់ major = get_major (*first);
    unsigned ចំនួនគត់ p_index = last_page_lookup;

    ប្រសិនបើ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ចំនួនគត់ rem = page_remainder (*first);
    ចំនួនគត់ elt = rem / page_t::ELT_BITS;
    unsigned ចំនួនគត់ bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned ចំនួនគត់ run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    ប្រសិនបើ (run < bit + 1)
      ត្រឡប់ true;

    elt--;

    ខណៈពេល (true)
    {
      សម្រាប់ (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        ប្រសិនបើ (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        unsigned ចំនួនគត់ zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        ត្រឡប់ true;
      }


      ប្រសិនបើ (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        ត្រឡប់ true;

      p_index--;
      const auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned ចំនួនគត់ next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned ចំនួនគត់    size) const
  {
    // By default, start at the first bit of the first page of values.
    unsigned ចំនួនគត់ start_page = 0;
    unsigned ចំនួនគត់ start_page_value = 0;
    ប្រសិនបើ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned ចំនួនគត់ major = get_major (codepoint);
      unsigned ចំនួនគត់ i = last_page_lookup;
      ប្រសិនបើ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ប្រសិនបើ (i >= page_map.length)
	  ត្រឡប់ 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      ប្រសិនបើ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ប្រសិនបើ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned ចំនួនគត់ initial_size = size;
    សម្រាប់ (unsigned ចំនួនគត់ i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ចំនួនគត់ n = pages[page_map.arrayZ[i].index].write (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ត្រឡប់ initial_size - size;
  }

  unsigned ចំនួនគត់ next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned ចំនួនគត់    size) const
  {
    unsigned ចំនួនគត់ initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned ចំនួនគត់ start_page = 0;
    unsigned ចំនួនគត់ start_page_value = 0;
    ប្រសិនបើ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned ចំនួនគត់ major = get_major (codepoint);
      unsigned ចំនួនគត់ i = last_page_lookup;
      ប្រសិនបើ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        ប្រសិនបើ (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ខណៈពេល (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          ត្រឡប់ initial_size - size;
        }
      }
      start_page = i;
      ប្រសិនបើ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ប្រសិនបើ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    សម្រាប់ (unsigned ចំនួនគត់ i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ចំនួនគត់ n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ខណៈពេល (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    ត្រឡប់ initial_size - size;
  }

  bool has_population () const { ត្រឡប់ population != UINT_MAX; }
  unsigned ចំនួនគត់ get_population () const
  {
    ប្រសិនបើ (has_population ())
      ត្រឡប់ population;

    unsigned ចំនួនគត់ pop = 0;
    unsigned ចំនួនគត់ count = pages.length;
    សម្រាប់ (unsigned ចំនួនគត់ i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    ត្រឡប់ pop;
  }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    ប្រសិនបើ (has_population ())
    {
      ប្រសិនបើ (population != 1) ត្រឡប់ false;
      *codepoint = get_min ();
      ត្រឡប់ true;
    }

    hb_codepoint_t singleton = INVALID;
    សម្រាប់ (const auto &map : page_map)
    {
      const auto &page = pages.arrayZ[map.index];
      សម្រាប់ (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	ប្រសិនបើ (!bits) continue;
	ប្រសិនបើ (singleton != INVALID || (bits & (bits - 1)))
	  ត្រឡប់ false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    ប្រសិនបើ (singleton == INVALID)
    {
      population = 0;
      ត្រឡប់ false;
    }
    population = 1;
    *codepoint = singleton;
    ត្រឡប់ true;
  }
  hb_codepoint_t get_min () const
  {
    unsigned count = pages.length;
    សម្រាប់ (unsigned i = 0; i < count; i++)
    {
      const auto& map = page_map.arrayZ[i];
      const auto& page = pages.arrayZ[map.index];

      ប្រសិនបើ (!page.is_empty ())
	ត្រឡប់ map.major * page_t::PAGE_BITS + page.get_min ();
    }
    ត្រឡប់ INVALID;
  }
  hb_codepoint_t get_max () const
  {
    unsigned count = pages.length;
    សម្រាប់ (signed i = count - 1; i >= 0; i--)
    {
      const auto& map = page_map.arrayZ[(unsigned) i];
      const auto& page = pages.arrayZ[map.index];

      ប្រសិនបើ (!page.is_empty ())
	ត្រឡប់ map.major * page_t::PAGE_BITS + page.get_max ();
    }
    ត្រឡប់ INVALID;
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
      ប្រសិនបើ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { ត្រឡប់ v; }
    bool __more__ () const { ត្រឡប់ v != INVALID; }
    void __next__ () { s->next (&v); ប្រសិនបើ (l) l--; }
    void __prev__ () { s->previous (&v); }
    unsigned __len__ () const { ត្រឡប់ l; }
    iter_t end () const { ត្រឡប់ iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { ត្រឡប់ v != o.v; }

    rak_sita:
    const hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () const { ត្រឡប់ iter_t (*this); }
  operator iter_t () const { ត្រឡប់ iter (); }

  rak_sita:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ប្រសិនបើ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ប្រសិនបើ (cached_page.major == major)
	ត្រឡប់ &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    ប្រសិនបើ (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      ប្រសិនបើ (!insert)
        ត្រឡប់ nullptr;

      ប្រសិនបើ (unlikely (!resize (pages.length + 1)))
	ត្រឡប់ nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    ត្រឡប់ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t *page_for (hb_codepoint_t g) const
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ប្រសិនបើ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ប្រសិនបើ (cached_page.major == major)
	ត្រឡប់ &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    ប្រសិនបើ (!page_map.bfind (key, &i))
      ត្រឡប់ nullptr;

    last_page_lookup = i;
    ត្រឡប់ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned ចំនួនគត់ i)
  {
    assert (i < page_map.length);
    ត្រឡប់ pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t &page_at (unsigned ចំនួនគត់ i) const
  {
    assert (i < page_map.length);
    ត្រឡប់ pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned ចំនួនគត់ get_major (hb_codepoint_t g) const { ត្រឡប់ g >> page_t::PAGE_BITS_LOG_2; }
  unsigned ចំនួនគត់ page_remainder (hb_codepoint_t g) const { ត្រឡប់ g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned ចំនួនគត់ major) const { ត្រឡប់ major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
