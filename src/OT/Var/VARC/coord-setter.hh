#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


struct coord_setter_t
{
  coord_setter_t (hb_array_t<const цяло> coords_)
  {
    length = coords_.length;
    ако (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * sizeof (цяло));
    иначе
      dynamic_coords.extend (coords_);
  }

  цяло& operator [] (unsigned idx)
  {
    ако (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      върни Crap(цяло);

    ако (length <= ARRAY_LENGTH (static_coords))
    {
      ако (idx < ARRAY_LENGTH (static_coords))
      {
        докато (length <= idx)
	  static_coords[length++] = 0;
	върни static_coords[idx];
      }
      иначе
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ако (dynamic_coords.length <= idx)
    {
      ако (unlikely (!dynamic_coords.resize (idx + 1)))
	върни Crap(цяло);
      length = idx + 1;
    }
    върни dynamic_coords.arrayZ[idx];
  }

  hb_array_t<цяло> get_coords ()
  { върни length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<цяло> dynamic_coords;
  unsigned length;
  цяло static_coords[sizeof (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
