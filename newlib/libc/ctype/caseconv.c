/*
Copyright (c) 2016 Corinna Vinschen <corinna@vinschen.de> 
Modified (m) 2017 Thomas Wolff: revise Unicode and locale/wchar handling
 */
/* Modified (m) 2017 Thomas Wolff: revise Unicode and locale/wchar handling */

#define _GNU_SOURCE
#include <wctype.h>
#include <stdint.h>
#include <langinfo.h>
#include "local.h"

static const struct caseconv_entry table [] = {
#include "caseconv.t"
};
#define first(ce)	((wint_t) ce.first)
#define last(ce)	((wint_t) (ce.first + ce.diff))
#define NCASECONV       (sizeof table / sizeof table[0])

/* auxiliary function for binary search in interval properties table */
const struct caseconv_entry *
__caseconv_lookup (wint_t ucs, struct __locale_t *locale)
{
  size_t min = 0;
  size_t max = NCASECONV-1;
  size_t mid;
  const char *ctype_locale;

  if (!locale)
      locale = __get_current_locale();
  ctype_locale = locale->categories[NL_LOCALE_NAME(LC_CTYPE) - NL_LOCALE_NAME(LC_ALL)];
  if (ucs >= 0x80 && !strcmp (ctype_locale, "C"))
    return 0;

  if (ucs < first(table[0]) || ucs > last(table[max]))
    return 0;
  while (max >= min)
    {
      mid = (min + max) / 2;
      if (ucs > last(table[mid]))
	min = mid + 1;
      else if (ucs < first(table[mid]))
	max = mid - 1;
      else
	return &table[mid];
    }
  return 0;
}
