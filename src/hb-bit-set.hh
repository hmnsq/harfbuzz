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

struct hb_bit_set_t
{
  hb_bit_set_t () = default;
  ~hb_bit_set_t () = default;

  hb_bit_set_t (const hb_bit_set_t& other) : hb_bit_set_t () { set (other, true); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*this, other); }
  hb_bit_set_t& operator= (const hb_bit_set_t& other) { set (other); 戻る *this; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*this, other); 戻る *this; }
  friend void swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    もし (likely (!a.successful || !b.successful))
      戻る;
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
  struct page_map_t
  {
    整数 cmp (const page_map_t &o) const { 戻る cmp (o.major); }
    整数 cmp (uint32_t o_major) const { 戻る (整数) o_major - (整数) major; }

    uint32_t major;
    uint32_t index;
  };

  bool successful = true; /* Allocations successful */
  mutable unsigned 整数 population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  void err () { もし (successful) successful = false; } /* TODO Remove */
  bool in_error () const { 戻る !successful; }

  bool resize (unsigned 整数 count, bool clear = true, bool exact_size = false)
  {
    もし (unlikely (!successful)) 戻る false;

    もし (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = true; // Most sets are small and local

    もし (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, false)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = false;
      戻る false;
    }
    戻る true;
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
    戻る *this;
  }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    もし (!pages.length && !population) 戻る;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  bool is_empty () const
  {
    unsigned 整数 count = pages.length;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (!pages[i].is_empty ())
	戻る false;
    戻る true;
  }
  explicit operator bool () const { 戻る !is_empty (); }

  uint32_t hash () const
  {
    uint32_t h = 0;
    ために (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      もし (unlikely (page.is_empty ())) continue;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    戻る h;
  }

  private:
  void dirty () { population = UINT_MAX; }
  public:

  void add (hb_codepoint_t g)
  {
    もし (unlikely (!successful)) 戻る;
    もし (unlikely (g == INVALID)) 戻る;
    dirty ();
    page_t *page = page_for (g, true); もし (unlikely (!page)) 戻る;
    page->add (g);
  }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  {
    もし (unlikely (!successful) || unlikely (!bits)) 戻る;
    assert (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); もし (unlikely (!page)) 戻る;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    もし (unlikely (!successful)) 戻る true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    もし (unlikely (a > b || a == INVALID || b == INVALID)) 戻る false;
    dirty ();
    unsigned 整数 ma = get_major (a);
    unsigned 整数 mb = get_major (b);
    もし (ma == mb)
    {
      page_t *page = page_for (a, true); もし (unlikely (!page)) 戻る false;
      page->add_range (a, b);
    }
    否则
    {
      page_t *page = page_for (a, true); もし (unlikely (!page)) 戻る false;
      page->add_range (a, major_start (ma + 1) - 1);

      ために (unsigned 整数 m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); もし (unlikely (!page)) 戻る false;
	page->init1 ();
      }

      page = page_for (b, true); もし (unlikely (!page)) 戻る false;
      page->add_range (major_start (mb), b);
    }
    戻る true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  static inline const Type& StructAtOffsetUnaligned(const void *P, unsigned 整数 offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    戻る * reinterpret_cast<const Type*> ((const char *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  void set_array (bool v, const T *array, unsigned 整数 count, unsigned 整数 stride=sizeof(T))
  {
    もし (unlikely (!successful)) 戻る;
    もし (!count) 戻る;
    dirty ();
    hb_codepoint_t g = *array;
    ながら (count)
    {
      unsigned 整数 m = get_major (g);
      page_t *page = page_for (g, v); もし (unlikely (v && !page)) 戻る;
      unsigned 整数 start = major_start (m);
      unsigned 整数 end = major_start (m + 1);
      do
      {
        もし (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ながら (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  void add_array (const T *array, unsigned 整数 count, unsigned 整数 stride=sizeof(T))
  { set_array (true, array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  void del_array (const T *array, unsigned 整数 count, unsigned 整数 stride=sizeof(T))
  { set_array (false, array, count, stride); }
  template <typename T>
  void del_array (const hb_array_t<const T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool set_sorted_array (bool v, const T *array, unsigned 整数 count, unsigned 整数 stride=sizeof(T))
  {
    もし (unlikely (!successful)) 戻る true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    もし (unlikely (!count)) 戻る true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    ながら (count)
    {
      unsigned 整数 m = get_major (g);
      page_t *page = page_for (g, v); もし (unlikely (v && !page)) 戻る false;
      unsigned 整数 end = major_start (m + 1);
      do
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	もし (g < last_g) 戻る false;
	last_g = g;

        もし (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      ながら (count && (g = *array, g < end));
    }
    戻る true;
  }

  template <typename T>
  bool add_sorted_array (const T *array, unsigned 整数 count, unsigned 整数 stride=sizeof(T))
  { 戻る set_sorted_array (true, array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { 戻る add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  bool del_sorted_array (const T *array, unsigned 整数 count, unsigned 整数 stride=sizeof(T))
  { 戻る set_sorted_array (false, array, count, stride); }
  template <typename T>
  bool del_sorted_array (const hb_sorted_array_t<const T>& arr) { 戻る del_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g)
  {
    もし (unlikely (!successful)) 戻る;
    page_t *page = page_for (g);
    もし (!page)
      戻る;
    dirty ();
    page->del (g);
  }
  void del_bits (hb_codepoint_t g, uint64_t bits)
  {
    もし (unlikely (!successful) || unlikely (!bits)) 戻る;
    assert (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    もし (!page)
      戻る;
    dirty ();
    page->del_bits (g, bits);
  }

  private:
  void del_pages (整数 ds, 整数 de)
  {
    もし (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      もし (unlikely (!allocate_compact_workspace (compact_workspace))) 戻る;

      unsigned 整数 write_index = 0;
      ために (unsigned 整数 i = 0; i < page_map.length; i++)
      {
	整数 m = (整数) page_map.arrayZ[i].major;
	もし (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  public:
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    もし (unlikely (!successful)) 戻る;
    もし (unlikely (a > b || a == INVALID)) 戻る;
    dirty ();
    unsigned 整数 ma = get_major (a);
    unsigned 整数 mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    整数 ds = (a == major_start (ma))? (整数) ma: (整数) (ma + 1);
    整数 de = (b + 1 == major_start (mb + 1))? (整数) mb: ((整数) mb - 1);
    もし (ds > de || (整数) ma < ds)
    {
      page_t *page = page_for (a);
      もし (page)
      {
	もし (ma == mb)
	  page->del_range (a, b);
	否则
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    もし (de < (整数) mb && ma != mb)
    {
      page_t *page = page_for (b);
      もし (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool get (hb_codepoint_t g) const
  {
    const page_t *page = page_for (g);
    もし (!page)
      戻る false;
    戻る page->get (g);
  }
  bool may_have (hb_codepoint_t g) const { 戻る get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { 戻る get (k); }
  bool has (hb_codepoint_t k) const { 戻る (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { 戻る has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); 戻る *this; }
  hb_bit_set_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); 戻る *this; }

  bool intersects (const hb_bit_set_t &other) const
  {
    unsigned 整数 na = pages.length;
    unsigned 整数 nb = other.pages.length;

    unsigned 整数 a = 0, b = 0;
    ために (; a < na && b < nb; )
    {
      もし (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	もし (page_at (a).intersects (other.page_at (b)))
	  戻る true;
	a++;
	b++;
      }
      否则 もし (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      否则
	b++;
    }
    戻る false;
  }
  bool may_intersect (const hb_bit_set_t &other) const
  { 戻る intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    戻る next (&c) && c <= last;
  }
  void set (const hb_bit_set_t &other, bool exact_size = false)
  {
    もし (unlikely (!successful)) 戻る;
    unsigned 整数 count = other.pages.length;
    もし (unlikely (!resize  (count, false, exact_size)))
      戻る;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (const hb_bit_set_t &other) const
  {
    もし (has_population () && other.has_population () &&
	population != other.population)
      戻る false;

    unsigned 整数 na = pages.length;
    unsigned 整数 nb = other.pages.length;

    unsigned 整数 a = 0, b = 0;
    ために (; a < na && b < nb; )
    {
      もし (page_at (a).is_empty ()) { a++; continue; }
      もし (other.page_at (b).is_empty ()) { b++; continue; }
      もし (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	戻る false;
      a++;
      b++;
    }
    ために (; a < na; a++)
      もし (!page_at (a).is_empty ()) { 戻る false; }
    ために (; b < nb; b++)
      もし (!other.page_at (b).is_empty ()) { 戻る false; }

    戻る true;
  }

  bool is_subset (const hb_bit_set_t &larger_set) const
  {
    もし (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      戻る false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    ながら (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      もし (spm < lpm) {
        もし (!sp.is_empty ())
          戻る false;
        spi++;
        continue;
      }

      もし (lpm < spm) {
        lpi++;
        continue;
      }

      auto lp = larger_set.page_at (lpi);
      もし (!sp.is_subset (lp))
        戻る false;

      spi++;
      lpi++;
    }

    ながら (spi < page_map.length)
      もし (!page_at (spi++).is_empty ())
        戻る false;

    戻る true;
  }

  private:
  bool allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    もし (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = false;
      戻る false;
    }

    戻る true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  void compact (hb_vector_t<unsigned>& workspace,
                unsigned 整数 length)
  {
    assert(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    ために (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  void compact_pages (const hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned 整数 write_index = 0;
    ために (unsigned 整数 i = 0; i < pages.length; i++)
    {
      もし (old_index_to_page_map_index[i] == 0xFFFFFFFF) continue;

      もし (write_index < i)
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
    もし (unlikely (!successful)) 戻る;

    dirty ();

    unsigned 整数 na = pages.length;
    unsigned 整数 nb = other.pages.length;
    unsigned 整数 next_page = na;

    unsigned 整数 count = 0, newCount = 0;
    unsigned 整数 a = 0, b = 0;
    unsigned 整数 write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    もし (!passthru_left)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      もし (unlikely (!allocated))
      {
	successful = false;
	戻る;
      }
    }

    ために (; a < na && b < nb; )
    {
      もし (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	もし (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  もし (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      否则 もし (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	もし (passthru_left)
	  count++;
	a++;
      }
      否则
      {
	もし (passthru_right)
	  count++;
	b++;
      }
    }
    もし (passthru_left)
      count += na - a;
    もし (passthru_right)
      count += nb - b;

    もし (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    もし (unlikely (!resize (count)))
      戻る;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    ために (; a && b; )
    {
      もし (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      否则 もし (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	もし (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      否则
      {
	b--;
	もし (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    もし (passthru_left)
      ながら (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    もし (passthru_right)
      ながら (b)
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
  { 戻る Op{} (a, b); }
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
    もし (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      戻る *codepoint != INVALID;
    }

    const auto* page_map_array = page_map.arrayZ;
    unsigned 整数 major = get_major (*codepoint);
    unsigned 整数 i = last_page_lookup;

    もし (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      もし (i >= page_map.length) {
        *codepoint = INVALID;
        戻る false;
      }
      last_page_lookup = i;
    }

    const auto* pages_array = pages.arrayZ;
    const page_map_t &current = page_map_array[i];
    もし (likely (current.major == major))
    {
      もし (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        戻る true;
      }
      i++;
    }

    ために (; i < page_map.length; i++)
    {
      const page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      もし (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	戻る true;
      }
    }
    *codepoint = INVALID;
    戻る false;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned 整数 i = 0;
    unsigned 整数 elt = 0;
    もし (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      もし (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        goto done;
      base += page_t::ELT_BITS;

      unsigned 整数 major = get_major (base);
      i = last_page_lookup;
      もし (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	もし (i >= page_map.length)
	  goto done;
      }
      もし (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    ために (; i < page_map.length; i++, elt = 0)
    {
      const page_map_t &map = page_map.arrayZ[i];
      const page_t &page = pages.arrayZ[map.index];
      ために (; elt < page_t::len (); elt++)
	もし (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  戻る true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    戻る false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    もし (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      戻る *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned 整数 i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    もし (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      もし (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	戻る true;
      }
    }
    i--;
    ために (; (整数) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      もし (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	戻る true;
      }
    }
    *codepoint = INVALID;
    戻る false;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *last;
    もし (!next (&i))
    {
      *last = *first = INVALID;
      戻る false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned 整数 major = get_major (*last);
    unsigned 整数 p_index = last_page_lookup;

    もし (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned 整数 rem = page_remainder (*last);
    unsigned 整数 elt = rem / page_t::ELT_BITS;
    unsigned 整数 bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned 整数 run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    もし (run < page_t::ELT_BITS - bit)
      戻る true;

    elt++;

    ながら (true)
    {
      ために (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        もし (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        戻る true;
      }

      p_index++;


      もし (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        戻る true;

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
    もし (!previous (&i))
    {
      *last = *first = INVALID;
      戻る false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned 整数 major = get_major (*first);
    unsigned 整数 p_index = last_page_lookup;

    もし (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned 整数 rem = page_remainder (*first);
    整数 elt = rem / page_t::ELT_BITS;
    unsigned 整数 bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned 整数 run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    もし (run < bit + 1)
      戻る true;

    elt--;

    ながら (true)
    {
      ために (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        もし (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        unsigned 整数 zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        戻る true;
      }


      もし (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        戻る true;

      p_index--;
      const auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned 整数 next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned 整数    size) const
  {
    // By default, start at the first bit of the first page of values.
    unsigned 整数 start_page = 0;
    unsigned 整数 start_page_value = 0;
    もし (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned 整数 major = get_major (codepoint);
      unsigned 整数 i = last_page_lookup;
      もし (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	もし (i >= page_map.length)
	  戻る 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      もし (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        もし (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned 整数 initial_size = size;
    ために (unsigned 整数 i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned 整数 n = pages[page_map.arrayZ[i].index].write (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    戻る initial_size - size;
  }

  unsigned 整数 next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned 整数    size) const
  {
    unsigned 整数 initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned 整数 start_page = 0;
    unsigned 整数 start_page_value = 0;
    もし (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned 整数 major = get_major (codepoint);
      unsigned 整数 i = last_page_lookup;
      もし (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        もし (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          ながら (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          戻る initial_size - size;
        }
      }
      start_page = i;
      もし (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        もし (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    ために (unsigned 整数 i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned 整数 n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    ながら (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    戻る initial_size - size;
  }

  bool has_population () const { 戻る population != UINT_MAX; }
  unsigned 整数 get_population () const
  {
    もし (has_population ())
      戻る population;

    unsigned 整数 pop = 0;
    unsigned 整数 count = pages.length;
    ために (unsigned 整数 i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    戻る pop;
  }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    もし (has_population ())
    {
      もし (population != 1) 戻る false;
      *codepoint = get_min ();
      戻る true;
    }

    hb_codepoint_t singleton = INVALID;
    ために (const auto &map : page_map)
    {
      const auto &page = pages.arrayZ[map.index];
      ために (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	もし (!bits) continue;
	もし (singleton != INVALID || (bits & (bits - 1)))
	  戻る false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    もし (singleton == INVALID)
    {
      population = 0;
      戻る false;
    }
    population = 1;
    *codepoint = singleton;
    戻る true;
  }
  hb_codepoint_t get_min () const
  {
    unsigned count = pages.length;
    ために (unsigned i = 0; i < count; i++)
    {
      const auto& map = page_map.arrayZ[i];
      const auto& page = pages.arrayZ[map.index];

      もし (!page.is_empty ())
	戻る map.major * page_t::PAGE_BITS + page.get_min ();
    }
    戻る INVALID;
  }
  hb_codepoint_t get_max () const
  {
    unsigned count = pages.length;
    ために (signed i = count - 1; i >= 0; i--)
    {
      const auto& map = page_map.arrayZ[(unsigned) i];
      const auto& page = pages.arrayZ[map.index];

      もし (!page.is_empty ())
	戻る map.major * page_t::PAGE_BITS + page.get_max ();
    }
    戻る INVALID;
  }

  static constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  struct iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    static constexpr bool has_fast_len = true;
    iter_t (const hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    bool init = true) : s (&s_), v (INVALID), l(0)
    {
      もし (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { 戻る v; }
    bool __more__ () const { 戻る v != INVALID; }
    void __next__ () { s->next (&v); もし (l) l--; }
    void __prev__ () { s->previous (&v); }
    unsigned __len__ () const { 戻る l; }
    iter_t end () const { 戻る iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { 戻る v != o.v; }

    protected:
    const hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () const { 戻る iter_t (*this); }
  operator iter_t () const { 戻る iter (); }

  protected:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    もし (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      もし (cached_page.major == major)
	戻る &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    もし (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      もし (!insert)
        戻る nullptr;

      もし (unlikely (!resize (pages.length + 1)))
	戻る nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    戻る &pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t *page_for (hb_codepoint_t g) const
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    もし (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      もし (cached_page.major == major)
	戻る &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    もし (!page_map.bfind (key, &i))
      戻る nullptr;

    last_page_lookup = i;
    戻る &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned 整数 i)
  {
    assert (i < page_map.length);
    戻る pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t &page_at (unsigned 整数 i) const
  {
    assert (i < page_map.length);
    戻る pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned 整数 get_major (hb_codepoint_t g) const { 戻る g >> page_t::PAGE_BITS_LOG_2; }
  unsigned 整数 page_remainder (hb_codepoint_t g) const { 戻る g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned 整数 major) const { 戻る major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
