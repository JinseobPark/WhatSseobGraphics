#include "stdafx.h"
#include "TextureClass.h"
#include "BitmapClass.h"



BitmapClass::BitmapClass()
{
}


BitmapClass::BitmapClass(const BitmapClass& other)
{
}


BitmapClass::~BitmapClass()
{
}


bool BitmapClass::Initialize(ID3D11Device* device, int screenWidth, int screenHeight, WCHAR* textureFilename, int bitmapWidth, int bitmapHeight)
{
	// ȭ�� ũ��
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// ������ ���
	m_bitmapWidth = bitmapWidth;
	m_bitmapHeight = bitmapHeight;

	// ������ ��ġ
	m_previousPosX = -1;
	m_previousPosY = -1;

	// ���� �ʱ�ȭ
	if (!InitializeBuffers(device))
	{
		return false;
	}

	// �ؽ�ó �ε�
	return LoadTexture(device, textureFilename);
}


void BitmapClass::Shutdown()
{
	//�ؽ�ó��ȯ
	ReleaseTexture();
	//���� ��ȯ
	ShutdownBuffers();
}


bool BitmapClass::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	// ���� ��ġ ������Ʈ
	if (!UpdateBuffers(deviceContext, positionX, positionY))
	{
		return false;
	}

	//  ���� ������
	RenderBuffers(deviceContext);

	return true;
}


int BitmapClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* BitmapClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool BitmapClass::InitializeBuffers(ID3D11Device* device)
{
	// vertex ���� ���� �簢���̹Ƿ� 6��
	m_indexCount = m_vertexCount = 6;

	// create vertex array
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// �迭 0���� �ʱ�ȭ
	memset(vertices, 0, (sizeof(VertexType) * m_vertexCount));

	// create index array
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// load index array 
	for (int i = 0; i < m_indexCount; i++)
	{
		indices[i] = i;
	}

	// vertex ���۱��� ���� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// vertex buffer data pointer
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// create vertex buffer
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// index ���۱��� ����, ����
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// index buffer data pointer
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// create index buffer
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// �����ǰ� ���� �Ҵ�� ���� ���ۿ� �ε��� ���۸� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void BitmapClass::ShutdownBuffers()
{
	// index buffer
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// vertex buffer
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

bool BitmapClass::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	float left, right, top, bottom;
	VertexType* vertices;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;
	HRESULT result;


	// �� ��Ʈ���� ������ �� ��ġ�� ������� ���� ��� ���� ���۸� ������Ʈ ���� ���ʽÿ�.
	// ���� �ùٸ� �Ű� ������ �ֽ��ϴ�.
	if ((positionX == m_previousPosX) && (positionY == m_previousPosY))
	{
		return true;
	}

	// ����� ��� �������Ǵ� ��ġ�� ������Ʈ�մϴ�.
	m_previousPosX = positionX;
	m_previousPosY = positionY;

	// ��Ʈ �� ������ ȭ�� ��ǥ�� ����մϴ�.
	left = (float)((m_screenWidth / 2) * -1) + (float)positionX;

	// ��Ʈ �� �������� ȭ�� ��ǥ�� ����մϴ�.
	right = left + (float)m_bitmapWidth;

	// ��Ʈ �� ����� ȭ�� ��ǥ�� ����մϴ�.
	top = (float)(m_screenHeight / 2) - (float)positionY;

	// ��Ʈ �� �Ʒ����� ȭ�� ��ǥ�� ����մϴ�.
	bottom = top - (float)m_bitmapHeight;

	// ���� �迭�� ����ϴ�.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// ���� �迭�� �����͸��ε��մϴ�.
	// ù ��° �ﰢ��
	vertices[0].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);  // Bottom left.
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	// �� ��° �ﰢ��
	vertices[3].position = XMFLOAT3(left, top, 0.0f);  // Top left.
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);  // Top right.
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);  // Bottom right.
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	// ���ؽ� ���۸� �� �� �ֵ��� ��޴ϴ�.
	result = deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	verticesPtr = (VertexType*)mappedResource.pData;

	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * m_vertexCount));

	// ���� ������ ����� �����մϴ�.
	deviceContext->Unmap(m_vertexBuffer, 0);

	// �� �̻� �ʿ����� ���� ������ �迭�� �����մϴ�.
	delete[] vertices;
	vertices = 0;

	return true;
}

void BitmapClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ������ ������ �������� �����մϴ�.
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	// ������ �� �� �ֵ��� �Է� ����������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ����������� �ε��� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool BitmapClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	// �ؽ�ó ������Ʈ�� �����Ѵ�.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// �ؽ�ó ������Ʈ�� �ʱ�ȭ�Ѵ�.
	return m_Texture->Initialize(device, filename);
}


void BitmapClass::ReleaseTexture()
{
	// �ؽ�ó ������Ʈ�� �������Ѵ�.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}