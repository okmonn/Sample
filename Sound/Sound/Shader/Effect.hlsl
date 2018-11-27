// ルートシグネチャの宣言
#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(CBV(b0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "DescriptorTable(UAV(u1, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
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

// 適応前データ
RWStructuredBuffer<float> origin : register(u0);
// 適応データ
RWStructuredBuffer<float> real : register(u1);

#define PI 3.14159265f

// ディレイ
void Delay(uint index)
{
    real[index] = origin[index];

    for (int i = 1; i <= loop; ++i)
    {
        int m = (int) (index - i * (sample * time));

        real[index] += (m >= 0) ? pow(attenuation, i) * origin[index] : 0.0f;
    }
}

// ディストーション
void Distortion(uint index)
{
    //増幅率
    float gain = 10.0f;
    //音量レベル
    float level = 0.5f;

    real[index] = origin[index] * gain;
    
    //クリッピング
    if(real[index] >= 0.0f)
    {
        real[index] = atan(real[index]) / (PI / 2.0f);
    }
    else if(real[index] < -0.0f)
    {
        real[index] = atan(real[index]) / (PI / 2.0f) * 0.1f;
    }

    //音量調節
    real[index] *= level;
}

// コンプレッサ
void Compressor(uint index)
{
    //しきい値
    float threshold = 0.2f;
    //レシオ
    float ratio = 1.0f / 10.0f;
    //増幅率
    float gain = 1.0f / (threshold + (1.0f - threshold) * ratio);

    real[index] = origin[index];

    //振幅の圧縮
    if (real[index] > threshold)
    {
        real[index] = threshold + (real[index] - threshold) * ratio;
    }
    else if (real[index] < -threshold)
    {
        real[index] = -threshold + (real[index] + threshold) * ratio;
    }
    
    real[index] *= gain;
}

// リミッタ
void Limiter(uint index)
{
    //しきい値
    float threshold = 0.2f;
    //レシオ
    float ratio = 1.0f / 10.0f;
    //増幅率
    float gain = 1.0f / (threshold + (1.0f - threshold) * ratio);

    real[index] = origin[index];

    //振幅の圧縮
    if (real[index] > threshold)
    {
        real[index] = threshold;
    }
    else if (real[index] < -threshold)
    {
        real[index] = -threshold;
    }
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    AllMemoryBarrierWithGroupSync();
}