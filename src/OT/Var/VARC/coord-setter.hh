#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#അന്തര്ഭാവ "../../../hb.hh"


നാമാകാശ OT {
//namespace Var {


ഘടന coord_setter_t
{
  coord_setter_t (hb_array_t<സ്ഥിരം പൂർണ്ണസംഖ്യ> coords_)
  {
    length = coords_.length;
    എങ്കിൽ (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * വലുപ്പം (പൂർണ്ണസംഖ്യ));
    അല്ലെങ്കിൽ
      dynamic_coords.extend (coords_);
  }

  പൂർണ്ണസംഖ്യ& operator [] (unsigned idx)
  {
    എങ്കിൽ (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      തിരികെ Crap(പൂർണ്ണസംഖ്യ);

    എങ്കിൽ (length <= ARRAY_LENGTH (static_coords))
    {
      എങ്കിൽ (idx < ARRAY_LENGTH (static_coords))
      {
        ആകുമ്പോൾ (length <= idx)
	  static_coords[length++] = 0;
	തിരികെ static_coords[idx];
      }
      അല്ലെങ്കിൽ
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    എങ്കിൽ (dynamic_coords.length <= idx)
    {
      എങ്കിൽ (unlikely (!dynamic_coords.resize (idx + 1)))
	തിരികെ Crap(പൂർണ്ണസംഖ്യ);
      length = idx + 1;
    }
    തിരികെ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<പൂർണ്ണസംഖ്യ> get_coords ()
  { തിരികെ length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  സ്വകാര്യ:
  hb_vector_t<പൂർണ്ണസംഖ്യ> dynamic_coords;
  unsigned length;
  പൂർണ്ണസംഖ്യ static_coords[വലുപ്പം (ശൂന്യം *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
