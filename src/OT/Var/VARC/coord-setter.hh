#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


sa_nracanaa coord_setter_t
{
  coord_setter_t (hb_array_t<const ចំនួនគត់> coords_)
  {
    length = coords_.length;
    ប្រសិនបើ (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * parimaa_na (ចំនួនគត់));
    បើមិនដូច្នេះ
      dynamic_coords.extend (coords_);
  }

  ចំនួនគត់& operator [] (unsigned idx)
  {
    ប្រសិនបើ (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ត្រឡប់ Crap(ចំនួនគត់);

    ប្រសិនបើ (length <= ARRAY_LENGTH (static_coords))
    {
      ប្រសិនបើ (idx < ARRAY_LENGTH (static_coords))
      {
        ខណៈពេល (length <= idx)
	  static_coords[length++] = 0;
	ត្រឡប់ static_coords[idx];
      }
      បើមិនដូច្នេះ
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ប្រសិនបើ (dynamic_coords.length <= idx)
    {
      ប្រសិនបើ (unlikely (!dynamic_coords.resize (idx + 1)))
	ត្រឡប់ Crap(ចំនួនគត់);
      length = idx + 1;
    }
    ត្រឡប់ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<ចំនួនគត់> get_coords ()
  { ត្រឡប់ length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<ចំនួនគត់> dynamic_coords;
  unsigned length;
  ចំនួនគត់ static_coords[parimaa_na (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
