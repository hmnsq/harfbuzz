#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


struct coord_setter_t
{
  coord_setter_t (hb_array_t<const целое> coords_)
  {
    length = coords_.length;
    если (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * sizeof (целое));
    иначе
      dynamic_coords.extend (coords_);
  }

  целое& operator [] (unsigned idx)
  {
    если (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      вернуть Crap(целое);

    если (length <= ARRAY_LENGTH (static_coords))
    {
      если (idx < ARRAY_LENGTH (static_coords))
      {
        пока (length <= idx)
	  static_coords[length++] = 0;
	вернуть static_coords[idx];
      }
      иначе
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    если (dynamic_coords.length <= idx)
    {
      если (unlikely (!dynamic_coords.resize (idx + 1)))
	вернуть Crap(целое);
      length = idx + 1;
    }
    вернуть dynamic_coords.arrayZ[idx];
  }

  hb_array_t<целое> get_coords ()
  { вернуть length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<целое> dynamic_coords;
  unsigned length;
  целое static_coords[sizeof (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
