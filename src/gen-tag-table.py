#!/usr/bin/env python3

"""Generator of the mapping from OpenType tags to BCP 47 tags and vice
versa.

It creates a ``const LangTag[]``, matching the tags from the OpenType
languages system tag list to the language subtags of the BCP 47 language
subtag registry, with some manual adjustments. The mappings are
supplemented with macrolanguages' sublanguages and retired codes'
replacements, according to BCP 47 and some manual additions where BCP 47
omits a retired code entirely.

Also generated is a function, ``hb_ot_ambiguous_tag_to_language``,
intended for use by ``hb_ot_tag_to_language``. It maps OpenType tags
back to BCP 47 tags. Ambiguous OpenType tags (those that correspond to
multiple BCP 47 tags) are listed here, except when the alphabetically
first BCP 47 tag happens to be the chosen disambiguated tag. In that
case, the fallback behavior will choose the right tag anyway.

usage: ./gen-tag-table.py [--rust] languagetags language-subtag-registry

Input files:
* https://docs.microsoft.com/en-us/typography/opentype/spec/languagetags
* https://www.iana.org/assignments/language-subtag-registry/language-subtag-registry
"""

ਆਯਾਤ collections
ਆਯਾਤ html
ਤਃ html.parser ਆਯਾਤ HTMLParser
ਆਯਾਤ itertools
ਆਯਾਤ os
ਆਯਾਤ re
ਆਯਾਤ sys
ਆਯਾਤ unicodedata

ਜੇ ਲੰਬਾਈ (sys.argv) > 1 ਅਤੇ sys.argv[1] == '--rust':
	ਹਟਾਓ sys.argv[1]
	output_rust = ਸੱਚ
ਨਹੀਂਤਾਂ:
	output_rust = ਝੂਠ

ਜੇ ਲੰਬਾਈ (sys.argv) != 3:
	sys.exit (__doc__)

ਪਰਿਭਾਸ਼ਾ expect (condition, ਸੁਨੇਹਾ=ਕੁਝ_ਨਹੀਂ):
	ਜੇ ਨਹੀਂ condition:
		ਜੇ ਸੁਨੇਹਾ ਹੈ ਕੁਝ_ਨਹੀਂ:
			ਉਠਾਓ AssertionError
		ਉਠਾਓ AssertionError (ਸੁਨੇਹਾ)

