#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "Camera.h"
#include "Structures.h"
#include "OBJLoader.h"
#include "GameObject.h"
using namespace DirectX;

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;

	//float gTime;

	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 DiffuseLight;
	XMFLOAT3 LightVecW;
	float pad;

	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 AmbientLight;

	XMFLOAT4 SpecularMtrl;
	XMFLOAT4 SpecularLight;
	float SpecularPower;
	XMFLOAT3 EyePosW; 	// Camera position in world space

};

class Application
{
private:
	HINSTANCE               _hInst;
	HWND                    _hWnd;
	D3D_DRIVER_TYPE         _driverType;
	D3D_FEATURE_LEVEL       _featureLevel;
	ID3D11Device*           _pd3dDevice;
	ID3D11DeviceContext*    _pImmediateContext;
	IDXGISwapChain*         _pSwapChain;
	ID3D11RenderTargetView* _pRenderTargetView;
	ID3D11VertexShader*     _pVertexShader;
	ID3D11PixelShader*      _pPixelShader;
	ID3D11InputLayout*      _pVertexLayout;
	ID3D11ShaderResourceView * _pTextureRV;
	ID3D11ShaderResourceView * _chainTextRV;
	ID3D11ShaderResourceView * _woodTextRV;
	ID3D11ShaderResourceView * _ballTextRV;
	ID3D11ShaderResourceView * _pTextureSpecRV;
	ID3D11SamplerState * _pSamplerLinear;

	ID3D11Buffer*           _pVertexBuffer;
	ID3D11Buffer*           _pIndexBuffer;

	ID3D11Buffer*           _pVertexBuffer2;
	ID3D11Buffer*           _pIndexBuffer2;

	MeshData carObjMeshData;
	GameObject* car;
	MeshData floorObjMeshData;
	GameObject* floor;
	GameObject* cage;
	GameObject* ball;
	MeshData cageObjMeshData;
	MeshData ballObjMeshData;

	ID3D11Buffer*           _pConstantBuffer;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11RasterizerState* _wireFrame;
	ID3D11RasterizerState* _solidShape;
	XMFLOAT4X4              _world;
	XMFLOAT4X4              _world2;

	//XMFLOAT4X4              _view;
	//XMFLOAT4X4              _projection;
	
	Camera *				_camera;
	Camera *				_camera2;
	Camera *				_camera3;
	Camera *				_mainCamera;

	float					gTime;

	XMFLOAT3 lightDirection;
	XMFLOAT4 diffuseMaterial;
	XMFLOAT4 diffuseLight;

	XMFLOAT4 ambientMaterial;
	XMFLOAT4 ambientLight;

	XMFLOAT4 SpecularMaterial;
	XMFLOAT4 SpecularLight;
	float SpecularPower;
	XMFLOAT3 EyePosW;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	HRESULT InitVertexBuffer2();
	HRESULT InitIndexBuffer2();
	UINT _WindowHeight;
	UINT _WindowWidth;


public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};

