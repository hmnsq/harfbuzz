#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


struct coord_setter_t
{
  coord_setter_t (hb_array_t<const 정수> coords_)
  {
    length = coords_.length;
    만약 (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * sizeof (정수));
    아니면
      dynamic_coords.extend (coords_);
  }

  정수& operator [] (unsigned idx)
  {
    만약 (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      반환 Crap(정수);

    만약 (length <= ARRAY_LENGTH (static_coords))
    {
      만약 (idx < ARRAY_LENGTH (static_coords))
      {
        동안 (length <= idx)
	  static_coords[length++] = 0;
	반환 static_coords[idx];
      }
      아니면
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    만약 (dynamic_coords.length <= idx)
    {
      만약 (unlikely (!dynamic_coords.resize (idx + 1)))
	반환 Crap(정수);
      length = idx + 1;
    }
    반환 dynamic_coords.arrayZ[idx];
  }

  hb_array_t<정수> get_coords ()
  { 반환 length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<정수> dynamic_coords;
  unsigned length;
  정수 static_coords[sizeof (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