ਪਰਿਭਾਸ਼ਾ ਲਿਖ (s):
	sys.stdout.flush ()
	sys.stdout.ਬਫਰ.ਲਿਖ (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = ਸੈੱਟ (" -—–.,;:’'ʼ()[]/!=>_+")

ਪਰਿਭਾਸ਼ਾ sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ਜੇ ਨਹੀਂ isinstance (s, str):
		ਉਠਾਓ TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	ਜੇ ਨਹੀਂ s:
		ਵਾਪਸ ''
	ਲਈ ch ਵਿੱਚ s:
		cat = unicodedata.category (ch)
		ਜੇ ਨਹੀਂ (cat.startswith (('L', 'M', 'N', 'Z')) ਜਾਂ ch ਵਿੱਚ _ALLOWED_COMMENT_PUNCT):
			ਉਠਾਓ ਮੁੱਲ_Error ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	ਵਾਪਸ s

ਪਰਿਭਾਸ਼ਾ sanitize_ਸਿਰਲੇਖ (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ਜੇ ਨਹੀਂ isinstance (s, str):
		ਉਠਾਓ TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	ਜੇ ਨਹੀਂ re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		ਉਠਾਓ ਮੁੱਲ_Error ("Invalid header line %r: contains disallowed characters" % s)
	ਵਾਪਸ s

ਪਰਿਭਾਸ਼ਾ sanitize_bcp47_ਟੈਗ (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ਜੇ ਨਹੀਂ isinstance (s, str):
		ਉਠਾਓ TypeError ("Expected string, got %s" % type (s).__name__)
	ਜੇ ਨਹੀਂ re.fullmatch (r'[a-zA-Z0-9-]+', s):
		ਉਠਾਓ ਮੁੱਲ_Error ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	ਵਾਪਸ s

ਪਰਿਭਾਸ਼ਾ sanitize_ਟੈਗ (ਟੈਗ_2):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ਜੇ ਨਹੀਂ isinstance (ਟੈਗ_2, str):
		ਉਠਾਓ TypeError ("Expected string for tag, got %s" % type (ਟੈਗ_2).__name__)
	ਜੇ ਟੈਗ_2 == DEFAULT_LANGUAGE_SYSTEM:
		ਵਾਪਸ ''
	ਜੇ ਨਹੀਂ re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', ਟੈਗ_2):
		ਉਠਾਓ ਮੁੱਲ_Error ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % ਟੈਗ_2)
	ਵਾਪਸ ਟੈਗ_2

DEFAULT_LANGUAGE_SYSTEM = ''

# from https://www-01.sil.org/iso639-3/iso-639-3.tab
ISO_639_3_ਮੰਜ਼ਿਲ_1 = {
	'aar': 'aa',
	'abk': 'ab',
	'afr': 'af',
	'aka': 'ak',
	'amh': 'am',
	'ara': 'ar',
	'arg': 'an',
	'asm': 'as',
	'ava': 'av',
	'ave': 'ae',
	'aym': 'ay',
	'aze': 'az',
	'bak': 'ba',
	'bam': 'bm',
	'bel': 'be',
	'ben': 'bn',
	'bis': 'bi',
	'bod': 'bo',
	'bos': 'bs',
	'bre': 'br',
	'bul': 'bg',
	'cat': 'ca',
	'ces': 'cs',
	'cha': 'ch',
	'che': 'ce',
	'chu': 'cu',
	'chv': 'cv',
	'cor': 'kw',
	'cos': 'co',
	'cre': 'cr',
	'cym': 'cy',
	'dan': 'da',
	'deu': 'de',
	'div': 'dv',
	'dzo': 'dz',
	'ell': 'el',
	'eng': 'en',
	'epo': 'eo',
	'est': 'et',
	'eus': 'eu',
	'ewe': 'ee',
	'fao': 'fo',
	'fas': 'fa',
	'fij': 'fj',
	'fin': 'fi',
	'fra': 'fr',
	'fry': 'fy',
	'ful': 'ff',
	'gla': 'gd',
	'gle': 'ga',
	'glg': 'gl',
	'glv': 'gv',
	'grn': 'gn',
	'guj': 'gu',
	'hat': 'ht',
	'hau': 'ha',
	'hbs': 'sh',
	'heb': 'he',
	'her': 'hz',
	'hin': 'hi',
	'hmo': 'ho',
	'hrv': 'hr',
	'hun': 'hu',
	'hye': 'hy',
	'ibo': 'ig',
	'ido': 'io',
	'iii': 'ii',
	'iku': 'iu',
	'ile': 'ie',
	'ina': 'ia',
	'ind': 'id',
	'ipk': 'ik',
	'isl': 'is',
	'ita': 'it',
	'jav': 'jv',
	'jpn': 'ja',
	'kal': 'kl',
	'kan': 'kn',
	'kas': 'ks',
	'kat': 'ka',
	'kau': 'kr',
	'kaz': 'kk',
	'khm': 'km',
	'kik': 'ki',
	'kin': 'rw',
	'kir': 'ky',
	'kom': 'kv',
	'kon': 'kg',
	'kor': 'ko',
	'kua': 'kj',
	'kur': 'ku',
	'lao': 'lo',
	'lat': 'la',
	'lav': 'lv',
	'lim': 'li',
	'lin': 'ln',
	'lit': 'lt',
	'ltz': 'lb',
	'lub': 'lu',
	'lug': 'lg',
	'mah': 'mh',
	'mal': 'ml',
	'mar': 'mr',
	'mkd': 'mk',
	'mlg': 'mg',
	'mlt': 'mt',
	'mol': 'mo',
	'mon': 'mn',
	'mri': 'mi',
	'msa': 'ms',
	'mya': 'my',
	'nau': 'na',
	'nav': 'nv',
	'nbl': 'nr',
	'nde': 'nd',
	'ndo': 'ng',
	'nep': 'ne',
	'nld': 'nl',
	'nno': 'nn',
	'nob': 'nb',
	'nor': 'no',
	'nya': 'ny',
	'oci': 'oc',
	'oji': 'oj',
	'ori': 'or',
	'orm': 'om',
	'oss': 'os',
	'pan': 'pa',
	'pli': 'pi',
	'pol': 'pl',
	'por': 'pt',
	'pus': 'ps',
	'que': 'qu',
	'roh': 'rm',
	'ron': 'ro',
	'run': 'rn',
	'rus': 'ru',
	'sag': 'sg',
	'san': 'sa',
	'sin': 'si',
	'slk': 'sk',
	'slv': 'sl',
	'sme': 'se',
	'smo': 'sm',
	'sna': 'sn',
	'snd': 'sd',
	'som': 'so',
	'sot': 'st',
	'spa': 'es',
	'sqi': 'sq',
	'srd': 'sc',
	'srp': 'sr',
	'ssw': 'ss',
	'sun': 'su',
	'swa': 'sw',
	'swe': 'sv',
	'tah': 'ty',
	'tam': 'ta',
	'tat': 'tt',
	'tel': 'te',
	'tgk': 'tg',
	'tgl': 'tl',
	'tha': 'th',
	'tir': 'ti',
	'ton': 'to',
	'tsn': 'tn',
	'tso': 'ts',
	'tuk': 'tk',
	'tur': 'tr',
	'twi': 'tw',
	'uig': 'ug',
	'ukr': 'uk',
	'urd': 'ur',
	'uzb': 'uz',
	'ven': 've',
	'vie': 'vi',
	'vol': 'vo',
	'wln': 'wa',
	'wol': 'wo',
	'xho': 'xh',
	'yid': 'yi',
	'yor': 'yo',
	'zha': 'za',
	'zho': 'zh',
	'zul': 'zu',
}

ਵਰਗ Language_ਟੈਗ (object):
	"""A BCP 47 language tag.

	Attributes:
		subtags (List[str]): The list of subtags in this tag.
		grandfathered (bool): Whether this tag is grandfathered. If
			``true``, the entire lowercased tag is the ``language``
			and the other subtag fields are empty.
		language (str): The language subtag.
		script (str): The script subtag.
		region (str): The region subtag.
		variant (str): The variant subtag.

	Args:
		tag (str): A BCP 47 language tag.

	"""
	ਪਰਿਭਾਸ਼ਾ __init__ (self, ਟੈਗ_2):
		ਵੈਸ਼੍ਵਿਕ bcp_47
		self.subtags = ਟੈਗ_2.ਹੇਠਲਾ ().ਵੰਡੋ ('-')
		self.grandfathered = ਟੈਗ_2.ਹੇਠਲਾ () ਵਿੱਚ bcp_47.grandfathered
		ਜੇ self.grandfathered:
			self.language = ਟੈਗ_2.ਹੇਠਲਾ ()
			self.script = ''
			self.region = ''
			self.variant = ''
		ਨਹੀਂਤਾਂ:
			self.language = self.subtags[0]
			self.script = self.ਲੱਭੋ_ਪਹਿਲਾ (ਅਨਾਮ s: ਲੰਬਾਈ (s) == 4 ਅਤੇ s[0] > '9', self.subtags)
			self.region = self.ਲੱਭੋ_ਪਹਿਲਾ (ਅਨਾਮ s: ਲੰਬਾਈ (s) == 2 ਅਤੇ s[0] > '9' ਜਾਂ ਲੰਬਾਈ (s) == 3 ਅਤੇ s[0] <= '9', self.subtags[1:])
			self.variant = self.ਲੱਭੋ_ਪਹਿਲਾ (ਅਨਾਮ s: ਲੰਬਾਈ (s) > 4 ਜਾਂ ਲੰਬਾਈ (s) == 4 ਅਤੇ s[0] <= '9', self.subtags)

	ਪਰਿਭਾਸ਼ਾ __str__(self):
		ਵਾਪਸ '-'.join(self.subtags)

	ਪਰਿਭਾਸ਼ਾ __repr__ (self):
		ਵਾਪਸ 'LanguageTag(%r)' % str(self)

	@staticmethod
	ਪਰਿਭਾਸ਼ਾ ਲੱਭੋ_ਪਹਿਲਾ (function, sequence):
		ਕੋਸ਼ਿਸ਼:
			ਵਾਪਸ ਅਗਲਾ (iter (filter (function, sequence)))
		ਅਪਵਾਦ StopIteration:
			ਵਾਪਸ ਕੁਝ_ਨਹੀਂ

	ਪਰਿਭਾਸ਼ਾ is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		ਵਾਪਸ ਨਹੀਂ (ਲੰਬਾਈ (self.subtags) == 1
			ਜਾਂ self.grandfathered
			ਅਤੇ ਲੰਬਾਈ (self.subtags[1]) != 3
			ਅਤੇ ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	ਪਰਿਭਾਸ਼ਾ get_ਸਮੂਹ (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		ਵਾਪਸ ('und'
			ਜੇ (self.language == 'und'
				ਜਾਂ self.variant ਵਿੱਚ bcp_47.prefixes ਅਤੇ ਲੰਬਾਈ (bcp_47.prefixes[self.variant]) == 1)
			ਨਹੀਂਤਾਂ self.language[0])

ਵਰਗ OpenTypeRegistryParser (HTMLParser):
	"""A parser for the OpenType language system tag registry.

	Attributes:
		header (str): The "last updated" line of the registry.
		names (Mapping[str, str]): A map of language system tags to the
			names they are given in the registry.
		ranks (DefaultDict[str, int]): A map of language system tags to
			numbers. If a single BCP 47 tag corresponds to multiple
			OpenType tags, the tags are ordered in increasing order by
			rank. The rank is based on the number of BCP 47 tags
			associated with a tag, though it may be manually modified.
		to_bcp_47 (DefaultDict[str, AbstractSet[str]]): A map of
			OpenType language system tags to sets of BCP 47 tags.
		from_bcp_47 (DefaultDict[str, AbstractSet[str]]): ``to_bcp_47``
			inverted. Its values start as unsorted sets;
			``sort_languages`` converts them to sorted lists.
		from_bcp_47_uninherited (Optional[Dict[str, AbstractSet[str]]]):
			A copy of ``from_bcp_47``. It starts as ``None`` and is
			populated at the beginning of the first call to
			``inherit_from_macrolanguages``.

	"""
	ਪਰਿਭਾਸ਼ਾ __init__ (self):
		HTMLParser.__init__ (self)
		self.ਸਿਰਲੇਖ = ''
		self.ਨਾਂ = {}
		self.ranks = collections.defaultdict (ਪੂਰਨ)
		self.ਮੰਜ਼ਿਲ_bcp_47 = collections.defaultdict (ਸੈੱਟ)
		self.from_bcp_47 = collections.defaultdict (ਸੈੱਟ)
		self.from_bcp_47_uninherited = ਕੁਝ_ਨਹੀਂ
		# Whether the parser is in a <td> element
		self._td = ਝੂਠ
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = ਝੂਠ
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	ਪਰਿਭਾਸ਼ਾ handle_starttag (self, ਟੈਗ_2, attrs):
		ਜੇ ਟੈਗ_2 == 'a':
			ਜੇ self._current_tr ਅਤੇ ਨਹੀਂ self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = ਸੱਚ
		ਜਾਂਜੇ ਟੈਗ_2 == 'br':
			self._disengaged = ਸੱਚ
		ਜਾਂਜੇ ਟੈਗ_2 == 'meta':
			ਲਈ attr, ਮੁੱਲ_2 ਵਿੱਚ attrs:
				ਜੇ attr == 'name' ਅਤੇ ਮੁੱਲ_2 == 'updated_at':
					self.ਸਿਰਲੇਖ = self.get_starttag_ਪਾਠ ()
					ਰੋਕੋ
		ਜਾਂਜੇ ਟੈਗ_2 == 'td':
			self._td = ਸੱਚ
			self._current_tr.append ('')
		ਜਾਂਜੇ ਟੈਗ_2 == 'tr':
			self._disengaged = ਝੂਠ
			self._current_tr = []

	ਪਰਿਭਾਸ਼ਾ handle_endtag (self, ਟੈਗ_2):
		ਜੇ ਟੈਗ_2 == 'td':
			self._td = ਝੂਠ
			self._disengaged = ਝੂਠ
		ਜਾਂਜੇ ਟੈਗ_2 == 'tr' ਅਤੇ self._current_tr:
			expect (2 <= ਲੰਬਾਈ (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			ਟੈਗ_2 = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			ਜੇ ਲੰਬਾਈ (ਟੈਗ_2) > 4:
				expect (ਟੈਗ_2.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % ਟੈਗ_2)
				name += ' (deprecated)'
				ਟੈਗ_2 = ਟੈਗ_2.ਵੰਡੋ (' ')[0]
				rank = 1
			self.ਨਾਂ[ਟੈਗ_2] = re.sub (' languages$', '', name)
			ਜੇ ਨਹੀਂ self._current_tr[2]:
				ਵਾਪਸ
			iso_codes = self._current_tr[2].strip ()
			self.ਮੰਜ਼ਿਲ_bcp_47[ਟੈਗ_2].update (ISO_639_3_ਮੰਜ਼ਿਲ_1.get (ਕੋਡ, ਕੋਡ) ਲਈ ਕੋਡ ਵਿੱਚ iso_codes.replace (' ', '').ਵੰਡੋ (','))
			rank += 2 * ਲੰਬਾਈ (self.ਮੰਜ਼ਿਲ_bcp_47[ਟੈਗ_2])
			self.ranks[ਟੈਗ_2] = rank

	ਪਰਿਭਾਸ਼ਾ handle_ਅੰਕੜੇ (self, ਅੰਕੜੇ):
		ਜੇ self._td ਅਤੇ ਨਹੀਂ self._disengaged:
			self._current_tr[-1] += ਅੰਕੜੇ

	ਪਰਿਭਾਸ਼ਾ handle_charref (self, name):
		self.handle_ਅੰਕੜੇ (html.unescape ('&#%s;' % name))

	ਪਰਿਭਾਸ਼ਾ handle_entityref (self, name):
		self.handle_ਅੰਕੜੇ (html.unescape ('&%s;' % name))

	ਪਰਿਭਾਸ਼ਾ parse (self, filename):
		"""Parse the OpenType language system tag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		ਨਾਲ open (filename, encoding='utf-8') ਇਵ f:
			self.feed (f.ਪਠ ())
		expect (self.ਸਿਰਲੇਖ)
		ਲਈ ਟੈਗ_2, iso_codes ਵਿੱਚ self.ਮੰਜ਼ਿਲ_bcp_47.ਚੀਜ਼ਾਂ ():
			ਲਈ iso_ਕੋਡ ਵਿੱਚ iso_codes:
				self.from_bcp_47[iso_ਕੋਡ].ਜੋੜੋ (ਟੈਗ_2)

	ਪਰਿਭਾਸ਼ਾ ਜੋੜੋ_language (self, bcp_47_ਟੈਗ, ot_ਟੈਗ):
		"""Add a language as if it were in the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag. If the tag is more than just
				a language subtag, and if the language subtag is a
				macrolanguage, then new languages are added corresponding
				to the macrolanguages' individual languages with the
				remainder of the tag appended.
			ot_tag (str): An OpenType language system tag.
		"""
		ਵੈਸ਼੍ਵਿਕ bcp_47
		self.ਮੰਜ਼ਿਲ_bcp_47[ot_ਟੈਗ].ਜੋੜੋ (bcp_47_ਟੈਗ)
		self.from_bcp_47[bcp_47_ਟੈਗ].ਜੋੜੋ (ot_ਟੈਗ)
		ਜੇ bcp_47_ਟੈਗ.ਹੇਠਲਾ () ਨਹੀਂ ਵਿੱਚ bcp_47.grandfathered:
			ਕੋਸ਼ਿਸ਼:
				[macrolanguage, suffix] = bcp_47_ਟੈਗ.ਵੰਡੋ ('-', 1)
				ਜੇ macrolanguage ਵਿੱਚ bcp_47.macrolanguages:
					s = ਸੈੱਟ ()
					ਲਈ language ਵਿੱਚ bcp_47.macrolanguages[macrolanguage]:
						ਜੇ language.ਹੇਠਲਾ () ਨਹੀਂ ਵਿੱਚ bcp_47.grandfathered:
							s.ਜੋੜੋ ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			ਅਪਵਾਦ ਮੁੱਲ_Error:
				ਲੰਘੋ

	@staticmethod
	ਪਰਿਭਾਸ਼ਾ _remove_language (ਟੈਗ_1, dict_1, dict_2):
		ਲਈ ਟੈਗ_2_2 ਵਿੱਚ dict_1.ਕੱਢੋ (ਟੈਗ_1):
			dict_2[ਟੈਗ_2_2].remove (ਟੈਗ_1)
			ਜੇ ਨਹੀਂ dict_2[ਟੈਗ_2_2]:
				ਹਟਾਓ dict_2[ਟੈਗ_2_2]

	ਪਰਿਭਾਸ਼ਾ remove_language_ot (self, ot_ਟੈਗ):
		"""Remove an OpenType tag from the registry.

		Args:
			ot_tag (str): An OpenType tag.
		"""
		self._remove_language (ot_ਟੈਗ, self.ਮੰਜ਼ਿਲ_bcp_47, self.from_bcp_47)

	ਪਰਿਭਾਸ਼ਾ remove_language_bcp_47 (self, bcp_47_ਟੈਗ):
		"""Remove a BCP 47 tag from the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag.
		"""
		self._remove_language (bcp_47_ਟੈਗ, self.from_bcp_47, self.ਮੰਜ਼ਿਲ_bcp_47)

	ਪਰਿਭਾਸ਼ਾ inherit_from_macrolanguages (self):
		"""Copy mappings from macrolanguages to individual languages.

		If a BCP 47 tag for an individual mapping has no OpenType
		mapping but its macrolanguage does, the mapping is copied to
		the individual language. For example, als (Tosk Albanian) has no
		explicit mapping, so it inherits from sq (Albanian) the mapping
		to SQI.

		However, if an OpenType tag maps to a BCP 47 macrolanguage and
		some but not all of its individual languages, the mapping is not
		inherited from the macrolanguage to the missing individual
		languages. For example, INUK (Nunavik Inuktitut) is mapped to
		ike (Eastern Canadian Inuktitut) and iu (Inuktitut) but not to
		ikt (Inuinnaqtun, which is an individual language of iu), so
		this method does not add a mapping from ikt to INUK.

		If a BCP 47 tag for a macrolanguage has no OpenType mapping but
		some of its individual languages do, their mappings are copied
		to the macrolanguage.
		"""
		ਵੈਸ਼੍ਵਿਕ bcp_47
		ਪਹਿਲਾ_ਸਮਾਂ = self.from_bcp_47_uninherited ਹੈ ਕੁਝ_ਨਹੀਂ
		ਜੇ ਪਹਿਲਾ_ਸਮਾਂ:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		ਲਈ macrolanguage, languages ਵਿੱਚ dict (bcp_47.macrolanguages).ਚੀਜ਼ਾਂ ():
			ot_macrolanguages = {
				ot_macrolanguage ਲਈ ot_macrolanguage ਵਿੱਚ self.from_bcp_47_uninherited.get (macrolanguage, ਸੈੱਟ ())
			}
			blocked_ot_macrolanguages = ਸੈੱਟ ()
			ਜੇ 'retired code' ਨਹੀਂ ਵਿੱਚ bcp_47.scopes.get (macrolanguage, ''):
				ਲਈ ot_macrolanguage ਵਿੱਚ ot_macrolanguages:
					ਗੇੜਾ_trip_macrolanguages = {
						l ਲਈ l ਵਿੱਚ self.ਮੰਜ਼ਿਲ_bcp_47[ot_macrolanguage]
						ਜੇ 'retired code' ਨਹੀਂ ਵਿੱਚ bcp_47.scopes.get (l, '')
					}
					ਗੇੜਾ_trip_languages = {
						l ਲਈ l ਵਿੱਚ languages
						ਜੇ 'retired code' ਨਹੀਂ ਵਿੱਚ bcp_47.scopes.get (l, '')
					}
					intersection = ਗੇੜਾ_trip_macrolanguages & ਗੇੜਾ_trip_languages
					ਜੇ intersection ਅਤੇ intersection != ਗੇੜਾ_trip_languages:
						blocked_ot_macrolanguages.ਜੋੜੋ (ot_macrolanguage)
			ਜੇ ot_macrolanguages:
				ਲਈ ot_macrolanguage ਵਿੱਚ ot_macrolanguages:
					ਜੇ ot_macrolanguage ਨਹੀਂ ਵਿੱਚ blocked_ot_macrolanguages:
						ਲਈ language ਵਿੱਚ languages:
							self.ਜੋੜੋ_language (language, ot_macrolanguage)
							ਜੇ ਨਹੀਂ blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			ਜਾਂਜੇ ਪਹਿਲਾ_ਸਮਾਂ:
				ਲਈ language ਵਿੱਚ languages:
					ਜੇ language ਵਿੱਚ self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					ਨਹੀਂਤਾਂ:
						ot_macrolanguages.clear ()
					ਜੇ ਨਹੀਂ ot_macrolanguages:
						ਰੋਕੋ
				ਲਈ ot_macrolanguage ਵਿੱਚ ot_macrolanguages:
					self.ਜੋੜੋ_language (macrolanguage, ot_macrolanguage)

	ਪਰਿਭਾਸ਼ਾ ਛਾਂਟੋ_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		ਲਈ language, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ self.from_bcp_47.ਚੀਜ਼ਾਂ ():
			self.from_bcp_47[language] = sorted (ਟੈਗ_ਸੂਚੀ,
					ਕੁੰਜੀ_2=ਅਨਾਮ t: (self.ranks[t] + rank_delta (language, t), t))

ot = OpenTypeRegistryParser ()

ਵਰਗ BCP47Parser (object):
	"""A parser for the BCP 47 subtag registry.

	Attributes:
		header (str): The "File-Date" line of the registry.
		names (Mapping[str, str]): A map of subtags to the names they
			are given in the registry. Each value is a
			``'\\n'``-separated list of names.
		scopes (Mapping[str, str]): A map of language subtags to strings
			suffixed to language names, including suffixes to explain
			language scopes.
		macrolanguages (DefaultDict[str, AbstractSet[str]]): A map of
			language subtags to the sets of language subtags which
			inherit from them. See
			``OpenTypeRegistryParser.inherit_from_macrolanguages``.
		prefixes (DefaultDict[str, AbstractSet[str]]): A map of variant
			subtags to their prefixes.
		grandfathered (AbstractSet[str]): The set of grandfathered tags,
			normalized to lowercase.

	"""
	ਪਰਿਭਾਸ਼ਾ __init__ (self):
		self.ਸਿਰਲੇਖ = ''
		self.ਨਾਂ = {}
		self.scopes = {}
		self.macrolanguages = collections.defaultdict (ਸੈੱਟ)
		self.prefixes = collections.defaultdict (ਸੈੱਟ)
		self.grandfathered = ਸੈੱਟ ()

	ਪਰਿਭਾਸ਼ਾ parse (self, filename):
		"""Parse the BCP 47 subtag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		ਨਾਲ open (filename, encoding='utf-8') ਇਵ f:
			subtag_type = ਕੁਝ_ਨਹੀਂ
			subtag = ਕੁਝ_ਨਹੀਂ
			deprecated = ਝੂਠ
			has_preferred_ਮੁੱਲ = ਝੂਠ
			ਸਤਰ_ਬਫਰ = ''
			ਲਈ ਸਤਰ ਵਿੱਚ itertools.chain (f, ['']):
				ਸਤਰ = ਸਤਰ.rstrip ()
				ਜੇ ਸਤਰ.startswith (' '):
					ਸਤਰ_ਬਫਰ += ਸਤਰ[1:]
					ਜਾਰੀ
				ਸਤਰ, ਸਤਰ_ਬਫਰ = ਸਤਰ_ਬਫਰ, ਸਤਰ
				ਜੇ ਸਤਰ.startswith ('Type: '):
					subtag_type = ਸਤਰ.ਵੰਡੋ (' ')[1]
					deprecated = ਝੂਠ
					has_preferred_ਮੁੱਲ = ਝੂਠ
				ਜਾਂਜੇ ਸਤਰ.startswith ('Subtag: ') ਜਾਂ ਸਤਰ.startswith ('Tag: '):
					subtag = ਸਤਰ.ਵੰਡੋ (' ')[1]
					ਜੇ subtag_type == 'grandfathered':
						self.grandfathered.ਜੋੜੋ (subtag.ਹੇਠਲਾ ())
				ਜਾਂਜੇ ਸਤਰ.startswith ('Description: '):
					description = ਸਤਰ.ਵੰਡੋ (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					ਜੇ subtag ਵਿੱਚ self.ਨਾਂ:
						self.ਨਾਂ[subtag] += '\n' + description
					ਨਹੀਂਤਾਂ:
						self.ਨਾਂ[subtag] = description
				ਜਾਂਜੇ subtag_type == 'language' ਜਾਂ subtag_type == 'grandfathered':
					ਜੇ ਸਤਰ.startswith ('Scope: '):
						scope = ਸਤਰ.ਵੰਡੋ (' ')[1]
						ਜੇ scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						ਜਾਂਜੇ scope == 'collection':
							scope = ' [collection]'
						ਨਹੀਂਤਾਂ:
							ਜਾਰੀ
						self.scopes[subtag] = scope
					ਜਾਂਜੇ ਸਤਰ.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = ਸੱਚ
					ਜਾਂਜੇ deprecated ਅਤੇ ਸਤਰ.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						ਲਈ language ਵਿੱਚ ਸਤਰ.replace (',', '').ਵੰਡੋ (' ')[2:]:
							self.ਜੋੜੋ_macrolanguage (subtag, language)
					ਜਾਂਜੇ ਸਤਰ.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_ਮੁੱਲ = ਸੱਚ
						macrolanguage = ਸਤਰ.ਵੰਡੋ (' ')[1]
						self.ਜੋੜੋ_macrolanguage (macrolanguage, subtag)
					ਜਾਂਜੇ ਨਹੀਂ has_preferred_ਮੁੱਲ ਅਤੇ ਸਤਰ.startswith ('Macrolanguage: '):
						self.ਜੋੜੋ_macrolanguage (ਸਤਰ.ਵੰਡੋ (' ')[1], subtag)
				ਜਾਂਜੇ subtag_type == 'variant':
					ਜੇ ਸਤਰ.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					ਜਾਂਜੇ ਸਤਰ.startswith ('Prefix: '):
						self.prefixes[subtag].ਜੋੜੋ (ਸਤਰ.ਵੰਡੋ (' ')[1])
				ਜਾਂਜੇ ਸਤਰ.startswith ('File-Date: '):
					self.ਸਿਰਲੇਖ = ਸਤਰ
		expect (self.ਸਿਰਲੇਖ)

	ਪਰਿਭਾਸ਼ਾ ਜੋੜੋ_macrolanguage (self, macrolanguage, language):
		ਵੈਸ਼੍ਵਿਕ ot
		ਜੇ language ਨਹੀਂ ਵਿੱਚ ot.from_bcp_47:
			ਲਈ l ਵਿੱਚ self.macrolanguages.get (language, ਸੈੱਟ ()):
				self.ਜੋੜੋ_macrolanguage (macrolanguage, l)
		ਜੇ macrolanguage ਨਹੀਂ ਵਿੱਚ ot.from_bcp_47:
			ਲਈ ls ਵਿੱਚ list (self.macrolanguages.ਮੁੱਲਾਂ ()):
				ਜੇ macrolanguage ਵਿੱਚ ls:
					ls.ਜੋੜੋ (language)
					ਵਾਪਸ
		self.macrolanguages[macrolanguage].ਜੋੜੋ (language)

	ਪਰਿਭਾਸ਼ਾ remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		ਲਈ macrolanguage, languages ਵਿੱਚ self.macrolanguages.ਚੀਜ਼ਾਂ ():
			ਲਈ language ਵਿੱਚ languages:
				inverted[language].append (macrolanguage)
		ਲਈ language, macrolanguages ਵਿੱਚ inverted.ਚੀਜ਼ਾਂ ():
			ਜੇ ਲੰਬਾਈ (macrolanguages) > 1:
				macrolanguages.ਛਾਂਟੋ (ਕੁੰਜੀ_2=ਅਨਾਮ ml: ਲੰਬਾਈ (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.ਕੱਢੋ ()
				ਲਈ macrolanguage ਵਿੱਚ macrolanguages:
					self.ਜੋੜੋ_macrolanguage (biggest_macrolanguage, macrolanguage)

	ਪਰਿਭਾਸ਼ਾ _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		ਵਾਪਸ self.ਨਾਂ[subtag].ਵੰਡੋ ('\n')[0] + self.scopes.get (subtag, '')

	ਪਰਿਭਾਸ਼ਾ get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		ਜੇ lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		ਜੇ lt.region:
			name += '; ' + self._get_name_piece (lt.region.ਉੱਤਲਾ ())
		ਜੇ lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		ਵਾਪਸ name

bcp_47 = BCP47Parser ()

ot.parse (sys.argv[1])
bcp_47.parse (sys.argv[2])

ot.ਜੋੜੋ_language ('ary', 'MOR')

ot.ਜੋੜੋ_language ('ath', 'ATH')

ot.ਜੋੜੋ_language ('bai', 'BML')

ot.ranks['BAL'] = ot.ranks['KAR'] + 1

ot.ਜੋੜੋ_language ('ber', 'BBR')

ot.remove_language_ot ('PGR')
ot.ਜੋੜੋ_language ('el-polyton', 'PGR')

bcp_47.ਨਾਂ['flm'] = 'Falam Chin'
bcp_47.scopes['flm'] = ' (retired code)'
bcp_47.macrolanguages['flm'] = {'cfm'}

ot.ranks['FNE'] = ot.ranks['TNE'] + 1

ot.ਜੋੜੋ_language ('und-fonipa', 'IPPH')

ot.ਜੋੜੋ_language ('und-fonnapa', 'APPH')

ot.ਜੋੜੋ_language ('und-fonupa', 'UPPH')

ot.ਜੋੜੋ_language ('ga-Latg', 'IRT')

ot.ਜੋੜੋ_language ('hy-arevmda', 'HYE')

ot.remove_language_ot ('KGE')
ot.ਜੋੜੋ_language ('und-Geok', 'KGE')

ot.ਜੋੜੋ_language ('kht', 'KHN')
ot.ਨਾਂ['KHN'] = ot.ਨਾਂ['KHT'] + ' (Microsoft fonts)'
ot.ranks['KHN'] = ot.ranks['KHT'] + 1

ot.ranks['LCR'] = ot.ranks['MCR'] + 1

ot.ਨਾਂ['MAL'] = 'Malayalam Traditional'
ot.ranks['MLR'] += 1

bcp_47.ਨਾਂ['mhv'] = 'Arakanese'
bcp_47.scopes['mhv'] = ' (retired code)'

ot.ਜੋੜੋ_language ('mnw-TH', 'MONT')

ot.ਜੋੜੋ_language ('no', 'NOR')

ot.ਜੋੜੋ_language ('oc-provenc', 'PRO')

ot.remove_language_ot ('QUZ')
ot.ਜੋੜੋ_language ('qu', 'QUZ')
ot.ਜੋੜੋ_language ('qub', 'QWH')
ot.ਜੋੜੋ_language ('qud', 'QVI')
ot.ਜੋੜੋ_language ('qug', 'QVI')
ot.ਜੋੜੋ_language ('qul', 'QUH')
ot.ਜੋੜੋ_language ('qup', 'QVI')
ot.ਜੋੜੋ_language ('qur', 'QWH')
ot.ਜੋੜੋ_language ('qus', 'QUH')
ot.ਜੋੜੋ_language ('quw', 'QVI')
ot.ਜੋੜੋ_language ('qux', 'QWH')
ot.ਜੋੜੋ_language ('qva', 'QWH')
ot.ਜੋੜੋ_language ('qvh', 'QWH')
ot.ਜੋੜੋ_language ('qvj', 'QVI')
ot.ਜੋੜੋ_language ('qvl', 'QWH')
ot.ਜੋੜੋ_language ('qvm', 'QWH')
ot.ਜੋੜੋ_language ('qvn', 'QWH')
ot.ਜੋੜੋ_language ('qvo', 'QVI')
ot.ਜੋੜੋ_language ('qvp', 'QWH')
ot.ਜੋੜੋ_language ('qvw', 'QWH')
ot.ਜੋੜੋ_language ('qvz', 'QVI')
ot.ਜੋੜੋ_language ('qwa', 'QWH')
ot.ਜੋੜੋ_language ('qws', 'QWH')
ot.ਜੋੜੋ_language ('qxa', 'QWH')
ot.ਜੋੜੋ_language ('qxc', 'QWH')
ot.ਜੋੜੋ_language ('qxh', 'QWH')
ot.ਜੋੜੋ_language ('qxl', 'QVI')
ot.ਜੋੜੋ_language ('qxn', 'QWH')
ot.ਜੋੜੋ_language ('qxo', 'QWH')
ot.ਜੋੜੋ_language ('qxr', 'QVI')
ot.ਜੋੜੋ_language ('qxt', 'QWH')
ot.ਜੋੜੋ_language ('qxw', 'QWH')

bcp_47.macrolanguages['ro-MD'].ਜੋੜੋ ('mo')

ot.remove_language_ot ('SYRE')
ot.remove_language_ot ('SYRJ')
ot.remove_language_ot ('SYRN')
ot.ਜੋੜੋ_language ('und-Syre', 'SYRE')
ot.ਜੋੜੋ_language ('und-Syrj', 'SYRJ')
ot.ਜੋੜੋ_language ('und-Syrn', 'SYRN')

bcp_47.ਨਾਂ['xst'] = "Silt'e"
bcp_47.scopes['xst'] = ' (retired code)'
bcp_47.macrolanguages['xst'] = {'stv', 'wle'}

ot.ਜੋੜੋ_language ('xwo', 'TOD')

ot.remove_language_ot ('ZHH')
ot.remove_language_ot ('ZHP')
ot.remove_language_ot ('ZHT')
ot.remove_language_ot ('ZHTM')
bcp_47.macrolanguages['zh'].remove ('lzh')
bcp_47.macrolanguages['zh'].remove ('yue')
ot.ਜੋੜੋ_language ('zh-Hant-MO', 'ZHH')
ot.ਜੋੜੋ_language ('zh-Hant-MO', 'ZHTM')
ot.ਜੋੜੋ_language ('zh-Hant-HK', 'ZHH')
ot.ਜੋੜੋ_language ('zh-Hans', 'ZHS')
ot.ਜੋੜੋ_language ('zh-Hant', 'ZHT')
ot.ਜੋੜੋ_language ('zh-HK', 'ZHH')
ot.ਜੋੜੋ_language ('zh-MO', 'ZHH')
ot.ਜੋੜੋ_language ('zh-MO', 'ZHTM')
ot.ਜੋੜੋ_language ('zh-TW', 'ZHT')
ot.ਜੋੜੋ_language ('lzh', 'ZHT')
ot.ਜੋੜੋ_language ('lzh-Hans', 'ZHS')
ot.ਜੋੜੋ_language ('yue', 'ZHH')
ot.ਜੋੜੋ_language ('yue-Hans', 'ZHS')

ਪਰਿਭਾਸ਼ਾ rank_delta (bcp_47, ot):
	"""Return a delta to apply to a BCP 47 tag's rank.

	Most OpenType tags have a constant rank, but a few have ranks that
	depend on the BCP 47 tag.

	Args:
		bcp_47 (str): A BCP 47 tag.
		ot (str): An OpenType tag to.

	Returns:
		A number to add to ``ot``'s rank when sorting ``bcp_47``'s
		OpenType equivalents.
	"""
	ਜੇ bcp_47 == 'ak' ਅਤੇ ot == 'AKA':
		ਵਾਪਸ -1
	ਜੇ bcp_47 == 'tw' ਅਤੇ ot == 'TWI':
		ਵਾਪਸ -1
	ਵਾਪਸ 0

disambiguation = {
	'ALT': 'alt',
	'ARK': 'rki',
	'ATH': 'ath',
	'BHI': 'bhb',
	'BLN': 'bjt',
	'BTI': 'beb',
	'CCHN': 'cco',
	'CMR': 'swb',
	'CPP': 'crp',
	'CRR': 'crx',
	'DUJ': 'dwu',
	'ECR': 'crj',
	'HAL': 'cfm',
	'HND': 'hnd',
	'HYE': 'hyw',
	'KIS': 'kqs',
	'KUI': 'uki',
	'LRC': 'bqi',
	'NDB': 'nd',
	'NIS': 'njz',
	'PLG': 'pce',
	'PRO': 'pro',
	'QIN': 'bgr',
	'QUH': 'quh',
	'QVI': 'qvi',
	'QWH': 'qwh',
	'SIG': 'stv',
	'SRB': 'sr',
	'SXT': 'xnj',
	'ZHH': 'zh-HK',
	'ZHS': 'zh-Hans',
	'ZHT': 'zh-Hant',
	'ZHTM': 'zh-MO',
}

ot.inherit_from_macrolanguages ()
bcp_47.remove_extra_macrolanguages ()
ot.inherit_from_macrolanguages ()
ot.ਨਾਂ[DEFAULT_LANGUAGE_SYSTEM] = ''
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.ਮੁੱਲਾਂ ()) + 1
ਲਈ tricky_ot_ਟੈਗ ਵਿੱਚ filter (ਅਨਾਮ ਟੈਗ_2: re.ਮੇਲ ('[A-Z]{3}$', ਟੈਗ_2), ot.ਨਾਂ):
	possible_bcp_47_ਟੈਗ = tricky_ot_ਟੈਗ.ਹੇਠਲਾ ()
	ਜੇ possible_bcp_47_ਟੈਗ ਵਿੱਚ bcp_47.ਨਾਂ ਅਤੇ ਨਹੀਂ ot.from_bcp_47[possible_bcp_47_ਟੈਗ]:
		ot.ਜੋੜੋ_language (possible_bcp_47_ਟੈਗ, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_ਟੈਗ] = ਸੈੱਟ ()
ot.ਛਾਂਟੋ_languages ()

ਜੇ output_rust:
	ਪਰਿਭਾਸ਼ਾ hb_ਟੈਗ_rust (ਟੈਗ_2):
		ਜੇ ਟੈਗ_2 == DEFAULT_LANGUAGE_SYSTEM:
			ਵਾਪਸ 'Tag::new(&[0; 4])'
		sanitize_ਟੈਗ (ਟੈਗ_2)
		ਵਾਪਸ 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % ਟੈਗ_2)[:4])

	ਪਰਿਭਾਸ਼ਾ get_variant_ਸੈੱਟ_rust (name):
		ਵਾਪਸ ਸੈੱਟ (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				ਲਈ n ਵਿੱਚ re.ਵੰਡੋ ('[\n(),]', name) ਜੇ n)

	ਪਰਿਭਾਸ਼ਾ language_name_intersection_rust (a, b):
		ਵਾਪਸ get_variant_ਸੈੱਟ_rust (a).intersection (get_variant_ਸੈੱਟ_rust (b))

	ਪਰਿਭਾਸ਼ਾ get_matching_language_name_rust (intersection, candidates):
		ਵਾਪਸ ਅਗਲਾ (iter (c ਲਈ c ਵਿੱਚ candidates ਜੇ ਨਹੀਂ intersection.isdisjoint (get_variant_ਸੈੱਟ_rust (c))))

	ਛਾਪੋ ('// WARNING: this file was generated by ../../harfbuzz/src/gen-tag-table.py --rust')
	ਛਾਪੋ ()
	ਛਾਪੋ ('use read_fonts::types::Tag;')
	ਛਾਪੋ ()
	ਛਾਪੋ ('pub(crate) struct LangTag {')
	ਛਾਪੋ ('    pub language: [u8; 4],')
	ਛਾਪੋ ('    pub tag: Tag,')
	ਛਾਪੋ ('}')
	ਛਾਪੋ ()
	ਛਾਪੋ ('const fn lang(language: &[u8]) -> [u8; 4] {')
	ਛਾਪੋ ('    let mut bytes = [0; 4];')
	ਛਾਪੋ ('    let mut i = 0;')
	ਛਾਪੋ ()
	ਛਾਪੋ ('    while i < language.len() {')
	ਛਾਪੋ ('        bytes[i] = language[i];')
	ਛਾਪੋ ('        i += 1;')
	ਛਾਪੋ ('    }')
	ਛਾਪੋ ()
	ਛਾਪੋ ('    bytes')
	ਛਾਪੋ ('}')
	ਛਾਪੋ ()
	ਛਾਪੋ ('use super::tag::{lang_matches, strncmp, subtag_matches};')
	ਛਾਪੋ ()
	ਛਾਪੋ ('#[rustfmt::skip]')
	ਛਾਪੋ ('pub(crate) static OPEN_TYPE_LANGUAGES: &[LangTag] = &[')

	ਲਈ language, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ sorted (ot.from_bcp_47.ਚੀਜ਼ਾਂ ()):
		ਜੇ language == '' ਜਾਂ '-' ਵਿੱਚ language:
			ਜਾਰੀ
		commented_ਬਾਹਰਲਾ = ਲੰਬਾਈ (language) == 3 ਅਤੇ ਲੰਬਾਈ (ਟੈਗ_ਸੂਚੀ) == 1 ਅਤੇ language == ਟੈਗ_ਸੂਚੀ[0].ਹੇਠਲਾ ()
		ਲਈ ਟੈਗ_2 ਵਿੱਚ ਟੈਗ_ਸੂਚੀ:
			ਛਾਪੋ ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' ਜੇ commented_ਬਾਹਰਲਾ ਨਹੀਂਤਾਂ '    ', sanitize_bcp47_ਟੈਗ (language), hb_ਟੈਗ_rust (ਟੈਗ_2)), ਸਿਰਾ='')
			ਛਾਪੋ (' // ', ਸਿਰਾ='')
			bcp_47_name = bcp_47.ਨਾਂ.get (language, '')
			bcp_47_name_candidates = bcp_47_name.ਵੰਡੋ ('\n')
			ot_name = ot.ਨਾਂ[ਟੈਗ_2]
			scope = bcp_47.scopes.get (language, '')
			ਜੇ ਟੈਗ_2 == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			ਨਹੀਂਤਾਂ:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				ਜੇ ਨਹੀਂ intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				ਨਹੀਂਤਾਂ:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.ਨਾਂ[language] = name
					comment = '%s%s' % (name ਜੇ ਲੰਬਾਈ (name) > ਲੰਬਾਈ (ot_name) ਨਹੀਂਤਾਂ ot_name, scope)
			ਛਾਪੋ (sanitize_comment (comment))

	ਛਾਪੋ ('];')
	ਛਾਪੋ ()
	ਛਾਪੋ ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	ਛਾਪੋ ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	ਪਰਿਭਾਸ਼ਾ print_subtag_matches_rust (subtag, new_ਸਤਰ):
		ਜੇ subtag:
			ਜੇ new_ਸਤਰ:
				ਛਾਪੋ (' && ', ਸਿਰਾ='')
			ਛਾਪੋ ('subtag_matches(language, "-%s")' % sanitize_bcp47_ਟੈਗ (subtag), ਸਿਰਾ='')

	complex_ਟੈਗ_ਸੂਚੀ = collections.defaultdict (list)
	ਲਈ initial, ਸਮੂਹ ਵਿੱਚ itertools.groupby ((lt_ਟੈਗ_ਸੂਚੀ ਲਈ lt_ਟੈਗ_ਸੂਚੀ ਵਿੱਚ [
				(Language_ਟੈਗ (language), ਟੈਗ_ਸੂਚੀ)
				ਲਈ language, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ sorted (ot.from_bcp_47.ਚੀਜ਼ਾਂ (),
					ਕੁੰਜੀ_2=ਅਨਾਮ i: (-ਲੰਬਾਈ (i[0]), i[0]))
			] ਜੇ lt_ਟੈਗ_ਸੂਚੀ[0].is_complex ()),
			ਕੁੰਜੀ_2=ਅਨਾਮ lt_ਟੈਗ_ਸੂਚੀ: lt_ਟੈਗ_ਸੂਚੀ[0].get_ਸਮੂਹ ()):
		complex_ਟੈਗ_ਸੂਚੀ[initial] += ਸਮੂਹ

	ਲਈ initial, ਚੀਜ਼ਾਂ ਵਿੱਚ sorted (complex_ਟੈਗ_ਸੂਚੀ.ਚੀਜ਼ਾਂ ()):
		ਜੇ initial != 'und':
			ਜਾਰੀ
		ਲਈ lt, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ ਚੀਜ਼ਾਂ:
			ਜੇ ਨਹੀਂ ਟੈਗ_ਸੂਚੀ:
				ਜਾਰੀ
			ਜੇ lt.variant ਵਿੱਚ bcp_47.prefixes:
				expect (ਅਗਲਾ (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			ਛਾਪੋ ('    if ', ਸਿਰਾ='')
			print_subtag_matches_rust (lt.script, ਝੂਠ)
			print_subtag_matches_rust (lt.region, ਝੂਠ)
			print_subtag_matches_rust (lt.variant, ਝੂਠ)
			ਛਾਪੋ (' {')
			ਛਾਪੋ ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			ਜੇ ਲੰਬਾਈ (ਟੈਗ_ਸੂਚੀ) == 1:
				ਛਾਪੋ ('        tags.push(%s); // %s' % (hb_ਟੈਗ_rust (ਟੈਗ_ਸੂਚੀ[0]), sanitize_comment (ot.ਨਾਂ[ਟੈਗ_ਸੂਚੀ[0]])))
			ਨਹੀਂਤਾਂ:
				ਛਾਪੋ ('        let possible_tags = &[')
				ਲਈ ਟੈਗ_2 ਵਿੱਚ ਟੈਗ_ਸੂਚੀ:
					ਛਾਪੋ ('            %s, // %s' % (hb_ਟੈਗ_rust (ਟੈਗ_2), sanitize_comment (ot.ਨਾਂ[ਟੈਗ_2])))
				ਛਾਪੋ ('        ];')
				ਛਾਪੋ ('        tags.extend_from_slice(possible_tags);')
			ਛਾਪੋ ('        return true;')
			ਛਾਪੋ ('    }')

	ਛਾਪੋ ('    match language.as_bytes()[0] {')
	ਲਈ initial, ਚੀਜ਼ਾਂ ਵਿੱਚ sorted (complex_ਟੈਗ_ਸੂਚੀ.ਚੀਜ਼ਾਂ ()):
		ਜੇ initial == 'und':
			ਜਾਰੀ
		ਛਾਪੋ ("        b'%s' => {" % sanitize_bcp47_ਟੈਗ (initial))
		ਲਈ lt, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ ਚੀਜ਼ਾਂ:
			ਜੇ ਨਹੀਂ ਟੈਗ_ਸੂਚੀ:
				ਜਾਰੀ
			ਛਾਪੋ ('            if ', ਸਿਰਾ='')
			script = lt.script
			region = lt.region
			ਜੇ lt.grandfathered:
				ਛਾਪੋ ('&language[1..] == "%s"' % sanitize_bcp47_ਟੈਗ (lt.language[1:]), ਸਿਰਾ='')
			ਨਹੀਂਤਾਂ:
				string_literal = lt.language[1:] + '-'
				ਜੇ script:
					string_literal += script
					script = ਕੁਝ_ਨਹੀਂ
					ਜੇ region:
						string_literal += '-' + region
						region = ਕੁਝ_ਨਹੀਂ
				ਜੇ string_literal[-1] == '-':
					ਛਾਪੋ ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_ਟੈਗ (string_literal), ਲੰਬਾਈ (string_literal)), ਸਿਰਾ='')
				ਨਹੀਂਤਾਂ:
					ਛਾਪੋ ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_ਟੈਗ (string_literal), ਸਿਰਾ='')
			print_subtag_matches_rust (script, ਸੱਚ)
			print_subtag_matches_rust (region, ਸੱਚ)
			print_subtag_matches_rust (lt.variant, ਸੱਚ)
			ਛਾਪੋ (' {')
			ਛਾਪੋ ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			ਜੇ ਲੰਬਾਈ (ਟੈਗ_ਸੂਚੀ) == 1:
				ਛਾਪੋ ('                tags.push(%s); // %s' % (hb_ਟੈਗ_rust (ਟੈਗ_ਸੂਚੀ[0]), sanitize_comment (ot.ਨਾਂ[ਟੈਗ_ਸੂਚੀ[0]])))
			ਨਹੀਂਤਾਂ:
				ਛਾਪੋ ('                let possible_tags = &[')
				ਲਈ ਟੈਗ_2 ਵਿੱਚ ਟੈਗ_ਸੂਚੀ:
					ਛਾਪੋ ('                    %s, // %s' % (hb_ਟੈਗ_rust (ਟੈਗ_2), sanitize_comment (ot.ਨਾਂ[ਟੈਗ_2])))
				ਛਾਪੋ ('                ];')
				ਛਾਪੋ ('                tags.extend_from_slice(possible_tags);')
			ਛਾਪੋ ('                return true;')
			ਛਾਪੋ ('            }')
		ਛਾਪੋ ('        }')
	ਛਾਪੋ ('        _ => {}')
	ਛਾਪੋ ('    }')
	ਛਾਪੋ ('    false')
	ਛਾਪੋ ('}')
	sys.exit (0)

ਛਾਪੋ ('/* == Start of generated table == */')
ਛਾਪੋ ('/*')
ਛਾਪੋ (' * The following table is generated by running:')
ਛਾਪੋ (' *')
ਛਾਪੋ (' *   %s languagetags language-subtag-registry' % os.ਰਸਤਾ.basename (sys.argv[0]))
ਛਾਪੋ (' *')
ਛਾਪੋ (' * on files with these headers:')
ਛਾਪੋ (' *')
ਛਾਪੋ (' * %s' % sanitize_ਸਿਰਲੇਖ (ot.ਸਿਰਲੇਖ))
ਛਾਪੋ (' * %s' % sanitize_ਸਿਰਲੇਖ (bcp_47.ਸਿਰਲੇਖ))
ਛਾਪੋ (' */')
ਛਾਪੋ ()
ਛਾਪੋ ('#ifndef HB_OT_TAG_TABLE_HH')
ਛਾਪੋ ('#define HB_OT_TAG_TABLE_HH')
ਛਾਪੋ ()

ਪਰਿਭਾਸ਼ਾ hb_ਟੈਗ (ਟੈਗ_2):
	"""Convert a tag to ``HB_TAG`` form.

	Args:
		tag (str): An OpenType tag.

	Returns:
		A snippet of C++ representing ``tag``.
	"""
	ਜੇ ਟੈਗ_2 == DEFAULT_LANGUAGE_SYSTEM:
		ਵਾਪਸ 'HB_TAG_NONE\t       '
	sanitize_ਟੈਗ (ਟੈਗ_2)
	ਵਾਪਸ "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % ਟੈਗ_2)[:4])

ਪਰਿਭਾਸ਼ਾ get_variant_ਸੈੱਟ (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	ਵਾਪਸ ਸੈੱਟ (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			ਲਈ n ਵਿੱਚ re.ਵੰਡੋ ('[\n(),]', name) ਜੇ n)

ਪਰਿਭਾਸ਼ਾ language_name_intersection (a, b):
	"""Return the names in common between two language names.

	Args:
		a (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.
		b (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		The normalized language names shared by ``a`` and ``b``.
	"""
	ਵਾਪਸ get_variant_ਸੈੱਟ (a).intersection (get_variant_ਸੈੱਟ (b))

ਪਰਿਭਾਸ਼ਾ get_matching_language_name (intersection, candidates):
	ਵਾਪਸ ਅਗਲਾ (iter (c ਲਈ c ਵਿੱਚ candidates ਜੇ ਨਹੀਂ intersection.isdisjoint (get_variant_ਸੈੱਟ (c))))

ਪਰਿਭਾਸ਼ਾ same_ਟੈਗ (bcp_47_ਟੈਗ, ot_ਟੈਗ_ਸੂਚੀ):
	ਵਾਪਸ ਲੰਬਾਈ (bcp_47_ਟੈਗ) == 3 ਅਤੇ ਲੰਬਾਈ (ot_ਟੈਗ_ਸੂਚੀ) == 1 ਅਤੇ bcp_47_ਟੈਗ == ot_ਟੈਗ_ਸੂਚੀ[0].ਹੇਠਲਾ ()

ਪਰਿਭਾਸ਼ਾ format_language_comment (language, ਟੈਗ_2):
	bcp_47_name = bcp_47.ਨਾਂ.get (language, '')
	bcp_47_name_candidates = bcp_47_name.ਵੰਡੋ ('\n')
	ot_name = ot.ਨਾਂ[ਟੈਗ_2]
	scope = bcp_47.scopes.get (language, '')
	ਜੇ ਟੈਗ_2 == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	ਨਹੀਂਤਾਂ:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		ਜੇ ਨਹੀਂ intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		ਨਹੀਂਤਾਂ:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.ਨਾਂ[language] = name
			comment = '%s%s' % (name ਜੇ ਲੰਬਾਈ (name) > ਲੰਬਾਈ (ot_name) ਨਹੀਂਤਾਂ ot_name, scope)
	ਵਾਪਸ sanitize_comment (comment)

ਛਾਪੋ ('static const LangTag ot_languages2[] = {')
ਲਈ language, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ sorted (ot.from_bcp_47.ਚੀਜ਼ਾਂ ()):
	ਜੇ language == '' ਜਾਂ '-' ਵਿੱਚ language ਜਾਂ ਲੰਬਾਈ (language) != 2:
		ਜਾਰੀ
	ਲਈ ਟੈਗ_2 ਵਿੱਚ ਟੈਗ_ਸੂਚੀ:
		ਛਾਪੋ ('  {%s,\t%s},\t/* %s */' % (
			hb_ਟੈਗ (language),
			hb_ਟੈਗ (ਟੈਗ_2),
			format_language_comment (language, ਟੈਗ_2)))
ਛਾਪੋ ('};')
ਛਾਪੋ ()

ਛਾਪੋ ('#ifndef HB_NO_LANGUAGE_LONG')

languages3_blocked = []
languages3_singles = []
languages3_multi_ranges = []
languages3_multi_ਮੁੱਲਾਂ = []
ਲਈ language, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ sorted (ot.from_bcp_47.ਚੀਜ਼ਾਂ ()):
	ਜੇ language == '' ਜਾਂ '-' ਵਿੱਚ language ਜਾਂ ਲੰਬਾਈ (language) != 3:
		ਜਾਰੀ
	ਜੇ same_ਟੈਗ (language, ਟੈਗ_ਸੂਚੀ):
		ਜਾਰੀ
	ਜੇ ਟੈਗ_ਸੂਚੀ == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, ਟੈਗ_ਸੂਚੀ[0]))
		ਜਾਰੀ
	ਜੇ ਲੰਬਾਈ (ਟੈਗ_ਸੂਚੀ) == 1:
		languages3_singles.append ((language, ਟੈਗ_ਸੂਚੀ[0]))
		ਜਾਰੀ
	ਵਿੱਥ = ਲੰਬਾਈ (languages3_multi_ਮੁੱਲਾਂ)
	languages3_multi_ਮੁੱਲਾਂ.extend ((language, ਟੈਗ_2) ਲਈ ਟੈਗ_2 ਵਿੱਚ ਟੈਗ_ਸੂਚੀ)
	languages3_multi_ranges.append ((language, ਵਿੱਥ, ਲੰਬਾਈ (ਟੈਗ_ਸੂਚੀ), ਟੈਗ_ਸੂਚੀ[0]))

ਛਾਪੋ ('static const hb_tag_t ot_languages3_blocked[] = {')
ਲਈ language, ਟੈਗ_2 ਵਿੱਚ languages3_blocked:
	ਛਾਪੋ ('  %s,\t/* %s */' % (hb_ਟੈਗ (language), format_language_comment (language, ਟੈਗ_2)))
ਛਾਪੋ ('};')
ਛਾਪੋ ()

ਛਾਪੋ ('static const LangTag ot_languages3[] = {')
ਲਈ language, ਟੈਗ_2 ਵਿੱਚ languages3_singles:
	ਛਾਪੋ ('  {%s,\t%s},\t/* %s */' % (
		hb_ਟੈਗ (language),
		hb_ਟੈਗ (ਟੈਗ_2),
		format_language_comment (language, ਟੈਗ_2)))
ਛਾਪੋ ('};')
ਛਾਪੋ ()

ਛਾਪੋ ('static const hb_tag_t ot_languages3_multi_values[] = {')
ਲਈ language, ਟੈਗ_2 ਵਿੱਚ languages3_multi_ਮੁੱਲਾਂ:
	ਛਾਪੋ ('  %s,\t/* %s */' % (hb_ਟੈਗ (ਟੈਗ_2), format_language_comment (language, ਟੈਗ_2)))
ਛਾਪੋ ('};')
ਛਾਪੋ ()

ਛਾਪੋ ('static const LangTagRange ot_languages3_multi[] = {')
ਲਈ language, ਵਿੱਥ, ਗਿਣਤੀ_2, ਪਹਿਲਾ_ਟੈਗ ਵਿੱਚ languages3_multi_ranges:
	ਛਾਪੋ ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_ਟੈਗ (language),
		ਪੂਰਨ(ਵਿੱਥ),
		ਪੂਰਨ(ਗਿਣਤੀ_2),
		format_language_comment (language, ਪਹਿਲਾ_ਟੈਗ)))
ਛਾਪੋ ('};')
ਛਾਪੋ ('#endif')
ਛਾਪੋ ()

ਛਾਪੋ ('/**')
ਛਾਪੋ (' * hb_ot_tags_from_complex_language:')
ਛਾਪੋ (' * @lang_str: a BCP 47 language tag to convert.')
ਛਾਪੋ (' * @limit: a pointer to the end of the substring of @lang_str to consider for')
ਛਾਪੋ (' * conversion.')
ਛਾਪੋ (' * @count: maximum number of language tags to retrieve (IN) and actual number of')
ਛਾਪੋ (' * language tags retrieved (OUT). If no tags are retrieved, it is not modified.')
ਛਾਪੋ (' * @tags: array of size at least @language_count to store the language tag')
ਛਾਪੋ (' * results')
ਛਾਪੋ (' *')
ਛਾਪੋ (' * Converts a multi-subtag BCP 47 language tag to language tags.')
ਛਾਪੋ (' *')
ਛਾਪੋ (' * Return value: Whether any language systems were retrieved.')
ਛਾਪੋ (' **/')
ਛਾਪੋ ('static inline bool')
ਛਾਪੋ ('hb_ot_tags_from_complex_language (const char   *lang_str,')
ਛਾਪੋ ('\t\t\t\t  const char   *limit,')
ਛਾਪੋ ('\t\t\t\t  unsigned int *count /* IN/OUT */,')
ਛਾਪੋ ('\t\t\t\t  hb_tag_t     *tags /* OUT */)')
ਛਾਪੋ ('{')

ਪਰਿਭਾਸ਼ਾ print_subtag_matches (subtag, string, new_ਸਤਰ):
	ਜੇ subtag:
		ਜੇ new_ਸਤਰ:
			ਛਾਪੋ ()
			ਛਾਪੋ ('\t&& ', ਸਿਰਾ='')
		ਛਾਪੋ ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_ਟੈਗ (subtag), 1 + ਲੰਬਾਈ (subtag)), ਸਿਰਾ='')

