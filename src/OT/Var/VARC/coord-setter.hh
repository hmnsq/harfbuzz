#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


struct coord_setter_t
{
  coord_setter_t (hb_array_t<const ціле> coords_)
  {
    length = coords_.length;
    якщо (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * sizeof (ціле));
    інакше
      dynamic_coords.extend (coords_);
  }

  ціле& operator [] (unsigned idx)
  {
    якщо (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      повернути Crap(ціле);

    якщо (length <= ARRAY_LENGTH (static_coords))
    {
      якщо (idx < ARRAY_LENGTH (static_coords))
      {
        поки (length <= idx)
	  static_coords[length++] = 0;
	повернути static_coords[idx];
      }
      інакше
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    якщо (dynamic_coords.length <= idx)
    {
      якщо (unlikely (!dynamic_coords.resize (idx + 1)))
	повернути Crap(ціле);
      length = idx + 1;
    }
    повернути dynamic_coords.arrayZ[idx];
  }

  hb_array_t<ціле> get_coords ()
  { повернути length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<ціле> dynamic_coords;
  unsigned length;
  ціле static_coords[sizeof (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
