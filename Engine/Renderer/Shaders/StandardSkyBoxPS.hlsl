TextureCube tBaseTexture : register(t0);

SamplerState sSampler : register(s0);

struct TPixelInput
{
	float4 m_f4Position : SV_POSITION;
	float4 m_f4Normals : NORMALS;
	float4 m_f4TexCoords : TEXCOORDS;
};

float4 main(TPixelInput tInput) : SV_TARGET
{
	float4 f4BaseColor = tBaseTexture.Sample(sSampler, tInput.m_f4Position.xyz);
	return f4BaseColor;
}