complex_ਟੈਗ_ਸੂਚੀ = collections.defaultdict (list)
ਲਈ initial, ਸਮੂਹ ਵਿੱਚ itertools.groupby ((lt_ਟੈਗ_ਸੂਚੀ ਲਈ lt_ਟੈਗ_ਸੂਚੀ ਵਿੱਚ [
			(Language_ਟੈਗ (language), ਟੈਗ_ਸੂਚੀ)
			ਲਈ language, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ sorted (ot.from_bcp_47.ਚੀਜ਼ਾਂ (),
				ਕੁੰਜੀ_2=ਅਨਾਮ i: (-ਲੰਬਾਈ (i[0]), i[0]))
		] ਜੇ lt_ਟੈਗ_ਸੂਚੀ[0].is_complex ()),
		ਕੁੰਜੀ_2=ਅਨਾਮ lt_ਟੈਗ_ਸੂਚੀ: lt_ਟੈਗ_ਸੂਚੀ[0].get_ਸਮੂਹ ()):
	complex_ਟੈਗ_ਸੂਚੀ[initial] += ਸਮੂਹ

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
ਲਈ initial, ਚੀਜ਼ਾਂ ਵਿੱਚ sorted (complex_ਟੈਗ_ਸੂਚੀ.ਚੀਜ਼ਾਂ ()):
	ਜੇ initial != 'und':
		ਜਾਰੀ
	ਲਈ lt, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ ਚੀਜ਼ਾਂ:
		ਜੇ ਨਹੀਂ ਟੈਗ_ਸੂਚੀ:
			ਜਾਰੀ
		subtag_len = 0
		subtag_len += 1 + ਲੰਬਾਈ (lt.script) ਜੇ lt.script ਹੈ ਨਹੀਂ ਕੁਝ_ਨਹੀਂ ਨਹੀਂਤਾਂ 0
		subtag_len += 1 + ਲੰਬਾਈ (lt.region) ਜੇ lt.region ਹੈ ਨਹੀਂ ਕੁਝ_ਨਹੀਂ ਨਹੀਂਤਾਂ 0
		subtag_len += 1 + ਲੰਬਾਈ (lt.variant) ਜੇ lt.variant ਹੈ ਨਹੀਂ ਕੁਝ_ਨਹੀਂ ਨਹੀਂਤਾਂ 0
		min_subtag_len = min(subtag_len, min_subtag_len)

