#pragma pack_matrix(row_major)

struct TVertexInput
{
	float4 m_f4Position : POSITION;
	float4 m_f4Normals : NORMALS;
	float4 m_f4TexCoords : TEXCOORDS;
};

struct TVertexOutput
{
	float4 m_f4Position : SV_POSITION;
	float4 m_f4Normals : NORMALS;
	float4 m_f4TexCoords : TEXCOORDS;
};

cbuffer bObjectBuffer : register(b0)
{
	float4x4 mWorldMatrix;
}
cbuffer bSceneBuffer : register(b1)
{
	float4x4 mViewMatrix;
	float4x4 mProjMatrix;
}

TVertexOutput main(TVertexInput tInput)
{
	//set local position
	float4 f4LocalPos = float4(tInput.m_f4Position.xyz, 1.0f);

	//perspective divide
	f4LocalPos = mul(f4LocalPos, mWorldMatrix);
	f4LocalPos = mul(f4LocalPos, mViewMatrix);
	f4LocalPos = mul(f4LocalPos, mProjMatrix);

	//set output variables
	TVertexOutput tOutput;
	tOutput.m_f4Position = f4LocalPos;
	tOutput.m_f4Normals.xyz = mul(tInput.m_f4Normals.xyz, (float3x3)mWorldMatrix);
	tOutput.m_f4TexCoords = tInput.m_f4TexCoords;

	return tOutput;
}