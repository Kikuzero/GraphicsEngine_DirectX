cbuffer ConstMatrixBuffer: register(b0)
{
	matrix World;
	matrix ProjectionData;
	matrix InverseTransposeWorld;
};

cbuffer MaterialBuffer : register(b1)
{
	float Ambient;
	float Shininess;
	float Specular;
	float _0;
}

cbuffer LightBuffer : register(b2)
{
	float3 ViewPosition;
	float _1;
	float3 LightDirection;
	float _2;
	float3 LightColor;
	float _3;
}

struct VS_IN
{
	float3 pos : POSITION;
	float3 norm : NORMAL;
};

struct PS_IN
{
	float4 pos : SV_POSITION0;
	float4 norm : NORMAL;
	float4 col : COLOR;
	float4 worldPos : TEXCOORD1;
};

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;
	
	output.worldPos = mul(float4(input.pos.xyz, 1.0f), World);
	output.pos = mul(float4(input.pos.xyz, 1.0f), ProjectionData);
	output.norm = mul(float4(input.norm.xyz, 1.f), InverseTransposeWorld);
	output.col = float4(1.f, 1.f, 1.f, 1.f);
	
	return output;
}

float4 PSMain( PS_IN input ) : SV_Target
{
	float4 color = input.col;

	float3 ambient = Ambient * LightColor;

	float3 normal = normalize(input.norm.xyz);
	float3 toLightDirection = -LightDirection;
	float diff = max(0.0, dot(toLightDirection, normal));
	float3 diffuse = diff * LightColor;

	float3 viewDirection = normalize(ViewPosition - input.worldPos.xyz);
	float3 reflectDirection = normalize(reflect(LightDirection, normal));
	float spec = pow(max(0.0, dot(viewDirection, reflectDirection)), Shininess);
	float3 specular = Specular * spec * LightColor;

	float4 result = float4((ambient + diffuse + specular) * color.xyz, 1.f);
	return result;
}