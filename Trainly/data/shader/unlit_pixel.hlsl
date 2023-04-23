struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TexCoord;
};

Texture2D objTex : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 pxColor = objTex.Sample(objSamplerState, input.texCoord);
    return float4(pxColor, 1.0f);
}