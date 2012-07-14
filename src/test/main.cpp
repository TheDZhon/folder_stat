#include <QTest>

#include "test/fs_cacher_test.h"

using namespace test;

int main (int argc, char ** argv)
{
	CacherTest cacher_test;
	int cacher_test_result = QTest::qExec(&cacher_test, argc, argv);
	
	return cacher_test_result;
}