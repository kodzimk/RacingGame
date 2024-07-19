#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex>& vertices, std::vector<DWORD>& indices)
{
	this->deviceContext = deviceContext;
	size = indices.size();

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(Vertex) * vertices.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();

	device->CreateBuffer(&bd, &sd, &vertexbuffer);

	D3D11_BUFFER_DESC ibd = {};
	ibd.ByteWidth = sizeof(DWORD) * indices.size();
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();

	device->CreateBuffer(&ibd, &isd, &indexbuffer);
}

Mesh::Mesh(const Mesh& mesh)
{
	size = mesh.size;
	indexbuffer = mesh.indexbuffer;
	vertexbuffer = mesh.vertexbuffer;
	deviceContext = mesh.deviceContext;
}

void Mesh::Draw()
{
	const UINT offset = 0;
	const UINT stride = sizeof(Vertex);

	deviceContext->IASetVertexBuffers(0u, 1u, vertexbuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(size, 0u, 0u);
}
