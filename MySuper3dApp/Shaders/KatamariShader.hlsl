cbuffer ConstProjectionBuffer: register(b0)
{
	matrix ProjectionData;
};

Texture2D DiffuseMap : register(t0);
SamplerState Sampler : register(s0);

struct VS_IN
{
	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
};

struct PS_IN
{
	float4 pos : SV_POSITION0;
	float2 tex : TEXCOORD0;
};

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;
	
	output.pos = mul(float4(input.pos.xyz, 1.0f), ProjectionData);
	output.tex = input.tex;
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 col = DiffuseMap.Sample(Sampler, float2(1 - input.tex.x, input.tex.y));
	clip(col.a - 0.01f);
	return col;
}