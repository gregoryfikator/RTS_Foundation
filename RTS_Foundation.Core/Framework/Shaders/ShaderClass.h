#pragma once

#ifndef _SHADERCLASS_H_
#define _SHADERCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>

class ShaderClass
{
protected:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

public:
	ShaderClass();
	~ShaderClass();

	virtual bool Initialize(ID3D11Device*, HWND) = 0;
	virtual void Shutdown();

protected:
	virtual bool InitializeShader(ID3D11Device*, HWND, const WCHAR*, const WCHAR*) = 0;
	virtual void RenderShader(ID3D11DeviceContext*, int) = 0;
	virtual void ShutdownShader();

	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

protected:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif