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
    ꦪꦼꦤ꧀ (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * parimaa_na (puur_naa_nka));
    ꦭꦶꦪ
      dynamic_coords.extend (coords_);
  }

  puur_naa_nka& operator [] (unsigned idx)
  {
    ꦪꦼꦤ꧀ (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ꦮꦭꦶ Crap(puur_naa_nka);

    ꦪꦼꦤ꧀ (length <= ARRAY_LENGTH (static_coords))
    {
      ꦪꦼꦤ꧀ (idx < ARRAY_LENGTH (static_coords))
      {
        ꦱꦼꦢꦼꦭ (length <= idx)
	  static_coords[length++] = 0;
	ꦮꦭꦶ static_coords[idx];
      }
      ꦭꦶꦪ
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ꦪꦼꦤ꧀ (dynamic_coords.length <= idx)
    {
      ꦪꦼꦤ꧀ (unlikely (!dynamic_coords.resize (idx + 1)))
	ꦮꦭꦶ Crap(puur_naa_nka);
      length = idx + 1;
    }
    ꦮꦭꦶ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<puur_naa_nka> get_coords ()
  { ꦮꦭꦶ length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  private:
  hb_vector_t<puur_naa_nka> dynamic_coords;
  unsigned length;
  puur_naa_nka static_coords[parimaa_na (void *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
