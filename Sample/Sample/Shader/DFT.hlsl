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

// 波形データ
float wave[1764] : register(b0);

// 実部
RWStructuredBuffer<float> real : register(u0);
// 虚部
//RWStructuredBuffer<float> imag : register(u1);

// 円周率
#define PI 3.14159265f

// データサイズ
#define SIZE 1764

// ハニング窓関数
float Hanning(uint i)
{
    return (SIZE % 2 == 0) ?
    //偶数
    0.5f - 0.5f * cos(2.0f * PI * i / SIZE) :
    //奇数
    0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / SIZE);
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    float hanning = Hanning(gID.x);

    float tmp = wave[gID.x] * hanning;

    float r = 0;
    float i = 0;
    for (int n = 0; n < SIZE; ++n)
    {
        r =  cos(2.0f * PI * gID.x * n / SIZE);
        i = -sin(2.0f * PI * gID.x * n / SIZE);

        real[gID.x] += r * tmp - i * 0.0f;
    }

    GroupMemoryBarrierWithGroupSync();
}