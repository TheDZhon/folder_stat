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

#include "fs_cacher.h"

#include <QtGlobal>
#include <QFileInfo>

namespace core
{
	Cacher::Cacher() :
		cache_(),
		lock_()
	{

	}

	core::StatDataPtr Cacher::get (const QString& path) const
	{
		QReadLocker _ (&lock_);
		return cache_.value (path);
	}

	void Cacher::store (const QString& path, const StatDataPtr& stat_data)
	{
		QWriteLocker _ (&lock_);
		cache_.insert (path, stat_data);
	}

	void Cacher::reserve (size_t num)
	{
		QWriteLocker _ (&lock_);
		cache_.reserve (num);
	}

	void Cacher::invalidate (const QString& path)
	{
		QWriteLocker _ (&lock_);
		cache_.remove (path);
	}

	void Cacher::clear()
	{
		QWriteLocker _ (&lock_);
		cache_.clear();
	}

}