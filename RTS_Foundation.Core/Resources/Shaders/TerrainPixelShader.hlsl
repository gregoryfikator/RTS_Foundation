// [0] - sand
// [1] - sand with grass
// [2] - grass
// [3] - grass with rock
// [4] - rock
// [5] - rock with snow
// [6] - snow

cbuffer LightBuffer : register(b1)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

cbuffer BiomHeightBuffer : register(b2)
{
    float4 textureBlendHeights[12];
};

Texture2D shaderTextures[7] : register(t0);
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPosition: POSITION; // y - y position in world
};

float4 TerrainPixelShader(PixelInputType input) : SV_TARGET
{
    float4 lowerTextureColor;
    float4 higherTextureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float4 textureColor;

    float minHeight;

    int index = 0;
    int value = 0;

    struct blendingData
    {
        float height;
        float4 result;
    };

    blendingData blending;

    blending.height = input.worldPosition.y;
    blending.result = shaderTextures[0].Sample(SampleType, input.tex);;

    for (int i = 1; i < 7; i++)
    {
        blending.height = max(0, input.worldPosition.y - textureBlendHeights[i].y);
        float t = min(1, blending.height * 0.3);
        blending.result = lerp(blending.result, shaderTextures[i].Sample(SampleType, input.tex) , t);
    }

    color = ambientColor;

    lightDir = -lightDirection;

    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
        color += (diffuseColor * lightIntensity);
    }

    color = saturate(color);

    color = color * blending.result;

    return color;
}