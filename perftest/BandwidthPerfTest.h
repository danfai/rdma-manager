#ifndef PerfThread_H
#define PerfThread_H

#include "PerfTest.h"
#include "../rdma/RDMAClient.h"
#include "../rdma/RDMAServer.h"

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <iostream>

namespace rdma {

class BandwidthPerfThread : public Thread {
public:
	BandwidthPerfThread(std::vector<std::string>& conns, size_t size, size_t iter);
	~BandwidthPerfThread();
	void run();
	bool ready() {
		return m_ready;
	}

private:
	bool m_ready = false;
	RDMAClient<ReliableRDMA> m_client;
	void* m_data;
	size_t m_size;
	size_t m_iter;
	std::vector<std::string> m_conns;
	std::vector<NodeID> m_addr;
	size_t* m_remOffsets;
};



class BandwidthPerfTest : public rdma::PerfTest {
public:
	BandwidthPerfTest(int thread_count, uint64_t mem_per_thread, uint64_t iterations);
	virtual ~BandwidthPerfTest();
	std::string getTestParameters();
	void setupTest();
	void runTest();
	std::string getTestResults();

	static mutex waitLock;
	static condition_variable waitCv;
	static bool signaled;

private:
	int thread_count;
	uint64_t mem_per_thread;
	uint64_t iterations;
	std::vector<std::string> m_conns;
	std::vector<BandwidthPerfThread*> m_threads;

	BaseMemory *m_memory;
	RDMAServer<ReliableRDMA>* m_dServer;
};



}
#endif