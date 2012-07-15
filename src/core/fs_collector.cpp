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

namespace {
	const QString kNotExists = QObject::tr ("Given path is not exists");
	const QString kNotADir = QObject::tr ("Given path is not a directory");
}

namespace core
{
	Collector::Collector (QObject* parent /*= NULL*/) :
		QObject (parent),
		cacher_()
	{}

	void Collector::collectImpl (const QString& path, CachePolicy policy)
	{
		emit progress (path, kStarted);

		const QString & canonPath = QFileInfo(path).canonicalFilePath();
		const QFileInfo pathInfo (canonPath);

		if (!pathInfo.exists()) { emit error(path, kNotExists); return; }
		if (!pathInfo.isDir()) { emit error(path, kNotADir); return; }

		if (policy != kNoCache) {
			const StatDataPtr & from_cache = cacher_.get(canonPath);
			if (!from_cache.isNull()) {
				emit progress (path, kDirCollected);
				emit progress (path, kStatCalculated);
				emit progress (path, kFinished);
				emit finished (path, from_cache);				
				return;
			}
		}
	}

	void Collector::pauseImpl (const QString& path)
	{

	}

	void Collector::cancelImpl (const QString& path)
	{

	}

	void Collector::clearCacheImpl (const QString& path)
	{
		if (path.isNull()) { cacher_.clear (); return; }

		cacher_.invalidate (path);
	}
}