// ルートシグネチャの宣言
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "StaticSampler(s0, "\
                                  "filter = FILTER_MIN_MAG_MIP_LINEAR, "\
                                  "addressU = TEXTURE_ADDRESS_WRAP, "\
                                  "addressV = TEXTURE_ADDRESS_WRAP, "\
                                  "addressW = TEXTURE_ADDRESS_WRAP, "\
                                  "mipLodBias = 0.0f, "\
                                  "maxAnisotropy = 0, "\
                                  "comparisonFunc = COMPARISON_NEVER, "\
                                  "borderColor = STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "\
                                  "minLOD = 0.0f, "\
                                  "maxLOD = 3.402823466e+38f, "\
                                  "space = 0, "\
                                  "visibility = SHADER_VISIBILITY_ALL)"

// パラメータ
cbuffer Param : register(b0)
{
    //減衰率
    float attenuation;
    //遅延時間
    float time;
    //ループ回数
    int loop;
    //サンプリング周波数
    int sample;
};

// 適応データ
RWStructuredBuffer<float> real : register(u0);

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
   //バックアップ
    float back = real[gID.x];

    for (int n = 1; n <= loop; ++n)
    {
        float m = gID.x - n * (sample * time);
        real[gID.x] += (m >= 0.0f) ? pow(2.0f, n) * back : 0.0f;
    }

    //クリッピング
    if (real[gID.x] > 1.0f)
    {
        real[gID.x] = 1.0f;
    }

    GroupMemoryBarrierWithGroupSync();
}