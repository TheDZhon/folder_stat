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
#include <QThreadPool>
#include <QAtomicInt>

namespace async = QtConcurrent;

namespace core
{
	/**
	 ** Statistics collector class.
	 ** Provides high-level API for calculating files information.
	 ** All public methods are thread-safe and do any work in background.
	 **/
	class Collector: public QObject
	{
		Q_OBJECT
	public:
		/**
		 ** Default QObject-style constructor.
		 ** @param[in] parent parent object
		 **/
		Collector (QObject* parent = NULL);
		/**
		 ** Destructor.
		 **/
		virtual ~Collector ();
	public slots:
		/**
		 ** Collect statistics about given file.
		 ** @param[in] path path for scanning
		 ** @param[in] use_cache use internal cache for fast lookup
		 **/
		inline void collect (const QString& path, bool use_cache) {
			terminator_flag_ = kWork;
			async::run (this, &Collector::collectImpl, path, use_cache);
		}
		/**
		 ** Cancel current collect operations.
		 **/
		void cancel ();
		/**
		 ** Set new cache capacity.
		 ** @param[in] sz capacity
		 **/
		inline void setCacheSize (size_t sz) {
			async::run (&cacher_, &Cacher::setMaxSize, sz);
		}
		/**
		 ** Clear all data from cache.
		 **/
		inline void clearCache () {
			async::run (&cacher_, &Cacher::clear);
		}
	signals:
		/**
		 ** Report error while scanning.
		 ** @param[in] path scanning path
		 ** @param[in] mess error message
		 **/
		void error (const QString& path, const QString& mess) const;
		/**
		 ** Report direct subfolders for path.
		 ** @param[in] path scanning path
		 ** @param[in] cnt subfolders number
		 **/
		void directSubfolders (const QString& path, int cnt) const;
		/**
		 ** Report scanning progress (current scanned directory).
		 ** @param[in] path scanning path
		 ** @param[in] current_dir current scanned directory
		 **/
		void currentScannedDir (const QString& path, const QString& current_dir) const;
		/**
		 ** Report successfully completion of scanning task.
		 ** @param[in] path scanning path
		 ** @param[in] stat_data collected statistical data
		 **/
		void finished (const QString& path, const core::StatDataPtr& stat_data) const;
	private:
		struct mapper;
		struct reducer;

		friend struct mapper;
		friend struct reducer;

		enum TerminationState {
			kTerminate,
			kWork
		};

		Q_DISABLE_COPY (Collector);

		void collectImpl (const QString& path, bool use_cache);
		StatDataPtr collectImplAux (const QString& path, const QFileInfo& pinfo, bool use_cache);

		QFileInfoList getSubdirs (const QFileInfo& f) const;
		QFileInfoList getFiles (const QFileInfo& f) const;

		Cacher cacher_;

		QAtomicInt terminator_flag_;
		QThreadPool terminator_pool_;
	};
}

#endif // FS_COLLECTOR_H__

