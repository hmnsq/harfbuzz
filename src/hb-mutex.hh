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

#अगर_अघोषित HB_MUTEX_HH
#घोषणा HB_MUTEX_HH

#समावेश "hb.hh"


/* mutex */

/* We need external help for these */

#अगर defined(hb_mutex_impl_init) \
 && defined(hb_mutex_impl_lock) \
 && defined(hb_mutex_impl_unlock) \
 && defined(hb_mutex_impl_finish)

/* Defined externally, i.e. in config.h; must have typedef'ed hb_mutex_impl_t as well. */


#या_अगर !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && (defined(HAVE_PTHREAD) || defined(__APPLE__))

#समावेश <pthread.h>
प्रकार pthread_mutex_t hb_mutex_impl_t;
#घोषणा hb_mutex_impl_init(M)	pthread_mutex_init (M, nullptr)
#घोषणा hb_mutex_impl_lock(M)	pthread_mutex_lock (M)
#घोषणा hb_mutex_impl_unlock(M)	pthread_mutex_unlock (M)
#घोषणा hb_mutex_impl_finish(M)	pthread_mutex_destroy (M)


#या_अगर !defined(HB_NO_MT) && !defined(HB_MUTEX_IMPL_STD_MUTEX) && defined(_WIN32)

प्रकार CRITICAL_SECTION hb_mutex_impl_t;
#अगर !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#घोषणा hb_mutex_impl_init(M)	InitializeCriticalSectionEx (M, 0, 0)
#अन्यथा
#घोषणा hb_mutex_impl_init(M)	InitializeCriticalSection (M)
#पूर्ण_अगर
#घोषणा hb_mutex_impl_lock(M)	EnterCriticalSection (M)
#घोषणा hb_mutex_impl_unlock(M)	LeaveCriticalSection (M)
#घोषणा hb_mutex_impl_finish(M)	DeleteCriticalSection (M)


#या_अगर !defined(HB_NO_MT)

#समावेश <mutex>
प्रकार std::mutex              hb_mutex_impl_t;
#घोषणा hb_mutex_impl_init(M)   HB_STMT_START { नया (M) hb_mutex_impl_t; } HB_STMT_END
#घोषणा hb_mutex_impl_lock(M)   (M)->lock ()
#घोषणा hb_mutex_impl_unlock(M) (M)->unlock ()
#घोषणा hb_mutex_impl_finish(M) HB_STMT_START { (M)->~hb_mutex_impl_t(); } HB_STMT_END


#अन्यथा /* defined(HB_NO_MT) */

प्रकार पूर्णांक hb_mutex_impl_t;
#घोषणा hb_mutex_impl_init(M)	HB_STMT_START {} HB_STMT_END
#घोषणा hb_mutex_impl_lock(M)	HB_STMT_START {} HB_STMT_END
#घोषणा hb_mutex_impl_unlock(M)	HB_STMT_START {} HB_STMT_END
#घोषणा hb_mutex_impl_finish(M)	HB_STMT_START {} HB_STMT_END


#पूर्ण_अगर


काष्ठा hb_mutex_t
{
  /* Create space for, but do not initialize m. */
  alignas(hb_mutex_impl_t) अक्षर m[माप (hb_mutex_impl_t)];

  hb_mutex_t () { init (); }
  ~hb_mutex_t () { fini (); }
  hb_mutex_t (स्थिर hb_mutex_t &) = मिटाओ;
  hb_mutex_t &चालक= (स्थिर hb_mutex_t &) = मिटाओ;

#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-align"
  व्योम init   () { hb_mutex_impl_init   ((hb_mutex_impl_t *) m); }
  व्योम lock   () { hb_mutex_impl_lock   ((hb_mutex_impl_t *) m); }
  व्योम unlock () { hb_mutex_impl_unlock ((hb_mutex_impl_t *) m); }
  व्योम fini   () { hb_mutex_impl_finish ((hb_mutex_impl_t *) m); }
#आशय GCC diagnostic निकालो
};

काष्ठा hb_lock_t
{
  hb_lock_t (hb_mutex_t &mutex_) : mutex (&mutex_) { mutex->lock (); }
  hb_lock_t (hb_mutex_t *mutex_) : mutex (mutex_) { अगर (mutex) mutex->lock (); }
  ~hb_lock_t () { अगर (mutex) mutex->unlock (); }

  hb_lock_t (स्थिर hb_lock_t &) = मिटाओ;
  hb_lock_t &चालक= (स्थिर hb_lock_t &) = मिटाओ;

  निजी:
  hb_mutex_t *mutex;
};


#पूर्ण_अगर /* HB_MUTEX_HH */
