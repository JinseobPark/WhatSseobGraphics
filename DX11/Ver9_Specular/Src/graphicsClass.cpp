////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightclass.h"
#include "LightShaderClass.h"
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
	//D3D 按眉 积己
	m_Direct3D = new D3DClass;
	if (!m_Direct3D)
	{
		return false;
	}

	//initialize
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd,
		FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Direct3D 按眉积己 坷幅", L"Error", MB_OK);
		return false;
	}

	//camera create
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	m_Camera->SetPosition(0.0f, 0.0f, -6.0f);


	//model create
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	//model initialize
	if (!m_Model->Initialize(m_Direct3D->GetDevice(), "data/cube.txt", L"data/seafloor.dds"))
	{
		MessageBox(hwnd, L"fail initialize the model object.", L"Error", MB_OK);
		return false;
	}

	//lightshader create
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// colorshader initialize
	if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"fail initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	//light create
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	m_Light->SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// m_Light 按眉 馆券
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// m_LightShader 按眉 馆券
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// m_Model 按眉 馆券
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	// m_Camera 按眉 馆券
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	//D3D 按眉 馆券
	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = NULL;
	}
	return;
}


bool GraphicsClass::Frame()
{
	static float rotation = 0.0f;

	//update rotation
	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	return Render(rotation);
}


bool GraphicsClass::Render(float rotation)
{
	//硅版 檬扁拳
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 墨皋扼狼 困摹俊 蝶扼 轰 青纺阑 积己钦聪促
	m_Camera->Render();

	// 墨皋扼 棺 d3d 按眉俊辑 岿靛, 轰 棺 捧康 青纺阑 啊廉可聪促
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixRotationY(rotation);
	// 葛胆 滚咆胶客 牢郸胶 滚欺甫 弊贰侨 颇捞橇 扼牢俊 硅摹窍咯 靛肺雷阑 霖厚钦聪促.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// 祸惑 溅捞歹甫 荤侩窍咯 葛胆阑 坊歹傅钦聪促.
	if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower()))
	{
		return false;
	}

	//拳搁 免仿
	m_Direct3D->EndScene();
	return true;
}
