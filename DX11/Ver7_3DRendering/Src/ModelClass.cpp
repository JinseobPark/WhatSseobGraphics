#include "stdafx.h"
#include "TextureClass.h"
#include "modelclass.h"

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

ModelClass::ModelClass()
{
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename,  WCHAR* textureFilename)
{
	//if (!LoadMyModel(modelFilename))
	//{
	//	return false;
	//}
	//ChangeObjToTxt(modelFilename);
	//if (!ChangeObjToTxt(modelFilename))
	//{
	//	return false;
	//}

	//load model
	if (!LoadModel(modelFilename))
	{
		return false;
	}


	// ���� �� �ε��� ���۸� �ʱ�ȭ�մϴ�.
	if (!InitializeBuffers(device))
	{
		return false;
	}

	// �� ���� �ؽ�ó�� �ε��մϴ�.
	return LoadTexture(device, textureFilename);
}


void ModelClass::Shutdown()
{
	// �� �ؽ��ĸ� ��ȯ�մϴ�.
	ReleaseTexture();
	// ���ؽ� �� �ε��� ���۸� �����մϴ�.
	ShutdownBuffers();
	// model data release
	ReleaseModel();
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// �׸��⸦ �غ��ϱ� ���� �׷��� ������ ���ο� �������� �ε��� ���۸� �����ϴ�.
	RenderBuffers(deviceContext);
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	// ���� �迭�� ����ϴ�.
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// �ε��� �迭�� ����ϴ�.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// ���� �迭�� �ε��� �迭�� �����ͷ� �о�ɴϴ�.
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// ���� ���� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ���� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// ���� �ε��� ������ ����ü�� �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ��մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� �����մϴ�.
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


void ModelClass::ShutdownBuffers()
{
	// �ε��� ���۸� �����մϴ�.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// ���� ���۸� �����մϴ�.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// ���� ������ ������ �������� �����մϴ�.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ �����մϴ�.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻���� �����մϴ�. ���⼭�� �ﰢ������ �����մϴ�.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
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


void ModelClass::ReleaseTexture()
{
	// �ؽ�ó ������Ʈ�� �������Ѵ�.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}

bool ModelClass::LoadModel(char* filename)
{

	//std::string output = filename;
	//output.replace(output.find(".obj"), 4, ".txt");
	//std::vector<char> writable(output.begin(), output.end());
	//writable.push_back('\0');
	//filename = &*writable.begin();

	// �� ������ ���ϴ�.
	ifstream fin;
	fin.open(filename);

	// ������ �� �� ������ �����մϴ�.
	if (fin.fail())
	{
		return false;
	}

	// ���ؽ� ī��Ʈ�� ������ �д´�.
	char input = 0;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// ���ؽ� ī��Ʈ�� �д´�.
	fin >> m_vertexCount;

	// �ε����� ���� ���� ���� ���� �����մϴ�.
	m_indexCount = m_vertexCount;

	// �о� ���� ���� ������ ����Ͽ� ���� ����ϴ�.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// �������� ���� �κб��� �д´�.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// ���ؽ� �����͸� �н��ϴ�.
	for (int i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// �� ������ �ݴ´�.
	fin.close();

	return true;
}


void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}


void ModelClass::GetModelFilename(char* filename)
{
	ifstream fin;
		
	// ������ ������ �õ��մϴ�.
	fin.open(filename);

	if (fin.good())
	{
		// ������ �����ϰ� �ƹ��� ������ ���ٸ� �츮�� ���� �̸��� ������ �ֱ� ������ �����մϴ�.
	}
	else
	{
		// ������ ���ų� �� �� ������ �߻��ϸ� ����ڿ��� �˸��� ���μ����� �ݺ��մϴ�.
		fin.clear();
	}

	return;
}


bool ModelClass::ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;


	// ī��Ʈ�� �ʱ�ȭ�մϴ�.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// ������ ���� ��.
	fin.open(filename);

	// ���� ���⿡ �����ߴ��� Ȯ���մϴ�.
	if (fin.fail() == true)
	{
		return false;
	}

	// ���Ͽ��� ���� ���� ������ ���� ���� �� ������ ��� �н��ϴ�.
	fin.get(input);
	while (!fin.eof())
	{
		// ������ 'v'�� �����ϸ� ������, �ؽ�ó ��ǥ �Ǵ� ���� ���͸� ����մϴ�.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// ������ 'f'�� �����ϸ�� ���� ������ŵ�ϴ�.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// �׷��� ������ ������ ���� �н��ϴ�.
		while (input != '\n')
		{
			fin.get(input);
		}

		// ���� ���� ���� �κ��� �б� �����Ѵ�.
		fin.get(input);
	}

	m_vertexCount = vertexCount;
	m_indexCount = m_vertexCount;

	// ������ �ݽ��ϴ�.
	fin.close();
	return true;
}


bool ModelClass::LoadDataStructures(char* filename, int vertexCount, int textureCount, int normalCount, int faceCount)
{
	VertexType2* vertices, * texcoords, * normals;
	FaceType2* faces;
	ifstream fin;
	int vertexIndex, texcoordIndex, normalIndex, faceIndex, vIndex, tIndex, nIndex;
	char input, input2;
	ofstream fout;


	// �� ���� ������ ������ �ʱ�ȭ�մϴ�.
	vertices = new VertexType2[vertexCount];
	if (!vertices)
	{
		return false;
	}

	texcoords = new VertexType2[textureCount];
	if (!texcoords)
	{
		return false;
	}

	normals = new VertexType2[normalCount];
	if (!normals)
	{
		return false;
	}

	faces = new FaceType2[faceCount];
	if (!faces)
	{
		return false;
	}

	// �ε����� �ʱ�ȭ�մϴ�.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// ������ ���� ��.
	fin.open(filename);

	// ���� ���⿡ �����ߴ��� Ȯ���Ͻʽÿ�.
	if (fin.fail() == true)
	{
		return false;
	}

	// ����, �ؽ�ó ��ǥ �� ������ ������ ������ �о���Դϴ�.
	// �߿� : Maya�� ������ ��ǥ�踦 ����ϱ� ������ �޼� ��ǥ��� ��ȯ�Ѵ�.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// ������ �н��ϴ�.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Z ������ ������ �޼� �ý������� �ٲ۴�.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// �ؽ�ó uv ��ǥ�� �н��ϴ�.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// V �ؽ�ó ��ǥ�� �޼� �ý������� ������ŵ�ϴ�.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// ������ �н��ϴ�.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Z ������ �������� �޼� �ý������� �����մϴ�.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// ǥ���� �д´�.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// ������ �ý��ۿ��� �޼� �ý������� ��ȯ�ϱ� ���� ǥ�� �����͸� �Ųٷ� �н��ϴ�.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// ������ ���� �н��ϴ�.
		while (input != '\n')
		{
			fin.get(input);
		}

		// ���� ���� ���� �κ��� �б� �����Ѵ�.
		fin.get(input);
	}

	// ������ �ݽ��ϴ�.
	fin.close();

	// ��� ������ ���ϴ�.
	//fout.open("model.txt");
	std::string output = filename;
	output.replace(output.find(".obj"), 4, ".txt");
	fout.open(output);

	// �� ���Ŀ��� ����ϴ� ���� ����� �ۼ��մϴ�.
	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// ���� ������ �ݺ��ϰ� ������ �� �������� ����մϴ�.
	for (int i = 0; i < faceIndex; i++)
	{
		vIndex = faces[i].vIndex1 - 1;
		tIndex = faces[i].tIndex1 - 1;
		nIndex = faces[i].nIndex1 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex2 - 1;
		tIndex = faces[i].tIndex2 - 1;
		nIndex = faces[i].nIndex2 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;

		vIndex = faces[i].vIndex3 - 1;
		tIndex = faces[i].tIndex3 - 1;
		nIndex = faces[i].nIndex3 - 1;

		fout << vertices[vIndex].x << ' ' << vertices[vIndex].y << ' ' << vertices[vIndex].z << ' '
			<< texcoords[tIndex].x << ' ' << texcoords[tIndex].y << ' '
			<< normals[nIndex].x << ' ' << normals[nIndex].y << ' ' << normals[nIndex].z << endl;
	}

	// ��� ������ �ݴ´�.
	fout.close();

	// �� ���� ������ ������ �����Ѵ�.
	if (vertices)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (texcoords)
	{
		delete[] texcoords;
		texcoords = 0;
	}
	if (normals)
	{
		delete[] normals;
		normals = 0;
	}
	if (faces)
	{
		delete[] faces;
		faces = 0;
	}

	return true;
}


bool ModelClass::ChangeObjToTxt(char* filename)
{
	std::string output = filename;
	output.replace(output.find(".obj"), 4, ".txt");
	std::vector<char> writable(output.begin(), output.end());
	writable.push_back('\0');
	filename = &*writable.begin();
	
	return true;
}

bool ModelClass::LoadMyModel(char* filename)
{
	bool result;
	int vertexCount, textureCount, normalCount, faceCount;


	// �� ������ �̸��� �н��ϴ�.
	GetModelFilename(filename);

	// �ʿ��� ��Ȯ�� ũ��� ������ ������ �ʱ�ȭ �� �� �ֵ��� ������ ��, �ؼ� ��ǥ, ���� �׸� ���� �н��ϴ�.
	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	// ������ ��� ���� ȭ�鿡 ī��Ʈ�� ǥ���մϴ�.
	cout << endl;
	cout << "Vertices: " << vertexCount << endl;
	cout << "UVs:      " << textureCount << endl;
	cout << "Normals:  " << normalCount << endl;
	cout << "Faces:    " << faceCount << endl;

	// ���� ������ �����͸� ������ ������ �о� ���� ���� �� �������� ����մϴ�.
	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}
}
