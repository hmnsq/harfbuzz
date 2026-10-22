#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#include "../../../hb.hh"


namespace OT {
//namespace Var {


sa_nracanaa coord_setter_t
{
  coord_setter_t (hb_array_t<const puur_naa_nka> coords_)
  {
    length = coords_.length;
    ᤀᤡ᤺ (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * parimaa_na (puur_naa_nka));
    ᤀᤡ᤺ᤆᤡ
      dynamic_coords.extend (coords_);
  }

  puur_naa_nka& operator [] (unsigned idx)
  {
    ᤀᤡ᤺ (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ᤀᤡᤴ Crap(puur_naa_nka);

    ᤀᤡ᤺ (length <= ARRAY_LENGTH (static_coords))
    {
      ᤀᤡ᤺ (idx < ARRAY_LENGTH (static_coords))
      {
        ᤀᤠ᤺ (length <= idx)
	  static_coords[length++] = 0;
	ᤀᤡᤴ static_coords[idx];
      }
      ᤀᤡ᤺ᤆᤡ
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ᤀᤡ᤺ (dynamic_coords.length <= idx)
    {
      ᤀᤡ᤺ (unlikely (!dynamic_coords.resize (idx + 1)))
	ᤀᤡᤴ Crap(puur_naa_nka);
      length = idx + 1;
    }
    ᤀᤡᤴ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<puur_naa_nka> get_coords ()
  { ᤀᤡᤴ length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<puur_naa_nka> dynamic_coords;
  unsigned length;
  puur_naa_nka static_coords[parimaa_na (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
