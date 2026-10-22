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
  hb_bit_set_t& operator= (const hb_bit_set_t& other) { set (other); trả_về *this; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*this, other); trả_về *this; }
  friend void swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    nếu (likely (!a.successful || !b.successful))
      trả_về;
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
    số_nguyên cmp (const page_map_t &o) const { trả_về cmp (o.major); }
    số_nguyên cmp (uint32_t o_major) const { trả_về (số_nguyên) o_major - (số_nguyên) major; }

    uint32_t major;
    uint32_t index;
  };

  bool successful = true; /* Allocations successful */
  mutable unsigned số_nguyên population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  void err () { nếu (successful) successful = false; } /* TODO Remove */
  bool in_error () const { trả_về !successful; }

  bool resize (unsigned số_nguyên count, bool clear = true, bool exact_size = false)
  {
    nếu (unlikely (!successful)) trả_về false;

    nếu (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = true; // Most sets are small and local

    nếu (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, false)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = false;
      trả_về false;
    }
    trả_về true;
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
    trả_về *this;
  }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    nếu (!pages.length && !population) trả_về;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  bool is_empty () const
  {
    unsigned số_nguyên count = pages.length;
    cho (unsigned số_nguyên i = 0; i < count; i++)
      nếu (!pages[i].is_empty ())
	trả_về false;
    trả_về true;
  }
  explicit operator bool () const { trả_về !is_empty (); }

  uint32_t hash () const
  {
    uint32_t h = 0;
    cho (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      nếu (unlikely (page.is_empty ())) continue;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    trả_về h;
  }

  private:
  void dirty () { population = UINT_MAX; }
  public:

  void add (hb_codepoint_t g)
  {
    nếu (unlikely (!successful)) trả_về;
    nếu (unlikely (g == INVALID)) trả_về;
    dirty ();
    page_t *page = page_for (g, true); nếu (unlikely (!page)) trả_về;
    page->add (g);
  }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  {
    nếu (unlikely (!successful) || unlikely (!bits)) trả_về;
    assert (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); nếu (unlikely (!page)) trả_về;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    nếu (unlikely (!successful)) trả_về true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    nếu (unlikely (a > b || a == INVALID || b == INVALID)) trả_về false;
    dirty ();
    unsigned số_nguyên ma = get_major (a);
    unsigned số_nguyên mb = get_major (b);
    nếu (ma == mb)
    {
      page_t *page = page_for (a, true); nếu (unlikely (!page)) trả_về false;
      page->add_range (a, b);
    }
    không_thì
    {
      page_t *page = page_for (a, true); nếu (unlikely (!page)) trả_về false;
      page->add_range (a, major_start (ma + 1) - 1);

      cho (unsigned số_nguyên m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); nếu (unlikely (!page)) trả_về false;
	page->init1 ();
      }

      page = page_for (b, true); nếu (unlikely (!page)) trả_về false;
      page->add_range (major_start (mb), b);
    }
    trả_về true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  static inline const Type& StructAtOffsetUnaligned(const void *P, unsigned số_nguyên offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    trả_về * reinterpret_cast<const Type*> ((const char *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  void set_array (bool v, const T *array, unsigned số_nguyên count, unsigned số_nguyên stride=sizeof(T))
  {
    nếu (unlikely (!successful)) trả_về;
    nếu (!count) trả_về;
    dirty ();
    hb_codepoint_t g = *array;
    trong_khi (count)
    {
      unsigned số_nguyên m = get_major (g);
      page_t *page = page_for (g, v); nếu (unlikely (v && !page)) trả_về;
      unsigned số_nguyên start = major_start (m);
      unsigned số_nguyên end = major_start (m + 1);
      do
      {
        nếu (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      trong_khi (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  void add_array (const T *array, unsigned số_nguyên count, unsigned số_nguyên stride=sizeof(T))
  { set_array (true, array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  void del_array (const T *array, unsigned số_nguyên count, unsigned số_nguyên stride=sizeof(T))
  { set_array (false, array, count, stride); }
  template <typename T>
  void del_array (const hb_array_t<const T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool set_sorted_array (bool v, const T *array, unsigned số_nguyên count, unsigned số_nguyên stride=sizeof(T))
  {
    nếu (unlikely (!successful)) trả_về true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    nếu (unlikely (!count)) trả_về true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    trong_khi (count)
    {
      unsigned số_nguyên m = get_major (g);
      page_t *page = page_for (g, v); nếu (unlikely (v && !page)) trả_về false;
      unsigned số_nguyên end = major_start (m + 1);
      do
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	nếu (g < last_g) trả_về false;
	last_g = g;

        nếu (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      trong_khi (count && (g = *array, g < end));
    }
    trả_về true;
  }

  template <typename T>
  bool add_sorted_array (const T *array, unsigned số_nguyên count, unsigned số_nguyên stride=sizeof(T))
  { trả_về set_sorted_array (true, array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { trả_về add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  bool del_sorted_array (const T *array, unsigned số_nguyên count, unsigned số_nguyên stride=sizeof(T))
  { trả_về set_sorted_array (false, array, count, stride); }
  template <typename T>
  bool del_sorted_array (const hb_sorted_array_t<const T>& arr) { trả_về del_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g)
  {
    nếu (unlikely (!successful)) trả_về;
    page_t *page = page_for (g);
    nếu (!page)
      trả_về;
    dirty ();
    page->del (g);
  }
  void del_bits (hb_codepoint_t g, uint64_t bits)
  {
    nếu (unlikely (!successful) || unlikely (!bits)) trả_về;
    assert (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    nếu (!page)
      trả_về;
    dirty ();
    page->del_bits (g, bits);
  }

  private:
  void del_pages (số_nguyên ds, số_nguyên de)
  {
    nếu (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      nếu (unlikely (!allocate_compact_workspace (compact_workspace))) trả_về;

      unsigned số_nguyên write_index = 0;
      cho (unsigned số_nguyên i = 0; i < page_map.length; i++)
      {
	số_nguyên m = (số_nguyên) page_map.arrayZ[i].major;
	nếu (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  public:
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    nếu (unlikely (!successful)) trả_về;
    nếu (unlikely (a > b || a == INVALID)) trả_về;
    dirty ();
    unsigned số_nguyên ma = get_major (a);
    unsigned số_nguyên mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    số_nguyên ds = (a == major_start (ma))? (số_nguyên) ma: (số_nguyên) (ma + 1);
    số_nguyên de = (b + 1 == major_start (mb + 1))? (số_nguyên) mb: ((số_nguyên) mb - 1);
    nếu (ds > de || (số_nguyên) ma < ds)
    {
      page_t *page = page_for (a);
      nếu (page)
      {
	nếu (ma == mb)
	  page->del_range (a, b);
	không_thì
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    nếu (de < (số_nguyên) mb && ma != mb)
    {
      page_t *page = page_for (b);
      nếu (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool get (hb_codepoint_t g) const
  {
    const page_t *page = page_for (g);
    nếu (!page)
      trả_về false;
    trả_về page->get (g);
  }
  bool may_have (hb_codepoint_t g) const { trả_về get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { trả_về get (k); }
  bool has (hb_codepoint_t k) const { trả_về (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { trả_về has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); trả_về *this; }
  hb_bit_set_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); trả_về *this; }

  bool intersects (const hb_bit_set_t &other) const
  {
    unsigned số_nguyên na = pages.length;
    unsigned số_nguyên nb = other.pages.length;

    unsigned số_nguyên a = 0, b = 0;
    cho (; a < na && b < nb; )
    {
      nếu (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	nếu (page_at (a).intersects (other.page_at (b)))
	  trả_về true;
	a++;
	b++;
      }
      không_thì nếu (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      không_thì
	b++;
    }
    trả_về false;
  }
  bool may_intersect (const hb_bit_set_t &other) const
  { trả_về intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    trả_về next (&c) && c <= last;
  }
  void set (const hb_bit_set_t &other, bool exact_size = false)
  {
    nếu (unlikely (!successful)) trả_về;
    unsigned số_nguyên count = other.pages.length;
    nếu (unlikely (!resize  (count, false, exact_size)))
      trả_về;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (const hb_bit_set_t &other) const
  {
    nếu (has_population () && other.has_population () &&
	population != other.population)
      trả_về false;

    unsigned số_nguyên na = pages.length;
    unsigned số_nguyên nb = other.pages.length;

    unsigned số_nguyên a = 0, b = 0;
    cho (; a < na && b < nb; )
    {
      nếu (page_at (a).is_empty ()) { a++; continue; }
      nếu (other.page_at (b).is_empty ()) { b++; continue; }
      nếu (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	trả_về false;
      a++;
      b++;
    }
    cho (; a < na; a++)
      nếu (!page_at (a).is_empty ()) { trả_về false; }
    cho (; b < nb; b++)
      nếu (!other.page_at (b).is_empty ()) { trả_về false; }

    trả_về true;
  }

  bool is_subset (const hb_bit_set_t &larger_set) const
  {
    nếu (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      trả_về false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    trong_khi (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      nếu (spm < lpm) {
        nếu (!sp.is_empty ())
          trả_về false;
        spi++;
        continue;
      }

      nếu (lpm < spm) {
        lpi++;
        continue;
      }

      auto lp = larger_set.page_at (lpi);
      nếu (!sp.is_subset (lp))
        trả_về false;

      spi++;
      lpi++;
    }

    trong_khi (spi < page_map.length)
      nếu (!page_at (spi++).is_empty ())
        trả_về false;

    trả_về true;
  }

  private:
  bool allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    nếu (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = false;
      trả_về false;
    }

    trả_về true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  void compact (hb_vector_t<unsigned>& workspace,
                unsigned số_nguyên length)
  {
    assert(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    cho (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  void compact_pages (const hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned số_nguyên write_index = 0;
    cho (unsigned số_nguyên i = 0; i < pages.length; i++)
    {
      nếu (old_index_to_page_map_index[i] == 0xFFFFFFFF) continue;

      nếu (write_index < i)
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
    nếu (unlikely (!successful)) trả_về;

    dirty ();

    unsigned số_nguyên na = pages.length;
    unsigned số_nguyên nb = other.pages.length;
    unsigned số_nguyên next_page = na;

    unsigned số_nguyên count = 0, newCount = 0;
    unsigned số_nguyên a = 0, b = 0;
    unsigned số_nguyên write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    nếu (!passthru_left)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      nếu (unlikely (!allocated))
      {
	successful = false;
	trả_về;
      }
    }

    cho (; a < na && b < nb; )
    {
      nếu (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	nếu (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  nếu (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      không_thì nếu (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	nếu (passthru_left)
	  count++;
	a++;
      }
      không_thì
      {
	nếu (passthru_right)
	  count++;
	b++;
      }
    }
    nếu (passthru_left)
      count += na - a;
    nếu (passthru_right)
      count += nb - b;

    nếu (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    nếu (unlikely (!resize (count)))
      trả_về;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    cho (; a && b; )
    {
      nếu (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      không_thì nếu (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	nếu (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      không_thì
      {
	b--;
	nếu (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    nếu (passthru_left)
      trong_khi (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    nếu (passthru_right)
      trong_khi (b)
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
  { trả_về Op{} (a, b); }
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
    nếu (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      trả_về *codepoint != INVALID;
    }

    const auto* page_map_array = page_map.arrayZ;
    unsigned số_nguyên major = get_major (*codepoint);
    unsigned số_nguyên i = last_page_lookup;

    nếu (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      nếu (i >= page_map.length) {
        *codepoint = INVALID;
        trả_về false;
      }
      last_page_lookup = i;
    }

    const auto* pages_array = pages.arrayZ;
    const page_map_t &current = page_map_array[i];
    nếu (likely (current.major == major))
    {
      nếu (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        trả_về true;
      }
      i++;
    }

    cho (; i < page_map.length; i++)
    {
      const page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      nếu (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	trả_về true;
      }
    }
    *codepoint = INVALID;
    trả_về false;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned số_nguyên i = 0;
    unsigned số_nguyên elt = 0;
    nếu (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      nếu (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        goto done;
      base += page_t::ELT_BITS;

      unsigned số_nguyên major = get_major (base);
      i = last_page_lookup;
      nếu (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	nếu (i >= page_map.length)
	  goto done;
      }
      nếu (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    cho (; i < page_map.length; i++, elt = 0)
    {
      const page_map_t &map = page_map.arrayZ[i];
      const page_t &page = pages.arrayZ[map.index];
      cho (; elt < page_t::len (); elt++)
	nếu (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  trả_về true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    trả_về false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    nếu (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      trả_về *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned số_nguyên i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    nếu (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      nếu (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	trả_về true;
      }
    }
    i--;
    cho (; (số_nguyên) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      nếu (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	trả_về true;
      }
    }
    *codepoint = INVALID;
    trả_về false;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *last;
    nếu (!next (&i))
    {
      *last = *first = INVALID;
      trả_về false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned số_nguyên major = get_major (*last);
    unsigned số_nguyên p_index = last_page_lookup;

    nếu (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned số_nguyên rem = page_remainder (*last);
    unsigned số_nguyên elt = rem / page_t::ELT_BITS;
    unsigned số_nguyên bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned số_nguyên run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    nếu (run < page_t::ELT_BITS - bit)
      trả_về true;

    elt++;

    trong_khi (true)
    {
      cho (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        nếu (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        trả_về true;
      }

      p_index++;


      nếu (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        trả_về true;

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
    nếu (!previous (&i))
    {
      *last = *first = INVALID;
      trả_về false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned số_nguyên major = get_major (*first);
    unsigned số_nguyên p_index = last_page_lookup;

    nếu (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned số_nguyên rem = page_remainder (*first);
    số_nguyên elt = rem / page_t::ELT_BITS;
    unsigned số_nguyên bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned số_nguyên run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    nếu (run < bit + 1)
      trả_về true;

    elt--;

    trong_khi (true)
    {
      cho (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        nếu (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        unsigned số_nguyên zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        trả_về true;
      }


      nếu (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        trả_về true;

      p_index--;
      const auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned số_nguyên next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned số_nguyên    size) const
  {
    // By default, start at the first bit of the first page of values.
    unsigned số_nguyên start_page = 0;
    unsigned số_nguyên start_page_value = 0;
    nếu (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned số_nguyên major = get_major (codepoint);
      unsigned số_nguyên i = last_page_lookup;
      nếu (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	nếu (i >= page_map.length)
	  trả_về 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      nếu (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        nếu (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned số_nguyên initial_size = size;
    cho (unsigned số_nguyên i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned số_nguyên n = pages[page_map.arrayZ[i].index].write (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    trả_về initial_size - size;
  }

  unsigned số_nguyên next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned số_nguyên    size) const
  {
    unsigned số_nguyên initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned số_nguyên start_page = 0;
    unsigned số_nguyên start_page_value = 0;
    nếu (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned số_nguyên major = get_major (codepoint);
      unsigned số_nguyên i = last_page_lookup;
      nếu (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        nếu (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          trong_khi (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          trả_về initial_size - size;
        }
      }
      start_page = i;
      nếu (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        nếu (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    cho (unsigned số_nguyên i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned số_nguyên n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    trong_khi (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    trả_về initial_size - size;
  }

  bool has_population () const { trả_về population != UINT_MAX; }
  unsigned số_nguyên get_population () const
  {
    nếu (has_population ())
      trả_về population;

    unsigned số_nguyên pop = 0;
    unsigned số_nguyên count = pages.length;
    cho (unsigned số_nguyên i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    trả_về pop;
  }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    nếu (has_population ())
    {
      nếu (population != 1) trả_về false;
      *codepoint = get_min ();
      trả_về true;
    }

    hb_codepoint_t singleton = INVALID;
    cho (const auto &map : page_map)
    {
      const auto &page = pages.arrayZ[map.index];
      cho (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	nếu (!bits) continue;
	nếu (singleton != INVALID || (bits & (bits - 1)))
	  trả_về false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    nếu (singleton == INVALID)
    {
      population = 0;
      trả_về false;
    }
    population = 1;
    *codepoint = singleton;
    trả_về true;
  }
  hb_codepoint_t get_min () const
  {
    unsigned count = pages.length;
    cho (unsigned i = 0; i < count; i++)
    {
      const auto& map = page_map.arrayZ[i];
      const auto& page = pages.arrayZ[map.index];

      nếu (!page.is_empty ())
	trả_về map.major * page_t::PAGE_BITS + page.get_min ();
    }
    trả_về INVALID;
  }
  hb_codepoint_t get_max () const
  {
    unsigned count = pages.length;
    cho (signed i = count - 1; i >= 0; i--)
    {
      const auto& map = page_map.arrayZ[(unsigned) i];
      const auto& page = pages.arrayZ[map.index];

      nếu (!page.is_empty ())
	trả_về map.major * page_t::PAGE_BITS + page.get_max ();
    }
    trả_về INVALID;
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
      nếu (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { trả_về v; }
    bool __more__ () const { trả_về v != INVALID; }
    void __next__ () { s->next (&v); nếu (l) l--; }
    void __prev__ () { s->previous (&v); }
    unsigned __len__ () const { trả_về l; }
    iter_t end () const { trả_về iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { trả_về v != o.v; }

    protected:
    const hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () const { trả_về iter_t (*this); }
  operator iter_t () const { trả_về iter (); }

  protected:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    nếu (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      nếu (cached_page.major == major)
	trả_về &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    nếu (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      nếu (!insert)
        trả_về nullptr;

      nếu (unlikely (!resize (pages.length + 1)))
	trả_về nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    trả_về &pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t *page_for (hb_codepoint_t g) const
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    nếu (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      nếu (cached_page.major == major)
	trả_về &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    nếu (!page_map.bfind (key, &i))
      trả_về nullptr;

    last_page_lookup = i;
    trả_về &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned số_nguyên i)
  {
    assert (i < page_map.length);
    trả_về pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t &page_at (unsigned số_nguyên i) const
  {
    assert (i < page_map.length);
    trả_về pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned số_nguyên get_major (hb_codepoint_t g) const { trả_về g >> page_t::PAGE_BITS_LOG_2; }
  unsigned số_nguyên page_remainder (hb_codepoint_t g) const { trả_về g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned số_nguyên major) const { trả_về major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */
