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
  hb_bit_set_t& operator= (const hb_bit_set_t& other) { set (other); ပြန်ပေး *this; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*this, other); ပြန်ပေး *this; }
  friend void swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    အကယ်၍ (likely (!a.successful || !b.successful))
      ပြန်ပေး;
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
    ကိန်းပြည့် cmp (const page_map_t &o) const { ပြန်ပေး cmp (o.major); }
    ကိန်းပြည့် cmp (uint32_t o_major) const { ပြန်ပေး (ကိန်းပြည့်) o_major - (ကိန်းပြည့်) major; }

    uint32_t major;
    uint32_t index;
  };

  bool successful = true; /* Allocations successful */
  mutable unsigned ကိန်းပြည့် population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  void err () { အကယ်၍ (successful) successful = false; } /* TODO Remove */
  bool in_error () const { ပြန်ပေး !successful; }

  bool resize (unsigned ကိန်းပြည့် count, bool clear = true, bool exact_size = false)
  {
    အကယ်၍ (unlikely (!successful)) ပြန်ပေး false;

    အကယ်၍ (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = true; // Most sets are small and local

    အကယ်၍ (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, false)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = false;
      ပြန်ပေး false;
    }
    ပြန်ပေး true;
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
    ပြန်ပေး *this;
  }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    အကယ်၍ (!pages.length && !population) ပြန်ပေး;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  bool is_empty () const
  {
    unsigned ကိန်းပြည့် count = pages.length;
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      အကယ်၍ (!pages[i].is_empty ())
	ပြန်ပေး false;
    ပြန်ပေး true;
  }
  explicit operator bool () const { ပြန်ပေး !is_empty (); }

  uint32_t hash () const
  {
    uint32_t h = 0;
    အတွက် (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      အကယ်၍ (unlikely (page.is_empty ())) continue;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    ပြန်ပေး h;
  }

  private:
  void dirty () { population = UINT_MAX; }
  public:

  void add (hb_codepoint_t g)
  {
    အကယ်၍ (unlikely (!successful)) ပြန်ပေး;
    အကယ်၍ (unlikely (g == INVALID)) ပြန်ပေး;
    dirty ();
    page_t *page = page_for (g, true); အကယ်၍ (unlikely (!page)) ပြန်ပေး;
    page->add (g);
  }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  {
    အကယ်၍ (unlikely (!successful) || unlikely (!bits)) ပြန်ပေး;
    assert (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); အကယ်၍ (unlikely (!page)) ပြန်ပေး;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    အကယ်၍ (unlikely (!successful)) ပြန်ပေး true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    အကယ်၍ (unlikely (a > b || a == INVALID || b == INVALID)) ပြန်ပေး false;
    dirty ();
    unsigned ကိန်းပြည့် ma = get_major (a);
    unsigned ကိန်းပြည့် mb = get_major (b);
    အကယ်၍ (ma == mb)
    {
      page_t *page = page_for (a, true); အကယ်၍ (unlikely (!page)) ပြန်ပေး false;
      page->add_range (a, b);
    }
    မဟုတ်ပါက
    {
      page_t *page = page_for (a, true); အကယ်၍ (unlikely (!page)) ပြန်ပေး false;
      page->add_range (a, major_start (ma + 1) - 1);

      အတွက် (unsigned ကိန်းပြည့် m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); အကယ်၍ (unlikely (!page)) ပြန်ပေး false;
	page->init1 ();
      }

      page = page_for (b, true); အကယ်၍ (unlikely (!page)) ပြန်ပေး false;
      page->add_range (major_start (mb), b);
    }
    ပြန်ပေး true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  static anta_hstha const Type& StructAtOffsetUnaligned(const void *P, unsigned ကိန်းပြည့် offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    ပြန်ပေး * reinterpret_cast<const Type*> ((const var_na *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  void set_array (bool v, const T *array, unsigned ကိန်းပြည့် count, unsigned ကိန်းပြည့် stride=parimaa_na(T))
  {
    အကယ်၍ (unlikely (!successful)) ပြန်ပေး;
    အကယ်၍ (!count) ပြန်ပေး;
    dirty ();
    hb_codepoint_t g = *array;
    နေစဉ် (count)
    {
      unsigned ကိန်းပြည့် m = get_major (g);
      page_t *page = page_for (g, v); အကယ်၍ (unlikely (v && !page)) ပြန်ပေး;
      unsigned ကိန်းပြည့် start = major_start (m);
      unsigned ကိန်းပြည့် end = major_start (m + 1);
      do
      {
        အကယ်၍ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      နေစဉ် (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  void add_array (const T *array, unsigned ကိန်းပြည့် count, unsigned ကိန်းပြည့် stride=parimaa_na(T))
  { set_array (true, array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  void del_array (const T *array, unsigned ကိန်းပြည့် count, unsigned ကိန်းပြည့် stride=parimaa_na(T))
  { set_array (false, array, count, stride); }
  template <typename T>
  void del_array (const hb_array_t<const T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool set_sorted_array (bool v, const T *array, unsigned ကိန်းပြည့် count, unsigned ကိန်းပြည့် stride=parimaa_na(T))
  {
    အကယ်၍ (unlikely (!successful)) ပြန်ပေး true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    အကယ်၍ (unlikely (!count)) ပြန်ပေး true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    နေစဉ် (count)
    {
      unsigned ကိန်းပြည့် m = get_major (g);
      page_t *page = page_for (g, v); အကယ်၍ (unlikely (v && !page)) ပြန်ပေး false;
      unsigned ကိန်းပြည့် end = major_start (m + 1);
      do
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	အကယ်၍ (g < last_g) ပြန်ပေး false;
	last_g = g;

        အကယ်၍ (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      နေစဉ် (count && (g = *array, g < end));
    }
    ပြန်ပေး true;
  }

  template <typename T>
  bool add_sorted_array (const T *array, unsigned ကိန်းပြည့် count, unsigned ကိန်းပြည့် stride=parimaa_na(T))
  { ပြန်ပေး set_sorted_array (true, array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { ပြန်ပေး add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  bool del_sorted_array (const T *array, unsigned ကိန်းပြည့် count, unsigned ကိန်းပြည့် stride=parimaa_na(T))
  { ပြန်ပေး set_sorted_array (false, array, count, stride); }
  template <typename T>
  bool del_sorted_array (const hb_sorted_array_t<const T>& arr) { ပြန်ပေး del_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g)
  {
    အကယ်၍ (unlikely (!successful)) ပြန်ပေး;
    page_t *page = page_for (g);
    အကယ်၍ (!page)
      ပြန်ပေး;
    dirty ();
    page->del (g);
  }
  void del_bits (hb_codepoint_t g, uint64_t bits)
  {
    အကယ်၍ (unlikely (!successful) || unlikely (!bits)) ပြန်ပေး;
    assert (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    အကယ်၍ (!page)
      ပြန်ပေး;
    dirty ();
    page->del_bits (g, bits);
  }

  private:
  void del_pages (ကိန်းပြည့် ds, ကိန်းပြည့် de)
  {
    အကယ်၍ (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      အကယ်၍ (unlikely (!allocate_compact_workspace (compact_workspace))) ပြန်ပေး;

      unsigned ကိန်းပြည့် write_index = 0;
      အတွက် (unsigned ကိန်းပြည့် i = 0; i < page_map.length; i++)
      {
	ကိန်းပြည့် m = (ကိန်းပြည့်) page_map.arrayZ[i].major;
	အကယ်၍ (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  public:
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    အကယ်၍ (unlikely (!successful)) ပြန်ပေး;
    အကယ်၍ (unlikely (a > b || a == INVALID)) ပြန်ပေး;
    dirty ();
    unsigned ကိန်းပြည့် ma = get_major (a);
    unsigned ကိန်းပြည့် mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    ကိန်းပြည့် ds = (a == major_start (ma))? (ကိန်းပြည့်) ma: (ကိန်းပြည့်) (ma + 1);
    ကိန်းပြည့် de = (b + 1 == major_start (mb + 1))? (ကိန်းပြည့်) mb: ((ကိန်းပြည့်) mb - 1);
    အကယ်၍ (ds > de || (ကိန်းပြည့်) ma < ds)
    {
      page_t *page = page_for (a);
      အကယ်၍ (page)
      {
	အကယ်၍ (ma == mb)
	  page->del_range (a, b);
	မဟုတ်ပါက
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    အကယ်၍ (de < (ကိန်းပြည့်) mb && ma != mb)
    {
      page_t *page = page_for (b);
      အကယ်၍ (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool get (hb_codepoint_t g) const
  {
    const page_t *page = page_for (g);
    အကယ်၍ (!page)
      ပြန်ပေး false;
    ပြန်ပေး page->get (g);
  }
  bool may_have (hb_codepoint_t g) const { ပြန်ပေး get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { ပြန်ပေး get (k); }
  bool has (hb_codepoint_t k) const { ပြန်ပေး (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { ပြန်ပေး has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); ပြန်ပေး *this; }
  hb_bit_set_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); ပြန်ပေး *this; }

  bool intersects (const hb_bit_set_t &other) const
  {
    unsigned ကိန်းပြည့် na = pages.length;
    unsigned ကိန်းပြည့် nb = other.pages.length;

    unsigned ကိန်းပြည့် a = 0, b = 0;
    အတွက် (; a < na && b < nb; )
    {
      အကယ်၍ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	အကယ်၍ (page_at (a).intersects (other.page_at (b)))
	  ပြန်ပေး true;
	a++;
	b++;
      }
      မဟုတ်ပါက အကယ်၍ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      မဟုတ်ပါက
	b++;
    }
    ပြန်ပေး false;
  }
  bool may_intersect (const hb_bit_set_t &other) const
  { ပြန်ပေး intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    ပြန်ပေး next (&c) && c <= last;
  }
  void set (const hb_bit_set_t &other, bool exact_size = false)
  {
    အကယ်၍ (unlikely (!successful)) ပြန်ပေး;
    unsigned ကိန်းပြည့် count = other.pages.length;
    အကယ်၍ (unlikely (!resize  (count, false, exact_size)))
      ပြန်ပေး;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (const hb_bit_set_t &other) const
  {
    အကယ်၍ (has_population () && other.has_population () &&
	population != other.population)
      ပြန်ပေး false;

    unsigned ကိန်းပြည့် na = pages.length;
    unsigned ကိန်းပြည့် nb = other.pages.length;

    unsigned ကိန်းပြည့် a = 0, b = 0;
    အတွက် (; a < na && b < nb; )
    {
      အကယ်၍ (page_at (a).is_empty ()) { a++; continue; }
      အကယ်၍ (other.page_at (b).is_empty ()) { b++; continue; }
      အကယ်၍ (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	ပြန်ပေး false;
      a++;
      b++;
    }
    အတွက် (; a < na; a++)
      အကယ်၍ (!page_at (a).is_empty ()) { ပြန်ပေး false; }
    အတွက် (; b < nb; b++)
      အကယ်၍ (!other.page_at (b).is_empty ()) { ပြန်ပေး false; }

    ပြန်ပေး true;
  }

  bool is_subset (const hb_bit_set_t &larger_set) const
  {
    အကယ်၍ (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      ပြန်ပေး false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    နေစဉ် (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      အကယ်၍ (spm < lpm) {
        အကယ်၍ (!sp.is_empty ())
          ပြန်ပေး false;
        spi++;
        continue;
      }

      အကယ်၍ (lpm < spm) {
        lpi++;
        continue;
      }

      auto lp = larger_set.page_at (lpi);
      အကယ်၍ (!sp.is_subset (lp))
        ပြန်ပေး false;

      spi++;
      lpi++;
    }

    နေစဉ် (spi < page_map.length)
      အကယ်၍ (!page_at (spi++).is_empty ())
        ပြန်ပေး false;

    ပြန်ပေး true;
  }

  private:
  bool allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    အကယ်၍ (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = false;
      ပြန်ပေး false;
    }

    ပြန်ပေး true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  void compact (hb_vector_t<unsigned>& workspace,
                unsigned ကိန်းပြည့် length)
  {
    assert(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    အတွက် (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  void compact_pages (const hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned ကိန်းပြည့် write_index = 0;
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < pages.length; i++)
    {
      အကယ်၍ (old_index_to_page_map_index[i] == 0xFFFFFFFF) continue;

      အကယ်၍ (write_index < i)
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
    အကယ်၍ (unlikely (!successful)) ပြန်ပေး;

    dirty ();

    unsigned ကိန်းပြည့် na = pages.length;
    unsigned ကိန်းပြည့် nb = other.pages.length;
    unsigned ကိန်းပြည့် next_page = na;

    unsigned ကိန်းပြည့် count = 0, newCount = 0;
    unsigned ကိန်းပြည့် a = 0, b = 0;
    unsigned ကိန်းပြည့် write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    အကယ်၍ (!passthru_left)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      အကယ်၍ (unlikely (!allocated))
      {
	successful = false;
	ပြန်ပေး;
      }
    }

    အတွက် (; a < na && b < nb; )
    {
      အကယ်၍ (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	အကယ်၍ (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  အကယ်၍ (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      မဟုတ်ပါက အကယ်၍ (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	အကယ်၍ (passthru_left)
	  count++;
	a++;
      }
      မဟုတ်ပါက
      {
	အကယ်၍ (passthru_right)
	  count++;
	b++;
      }
    }
    အကယ်၍ (passthru_left)
      count += na - a;
    အကယ်၍ (passthru_right)
      count += nb - b;

    အကယ်၍ (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    အကယ်၍ (unlikely (!resize (count)))
      ပြန်ပေး;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    အတွက် (; a && b; )
    {
      အကယ်၍ (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      မဟုတ်ပါက အကယ်၍ (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	အကယ်၍ (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      မဟုတ်ပါက
      {
	b--;
	အကယ်၍ (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    အကယ်၍ (passthru_left)
      နေစဉ် (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    အကယ်၍ (passthru_right)
      နေစဉ် (b)
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
  { ပြန်ပေး Op{} (a, b); }
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
    အကယ်၍ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      ပြန်ပေး *codepoint != INVALID;
    }

    const auto* page_map_array = page_map.arrayZ;
    unsigned ကိန်းပြည့် major = get_major (*codepoint);
    unsigned ကိန်းပြည့် i = last_page_lookup;

    အကယ်၍ (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      အကယ်၍ (i >= page_map.length) {
        *codepoint = INVALID;
        ပြန်ပေး false;
      }
      last_page_lookup = i;
    }

    const auto* pages_array = pages.arrayZ;
    const page_map_t &current = page_map_array[i];
    အကယ်၍ (likely (current.major == major))
    {
      အကယ်၍ (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        ပြန်ပေး true;
      }
      i++;
    }

    အတွက် (; i < page_map.length; i++)
    {
      const page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      အကယ်၍ (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	ပြန်ပေး true;
      }
    }
    *codepoint = INVALID;
    ပြန်ပေး false;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned ကိန်းပြည့် i = 0;
    unsigned ကိန်းပြည့် elt = 0;
    အကယ်၍ (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      အကယ်၍ (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        goto done;
      base += page_t::ELT_BITS;

      unsigned ကိန်းပြည့် major = get_major (base);
      i = last_page_lookup;
      အကယ်၍ (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	အကယ်၍ (i >= page_map.length)
	  goto done;
      }
      အကယ်၍ (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    အတွက် (; i < page_map.length; i++, elt = 0)
    {
      const page_map_t &map = page_map.arrayZ[i];
      const page_t &page = pages.arrayZ[map.index];
      အတွက် (; elt < page_t::len (); elt++)
	အကယ်၍ (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  ပြန်ပေး true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    ပြန်ပေး false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    အကယ်၍ (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      ပြန်ပေး *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned ကိန်းပြည့် i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    အကယ်၍ (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      အကယ်၍ (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	ပြန်ပေး true;
      }
    }
    i--;
    အတွက် (; (ကိန်းပြည့်) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      အကယ်၍ (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	ပြန်ပေး true;
      }
    }
    *codepoint = INVALID;
    ပြန်ပေး false;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *last;
    အကယ်၍ (!next (&i))
    {
      *last = *first = INVALID;
      ပြန်ပေး false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned ကိန်းပြည့် major = get_major (*last);
    unsigned ကိန်းပြည့် p_index = last_page_lookup;

    အကယ်၍ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ကိန်းပြည့် rem = page_remainder (*last);
    unsigned ကိန်းပြည့် elt = rem / page_t::ELT_BITS;
    unsigned ကိန်းပြည့် bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned ကိန်းပြည့် run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    အကယ်၍ (run < page_t::ELT_BITS - bit)
      ပြန်ပေး true;

    elt++;

    နေစဉ် (true)
    {
      အတွက် (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        အကယ်၍ (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        ပြန်ပေး true;
      }

      p_index++;


      အကယ်၍ (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        ပြန်ပေး true;

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
    အကယ်၍ (!previous (&i))
    {
      *last = *first = INVALID;
      ပြန်ပေး false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned ကိန်းပြည့် major = get_major (*first);
    unsigned ကိန်းပြည့် p_index = last_page_lookup;

    အကယ်၍ (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned ကိန်းပြည့် rem = page_remainder (*first);
    ကိန်းပြည့် elt = rem / page_t::ELT_BITS;
    unsigned ကိန်းပြည့် bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned ကိန်းပြည့် run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    အကယ်၍ (run < bit + 1)
      ပြန်ပေး true;

    elt--;

    နေစဉ် (true)
    {
      အတွက် (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        အကယ်၍ (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        unsigned ကိန်းပြည့် zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        ပြန်ပေး true;
      }


      အကယ်၍ (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        ပြန်ပေး true;

      p_index--;
      const auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned ကိန်းပြည့် next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned ကိန်းပြည့်    size) const
  {
    // By default, start at the first bit of the first page of values.
    unsigned ကိန်းပြည့် start_page = 0;
    unsigned ကိန်းပြည့် start_page_value = 0;
    အကယ်၍ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned ကိန်းပြည့် major = get_major (codepoint);
      unsigned ကိန်းပြည့် i = last_page_lookup;
      အကယ်၍ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	အကယ်၍ (i >= page_map.length)
	  ပြန်ပေး 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      အကယ်၍ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        အကယ်၍ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned ကိန်းပြည့် initial_size = size;
    အတွက် (unsigned ကိန်းပြည့် i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ကိန်းပြည့် n = pages[page_map.arrayZ[i].index].write (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ပြန်ပေး initial_size - size;
  }

  unsigned ကိန်းပြည့် next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned ကိန်းပြည့်    size) const
  {
    unsigned ကိန်းပြည့် initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned ကိန်းပြည့် start_page = 0;
    unsigned ကိန်းပြည့် start_page_value = 0;
    အကယ်၍ (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned ကိန်းပြည့် major = get_major (codepoint);
      unsigned ကိန်းပြည့် i = last_page_lookup;
      အကယ်၍ (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        အကယ်၍ (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          နေစဉ် (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          ပြန်ပေး initial_size - size;
        }
      }
      start_page = i;
      အကယ်၍ (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        အကယ်၍ (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    အတွက် (unsigned ကိန်းပြည့် i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned ကိန်းပြည့် n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    နေစဉ် (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    ပြန်ပေး initial_size - size;
  }

  bool has_population () const { ပြန်ပေး population != UINT_MAX; }
  unsigned ကိန်းပြည့် get_population () const
  {
    အကယ်၍ (has_population ())
      ပြန်ပေး population;

    unsigned ကိန်းပြည့် pop = 0;
    unsigned ကိန်းပြည့် count = pages.length;
    အတွက် (unsigned ကိန်းပြည့် i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    ပြန်ပေး pop;
  }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    အကယ်၍ (has_population ())
    {
      အကယ်၍ (population != 1) ပြန်ပေး false;
      *codepoint = get_min ();
      ပြန်ပေး true;
    }

    hb_codepoint_t singleton = INVALID;
    အတွက် (const auto &map : page_map)
    {
      const auto &page = pages.arrayZ[map.index];
      အတွက် (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	အကယ်၍ (!bits) continue;
	အကယ်၍ (singleton != INVALID || (bits & (bits - 1)))
	  ပြန်ပေး false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    အကယ်၍ (singleton == INVALID)
    {
      population = 0;
      ပြန်ပေး false;
    }
    population = 1;
    *codepoint = singleton;
    ပြန်ပေး true;
  }
  hb_codepoint_t get_min () const
  {
    unsigned count = pages.length;
    အတွက် (unsigned i = 0; i < count; i++)
    {
      const auto& map = page_map.arrayZ[i];
      const auto& page = pages.arrayZ[map.index];

      အကယ်၍ (!page.is_empty ())
	ပြန်ပေး map.major * page_t::PAGE_BITS + page.get_min ();
    }
    ပြန်ပေး INVALID;
  }
  hb_codepoint_t get_max () const
  {
    unsigned count = pages.length;
    အတွက် (signed i = count - 1; i >= 0; i--)
    {
      const auto& map = page_map.arrayZ[(unsigned) i];
      const auto& page = pages.arrayZ[map.index];

      အကယ်၍ (!page.is_empty ())
	ပြန်ပေး map.major * page_t::PAGE_BITS + page.get_max ();
    }
    ပြန်ပေး INVALID;
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
      အကယ်၍ (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { ပြန်ပေး v; }
    bool __more__ () const { ပြန်ပေး v != INVALID; }
    void __next__ () { s->next (&v); အကယ်၍ (l) l--; }
    void __prev__ () { s->previous (&v); }
    unsigned __len__ () const { ပြန်ပေး l; }
    iter_t end () const { ပြန်ပေး iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { ပြန်ပေး v != o.v; }

    rak_sita:
    const hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () const { ပြန်ပေး iter_t (*this); }
  operator iter_t () const { ပြန်ပေး iter (); }

  rak_sita:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    အကယ်၍ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      အကယ်၍ (cached_page.major == major)
	ပြန်ပေး &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    အကယ်၍ (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      အကယ်၍ (!insert)
        ပြန်ပေး nullptr;

      အကယ်၍ (unlikely (!resize (pages.length + 1)))
	ပြန်ပေး nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    ပြန်ပေး &pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t *page_for (hb_codepoint_t g) const
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    အကယ်၍ (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      အကယ်၍ (cached_page.major == major)
	ပြန်ပေး &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    အကယ်၍ (!page_map.bfind (key, &i))
      ပြန်ပေး nullptr;

    last_page_lookup = i;
    ပြန်ပေး &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned ကိန်းပြည့် i)
  {
    assert (i < page_map.length);
    ပြန်ပေး pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t &page_at (unsigned ကိန်းပြည့် i) const
  {
    assert (i < page_map.length);
    ပြန်ပေး pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned ကိန်းပြည့် get_major (hb_codepoint_t g) const { ပြန်ပေး g >> page_t::PAGE_BITS_LOG_2; }
  unsigned ကိန်းပြည့် page_remainder (hb_codepoint_t g) const { ပြန်ပေး g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned ကိန်းပြည့် major) const { ပြန်ပေး major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
