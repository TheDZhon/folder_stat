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

#ifndef FS_COLLECTOR_H__
#define FS_COLLECTOR_H__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "fs_cacher.h"

#include <QObject>
#include <QtCore>
#include <QSharedPointer>
#include <QAtomicInt>

namespace async = QtConcurrent;

namespace core
{
	class Collector: public QObject
	{
		Q_OBJECT
	public:
		Collector (QObject* parent = NULL);
		virtual ~Collector ();
	public slots:
		inline void collect (const QString& path, bool use_cache = true) {
			async::run (this, &Collector::collectImpl, path, use_cache);
		}
		inline void cancel () {
			terminator_ = kTerminate; // lightweight
		}
		inline void setCacheSize (size_t sz) {
			async::run (this, &Collector::setCacheSizeImpl, sz);
		}
		inline void clearCache (const QString& path = QString()) {
			async::run (this, &Collector::clearCacheImpl, path);
		}
	signals:
		void error (const QString&, const QString&) const;
		void dirsCollected (const QString&, const QFileInfoList&) const;
		void finished (const QString&, const core::StatDataPtr&) const;
	private:
		struct mapper;
		struct reducer;

		friend struct mapper;
		friend struct reducer;

		enum {
			kTerminate,
			kWork
		};

		Q_DISABLE_COPY (Collector);

		void collectImpl (const QString& path, bool use_cache);
		void setCacheSizeImpl (size_t sz);
		void clearCacheImpl (const QString& path);

		StatDataPtr collectImplAux (const QFileInfo& pinfo, bool use_cache);

		QFileInfoList getSubdirs (const QFileInfo& f) const;
		QFileInfoList getFiles (const QFileInfo& f) const;

		Cacher cacher_;

		QAtomicInt terminator_;
	};
}

#endif // FS_COLLECTOR_H__

