#include "stdafx.h"
#include "TextureClass.h"
#include "FontClass.h"

#include <fstream>
using namespace std;


FontClass::FontClass()
{
}

FontClass::FontClass(const FontClass& other)
{
}

FontClass::~FontClass()
{
}


bool FontClass::Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename)
{
	// Load text file with font
	if (!LoadFontData(fontFilename))
	{
		return false;
	}

	// load texture with font
	return LoadTexture(device, textureFilename);
}

void FontClass::Shutdown()
{
	// Release font texture.
	ReleaseTexture();

	// Release font data.
	ReleaseFontData();
}



bool FontClass::LoadFontData(char* filename)
{
	ifstream fin;
	int i;
	char temp;


	// create font buffer
	m_Font = new FontType[95];
	if (!m_Font)
	{
		return false;
	}

	// open file
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// read 95 ascii characters for text
	for (i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> m_Font[i].left;
		fin >> m_Font[i].right;
		fin >> m_Font[i].size;
	}

	// close the file
	fin.close();

	return true;
}

void FontClass::ReleaseFontData()
{
	// release buffer
	if (m_Font)
	{
		delete[] m_Font;
		m_Font = 0;
	}
}

bool FontClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	// create texture object
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// initialize texture
	return m_Texture->Initialize(device, filename);
}

void FontClass::ReleaseTexture()
{
	// release texture object
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}

ID3D11ShaderResourceView* FontClass::GetTexture()
{
	return m_Texture->GetTexture();
}


void FontClass::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, i, letter;


	// input vertices
	vertexPtr = (VertexType*)vertices;

	// get length of sentence
	numLetters = (int)strlen(sentence);

	// initialize index
	index = 0;

	// draw
	for (i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// 'space' character
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// first triangle in quad
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 1.0f);
			index++;

			// second triangle in quad
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + m_Font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + m_Font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(m_Font[letter].right, 1.0f);
			index++;

			// update draw position
			drawX = drawX + m_Font[letter].size + 1.0f;
		}
	}
}