ਛਾਪੋ ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
ਛਾਪੋ ('  {')
ਛਾਪੋ ("    const char *p = strchr (lang_str, '-');")
ਛਾਪੋ ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
ਲਈ initial, ਚੀਜ਼ਾਂ ਵਿੱਚ sorted (complex_ਟੈਗ_ਸੂਚੀ.ਚੀਜ਼ਾਂ ()):
	ਜੇ initial != 'und':
		ਜਾਰੀ
	ਲਈ lt, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ ਚੀਜ਼ਾਂ:
		ਜੇ ਨਹੀਂ ਟੈਗ_ਸੂਚੀ:
			ਜਾਰੀ
		ਜੇ lt.variant ਵਿੱਚ bcp_47.prefixes:
			expect (ਅਗਲਾ (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		ਛਾਪੋ ('    if (', ਸਿਰਾ='')
		print_subtag_matches (lt.script, 'p', ਝੂਠ)
		print_subtag_matches (lt.region, 'p', ਝੂਠ)
		print_subtag_matches (lt.variant, 'p', ਝੂਠ)
		ਛਾਪੋ (')')
		ਛਾਪੋ ('    {')
		ਲਿਖ ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		ਛਾਪੋ ()
		ਜੇ ਲੰਬਾਈ (ਟੈਗ_ਸੂਚੀ) == 1:
			ਲਿਖ ('      tags[0] = %s;  /* %s */' % (hb_ਟੈਗ (ਟੈਗ_ਸੂਚੀ[0]), sanitize_comment (ot.ਨਾਂ[ਟੈਗ_ਸੂਚੀ[0]])))
			ਛਾਪੋ ()
			ਛਾਪੋ ('      *count = 1;')
		ਨਹੀਂਤਾਂ:
			ਛਾਪੋ ('    hb_tag_t possible_tags[] = {')
			ਲਈ ਟੈਗ_2 ਵਿੱਚ ਟੈਗ_ਸੂਚੀ:
				ਲਿਖ ('      %s,  /* %s */' % (hb_ਟੈਗ (ਟੈਗ_2), sanitize_comment (ot.ਨਾਂ[ਟੈਗ_2])))
				ਛਾਪੋ ()
			ਛਾਪੋ ('      };')
			ਛਾਪੋ ('      for (i = 0; i < %s && i < *count; i++)' % ਲੰਬਾਈ (ਟੈਗ_ਸੂਚੀ))
			ਛਾਪੋ ('\ttags[i] = possible_tags[i];')
			ਛਾਪੋ ('      *count = i;')
		ਛਾਪੋ ('      return true;')
		ਛਾਪੋ ('    }')
ਛਾਪੋ ('  }')
ਛਾਪੋ ('out:')

ਛਾਪੋ ('  switch (lang_str[0])')
ਛਾਪੋ ('  {')
ਲਈ initial, ਚੀਜ਼ਾਂ ਵਿੱਚ sorted (complex_ਟੈਗ_ਸੂਚੀ.ਚੀਜ਼ਾਂ ()):
	ਜੇ initial == 'und':
		ਜਾਰੀ
	ਛਾਪੋ ("  case '%s':" % sanitize_bcp47_ਟੈਗ (initial))
	ਲਈ lt, ਟੈਗ_ਸੂਚੀ ਵਿੱਚ ਚੀਜ਼ਾਂ:
		ਜੇ ਨਹੀਂ ਟੈਗ_ਸੂਚੀ:
			ਜਾਰੀ
		ਛਾਪੋ ('    if (', ਸਿਰਾ='')
		script = lt.script
		region = lt.region
		ਜੇ lt.grandfathered:
			ਛਾਪੋ ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_ਟੈਗ (lt.language[1:]), ਸਿਰਾ='')
		ਨਹੀਂਤਾਂ:
			string_literal = lt.language[1:] + '-'
			ਜੇ script:
				string_literal += script
				script = ਕੁਝ_ਨਹੀਂ
				ਜੇ region:
					string_literal += '-' + region
					region = ਕੁਝ_ਨਹੀਂ
			ਜੇ string_literal[-1] == '-':
				ਛਾਪੋ ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_ਟੈਗ (string_literal), ਲੰਬਾਈ (string_literal)), ਸਿਰਾ='')
			ਨਹੀਂਤਾਂ:
				ਛਾਪੋ ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_ਟੈਗ (string_literal), ਲੰਬਾਈ (string_literal)), ਸਿਰਾ='')
		print_subtag_matches (script, 'lang_str', ਸੱਚ)
		print_subtag_matches (region, 'lang_str', ਸੱਚ)
		print_subtag_matches (lt.variant, 'lang_str', ਸੱਚ)
		ਛਾਪੋ (')')
		ਛਾਪੋ ('    {')
		ਲਿਖ ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		ਛਾਪੋ ()
		ਜੇ ਲੰਬਾਈ (ਟੈਗ_ਸੂਚੀ) == 1:
			ਲਿਖ ('      tags[0] = %s;  /* %s */' % (hb_ਟੈਗ (ਟੈਗ_ਸੂਚੀ[0]), sanitize_comment (ot.ਨਾਂ[ਟੈਗ_ਸੂਚੀ[0]])))
			ਛਾਪੋ ()
			ਛਾਪੋ ('      *count = 1;')
		ਨਹੀਂਤਾਂ:
			ਛਾਪੋ ('      unsigned int i;')
			ਛਾਪੋ ('      hb_tag_t possible_tags[] = {')
			ਲਈ ਟੈਗ_2 ਵਿੱਚ ਟੈਗ_ਸੂਚੀ:
				ਲਿਖ ('\t%s,  /* %s */' % (hb_ਟੈਗ (ਟੈਗ_2), sanitize_comment (ot.ਨਾਂ[ਟੈਗ_2])))
				ਛਾਪੋ ()
			ਛਾਪੋ ('      };')
			ਛਾਪੋ ('      for (i = 0; i < %s && i < *count; i++)' % ਲੰਬਾਈ (ਟੈਗ_ਸੂਚੀ))
			ਛਾਪੋ ('\ttags[i] = possible_tags[i];')
			ਛਾਪੋ ('      *count = i;')
		ਛਾਪੋ ('      return true;')
		ਛਾਪੋ ('    }')
	ਛਾਪੋ ('    break;')

