#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


sa_nracanaa coord_setter_t
{
  coord_setter_t (hb_array_t<const ကိန်းပြည့်> coords_)
  {
    length = coords_.length;
    အကယ်၍ (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * parimaa_na (ကိန်းပြည့်));
    မဟုတ်ပါက
      dynamic_coords.extend (coords_);
  }

  ကိန်းပြည့်& operator [] (unsigned idx)
  {
    အကယ်၍ (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ပြန်ပေး Crap(ကိန်းပြည့်);

    အကယ်၍ (length <= ARRAY_LENGTH (static_coords))
    {
      အကယ်၍ (idx < ARRAY_LENGTH (static_coords))
      {
        နေစဉ် (length <= idx)
	  static_coords[length++] = 0;
	ပြန်ပေး static_coords[idx];
      }
      မဟုတ်ပါက
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    အကယ်၍ (dynamic_coords.length <= idx)
    {
      အကယ်၍ (unlikely (!dynamic_coords.resize (idx + 1)))
	ပြန်ပေး Crap(ကိန်းပြည့်);
      length = idx + 1;
    }
    ပြန်ပေး dynamic_coords.arrayZ[idx];
  }

  hb_array_t<ကိန်းပြည့်> get_coords ()
  { ပြန်ပေး length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<ကိန်းပြည့်> dynamic_coords;
  unsigned length;
  ကိန်းပြည့် static_coords[parimaa_na (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
