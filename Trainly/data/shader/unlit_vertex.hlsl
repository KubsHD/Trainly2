struct VS_INPUT
{
    float3 pos : Position;
    float2 texCoord : TexCoord;
    float3 normal : Normal;
};

struct VS_Out
{
	float4 pos : SV_Position;
    float2 texCoord : TexCoord;
};

cbuffer TransformBuffer
{
    matrix transform;
};

VS_Out main(VS_INPUT input)
{
	VS_Out vso;
    vso.pos = mul(float4(input.pos, 1.0f), transform);
    vso.texCoord = input.texCoord;
	return vso;
}