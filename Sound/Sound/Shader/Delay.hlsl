// ���[�g�V�O�l�`���̐錾
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

// �p�����[�^
cbuffer Param : register(b0)
{
    //������
    float attenuation;
    //�x������
    float time;
    //���[�v��
    uint waveIndex;
    int loop;
    //�T���v�����O���g��
    int sample;
};

// �K���O�f�[�^
RWStructuredBuffer<float> origin : register(u0);
// �K���f�[�^
RWStructuredBuffer<float> real : register(u1);

#define PI 3.14159265f

// �f�B�X�g�[�V����
void Distortion(uint i)
{
    //�g�`�̑���
    real[i] = origin[i] * 10.0f;
    if (real[i] >= 0.0f)
    {
        real[i] = atan(real[i]) / (PI / 2.0f);
    }
    else if (real[i] < -0.0f)
    {
        real[i] = atan(real[i]) / (PI / 2.0f) * 0.1f;
    }

    //���ʂ𒲐�
    real[i] *= 0.5f;
}

// ���~�b�^�[
void Limiter(uint i)
{
    //�������l
    float threshold = 0.2f;

    //�U���̏C��
    if (real[i] > threshold)
    {
        real[i] = threshold;

    }
    else if (real[i] < -threshold)
    {
        real[i] = -threshold;
    }
}

// �R���v���b�T
void Compressor(uint i)
{
    //�������l
    float threshold = 0.2f;
    //���V�I
    float ratio = 1.0f / 10.0f;
    //������
    float gain = 1.0f / (threshold + (1.0f - threshold) * ratio);

    //�U���̈��k
    if(real[i] > threshold)
    {
        real[i] = threshold + (real[i] - threshold) * ratio;

    }
    else if (real[i] < -threshold)
    {
        real[i] = -threshold + (real[i] + threshold) * ratio;
    }

    //�U���̑���
    real[i] *= gain;
}

// �n�j���O���֐�
float Hanning(uint i, uint size)
{
    return (size % 2 == 0) ?
    //����
    0.5f - 0.5f * cos(2.0f * PI * i / size) :
    //�
    0.5f - 0.5f * cos(2.0f * PI * (i + 0.5f) / size);
}

// �V���N�֐�
float Sinc(float i)
{
    return (i == 0.0f) ? 1.0f : sin(i) / i;
}

// FIR_LPF
void FIR_LPF(uint index)
{
    //�G�b�W���g��
    float edge = 1000.0f / sample;
    //�J�ڑш敝
    float delta = 1000.0f / sample;

    //�x����̐�
    int num = (int)(3.1f / delta + 0.5f) - 1;
    if(num % 2 != 0)
    {
        ++num;
    }

    float data[10];
    //int offset = num / 2;

    //for (int i = -offset; i <= offset; ++i)
    //{
    //    data[offset + i] = 2.0f * edge * Sinc(2.0f * PI * edge * i);
    //}

    //for (int n = 0; n < num + 1; ++n)
    //{
    //    data[n] *= Hanning(n, num + 1);
    //}

    //for (int m = 0; m <= num; ++m)
    //{
    //    real[index] += (index - m >= 0) ? data[m] * origin[index - m] : 0.0f;
    //}
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

    //�N���b�s���O
    if (real[gID.x] > 1.0f)
    {
        real[gID.x] = 1.0f;
    }

    AllMemoryBarrierWithGroupSync();

}