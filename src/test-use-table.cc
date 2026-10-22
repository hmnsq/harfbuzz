#समावेश "hb-ot-shaper-use-table.hh"

पूर्णांक मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अगर (argc != 2)
  {
    क्रम (अचिन्हित u = 0; u < 0x10FFFFu; u++)
      म_लिखो ("U+%04X %d\n", u, hb_use_get_category (u));
    वापस 0;
  }

  hb_codepoint_t u;
  माला_पूछो (argv[1], "%x", &u);

  म_लिखो ("%d\n", hb_use_get_category (u));

  वापस 0;
}
