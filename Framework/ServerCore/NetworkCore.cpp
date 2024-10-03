#include "pch.h"
#include "NetworkCore.h"
#include "ThreadManager.h"
#include "Service.h"

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

	for (int32 i = 0; i < 6; ++i) {
		GThreadManager->RunThreads([this]() {
			CLInfo("io thread start. ThreadId={}", LThreadId);
			m_context.run(); 
			});
	}

	//todo: ��ü ���ǿ� ���� �ϳ��� Ÿ�̸Ӹ� ������ �ִ�. ���� ������ �ʿ��غ���
	for (int32 i = 0; i < 1; ++i) {
		GThreadManager->RunThreads([this]() {
			CLInfo("timer thread start. ThreadId={}", LThreadId);
			m_timer->StartTimer(100);	//timer temp
			});
	}
}
