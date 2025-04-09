#include "../pch.h"

NetworkCore::NetworkCore()
{
}

NetworkCore::~NetworkCore()
{
	m_context.stop();
}

void NetworkCore::RunObject(std::shared_ptr<Service> _service, std::shared_ptr<Timer> _timer)
{
	_service->ServiceStart(m_context);
	m_timer = std::move(_timer);
	m_timer->SetService(_service.get());

	//todo: 스레드 개수 임의 지정
	for (int32 i = 0; i < 3; ++i) {
		GThreadManager->RunThreads([this]() {
			CLInfo("io thread start. ThreadId={}", LThreadId);
			m_context.run(); 
			});
	}

	for (int32 i = 0; i < 1; ++i) {
		GThreadManager->RunThreads([this]() {
			CLInfo("worker thread start. ThreadId={}", LThreadId);
			GPacketQueue->ProcessJob();
			});
	}

	//todo: 전체 세션에 대해 하나의 타이머만 돌리고 있다. 성능 개선이 필요해보임
	for (int32 i = 0; i < 1; ++i) {
		GThreadManager->RunThreads([this]() {
			CLInfo("timer thread start. ThreadId={}", LThreadId);
			m_timer->StartTimer(100);	//timer temp
			});
	}
}
