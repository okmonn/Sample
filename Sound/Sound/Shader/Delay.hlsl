// [gVOl`Ìé¾
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

// p[^
cbuffer Param : register(b0)
{
    //¸¦
    float attenuation;
    //xÔ
    float time;
    //[vñ
    int loop;
    //TvOüg
    int sample;
};

// KOf[^
RWStructuredBuffer<float> origin : register(u0);
// Kf[^
RWStructuredBuffer<float> real : register(u1);

#define PI 3.14159265f

// fBXg[V
void Distortion(uint i)
{
    //g`Ì
    real[i] = origin[i] * 10.0f;
    if (real[i] >= 0.0f)
    {
        real[i] = atan(real[i]) / (PI / 2.0f);
    }
    else if (real[i] < -0.0f)
    {
        real[i] = atan(real[i]) / (PI / 2.0f) * 0.1f;
    }

    //¹Êð²ß
    real[i] *= 0.5f;
}

// ~b^[
void Limiter(uint i)
{
    //µ«¢l
    float threshold = 0.2f;

    //UÌC³
    if (real[i] > threshold)
    {
        real[i] = threshold;

    }
    else if (real[i] < -threshold)
    {
        real[i] = -threshold;
    }
}

// RvbT
void Compressor(uint i)
{
    //µ«¢l
    float threshold = 0.2f;
    //VI
    float ratio = 1.0f / 10.0f;
    //¦
    float gain = 1.0f / (threshold + (1.0f - threshold) * ratio);

    //UÌ³k
    if(real[i] > threshold)
    {
        real[i] = threshold + (real[i] - threshold) * ratio;

    }
    else if (real[i] < -threshold)
    {
        real[i] = -threshold + (real[i] + threshold) * ratio;
    }

    //UÌ
    real[i] *= gain;
}

// njOÖ
float Hanning(uint i, uint size)
{
    return (size % 2 == 0) ?
    //ô
    0.5f - 0.5f * cos(2.0f * PI * i / size) :
    //ï
    0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / size);
}

// VNÖ
float Sinc(float i)
{
    return (i == 0.0f) ? 1.0f : sin(i) / i;
}

// FIRtB^ÌLPF
float FIR_LPF(float data[], float edge, int num)
{
    int offset = num / 2;

    for (int i = -offset; i <= offset; ++i)
    {
        data[offset + i] = 2.0f * edge * Sinc(2.0f * PI * edge * i);
    }

    for (int n = 0; n < num + 1; ++n)
    {
        data[n] *= Hanning(n, num + 1);
    }
}

// LPF
void LPF(uint i)
{
    //GbWüg
    float edge = 1000.0f / sample;
    //JÚÑæ
    float delta = 1000.0f / sample;

    //xíÌ
    int num = (int)(3.1f / delta + 0.5f) - 1;
    if(num % 2 != 0)
    {
        ++num;
    }

    float data[num + 1];
    FIR_LPF(data, edge, num);

    for (int n = 0; n <= num; ++n)
    {
        real[i] += (i - n >= 0) ? data[n] * origin[i - n] : 0.0f;
    }
}

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void CS(uint3 gID : SV_GroupID, uint3 gtID : SV_GroupThreadID, uint3 disID : SV_DispatchThreadID)
{
    for (int n = 1; n <= loop; ++n)
    {
        float m = disID.x - n * (sample * time);
        real[disID.x] += (m >= 0.0f) ? pow(attenuation, n) * origin[gID.x] : 0.0f;
    }

    //NbsO
    if (real[gID.x] > 1.0f)
    {
        real[gID.x] = 1.0f;
    }

    AllMemoryBarrierWithGroupSync();

}