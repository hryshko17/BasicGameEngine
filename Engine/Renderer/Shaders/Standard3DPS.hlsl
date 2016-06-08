
texture2D tBaseTexture : register(t0);

SamplerState sSampler : register(s0);

struct TPixelInput
{
	float4 m_f4Position : SV_POSITION;
	float4 m_f4Normals : NORMALS;
	float4 m_f4TexCoords : TEXCOORDS;
};

float4 main(TPixelInput tInput) : SV_TARGET
{
	float4 f4Diffuse = tBaseTexture.Sample(sSampler, tInput.m_f4TexCoords.xy);
	return f4Diffuse;
}