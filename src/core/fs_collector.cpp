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

#include "fs_collector.h"

#include <QDir>
#include <QFileInfo>
#include <QRunnable>

namespace
{
	const char* kNotExists = QT_TRANSLATE_NOOP ("core::Collector", "Invalid path");
	const char* kNotADir = QT_TRANSLATE_NOOP ("core::Collector", "Not a directory");
	const char* kCantRead = QT_TRANSLATE_NOOP ("core::Collector", "Path is not readable");
	const char* kCanceled = QT_TRANSLATE_NOOP ("core::Collector", "Task canceled by user");

	const QDir::Filters kCommonFilters = QDir::Readable
										 | QDir::Hidden
										 | QDir::System
										 | QDir::NoSymLinks
										 | QDir::NoDotAndDotDot;

	const QDir::Filters kDirsFilter = QDir::Dirs | kCommonFilters;
	const QDir::Filters kFilesFilter = QDir::Files | kCommonFilters;
}

namespace core
{
	class TerminatorTask: public QRunnable
	{
		void run () {
			QThreadPool::globalInstance()->waitForDone();
		}
	};

	struct Collector::mapper:
			std::unary_function<const QFileInfo&, StatDataPtr> {

		mapper (const QString& path, Collector* c, bool use_cache) :
			path_ (path), collector_ (c), use_cache_ (use_cache) { Q_ASSERT (c); }

		StatDataPtr operator() (const QFileInfo& finfo) {
			Q_ASSERT (finfo.isDir());

			return collector_->collectImplAux (path_, finfo, use_cache_);
		}

		QString path_;
		Collector* collector_;
		bool use_cache_;
	};

	struct Collector::reducer :
			std::binary_function<StatDataPtr&, const StatDataPtr&, void> {

		reducer (StatDataPtr acc) : acc_ (acc) { Q_ASSERT (!acc.isNull()); }

		void operator () (StatDataPtr& acc, const StatDataPtr& cur) {
			Q_ASSERT (!cur.isNull());
			acc_->appendOther (*cur);
		}

		StatDataPtr acc_;
	};

	Collector::Collector (QObject* parent) :
		QObject (parent),
		cacher_(),
		terminator_flag_ (kWork),
		terminator_pool_()
	{
		terminator_pool_.reserveThread();
	}

	Collector::~Collector ()
	{
		cancel ();
	}

	void Collector::cancel()
	{
		terminator_flag_ = kTerminate;
		terminator_pool_.start (new TerminatorTask, QThread::HighestPriority);
	}

	void Collector::collectImpl (const QString& path, bool use_cache)
	{
		const QString& canonPath = QFileInfo (path).canonicalFilePath();
		const QFileInfo pathInfo (canonPath);

		if (!pathInfo.exists()) { emit error (path, tr (kNotExists)); return; }
		if (!pathInfo.isDir()) { emit error (path, tr (kNotADir)); return; }
		if (!pathInfo.isReadable()) { emit error (path, tr (kCantRead)); return; }

		emit directSubfolders (path, getSubdirs (pathInfo).size());

		StatDataPtr full_answer = collectImplAux (path, pathInfo, use_cache);

		if (terminator_flag_ == kTerminate) {
			emit error (path, tr (kCanceled));
			cacher_.clear();
		} else {
			emit finished (path, full_answer);
		}
	}

	StatDataPtr Collector::collectImplAux (const QString& path, const QFileInfo& pinfo, bool use_cache)
	{
		typedef QFileInfoList::iterator It;

		Q_ASSERT (pinfo.isDir());

		const QString& cpath = pinfo.canonicalFilePath();

		if (use_cache) {
			const StatDataPtr& from_cache = cacher_.get (cpath);
			if (!from_cache.isNull()) {
				return from_cache;
			}
		}

		StatDataPtr answer (new StatData);

		const QFileInfoList& subdirs = getSubdirs (pinfo);
		emit currentScannedDir (path, pinfo.canonicalPath());

		const QFileInfoList& files = getFiles (pinfo);

		answer->collectFilesExts (files);

		if (terminator_flag_ != kTerminate) {
			async::blockingMappedReduced<StatDataPtr> (subdirs, mapper (path, this, use_cache), reducer (answer));
			if (use_cache) { cacher_.store (pinfo.canonicalFilePath(), answer); }
		}

		return answer;
	}

	QFileInfoList Collector::getSubdirs (const QFileInfo& f) const
	{
		Q_ASSERT (f.isDir());
		return QDir (f.canonicalFilePath()).entryInfoList (kDirsFilter);
	}

	QFileInfoList Collector::getFiles (const QFileInfo& f) const
	{
		Q_ASSERT (f.isDir());
		return QDir (f.canonicalFilePath()).entryInfoList (kFilesFilter);
	}
}