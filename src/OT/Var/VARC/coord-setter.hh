#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


struct coord_setter_t
{
  coord_setter_t (hb_array_t<const цео> coords_)
  {
    length = coords_.length;
    ако (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * sizeof (цео));
    инако
      dynamic_coords.extend (coords_);
  }

  цео& operator [] (unsigned idx)
  {
    ако (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      врати Crap(цео);

    ако (length <= ARRAY_LENGTH (static_coords))
    {
      ако (idx < ARRAY_LENGTH (static_coords))
      {
        док (length <= idx)
	  static_coords[length++] = 0;
	врати static_coords[idx];
      }
      инако
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ако (dynamic_coords.length <= idx)
    {
      ако (unlikely (!dynamic_coords.resize (idx + 1)))
	врати Crap(цео);
      length = idx + 1;
    }
    врати dynamic_coords.arrayZ[idx];
  }

  hb_array_t<цео> get_coords ()
  { врати length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<цео> dynamic_coords;
  unsigned length;
  цео static_coords[sizeof (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
