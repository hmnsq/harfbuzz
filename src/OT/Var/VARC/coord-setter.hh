#ifndef OT_VAR_VARC_COORD_SETTER_HH
#define OT_VAR_VARC_COORD_SETTER_HH


#ଅନ୍ତର୍ଭାଵ "../../../hb.hh"


ନାମାକାଶ OT {
//namespace Var {


ସଂରଚନା coord_setter_t
{
  coord_setter_t (hb_array_t<ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା> coords_)
  {
    length = coords_.length;
    ଯଦି (length <= ARRAY_LENGTH (static_coords))
      hb_memcpy (static_coords, coords_.arrayZ, length * ଆକାର (ପୂର୍ଣ୍ଣସଂଖ୍ୟା));
    ଅନ୍ୟଥା
      dynamic_coords.extend (coords_);
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା& operator [] (unsigned idx)
  {
    ଯଦି (unlikely (idx >= HB_VAR_COMPOSITE_MAX_AXES))
      ଫେରନ୍ତୁ Crap(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);

    ଯଦି (length <= ARRAY_LENGTH (static_coords))
    {
      ଯଦି (idx < ARRAY_LENGTH (static_coords))
      {
        ଯେତେବେଳେ (length <= idx)
	  static_coords[length++] = 0;
	ଫେରନ୍ତୁ static_coords[idx];
      }
      ଅନ୍ୟଥା
        dynamic_coords.extend (hb_array (static_coords, length));
    }

    ଯଦି (dynamic_coords.length <= idx)
    {
      ଯଦି (unlikely (!dynamic_coords.resize (idx + 1)))
	ଫେରନ୍ତୁ Crap(ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
      length = idx + 1;
    }
    ଫେରନ୍ତୁ dynamic_coords.arrayZ[idx];
  }

  hb_array_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> get_coords ()
  { ଫେରନ୍ତୁ length <= ARRAY_LENGTH (static_coords) ? hb_array (static_coords, length) : dynamic_coords.as_array (); }

  ବ୍ୟକ୍ତିଗତ:
  hb_vector_t<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> dynamic_coords;
  unsigned length;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା static_coords[ଆକାର (ଶୂନ୍ୟ *) * 8];
};


//} // namespace Var

} // namespace OT

#endif /* OT_VAR_VARC_COORD_SETTER_HH */
