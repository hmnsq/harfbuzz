#अन्तर्भाव "hb-ot-shaper-use-table.hh"

पूर्णांक main (पूर्णांक argc, अक्षर **argv)
{
  जर (argc != 2)
  {
    साठी (अचिन्हित u = 0; u < 0x10FFFFu; u++)
      म_लिखो ("U+%04X %d\n", u, hb_use_get_category (u));
    परत 0;
  }

  hb_codepoint_t u;
  माला_पूछो (argv[1], "%x", &u);

  म_लिखो ("%d\n", hb_use_get_category (u));

  परत 0;
}
