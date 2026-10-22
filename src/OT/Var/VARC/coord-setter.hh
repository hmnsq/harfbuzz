#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


sa_nracanaa coord_setter_t
{
  coord_setter_t (hb_array_t<const පූර්ණ_සංඛ්‍යා> coords_)
  {
    length = coords_.length;
    නම් (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * parimaa_na (පූර්ණ_සංඛ්‍යා));
    නැතහොත්
      dynamic_coords.extend (coords_);
  }

  පූර්ණ_සංඛ්‍යා& operator [] (unsigned idx)
  {
    නම් (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ආපසු Crap(පූර්ණ_සංඛ්‍යා);

    නම් (length <= ARRAY_LENGTH (static_coords))
    {
      නම් (idx < ARRAY_LENGTH (static_coords))
      {
        අතරතුර (length <= idx)
	  static_coords[length++] = 0;
	ආපසු static_coords[idx];
      }
      නැතහොත්
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    නම් (dynamic_coords.length <= idx)
    {
      නම් (unlikely (!dynamic_coords.resize (idx + 1)))
	ආපසු Crap(පූර්ණ_සංඛ්‍යා);
      length = idx + 1;
    }
    ආපසු dynamic_coords.arrayZ[idx];
  }

  hb_array_t<පූර්ණ_සංඛ්‍යා> get_coords ()
  { ආපසු length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<පූර්ණ_සංඛ්‍යා> dynamic_coords;
  unsigned length;
  පූර්ණ_සංඛ්‍යා static_coords[parimaa_na (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
