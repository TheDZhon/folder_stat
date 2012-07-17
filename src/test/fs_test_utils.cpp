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

#include "fs_test_utils.h"

namespace
{
	const QDir::Filters kDirsFilter = QDir::NoDotAndDotDot
									  | QDir::AllDirs
									  | QDir::Files
									  | QDir::DirsFirst;

	const QDir kTempPath = QDir::temp();

	const QString kMkPathErrorMess = "Can't create folders in TEMP: ";
	const QString kRmPathErrorMess = "Can't remove folders in TEMP: ";
	const QString kMkFileErrorMess = "Can't create files in TEMP: ";

	const QIODevice::OpenMode kDefaultOpenMode = QIODevice::WriteOnly
												 | QIODevice::Truncate;
}

namespace test
{
	bool rmPathRecursive (const QString& path)
	{
		const QDir dir (path);

		if (!dir.exists()) { return true; }

		const QFileInfoList& children = dir.entryInfoList (kDirsFilter);
		typedef QFileInfoList::const_iterator It;
		for (It it = children.begin();
			 it != children.end();
			 ++it) {
			if (it->isDir() && !rmPathRecursive (it->absoluteFilePath())) { return false; }
			if (it->isFile() && !QFile::remove (it->absoluteFilePath())) { return false; }
		}

		return dir.rmdir (path);
	}

	void rmPathInTempRecursive (const QString& path)
	{
		QVERIFY2 (rmPathRecursive (kTempPath.canonicalPath() + "/" + path),
				  QString (kRmPathErrorMess + path).toAscii().constData());
	}

	void mkPathInTemp (const QString& path)
	{
		QVERIFY2 (kTempPath.mkpath (path),
				  QString (kMkPathErrorMess + path).toAscii().constData());
	}

	QFileInfoList createTestFiles (const QString& path, const SizeTVector& cnt_list)
	{
		QFileInfoList ret;

		typedef SizeTVector::const_iterator It;

		for (It it = cnt_list.begin();
			 it != cnt_list.end();
			 ++it) {
			const size_t index = std::distance (cnt_list.begin(), it);
			for (size_t i = 0; i < *it; ++i) {
				const QString fname = kTempPath.canonicalPath()
									  + "/"
									  + path
									  + "/"
									  + QString::number (i)
									  + "."
									  + QString::number (index);
				ret << fname;

				QFile current_file (fname);
				current_file.open (kDefaultOpenMode);
				current_file.write (QByteArray (kDefaultFileSz, '0'));
			}
		}

		return ret;
	}

	QFileInfoList createTestSubdirs (const QString& path, size_t subdirs_cnt)
	{
		QFileInfoList ret;

		for (size_t i = 0; i < subdirs_cnt; ++i) {
			const QString subdir = path + "/" + QString::number (i);
			mkPathInTemp (subdir);
			ret << subdir;
		}

		return ret;
	}
}