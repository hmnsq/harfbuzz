#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


sa_nracanaa coord_setter_t
{
  coord_setter_t (hb_array_t<const จำนวนเต็ม> coords_)
  {
    length = coords_.length;
    ถ้า (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * parimaa_na (จำนวนเต็ม));
    มิฉะนั้น
      dynamic_coords.extend (coords_);
  }

  จำนวนเต็ม& operator [] (unsigned idx)
  {
    ถ้า (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      คืน Crap(จำนวนเต็ม);

    ถ้า (length <= ARRAY_LENGTH (static_coords))
    {
      ถ้า (idx < ARRAY_LENGTH (static_coords))
      {
        ขณะ (length <= idx)
	  static_coords[length++] = 0;
	คืน static_coords[idx];
      }
      มิฉะนั้น
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ถ้า (dynamic_coords.length <= idx)
    {
      ถ้า (unlikely (!dynamic_coords.resize (idx + 1)))
	คืน Crap(จำนวนเต็ม);
      length = idx + 1;
    }
    คืน dynamic_coords.arrayZ[idx];
  }

  hb_array_t<จำนวนเต็ม> get_coords ()
  { คืน length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<จำนวนเต็ม> dynamic_coords;
  unsigned length;
  จำนวนเต็ม static_coords[parimaa_na (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
