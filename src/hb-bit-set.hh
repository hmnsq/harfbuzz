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
  hb_bit_set_t& operator= (const hb_bit_set_t& other) { set (other); คืน *this; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*this, other); คืน *this; }
  friend void swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    ถ้า (likely (!a.successful || !b.successful))
      คืน;
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
    จำนวนเต็ม cmp (const page_map_t &o) const { คืน cmp (o.major); }
    จำนวนเต็ม cmp (uint32_t o_major) const { คืน (จำนวนเต็ม) o_major - (จำนวนเต็ม) major; }

    uint32_t major;
    uint32_t index;
  };

  bool successful = true; /* Allocations successful */
  mutable unsigned จำนวนเต็ม population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  void err () { ถ้า (successful) successful = false; } /* TODO Remove */
  bool in_error () const { คืน !successful; }

  bool resize (unsigned จำนวนเต็ม count, bool clear = true, bool exact_size = false)
  {
    ถ้า (unlikely (!successful)) คืน false;

    ถ้า (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = true; // Most sets are small and local

    ถ้า (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, false)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = false;
      คืน false;
    }
    คืน true;
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
    คืน *this;
  }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ถ้า (!pages.length && !population) คืน;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  bool is_empty () const
  {
    unsigned จำนวนเต็ม count = pages.length;
    สำหรับ (unsigned จำนวนเต็ม i = 0; i < count; i++)
      ถ้า (!pages[i].is_empty ())
	คืน false;
    คืน true;
  }
  explicit operator bool () const { คืน !is_empty (); }

  uint32_t hash () const
  {
    uint32_t h = 0;
    สำหรับ (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      ถ้า (unlikely (page.is_empty ())) continue;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    คืน h;
  }

  private:
  void dirty () { population = UINT_MAX; }
  public:

  void add (hb_codepoint_t g)
  {
    ถ้า (unlikely (!successful)) คืน;
    ถ้า (unlikely (g == INVALID)) คืน;
    dirty ();
    page_t *page = page_for (g, true); ถ้า (unlikely (!page)) คืน;
    page->add (g);
  }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  {
    ถ้า (unlikely (!successful) || unlikely (!bits)) คืน;
    assert (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); ถ้า (unlikely (!page)) คืน;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ถ้า (unlikely (!successful)) คืน true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ถ้า (unlikely (a > b || a == INVALID || b == INVALID)) คืน false;
    dirty ();
    unsigned จำนวนเต็ม ma = get_major (a);
    unsigned จำนวนเต็ม mb = get_major (b);
    ถ้า (ma == mb)
    {
      page_t *page = page_for (a, true); ถ้า (unlikely (!page)) คืน false;
      page->add_range (a, b);
    }
    มิฉะนั้น
    {
      page_t *page = page_for (a, true); ถ้า (unlikely (!page)) คืน false;
      page->add_range (a, major_start (ma + 1) - 1);

      สำหรับ (unsigned จำนวนเต็ม m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); ถ้า (unlikely (!page)) คืน false;
	page->init1 ();
      }

      page = page_for (b, true); ถ้า (unlikely (!page)) คืน false;
      page->add_range (major_start (mb), b);
    }
    คืน true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  static anta_hstha const Type& StructAtOffsetUnaligned(const void *P, unsigned จำนวนเต็ม offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    คืน * reinterpret_cast<const Type*> ((const var_na *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  void set_array (bool v, const T *array, unsigned จำนวนเต็ม count, unsigned จำนวนเต็ม stride=parimaa_na(T))
  {
    ถ้า (unlikely (!successful)) คืน;
    ถ้า (!count) คืน;
    dirty ();
    hb_codepoint_t g = *array;
    ขณะ (count)
    {
      unsigned จำนวนเต็ม m = get_major (g);
      page_t *page = page_for (g, v); ถ้า (unlikely (v && !page)) คืน;
      unsigned จำนวนเต็ม start = major_start (m);
      unsigned จำนวนเต็ม end = major_start (m + 1);
      do
      {
        ถ้า (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ขณะ (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  void add_array (const T *array, unsigned จำนวนเต็ม count, unsigned จำนวนเต็ม stride=parimaa_na(T))
  { set_array (true, array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  void del_array (const T *array, unsigned จำนวนเต็ม count, unsigned จำนวนเต็ม stride=parimaa_na(T))
  { set_array (false, array, count, stride); }
  template <typename T>
  void del_array (const hb_array_t<const T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool set_sorted_array (bool v, const T *array, unsigned จำนวนเต็ม count, unsigned จำนวนเต็ม stride=parimaa_na(T))
  {
    ถ้า (unlikely (!successful)) คืน true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ถ้า (unlikely (!count)) คืน true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ขณะ (count)
    {
      unsigned จำนวนเต็ม m = get_major (g);
      page_t *page = page_for (g, v); ถ้า (unlikely (v && !page)) คืน false;
      unsigned จำนวนเต็ม end = major_start (m + 1);
      do
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ถ้า (g < last_g) คืน false;
	last_g = g;

        ถ้า (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ขณะ (count && (g = *array, g < end));
    }
    คืน true;
  }

  template <typename T>
  bool add_sorted_array (const T *array, unsigned จำนวนเต็ม count, unsigned จำนวนเต็ม stride=parimaa_na(T))
  { คืน set_sorted_array (true, array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { คืน add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  bool del_sorted_array (const T *array, unsigned จำนวนเต็ม count, unsigned จำนวนเต็ม stride=parimaa_na(T))
  { คืน set_sorted_array (false, array, count, stride); }
  template <typename T>
  bool del_sorted_array (const hb_sorted_array_t<const T>& arr) { คืน del_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g)
  {
    ถ้า (unlikely (!successful)) คืน;
    page_t *page = page_for (g);
    ถ้า (!page)
      คืน;
    dirty ();
    page->del (g);
  }
  void del_bits (hb_codepoint_t g, uint64_t bits)
  {
    ถ้า (unlikely (!successful) || unlikely (!bits)) คืน;
    assert (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    ถ้า (!page)
      คืน;
    dirty ();
    page->del_bits (g, bits);
  }

  private:
  void del_pages (จำนวนเต็ม ds, จำนวนเต็ม de)
  {
    ถ้า (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      ถ้า (unlikely (!allocate_compact_workspace (compact_workspace))) คืน;

      unsigned จำนวนเต็ม write_index = 0;
      สำหรับ (unsigned จำนวนเต็ม i = 0; i < page_map.length; i++)
      {
	จำนวนเต็ม m = (จำนวนเต็ม) page_map.arrayZ[i].major;
	ถ้า (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  public:
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ถ้า (unlikely (!successful)) คืน;
    ถ้า (unlikely (a > b || a == INVALID)) คืน;
    dirty ();
    unsigned จำนวนเต็ม ma = get_major (a);
    unsigned จำนวนเต็ม mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    จำนวนเต็ม ds = (a == major_start (ma))? (จำนวนเต็ม) ma: (จำนวนเต็ม) (ma + 1);
    จำนวนเต็ม de = (b + 1 == major_start (mb + 1))? (จำนวนเต็ม) mb: ((จำนวนเต็ม) mb - 1);
    ถ้า (ds > de || (จำนวนเต็ม) ma < ds)
    {
      page_t *page = page_for (a);
      ถ้า (page)
      {
	ถ้า (ma == mb)
	  page->del_range (a, b);
	มิฉะนั้น
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    ถ้า (de < (จำนวนเต็ม) mb && ma != mb)
    {
      page_t *page = page_for (b);
      ถ้า (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool get (hb_codepoint_t g) const
  {
    const page_t *page = page_for (g);
    ถ้า (!page)
      คืน false;
    คืน page->get (g);
  }
  bool may_have (hb_codepoint_t g) const { คืน get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { คืน get (k); }
  bool has (hb_codepoint_t k) const { คืน (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { คืน has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); คืน *this; }
  hb_bit_set_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); คืน *this; }

  bool intersects (const hb_bit_set_t &other) const
  {
    unsigned จำนวนเต็ม na = pages.length;
    unsigned จำนวนเต็ม nb = other.pages.length;

    unsigned จำนวนเต็ม a = 0, b = 0;
    สำหรับ (; a < na && b < nb; )
    {
      ถ้า (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ถ้า (page_at (a).intersects (other.page_at (b)))
	  คืน true;
	a++;
	b++;
      }
      มิฉะนั้น ถ้า (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      มิฉะนั้น
	b++;
    }
    คืน false;
  }
  bool may_intersect (const hb_bit_set_t &other) const
  { คืน intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    คืน next (&c) && c <= last;
  }
  void set (const hb_bit_set_t &other, bool exact_size = false)
  {
    ถ้า (unlikely (!successful)) คืน;
    unsigned จำนวนเต็ม count = other.pages.length;
    ถ้า (unlikely (!resize  (count, false, exact_size)))
      คืน;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (const hb_bit_set_t &other) const
  {
    ถ้า (has_population () && other.has_population () &&
	population != other.population)
      คืน false;

    unsigned จำนวนเต็ม na = pages.length;
    unsigned จำนวนเต็ม nb = other.pages.length;

    unsigned จำนวนเต็ม a = 0, b = 0;
    สำหรับ (; a < na && b < nb; )
    {
      ถ้า (page_at (a).is_empty ()) { a++; continue; }
      ถ้า (other.page_at (b).is_empty ()) { b++; continue; }
      ถ้า (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	คืน false;
      a++;
      b++;
    }
    สำหรับ (; a < na; a++)
      ถ้า (!page_at (a).is_empty ()) { คืน false; }
    สำหรับ (; b < nb; b++)
      ถ้า (!other.page_at (b).is_empty ()) { คืน false; }

    คืน true;
  }

  bool is_subset (const hb_bit_set_t &larger_set) const
  {
    ถ้า (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      คืน false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ขณะ (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      ถ้า (spm < lpm) {
        ถ้า (!sp.is_empty ())
          คืน false;
        spi++;
        continue;
      }

      ถ้า (lpm < spm) {
        lpi++;
        continue;
      }

      auto lp = larger_set.page_at (lpi);
      ถ้า (!sp.is_subset (lp))
        คืน false;

      spi++;
      lpi++;
    }

    ขณะ (spi < page_map.length)
      ถ้า (!page_at (spi++).is_empty ())
        คืน false;

    คืน true;
  }

  private:
  bool allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    ถ้า (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = false;
      คืน false;
    }

    คืน true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  void compact (hb_vector_t<unsigned>& workspace,
                unsigned จำนวนเต็ม length)
  {
    assert(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    สำหรับ (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  void compact_pages (const hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned จำนวนเต็ม write_index = 0;
    สำหรับ (unsigned จำนวนเต็ม i = 0; i < pages.length; i++)
    {
      ถ้า (old_index_to_page_map_index[i] == 0xFFFFFFFF) continue;

      ถ้า (write_index < i)
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
    ถ้า (unlikely (!successful)) คืน;

    dirty ();

    unsigned จำนวนเต็ม na = pages.length;
    unsigned จำนวนเต็ม nb = other.pages.length;
    unsigned จำนวนเต็ม next_page = na;

    unsigned จำนวนเต็ม count = 0, newCount = 0;
    unsigned จำนวนเต็ม a = 0, b = 0;
    unsigned จำนวนเต็ม write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    ถ้า (!passthru_left)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      ถ้า (unlikely (!allocated))
      {
	successful = false;
	คืน;
      }
    }

    สำหรับ (; a < na && b < nb; )
    {
      ถ้า (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ถ้า (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ถ้า (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      มิฉะนั้น ถ้า (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	ถ้า (passthru_left)
	  count++;
	a++;
      }
      มิฉะนั้น
      {
	ถ้า (passthru_right)
	  count++;
	b++;
      }
    }
    ถ้า (passthru_left)
      count += na - a;
    ถ้า (passthru_right)
      count += nb - b;

    ถ้า (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    ถ้า (unlikely (!resize (count)))
      คืน;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    สำหรับ (; a && b; )
    {
      ถ้า (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      มิฉะนั้น ถ้า (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	ถ้า (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      มิฉะนั้น
      {
	b--;
	ถ้า (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    ถ้า (passthru_left)
      ขณะ (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    ถ้า (passthru_right)
      ขณะ (b)
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
  { คืน Op{} (a, b); }
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
    ถ้า (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      คืน *codepoint != INVALID;
    }

    const auto* page_map_array = page_map.arrayZ;
    unsigned จำนวนเต็ม major = get_major (*codepoint);
    unsigned จำนวนเต็ม i = last_page_lookup;

    ถ้า (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      ถ้า (i >= page_map.length) {
        *codepoint = INVALID;
        คืน false;
      }
      last_page_lookup = i;
    }

    const auto* pages_array = pages.arrayZ;
    const page_map_t &current = page_map_array[i];
    ถ้า (likely (current.major == major))
    {
      ถ้า (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        คืน true;
      }
      i++;
    }

    สำหรับ (; i < page_map.length; i++)
    {
      const page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      ถ้า (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	คืน true;
      }
    }
    *codepoint = INVALID;
    คืน false;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned จำนวนเต็ม i = 0;
    unsigned จำนวนเต็ม elt = 0;
    ถ้า (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      ถ้า (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        goto done;
      base += page_t::ELT_BITS;

      unsigned จำนวนเต็ม major = get_major (base);
      i = last_page_lookup;
      ถ้า (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ถ้า (i >= page_map.length)
	  goto done;
      }
      ถ้า (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    สำหรับ (; i < page_map.length; i++, elt = 0)
    {
      const page_map_t &map = page_map.arrayZ[i];
      const page_t &page = pages.arrayZ[map.index];
      สำหรับ (; elt < page_t::len (); elt++)
	ถ้า (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  คืน true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    คืน false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    ถ้า (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      คืน *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned จำนวนเต็ม i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    ถ้า (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      ถ้า (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	คืน true;
      }
    }
    i--;
    สำหรับ (; (จำนวนเต็ม) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      ถ้า (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	คืน true;
      }
    }
    *codepoint = INVALID;
    คืน false;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *last;
    ถ้า (!next (&i))
    {
      *last = *first = INVALID;
      คืน false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned จำนวนเต็ม major = get_major (*last);
    unsigned จำนวนเต็ม p_index = last_page_lookup;

    ถ้า (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned จำนวนเต็ม rem = page_remainder (*last);
    unsigned จำนวนเต็ม elt = rem / page_t::ELT_BITS;
    unsigned จำนวนเต็ม bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned จำนวนเต็ม run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    ถ้า (run < page_t::ELT_BITS - bit)
      คืน true;

    elt++;

    ขณะ (true)
    {
      สำหรับ (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        ถ้า (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        คืน true;
      }

      p_index++;


      ถ้า (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        คืน true;

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
    ถ้า (!previous (&i))
    {
      *last = *first = INVALID;
      คืน false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned จำนวนเต็ม major = get_major (*first);
    unsigned จำนวนเต็ม p_index = last_page_lookup;

    ถ้า (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned จำนวนเต็ม rem = page_remainder (*first);
    จำนวนเต็ม elt = rem / page_t::ELT_BITS;
    unsigned จำนวนเต็ม bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned จำนวนเต็ม run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    ถ้า (run < bit + 1)
      คืน true;

    elt--;

    ขณะ (true)
    {
      สำหรับ (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        ถ้า (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        unsigned จำนวนเต็ม zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        คืน true;
      }


      ถ้า (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        คืน true;

      p_index--;
      const auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned จำนวนเต็ม next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned จำนวนเต็ม    size) const
  {
    // By default, start at the first bit of the first page of values.
    unsigned จำนวนเต็ม start_page = 0;
    unsigned จำนวนเต็ม start_page_value = 0;
    ถ้า (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned จำนวนเต็ม major = get_major (codepoint);
      unsigned จำนวนเต็ม i = last_page_lookup;
      ถ้า (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ถ้า (i >= page_map.length)
	  คืน 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      ถ้า (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ถ้า (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned จำนวนเต็ม initial_size = size;
    สำหรับ (unsigned จำนวนเต็ม i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned จำนวนเต็ม n = pages[page_map.arrayZ[i].index].write (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    คืน initial_size - size;
  }

  unsigned จำนวนเต็ม next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned จำนวนเต็ม    size) const
  {
    unsigned จำนวนเต็ม initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned จำนวนเต็ม start_page = 0;
    unsigned จำนวนเต็ม start_page_value = 0;
    ถ้า (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned จำนวนเต็ม major = get_major (codepoint);
      unsigned จำนวนเต็ม i = last_page_lookup;
      ถ้า (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        ถ้า (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ขณะ (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          คืน initial_size - size;
        }
      }
      start_page = i;
      ถ้า (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ถ้า (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    สำหรับ (unsigned จำนวนเต็ม i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned จำนวนเต็ม n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ขณะ (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    คืน initial_size - size;
  }

  bool has_population () const { คืน population != UINT_MAX; }
  unsigned จำนวนเต็ม get_population () const
  {
    ถ้า (has_population ())
      คืน population;

    unsigned จำนวนเต็ม pop = 0;
    unsigned จำนวนเต็ม count = pages.length;
    สำหรับ (unsigned จำนวนเต็ม i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    คืน pop;
  }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    ถ้า (has_population ())
    {
      ถ้า (population != 1) คืน false;
      *codepoint = get_min ();
      คืน true;
    }

    hb_codepoint_t singleton = INVALID;
    สำหรับ (const auto &map : page_map)
    {
      const auto &page = pages.arrayZ[map.index];
      สำหรับ (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	ถ้า (!bits) continue;
	ถ้า (singleton != INVALID || (bits & (bits - 1)))
	  คืน false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    ถ้า (singleton == INVALID)
    {
      population = 0;
      คืน false;
    }
    population = 1;
    *codepoint = singleton;
    คืน true;
  }
  hb_codepoint_t get_min () const
  {
    unsigned count = pages.length;
    สำหรับ (unsigned i = 0; i < count; i++)
    {
      const auto& map = page_map.arrayZ[i];
      const auto& page = pages.arrayZ[map.index];

      ถ้า (!page.is_empty ())
	คืน map.major * page_t::PAGE_BITS + page.get_min ();
    }
    คืน INVALID;
  }
  hb_codepoint_t get_max () const
  {
    unsigned count = pages.length;
    สำหรับ (signed i = count - 1; i >= 0; i--)
    {
      const auto& map = page_map.arrayZ[(unsigned) i];
      const auto& page = pages.arrayZ[map.index];

      ถ้า (!page.is_empty ())
	คืน map.major * page_t::PAGE_BITS + page.get_max ();
    }
    คืน INVALID;
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
      ถ้า (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { คืน v; }
    bool __more__ () const { คืน v != INVALID; }
    void __next__ () { s->next (&v); ถ้า (l) l--; }
    void __prev__ () { s->previous (&v); }
    unsigned __len__ () const { คืน l; }
    iter_t end () const { คืน iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { คืน v != o.v; }

    rak_sita:
    const hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () const { คืน iter_t (*this); }
  operator iter_t () const { คืน iter (); }

  rak_sita:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ถ้า (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ถ้า (cached_page.major == major)
	คืน &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    ถ้า (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      ถ้า (!insert)
        คืน nullptr;

      ถ้า (unlikely (!resize (pages.length + 1)))
	คืน nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    คืน &pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t *page_for (hb_codepoint_t g) const
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ถ้า (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ถ้า (cached_page.major == major)
	คืน &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    ถ้า (!page_map.bfind (key, &i))
      คืน nullptr;

    last_page_lookup = i;
    คืน &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned จำนวนเต็ม i)
  {
    assert (i < page_map.length);
    คืน pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t &page_at (unsigned จำนวนเต็ม i) const
  {
    assert (i < page_map.length);
    คืน pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned จำนวนเต็ม get_major (hb_codepoint_t g) const { คืน g >> page_t::PAGE_BITS_LOG_2; }
  unsigned จำนวนเต็ม page_remainder (hb_codepoint_t g) const { คืน g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned จำนวนเต็ม major) const { คืน major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
