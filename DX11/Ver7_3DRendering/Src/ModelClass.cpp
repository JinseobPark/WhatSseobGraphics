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


	// 정점 및 인덱스 버퍼를 초기화합니다.
	if (!InitializeBuffers(device))
	{
		return false;
	}

	// 이 모델의 텍스처를 로드합니다.
	return LoadTexture(device, textureFilename);
}


void ModelClass::Shutdown()
{
	// 모델 텍스쳐를 반환합니다.
	ReleaseTexture();
	// 버텍스 및 인덱스 버퍼를 종료합니다.
	ShutdownBuffers();
	// model data release
	ReleaseModel();
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// 그리기를 준비하기 위해 그래픽 파이프 라인에 꼭지점과 인덱스 버퍼를 놓습니다.
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
	// 정점 배열을 만듭니다.
	VertexType* vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// 인덱스 배열을 만듭니다.
	unsigned long* indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// 정점 배열과 인덱스 배열을 데이터로 읽어옵니다.
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	// 정적 정점 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 이제 정점 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
	{
		return false;
	}

	// 정적 인덱스 버퍼의 구조체를 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성합니다.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 생성합니다.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
	{
		return false;
	}

	// 생성되고 값이 할당된 정점 버퍼와 인덱스 버퍼를 해제합니다.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// 인덱스 버퍼를 해제합니다.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// 정점 버퍼를 해제합니다.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	// 정점 버퍼의 단위와 오프셋을 설정합니다.
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	// 렌더링 할 수 있도록 입력 어셈블러에서 정점 버퍼를 활성으로 설정합니다.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// 렌더링 할 수 있도록 입력 어셈블러에서 인덱스 버퍼를 활성으로 설정합니다.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼로 그릴 기본형을 설정합니다. 여기서는 삼각형으로 설정합니다.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	// 텍스처 오브젝트를 생성한다.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// 텍스처 오브젝트를 초기화한다.
	return m_Texture->Initialize(device, filename);
}


void ModelClass::ReleaseTexture()
{
	// 텍스처 오브젝트를 릴리즈한다.
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

	// 모델 파일을 엽니다.
	ifstream fin;
	fin.open(filename);

	// 파일을 열 수 없으면 종료합니다.
	if (fin.fail())
	{
		return false;
	}

	// 버텍스 카운트의 값까지 읽는다.
	char input = 0;
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// 버텍스 카운트를 읽는다.
	fin >> m_vertexCount;

	// 인덱스의 수를 정점 수와 같게 설정합니다.
	m_indexCount = m_vertexCount;

	// 읽어 들인 정점 개수를 사용하여 모델을 만듭니다.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// 데이터의 시작 부분까지 읽는다.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// 버텍스 데이터를 읽습니다.
	for (int i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// 모델 파일을 닫는다.
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
		
	// 파일을 열려고 시도합니다.
	fin.open(filename);

	if (fin.good())
	{
		// 파일이 존재하고 아무런 문제가 없다면 우리는 파일 이름을 가지고 있기 때문에 종료합니다.
	}
	else
	{
		// 파일이 없거나 열 때 문제가 발생하면 사용자에게 알리고 프로세스를 반복합니다.
		fin.clear();
	}

	return;
}


bool ModelClass::ReadFileCounts(char* filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount)
{
	ifstream fin;
	char input;


	// 카운트를 초기화합니다.
	vertexCount = 0;
	textureCount = 0;
	normalCount = 0;
	faceCount = 0;

	// 파일을 엽니 다.
	fin.open(filename);

	// 파일 열기에 성공했는지 확인합니다.
	if (fin.fail() == true)
	{
		return false;
	}

	// 파일에서 읽은 다음 파일의 끝에 도달 할 때까지 계속 읽습니다.
	fin.get(input);
	while (!fin.eof())
	{
		// 라인이 'v'로 시작하면 꼭지점, 텍스처 좌표 또는 법선 벡터를 계산합니다.
		if (input == 'v')
		{
			fin.get(input);
			if (input == ' ') { vertexCount++; }
			if (input == 't') { textureCount++; }
			if (input == 'n') { normalCount++; }
		}

		// 라인이 'f'로 시작하면면 수를 증가시킵니다.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ') { faceCount++; }
		}

		// 그렇지 않으면 나머지 행을 읽습니다.
		while (input != '\n')
		{
			fin.get(input);
		}

		// 다음 줄의 시작 부분을 읽기 시작한다.
		fin.get(input);
	}

	m_vertexCount = vertexCount;
	m_indexCount = m_vertexCount;

	// 파일을 닫습니다.
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


	// 네 개의 데이터 구조를 초기화합니다.
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

	// 인덱스를 초기화합니다.
	vertexIndex = 0;
	texcoordIndex = 0;
	normalIndex = 0;
	faceIndex = 0;

	// 파일을 엽니 다.
	fin.open(filename);

	// 파일 열기에 성공했는지 확인하십시오.
	if (fin.fail() == true)
	{
		return false;
	}

	// 정점, 텍스처 좌표 및 법선을 데이터 구조로 읽어들입니다.
	// 중요 : Maya는 오른손 좌표계를 사용하기 때문에 왼손 좌표계로 변환한다.
	fin.get(input);
	while (!fin.eof())
	{
		if (input == 'v')
		{
			fin.get(input);

			// 정점을 읽습니다.
			if (input == ' ')
			{
				fin >> vertices[vertexIndex].x >> vertices[vertexIndex].y >> vertices[vertexIndex].z;

				// Z 정점을 뒤집어 왼손 시스템으로 바꾼다.
				vertices[vertexIndex].z = vertices[vertexIndex].z * -1.0f;
				vertexIndex++;
			}

			// 텍스처 uv 좌표를 읽습니다.
			if (input == 't')
			{
				fin >> texcoords[texcoordIndex].x >> texcoords[texcoordIndex].y;

				// V 텍스처 좌표를 왼손 시스템으로 반전시킵니다.
				texcoords[texcoordIndex].y = 1.0f - texcoords[texcoordIndex].y;
				texcoordIndex++;
			}

			// 법선을 읽습니다.
			if (input == 'n')
			{
				fin >> normals[normalIndex].x >> normals[normalIndex].y >> normals[normalIndex].z;

				// Z 법선을 반전시켜 왼손 시스템으로 변경합니다.
				normals[normalIndex].z = normals[normalIndex].z * -1.0f;
				normalIndex++;
			}
		}

		// 표면을 읽는다.
		if (input == 'f')
		{
			fin.get(input);
			if (input == ' ')
			{
				// 오른손 시스템에서 왼손 시스템으로 변환하기 위해 표면 데이터를 거꾸로 읽습니다.
				fin >> faces[faceIndex].vIndex3 >> input2 >> faces[faceIndex].tIndex3 >> input2 >> faces[faceIndex].nIndex3
					>> faces[faceIndex].vIndex2 >> input2 >> faces[faceIndex].tIndex2 >> input2 >> faces[faceIndex].nIndex2
					>> faces[faceIndex].vIndex1 >> input2 >> faces[faceIndex].tIndex1 >> input2 >> faces[faceIndex].nIndex1;
				faceIndex++;
			}
		}

		// 나머지 행을 읽습니다.
		while (input != '\n')
		{
			fin.get(input);
		}

		// 다음 줄의 시작 부분을 읽기 시작한다.
		fin.get(input);
	}

	// 파일을 닫습니다.
	fin.close();

	// 출력 파일을 엽니다.
	//fout.open("model.txt");
	std::string output = filename;
	output.replace(output.find(".obj"), 4, ".txt");
	fout.open(output);

	// 모델 형식에서 사용하는 파일 헤더를 작성합니다.
	fout << "Vertex Count: " << (faceCount * 3) << endl;
	fout << endl;
	fout << "Data:" << endl;
	fout << endl;

	// 이제 모든면을 반복하고 각면의 세 꼭지점을 출력합니다.
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

	// 출력 파일을 닫는다.
	fout.close();

	// 네 개의 데이터 변수를 해제한다.
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


	// 모델 파일의 이름을 읽습니다.
	GetModelFilename(filename);

	// 필요한 정확한 크기로 데이터 구조를 초기화 할 수 있도록 꼭지점 수, 텍셀 좌표, 법선 및면 수를 읽습니다.
	result = ReadFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}

	// 정보를 얻기 위해 화면에 카운트를 표시합니다.
	cout << endl;
	cout << "Vertices: " << vertexCount << endl;
	cout << "UVs:      " << textureCount << endl;
	cout << "Normals:  " << normalCount << endl;
	cout << "Faces:    " << faceCount << endl;

	// 이제 파일의 데이터를 데이터 구조로 읽어 들인 다음 모델 형식으로 출력합니다.
	result = LoadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return -1;
	}
}
