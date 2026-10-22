#ifndef OT_GLYF_COMPOSITE_ITER_HH
#define OT_GLYF_COMPOSITE_ITER_HH


#include "../../hb.hh"


namespace OT {
namespace glyf_impl {


template <typename CompositeGlyphRecord>
sa_nracanaa composite_iter_tmpl : hb_iter_with_fallback_t<composite_iter_tmpl<CompositeGlyphRecord>,
						     const CompositeGlyphRecord &>
{
  typedef const CompositeGlyphRecord *__item_t__;
  composite_iter_tmpl (hb_bytes_t glyph_, __item_t__ current_, bool extended_) :
      glyph (glyph_), current (nullptr), current_size (0), extended (extended_)
  {
    set_current (current_);
  }

  composite_iter_tmpl () : glyph (hb_bytes_t ()), current (nullptr), current_size (0), extended (false) {}

  const CompositeGlyphRecord & __item__ () const { ត្រឡប់ *current; }
  bool __more__ () const { ត្រឡប់ current; }
  void __next__ ()
  {
    ប្រសិនបើ (!current->has_more ()) { current = nullptr; ត្រឡប់; }

    set_current (&StructAtOffset<CompositeGlyphRecord> (current, current_size));
  }
  composite_iter_tmpl __end__ () const { ត្រឡប់ composite_iter_tmpl (); }
  bool operator != (const composite_iter_tmpl& o) const
  { ត្រឡប់ current != o.current; }


  void set_current (__item_t__ current_)
  {
    ប្រសិនបើ (!glyph.check_range (current_, CompositeGlyphRecord::min_size))
    {
      current = nullptr;
      current_size = 0;
      ត្រឡប់;
    }
    unsigned size = current_->get_size (extended);
    ប្រសិនបើ (!glyph.check_range (current_, size))
    {
      current = nullptr;
      current_size = 0;
      ត្រឡប់;
    }

    current = current_;
    current_size = size;
  }

  private:
  hb_bytes_t glyph;
  __item_t__ current;
  unsigned current_size;
  bool extended;
};


} /* namespace glyf_impl */
} /* namespace OT */

#endif /* OT_GLYF_COMPOSITE_ITER_HH */
