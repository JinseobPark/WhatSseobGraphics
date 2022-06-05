////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	//D3D ��ü ����
	m_Direct3D = (D3DClass*)_aligned_malloc(sizeof(D3DClass), 16);
	if (!m_Direct3D)
	{
		return false;
	}

	//initialize
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd,
		FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Direct3D ��ü���� ����", L"Error", MB_OK);
		return false;
	}

	//camera create
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	m_Camera->SetPosition(0.0f, 0.0f, -15.0f);


	//model create
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	//model initialize
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "data/stone01.tga"))
	{
		MessageBox(hwnd, L"fail initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//textureshader create
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// colorshader initialize
	if (!m_TextureShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"fail initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// m_ColorShader ��ü ��ȯ
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// m_Model ��ü ��ȯ
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// m_Camera ��ü ��ȯ
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//D3D ��ü ��ȯ
	if (!m_Direct3D)
	{
		m_Direct3D->Shutdown();
		_aligned_free(m_Direct3D);
		m_Direct3D = NULL;
	}
	return;
}


bool GraphicsClass::Frame()
{

	return Render();
}


bool GraphicsClass::Render()
{
	//��� �ʱ�ȭ
	m_Direct3D->BeginScene(0.0f, 0.9f, 0.5f, 1.0f);

	// ī�޶��� ��ġ�� ���� �� ����� �����մϴ�
	m_Camera->Render();

	// ī�޶� �� d3d ��ü���� ����, �� �� ���� ����� �����ɴϴ�
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// �� ���ؽ��� �ε��� ���۸� �׷��� ������ ���ο� ��ġ�Ͽ� ������� �غ��մϴ�.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// ���� ���̴��� ����Ͽ� ���� �������մϴ�.
	if (!m_TextureShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture()))
	{
		return false;
	}

	//ȭ�� ���
	m_Direct3D->EndScene();
	return true;
}
