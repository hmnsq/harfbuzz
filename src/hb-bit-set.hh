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
  hb_bit_set_t& operator= (const hb_bit_set_t& other) { set (other); ສົ່ງຄືນ *this; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*this, other); ສົ່ງຄືນ *this; }
  friend void swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    ຖ້າ (likely (!a.successful || !b.successful))
      ສົ່ງຄືນ;
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
    ຈຳນວນເຕັມ cmp (const page_map_t &o) const { ສົ່ງຄືນ cmp (o.major); }
    ຈຳນວນເຕັມ cmp (uint32_t o_major) const { ສົ່ງຄືນ (ຈຳນວນເຕັມ) o_major - (ຈຳນວນເຕັມ) major; }

    uint32_t major;
    uint32_t index;
  };

  bool successful = true; /* Allocations successful */
  mutable unsigned ຈຳນວນເຕັມ population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  void err () { ຖ້າ (successful) successful = false; } /* TODO Remove */
  bool in_error () const { ສົ່ງຄືນ !successful; }

  bool resize (unsigned ຈຳນວນເຕັມ count, bool clear = true, bool exact_size = false)
  {
    ຖ້າ (unlikely (!successful)) ສົ່ງຄືນ false;

    ຖ້າ (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = true; // Most sets are small and local

    ຖ້າ (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, false)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = false;
      ສົ່ງຄືນ false;
    }
    ສົ່ງຄືນ true;
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
    ສົ່ງຄືນ *this;
  }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ຖ້າ (!pages.length && !population) ສົ່ງຄືນ;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  bool is_empty () const
  {
    unsigned ຈຳນວນເຕັມ count = pages.length;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
      ຖ້າ (!pages[i].is_empty ())
	ສົ່ງຄືນ false;
    ສົ່ງຄືນ true;
  }
  explicit operator bool () const { ສົ່ງຄືນ !is_empty (); }

  uint32_t hash () const
  {
    uint32_t h = 0;
    ສຳລັບ (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      ຖ້າ (unlikely (page.is_empty ())) continue;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    ສົ່ງຄືນ h;
  }

  private:
  void dirty () { population = UINT_MAX; }
  public:

  void add (hb_codepoint_t g)
  {
    ຖ້າ (unlikely (!successful)) ສົ່ງຄືນ;
    ຖ້າ (unlikely (g == INVALID)) ສົ່ງຄືນ;
    dirty ();
    page_t *page = page_for (g, true); ຖ້າ (unlikely (!page)) ສົ່ງຄືນ;
    page->add (g);
  }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  {
    ຖ້າ (unlikely (!successful) || unlikely (!bits)) ສົ່ງຄືນ;
    assert (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); ຖ້າ (unlikely (!page)) ສົ່ງຄືນ;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ຖ້າ (unlikely (!successful)) ສົ່ງຄືນ true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ຖ້າ (unlikely (a > b || a == INVALID || b == INVALID)) ສົ່ງຄືນ false;
    dirty ();
    unsigned ຈຳນວນເຕັມ ma = get_major (a);
    unsigned ຈຳນວນເຕັມ mb = get_major (b);
    ຖ້າ (ma == mb)
    {
      page_t *page = page_for (a, true); ຖ້າ (unlikely (!page)) ສົ່ງຄືນ false;
      page->add_range (a, b);
    }
    ບໍ່ດັ່ງນັ້ນ
    {
      page_t *page = page_for (a, true); ຖ້າ (unlikely (!page)) ສົ່ງຄືນ false;
      page->add_range (a, major_start (ma + 1) - 1);

      ສຳລັບ (unsigned ຈຳນວນເຕັມ m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); ຖ້າ (unlikely (!page)) ສົ່ງຄືນ false;
	page->init1 ();
      }

      page = page_for (b, true); ຖ້າ (unlikely (!page)) ສົ່ງຄືນ false;
      page->add_range (major_start (mb), b);
    }
    ສົ່ງຄືນ true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  static anta_hstha const Type& StructAtOffsetUnaligned(const void *P, unsigned ຈຳນວນເຕັມ offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    ສົ່ງຄືນ * reinterpret_cast<const Type*> ((const var_na *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  void set_array (bool v, const T *array, unsigned ຈຳນວນເຕັມ count, unsigned ຈຳນວນເຕັມ stride=parimaa_na(T))
  {
    ຖ້າ (unlikely (!successful)) ສົ່ງຄືນ;
    ຖ້າ (!count) ສົ່ງຄືນ;
    dirty ();
    hb_codepoint_t g = *array;
    ໃນຂະນະທີ່ (count)
    {
      unsigned ຈຳນວນເຕັມ m = get_major (g);
      page_t *page = page_for (g, v); ຖ້າ (unlikely (v && !page)) ສົ່ງຄືນ;
      unsigned ຈຳນວນເຕັມ start = major_start (m);
      unsigned ຈຳນວນເຕັມ end = major_start (m + 1);
      do
      {
        ຖ້າ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ໃນຂະນະທີ່ (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  void add_array (const T *array, unsigned ຈຳນວນເຕັມ count, unsigned ຈຳນວນເຕັມ stride=parimaa_na(T))
  { set_array (true, array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  void del_array (const T *array, unsigned ຈຳນວນເຕັມ count, unsigned ຈຳນວນເຕັມ stride=parimaa_na(T))
  { set_array (false, array, count, stride); }
  template <typename T>
  void del_array (const hb_array_t<const T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool set_sorted_array (bool v, const T *array, unsigned ຈຳນວນເຕັມ count, unsigned ຈຳນວນເຕັມ stride=parimaa_na(T))
  {
    ຖ້າ (unlikely (!successful)) ສົ່ງຄືນ true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ຖ້າ (unlikely (!count)) ສົ່ງຄືນ true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ໃນຂະນະທີ່ (count)
    {
      unsigned ຈຳນວນເຕັມ m = get_major (g);
      page_t *page = page_for (g, v); ຖ້າ (unlikely (v && !page)) ສົ່ງຄືນ false;
      unsigned ຈຳນວນເຕັມ end = major_start (m + 1);
      do
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ຖ້າ (g < last_g) ສົ່ງຄືນ false;
	last_g = g;

        ຖ້າ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ໃນຂະນະທີ່ (count && (g = *array, g < end));
    }
    ສົ່ງຄືນ true;
  }

  template <typename T>
  bool add_sorted_array (const T *array, unsigned ຈຳນວນເຕັມ count, unsigned ຈຳນວນເຕັມ stride=parimaa_na(T))
  { ສົ່ງຄືນ set_sorted_array (true, array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { ສົ່ງຄືນ add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  bool del_sorted_array (const T *array, unsigned ຈຳນວນເຕັມ count, unsigned ຈຳນວນເຕັມ stride=parimaa_na(T))
  { ສົ່ງຄືນ set_sorted_array (false, array, count, stride); }
  template <typename T>
  bool del_sorted_array (const hb_sorted_array_t<const T>& arr) { ສົ່ງຄືນ del_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g)
  {
    ຖ້າ (unlikely (!successful)) ສົ່ງຄືນ;
    page_t *page = page_for (g);
    ຖ້າ (!page)
      ສົ່ງຄືນ;
    dirty ();
    page->del (g);
  }
  void del_bits (hb_codepoint_t g, uint64_t bits)
  {
    ຖ້າ (unlikely (!successful) || unlikely (!bits)) ສົ່ງຄືນ;
    assert (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    ຖ້າ (!page)
      ສົ່ງຄືນ;
    dirty ();
    page->del_bits (g, bits);
  }

  private:
  void del_pages (ຈຳນວນເຕັມ ds, ຈຳນວນເຕັມ de)
  {
    ຖ້າ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      ຖ້າ (unlikely (!allocate_compact_workspace (compact_workspace))) ສົ່ງຄືນ;

      unsigned ຈຳນວນເຕັມ write_index = 0;
      ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < page_map.length; i++)
      {
	ຈຳນວນເຕັມ m = (ຈຳນວນເຕັມ) page_map.arrayZ[i].major;
	ຖ້າ (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  public:
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ຖ້າ (unlikely (!successful)) ສົ່ງຄືນ;
    ຖ້າ (unlikely (a > b || a == INVALID)) ສົ່ງຄືນ;
    dirty ();
    unsigned ຈຳນວນເຕັມ ma = get_major (a);
    unsigned ຈຳນວນເຕັມ mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    ຈຳນວນເຕັມ ds = (a == major_start (ma))? (ຈຳນວນເຕັມ) ma: (ຈຳນວນເຕັມ) (ma + 1);
    ຈຳນວນເຕັມ de = (b + 1 == major_start (mb + 1))? (ຈຳນວນເຕັມ) mb: ((ຈຳນວນເຕັມ) mb - 1);
    ຖ້າ (ds > de || (ຈຳນວນເຕັມ) ma < ds)
    {
      page_t *page = page_for (a);
      ຖ້າ (page)
      {
	ຖ້າ (ma == mb)
	  page->del_range (a, b);
	ບໍ່ດັ່ງນັ້ນ
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    ຖ້າ (de < (ຈຳນວນເຕັມ) mb && ma != mb)
    {
      page_t *page = page_for (b);
      ຖ້າ (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool get (hb_codepoint_t g) const
  {
    const page_t *page = page_for (g);
    ຖ້າ (!page)
      ສົ່ງຄືນ false;
    ສົ່ງຄືນ page->get (g);
  }
  bool may_have (hb_codepoint_t g) const { ສົ່ງຄືນ get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { ສົ່ງຄືນ get (k); }
  bool has (hb_codepoint_t k) const { ສົ່ງຄືນ (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { ສົ່ງຄືນ has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); ສົ່ງຄືນ *this; }
  hb_bit_set_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ສົ່ງຄືນ *this; }

  bool intersects (const hb_bit_set_t &other) const
  {
    unsigned ຈຳນວນເຕັມ na = pages.length;
    unsigned ຈຳນວນເຕັມ nb = other.pages.length;

    unsigned ຈຳນວນເຕັມ a = 0, b = 0;
    ສຳລັບ (; a < na && b < nb; )
    {
      ຖ້າ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ຖ້າ (page_at (a).intersects (other.page_at (b)))
	  ສົ່ງຄືນ true;
	a++;
	b++;
      }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      ບໍ່ດັ່ງນັ້ນ
	b++;
    }
    ສົ່ງຄືນ false;
  }
  bool may_intersect (const hb_bit_set_t &other) const
  { ສົ່ງຄືນ intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    ສົ່ງຄືນ next (&c) && c <= last;
  }
  void set (const hb_bit_set_t &other, bool exact_size = false)
  {
    ຖ້າ (unlikely (!successful)) ສົ່ງຄືນ;
    unsigned ຈຳນວນເຕັມ count = other.pages.length;
    ຖ້າ (unlikely (!resize  (count, false, exact_size)))
      ສົ່ງຄືນ;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (const hb_bit_set_t &other) const
  {
    ຖ້າ (has_population () && other.has_population () &&
	population != other.population)
      ສົ່ງຄືນ false;

    unsigned ຈຳນວນເຕັມ na = pages.length;
    unsigned ຈຳນວນເຕັມ nb = other.pages.length;

    unsigned ຈຳນວນເຕັມ a = 0, b = 0;
    ສຳລັບ (; a < na && b < nb; )
    {
      ຖ້າ (page_at (a).is_empty ()) { a++; continue; }
      ຖ້າ (other.page_at (b).is_empty ()) { b++; continue; }
      ຖ້າ (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	ສົ່ງຄືນ false;
      a++;
      b++;
    }
    ສຳລັບ (; a < na; a++)
      ຖ້າ (!page_at (a).is_empty ()) { ສົ່ງຄືນ false; }
    ສຳລັບ (; b < nb; b++)
      ຖ້າ (!other.page_at (b).is_empty ()) { ສົ່ງຄືນ false; }

    ສົ່ງຄືນ true;
  }

  bool is_subset (const hb_bit_set_t &larger_set) const
  {
    ຖ້າ (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      ສົ່ງຄືນ false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ໃນຂະນະທີ່ (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      ຖ້າ (spm < lpm) {
        ຖ້າ (!sp.is_empty ())
          ສົ່ງຄືນ false;
        spi++;
        continue;
      }

      ຖ້າ (lpm < spm) {
        lpi++;
        continue;
      }

      auto lp = larger_set.page_at (lpi);
      ຖ້າ (!sp.is_subset (lp))
        ສົ່ງຄືນ false;

      spi++;
      lpi++;
    }

    ໃນຂະນະທີ່ (spi < page_map.length)
      ຖ້າ (!page_at (spi++).is_empty ())
        ສົ່ງຄືນ false;

    ສົ່ງຄືນ true;
  }

  private:
  bool allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    ຖ້າ (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = false;
      ສົ່ງຄືນ false;
    }

    ສົ່ງຄືນ true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  void compact (hb_vector_t<unsigned>& workspace,
                unsigned ຈຳນວນເຕັມ length)
  {
    assert(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    ສຳລັບ (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  void compact_pages (const hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned ຈຳນວນເຕັມ write_index = 0;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < pages.length; i++)
    {
      ຖ້າ (old_index_to_page_map_index[i] == 0xFFFFFFFF) continue;

      ຖ້າ (write_index < i)
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
    ຖ້າ (unlikely (!successful)) ສົ່ງຄືນ;

    dirty ();

    unsigned ຈຳນວນເຕັມ na = pages.length;
    unsigned ຈຳນວນເຕັມ nb = other.pages.length;
    unsigned ຈຳນວນເຕັມ next_page = na;

    unsigned ຈຳນວນເຕັມ count = 0, newCount = 0;
    unsigned ຈຳນວນເຕັມ a = 0, b = 0;
    unsigned ຈຳນວນເຕັມ write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    ຖ້າ (!passthru_left)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      ຖ້າ (unlikely (!allocated))
      {
	successful = false;
	ສົ່ງຄືນ;
      }
    }

    ສຳລັບ (; a < na && b < nb; )
    {
      ຖ້າ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ຖ້າ (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ຖ້າ (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	ຖ້າ (passthru_left)
	  count++;
	a++;
      }
      ບໍ່ດັ່ງນັ້ນ
      {
	ຖ້າ (passthru_right)
	  count++;
	b++;
      }
    }
    ຖ້າ (passthru_left)
      count += na - a;
    ຖ້າ (passthru_right)
      count += nb - b;

    ຖ້າ (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    ຖ້າ (unlikely (!resize (count)))
      ສົ່ງຄືນ;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    ສຳລັບ (; a && b; )
    {
      ຖ້າ (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	ຖ້າ (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      ບໍ່ດັ່ງນັ້ນ
      {
	b--;
	ຖ້າ (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    ຖ້າ (passthru_left)
      ໃນຂະນະທີ່ (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    ຖ້າ (passthru_right)
      ໃນຂະນະທີ່ (b)
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
  { ສົ່ງຄືນ Op{} (a, b); }
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
    ຖ້າ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ສົ່ງຄືນ *codepoint != INVALID;
    }

    const auto* page_map_array = page_map.arrayZ;
    unsigned ຈຳນວນເຕັມ major = get_major (*codepoint);
    unsigned ຈຳນວນເຕັມ i = last_page_lookup;

    ຖ້າ (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      ຖ້າ (i >= page_map.length) {
        *codepoint = INVALID;
        ສົ່ງຄືນ false;
      }
      last_page_lookup = i;
    }

    const auto* pages_array = pages.arrayZ;
    const page_map_t &current = page_map_array[i];
    ຖ້າ (likely (current.major == major))
    {
      ຖ້າ (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        ສົ່ງຄືນ true;
      }
      i++;
    }

    ສຳລັບ (; i < page_map.length; i++)
    {
      const page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      ຖ້າ (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	ສົ່ງຄືນ true;
      }
    }
    *codepoint = INVALID;
    ສົ່ງຄືນ false;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned ຈຳນວນເຕັມ i = 0;
    unsigned ຈຳນວນເຕັມ elt = 0;
    ຖ້າ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      ຖ້າ (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        goto done;
      base += page_t::ELT_BITS;

      unsigned ຈຳນວນເຕັມ major = get_major (base);
      i = last_page_lookup;
      ຖ້າ (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ຖ້າ (i >= page_map.length)
	  goto done;
      }
      ຖ້າ (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    ສຳລັບ (; i < page_map.length; i++, elt = 0)
    {
      const page_map_t &map = page_map.arrayZ[i];
      const page_t &page = pages.arrayZ[map.index];
      ສຳລັບ (; elt < page_t::len (); elt++)
	ຖ້າ (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  ສົ່ງຄືນ true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ສົ່ງຄືນ false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    ຖ້າ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ສົ່ງຄືນ *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned ຈຳນວນເຕັມ i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    ຖ້າ (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      ຖ້າ (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	ສົ່ງຄືນ true;
      }
    }
    i--;
    ສຳລັບ (; (ຈຳນວນເຕັມ) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      ຖ້າ (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	ສົ່ງຄືນ true;
      }
    }
    *codepoint = INVALID;
    ສົ່ງຄືນ false;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *last;
    ຖ້າ (!next (&i))
    {
      *last = *first = INVALID;
      ສົ່ງຄືນ false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned ຈຳນວນເຕັມ major = get_major (*last);
    unsigned ຈຳນວນເຕັມ p_index = last_page_lookup;

    ຖ້າ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ຈຳນວນເຕັມ rem = page_remainder (*last);
    unsigned ຈຳນວນເຕັມ elt = rem / page_t::ELT_BITS;
    unsigned ຈຳນວນເຕັມ bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned ຈຳນວນເຕັມ run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    ຖ້າ (run < page_t::ELT_BITS - bit)
      ສົ່ງຄືນ true;

    elt++;

    ໃນຂະນະທີ່ (true)
    {
      ສຳລັບ (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        ຖ້າ (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        ສົ່ງຄືນ true;
      }

      p_index++;


      ຖ້າ (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        ສົ່ງຄືນ true;

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
    ຖ້າ (!previous (&i))
    {
      *last = *first = INVALID;
      ສົ່ງຄືນ false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned ຈຳນວນເຕັມ major = get_major (*first);
    unsigned ຈຳນວນເຕັມ p_index = last_page_lookup;

    ຖ້າ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ຈຳນວນເຕັມ rem = page_remainder (*first);
    ຈຳນວນເຕັມ elt = rem / page_t::ELT_BITS;
    unsigned ຈຳນວນເຕັມ bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned ຈຳນວນເຕັມ run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    ຖ້າ (run < bit + 1)
      ສົ່ງຄືນ true;

    elt--;

    ໃນຂະນະທີ່ (true)
    {
      ສຳລັບ (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        ຖ້າ (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        unsigned ຈຳນວນເຕັມ zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        ສົ່ງຄືນ true;
      }


      ຖ້າ (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        ສົ່ງຄືນ true;

      p_index--;
      const auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned ຈຳນວນເຕັມ next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned ຈຳນວນເຕັມ    size) const
  {
    // By default, start at the first bit of the first page of values.
    unsigned ຈຳນວນເຕັມ start_page = 0;
    unsigned ຈຳນວນເຕັມ start_page_value = 0;
    ຖ້າ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned ຈຳນວນເຕັມ major = get_major (codepoint);
      unsigned ຈຳນວນເຕັມ i = last_page_lookup;
      ຖ້າ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ຖ້າ (i >= page_map.length)
	  ສົ່ງຄືນ 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      ຖ້າ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ຖ້າ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned ຈຳນວນເຕັມ initial_size = size;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ຈຳນວນເຕັມ n = pages[page_map.arrayZ[i].index].write (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ສົ່ງຄືນ initial_size - size;
  }

  unsigned ຈຳນວນເຕັມ next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned ຈຳນວນເຕັມ    size) const
  {
    unsigned ຈຳນວນເຕັມ initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned ຈຳນວນເຕັມ start_page = 0;
    unsigned ຈຳນວນເຕັມ start_page_value = 0;
    ຖ້າ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned ຈຳນວນເຕັມ major = get_major (codepoint);
      unsigned ຈຳນວນເຕັມ i = last_page_lookup;
      ຖ້າ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        ຖ້າ (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ໃນຂະນະທີ່ (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          ສົ່ງຄືນ initial_size - size;
        }
      }
      start_page = i;
      ຖ້າ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ຖ້າ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ຈຳນວນເຕັມ n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ໃນຂະນະທີ່ (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    ສົ່ງຄືນ initial_size - size;
  }

  bool has_population () const { ສົ່ງຄືນ population != UINT_MAX; }
  unsigned ຈຳນວນເຕັມ get_population () const
  {
    ຖ້າ (has_population ())
      ສົ່ງຄືນ population;

    unsigned ຈຳນວນເຕັມ pop = 0;
    unsigned ຈຳນວນເຕັມ count = pages.length;
    ສຳລັບ (unsigned ຈຳນວນເຕັມ i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    ສົ່ງຄືນ pop;
  }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    ຖ້າ (has_population ())
    {
      ຖ້າ (population != 1) ສົ່ງຄືນ false;
      *codepoint = get_min ();
      ສົ່ງຄືນ true;
    }

    hb_codepoint_t singleton = INVALID;
    ສຳລັບ (const auto &map : page_map)
    {
      const auto &page = pages.arrayZ[map.index];
      ສຳລັບ (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	ຖ້າ (!bits) continue;
	ຖ້າ (singleton != INVALID || (bits & (bits - 1)))
	  ສົ່ງຄືນ false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    ຖ້າ (singleton == INVALID)
    {
      population = 0;
      ສົ່ງຄືນ false;
    }
    population = 1;
    *codepoint = singleton;
    ສົ່ງຄືນ true;
  }
  hb_codepoint_t get_min () const
  {
    unsigned count = pages.length;
    ສຳລັບ (unsigned i = 0; i < count; i++)
    {
      const auto& map = page_map.arrayZ[i];
      const auto& page = pages.arrayZ[map.index];

      ຖ້າ (!page.is_empty ())
	ສົ່ງຄືນ map.major * page_t::PAGE_BITS + page.get_min ();
    }
    ສົ່ງຄືນ INVALID;
  }
  hb_codepoint_t get_max () const
  {
    unsigned count = pages.length;
    ສຳລັບ (signed i = count - 1; i >= 0; i--)
    {
      const auto& map = page_map.arrayZ[(unsigned) i];
      const auto& page = pages.arrayZ[map.index];

      ຖ້າ (!page.is_empty ())
	ສົ່ງຄືນ map.major * page_t::PAGE_BITS + page.get_max ();
    }
    ສົ່ງຄືນ INVALID;
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
      ຖ້າ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { ສົ່ງຄືນ v; }
    bool __more__ () const { ສົ່ງຄືນ v != INVALID; }
    void __next__ () { s->next (&v); ຖ້າ (l) l--; }
    void __prev__ () { s->previous (&v); }
    unsigned __len__ () const { ສົ່ງຄືນ l; }
    iter_t end () const { ສົ່ງຄືນ iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { ສົ່ງຄືນ v != o.v; }

    rak_sita:
    const hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () const { ສົ່ງຄືນ iter_t (*this); }
  operator iter_t () const { ສົ່ງຄືນ iter (); }

  rak_sita:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ຖ້າ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ຖ້າ (cached_page.major == major)
	ສົ່ງຄືນ &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    ຖ້າ (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      ຖ້າ (!insert)
        ສົ່ງຄືນ nullptr;

      ຖ້າ (unlikely (!resize (pages.length + 1)))
	ສົ່ງຄືນ nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    ສົ່ງຄືນ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t *page_for (hb_codepoint_t g) const
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ຖ້າ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ຖ້າ (cached_page.major == major)
	ສົ່ງຄືນ &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    ຖ້າ (!page_map.bfind (key, &i))
      ສົ່ງຄືນ nullptr;

    last_page_lookup = i;
    ສົ່ງຄືນ &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned ຈຳນວນເຕັມ i)
  {
    assert (i < page_map.length);
    ສົ່ງຄືນ pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t &page_at (unsigned ຈຳນວນເຕັມ i) const
  {
    assert (i < page_map.length);
    ສົ່ງຄືນ pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned ຈຳນວນເຕັມ get_major (hb_codepoint_t g) const { ສົ່ງຄືນ g >> page_t::PAGE_BITS_LOG_2; }
  unsigned ຈຳນວນເຕັມ page_remainder (hb_codepoint_t g) const { ສົ່ງຄືນ g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned ຈຳນວນເຕັມ major) const { ສົ່ງຄືນ major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
