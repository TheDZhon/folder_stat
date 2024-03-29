//------------------------------------------------------------------------------------------------------------------------------------------
//    Copyright (c) 2012, Eugene Mamin <TheDZhon@gmail.com>
//    All rights reserved.
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions are met:
//        * Redistributions of source code must retain the above copyright
//        notice, this list of conditions and the following disclaimer.
//        * Redistributions in binary form must reproduce the above copyright
//        notice, this list of conditions and the following disclaimer in the
//        documentation and/or other materials provided with the distribution.
//        * Neither the name of the Prefix Increment nor the
//        names of its contributors may be used to endorse or promote products
//        derived from this software without specific prior written permission.
//
//    THIS SOFTWARE IS PROVIDED BY Eugene Mamin <TheDZhon@gmail.com> ''AS IS'' AND ANY
//    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//    DISCLAIMED. IN NO EVENT SHALL Eugene Mamin <TheDZhon@gmail.com> BE LIABLE FOR ANY
//    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------------------------------------------------------------------

#ifndef FS_CACHER_H__
#define FS_CACHER_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "fs_stat_data.h"

#include <QCache>
#include <QReadWriteLock>

namespace core
{
	/**
	 ** Cacher class.
	 ** Provides thread-safe data caching with many readers/one writer scheme.
	 **/
	class Cacher
	{
	public:
		/**
		 ** Default constructor.
		 ** @param[in] init_sz cache capacity.
		 **/
		Cacher (size_t init_sz = 10000);
		/**
		 ** Destructor.
		 **/
		~Cacher () {}
		/**
		 ** Get stat data item from cache by path.
		 ** @param[in] path key
		 ** @return found value or default constructed pointer if key not exists
		 **/
		StatDataPtr get (const QString& path) const;
		/**
		 ** Store stat data item in cache
		 ** @param[in] path key
		 ** @param[in] stat_data value
		 **/
		void store (const QString& path, const StatDataPtr& stat_data);
		/**
		 ** Set cache capacity.
		 ** Old items removed when capacity exceeded.
		 ** @param[in] num new capacity
		 **/
		void setMaxSize (size_t num);
		/**
		 ** Remove stat data for given path from cache.
		 ** Does nothing for unknown path.
		 ** @param[in] path key
		 **/
		void invalidate (const QString& path);
		/**
		 ** Remove all stat data from cache.
		 **/
		void clear ();
	private:
		typedef QCache<QString, StatDataPtr> StatDataCache;

		Q_DISABLE_COPY (Cacher);

		StatDataCache cache_;
		mutable QReadWriteLock lock_;
	};
}

#endif // FS_CACHER_H__

