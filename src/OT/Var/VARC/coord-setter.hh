#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


struct coord_setter_t
{
  coord_setter_t (hb_array_t<const số_nguyên> coords_)
  {
    length = coords_.length;
    nếu (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * sizeof (số_nguyên));
    không_thì
      dynamic_coords.extend (coords_);
  }

  số_nguyên& operator [] (unsigned idx)
  {
    nếu (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      trả_về Crap(số_nguyên);

    nếu (length <= ARRAY_LENGTH (static_coords))
    {
      nếu (idx < ARRAY_LENGTH (static_coords))
      {
        trong_khi (length <= idx)
	  static_coords[length++] = 0;
	trả_về static_coords[idx];
      }
      không_thì
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    nếu (dynamic_coords.length <= idx)
    {
      nếu (unlikely (!dynamic_coords.resize (idx + 1)))
	trả_về Crap(số_nguyên);
      length = idx + 1;
    }
    trả_về dynamic_coords.arrayZ[idx];
  }

  hb_array_t<số_nguyên> get_coords ()
  { trả_về length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<số_nguyên> dynamic_coords;
  unsigned length;
  số_nguyên static_coords[sizeof (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
