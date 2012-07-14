#include <QTest>

#include "test/fs_stat_data_test.h"
#include "test/fs_cacher_test.h"
#include "test/fs_collector_test.h"

using namespace test;

int main (int argc, char** argv)
{
	StatDataTest stat_test;
	int stat_test_result = QTest::qExec (&stat_test, argc, argv);
	if (stat_test_result != EXIT_SUCCESS) { return stat_test_result; }

	CacherTest cacher_test;
	int cacher_test_result = QTest::qExec (&cacher_test, argc, argv);
	if (cacher_test_result != EXIT_SUCCESS) { return cacher_test_result; }

	CollectorTest collector_test;
	int collector_test_result = QTest::qExec (&collector_test, argc, argv);
	if (collector_test_result != EXIT_SUCCESS) { return collector_test_result; }	

	return EXIT_SUCCESS;
}