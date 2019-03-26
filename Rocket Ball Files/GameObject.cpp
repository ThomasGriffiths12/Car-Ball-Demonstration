#include "GameObject.h"

GameObject::GameObject(void)
{
}

GameObject::~GameObject(void)
{
}

void GameObject::Initialise(MeshData meshData)
{
	_meshData = meshData;

	XMStoreFloat4x4(&_world, XMMatrixIdentity());
	XMStoreFloat4x4(&_scale, XMMatrixIdentity());
	XMStoreFloat4x4(&_rotate, XMMatrixIdentity());
	XMStoreFloat4x4(&_translate, XMMatrixIdentity());
}

void GameObject::SetScale(float x, float y, float z)
{
	XMStoreFloat4x4(&_scale, XMMatrixScaling(x, y, z));
}

void GameObject::SetRotation(float x, float y, float z)
{
	XMStoreFloat4x4(&_rotate, XMMatrixRotationX(x) * XMMatrixRotationY(y) * XMMatrixRotationZ(z));
}

void GameObject::SetTranslation(float x, float y, float z)
{
	XMStoreFloat4x4(&_translate, XMMatrixTranslation(x, y, z));
}

void GameObject::UpdateWorld()
{
	XMMATRIX scale = XMLoadFloat4x4(&_scale);
	XMMATRIX rotate = XMLoadFloat4x4(&_rotate);
	XMMATRIX translate = XMLoadFloat4x4(&_translate);

	XMStoreFloat4x4(&_world, scale * rotate * translate);
}
void GameObject::SetPos(XMFLOAT3 pos)
{
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
}

void GameObject::Turn(float yaw)
{
	rotation.y += XMConvertToRadians(yaw);
	XMStoreFloat4x4(&_rotate, XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z));
	UpdateWorld();
}

void GameObject::Move(float speed)
{
	position.x += (speed * sin(rotation.y));
	position.z += (speed * cos(rotation.y));

	XMStoreFloat4x4(&_translate, XMMatrixTranslation(position.x, position.y, position.z));
	UpdateWorld();

}
void GameObject::Update(float elapsedTime)
{
	// TODO: Add GameObject logic 
}

void GameObject::Draw(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pImmediateContext)
{
	// NOTE: We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_meshData.VertexBuffer, &_meshData.VBStride, &_meshData.VBOffset);
	pImmediateContext->IASetIndexBuffer(_meshData.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_meshData.IndexCount, 0, 0);   
}