ਛਾਪੋ ('  }')
ਛਾਪੋ ('  return false;')
ਛਾਪੋ ('}')
ਛਾਪੋ ()
ਛਾਪੋ ('/**')
ਛਾਪੋ (' * hb_ot_ambiguous_tag_to_language')
ਛਾਪੋ (' * @tag: A language tag.')
ਛਾਪੋ (' *')
ਛਾਪੋ (' * Converts @tag to a BCP 47 language tag if it is ambiguous (it corresponds to')
ਛਾਪੋ (' * many language tags) and the best tag is not the first (sorted alphabetically,')
ਛਾਪੋ (' * with two-letter tags having priority over all three-letter tags), or if the')
ਛਾਪੋ (' * best tag consists of multiple subtags, or if the best tag does not appear in')
ਛਾਪੋ (' * #ot_languages2 or #ot_languages3.')
ਛਾਪੋ (' *')
ਛਾਪੋ (' * Return value: The #hb_language_t corresponding to the BCP 47 language tag,')
ਛਾਪੋ (' * or #HB_LANGUAGE_INVALID if @tag is not ambiguous.')
ਛਾਪੋ (' **/')
ਛਾਪੋ ('static inline hb_language_t')
ਛਾਪੋ ('hb_ot_ambiguous_tag_to_language (hb_tag_t tag)')
ਛਾਪੋ ('{')
ਛਾਪੋ ('  switch (tag)')
ਛਾਪੋ ('  {')

