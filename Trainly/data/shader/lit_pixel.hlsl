struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 normal : Normal;
    float2 texCoord : TexCoord;
};

Texture2D objTex : TEXTURE: register(t0);
SamplerState objSamplerState : SAMPLER: register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{

    float3 d = { 1.0f,1.0f,1.0f };

    float3 diffuse = 1 * max(dot(normalize(input.normal), d), float3(0.0f, 0.0f, 0.0f));

    float3 pxColor = objTex.Sample(objSamplerState, input.texCoord);
    float3 result = pxColor * diffuse;

    float intensity = dot(normalize(d), input.normal);

    float4 color2;

    if (intensity > 0.95f)      color2 = float4(1.0f, 1.0f, 1.0f, 1.0f);
    else if (intensity > 0.75f) color2 = float4(0.8f, 0.8f, 0.8f, 1.0f);
    else if (intensity > 0.50f) color2 = float4(0.6f, 0.6f, 0.6f, 1.0f);
    else if (intensity > 0.25f) color2 = float4(0.4f, 0.4f, 0.4f, 1.0f);
    else                       color2 = float4(0.2f, 0.2f, 0.2f, 1.0f);

    return float4(pxColor * color2 * 2, 1.0f);
}