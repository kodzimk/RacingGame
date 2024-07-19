struct VSOut
{
    float2 TexCoord : TEXCOORD;
    float Alpha : ALPHA;
    float4 position : SV_Position;
};

cbuffer CBuf
{
     matrix camera;
};


VSOut main(float3 pos : Pos , float2 TexCoord : TEXCOORD,float Alpha : ALPHA)
{
    VSOut vs;
    vs.TexCoord = TexCoord;
    vs.position = mul(float4(pos, 1.0f), camera);
    vs.Alpha = Alpha;
    return vs;
}