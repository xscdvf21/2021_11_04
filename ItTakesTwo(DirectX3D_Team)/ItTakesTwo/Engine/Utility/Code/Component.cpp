#include "Component.h"

USING(Engine)

Engine::CComponent::CComponent(void)
{

}

Engine::CComponent::~CComponent(void)
{

}

Engine::_int Engine::CComponent::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

Engine::_int Engine::CComponent::LateUpdate_Component(const _float& fTimeDelta)
{
	return 0;
}
