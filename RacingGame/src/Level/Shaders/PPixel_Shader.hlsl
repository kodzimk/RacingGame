float4 main(float2 TexCoord : TEXCOORD,float Alpha : ALPHA) : SV_Target
{
    return float4(1.0f,0.0f,0.0f, Alpha);
}