ਪਰਿਭਾਸ਼ਾ verify_disambiguation_dict ():
	"""Verify and normalize ``disambiguation``.

	``disambiguation`` is a map of ambiguous OpenType language system
	tags to the particular BCP 47 tags they correspond to. This function
	checks that all its keys really are ambiguous and that each key's
	value is valid for that key. It checks that no ambiguous tag is
	missing, except when it can figure out which BCP 47 tag is the best
	by itself.

	It modifies ``disambiguation`` to remove keys whose values are the
	same as those that the fallback would return anyway, and to add
	ambiguous keys whose disambiguations it determined automatically.

	Raises:
		AssertionError: Verification failed.
	"""
	ਵੈਸ਼੍ਵਿਕ bcp_47
	ਵੈਸ਼੍ਵਿਕ disambiguation
	ਵੈਸ਼੍ਵਿਕ ot
	ਲਈ ot_ਟੈਗ, bcp_47_ਟੈਗ_ਸੂਚੀ ਵਿੱਚ ot.ਮੰਜ਼ਿਲ_bcp_47.ਚੀਜ਼ਾਂ ():
		ਜੇ ot_ਟੈਗ == DEFAULT_LANGUAGE_SYSTEM:
			primary_ਟੈਗ_ਸੂਚੀ = []
		ਨਹੀਂਤਾਂ:
			primary_ਟੈਗ_ਸੂਚੀ = list (t ਲਈ t ਵਿੱਚ bcp_47_ਟੈਗ_ਸੂਚੀ ਜੇ t ਨਹੀਂ ਵਿੱਚ bcp_47.grandfathered ਅਤੇ ot.from_bcp_47.get (t)[0] == ot_ਟੈਗ)
		ਜੇ ਲੰਬਾਈ (primary_ਟੈਗ_ਸੂਚੀ) == 1:
			expect (ot_ਟੈਗ ਨਹੀਂ ਵਿੱਚ disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_ਟੈਗ)
			ਜੇ '-' ਵਿੱਚ primary_ਟੈਗ_ਸੂਚੀ[0]:
				disambiguation[ot_ਟੈਗ] = primary_ਟੈਗ_ਸੂਚੀ[0]
			ਨਹੀਂਤਾਂ:
				ਪਹਿਲਾ_ਟੈਗ = sorted ((t ਲਈ t ਵਿੱਚ bcp_47_ਟੈਗ_ਸੂਚੀ ਜੇ t ਨਹੀਂ ਵਿੱਚ bcp_47.grandfathered ਅਤੇ ot_ਟੈਗ ਵਿੱਚ ot.from_bcp_47.get (t)),
						ਕੁੰਜੀ_2=ਅਨਾਮ t: (ਲੰਬਾਈ (t), t))[0]
				ਜੇ primary_ਟੈਗ_ਸੂਚੀ[0] != ਪਹਿਲਾ_ਟੈਗ:
					disambiguation[ot_ਟੈਗ] = primary_ਟੈਗ_ਸੂਚੀ[0]
		ਜਾਂਜੇ ਲੰਬਾਈ (primary_ਟੈਗ_ਸੂਚੀ) == 0:
			expect (ot_ਟੈਗ ਨਹੀਂ ਵਿੱਚ disambiguation, 'There is no possible valid disambiguation for %s' % ot_ਟੈਗ)
		ਨਹੀਂਤਾਂ:
			original_languages = [t ਲਈ t ਵਿੱਚ primary_ਟੈਗ_ਸੂਚੀ ਜੇ t ਵਿੱਚ ot.from_bcp_47_uninherited ਅਤੇ 'retired code' ਨਹੀਂ ਵਿੱਚ bcp_47.scopes.get (t, '')]
			ਜੇ ਲੰਬਾਈ (original_languages) == 1:
				macrolanguages = original_languages
			ਨਹੀਂਤਾਂ:
				macrolanguages = [t ਲਈ t ਵਿੱਚ primary_ਟੈਗ_ਸੂਚੀ ਜੇ bcp_47.scopes.get (t) == ' [macrolanguage]']
			ਜੇ ਲੰਬਾਈ (macrolanguages) != 1:
				macrolanguages = list (t ਲਈ t ਵਿੱਚ primary_ਟੈਗ_ਸੂਚੀ ਜੇ bcp_47.scopes.get (t) == ' [collection]')
			ਜੇ ਲੰਬਾਈ (macrolanguages) != 1:
				macrolanguages = list (t ਲਈ t ਵਿੱਚ primary_ਟੈਗ_ਸੂਚੀ ਜੇ 'retired code' ਨਹੀਂ ਵਿੱਚ bcp_47.scopes.get (t, ''))
			ਜੇ ਲੰਬਾਈ (macrolanguages) != 1:
				macrolanguages = list (t ਲਈ t ਵਿੱਚ primary_ਟੈਗ_ਸੂਚੀ ਜੇ t.ਹੇਠਲਾ () == ISO_639_3_ਮੰਜ਼ਿਲ_1.get (ot_ਟੈਗ.ਹੇਠਲਾ (), ot_ਟੈਗ.ਹੇਠਲਾ ()))
			ਜੇ ਲੰਬਾਈ (macrolanguages) != 1:
				macrolanguages = list (t ਲਈ t ਵਿੱਚ primary_ਟੈਗ_ਸੂਚੀ ਜੇ '-' ਨਹੀਂ ਵਿੱਚ t)
			ਜੇ ਲੰਬਾਈ (macrolanguages) != 1:
				expect (ot_ਟੈਗ ਵਿੱਚ disambiguation, 'ambiguous OT tag: %s %s' % (ot_ਟੈਗ, sorted (primary_ਟੈਗ_ਸੂਚੀ)))
				expect (disambiguation[ot_ਟੈਗ] ਵਿੱਚ bcp_47_ਟੈਗ_ਸੂਚੀ,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_ਟੈਗ], ot_ਟੈਗ))
			ਜਾਂਜੇ ot_ਟੈਗ ਨਹੀਂ ਵਿੱਚ disambiguation:
				disambiguation[ot_ਟੈਗ] = macrolanguages[0]
			ਜੇ '-' ਨਹੀਂ ਵਿੱਚ disambiguation[ot_ਟੈਗ]:
				different_bcp_47_ਟੈਗ_ਸੂਚੀ = sorted ((t ਲਈ t ਵਿੱਚ bcp_47_ਟੈਗ_ਸੂਚੀ ਜੇ ਨਹੀਂ same_ਟੈਗ (t, ot.from_bcp_47.get (t))),
						ਕੁੰਜੀ_2=ਅਨਾਮ t: (ਲੰਬਾਈ (t), t))
				ਜੇ different_bcp_47_ਟੈਗ_ਸੂਚੀ ਅਤੇ disambiguation[ot_ਟੈਗ] == different_bcp_47_ਟੈਗ_ਸੂਚੀ[0]:
					ਹਟਾਓ disambiguation[ot_ਟੈਗ]
	ਲਈ ot_ਟੈਗ ਵਿੱਚ disambiguation.ਕੁੰਜੀਆਂ ():
		expect (ot_ਟੈਗ ਵਿੱਚ ot.ਮੰਜ਼ਿਲ_bcp_47, 'unknown OT tag: %s' % ot_ਟੈਗ)

verify_disambiguation_dict ()
ਲਈ ot_ਟੈਗ, bcp_47_ਟੈਗ ਵਿੱਚ sorted (disambiguation.ਚੀਜ਼ਾਂ ()):
	ਲਿਖ ('  case %s:  /* %s */' % (hb_ਟੈਗ (ot_ਟੈਗ), sanitize_comment (ot.ਨਾਂ[ot_ਟੈਗ])))
	ਛਾਪੋ ()
	ਲਿਖ ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_ਟੈਗ (bcp_47_ਟੈਗ), sanitize_comment (bcp_47.get_name (Language_ਟੈਗ (bcp_47_ਟੈਗ)))))
	ਛਾਪੋ ()

ਛਾਪੋ ('  default:')
ਛਾਪੋ ('    return HB_LANGUAGE_INVALID;')
ਛਾਪੋ ('  }')
ਛਾਪੋ ('}')

ਛਾਪੋ ()
ਛਾਪੋ ('#endif /* HB_OT_TAG_TABLE_HH */')
ਛਾਪੋ ()
ਛਾਪੋ ('/* == End of generated table == */')
