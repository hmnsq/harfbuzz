#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


struct coord_setter_t
{
  coord_setter_t (hb_array_t<const 整数> coords_)
  {
    length = coords_.length;
    如果 (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * sizeof (整数));
    否则
      dynamic_coords.extend (coords_);
  }

  整数& operator [] (unsigned idx)
  {
    如果 (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      返回 Crap(整数);

    如果 (length <= ARRAY_LENGTH (static_coords))
    {
      如果 (idx < ARRAY_LENGTH (static_coords))
      {
        当 (length <= idx)
	  static_coords[length++] = 0;
	返回 static_coords[idx];
      }
      否则
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    如果 (dynamic_coords.length <= idx)
    {
      如果 (unlikely (!dynamic_coords.resize (idx + 1)))
	返回 Crap(整数);
      length = idx + 1;
    }
    返回 dynamic_coords.arrayZ[idx];
  }

  hb_array_t<整数> get_coords ()
  { 返回 length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<整数> dynamic_coords;
  unsigned length;
  整数 static_coords[sizeof (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
