/*
 * Copyright © 2007  Chris Wilson
 * Copyright © 2009,2010  Red Hat, Inc.
 * Copyright © 2011,2012  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Contributor(s):
 *	Chris Wilson <chris@chris-wilson.co.uk>
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_MUTEX_HH
#define HB_MUTEX_HH

#അന്തര്ഭാവ "hb.hh"


/* mutex */

/* We need external help for these */

#എങ്കിൽ defined(hb_mutex_impl_init) \
 && defined(hb_mutex_impl_lock) \
 && defined(hb_mutex_impl_unlock) \
 && defined(hb_mutex_impl_finish)

/* Defined externally, i.e. in config.h; must have typedef'ed hb_mutex_impl_t as well. */


#അല്ലെങ്കിലെങ്കിൽ !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && (defined(HAVE_PTHREAD) || defined(__APPLE__))

#അന്തര്ഭാവ <pthread.h>
തരനിർവചനം pthread_mutex_t hb_mutex_impl_t;
#define hb_mutex_impl_init(M)	pthread_mutex_init (M, nullptr)
#define hb_mutex_impl_lock(M)	pthread_mutex_lock (M)
#define hb_mutex_impl_unlock(M)	pthread_mutex_unlock (M)
#define hb_mutex_impl_finish(M)	pthread_mutex_destroy (M)


#അല്ലെങ്കിലെങ്കിൽ !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && defined(_WIN32)

തരനിർവചനം CRITICAL_SECTION hb_mutex_impl_t;
#എങ്കിൽ !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#define hb_mutex_impl_init(M)	InitializeCriticalSectionEx (M, 0, 0)
#അല്ലെങ്കിൽ
#define hb_mutex_impl_init(M)	InitializeCriticalSection (M)
#endif
#define hb_mutex_impl_lock(M)	EnterCriticalSection (M)
#define hb_mutex_impl_unlock(M)	LeaveCriticalSection (M)
#define hb_mutex_impl_finish(M)	DeleteCriticalSection (M)


#അല്ലെങ്കിലെങ്കിൽ !defined(HB_NO_MT)

#അന്തര്ഭാവ <mutex>
തരനിർവചനം std::mutex              hb_mutex_impl_t;
#define hb_mutex_impl_init(M)   HB_STMT_START { പുതിയ (M) hb_mutex_impl_t; } HB_STMT_END
#define hb_mutex_impl_lock(M)   (M)->lock ()
#define hb_mutex_impl_unlock(M) (M)->unlock ()
#define hb_mutex_impl_finish(M) HB_STMT_START { (M)->~hb_mutex_impl_t(); } HB_STMT_END


#അല്ലെങ്കിൽ /* defined(HB_NO_MT) */

തരനിർവചനം പൂർണ്ണസംഖ്യ hb_mutex_impl_t;
#define hb_mutex_impl_init(M)	HB_STMT_START {} HB_STMT_END
#define hb_mutex_impl_lock(M)	HB_STMT_START {} HB_STMT_END
#define hb_mutex_impl_unlock(M)	HB_STMT_START {} HB_STMT_END
#define hb_mutex_impl_finish(M)	HB_STMT_START {} HB_STMT_END


#endif


ഘടന hb_mutex_t
{
  /* Create space for, but do not initialize m. */
  alignas(hb_mutex_impl_t) അക്ഷരം m[വലുപ്പം (hb_mutex_impl_t)];

  hb_mutex_t () { init (); }
  ~hb_mutex_t () { fini (); }
  hb_mutex_t (സ്ഥിരം hb_mutex_t &) = delete;
  hb_mutex_t &operator= (സ്ഥിരം hb_mutex_t &) = delete;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
  ശൂന്യം init   () { hb_mutex_impl_init   ((hb_mutex_impl_t *) m); }
  ശൂന്യം lock   () { hb_mutex_impl_lock   ((hb_mutex_impl_t *) m); }
  ശൂന്യം unlock () { hb_mutex_impl_unlock ((hb_mutex_impl_t *) m); }
  ശൂന്യം fini   () { hb_mutex_impl_finish ((hb_mutex_impl_t *) m); }
#pragma GCC diagnostic എടുക്കുക
};

ഘടന hb_lock_t
{
  hb_lock_t (hb_mutex_t &mutex_) : mutex (&mutex_) { mutex->lock (); }
  hb_lock_t (hb_mutex_t *mutex_) : mutex (mutex_) { എങ്കിൽ (mutex) mutex->lock (); }
  ~hb_lock_t () { എങ്കിൽ (mutex) mutex->unlock (); }

  hb_lock_t (സ്ഥിരം hb_lock_t &) = delete;
  hb_lock_t &operator= (സ്ഥിരം hb_lock_t &) = delete;

  സ്വകാര്യ:
  hb_mutex_t *mutex;
};


#endif /* HB_MUTEX_HH */
