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

#ifndef FS_LIGHTWEIGHT_ICON_PROVIDER__
#define FS_LIGHTWEIGHT_ICON_PROVIDER__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <QFileIconProvider>

namespace
{
	const QString kEmptyType = "_";
	const size_t kLettersInDrivePath = 3;
}

namespace gui
{
	/**
	 ** Lightweight icon provider for QFileSystemModel.
	 ** Used by DirsTreeWidget for drawing optimizations.
	 ** @sa DirsTreeWidget
	 **/
	class LightweightIconProvider:
		public QFileIconProvider
	{
	public:
		LightweightIconProvider() :
			folder_ (":/icons/folder"),
			drive_ (":/icons/drive")
		{}
		/**
		 ** Get icon by type.
		 ** Reimplemented from base.
		 ** @param[in] type icon type
		 ** @return icon object
		 **/
		virtual QIcon icon (QFileIconProvider::IconType type) const {
			if (type == QFileIconProvider::Drive) { return drive_; }
			return folder_;
		}
		/**
		 ** Get icon by file information object.
		 ** Reimplemented from base.
		 ** @param[in] info file information
		 ** @return icon object
		 **/
		virtual QIcon icon (const QFileInfo& info) const {
#if defined(Q_OS_WIN)
			if (info.canonicalFilePath().size() == kLettersInDrivePath) { return drive_; }
#endif
			return folder_;
		}
		/**
		 ** Get icon type string by file information.
		 ** Reimplemented from base.
		 ** @param[in] info file information
		 ** @return type string
		 **/
		virtual QString	type (const QFileInfo& info) const {
			return kEmptyType;
		}
	private:
		QIcon folder_;
		QIcon drive_;
	};
}

#endif // FS_LIGHTWEIGHT_ICON_PROVIDER__